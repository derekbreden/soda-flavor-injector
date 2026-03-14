#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <LittleFS.h>
#include <SerialPIO.h>
#include <uart_st.h>

// Seed images (compiled in for first-boot only, then served from LittleFS)
#include "flavor1_bitmap.h"
#include "flavor2_bitmap.h"
#include "flavor3_bitmap.h"

// ── Flavor switch ──
#define FLAVOR_SW_PIN  29  // GP29 (ADC3) on SH1.0 connector

// ── Bidirectional UART with ESP32 (PIO-based) ──
#define UART_TX_PIN    27  // GP27 – sends ACKs/responses to ESP32
#define UART_RX_PIN    26  // GP26 – receives commands from ESP32
SerialPIO pioSerial(UART_TX_PIN, UART_RX_PIN, 512);
SerialTransfer stLink;

// ── Display wiring (fixed on RP2040-LCD-0.99-B board) ──
#define LCD_DC   8
#define LCD_CS   9
#define LCD_CLK  10
#define LCD_DIN  11
#define LCD_RST  13
#define LCD_BL   25

#define SCREEN_W 128
#define SCREEN_H 115
#define IMAGE_BYTES (SCREEN_W * SCREEN_H * 2)  // 29440

// ── Display bus & driver ──
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(
    LCD_DC, LCD_CS, LCD_CLK, LCD_DIN, -1 /* MISO */, spi1);

Arduino_GFX *gfx = new Arduino_GC9107(
    bus, LCD_RST, 0 /* rotation */, true /* IPS */,
    SCREEN_W, SCREEN_H,
    0 /* col offset */, 13 /* row offset */);

// ── Image mapping & state ──
static uint8_t imageMap[2] = { 0, 1 };
static uint8_t numImages = 0;
static int8_t activeFlavor = -1;
static uint16_t displayBuffer[SCREEN_W * SCREEN_H];  // RAM buffer for current image

#define CONFIG_PATH  "/config.txt"
#define META_PATH    "/meta.txt"
#define LABELS_PATH  "/labels.txt"
#define MAX_IMAGES   99
#define MAX_LABEL_LEN 32
#define MAX_CHUNK_SIZE 128

static char labels[MAX_IMAGES][MAX_LABEL_LEN + 1];  // null-terminated labels per slot

// ════════════════════════════════════════════════════════════
//  LittleFS image management
// ════════════════════════════════════════════════════════════

static void imagePath(char *buf, uint8_t slot) {
  snprintf(buf, 16, "/img%02d.bin", slot);
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
  size_t n = f.read((uint8_t *)displayBuffer, IMAGE_BYTES);
  f.close();
  return (n == IMAGE_BYTES);
}

// ════════════════════════════════════════════════════════════
//  First-boot seeding: write PROGMEM images to LittleFS
// ════════════════════════════════════════════════════════════

static const uint16_t *seedBitmaps[] = {
  flavor1_bitmap,   // 0 = Diet Wild Cherry Pepsi
  flavor2_bitmap,   // 1 = Diet Mountain Dew
  flavor3_bitmap,   // 2 = Diet Coke
};
static const char *seedLabels[] = {
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
      // On RP2040, PROGMEM is directly addressable (XIP flash)
      f.write((const uint8_t *)seedBitmaps[i], IMAGE_BYTES);
      f.close();
      Serial.printf("  Seeded %s (%d bytes)\n", path, IMAGE_BYTES);
    }
  }
  // Set default labels
  for (uint8_t i = 0; i < 3; i++) {
    strncpy(labels[i], seedLabels[i], MAX_LABEL_LEN);
    labels[i][MAX_LABEL_LEN] = '\0';
  }

  updateMeta();
  saveLabels();
  Serial.println("Seeding complete.");
}

// ════════════════════════════════════════════════════════════
//  Config persistence (image mapping)
// ════════════════════════════════════════════════════════════

