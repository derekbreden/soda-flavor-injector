#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Adafruit_NeoPixel.h>
#include <lvgl.h>
#include <LittleFS.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "CST816D.h"
#include "font_ratio_64.h"

// Seed images (compiled in for first-boot only, then served from LittleFS)
#include "images/flavor0_240.h"
#include "images/flavor1_240.h"
#include "images/flavor2_240.h"

// ════════════════════════════════════════════════════════════
//  ESP32-S3 Config Display — Soda Flavor Injector
// ════════════════════════════════════════════════════════════
// Pin assignments for Meshnology ESP32-S3 1.28" Round Rotary Display

// ── Display SPI (GC9A01A, 240x240) ──
#define TFT_BLK  46
#define TFT_RST  14
#define TFT_CS    9
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_DC    3

// ── Rotary Encoder ──
#define ENCODER_CLK 45
#define ENCODER_DT  42
#define ENCODER_BTN 41

// ── Touch (CST816D) ──
#define TOUCH_SDA  6
#define TOUCH_SCL  7
#define TOUCH_INT  5
#define TOUCH_RST 13

// ── RGB LEDs (WS2812, 5 units) ──
#define LED_DATA  48
#define LED_COUNT  5

// ── Image constants ──
#define S3_SCREEN_W   240
#define S3_SCREEN_H   240
#define IMAGE_BYTES   (S3_SCREEN_W * S3_SCREEN_H * 2)  // 115200

#define META_PATH    "/meta.txt"
#define LABELS_PATH  "/labels.txt"
#define MAX_IMAGES   99
#define MAX_LABEL_LEN 32

static uint8_t numImages = 0;
static char labels[MAX_IMAGES][MAX_LABEL_LEN + 1];
static uint16_t imageBuf[S3_SCREEN_W * S3_SCREEN_H];  // ~115KB RAM buffer

// ── Hardware objects ──
Arduino_ESP32SPI *spi_bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED, FSPI, true);
Arduino_GC9A01 *hw_display = new Arduino_GC9A01(spi_bus, TFT_RST, 0, true);
Adafruit_NeoPixel leds(LED_COUNT, LED_DATA, NEO_GRB + NEO_KHZ800);
CST816D touch(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

// ── LVGL display buffer ──
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *lvgl_buf;

// ── BLE (built-in NimBLE via Arduino BLE API) ──
#define NUS_SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_RX_UUID             "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_TX_UUID             "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

static BLECharacteristic *pTxChar = nullptr;
static bool bleConnected = false;

// Forward declarations
static void bleSendLine(const char *line);
static bool loadImageFromFS(uint8_t slot);
static void imagePath(char *buf, uint8_t slot);

// ── BLE image download state ──
// Streams directly from LittleFS file — no imageBuf aliasing.
static bool bleImageSending = false;
static uint8_t bleImageSlot = 0;
static uint32_t bleImageSize = 0;
static File bleFile;
static uint8_t bleSendChunkBuf[240];

// ── BLE cross-task safety ──
// BLE RX callback runs on NimBLE task — must not do LittleFS I/O or Serial0
// writes directly. Instead, callback sets a request + buffers data, and loop()
// processes it on the main task.
enum BleRequest { BLE_REQ_NONE, BLE_REQ_LIST, BLE_REQ_GETPNG, BLE_REQ_GETIMG, BLE_REQ_FORWARD };
static volatile BleRequest bleRequest = BLE_REQ_NONE;
static volatile int bleRequestSlot = -1;
static char bleForwardBuf[64];  // buffered command to forward to ESP32

class BLEServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) override {
    bleConnected = true;
    Serial.println("BLE: client connected");
  }
  void onDisconnect(BLEServer *pServer) override {
    bleConnected = false;
    if (bleImageSending) {
      bleImageSending = false;
      if (bleFile) bleFile.close();
      Serial.println("BLE: aborted image send on disconnect");
    }
    Serial.println("BLE: client disconnected");
    pServer->startAdvertising();
  }
};

static void bleImageSendChunks() {
  if (!bleImageSending || !bleConnected || !pTxChar) return;

  if (!bleFile || !bleFile.available()) {
    if (bleFile) bleFile.close();
    delay(50);
    bleSendLine("IMGEND");
    bleImageSending = false;
    Serial.printf("BLE image %d send complete (%u bytes)\n", bleImageSlot, bleImageSize);
    return;
  }

  size_t n = bleFile.read(bleSendChunkBuf, sizeof(bleSendChunkBuf));
  if (n == 0) {
    bleFile.close();
    delay(50);
    bleSendLine("IMGEND");
    bleImageSending = false;
    return;
  }

  pTxChar->setValue(bleSendChunkBuf, n);
  pTxChar->notify();
  delay(20);
}

static bool openPngForBLE(uint8_t slot) {
  char path[24];
  snprintf(path, sizeof(path), "/s3_png%02d.png", slot);
  if (!LittleFS.exists(path)) {
    Serial.printf("loadPng: %s does not exist\n", path);
    return false;
  }
  bleFile = LittleFS.open(path, "r");
  if (!bleFile) {
    Serial.printf("loadPng: %s open failed\n", path);
    return false;
  }
  uint32_t size = bleFile.size();
  if (size == 0 || size > IMAGE_BYTES) {
    Serial.printf("loadPng: %s bad size %u\n", path, size);
    bleFile.close();
    return false;
  }
  bleImageSize = size;
  Serial.printf("Opened PNG slot %d for BLE: %u bytes\n", slot, size);
  return true;
}

class BLERxCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
    String val = pChar->getValue();
    if (val.length() == 0) return;

    // Drop if a previous request hasn't been processed yet
    if (bleRequest != BLE_REQ_NONE) return;

    if (val == "LIST") {
      bleRequest = BLE_REQ_LIST;
    } else if (val.startsWith("GETPNG:")) {
      bleRequestSlot = val.substring(7).toInt();
      bleRequest = BLE_REQ_GETPNG;
    } else if (val.startsWith("GETIMG:")) {
      bleRequestSlot = val.substring(7).toInt();
      bleRequest = BLE_REQ_GETIMG;
    } else {
      // Buffer the command for forwarding to ESP32 on main task
      strncpy(bleForwardBuf, val.c_str(), sizeof(bleForwardBuf) - 1);
      bleForwardBuf[sizeof(bleForwardBuf) - 1] = '\0';
      bleRequest = BLE_REQ_FORWARD;
    }
  }
};

// Process BLE requests on main task (safe for LittleFS I/O and Serial0)
static void processBleRequest() {
  BleRequest req = bleRequest;
  if (req == BLE_REQ_NONE) return;

  int slot = bleRequestSlot;
  Serial.printf("BLE RX: req=%d slot=%d\n", req, slot);

  switch (req) {
    case BLE_REQ_LIST:
      for (uint8_t i = 0; i < numImages; i++) {
        char buf[48];
        snprintf(buf, sizeof(buf), "IMG:%d:%s", i, labels[i]);
        bleSendLine(buf);
        delay(20);
      }
      bleSendLine("END");
      break;

    case BLE_REQ_GETPNG:
      if (slot < 0 || slot >= numImages) {
        bleSendLine("ERR:INVALID_SLOT");
      } else if (!openPngForBLE(slot)) {
        bleSendLine("ERR:LOAD_FAILED");
      } else {
        char hdr[32];
        snprintf(hdr, sizeof(hdr), "IMGSTART:%d:%u", slot, bleImageSize);
        bleSendLine(hdr);
        delay(20);
        bleImageSlot = slot;
        bleImageSending = true;
        Serial.printf("BLE PNG download: slot %d, %u bytes\n", slot, bleImageSize);
      }
      break;

    case BLE_REQ_GETIMG: {
      if (slot < 0 || slot >= numImages) {
        bleSendLine("ERR:INVALID_SLOT");
      } else {
        char path[16];
        imagePath(path, slot);
        bleFile = LittleFS.open(path, "r");
        if (!bleFile) {
          bleSendLine("ERR:LOAD_FAILED");
        } else {
          bleImageSize = IMAGE_BYTES;
          char hdr[32];
          snprintf(hdr, sizeof(hdr), "IMGSTART:%d:%d", slot, IMAGE_BYTES);
          bleSendLine(hdr);
          delay(20);
          bleImageSlot = slot;
          bleImageSending = true;
          Serial.printf("BLE image download: slot %d, %d bytes\n", slot, IMAGE_BYTES);
        }
      }
      break;
    }

    case BLE_REQ_FORWARD:
      Serial0.println(bleForwardBuf);
      Serial0.flush();
      break;

    default:
      break;
  }

  bleRequest = BLE_REQ_NONE;
}

static void initBLE() {
  BLEDevice::init("SodaMachine");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLEServerCB());

  BLEService *pService = pServer->createService(NUS_SERVICE_UUID);

  pTxChar = pService->createCharacteristic(
    NUS_TX_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pTxChar->addDescriptor(new BLE2902());

  BLECharacteristic *pRxChar = pService->createCharacteristic(
    NUS_RX_UUID,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  pRxChar->setCallbacks(new BLERxCB());

  pService->start();

  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(NUS_SERVICE_UUID);
  pAdv->setScanResponse(true);
  pAdv->start();

  Serial.println("BLE: NUS service started, advertising as 'SodaMachine'");
}

// ── Config state (synced from ESP32 via UART) ──
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;
uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;

// ── UART to ESP32 (Serial0 = UART0, J34 connector) ──
bool configSynced = false;
unsigned long lastGetConfig = 0;

// ── Menu ──
enum MenuItem { MENU_F1_IMAGE, MENU_F1_RATIO, MENU_F2_IMAGE, MENU_F2_RATIO, MENU_COUNT };
const char* menuLabels[] = { "Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio" };

int menuIndex = 0;
bool editing = false;

// ── Encoder state ──
int lastClk = HIGH;

// ── Touch state ──
unsigned long lastTapTime = 0;
bool lastTouchState = false;

// ── Circular image rendering ──
// Browse: 90px diameter, Edit: 128px diameter (matches external RP2040 display)
#define THUMB_BROWSE 90
#define THUMB_EDIT   128
#define THUMB_MAX    128
static lv_color_t thumb_buf[THUMB_MAX * THUMB_MAX];

// ════════════════════════════════════════════════════════════
//  CRC-16/CCITT (poly 0x1021, init 0xFFFF)
// ════════════════════════════════════════════════════════════

static uint16_t crc16(const uint8_t *data, size_t len) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= (uint16_t)data[i] << 8;
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
    }
  }
  return crc;
}

// ════════════════════════════════════════════════════════════
//  CRC-32 (standard, same as zlib)
// ════════════════════════════════════════════════════════════

