#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Adafruit_NeoPixel.h>
#include <lvgl.h>
#include <LittleFS.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <uart_st.h>
#include "CST816D.h"
#include "font_ratio_64.h"
#include "fw_version.h"

// Seed images (compiled in for first-boot only, then served from LittleFS)
#include "images/flavor0_240.h"
#include "images/flavor1_240.h"
#include "images/flavor2_240.h"

// ════════════════════════════════════════════════════════════
//  ESP32-S3 Config Display — Soda Flavor Injector
// ════════════════════════════════════════════════════════════

// ── Theme colors (match iOS app / app icon) ──
#define THEME_BG           lv_color_hex(0x1a1a2e)
#define THEME_BG_RGB565    0x18C5
#define THEME_TEXT_PRIMARY  lv_color_white()
#define THEME_TEXT_SECONDARY lv_color_hex(0x999999)
#define THEME_TEXT_INACTIVE lv_color_hex(0x505058)
#define THEME_DOT_ACTIVE   lv_color_white()
#define THEME_DOT_INACTIVE lv_color_hex(0x3a3a46)
#define THEME_ALERT        lv_color_hex(0xFF4444)
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

#define META_PATH     "/meta.txt"
#define LABELS_PATH   "/labels.txt"
#define MAX_IMAGES    99
#define MAX_LABEL_LEN 32
#define MAX_CHUNK_SIZE 128

static uint8_t numImages = 0;
static char labels[MAX_IMAGES][MAX_LABEL_LEN + 1];
static uint16_t *imageBuf = nullptr;  // allocated in PSRAM at setup()

// ── Config state (synced from ESP32 via UART) ──
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;
uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;
bool configSynced = false;
unsigned long lastGetConfig = 0;