static void loadImageMap() {
  if (!LittleFS.exists(CONFIG_PATH)) return;
  File f = LittleFS.open(CONFIG_PATH, "r");
  if (!f) return;
  String line = f.readStringUntil('\n');
  f.close();
  int comma = line.indexOf(',');
  if (comma > 0) {
    uint8_t a = line.substring(0, comma).toInt();
    uint8_t b = line.substring(comma + 1).toInt();
    if (a < numImages && b < numImages) {
      imageMap[0] = a;
      imageMap[1] = b;
      Serial.printf("Loaded image map: 0->%d, 1->%d\n", a, b);
    }
  }
}

static void saveImageMap() {
  File f = LittleFS.open(CONFIG_PATH, "w");
  if (!f) return;
  f.printf("%d,%d\n", imageMap[0], imageMap[1]);
  f.close();
  Serial.printf("Saved image map: 0->%d, 1->%d\n", imageMap[0], imageMap[1]);
}

// ════════════════════════════════════════════════════════════
//  Display drawing
// ════════════════════════════════════════════════════════════

static void drawFlavor(uint8_t flavor) {
  uint8_t slot = imageMap[flavor];
  if (loadImageFromFS(slot)) {
    gfx->draw16bitRGBBitmap(0, 0, displayBuffer, SCREEN_W, SCREEN_H);
  } else {
    Serial.printf("Failed to load image slot %d\n", slot);
  }
}

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

static void abortUpload() {
  if (upload.file) {
    upload.file.close();
  }
  LittleFS.remove("/tmp.bin");
  upload.state = UPLOAD_IDLE;
  Serial.println("Upload aborted");
}