static uint32_t crc32_update(uint32_t crc, const uint8_t *data, size_t len) {
  crc = ~crc;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
    }
  }
  return ~crc;
}

// ════════════════════════════════════════════════════════════
//  LittleFS image management
// ════════════════════════════════════════════════════════════

static void imagePath(char *buf, uint8_t slot) {
  snprintf(buf, 16, "/img%02d.bin", slot);
}

static void pngPath(char *buf, uint8_t slot) {
  snprintf(buf, 24, "/s3_png%02d.png", slot);
}

static uint8_t countImages() {
  uint8_t count = 0;
  char path[16];
  for (uint8_t i = 0; i < MAX_IMAGES; i++) {
    imagePath(path, i);
    if (!LittleFS.exists(path)) break;
    count = i + 1;
  }
  return count;
}

static void updateMeta() {
  numImages = countImages();
  File f = LittleFS.open(META_PATH, "w");
  if (f) {
    f.printf("num_images=%d\n", numImages);
    f.close();
  }
}

static void loadLabels() {
  memset(labels, 0, sizeof(labels));
  if (!LittleFS.exists(LABELS_PATH)) return;
  File f = LittleFS.open(LABELS_PATH, "r");
  if (!f) return;
  for (uint8_t i = 0; i < MAX_IMAGES && f.available(); i++) {
    String line = f.readStringUntil('\n');
    line.trim();
    strncpy(labels[i], line.c_str(), MAX_LABEL_LEN);
    labels[i][MAX_LABEL_LEN] = '\0';
  }
  f.close();
}

static void saveLabels() {
  File f = LittleFS.open(LABELS_PATH, "w");
  if (!f) return;
  for (uint8_t i = 0; i < numImages; i++) {
    f.println(labels[i]);
  }
  f.close();
}

static bool loadImageFromFS(uint8_t slot) {
  char path[16];
  imagePath(path, slot);
  if (!LittleFS.exists(path)) return false;
  File f = LittleFS.open(path, "r");
  if (!f) return false;
  size_t n = f.read((uint8_t *)imageBuf, IMAGE_BYTES);
  f.close();
  return (n == IMAGE_BYTES);
}

// ════════════════════════════════════════════════════════════
//  First-boot seeding: write PROGMEM images to LittleFS
// ════════════════════════════════════════════════════════════

static const uint16_t *seedBitmaps[] = {
  flavor0_240,   // 0 = Diet Wild Cherry Pepsi
  flavor1_240,   // 1 = Diet Mountain Dew
  flavor2_240,   // 2 = Diet Coke
};
static const char *seedLabelsArr[] = {
  "diet_wild_cherry_pepsi",
  "diet_mtn_dew",
  "diet_coke",
};

static void seedDefaultImages() {
  if (LittleFS.exists(META_PATH)) return;  // already seeded

  Serial.println("First boot: seeding default images to LittleFS...");
  for (uint8_t i = 0; i < 3; i++) {
    char path[16];
    imagePath(path, i);
    File f = LittleFS.open(path, "w");
    if (f) {
      // On ESP32-S3, PROGMEM is memory-mapped flash (same as regular const)
      f.write((const uint8_t *)seedBitmaps[i], IMAGE_BYTES);
      f.close();
      Serial.printf("  Seeded %s (%d bytes)\n", path, IMAGE_BYTES);
    }
  }
  // Set default labels
  for (uint8_t i = 0; i < 3; i++) {
    strncpy(labels[i], seedLabelsArr[i], MAX_LABEL_LEN);
    labels[i][MAX_LABEL_LEN] = '\0';
  }

  updateMeta();
  saveLabels();
  Serial.println("Seeding complete.");
}

// ════════════════════════════════════════════════════════════
//  Circular image rendering (from LittleFS via imageBuf)
// ════════════════════════════════════════════════════════════

void renderCircularThumb(uint8_t imgIdx, int size) {
  if (!loadImageFromFS(imgIdx)) {
    // Fill with dark gray if load fails
    for (int i = 0; i < size * size; i++) thumb_buf[i].full = 0x2104;
    return;
  }
  const uint16_t *src = imageBuf;
  int radius = size / 2;
  int r2 = radius * radius;
  for (int y = 0; y < size; y++) {
    int dy = y - radius;
    int srcY = y * S3_SCREEN_W / size;
    for (int x = 0; x < size; x++) {
      int dx = x - radius;
      if (dx * dx + dy * dy <= r2) {
        int srcX = x * S3_SCREEN_W / size;
        thumb_buf[y * size + x].full = src[srcY * S3_SCREEN_W + srcX];
      } else {
        thumb_buf[y * size + x].full = 0;
      }
    }
  }
}

// ── LVGL flush callback ──
void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;
  hw_display->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);
}

// ── Helpers ──
uint8_t getCurrentImage() {
  return (menuIndex == MENU_F1_IMAGE) ? flavor1Image : flavor2Image;
}

uint8_t getCurrentRatio() {
  return (menuIndex == MENU_F1_RATIO) ? flavor1Ratio : flavor2Ratio;
}

bool isImageItem() {
  return (menuIndex == MENU_F1_IMAGE || menuIndex == MENU_F2_IMAGE);
}

// Forward declaration (needed by UART handler)
void drawScreen();

// ════════════════════════════════════════════════════════════
//  Binary protocol constants
// ════════════════════════════════════════════════════════════

#define STX 0x02