// ── Hardware objects ──
Arduino_ESP32SPI *spi_bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED, FSPI, true);
Arduino_GC9A01 *hw_display = new Arduino_GC9A01(spi_bus, TFT_RST, 0, true);
Adafruit_NeoPixel leds(LED_COUNT, LED_DATA, NEO_GRB + NEO_KHZ800);
CST816D touch(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

// ── LVGL display buffer ──
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *lvgl_buf;

// ── UART to ESP32 (Serial0 = UART0, J34 connector) ──
SerialTransfer stLink;  // SerialTransfer on Serial0 (ESP32 link)

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
static void updateMeta();

// ── BLE image download state ──
// Streams directly from LittleFS file — no imageBuf aliasing.
static bool bleImageSending = false;
static uint8_t bleImageSlot = 0;
static uint32_t bleImageSize = 0;
static File bleFile;
static uint8_t bleSendChunkBuf[180];  // must fit within negotiated ATT MTU - 3

// ── BLE cross-task safety ──
// BLE RX callback runs on NimBLE task — must not do LittleFS I/O or Serial0
// writes directly. Instead, callback sets a request + buffers data, and loop()
// processes it on the main task.
enum BleRequest { BLE_REQ_NONE, BLE_REQ_LIST, BLE_REQ_GETPNG, BLE_REQ_GETIMG, BLE_REQ_FORWARD,
                  BLE_REQ_UPLOAD_START, BLE_REQ_GET_CONFIG, BLE_REQ_GET_VERSION };
static volatile BleRequest bleRequest = BLE_REQ_NONE;
static volatile int bleRequestSlot = -1;
static char bleForwardBuf[64];  // buffered command to forward to ESP32

// ── BLE upload state (phone → S3, accumulated in imageBuf) ──
enum BleUploadPhase { BLE_UP_IDLE, BLE_UP_WAIT_DATA, BLE_UP_RECEIVED, BLE_UP_FORWARDING };
static struct {
  volatile BleUploadPhase phase = BLE_UP_IDLE;
  uint8_t  slot;
  uint8_t  fileType;      // 0=png, 1=s3_rgb, 2=rp_rgb
  uint32_t expectedSize;
  uint32_t expectedCrc32;
  volatile uint32_t receivedBytes;
  char     label[MAX_LABEL_LEN + 1];
  bool     hasLabel;
  unsigned long lastDataTime;
} bleUpload;

class BLEServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) override {
    bleConnected = true;
    Serial.printf("BLE: client connected (heap=%lu)\n", (unsigned long)ESP.getFreeHeap());
  }
  void onDisconnect(BLEServer *pServer) override {
    bleConnected = false;
    if (bleImageSending) {
      bleImageSending = false;
      if (bleFile) bleFile.close();
      Serial.println("BLE: aborted image send on disconnect");
    }
    if (bleUpload.phase != BLE_UP_IDLE) {
      bleUpload.phase = BLE_UP_IDLE;
      Serial.println("BLE: aborted upload on disconnect");
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

    // Binary accumulation during BLE upload (runs on NimBLE task — memcpy only, no I/O)
    if (bleUpload.phase == BLE_UP_WAIT_DATA) {
      size_t len = val.length();
      if (bleUpload.receivedBytes + len <= IMAGE_BYTES) {
        memcpy(((uint8_t*)imageBuf) + bleUpload.receivedBytes, val.c_str(), len);
        bleUpload.receivedBytes += len;
      }
      bleUpload.lastDataTime = millis();
      if (bleUpload.receivedBytes >= bleUpload.expectedSize) {
        bleUpload.phase = BLE_UP_RECEIVED;
      }
      return;
    }

    // Drop if a previous request hasn't been processed yet
    if (bleRequest != BLE_REQ_NONE) return;

    if (val.startsWith("IMG_UPLOAD:")) {
      // IMG_UPLOAD:slot=N,type=png|s3|rp,size=X,crc32=0xY[,label=Z]
      int slot = -1;
      char typeBuf[4] = {0};
      unsigned long size = 0, crc = 0;
      sscanf(val.c_str() + 11, "slot=%d,type=%3[^,],size=%lu,crc32=0x%lx",
             &slot, typeBuf, &size, &crc);

      bleUpload.slot = slot;
      bleUpload.expectedSize = size;
      bleUpload.expectedCrc32 = crc;
      bleUpload.receivedBytes = 0;
      bleUpload.hasLabel = false;

      if (strcmp(typeBuf, "png") == 0)      bleUpload.fileType = 0;
      else if (strcmp(typeBuf, "s3") == 0)  bleUpload.fileType = 1;
      else                                   bleUpload.fileType = 2;  // rp

      // Parse optional label
      const char *lblP = strstr(val.c_str() + 11, "label=");
      if (lblP) {
        strncpy(bleUpload.label, lblP + 6, MAX_LABEL_LEN);
        bleUpload.label[MAX_LABEL_LEN] = '\0';
        bleUpload.hasLabel = true;
      }

      bleRequest = BLE_REQ_UPLOAD_START;

    } else if (val == "GET_CONFIG") {
      bleRequest = BLE_REQ_GET_CONFIG;
    } else if (val == "GET_VERSION") {
      bleRequest = BLE_REQ_GET_VERSION;
    } else if (val == "LIST") {
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
      __sync_synchronize();  // ensure buffer write is visible before setting request flag
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
        char hdr[64];
        snprintf(hdr, sizeof(hdr), "DBG:heap=%lu", (unsigned long)ESP.getFreeHeap());
        bleSendLine(hdr);
        delay(20);
        snprintf(hdr, sizeof(hdr), "IMGSTART:%d:%u", slot, bleImageSize);
        bleSendLine(hdr);
        delay(20);
        bleImageSlot = slot;
        bleImageSending = true;
        Serial.printf("BLE PNG download: slot %d, %u bytes, heap %lu\n",
                      slot, bleImageSize, (unsigned long)ESP.getFreeHeap());
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

    case BLE_REQ_GET_CONFIG: {
      // Respond from cached config — avoids losing the request if ESP32 is busy
      char cfg[128];
      snprintf(cfg, sizeof(cfg),
               "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d,numS3Images=%d",
               flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numImages, numImages);
      bleSendLine(cfg);
      // Also forward to ESP32 so it knows to push a fresh CONFIG update
      stSendText(stLink, "GET_CONFIG");
      break;
    }

    case BLE_REQ_GET_VERSION: {
      // Send S3's own version, then forward to ESP32 for ESP32+RP2040 versions
      char s3ver[64];
      snprintf(s3ver, sizeof(s3ver), "VERSION:S3=%s", FW_BUILD_TIME);
      bleSendLine(s3ver);
      stSendText(stLink, "GET_VERSION");
      break;
    }

    case BLE_REQ_FORWARD:
      stSendText(stLink, bleForwardBuf);
      break;

    case BLE_REQ_UPLOAD_START: {
      // Validate upload request — PNGs vary in size, RGB565 must fit in imageBuf
      if (bleUpload.slot < 0 || bleUpload.expectedSize == 0 || bleUpload.expectedSize > IMAGE_BYTES) {
        char err[64];
        snprintf(err, sizeof(err), "IMG_ERR:INVALID_PARAMS(slot=%d,size=%lu,max=%d)",
                 bleUpload.slot, bleUpload.expectedSize, IMAGE_BYTES);
        bleSendLine(err);
        Serial.printf("Upload rejected: slot=%d size=%lu max=%d\n",
                      bleUpload.slot, bleUpload.expectedSize, IMAGE_BYTES);
        bleUpload.phase = BLE_UP_IDLE;
        break;
      }
      if (bleImageSending) {
        bleSendLine("IMG_ERR:BUSY");
        bleUpload.phase = BLE_UP_IDLE;
        break;
      }
      bleUpload.receivedBytes = 0;
      bleUpload.lastDataTime = millis();
      bleUpload.phase = BLE_UP_WAIT_DATA;
      bleSendLine("UPLOAD_READY");
      Serial.printf("BLE upload: slot %d type %d size %lu\n",
                    bleUpload.slot, bleUpload.fileType, bleUpload.expectedSize);
      break;
    }

    default:
      break;
  }

  bleRequest = BLE_REQ_NONE;
}

// Process completed BLE upload: verify CRC, write to LittleFS
static void processBleUpload() {
  if (bleUpload.phase != BLE_UP_RECEIVED) return;

  // Verify CRC-32
  uint32_t crc = uartCrc32Update(0, (const uint8_t*)imageBuf, bleUpload.expectedSize);
  if (crc != bleUpload.expectedCrc32) {
    Serial.printf("BLE upload CRC mismatch: got 0x%08lX expected 0x%08lX\n", crc, bleUpload.expectedCrc32);
    bleSendLine("IMG_ERR:CRC_MISMATCH");
    bleUpload.phase = BLE_UP_IDLE;
    return;
  }

  // Write to LittleFS (S3 stores PNG + S3 RGB565; skips RP2040 RGB565)
  char path[24];
  if (bleUpload.fileType == 0) {         // PNG
    snprintf(path, sizeof(path), "/s3_png%02d.png", bleUpload.slot);
  } else if (bleUpload.fileType == 1) {  // S3 RGB565
    imagePath(path, bleUpload.slot);
  } else {                                // RP2040 RGB565 — S3 doesn't store
    path[0] = '\0';
  }

  if (path[0]) {
    LittleFS.remove(path);
    File f = LittleFS.open(path, "w");
    if (!f) {
      bleSendLine("IMG_ERR:WRITE_FAIL");
      bleUpload.phase = BLE_UP_IDLE;
      return;
    }
    f.write((const uint8_t*)imageBuf, bleUpload.expectedSize);
    f.close();
    Serial.printf("BLE upload: wrote %s (%lu bytes)\n", path, bleUpload.expectedSize);

    // Update S3 metadata if new S3 RGB565 slot
    if (bleUpload.fileType == 1 && bleUpload.slot >= numImages) {
      numImages = bleUpload.slot + 1;
      updateMeta();
    }
  }

  // Transition to forwarding
  bleUpload.phase = BLE_UP_FORWARDING;
}

// ════════════════════════════════════════════════════════════
//  SerialTransfer sender: forward BLE uploads to ESP32
// ════════════════════════════════════════════════════════════

static void processTextLine(const char *line);  // forward decl

static bool waitForEspResponse(uint8_t expectedPktId, unsigned long timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (stLink.available()) {
      uint8_t id = stLink.currentPacketID();
      if (id == expectedPktId) return true;
      if (id >= 0xE0) return false;  // error packet
      if (id == PKT_TEXT) {
        // Handle inline text (e.g. CONFIG: push during forwarding)
        char line[256];
        uint16_t len = stLink.bytesRead;
        uint16_t copyLen = (len < 255) ? len : 255;
        memcpy(line, stLink.packet.rxBuff, copyLen);
        line[copyLen] = '\0';
        processTextLine(line);
      }
    }
  }
  return false;
}

