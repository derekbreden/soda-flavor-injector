#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <LittleFS.h>
#include <SerialPIO.h>

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

static char labels[MAX_IMAGES][MAX_LABEL_LEN + 1];  // null-terminated labels per slot

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
//  Binary protocol constants
// ════════════════════════════════════════════════════════════

#define STX 0x02

// Commands (ESP32 -> RP2040)
#define CMD_UPLOAD_START 0x01
#define CMD_CHUNK_DATA   0x02
#define CMD_UPLOAD_DONE  0x03
#define CMD_QUERY_COUNT  0x04
#define CMD_DELETE_IMAGE 0x05
#define CMD_SWAP_IMAGES  0x06

// Responses (RP2040 -> ESP32)
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
  pioSerial.write(resp, 6);
  pioSerial.flush();
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

  if (slot > MAX_IMAGES) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
  }
  if (size != IMAGE_BYTES) {
    sendBinaryResponse(ERR_SIZE_MISMATCH, 0);
    return;
  }

  // Check if we have space (only allow writing to existing slot or next slot)
  if (slot > numImages) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
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
  char path[16];
  imagePath(path, slot);
  LittleFS.remove(path);  // remove old if overwriting
  LittleFS.rename("/tmp.bin", path);

  upload.state = UPLOAD_IDLE;
  updateMeta();

  Serial.printf("Upload complete: slot %d, %d images total\n", slot, numImages);
  sendBinaryResponse(RESP_UPLOAD_OK, numImages);
}

static void handleQueryCount() {
  sendBinaryResponse(RESP_COUNT, numImages);
}

static void handleDeleteImage(const uint8_t *msg) {
  uint8_t slot = msg[3];

  if (slot >= numImages) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
  }

  // Can't delete if only 1 image remains
  if (numImages <= 1) {
    sendBinaryResponse(ERR_SLOT_INVALID, 0);
    return;
  }

  // Delete the file
  char path[16];
  imagePath(path, slot);
  LittleFS.remove(path);

  // Shift all files above this slot down by 1
  char pathFrom[16], pathTo[16];
  for (uint8_t i = slot + 1; i < numImages; i++) {
    imagePath(pathFrom, i);
    imagePath(pathTo, i - 1);
    LittleFS.rename(pathFrom, pathTo);
  }

  // Shift labels down (before updateMeta changes numImages)
  for (uint8_t i = slot; i + 1 < numImages; i++) {
    strncpy(labels[i], labels[i + 1], MAX_LABEL_LEN);
  }
  if (numImages > 0) labels[numImages - 1][0] = '\0';

  updateMeta();
  saveLabels();

  // Fix image map: adjust any references to shifted slots
  for (uint8_t i = 0; i < 2; i++) {
    if (imageMap[i] == slot) {
      imageMap[i] = 0;  // deleted slot → fall back to 0
    } else if (imageMap[i] > slot) {
      imageMap[i]--;     // shifted down
    }
    // Clamp to valid range
    if (imageMap[i] >= numImages) imageMap[i] = 0;
  }
  saveImageMap();

  // Redraw if the active image changed
  if (activeFlavor >= 0) drawFlavor(activeFlavor);

  Serial.printf("Deleted slot %d, shifted down, %d images remain\n", slot, numImages);
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

  // Swap via temp file: A -> tmp, B -> A, tmp -> B
  char pathA[16], pathB[16];
  imagePath(pathA, slotA);
  imagePath(pathB, slotB);

  LittleFS.rename(pathA, "/swap.bin");
  LittleFS.rename(pathB, pathA);
  LittleFS.rename("/swap.bin", pathB);

  // Swap labels
  char tmpLabel[MAX_LABEL_LEN + 1];
  strncpy(tmpLabel, labels[slotA], MAX_LABEL_LEN + 1);
  strncpy(labels[slotA], labels[slotB], MAX_LABEL_LEN + 1);
  strncpy(labels[slotB], tmpLabel, MAX_LABEL_LEN + 1);
  saveLabels();

  // Fix image map references
  for (uint8_t i = 0; i < 2; i++) {
    if (imageMap[i] == slotA) imageMap[i] = slotB;
    else if (imageMap[i] == slotB) imageMap[i] = slotA;
  }
  saveImageMap();

  // Redraw if the active image was involved
  if (activeFlavor >= 0) drawFlavor(activeFlavor);

  Serial.printf("Swapped slots %d <-> %d\n", slotA, slotB);
  sendBinaryResponse(RESP_SWAP_OK, numImages);
}