// Commands (ESP32 -> S3)
#define CMD_UPLOAD_START 0x01
#define CMD_CHUNK_DATA   0x02
#define CMD_UPLOAD_DONE  0x03
#define CMD_QUERY_COUNT  0x04
#define CMD_DELETE_IMAGE 0x05
#define CMD_SWAP_IMAGES  0x06

// Responses (S3 -> ESP32)
#define RESP_READY          0x10
#define RESP_CHUNK_OK       0x11
#define RESP_UPLOAD_OK      0x12
#define RESP_DELETE_OK      0x13
#define RESP_COUNT          0x14
#define RESP_SWAP_OK        0x15
#define ERR_SLOT_INVALID    0xE1
#define ERR_NO_SPACE        0xE2
#define ERR_BUSY            0xE3
#define ERR_CRC             0xE4
#define ERR_SEQ             0xE5
#define ERR_WRITE           0xE6
#define ERR_SIZE_MISMATCH   0xE7
#define ERR_CRC32_MISMATCH  0xE8

#define MAX_CHUNK_SIZE 128

// ════════════════════════════════════════════════════════════
//  Upload state machine
// ════════════════════════════════════════════════════════════

enum UploadState { UPLOAD_IDLE, UPLOAD_RECEIVING };

static struct {
  UploadState state = UPLOAD_IDLE;
  uint8_t     slot;
  uint32_t    expectedSize;
  uint32_t    receivedBytes;
  uint8_t     nextSeq;
  uint32_t    runningCrc32;
  unsigned long lastChunkTime;
  File        file;
} upload;

static void sendBinaryResponse(uint8_t code, uint8_t extra) {
  uint8_t resp[6];
  resp[0] = STX;
  resp[1] = STX;
  resp[2] = code;
  resp[3] = extra;
  uint16_t crc = crc16(resp, 4);
  resp[4] = crc & 0xFF;
  resp[5] = (crc >> 8) & 0xFF;
  Serial0.write(resp, 6);
  Serial0.flush();
}

static void abortUpload() {
  if (upload.file) {
    upload.file.close();
  }
  LittleFS.remove("/tmp.bin");
  upload.state = UPLOAD_IDLE;
  Serial.println("Upload aborted");
}

static void handleUploadStart(const uint8_t *msg) {
  uint8_t slot = msg[3];
  uint32_t size = msg[4] | (msg[5] << 8) | (msg[6] << 16) | (msg[7] << 24);

  if (upload.state == UPLOAD_RECEIVING) {
    abortUpload();
  }

  bool isPng = (slot >= 100);
  uint8_t realSlot = isPng ? (slot - 100) : slot;

  if (isPng) {
    // PNG upload: any existing image slot is valid, size varies
    if (realSlot >= numImages) {
      sendBinaryResponse(ERR_SLOT_INVALID, 0);
      return;
    }
    if (size > IMAGE_BYTES || size == 0) {
      sendBinaryResponse(ERR_SIZE_MISMATCH, 0);
      return;
    }
  } else {
    // RGB565 upload: fixed size, sequential slots
    if (slot > MAX_IMAGES) {
      sendBinaryResponse(ERR_SLOT_INVALID, 0);
      return;
    }
    if (size != IMAGE_BYTES) {
      sendBinaryResponse(ERR_SIZE_MISMATCH, 0);
      return;
    }
    if (slot > numImages) {
      sendBinaryResponse(ERR_SLOT_INVALID, 0);
      return;
    }
  }

  LittleFS.remove("/tmp.bin");
  upload.file = LittleFS.open("/tmp.bin", "w");
  if (!upload.file) {
    sendBinaryResponse(ERR_NO_SPACE, 0);
    return;
  }

  upload.state = UPLOAD_RECEIVING;
  upload.slot = slot;
  upload.expectedSize = size;
  upload.receivedBytes = 0;
  upload.nextSeq = 0;
  upload.runningCrc32 = 0;
  upload.lastChunkTime = millis();

  Serial.printf("Upload started: slot %d, %lu bytes\n", slot, size);
  sendBinaryResponse(RESP_READY, 0);
}

static void handleChunkData(const uint8_t *msg, size_t msgLen) {
  if (upload.state != UPLOAD_RECEIVING) {
    sendBinaryResponse(ERR_BUSY, 0);
    return;
  }

  uint8_t seq = msg[3];
  uint16_t payloadLen = msg[4] | (msg[5] << 8);

  if (seq != upload.nextSeq) {
    sendBinaryResponse(ERR_SEQ, upload.nextSeq);
    return;
  }

  if (payloadLen == 0 || payloadLen > MAX_CHUNK_SIZE) {
    sendBinaryResponse(ERR_CRC, 0);
    return;
  }

  const uint8_t *payload = msg + 6;

  // Write to file
  size_t written = upload.file.write(payload, payloadLen);
  if (written != payloadLen) {
    sendBinaryResponse(ERR_WRITE, 0);
    abortUpload();
    return;
  }

  upload.receivedBytes += payloadLen;
  upload.runningCrc32 = crc32_update(upload.runningCrc32, payload, payloadLen);
  upload.nextSeq = (upload.nextSeq + 1) & 0xFF;
  upload.lastChunkTime = millis();

  sendBinaryResponse(RESP_CHUNK_OK, upload.nextSeq);
}