// Forward from imageBuf (RP2040 RGB565 — S3 didn't write to file)
static bool stForwardToEsp(uint8_t slot, uint8_t startPktId,
                           const uint8_t *data, uint32_t dataSize) {
  UploadStartPayload startPl;
  startPl.slot = slot;
  startPl.size = dataSize;
  stLink.txObj(startPl);
  stLink.sendData(sizeof(startPl), startPktId);

  if (!waitForEspResponse(PKT_RESP_READY, 3000)) {
    Serial.println("Forward: ESP32 not ready");
    return false;
  }

  uint8_t seq = 0;
  uint32_t offset = 0;
  uint32_t runCrc = 0;

  while (offset < dataSize) {
    uint16_t chunkLen = min((uint32_t)MAX_CHUNK_SIZE, dataSize - offset);

    stLink.packet.txBuff[0] = seq;
    memcpy(stLink.packet.txBuff + 1, data + offset, chunkLen);

    bool ok = false;
    for (int attempt = 0; attempt < 5; attempt++) {
      stLink.sendData(1 + chunkLen, PKT_CHUNK_DATA);
      if (waitForEspResponse(PKT_RESP_CHUNK_OK, 2000)) { ok = true; break; }
      Serial.printf("Forward: chunk %d retry %d\n", seq, attempt + 1);
    }
    if (!ok) {
      Serial.printf("Forward: chunk %d failed\n", seq);
      return false;
    }

    runCrc = uartCrc32Update(runCrc, data + offset, chunkLen);
    offset += chunkLen;
    seq++;
  }

  UploadDonePayload donePl;
  donePl.slot = slot;
  donePl.crc32 = runCrc;
  stLink.txObj(donePl);
  stLink.sendData(sizeof(donePl), PKT_UPLOAD_DONE);

  if (!waitForEspResponse(PKT_RESP_UPLOAD_OK, 5000)) {
    Serial.println("Forward: verification failed");
    return false;
  }

  Serial.printf("Forward: slot %d pktId 0x%02X OK (%lu bytes)\n", slot, startPktId, dataSize);
  return true;
}