// ════════════════════════════════════════════════════════════
//  UART byte-level parser (text + binary multiplexed)
// ════════════════════════════════════════════════════════════

#define TEXT_BUF_SIZE 64  // must fit "LABEL:NN:name" commands
#define BIN_BUF_SIZE 142  // max: 6 header + 128 payload + 2 CRC + margin

static char textBuf[TEXT_BUF_SIZE];
static uint8_t textPos = 0;

static uint8_t binBuf[BIN_BUF_SIZE];
static uint8_t binPos = 0;
static bool inBinary = false;

static void processTextCommand(const char *cmd) {
  if (strncmp(cmd, "MAP:", 4) == 0) {
    // MAP:<img0>,<img1>
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
    // LABEL:slot:name — set label for a slot
    int slot = atoi(cmd + 6);
    const char *colon = strchr(cmd + 6, ':');
    if (colon && slot >= 0 && slot < numImages) {
      strncpy(labels[slot], colon + 1, MAX_LABEL_LEN);
      labels[slot][MAX_LABEL_LEN] = '\0';
      saveLabels();
      Serial.printf("Label set: slot %d = %s\n", slot, labels[slot]);
    }

  } else if (strcmp(cmd, "LIST") == 0) {
    // Return all slot labels as text over pioSerial
    for (uint8_t i = 0; i < numImages; i++) {
      pioSerial.printf("IMG:%d:%s\n", i, labels[i]);
    }
    pioSerial.println("END");
    pioSerial.flush();
  }
}

static bool tryParseBinaryMessage() {
  if (binPos < 3) return false;

  uint8_t cmd = binBuf[2];
  int expectedLen = -1;

  switch (cmd) {
    case CMD_UPLOAD_START:  // 10 bytes
      expectedLen = 10;
      break;
    case CMD_CHUNK_DATA:    // variable: 6 + payloadLen + 2
      if (binPos < 6) return false;
      {
        uint16_t payloadLen = binBuf[4] | (binBuf[5] << 8);
        if (payloadLen == 0 || payloadLen > MAX_CHUNK_SIZE) {
          return true;  // invalid, discard
        }
        expectedLen = 6 + payloadLen + 2;
      }
      break;
    case CMD_UPLOAD_DONE:   // 10 bytes
      expectedLen = 10;
      break;
    case CMD_QUERY_COUNT:   // 6 bytes
      expectedLen = 6;
      break;
    case CMD_DELETE_IMAGE:  // 6 bytes: STX STX CMD slot CRC16
      expectedLen = 6;
      break;
    case CMD_SWAP_IMAGES:   // 7 bytes: STX STX CMD slotA slotB CRC16
      expectedLen = 7;
      break;
    default:
      return true;  // unknown command, discard
  }

  if (binPos < expectedLen) return false;  // need more bytes

  // Validate CRC-16 over everything except the last 2 bytes
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

static void checkUART() {
  while (pioSerial.available()) {
    uint8_t b = pioSerial.read();

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
        processTextCommand(textBuf);
        textPos = 0;
      }
    } else if (textPos < TEXT_BUF_SIZE - 1) {
      textBuf[textPos++] = b;
    } else {
      textPos = 0;  // overflow, discard line
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
  checkUART();

  uint8_t newFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;

  if (newFlavor != activeFlavor) {
    activeFlavor = newFlavor;
    drawFlavor(activeFlavor);
    Serial.printf("Switched to flavor %d (image %d)\n",
                  activeFlavor + 1, imageMap[activeFlavor]);
  }

  delay(50);
}