static void handleUploadDone(const uint8_t *msg) {
  if (upload.state != UPLOAD_RECEIVING) {
    sendBinaryResponse(ERR_BUSY, 0);
    return;
  }

  uint8_t slot = msg[3];
  uint32_t expectedCrc32 = msg[4] | (msg[5] << 8) | (msg[6] << 16) | (msg[7] << 24);

  upload.file.close();

  if (upload.receivedBytes != upload.expectedSize) {
    Serial.printf("Size mismatch: got %lu, expected %lu\n",
                  upload.receivedBytes, upload.expectedSize);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    sendBinaryResponse(ERR_SIZE_MISMATCH, 0);
    return;
  }

  if (upload.runningCrc32 != expectedCrc32) {
    Serial.printf("CRC32 mismatch: got 0x%08lX, expected 0x%08lX\n",
                  upload.runningCrc32, expectedCrc32);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    sendBinaryResponse(ERR_CRC32_MISMATCH, 0);
    return;
  }

  // Move temp file to final slot
  bool isPng = (slot >= 100);
  uint8_t realSlot = isPng ? (slot - 100) : slot;

  if (isPng) {
    char path[24];
    pngPath(path, realSlot);
    LittleFS.remove(path);
    bool renameOk = LittleFS.rename("/tmp.bin", path);
    upload.state = UPLOAD_IDLE;
    Serial.printf("PNG upload complete: slot %d (%lu bytes) rename=%s\n",
                  realSlot, upload.receivedBytes, renameOk ? "ok" : "FAIL");
    // Verify file exists after rename
    if (LittleFS.exists(path)) {
      File verify = LittleFS.open(path, "r");
      if (verify) {
        Serial.printf("PNG verify: %s exists, %u bytes\n", path, verify.size());
        verify.close();
      }
    } else {
      Serial.printf("PNG verify: %s MISSING after rename!\n", path);
    }
    sendBinaryResponse(RESP_UPLOAD_OK, numImages);
  } else {
    char path[16];
    imagePath(path, slot);
    LittleFS.remove(path);
    LittleFS.rename("/tmp.bin", path);
    upload.state = UPLOAD_IDLE;
    updateMeta();
    Serial.printf("Upload complete: slot %d, %d images total\n", slot, numImages);
    sendBinaryResponse(RESP_UPLOAD_OK, numImages);
  }
}

static void handleQueryCount() {
  sendBinaryResponse(RESP_COUNT, numImages);
}

static void handleDeleteImage(const uint8_t *msg) {
  uint8_t slot = msg[3];

  if (slot >= numImages || numImages <= 1) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
  }

  // Delete the RGB565 file and PNG file
  char path[16];
  imagePath(path, slot);
  LittleFS.remove(path);
  char pPath[24];
  pngPath(pPath, slot);
  LittleFS.remove(pPath);

  // Shift all files above this slot down by 1
  char pathFrom[16], pathTo[16];
  char pPathFrom[24], pPathTo[24];
  for (uint8_t i = slot + 1; i < numImages; i++) {
    imagePath(pathFrom, i);
    imagePath(pathTo, i - 1);
    LittleFS.rename(pathFrom, pathTo);
    pngPath(pPathFrom, i);
    pngPath(pPathTo, i - 1);
    if (LittleFS.exists(pPathFrom)) {
      LittleFS.rename(pPathFrom, pPathTo);
    }
  }

  // Shift labels down
  for (uint8_t i = slot; i + 1 < numImages; i++) {
    strncpy(labels[i], labels[i + 1], MAX_LABEL_LEN);
  }
  if (numImages > 0) labels[numImages - 1][0] = '\0';

  updateMeta();
  saveLabels();

  // Adjust local image references
  if (flavor1Image == slot) flavor1Image = 0;
  else if (flavor1Image > slot) flavor1Image--;
  if (flavor2Image == slot) flavor2Image = 0;
  else if (flavor2Image > slot) flavor2Image--;
  if (flavor1Image >= numImages) flavor1Image = 0;
  if (flavor2Image >= numImages) flavor2Image = 0;

  Serial.printf("Deleted slot %d, %d images remain\n", slot, numImages);
  sendBinaryResponse(RESP_DELETE_OK, numImages);
}

static void handleSwapImages(const uint8_t *msg) {
  uint8_t slotA = msg[3];
  uint8_t slotB = msg[4];

  if (slotA >= numImages || slotB >= numImages) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
  }

  if (slotA == slotB) {
    sendBinaryResponse(RESP_SWAP_OK, numImages);
    return;
  }

  // Swap RGB565 via temp file
  char pathA[16], pathB[16];
  imagePath(pathA, slotA);
  imagePath(pathB, slotB);
  LittleFS.rename(pathA, "/swap.bin");
  LittleFS.rename(pathB, pathA);
  LittleFS.rename("/swap.bin", pathB);

  // Swap PNGs via temp file
  char pPathA[24], pPathB[24];
  pngPath(pPathA, slotA);
  pngPath(pPathB, slotB);
  bool hasPngA = LittleFS.exists(pPathA);
  bool hasPngB = LittleFS.exists(pPathB);
  if (hasPngA && hasPngB) {
    LittleFS.rename(pPathA, "/swap.png");
    LittleFS.rename(pPathB, pPathA);
    LittleFS.rename("/swap.png", pPathB);
  } else if (hasPngA) {
    LittleFS.rename(pPathA, pPathB);
  } else if (hasPngB) {
    LittleFS.rename(pPathB, pPathA);
  }

  // Swap labels
  char tmpLabel[MAX_LABEL_LEN + 1];
  strncpy(tmpLabel, labels[slotA], MAX_LABEL_LEN + 1);
  strncpy(labels[slotA], labels[slotB], MAX_LABEL_LEN + 1);
  strncpy(labels[slotB], tmpLabel, MAX_LABEL_LEN + 1);
  saveLabels();

  // Adjust local image references
  if (flavor1Image == slotA) flavor1Image = slotB;
  else if (flavor1Image == slotB) flavor1Image = slotA;
  if (flavor2Image == slotA) flavor2Image = slotB;
  else if (flavor2Image == slotB) flavor2Image = slotA;

  Serial.printf("Swapped slots %d <-> %d\n", slotA, slotB);
  sendBinaryResponse(RESP_SWAP_OK, numImages);
}