// Forward from LittleFS file (PNG or S3 RGB565 — already written)
static bool stForwardFileToEsp(const char *path, uint8_t slot, uint8_t startPktId) {
  File f = LittleFS.open(path, "r");
  if (!f) {
    Serial.printf("Forward: %s not found\n", path);
    return false;
  }
  uint32_t fileSize = f.size();

  UploadStartPayload startPl;
  startPl.slot = slot;
  startPl.size = fileSize;
  stLink.txObj(startPl);
  stLink.sendData(sizeof(startPl), startPktId);

  if (!waitForEspResponse(PKT_RESP_READY, 3000)) {
    Serial.println("Forward: ESP32 not ready");
    f.close();
    return false;
  }

  uint8_t seq = 0;
  uint32_t runCrc = 0;
  uint8_t chunkBuf[MAX_CHUNK_SIZE];

  while (f.available()) {
    int n = f.read(chunkBuf, MAX_CHUNK_SIZE);
    if (n <= 0) break;

    stLink.packet.txBuff[0] = seq;
    memcpy(stLink.packet.txBuff + 1, chunkBuf, n);

    bool ok = false;
    for (int attempt = 0; attempt < 5; attempt++) {
      stLink.sendData(1 + n, PKT_CHUNK_DATA);
      if (waitForEspResponse(PKT_RESP_CHUNK_OK, 2000)) { ok = true; break; }
      Serial.printf("Forward: chunk %d retry %d\n", seq, attempt + 1);
    }
    if (!ok) { f.close(); return false; }

    runCrc = uartCrc32Update(runCrc, chunkBuf, n);
    seq++;
  }
  f.close();

  UploadDonePayload donePl;
  donePl.slot = slot;
  donePl.crc32 = runCrc;
  stLink.txObj(donePl);
  stLink.sendData(sizeof(donePl), PKT_UPLOAD_DONE);

  if (!waitForEspResponse(PKT_RESP_UPLOAD_OK, 5000)) {
    Serial.println("Forward: verification failed");
    return false;
  }

  Serial.printf("Forward file: %s slot %d OK (%lu bytes)\n", path, slot, fileSize);
  return true;
}