static void handleUploadStart(uint8_t slot, uint32_t size) {
  if (upload.state == UPLOAD_RECEIVING) {
    abortUpload();
  }

  if (slot > MAX_IMAGES) {
    stSendResponse(stLink, PKT_ERR_SLOT_INVALID, 0);
    return;
  }
  if (size != IMAGE_BYTES) {
    stSendResponse(stLink, PKT_ERR_SIZE_MISMATCH, 0);
    return;
  }
  if (slot > numImages) {
    stSendResponse(stLink, PKT_ERR_SLOT_INVALID, 0);
    return;
  }

  LittleFS.remove("/tmp.bin");
  upload.file = LittleFS.open("/tmp.bin", "w");
  if (!upload.file) {
    stSendResponse(stLink, PKT_ERR_NO_SPACE, 0);
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
  stSendEmptyResponse(stLink, PKT_RESP_READY);
}

static void handleChunkData(uint8_t seq, const uint8_t *data, uint16_t dataLen) {
  if (upload.state != UPLOAD_RECEIVING) {
    stSendResponse(stLink, PKT_ERR_BUSY, 0);
    return;
  }

  if (seq != upload.nextSeq) {
    stSendResponse(stLink, PKT_ERR_SEQ, upload.nextSeq);
    return;
  }

  if (dataLen == 0 || dataLen > MAX_CHUNK_SIZE) {
    stSendResponse(stLink, PKT_ERR_CRC, 0);
    return;
  }

  size_t written = upload.file.write(data, dataLen);
  if (written != dataLen) {
    stSendResponse(stLink, PKT_ERR_WRITE, 0);
    abortUpload();
    return;
  }

  upload.receivedBytes += dataLen;
  upload.runningCrc32 = uartCrc32Update(upload.runningCrc32, data, dataLen);
  upload.nextSeq = (upload.nextSeq + 1) & 0xFF;
  upload.lastChunkTime = millis();

  stSendResponse(stLink, PKT_RESP_CHUNK_OK, upload.nextSeq);
}

static void handleUploadDone(uint8_t slot, uint32_t expectedCrc32) {
  if (upload.state != UPLOAD_RECEIVING) {
    stSendResponse(stLink, PKT_ERR_BUSY, 0);
    return;
  }

  upload.file.close();

  if (upload.receivedBytes != upload.expectedSize) {
    Serial.printf("Size mismatch: got %lu, expected %lu\n",
                  upload.receivedBytes, upload.expectedSize);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    stSendResponse(stLink, PKT_ERR_SIZE_MISMATCH, 0);
    return;
  }

  if (upload.runningCrc32 != expectedCrc32) {
    Serial.printf("CRC32 mismatch: got 0x%08lX, expected 0x%08lX\n",
                  upload.runningCrc32, expectedCrc32);
    LittleFS.remove("/tmp.bin");
    upload.state = UPLOAD_IDLE;
    stSendResponse(stLink, PKT_ERR_CRC32_MISMATCH, 0);
    return;
  }

  char path[16];
  imagePath(path, slot);
  LittleFS.remove(path);
  LittleFS.rename("/tmp.bin", path);

  upload.state = UPLOAD_IDLE;
  updateMeta();

  Serial.printf("Upload complete: slot %d, %d images total\n", slot, numImages);
  stSendResponse(stLink, PKT_RESP_UPLOAD_OK, numImages);
}

static void handleDeleteImage(uint8_t slot) {
  if (slot >= numImages || numImages <= 1) {
    stSendResponse(stLink, PKT_ERR_SLOT_INVALID, 0);
    return;
  }

  char path[16];
  imagePath(path, slot);
  LittleFS.remove(path);

  char pathFrom[16], pathTo[16];
  for (uint8_t i = slot + 1; i < numImages; i++) {
    imagePath(pathFrom, i);
    imagePath(pathTo, i - 1);
    LittleFS.rename(pathFrom, pathTo);
  }

  for (uint8_t i = slot; i + 1 < numImages; i++) {
    strncpy(labels[i], labels[i + 1], MAX_LABEL_LEN);
  }
  if (numImages > 0) labels[numImages - 1][0] = '\0';

  updateMeta();
  saveLabels();

  for (uint8_t i = 0; i < 2; i++) {
    if (imageMap[i] == slot) {
      imageMap[i] = 0;
    } else if (imageMap[i] > slot) {
      imageMap[i]--;
    }
    if (imageMap[i] >= numImages) imageMap[i] = 0;
  }
  saveImageMap();

  if (activeFlavor >= 0) drawFlavor(activeFlavor);

  Serial.printf("Deleted slot %d, shifted down, %d images remain\n", slot, numImages);
  stSendResponse(stLink, PKT_RESP_DELETE_OK, numImages);
}

static void handleSwapImages(uint8_t slotA, uint8_t slotB) {
  if (slotA >= numImages || slotB >= numImages) {
    stSendResponse(stLink, PKT_ERR_SLOT_INVALID, 0);
    return;
  }

  if (slotA == slotB) {
    stSendResponse(stLink, PKT_RESP_SWAP_OK, numImages);
    return;
  }

  char pathA[16], pathB[16];
  imagePath(pathA, slotA);
  imagePath(pathB, slotB);

  LittleFS.rename(pathA, "/swap.bin");
  LittleFS.rename(pathB, pathA);
  LittleFS.rename("/swap.bin", pathB);

  char tmpLabel[MAX_LABEL_LEN + 1];
  strncpy(tmpLabel, labels[slotA], MAX_LABEL_LEN + 1);
  strncpy(labels[slotA], labels[slotB], MAX_LABEL_LEN + 1);
  strncpy(labels[slotB], tmpLabel, MAX_LABEL_LEN + 1);
  saveLabels();

  for (uint8_t i = 0; i < 2; i++) {
    if (imageMap[i] == slotA) imageMap[i] = slotB;
    else if (imageMap[i] == slotB) imageMap[i] = slotA;
  }
  saveImageMap();

  if (activeFlavor >= 0) drawFlavor(activeFlavor);

  Serial.printf("Swapped slots %d <-> %d\n", slotA, slotB);
  stSendResponse(stLink, PKT_RESP_SWAP_OK, numImages);
}

// ════════════════════════════════════════════════════════════
//  Process text command received via PKT_TEXT
// ════════════════════════════════════════════════════════════

static void processTextCommand(const char *cmd) {
  if (strncmp(cmd, "MAP:", 4) == 0) {
    const char *payload = cmd + 4;
    const char *comma = strchr(payload, ',');
    if (!comma) return;

    uint8_t a = atoi(payload);
    uint8_t b = atoi(comma + 1);

    if (a >= numImages || b >= numImages) {
      Serial.printf("MAP rejected: (%d,%d), numImages=%d\n", a, b, numImages);
      return;
    }

    if (a != imageMap[0] || b != imageMap[1]) {
      imageMap[0] = a;
      imageMap[1] = b;
      saveImageMap();
      if (activeFlavor >= 0) drawFlavor(activeFlavor);
      Serial.printf("MAP applied: 0->%d, 1->%d\n", a, b);
    }

  } else if (strncmp(cmd, "LABEL:", 6) == 0) {
    int slot = atoi(cmd + 6);
    const char *colon = strchr(cmd + 6, ':');
    if (colon && slot >= 0 && slot < numImages) {
      strncpy(labels[slot], colon + 1, MAX_LABEL_LEN);
      labels[slot][MAX_LABEL_LEN] = '\0';
      saveLabels();
      Serial.printf("Label set: slot %d = %s\n", slot, labels[slot]);
    }

  } else if (strcmp(cmd, "LIST") == 0) {
    for (uint8_t i = 0; i < numImages; i++) {
      char buf[48];
      snprintf(buf, sizeof(buf), "IMG:%d:%s", i, labels[i]);
      stSendText(stLink, buf);
    }
    stSendText(stLink, "END");
  }
}

// ════════════════════════════════════════════════════════════
//  SerialTransfer packet handler
// ════════════════════════════════════════════════════════════

static void checkSerialTransfer() {
  if (stLink.available()) {
    uint8_t pktId = stLink.currentPacketID();

    switch (pktId) {
      case PKT_UPLOAD_START: {
        UploadStartPayload p;
        stLink.rxObj(p);
        handleUploadStart(p.slot, p.size);
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
        UploadDonePayload p;
        stLink.rxObj(p);
        handleUploadDone(p.slot, p.crc32);
        break;
      }
      case PKT_QUERY_COUNT:
        stSendResponse(stLink, PKT_RESP_COUNT, numImages);
        break;
      case PKT_DELETE_IMAGE: {
        SlotPayload p;
        stLink.rxObj(p);
        handleDeleteImage(p.slot);
        break;
      }
      case PKT_SWAP_IMAGES: {
        SwapPayload p;
        stLink.rxObj(p);
        handleSwapImages(p.slotA, p.slotB);
        break;
      }
      case PKT_TEXT: {
        char textBuf[256];
        uint16_t len = stLink.bytesRead;
        if (len > sizeof(textBuf) - 1) len = sizeof(textBuf) - 1;
        memcpy(textBuf, stLink.packet.rxBuff, len);
        textBuf[len] = '\0';
        processTextCommand(textBuf);
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

// ════════════════════════════════════════════════════════════
//  Setup
// ════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);

  // Init LittleFS (1.5MB partition)
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed, formatting...");
    LittleFS.format();
    LittleFS.begin();
  }

  // Seed default images on first boot
  seedDefaultImages();

  // Count available images, load labels and mapping
  numImages = countImages();
  loadLabels();
  Serial.printf("Found %d images in LittleFS\n", numImages);
  for (uint8_t i = 0; i < numImages; i++) {
    Serial.printf("  Slot %d: %s\n", i, labels[i][0] ? labels[i] : "(unlabeled)");
  }
  loadImageMap();

  // Bidirectional UART with ESP32 at 38400 baud
  pioSerial.begin(38400);
  stLink.begin(pioSerial);

  // Flavor switch
  pinMode(FLAVOR_SW_PIN, INPUT_PULLUP);

  // Display
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  gfx->begin();

  // Initial display
  activeFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;
  drawFlavor(activeFlavor);

  Serial.printf("Display ready - flavor %d (image %d)\n",
                activeFlavor + 1, imageMap[activeFlavor]);
}

// ════════════════════════════════════════════════════════════
//  Loop
// ════════════════════════════════════════════════════════════

void loop() {
  checkSerialTransfer();

  uint8_t newFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;

  if (newFlavor != activeFlavor) {
    activeFlavor = newFlavor;
    drawFlavor(activeFlavor);
    Serial.printf("Switched to flavor %d (image %d)\n",
                  activeFlavor + 1, imageMap[activeFlavor]);
  }

  delay(50);
}