// ════════════════════════════════════════════════════════════
//  UART byte-level parser (text + binary multiplexed)
// ════════════════════════════════════════════════════════════

#define TEXT_BUF_SIZE 128
#define BIN_BUF_SIZE 142  // max: 6 header + 128 payload + 2 CRC + margin

static char textBuf[TEXT_BUF_SIZE];
static uint8_t textPos = 0;

static uint8_t binBuf[BIN_BUF_SIZE];
static uint8_t binPos = 0;
static bool inBinary = false;

// Forward declaration
static void processTextLine(const char *line);

static bool tryParseBinaryMessage() {
  if (binPos < 3) return false;

  uint8_t cmd = binBuf[2];
  int expectedLen = -1;

  switch (cmd) {
    case CMD_UPLOAD_START:
      expectedLen = 10;
      break;
    case CMD_CHUNK_DATA:
      if (binPos < 6) return false;
      {
        uint16_t payloadLen = binBuf[4] | (binBuf[5] << 8);
        if (payloadLen == 0 || payloadLen > MAX_CHUNK_SIZE) {
          return true;  // invalid, discard
        }
        expectedLen = 6 + payloadLen + 2;
      }
      break;
    case CMD_UPLOAD_DONE:
      expectedLen = 10;
      break;
    case CMD_QUERY_COUNT:
      expectedLen = 6;
      break;
    case CMD_DELETE_IMAGE:
      expectedLen = 6;
      break;
    case CMD_SWAP_IMAGES:
      expectedLen = 7;
      break;
    default:
      return true;  // unknown, discard
  }

  if (binPos < expectedLen) return false;

  // Validate CRC-16
  uint16_t rxCrc = binBuf[expectedLen - 2] | (binBuf[expectedLen - 1] << 8);
  uint16_t calcCrc = crc16(binBuf, expectedLen - 2);
  if (rxCrc != calcCrc) {
    sendBinaryResponse(ERR_CRC, 0);
    return true;
  }

  // Dispatch
  switch (cmd) {
    case CMD_UPLOAD_START: handleUploadStart(binBuf); break;
    case CMD_CHUNK_DATA:   handleChunkData(binBuf, expectedLen); break;
    case CMD_UPLOAD_DONE:  handleUploadDone(binBuf); break;
    case CMD_QUERY_COUNT:  handleQueryCount(); break;
    case CMD_DELETE_IMAGE: handleDeleteImage(binBuf); break;
    case CMD_SWAP_IMAGES:  handleSwapImages(binBuf); break;
  }
  return true;
}

// ── Text line processing (CONFIG: responses, OK:/ERR:, LIST, LABEL:) ──

void parseConfigResponse(const char* line) {
  if (strncmp(line, "CONFIG:", 7) != 0) return;

  const char* p = line + 7;
  int f1r = 0, f2r = 0, f1i = 0, f2i = 0;

  if (sscanf(p, "F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d",
             &f1r, &f2r, &f1i, &f2i) == 4) {
    flavor1Ratio = constrain(f1r, 6, 24);
    flavor2Ratio = constrain(f2r, 6, 24);
    flavor1Image = constrain(f1i, 0, (int)numImages - 1);
    flavor2Image = constrain(f2i, 0, (int)numImages - 1);
    configSynced = true;
    Serial.printf("Config synced: F1_RATIO=%d F2_RATIO=%d F1_IMAGE=%d F2_IMAGE=%d\n",
                  flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image);
    drawScreen();
  }
}

// Forward a text line to BLE client (if connected)
static void bleSendLine(const char *line) {
  if (bleConnected && pTxChar) {
    pTxChar->setValue((uint8_t *)line, strlen(line));
    pTxChar->notify();
  }
}