// Forward completed BLE upload to ESP32 via SerialTransfer
static void processBleUploadForward() {
  if (bleUpload.phase != BLE_UP_FORWARDING) return;

  bool ok = false;
  char path[24];

  if (bleUpload.fileType == 0) {         // PNG — forward from file
    snprintf(path, sizeof(path), "/s3_png%02d.png", bleUpload.slot);
    ok = stForwardFileToEsp(path, bleUpload.slot, PKT_UPLOAD_PNG_START);
  } else if (bleUpload.fileType == 1) {  // S3 RGB565 — forward from file
    imagePath(path, bleUpload.slot);
    ok = stForwardFileToEsp(path, bleUpload.slot, PKT_UPLOAD_START);
  } else {                                // RP2040 RGB565 — forward from imageBuf
    ok = stForwardToEsp(bleUpload.slot, PKT_UPLOAD_RP_START,
                        (const uint8_t*)imageBuf, bleUpload.expectedSize);
  }

  if (ok) {
    char resp[32];
    const char *typeStr = bleUpload.fileType == 0 ? "png" :
                          (bleUpload.fileType == 1 ? "s3" : "rp");
    snprintf(resp, sizeof(resp), "IMG_OK:%d:%s", bleUpload.slot, typeStr);
    bleSendLine(resp);
  } else {
    bleSendLine("IMG_ERR:FORWARD_FAIL");
  }

  bleUpload.phase = BLE_UP_IDLE;
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

// ── Menu ──
enum MenuItem { MENU_F1_IMAGE, MENU_F1_RATIO, MENU_F2_IMAGE, MENU_F2_RATIO, MENU_SETTINGS, MENU_COUNT };
const char* menuLabels[] = { "Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio", "Settings" };

// ── Settings sub-menu ──
enum SettingsItem { SET_BACK, SET_FACTORY_RESET, SET_ABOUT, SETTINGS_COUNT };
const char* settingsLabels[] = { "Back", "Factory Reset", "About" };
int settingsIndex = 0;
bool inSettings = false;       // true when inside the settings sub-menu
bool settingsConfirm = false;  // true when confirming factory reset
int confirmIndex = 1;          // 0 = Yes, 1 = No (default to No)
static bool factoryResetPending = false;
bool inAbout = false;
static char espVersion[32] = "";
static char rpVersion[32] = "";

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

// CRC-32 provided by uartCrc32Update() in uart_st.h

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
        thumb_buf[y * size + x].full = THEME_BG_RGB565;
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

// Packet IDs and payload structs are in uart_st.h

// ════════════════════════════════════════════════════════════
//  Upload state machine
// ════════════════════════════════════════════════════════════

enum UploadState { UPLOAD_IDLE, UPLOAD_RECEIVING };

static struct {
  UploadState state = UPLOAD_IDLE;
  uint8_t     slot;
  bool        isPng;
  uint32_t    expectedSize;
  uint32_t    receivedBytes;
  uint8_t     nextSeq;
  uint32_t    runningCrc32;
  unsigned long lastChunkTime;
  File        file;
} upload;

static void sendStResp(uint8_t pktId, uint8_t value) {
  stSendResponse(stLink, pktId, value);
}

static void sendStEmpty(uint8_t pktId) {
  stSendEmptyResponse(stLink, pktId);
}

static void abortUpload() {
  if (upload.file) {
    upload.file.close();
  }
  LittleFS.remove("/tmp.bin");
  upload.state = UPLOAD_IDLE;
  Serial.println("Upload aborted");
}

static void handleUploadStart(uint8_t slot, uint32_t size, bool isPng) {
  if (upload.state == UPLOAD_RECEIVING) {
    abortUpload();
  }

  if (isPng) {
    if (slot >= numImages) {
      sendStResp(PKT_ERR_SLOT_INVALID, 0);
      return;
    }
    if (size > IMAGE_BYTES || size == 0) {
      sendStResp(PKT_ERR_SIZE_MISMATCH, 0);
      return;
    }
  } else {
    if (slot > MAX_IMAGES) {
      sendStResp(PKT_ERR_SLOT_INVALID, 0);
      return;
    }
    if (size != IMAGE_BYTES) {
      sendStResp(PKT_ERR_SIZE_MISMATCH, 0);
      return;
    }
    if (slot > numImages) {
      sendStResp(PKT_ERR_SLOT_INVALID, 0);
      return;
    }
  }

  LittleFS.remove("/tmp.bin");
  upload.file = LittleFS.open("/tmp.bin", "w");
  if (!upload.file) {
    sendStResp(PKT_ERR_NO_SPACE, 0);
    return;
  }

  upload.state = UPLOAD_RECEIVING;
  upload.slot = slot;
  upload.isPng = isPng;
  upload.expectedSize = size;
  upload.receivedBytes = 0;
  upload.nextSeq = 0;
  upload.runningCrc32 = 0;
  upload.lastChunkTime = millis();

  Serial.printf("Upload started: slot %d, %lu bytes%s\n", slot, size, isPng ? " (PNG)" : "");
  sendStEmpty(PKT_RESP_READY);
}

static void handleChunkData(uint8_t seq, const uint8_t *data, uint16_t dataLen) {
  if (upload.state != UPLOAD_RECEIVING) {
    sendStResp(PKT_ERR_BUSY, 0);
    return;
  }

  if (seq != upload.nextSeq) {
    sendStResp(PKT_ERR_SEQ, upload.nextSeq);
    return;
  }

  if (dataLen == 0 || dataLen > MAX_CHUNK_SIZE) {
    sendStResp(PKT_ERR_CRC, 0);
    return;
  }

  size_t written = upload.file.write(data, dataLen);
  if (written != dataLen) {
    sendStResp(PKT_ERR_WRITE, 0);
    abortUpload();
    return;
  }

  upload.receivedBytes += dataLen;
  upload.runningCrc32 = uartCrc32Update(upload.runningCrc32, data, dataLen);
  upload.nextSeq = (upload.nextSeq + 1) & 0xFF;
  upload.lastChunkTime = millis();

  sendStResp(PKT_RESP_CHUNK_OK, upload.nextSeq);
}

static void handleUploadDone(uint8_t slot, uint32_t expectedCrc32) {
  if (upload.state != UPLOAD_RECEIVING) {
    sendStResp(PKT_ERR_BUSY, 0);
    return;
  }

  upload.file.close();

  if (upload.receivedBytes != upload.expectedSize) {
    Serial.printf("Size mismatch: got %lu, expected %lu\n",
                  upload.receivedBytes, upload.expectedSize);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    sendStResp(PKT_ERR_SIZE_MISMATCH, 0);
    return;
  }

  if (upload.runningCrc32 != expectedCrc32) {
    Serial.printf("CRC32 mismatch: got 0x%08lX, expected 0x%08lX\n",
                  upload.runningCrc32, expectedCrc32);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    sendStResp(PKT_ERR_CRC32_MISMATCH, 0);
    return;
  }

  if (upload.isPng) {
    char path[24];
    pngPath(path, slot);
    LittleFS.remove(path);
    bool renameOk = LittleFS.rename("/tmp.bin", path);
    upload.state = UPLOAD_IDLE;
    Serial.printf("PNG upload complete: slot %d (%lu bytes) rename=%s\n",
                  slot, upload.receivedBytes, renameOk ? "ok" : "FAIL");
    if (LittleFS.exists(path)) {
      File verify = LittleFS.open(path, "r");
      if (verify) {
        Serial.printf("PNG verify: %s exists, %u bytes\n", path, verify.size());
        verify.close();
      }
    } else {
      Serial.printf("PNG verify: %s MISSING after rename!\n", path);
    }
    sendStResp(PKT_RESP_UPLOAD_OK, numImages);
  } else {
    char path[16];
    imagePath(path, slot);
    LittleFS.remove(path);
    LittleFS.rename("/tmp.bin", path);
    upload.state = UPLOAD_IDLE;
    updateMeta();
    Serial.printf("Upload complete: slot %d, %d images total\n", slot, numImages);
    sendStResp(PKT_RESP_UPLOAD_OK, numImages);
  }
}

static void handleQueryCount() {
  sendStResp(PKT_RESP_COUNT, numImages);
}

static void handleDeleteImage(uint8_t slot) {

  if (slot >= numImages || numImages <= 1) {
    sendStResp(PKT_ERR_SLOT_INVALID, 0);
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
  sendStResp(PKT_RESP_DELETE_OK, numImages);
}

static void handleSwapImages(uint8_t slotA, uint8_t slotB) {
  if (slotA >= numImages || slotB >= numImages) {
    sendStResp(PKT_ERR_SLOT_INVALID, 0);
    return;
  }

  if (slotA == slotB) {
    sendStResp(PKT_RESP_SWAP_OK, numImages);
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
  sendStResp(PKT_RESP_SWAP_OK, numImages);
}

// Forward declaration
static void processTextLine(const char *line);

// ── Text line processing (CONFIG: responses, OK:/ERR:, LIST, LABEL:) ──

void parseConfigResponse(const char* line) {
  if (strncmp(line, "CONFIG:", 7) != 0) return;

  const char* p = line + 7;
  int f1r = 0, f2r = 0, f1i = 0, f2i = 0, ni = 0;

  if (sscanf(p, "F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d",
             &f1r, &f2r, &f1i, &f2i, &ni) >= 4) {
    // Update numImages if provided (5th field), before constraining image refs
    if (ni > 0 && (uint8_t)ni != numImages) {
      uint8_t newCount = (uint8_t)ni;
      // Delete orphaned files when count decreases (e.g. factory reset)
      if (newCount < numImages) {
        for (uint8_t i = newCount; i < numImages; i++) {
          char imgP[16];  imagePath(imgP, i);  LittleFS.remove(imgP);
          char pngP[24];  pngPath(pngP, i);    LittleFS.remove(pngP);
          Serial.printf("Removed orphaned slot %d\n", i);
        }
      }
      // updateMeta() calls countImages() which counts physical files,
      // so orphaned files must be deleted first
      updateMeta();
      Serial.printf("numImages updated to %d\n", numImages);
    }
    flavor1Ratio = constrain(f1r, 6, 24);
    flavor2Ratio = constrain(f2r, 6, 24);
    flavor1Image = constrain(f1i, 0, (int)numImages - 1);
    flavor2Image = constrain(f2i, 0, (int)numImages - 1);
    configSynced = true;
    Serial.printf("Config synced: F1_RATIO=%d F2_RATIO=%d F1_IMAGE=%d F2_IMAGE=%d numImages=%d\n",
                  flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numImages);
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
  } else if (strncmp(line, "OK:UPLOAD_DONE:", 15) == 0) {
    // Upload finalized on ESP32 — update S3's own label and metadata
    int slot = atoi(line + 15);
    if (bleUpload.hasLabel && slot >= 0 && slot < MAX_IMAGES) {
      strncpy(labels[slot], bleUpload.label, MAX_LABEL_LEN);
      labels[slot][MAX_LABEL_LEN] = '\0';
      if ((uint8_t)(slot + 1) > numImages) {
        numImages = slot + 1;
        updateMeta();
      }
      saveLabels();
      bleUpload.hasLabel = false;
    }
    Serial.printf("ESP32 confirmed: %s\n", line);
    bleSendLine(line);
  } else if (strncmp(line, "OK:FACTORY_RESET", 16) == 0) {
    Serial.printf("Factory reset complete: %s\n", line);
    factoryResetPending = false;
    settingsConfirm = false;
    inSettings = false;
    // Re-sync config from ESP32
    stSendText(stLink, "GET_CONFIG");
    drawScreen();
    bleSendLine(line);
  } else if (strncmp(line, "VERSION:ESP32=", 14) == 0) {
    strncpy(espVersion, line + 14, sizeof(espVersion) - 1);
    espVersion[sizeof(espVersion) - 1] = '\0';
    Serial.printf("Got ESP32 version: %s\n", espVersion);
    if (inAbout) drawScreen();
    bleSendLine(line);
  } else if (strncmp(line, "VERSION:RP2040=", 15) == 0) {
    strncpy(rpVersion, line + 15, sizeof(rpVersion) - 1);
    rpVersion[sizeof(rpVersion) - 1] = '\0';
    Serial.printf("Got RP2040 version: %s\n", rpVersion);
    if (inAbout) drawScreen();
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
    for (uint8_t i = 0; i < numImages; i++) {
      char buf[48];
      snprintf(buf, sizeof(buf), "IMG:%d:%s", i, labels[i]);
      stSendText(stLink, buf);
      delay(10);
    }
    stSendText(stLink, "END");
  } else if (strcmp(line, "LISTPNGS") == 0) {
    char buf[64];
    snprintf(buf, sizeof(buf), "PNGS:%d images", numImages);
    stSendText(stLink, buf);
    for (uint8_t i = 0; i < numImages; i++) {
      char path[24];
      pngPath(path, i);
      if (LittleFS.exists(path)) {
        File f = LittleFS.open(path, "r");
        if (f) {
          snprintf(buf, sizeof(buf), "PNG:%d:%u:%s", i, f.size(), path);
          f.close();
        } else {
          snprintf(buf, sizeof(buf), "PNG:%d:OPEN_FAIL:%s", i, path);
        }
      } else {
        snprintf(buf, sizeof(buf), "PNG:%d:MISSING:%s", i, path);
      }
      stSendText(stLink, buf);
      delay(10);
    }
    stSendText(stLink, "END");
  }
}

static void checkSerialTransfer() {
  if (stLink.available()) {
    uint8_t pktId = stLink.currentPacketID();

    switch (pktId) {
      case PKT_UPLOAD_START: {
        UploadStartPayload pl;
        stLink.rxObj(pl);
        handleUploadStart(pl.slot, pl.size, false);
        break;
      }
      case PKT_UPLOAD_PNG_START: {
        UploadStartPayload pl;
        stLink.rxObj(pl);
        handleUploadStart(pl.slot, pl.size, true);
        break;
      }
      case PKT_CHUNK_DATA: {
        ChunkDataPayload hdr;
        stLink.rxObj(hdr);
        uint16_t dataLen = stLink.bytesRead - sizeof(hdr);
        handleChunkData(hdr.seq, stLink.packet.rxBuff + sizeof(hdr), dataLen);
        break;
      }
      case PKT_UPLOAD_DONE: {
        UploadDonePayload pl;
        stLink.rxObj(pl);
        handleUploadDone(pl.slot, pl.crc32);
        break;
      }
      case PKT_QUERY_COUNT:
        handleQueryCount();
        break;
      case PKT_DELETE_IMAGE: {
        SlotPayload pl;
        stLink.rxObj(pl);
        handleDeleteImage(pl.slot);
        break;
      }
      case PKT_SWAP_IMAGES: {
        SwapPayload pl;
        stLink.rxObj(pl);
        handleSwapImages(pl.slotA, pl.slotB);
        break;
      }
      case PKT_TEXT: {
        char line[256];
        uint16_t len = stLink.bytesRead;
        uint16_t copyLen = (len < 255) ? len : 255;
        memcpy(line, stLink.packet.rxBuff, copyLen);
        line[copyLen] = '\0';
        processTextLine(line);
        break;
      }
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
  char buf[32];
  snprintf(buf, sizeof(buf), "SET:%s=%d", key, value);
  stSendText(stLink, buf);
  Serial.printf("UART TX: %s\n", buf);
}

void sendSave() {
  stSendText(stLink, "SAVE");
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
      (i == menuIndex) ? THEME_DOT_ACTIVE : THEME_DOT_INACTIVE, 0);
    lv_obj_set_pos(dot, startX + i * dotSpacing - 3, 207);
  }
}

void drawBrowse() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, THEME_BG, 0);

  // Title
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, THEME_TEXT_SECONDARY, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

  if (menuIndex == MENU_SETTINGS) {
    lv_obj_t *icon = lv_label_create(scr);
    lv_label_set_text(icon, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(icon, THEME_TEXT_SECONDARY, 0);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, 10);
  } else if (isImageItem()) {
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
    lv_obj_set_style_text_color(ratio, THEME_TEXT_SECONDARY, 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }

  drawNavDots();
}

void drawEdit() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, THEME_BG, 0);

  // Title — always white when editing
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_color(title, THEME_TEXT_PRIMARY, 0);

  if (menuIndex == MENU_SETTINGS) {
    // This shouldn't be reached — settings uses drawSettings() instead
    return;
  } else if (isImageItem()) {
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
    lv_obj_set_style_text_color(ratio, THEME_TEXT_PRIMARY, 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }
}

void drawAbout() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, THEME_BG, 0);

  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, "About");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, THEME_TEXT_PRIMARY, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

  int lineHeight = 24;
  int startY = 75;

  const char *labelNames[] = { "S3", "ESP32", "RP2040" };
  const char *versions[] = { FW_BUILD_TIME, espVersion, rpVersion };

  for (int i = 0; i < 3; i++) {
    lv_obj_t *name = lv_label_create(scr);
    lv_label_set_text(name, labelNames[i]);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(name, THEME_TEXT_SECONDARY, 0);
    lv_obj_align(name, LV_ALIGN_TOP_MID, 0, startY + i * (lineHeight * 2));

    lv_obj_t *ver = lv_label_create(scr);
    if (versions[i][0] != '\0') {
      lv_label_set_text(ver, versions[i]);
    } else {
      lv_label_set_text(ver, "...");
    }
    lv_obj_set_style_text_font(ver, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(ver, THEME_TEXT_PRIMARY, 0);
    lv_obj_align(ver, LV_ALIGN_TOP_MID, 0, startY + i * (lineHeight * 2) + lineHeight);
  }
}