static void processTextLine(const char *line) {
  Serial.printf("UART RX: %s\n", line);

  if (strncmp(line, "CONFIG:", 7) == 0) {
    parseConfigResponse(line);
    bleSendLine(line);
  } else if (strncmp(line, "OK:", 3) == 0) {
    Serial.printf("ESP32 confirmed: %s\n", line);
    bleSendLine(line);
  } else if (strncmp(line, "ERR:", 4) == 0) {
    Serial.printf("ESP32 error: %s\n", line);
    bleSendLine(line);
  } else if (strncmp(line, "LABEL:", 6) == 0) {
    // LABEL:slot:name — set label for a slot
    int slot = atoi(line + 6);
    const char *colon = strchr(line + 6, ':');
    if (colon && slot >= 0 && slot < numImages) {
      strncpy(labels[slot], colon + 1, MAX_LABEL_LEN);
      labels[slot][MAX_LABEL_LEN] = '\0';
      saveLabels();
      Serial.printf("Label set: slot %d = %s\n", slot, labels[slot]);
    }
  } else if (strncmp(line, "IMG:", 4) == 0 || strcmp(line, "END") == 0) {
    // Forward image list responses to BLE
    bleSendLine(line);
  } else if (strcmp(line, "LIST") == 0) {
    // Return all slot labels as text
    for (uint8_t i = 0; i < numImages; i++) {
      Serial0.printf("IMG:%d:%s\n", i, labels[i]);
    }
    Serial0.println("END");
    Serial0.flush();
  } else if (strcmp(line, "LISTPNGS") == 0) {
    // Diagnostic: list all PNG files on S3
    Serial0.printf("PNGS:%d images\n", numImages);
    for (uint8_t i = 0; i < numImages; i++) {
      char path[24];
      pngPath(path, i);
      if (LittleFS.exists(path)) {
        File f = LittleFS.open(path, "r");
        if (f) {
          Serial0.printf("PNG:%d:%u:%s\n", i, f.size(), path);
          f.close();
        } else {
          Serial0.printf("PNG:%d:OPEN_FAIL:%s\n", i, path);
        }
      } else {
        Serial0.printf("PNG:%d:MISSING:%s\n", i, path);
      }
    }
    Serial0.println("END");
    Serial0.flush();
  }
}

static void checkUart() {
  while (Serial0.available()) {
    uint8_t b = Serial0.read();

    if (inBinary) {
      if (binPos < BIN_BUF_SIZE) {
        binBuf[binPos++] = b;
      }
      if (tryParseBinaryMessage()) {
        inBinary = false;
        binPos = 0;
      } else if (binPos >= BIN_BUF_SIZE) {
        inBinary = false;
        binPos = 0;
      }
      continue;
    }

    // Check for binary sentinel: two consecutive 0x02 bytes
    if (b == STX && textPos == 1 && textBuf[0] == STX) {
      inBinary = true;
      binBuf[0] = STX;
      binBuf[1] = STX;
      binPos = 2;
      textPos = 0;
      continue;
    }

    // Regular text accumulation
    if (b == '\n' || b == '\r') {
      if (textPos > 0) {
        textBuf[textPos] = '\0';
        processTextLine(textBuf);
        textPos = 0;
      }
    } else if (textPos < TEXT_BUF_SIZE - 1) {
      textBuf[textPos++] = b;
    } else {
      textPos = 0;  // overflow, discard
    }
  }

  // Check upload timeout
  if (upload.state == UPLOAD_RECEIVING) {
    if (millis() - upload.lastChunkTime > 3000) {
      abortUpload();
    }
  }
}

// ── UART send helpers ──

void sendSetCommand(const char* key, int value) {
  Serial0.printf("SET:%s=%d\n", key, value);
  Serial.printf("UART TX: SET:%s=%d\n", key, value);
}

void sendSave() {
  Serial0.println("SAVE");
  Serial.println("UART TX: SAVE");
}

void sendCurrentValue() {
  switch (menuIndex) {
    case MENU_F1_IMAGE:
      sendSetCommand("F1_IMAGE", flavor1Image);
      break;
    case MENU_F1_RATIO:
      sendSetCommand("F1_RATIO", flavor1Ratio);
      break;
    case MENU_F2_IMAGE:
      sendSetCommand("F2_IMAGE", flavor2Image);
      break;
    case MENU_F2_RATIO:
      sendSetCommand("F2_RATIO", flavor2Ratio);
      break;
  }
  sendSave();
}

// ════════════════════════════════════════════════════════════
//  UI drawing
// ════════════════════════════════════════════════════════════

void drawNavDots() {
  int dotSpacing = 16;
  int totalWidth = (MENU_COUNT - 1) * dotSpacing;
  int startX = (240 - totalWidth) / 2;
  for (int i = 0; i < MENU_COUNT; i++) {
    lv_obj_t *dot = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, 6, 6);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(dot,
      (i == menuIndex) ? lv_color_white() : lv_color_hex(0x303030), 0);
    lv_obj_set_pos(dot, startX + i * dotSpacing - 3, 207);
  }
}

void drawBrowse() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

  // Title
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0x808080), 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

  if (isImageItem()) {
    renderCircularThumb(getCurrentImage(), THUMB_BROWSE);
    lv_obj_t *canvas = lv_canvas_create(scr);
    lv_canvas_set_buffer(canvas, thumb_buf, THUMB_BROWSE, THUMB_BROWSE, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 10);
  } else {
    char buf[8];
    snprintf(buf, sizeof(buf), "1:%d", getCurrentRatio());
    lv_obj_t *ratio = lv_label_create(scr);
    lv_label_set_text(ratio, buf);
    lv_obj_set_style_text_font(ratio, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(ratio, lv_color_hex(0x808080), 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }

  drawNavDots();
}

void drawEdit() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

  // Title — always white when editing
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_color(title, lv_color_white(), 0);

  if (isImageItem()) {
    renderCircularThumb(getCurrentImage(), THUMB_EDIT);
    lv_obj_t *canvas = lv_canvas_create(scr);
    lv_canvas_set_buffer(canvas, thumb_buf, THUMB_EDIT, THUMB_EDIT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 8);

    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 28);
  } else {
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);

    char buf[8];
    snprintf(buf, sizeof(buf), "1:%d", getCurrentRatio());
    lv_obj_t *ratio = lv_label_create(scr);
    lv_label_set_text(ratio, buf);
    lv_obj_set_style_text_font(ratio, &font_ratio_64, 0);
    lv_obj_set_style_text_color(ratio, lv_color_white(), 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }
}

void drawScreen() {
  if (editing) {
    drawEdit();
  } else {
    drawBrowse();
  }
  lv_refr_now(NULL);
}

// ════════════════════════════════════════════════════════════
//  Input reading
// ════════════════════════════════════════════════════════════

int readEncoder() {
  int clk = digitalRead(ENCODER_CLK);
  int direction = 0;
  if (clk != lastClk && clk == LOW) {
    int dt = digitalRead(ENCODER_DT);
    direction = (dt != clk) ? 1 : -1;
  }
  lastClk = clk;
  return direction;
}

bool readTap() {
  uint16_t x, y;
  uint8_t gesture;
  bool touching = touch.getTouch(&x, &y, &gesture);
  bool tapped = (touching && !lastTouchState && millis() - lastTapTime > 300);
  if (tapped) lastTapTime = millis();
  lastTouchState = touching;
  return tapped;
}

// ════════════════════════════════════════════════════════════
//  Menu logic
// ════════════════════════════════════════════════════════════

void handleNavigation(int dir) {
  if (dir == 0) return;

  if (editing) {
    switch (menuIndex) {
      case MENU_F1_IMAGE:
        flavor1Image = (flavor1Image + dir + numImages) % numImages;
        break;
      case MENU_F1_RATIO:
        flavor1Ratio = constrain(flavor1Ratio + dir, 6, 24);
        break;
      case MENU_F2_IMAGE:
        flavor2Image = (flavor2Image + dir + numImages) % numImages;
        break;
      case MENU_F2_RATIO:
        flavor2Ratio = constrain(flavor2Ratio + dir, 6, 24);
        break;
    }
    drawScreen();
  } else {
    menuIndex = (menuIndex + dir + MENU_COUNT) % MENU_COUNT;
    drawScreen();
  }
}

void handleTap() {
  if (editing) {
    editing = false;
    Serial.printf("Confirmed: %s\n", menuLabels[menuIndex]);
    sendCurrentValue();
  } else {
    editing = true;
    Serial.printf("Editing: %s\n", menuLabels[menuIndex]);
  }
  drawScreen();
}

// ════════════════════════════════════════════════════════════
//  Arduino setup/loop
// ════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  Serial0.begin(38400, SERIAL_8N1, 44, 43);  // UART0 on J34 connector (RX=44, TX=43)
  delay(500);
  Serial.println("ESP32-S3 Config Display starting...");

  // Init LittleFS
  if (!LittleFS.begin(true)) {  // true = format on fail
    Serial.println("LittleFS mount failed!");
  }
  seedDefaultImages();
  numImages = countImages();
  loadLabels();
  Serial.printf("Found %d images in LittleFS\n", numImages);
  for (uint8_t i = 0; i < numImages; i++) {
    Serial.printf("  Slot %d: %s\n", i, labels[i][0] ? labels[i] : "(unlabeled)");
  }
  // List PNG files for diagnostics
  for (uint8_t i = 0; i < numImages; i++) {
    char path[24];
    pngPath(path, i);
    if (LittleFS.exists(path)) {
      File f = LittleFS.open(path, "r");
      if (f) {
        Serial.printf("  PNG %d: %s (%u bytes)\n", i, path, f.size());
        f.close();
      }
    } else {
      Serial.printf("  PNG %d: %s MISSING\n", i, path);
    }
  }

  // RGB LEDs (unused, turned off)
  leds.begin();
  leds.clear();
  leds.show();

  // Power pins (required by board hardware)
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Backlight
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);

  // Encoder
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);

  // Touch
  touch.begin();

  // Display hardware init
  hw_display->begin();

  // LVGL init
  lv_init();

  // Half-screen draw buffer (57KB — leaves room for BLE heap)
  lvgl_buf = (lv_color_t *)malloc(240 * 120 * sizeof(lv_color_t));
  if (!lvgl_buf) {
    Serial.println("FATAL: failed to allocate LVGL buffer!");
    while (1) delay(1000);
  }
  lv_disp_draw_buf_init(&draw_buf, lvgl_buf, NULL, 240 * 120);

  // Register display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = lvgl_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  lastClk = digitalRead(ENCODER_CLK);

  drawScreen();

  // Init BLE (after display so UI is visible during BLE init)
  initBLE();

  Serial.println("Ready. Rotate to navigate, tap to edit/confirm.");
}

void loop() {
  // Boot sync: request config from ESP32 every 500ms until synced
  // Suppress during binary upload — text on Serial0 corrupts protocol responses
  if (!configSynced && upload.state != UPLOAD_RECEIVING && millis() - lastGetConfig > 500) {
    Serial0.println("GET_CONFIG");
    Serial.println("UART TX: GET_CONFIG (boot sync)");
    lastGetConfig = millis();
  }

  // Check for incoming UART data (text + binary)
  checkUart();

  // Process BLE requests on main task (safe for LittleFS + Serial0)
  processBleRequest();

  // BLE image streaming (non-blocking, sends a few chunks per loop)
  bleImageSendChunks();

  int dir = readEncoder();
  handleNavigation(dir);

  if (readTap()) {
    handleTap();
  }

  lv_timer_handler();
  delay(5);
}