void drawSettings() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, THEME_BG, 0);

  // Title
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, settingsConfirm ? "Factory Reset" : "Settings");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, THEME_TEXT_PRIMARY, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

  if (factoryResetPending) {
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Resetting...");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label, THEME_ALERT, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 10);
  } else if (settingsConfirm) {
    // Yes / No confirmation list
    const char *options[] = { "Yes", "No" };
    int lineHeight = 28;
    int startY = (240 - 2 * lineHeight) / 2;
    for (int i = 0; i < 2; i++) {
      lv_obj_t *item = lv_label_create(scr);
      lv_label_set_text(item, options[i]);
      lv_obj_set_style_text_font(item, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(item,
        (i == confirmIndex) ? THEME_TEXT_PRIMARY : THEME_TEXT_INACTIVE, 0);
      lv_obj_align(item, LV_ALIGN_TOP_MID, 0, startY + i * lineHeight);
    }
  } else {
    // Vertical list of all settings items
    int lineHeight = 28;
    int totalHeight = SETTINGS_COUNT * lineHeight;
    int startY = (240 - totalHeight) / 2;

    for (int i = 0; i < SETTINGS_COUNT; i++) {
      lv_obj_t *item = lv_label_create(scr);
      lv_label_set_text(item, settingsLabels[i]);
      lv_obj_set_style_text_font(item, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(item,
        (i == settingsIndex) ? THEME_TEXT_PRIMARY : THEME_TEXT_INACTIVE, 0);
      lv_obj_align(item, LV_ALIGN_TOP_MID, 0, startY + i * lineHeight);
    }
  }
}

void drawScreen() {
  if (inAbout) {
    drawAbout();
  } else if (inSettings) {
    drawSettings();
  } else if (editing) {
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
  if (factoryResetPending) return;  // locked during reset
  if (inAbout) return;  // About is view-only, tap to go back

  if (inSettings) {
    if (settingsConfirm) {
      confirmIndex = (confirmIndex + dir + 2) % 2;
      drawScreen();
      return;
    }
    settingsIndex = (settingsIndex + dir + SETTINGS_COUNT) % SETTINGS_COUNT;
    drawScreen();
  } else if (editing) {
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
  if (inAbout) {
    inAbout = false;
    drawScreen();
    return;
  }
  if (inSettings) {
    if (factoryResetPending) return;
    if (settingsConfirm) {
      if (confirmIndex == 0) {
        // Yes — execute factory reset
        factoryResetPending = true;
        Serial.println("Factory Reset confirmed — sending to ESP32");
        stSendText(stLink, "FACTORY_RESET");
      } else {
        // No — back to settings list
        settingsConfirm = false;
      }
      drawScreen();
    } else if (settingsIndex == SET_BACK) {
      inSettings = false;
      Serial.println("Exiting Settings");
      drawScreen();
    } else if (settingsIndex == SET_FACTORY_RESET) {
      settingsConfirm = true;
      confirmIndex = 1;  // default to No
      Serial.println("Factory Reset — select Yes or No");
      drawScreen();
    } else if (settingsIndex == SET_ABOUT) {
      inAbout = true;
      espVersion[0] = '\0';
      rpVersion[0] = '\0';
      stSendText(stLink, "GET_VERSION");
      Serial.println("Entering About — requesting versions");
    }
    return;
  }
  if (menuIndex == MENU_SETTINGS) {
    // Enter settings sub-menu
    inSettings = true;
    settingsIndex = 0;
    settingsConfirm = false;
    Serial.println("Entering Settings");
    drawScreen();
    return;
  }
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
  stLink.begin(Serial0);
  delay(500);
  Serial.println("ESP32-S3 Config Display starting...");

  // Allocate image buffer in PSRAM to free internal RAM for BLE stack
  imageBuf = (uint16_t *)ps_malloc(IMAGE_BYTES);
  if (!imageBuf) {
    Serial.println("FATAL: failed to allocate imageBuf in PSRAM!");
    while (1) delay(1000);
  }
  Serial.printf("imageBuf allocated in PSRAM (%u bytes), heap=%lu\n",
                IMAGE_BYTES, (unsigned long)ESP.getFreeHeap());

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
  if (!configSynced && millis() - lastGetConfig > 500) {
    stSendText(stLink, "GET_CONFIG");
    Serial.println("UART TX: GET_CONFIG (boot sync)");
    lastGetConfig = millis();
  }

  // Check for incoming SerialTransfer packets
  checkSerialTransfer();

  // Process BLE requests on main task (safe for LittleFS + Serial0)
  processBleRequest();

  // Process completed BLE uploads (verify CRC, write to LittleFS, forward)
  processBleUpload();
  processBleUploadForward();

  // BLE upload timeout (30s with no data)
  if (bleUpload.phase == BLE_UP_WAIT_DATA &&
      millis() - bleUpload.lastDataTime > 30000) {
    bleUpload.phase = BLE_UP_IDLE;
    bleSendLine("IMG_ERR:TIMEOUT");
    Serial.println("BLE upload timed out");
  }

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
