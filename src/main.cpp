#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <uart_st.h>
#include "fw_version.h"

// ════════════════════════════════════════════════════════════
//  Dual-Flavor Soda Maker
// ════════════════════════════════════════════════════════════

// ── L298N Board A (flavor 1) ──
#define A_ENA  33
#define A_IN1  25
#define A_IN2  26
#define A_IN3  27
#define A_IN4  14
#define A_ENB  12

// ── L298N Board B (flavor 2) ──
#define B_ENA  19
#define B_IN1  18
#define B_IN2   5
#define B_IN3  17
#define B_IN4  16
#define B_ENB   4

// ── Inputs ──
#define FLAVOR_SW_PIN   13   // latching toggle: flavor select (air switch)
#define PRIME_BTN_PIN   22   // momentary: manual prime / activate
#define FLOW_PIN        23   // flow meter pulse input

// ── LEDs ──
#define LED_FLAVOR1     21   // lit when flavor 1 is selected (blinks while dispensing)
#define LED_FLAVOR2      2   // lit when flavor 2 is selected (blinks while dispensing)

// ── Per-flavor config (runtime, persisted in LittleFS) ──
// Ratio: flavoring to water in 1:X. Lower = more flavor.
//   6  = maximum strength (traditional BIB, e.g. Coke syrup)
//  20  = tuned for SodaStream concentrates
//  24  = minimum strength (hard limit floor)
// Image: index into the RP2040's LittleFS image store
uint8_t numImages   = 0;  // updated at boot via QUERY_COUNT to RP2040
uint8_t numS3Images = 0;  // updated at boot via QUERY_COUNT to S3

// ── Image store (ESP32 LittleFS — authoritative source) ──
#define RP2040_IMAGE_BYTES (128 * 115 * 2)   // 29440
#define S3_IMAGE_BYTES     (240 * 240 * 2)    // 115200
#define MAX_STORE_IMAGES   23
#define MAX_LABEL_LEN      32
#define STORE_CHUNK_SIZE   128
#define ESP_META_PATH      "/meta.txt"
#define ESP_LABELS_PATH    "/labels.txt"
#define FW_VERSION_PATH    "/fw_version.txt"
#define USER_CONFIG_PATH   "/user_config.txt"
#define FW_VERSION         FW_BUILD_TIME

// Factory defaults JSON (embedded in firmware flash at build time)
extern const char factory_defaults_start[] asm("_binary_images_factory_defaults_json_start");

// Binary protocol constants
#define CMD_UPLOAD_START  0x01
#define CMD_CHUNK_DATA    0x02
#define CMD_UPLOAD_DONE   0x03
#define RESP_READY        0x10
#define RESP_CHUNK_OK     0x11
#define RESP_UPLOAD_OK    0x12

uint8_t espNumImages = 0;
char espLabels[MAX_STORE_IMAGES][MAX_LABEL_LEN + 1];

uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;

bool firstBoot = false;

// ── Display UART (ESP32 ↔ RP2040, bidirectional) ──
#define DISPLAY_TX_PIN          32     // UART TX to RP2040 display
#define DISPLAY_RX_PIN          35     // UART RX from RP2040 (input-only GPIO)
#define CONFIG_SEND_INTERVAL_MS 30000  // resend image mapping every 30s

SerialTransfer stRP;  // SerialTransfer on Serial2 (RP2040 link)
SerialTransfer stS3;  // SerialTransfer on Serial1 (S3 link)

void sendMapToRP() {
  char buf[20];
  snprintf(buf, sizeof(buf), "MAP:%d,%d", flavor1Image, flavor2Image);
  stSendText(stRP, buf);
}

// ── Config UART (ESP32 ↔ ESP32-S3, bidirectional) ──
// GPIO 15 for TX: frees GPIO 2 (boot-strap pin that must be LOW to flash).
// GPIO 15 is a strapping pin too but only affects boot log silence — won't block flashing.
#define CONFIG_TX_PIN  15    // UART TX to ESP32-S3 (was LED_FLAVOR2)
#define CONFIG_RX_PIN  34    // UART RX from ESP32-S3 (input-only pin)

// ── Pump hard limits (physical constraints) ──
#define PUMP_ON_MIN_MS     50   // below this, pump doesn't reliably dispense
#define PUMP_OFF_MAX_MS  1000   // above this, reaction time is too slow
#define PUMP_SPEED        255

// ── Recipe shape (empirically tuned baseline, not user-adjustable) ──
// These define how duty cycle scales with flow rate.
// At FLAVOR_RATIO=20 (baseline) they produce:
//   1 pulse →  50 on / 600 off  (7.7% duty)
//   6 pulse → 200 on / 300 off  (40% duty)
#define SHAPE_ON_BASE    20
#define SHAPE_ON_SLOPE   30
#define SHAPE_OFF_BASE  660
#define SHAPE_OFF_SLOPE  60

// ── LED blink while dispensing ──
#define BLINK_INTERVAL_MS  50

// ── Flow detection ──
#define FLOW_CHECK_INTERVAL_MS  50
#define FLOW_MIN_PULSES          1   // minimum to count as flowing
#define FLOW_FULL_PULSES         6   // pulses per interval at full flow
#define COOLDOWN_MS           1000   // settle time after zero detected in cycle

// ════════════════════════════════════════════════════════════
//  Motor abstraction
// ════════════════════════════════════════════════════════════

struct MotorChannel {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
};

struct Flavor {
  MotorChannel pump;
  MotorChannel valve;
  uint8_t ratio;
};

Flavor flavors[] = {
  // Flavor 1 — Board A
  {
    { A_ENA, A_IN1, A_IN2 },   // pump
    { A_ENB, A_IN3, A_IN4 },   // valve
    20,                         // ratio (overwritten by loadConfig)
  },
  // Flavor 2 — Board B
  {
    { B_ENA, B_IN1, B_IN2 },   // pump
    { B_ENB, B_IN3, B_IN4 },   // valve
    20,                         // ratio (overwritten by loadConfig)
  },
};

void motorOn(const MotorChannel& m, uint8_t speed) {
  digitalWrite(m.in1, HIGH);
  digitalWrite(m.in2, LOW);
  analogWrite(m.ena, speed);
}

void motorOff(const MotorChannel& m) {
  digitalWrite(m.in1, LOW);
  digitalWrite(m.in2, LOW);
  analogWrite(m.ena, 0);
}

// ════════════════════════════════════════════════════════════
//  Flow meter ISR
// ════════════════════════════════════════════════════════════

volatile unsigned long pulseCount = 0;

void IRAM_ATTR flowPulse() {
  pulseCount++;
}

// ════════════════════════════════════════════════════════════
//  Cycle timing
// ════════════════════════════════════════════════════════════

// Compute pump on/off times from flow rate and FLAVOR_RATIO.
// The scaling factor S maps FLAVOR_RATIO to a duty cycle multiplier:
//   S = 2.5 at FLAVOR_RATIO=6  (constant on at full flow)
//   S = 1.0 at FLAVOR_RATIO=20 (baseline recipe)
// Off-time scales as baseline/S.  On-time is derived from duty cycle.
// Both are clamped to hard limits.
void computeCycleTiming(unsigned long pulses, uint8_t ratio, unsigned long &onMs, unsigned long &offMs) {
  unsigned long clamped = constrain(pulses, FLOW_MIN_PULSES, FLOW_FULL_PULSES);

  // Baseline on/off at this flow rate (what ratio=20 produces)
  unsigned long onBase  = SHAPE_ON_BASE  + SHAPE_ON_SLOPE  * clamped;
  unsigned long offBase = SHAPE_OFF_BASE - SHAPE_OFF_SLOPE * clamped;
  unsigned long total   = onBase + offBase;

  // Scale factor from ratio
  float S = 2.5f - 1.5f * (ratio - 6) / 14.0f;

  // Duty cycle at this flow rate, scaled by S
  float duty = S * (float)onBase / (float)total;

  if (duty >= 1.0f) {
    // Constant on — no off phase
    onMs  = total;
    offMs = 0;
  } else {
    offMs = (unsigned long)(offBase / S);
    onMs  = (unsigned long)(offMs * duty / (1.0f - duty));
  }

  // Clamp to hard limits
  onMs  = max(onMs, (unsigned long)PUMP_ON_MIN_MS);
  offMs = min(offMs, (unsigned long)PUMP_OFF_MAX_MS);
}

// ════════════════════════════════════════════════════════════
//  State
// ════════════════════════════════════════════════════════════

uint8_t activeFlavor   = 0;       // 0 or 1
bool    waterFlowing   = false;   // true if latest 50ms reading >= FLOW_MIN_PULSES
unsigned long flowPulses = 0;     // pulse count from latest 50ms check
bool    primePressed   = false;

// ── Pump state machine ──
// A "cycle" = one ON phase + one OFF phase, timing locked at cycle start.
// PRIME = manual override, pump runs continuously.
// COOLDOWN = saw a zero during cycle, pump off for 1000ms, readings discarded.
enum PumpState { PUMP_IDLE, PUMP_ON, PUMP_OFF, PUMP_COOLDOWN, PUMP_PRIME };
PumpState pumpState              = PUMP_IDLE;
unsigned long phaseStart         = 0;
unsigned long cycleOnMs          = 0;     // locked for entire cycle
unsigned long cycleOffMs         = 0;     // locked for entire cycle
unsigned long cyclePulseSum      = 0;     // accumulated during cycle
unsigned long cyclePulseReadings = 0;     // readings taken during cycle
bool cycleSawZero                = false; // any 0 reading during this cycle?

// ── Valve + LED ──
bool valveOpen                   = false;
unsigned long lastFlowCheck      = 0;
unsigned long lastBlinkToggle    = 0;
bool blinkState                  = true;
unsigned long lastConfigSend     = 0;

// ════════════════════════════════════════════════════════════
//  Factory defaults & LittleFS config persistence
// ════════════════════════════════════════════════════════════

// Forward declarations (defined after image store section)
void saveEspMeta();
void saveEspLabels();

// Parse compiled-in factory_defaults.json → set runtime config + labels + store count
void applyFactoryDefaults() {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, factory_defaults_start);
  if (err) {
    Serial.printf("WARNING: factory_defaults.json parse failed: %s\n", err.c_str());
    return;
  }

  flavor1Ratio = doc["flavor1_ratio"] | 20;
  flavor2Ratio = doc["flavor2_ratio"] | 20;
  flavor1Image = doc["flavor1_image"] | 0;
  flavor2Image = doc["flavor2_image"] | 1;

  flavors[0].ratio = flavor1Ratio;
  flavors[1].ratio = flavor2Ratio;

  // Load factory image labels
  JsonArray images = doc["images"];
  memset(espLabels, 0, sizeof(espLabels));
  uint8_t count = 0;
  for (JsonVariant v : images) {
    if (count >= MAX_STORE_IMAGES) break;
    strncpy(espLabels[count], v.as<const char*>(), MAX_LABEL_LEN);
    count++;
  }

  // Update store count to match factory defaults and persist
  espNumImages = count;
  saveEspMeta();
  saveEspLabels();

  Serial.printf("Factory defaults applied: F1 ratio=%d image=%d, F2 ratio=%d image=%d, %d images\n",
                flavor1Ratio, flavor1Image, flavor2Ratio, flavor2Image, count);
}

// Check if this is a new firmware version (first boot after flash)
bool checkFirstBoot() {
  File f = LittleFS.open(FW_VERSION_PATH, "r");
  if (f) {
    String stored = f.readStringUntil('\n');
    stored.trim();
    f.close();
    if (stored == FW_VERSION) {
      return false;  // same firmware — normal boot
    }
  }

  // New firmware (or first ever boot) — apply factory defaults
  Serial.println("First boot detected — applying factory defaults");

  // Write new version
  f = LittleFS.open(FW_VERSION_PATH, "w");
  if (f) { f.println(FW_VERSION); f.close(); }

  // Delete user config so defaults take effect
  if (LittleFS.exists(USER_CONFIG_PATH)) {
    LittleFS.remove(USER_CONFIG_PATH);
  }

  applyFactoryDefaults();
  return true;
}

// Load user config from LittleFS (key=value text file)
void loadUserConfig() {
  File f = LittleFS.open(USER_CONFIG_PATH, "r");
  if (!f) {
    // No user config — use current runtime values (factory defaults or hardcoded)
    Serial.println("No user config — using defaults");
    return;
  }

  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    int eq = line.indexOf('=');
    if (eq < 0) continue;
    String key = line.substring(0, eq);
    int val = line.substring(eq + 1).toInt();

    if (key == "f1ratio")      flavor1Ratio = val;
    else if (key == "f2ratio") flavor2Ratio = val;
    else if (key == "f1image") flavor1Image = val;
    else if (key == "f2image") flavor2Image = val;
  }
  f.close();

  flavors[0].ratio = flavor1Ratio;
  flavors[1].ratio = flavor2Ratio;

  Serial.printf("Config loaded: F1 ratio=%d image=%d, F2 ratio=%d image=%d\n",
                flavor1Ratio, flavor1Image, flavor2Ratio, flavor2Image);
}

// Save user config to LittleFS
void saveUserConfig() {
  File f = LittleFS.open(USER_CONFIG_PATH, "w");
  if (!f) {
    Serial.println("ERROR: failed to write user config");
    return;
  }
  f.printf("f1ratio=%d\n", flavor1Ratio);
  f.printf("f2ratio=%d\n", flavor2Ratio);
  f.printf("f1image=%d\n", flavor1Image);
  f.printf("f2image=%d\n", flavor2Image);
  f.close();
  Serial.println("Config saved to LittleFS");
}

// ════════════════════════════════════════════════════════════
//  CRC-16/CCITT for binary protocol with RP2040
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

static uint32_t crc32_update(uint32_t prev, const uint8_t *data, size_t len) {
  uint32_t crc = ~prev;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }
  }
  return ~crc;
}

// ════════════════════════════════════════════════════════════
//  Query RP2040 image count (binary protocol)
// ════════════════════════════════════════════════════════════

bool queryImageCount() {
  stRP.sendData(0, PKT_QUERY_COUNT);

  unsigned long start = millis();
  while (millis() - start < 500) {
    if (stRP.available()) {
      if (stRP.currentPacketID() == PKT_RESP_COUNT) {
        ResponsePayload resp;
        stRP.rxObj(resp);
        numImages = resp.value;
        Serial.printf("RP2040 reports %d images\n", numImages);
        return true;
      }
    }
  }
  return false;
}

// ════════════════════════════════════════════════════════════
//  Wait for a specific SerialTransfer response packet
// ════════════════════════════════════════════════════════════

bool waitStResponse(SerialTransfer &st, uint8_t expectedPktId, unsigned long timeoutMs, ResponsePayload *out = nullptr) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (st.available()) {
      if (st.currentPacketID() == expectedPktId) {
        if (out) st.rxObj(*out);
        return true;
      }
      // Unexpected packet — could be an error
      return false;
    }
  }
  return false;
}

// ════════════════════════════════════════════════════════════
//  Query S3 image count via SerialTransfer
// ════════════════════════════════════════════════════════════

bool queryS3ImageCount() {
  stS3.sendData(0, PKT_QUERY_COUNT);

  unsigned long start = millis();
  while (millis() - start < 500) {
    if (stS3.available()) {
      if (stS3.currentPacketID() == PKT_RESP_COUNT) {
        ResponsePayload resp;
        stS3.rxObj(resp);
        numS3Images = resp.value;
        Serial.printf("S3 reports %d images\n", numS3Images);
        return true;
      }
    }
  }
  return false;
}

// ════════════════════════════════════════════════════════════
//  ESP32 image store utilities (LittleFS)
// ════════════════════════════════════════════════════════════

String espRpPath(uint8_t slot) {
  char buf[16];
  snprintf(buf, sizeof(buf), "/rp_img%02d.bin", slot);
  return String(buf);
}

String espS3Path(uint8_t slot) {
  char buf[16];
  snprintf(buf, sizeof(buf), "/s3_img%02d.bin", slot);
  return String(buf);
}

String espS3PngPath(uint8_t slot) {
  char buf[24];
  snprintf(buf, sizeof(buf), "/s3_png%02d.png", slot);
  return String(buf);
}

void readEspMeta() {
  File f = LittleFS.open(ESP_META_PATH, "r");
  if (f) {
    espNumImages = f.parseInt();
    f.close();
  }
}

void saveEspMeta() {
  File f = LittleFS.open(ESP_META_PATH, "w");
  if (f) {
    f.println(espNumImages);
    f.close();
  }
}

void loadEspLabels() {
  memset(espLabels, 0, sizeof(espLabels));
  File f = LittleFS.open(ESP_LABELS_PATH, "r");
  if (!f) return;
  uint8_t i = 0;
  while (f.available() && i < MAX_STORE_IMAGES) {
    String line = f.readStringUntil('\n');
    line.trim();
    strncpy(espLabels[i], line.c_str(), MAX_LABEL_LEN);
    i++;
  }
  f.close();
}

void saveEspLabels() {
  File f = LittleFS.open(ESP_LABELS_PATH, "w");
  if (!f) return;
  for (uint8_t i = 0; i < espNumImages; i++) {
    f.println(espLabels[i]);
  }
  f.close();
}

// ════════════════════════════════════════════════════════════
//  Store mode: receive binary upload to ESP32 LittleFS
// ════════════════════════════════════════════════════════════

void enterStoreMode(bool isS3, uint8_t slot, bool isPng = false) {
  String path = isPng ? espS3PngPath(slot) : (isS3 ? espS3Path(slot) : espRpPath(slot));
  uint32_t expectedSize = isPng ? 0 : (isS3 ? S3_IMAGE_BYTES : RP2040_IMAGE_BYTES);
  Serial.printf("Store mode: %s (%s)\n", path.c_str(),
                isPng ? "variable size" : String(expectedSize).c_str());

  uint8_t buf[256];
  uint16_t bufLen = 0;
  unsigned long lastActivity = millis();
  File f;
  bool fileOpen = false;
  uint32_t receivedBytes = 0;
  uint32_t runCrc = 0;

  while (millis() - lastActivity < 5000) {
    while (Serial.available()) {
      if (bufLen < sizeof(buf)) {
        buf[bufLen++] = Serial.read();
      } else {
        Serial.read();  // overflow discard
      }
      lastActivity = millis();
    }

    // Find STX STX
    int stxIdx = -1;
    for (int i = 0; i < (int)bufLen - 1; i++) {
      if (buf[i] == 0x02 && buf[i + 1] == 0x02) { stxIdx = i; break; }
    }
    if (stxIdx < 0) continue;
    if (stxIdx > 0) {
      memmove(buf, buf + stxIdx, bufLen - stxIdx);
      bufLen -= stxIdx;
    }

    if (bufLen < 4) continue;
    uint8_t cmd = buf[2];

    if (cmd == CMD_UPLOAD_START) {
      // STX STX 0x01 slot size(4B) CRC16 = 10 bytes
      if (bufLen < 10) continue;
      uint16_t rxCrc = buf[8] | (buf[9] << 8);
      if (crc16(buf, 8) != rxCrc) { bufLen = 0; continue; }

      uint32_t size;
      memcpy(&size, buf + 4, 4);
      if (expectedSize > 0 && size != expectedSize) {
        Serial.printf("Store: wrong size %lu, expected %lu\n", size, expectedSize);
        break;
      }

      f = LittleFS.open(path, "w");
      if (!f) { Serial.println("Store: failed to open file"); break; }
      fileOpen = true;
      receivedBytes = 0;
      runCrc = 0;

      // Send RESP_READY
      uint8_t resp[6];
      resp[0] = 0x02; resp[1] = 0x02;
      resp[2] = RESP_READY; resp[3] = slot;
      uint16_t c = crc16(resp, 4);
      resp[4] = c & 0xFF; resp[5] = (c >> 8) & 0xFF;
      Serial.write(resp, 6);

      memmove(buf, buf + 10, bufLen - 10);
      bufLen -= 10;

    } else if (cmd == CMD_CHUNK_DATA) {
      // STX STX 0x02 seq len(2B) data CRC16
      if (bufLen < 7) continue;
      uint16_t dataLen = buf[4] | (buf[5] << 8);
      uint16_t totalLen = 6 + dataLen + 2;
      if (bufLen < totalLen) continue;

      uint16_t rxCrc = buf[totalLen - 2] | (buf[totalLen - 1] << 8);
      if (crc16(buf, totalLen - 2) != rxCrc) {
        memmove(buf, buf + totalLen, bufLen - totalLen);
        bufLen -= totalLen;
        continue;
      }

      if (fileOpen) {
        f.write(buf + 6, dataLen);
        runCrc = crc32_update(runCrc, buf + 6, dataLen);
        receivedBytes += dataLen;
      }

      // Send RESP_CHUNK_OK
      uint8_t resp[6];
      resp[0] = 0x02; resp[1] = 0x02;
      resp[2] = RESP_CHUNK_OK; resp[3] = buf[3];
      uint16_t c = crc16(resp, 4);
      resp[4] = c & 0xFF; resp[5] = (c >> 8) & 0xFF;
      Serial.write(resp, 6);

      memmove(buf, buf + totalLen, bufLen - totalLen);
      bufLen -= totalLen;

    } else if (cmd == CMD_UPLOAD_DONE) {
      // STX STX 0x03 slot crc32(4B) CRC16 = 10 bytes
      if (bufLen < 10) continue;
      uint16_t rxCrc = buf[8] | (buf[9] << 8);
      if (crc16(buf, 8) != rxCrc) { bufLen = 0; continue; }

      uint32_t expectedCrc;
      memcpy(&expectedCrc, buf + 4, 4);

      if (fileOpen) { f.close(); fileOpen = false; }

      bool crcOk = (runCrc == expectedCrc);

      // Update slot count (only for image files, not PNGs)
      if (!isPng && slot >= espNumImages) {
        espNumImages = slot + 1;
        saveEspMeta();
      }

      // Send RESP_UPLOAD_OK
      uint8_t resp[6];
      resp[0] = 0x02; resp[1] = 0x02;
      resp[2] = RESP_UPLOAD_OK; resp[3] = espNumImages;
      uint16_t c = crc16(resp, 4);
      resp[4] = c & 0xFF; resp[5] = (c >> 8) & 0xFF;
      Serial.write(resp, 6);

      Serial.printf("Store done: %s, %lu bytes, CRC %s\n",
                    path.c_str(), receivedBytes, crcOk ? "OK" : "MISMATCH");
      break;

    } else {
      // Unknown command byte, skip past the STX STX
      memmove(buf, buf + 2, bufLen - 2);
      bufLen -= 2;
    }
  }

  if (fileOpen) f.close();
}

// ════════════════════════════════════════════════════════════
//  Push images from ESP32 store to display devices
// ════════════════════════════════════════════════════════════

enum DeviceTarget { DEVICE_RP2040, DEVICE_S3 };

bool pushImageToDevice(DeviceTarget dev, uint8_t slot) {
  const char *devName = (dev == DEVICE_RP2040) ? "RP2040" : "S3";
  String path = (dev == DEVICE_RP2040) ? espRpPath(slot) : espS3Path(slot);
  uint32_t expectedSize = (dev == DEVICE_RP2040) ? RP2040_IMAGE_BYTES : S3_IMAGE_BYTES;

  File f = LittleFS.open(path, "r");
  if (!f) {
    Serial.printf("Push %s: %s not found\n", devName, path.c_str());
    return false;
  }
  if ((uint32_t)f.size() != expectedSize) {
    Serial.printf("Push %s: wrong size %d\n", devName, (int)f.size());
    f.close();
    return false;
  }

  uint8_t chunkBuf[STORE_CHUNK_SIZE];
  uint8_t seq = 0;
  uint32_t runCrc = 0;

  if (dev == DEVICE_RP2040) {
    // ── SerialTransfer path ──
    UploadStartPayload startPl{slot, expectedSize};
    stRP.txObj(startPl);
    stRP.sendData(sizeof(startPl), PKT_UPLOAD_START);

    if (!waitStResponse(stRP, PKT_RESP_READY, 3000)) {
      Serial.printf("Push %s: not ready\n", devName);
      f.close();
      return false;
    }

    while (f.available()) {
      int n = f.read(chunkBuf, STORE_CHUNK_SIZE);
      if (n <= 0) break;

      stRP.packet.txBuff[0] = seq;
      memcpy(stRP.packet.txBuff + 1, chunkBuf, n);

      bool ok = false;
      for (int attempt = 0; attempt < 5; attempt++) {
        stRP.sendData(1 + n, PKT_CHUNK_DATA);
        if (waitStResponse(stRP, PKT_RESP_CHUNK_OK, 2000)) { ok = true; break; }
        Serial.printf("Push %s: chunk %d retry %d\n", devName, seq, attempt + 1);
      }
      if (!ok) {
        Serial.printf("Push %s: chunk %d failed\n", devName, seq);
        f.close();
        return false;
      }

      runCrc = uartCrc32Update(runCrc, chunkBuf, n);
      seq++;
    }
    f.close();

    UploadDonePayload donePl{slot, runCrc};
    stRP.txObj(donePl);
    stRP.sendData(sizeof(donePl), PKT_UPLOAD_DONE);

    if (!waitStResponse(stRP, PKT_RESP_UPLOAD_OK, 5000)) {
      Serial.printf("Push %s: verification failed\n", devName);
      return false;
    }

  } else {
    // ── SerialTransfer path for S3 ──
    UploadStartPayload startPl{slot, expectedSize};
    stS3.txObj(startPl);
    stS3.sendData(sizeof(startPl), PKT_UPLOAD_START);

    if (!waitStResponse(stS3, PKT_RESP_READY, 3000)) {
      Serial.printf("Push %s: not ready\n", devName);
      f.close();
      return false;
    }

    while (f.available()) {
      int n = f.read(chunkBuf, STORE_CHUNK_SIZE);
      if (n <= 0) break;

      stS3.packet.txBuff[0] = seq;
      memcpy(stS3.packet.txBuff + 1, chunkBuf, n);

      bool ok = false;
      for (int attempt = 0; attempt < 5; attempt++) {
        stS3.sendData(1 + n, PKT_CHUNK_DATA);
        if (waitStResponse(stS3, PKT_RESP_CHUNK_OK, 2000)) { ok = true; break; }
        Serial.printf("Push %s: chunk %d retry %d\n", devName, seq, attempt + 1);
      }
      if (!ok) {
        Serial.printf("Push %s: chunk %d failed\n", devName, seq);
        f.close();
        return false;
      }

      runCrc = uartCrc32Update(runCrc, chunkBuf, n);
      seq++;
    }
    f.close();

    UploadDonePayload donePl{slot, runCrc};
    stS3.txObj(donePl);
    stS3.sendData(sizeof(donePl), PKT_UPLOAD_DONE);

    if (!waitStResponse(stS3, PKT_RESP_UPLOAD_OK, 5000)) {
      Serial.printf("Push %s: verification failed\n", devName);
      return false;
    }
  }

  Serial.printf("Push %s: slot %d OK\n", devName, slot);
  return true;
}

bool pushPngToS3(uint8_t slot) {
  String path = espS3PngPath(slot);
  File f = LittleFS.open(path, "r");
  if (!f) {
    Serial.printf("Push S3 PNG: %s not found (skipping)\n", path.c_str());
    return true;  // not fatal — PNG is optional
  }
  uint32_t fileSize = f.size();
  if (fileSize == 0 || fileSize > S3_IMAGE_BYTES) {
    Serial.printf("Push S3 PNG: bad size %lu\n", fileSize);
    f.close();
    return false;
  }

  // Send PKT_UPLOAD_PNG_START (0x07) — replaces slot+100 hack
  UploadStartPayload startPl{slot, fileSize};
  stS3.txObj(startPl);
  stS3.sendData(sizeof(startPl), PKT_UPLOAD_PNG_START);

  if (!waitStResponse(stS3, PKT_RESP_READY, 3000)) {
    Serial.printf("Push S3 PNG: not ready\n");
    f.close();
    return false;
  }

  uint8_t chunkBuf[STORE_CHUNK_SIZE];
  uint8_t seq = 0;
  uint32_t runCrc = 0;

  while (f.available()) {
    int n = f.read(chunkBuf, STORE_CHUNK_SIZE);
    if (n <= 0) break;

    stS3.packet.txBuff[0] = seq;
    memcpy(stS3.packet.txBuff + 1, chunkBuf, n);

    bool ok = false;
    for (int attempt = 0; attempt < 5; attempt++) {
      stS3.sendData(1 + n, PKT_CHUNK_DATA);
      if (waitStResponse(stS3, PKT_RESP_CHUNK_OK, 2000)) { ok = true; break; }
      Serial.printf("Push S3 PNG: chunk %d retry %d\n", seq, attempt + 1);
    }
    if (!ok) {
      Serial.printf("Push S3 PNG: chunk %d failed\n", seq);
      f.close();
      return false;
    }

    runCrc = uartCrc32Update(runCrc, chunkBuf, n);
    seq++;
  }
  f.close();

  UploadDonePayload donePl{slot, runCrc};
  stS3.txObj(donePl);
  stS3.sendData(sizeof(donePl), PKT_UPLOAD_DONE);

  if (!waitStResponse(stS3, PKT_RESP_UPLOAD_OK, 5000)) {
    Serial.printf("Push S3 PNG: verification failed\n");
    return false;
  }

  Serial.printf("Push S3 PNG: slot %d OK (%lu bytes)\n", slot, fileSize);
  return true;
}

void pushLabelsToDevice(DeviceTarget dev) {
  for (uint8_t i = 0; i < espNumImages; i++) {
    char lbuf[48];
    snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
    if (dev == DEVICE_RP2040) {
      stSendText(stRP, lbuf);
    } else {
      stSendText(stS3, lbuf);
    }
    delay(50);
  }
}

// Delete the last image slot on a device (PKT_DELETE_IMAGE).
// Device handlers shift files down, so always deleting the last slot
// avoids disrupting lower slots we just pushed.
bool deleteLastDeviceImage(DeviceTarget dev, uint8_t slot) {
  const char *devName = (dev == DEVICE_RP2040) ? "RP2040" : "S3";

  if (dev == DEVICE_RP2040) {
    SlotPayload sp{slot};
    stRP.txObj(sp);
    stRP.sendData(sizeof(sp), PKT_DELETE_IMAGE);

    if (!waitStResponse(stRP, PKT_RESP_DELETE_OK, 3000)) {
      Serial.printf("Delete %s slot %d: failed\n", devName, slot);
      return false;
    }
  } else {
    SlotPayload sp{slot};
    stS3.txObj(sp);
    stS3.sendData(sizeof(sp), PKT_DELETE_IMAGE);

    if (!waitStResponse(stS3, PKT_RESP_DELETE_OK, 3000)) {
      Serial.printf("Delete %s slot %d: failed\n", devName, slot);
      return false;
    }
  }

  Serial.printf("Delete %s slot %d: OK\n", devName, slot);
  return true;
}

bool pushAllToDevice(DeviceTarget dev) {
  const char *devName = (dev == DEVICE_RP2040) ? "RP2040" : "S3";
  Serial.printf("Pushing %d images to %s...\n", espNumImages, devName);

  for (uint8_t i = 0; i < espNumImages; i++) {
    Serial.printf("  Slot %d/%d...\n", i + 1, espNumImages);
    if (!pushImageToDevice(dev, i)) return false;
  }

  // Delete extra slots beyond espNumImages.  After pushing, the device's
  // countImages() still sees old files in higher slots.  Repeatedly delete
  // slot espNumImages: the device shifts higher files down each time, so
  // this converges.  Stops when the device rejects (count already matches).
  while (deleteLastDeviceImage(dev, espNumImages)) {
    // keep trimming until device's count == espNumImages
  }

  // Push compressed PNGs to S3 (for iOS BLE image serving)
  if (dev == DEVICE_S3) {
    for (uint8_t i = 0; i < espNumImages; i++) {
      pushPngToS3(i);
    }
  }

  pushLabelsToDevice(dev);
  Serial.printf("Push to %s complete\n", devName);
  return true;
}

// ════════════════════════════════════════════════════════════
//  Boot sync: compare store with devices, push if needed
// ════════════════════════════════════════════════════════════

void bootSync() {
  if (!LittleFS.exists(ESP_META_PATH)) {
    Serial.println("Store not initialized — skipping boot sync");
    return;
  }
  readEspMeta();
  loadEspLabels();

  if (espNumImages == 0) {
    Serial.println("Store empty — skipping boot sync");
    return;
  }

  Serial.printf("Boot sync: store has %d images\n", espNumImages);

  // RP2040: count mismatch → full re-upload
  if (numImages != espNumImages) {
    Serial.printf("RP2040 mismatch: %d vs %d — pushing all\n", numImages, espNumImages);
    if (pushAllToDevice(DEVICE_RP2040)) numImages = espNumImages;
  } else {
    Serial.println("RP2040 in sync");
  }

  // S3: count mismatch → full re-upload
  if (numS3Images != espNumImages) {
    Serial.printf("S3 mismatch: %d vs %d — pushing all\n", numS3Images, espNumImages);
    if (pushAllToDevice(DEVICE_S3)) numS3Images = espNumImages;
  } else {
    Serial.println("S3 RGB565 in sync — ensuring PNGs exist");
    for (uint8_t i = 0; i < espNumImages; i++) {
      pushPngToS3(i);
    }
  }
}

// ════════════════════════════════════════════════════════════
//  Config UART command parser
// ════════════════════════════════════════════════════════════

void sendConfigResponse(Stream &out) {
  out.printf("CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d,numS3Images=%d\n",
             flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numImages, numS3Images);
}

void processConfigCommand(const char *cmd, Stream &out) {
  if (strcmp(cmd, "GET_CONFIG") == 0) {
    sendConfigResponse(out);

  } else if (strncmp(cmd, "SET:", 4) == 0) {
    const char *param = cmd + 4;
    char key[16];
    int val;
    if (sscanf(param, "%15[^=]=%d", key, &val) == 2) {
      bool ok = false;
      if (strcmp(key, "F1_RATIO") == 0 && val >= 6 && val <= 24) {
        flavor1Ratio = val; flavors[0].ratio = val; ok = true;
      } else if (strcmp(key, "F2_RATIO") == 0 && val >= 6 && val <= 24) {
        flavor2Ratio = val; flavors[1].ratio = val; ok = true;
      } else if (strcmp(key, "F1_IMAGE") == 0) {
        uint8_t maxImg = min(numImages, numS3Images);
        if (val >= 0 && val < maxImg) {
          flavor1Image = val; ok = true;
          sendMapToRP();
        }
      } else if (strcmp(key, "F2_IMAGE") == 0) {
        uint8_t maxImg = min(numImages, numS3Images);
        if (val >= 0 && val < maxImg) {
          flavor2Image = val; ok = true;
          sendMapToRP();
        }
      }

      if (ok) {
        out.printf("OK:%s=%d\n", key, val);
        Serial.printf("Config SET: %s=%d\n", key, val);
      } else {
        out.printf("ERR:%s out of range\n", key);
      }
    }

  } else if (strcmp(cmd, "SAVE") == 0) {
    saveUserConfig();
    out.printf("OK:SAVED\n");

    // Push updated config to S3 so it can forward to BLE (iOS app)
    // This eliminates the need for iOS to poll GET_CONFIG every 2 seconds
    {
      char cfgBuf[128];
      snprintf(cfgBuf, sizeof(cfgBuf),
               "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d,numS3Images=%d",
               flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numImages, numS3Images);
      stSendText(stS3, cfgBuf);
    }

  } else if (strcmp(cmd, "QUERY_IMAGES") == 0) {
    queryImageCount();
    out.printf("OK:NUM_IMAGES=%d\n", numImages);

  } else if (strcmp(cmd, "LIST_IMAGES") == 0) {
    // Send LIST to RP2040 via SerialTransfer, read PKT_TEXT responses
    stSendText(stRP, "LIST");

    unsigned long t = millis();
    while (millis() - t < 2000) {
      if (stRP.available()) {
        if (stRP.currentPacketID() == PKT_TEXT) {
          uint16_t len = stRP.bytesRead;
          char line[256];
          uint16_t copyLen = (len < 255) ? len : 255;
          memcpy(line, stRP.packet.rxBuff, copyLen);
          line[copyLen] = '\0';
          if (strcmp(line, "END") == 0) break;
          out.println(line);
          t = millis();
        }
      }
    }
    out.println("END");

  } else if (strncmp(cmd, "SET_LABEL:", 10) == 0) {
    // SET_LABEL:slot=name → forward LABEL:slot:name to RP2040
    int slot;
    char name[33] = {0};
    if (sscanf(cmd + 10, "%d=%32[^\n]", &slot, name) >= 1) {
      if (slot >= 0 && slot < numImages) {
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); stSendText(stRP, lbuf); }
        out.printf("OK:LABEL=%d:%s\n", slot, name);
      } else {
        out.printf("ERR:invalid slot\n");
      }
    }

  } else if (strncmp(cmd, "DELETE_IMG:", 11) == 0) {
    int slot = atoi(cmd + 11);
    if (slot < 0 || slot >= numImages) {
      out.printf("ERR:invalid slot (0-%d)\n", numImages - 1);
      return;
    }
    if (numImages <= 1) {
      out.printf("ERR:cannot delete last image\n");
      return;
    }

    SlotPayload delPl{(uint8_t)slot};
    stRP.txObj(delPl);
    stRP.sendData(sizeof(delPl), PKT_DELETE_IMAGE);

    ResponsePayload rp;
    if (waitStResponse(stRP, PKT_RESP_DELETE_OK, 3000, &rp)) {
      numImages = rp.value;
      // Adjust ESP32-side image references
      if (flavor1Image == slot) flavor1Image = 0;
      else if (flavor1Image > slot) flavor1Image--;
      if (flavor2Image == slot) flavor2Image = 0;
      else if (flavor2Image > slot) flavor2Image--;
      if (flavor1Image >= numImages) flavor1Image = 0;
      if (flavor2Image >= numImages) flavor2Image = 0;
      sendMapToRP();
      out.printf("OK:DELETED=%d,NUM_IMAGES=%d\n", slot, numImages);
    } else {
      out.printf("ERR:delete failed\n");
    }

  } else if (strncmp(cmd, "SWAP_IMG:", 9) == 0) {
    int a, b;
    if (sscanf(cmd + 9, "%d,%d", &a, &b) != 2) {
      out.printf("ERR:usage SWAP_IMG:A,B\n");
      return;
    }
    if (a < 0 || a >= numImages || b < 0 || b >= numImages) {
      out.printf("ERR:invalid slots (0-%d)\n", numImages - 1);
      return;
    }

    SwapPayload swPl{(uint8_t)a, (uint8_t)b};
    stRP.txObj(swPl);
    stRP.sendData(sizeof(swPl), PKT_SWAP_IMAGES);

    if (waitStResponse(stRP, PKT_RESP_SWAP_OK, 3000)) {
      // Adjust ESP32-side image references
      if (flavor1Image == a) flavor1Image = b;
      else if (flavor1Image == b) flavor1Image = a;
      if (flavor2Image == a) flavor2Image = b;
      else if (flavor2Image == b) flavor2Image = a;
      sendMapToRP();
      out.printf("OK:SWAPPED=%d,%d\n", a, b);
    } else {
      out.printf("ERR:swap failed\n");
    }

  // ── S3 image management commands ──────────────────────────

  } else if (strcmp(cmd, "QUERY_S3_IMAGES") == 0) {
    queryS3ImageCount();
    out.printf("OK:NUM_S3_IMAGES=%d\n", numS3Images);

  } else if (strcmp(cmd, "LIST_S3_IMAGES") == 0) {
    stSendText(stS3, "LIST");

    unsigned long t = millis();
    while (millis() - t < 2000) {
      if (stS3.available()) {
        if (stS3.currentPacketID() == PKT_TEXT) {
          uint16_t len = stS3.bytesRead;
          char line[256];
          uint16_t copyLen = (len < 255) ? len : 255;
          memcpy(line, stS3.packet.rxBuff, copyLen);
          line[copyLen] = '\0';
          if (strcmp(line, "END") == 0) break;
          out.println(line);
          t = millis();
        }
      }
    }
    out.println("END");

  } else if (strcmp(cmd, "LIST_S3_PNGS") == 0) {
    stSendText(stS3, "LISTPNGS");

    unsigned long t = millis();
    while (millis() - t < 2000) {
      if (stS3.available()) {
        if (stS3.currentPacketID() == PKT_TEXT) {
          uint16_t len = stS3.bytesRead;
          char line[256];
          uint16_t copyLen = (len < 255) ? len : 255;
          memcpy(line, stS3.packet.rxBuff, copyLen);
          line[copyLen] = '\0';
          if (strcmp(line, "END") == 0) break;
          out.println(line);
          t = millis();
        }
      }
    }
    out.println("END");

  } else if (strncmp(cmd, "SET_S3_LABEL:", 13) == 0) {
    int slot;
    char name[33] = {0};
    if (sscanf(cmd + 13, "%d=%32[^\n]", &slot, name) >= 1) {
      if (slot >= 0 && slot < numS3Images) {
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); stSendText(stS3, lbuf); }
        out.printf("OK:S3_LABEL=%d:%s\n", slot, name);
      } else {
        out.printf("ERR:invalid slot\n");
      }
    }

  } else if (strncmp(cmd, "DELETE_S3_IMG:", 14) == 0) {
    int slot = atoi(cmd + 14);
    if (slot < 0 || slot >= numS3Images) {
      out.printf("ERR:invalid slot (0-%d)\n", numS3Images - 1);
      return;
    }
    if (numS3Images <= 1) {
      out.printf("ERR:cannot delete last image\n");
      return;
    }

    SlotPayload sp{(uint8_t)slot};
    stS3.txObj(sp);
    stS3.sendData(sizeof(sp), PKT_DELETE_IMAGE);

    ResponsePayload rp;
    if (waitStResponse(stS3, PKT_RESP_DELETE_OK, 3000, &rp)) {
      numS3Images = rp.value;
      out.printf("OK:S3_DELETED=%d,NUM_S3_IMAGES=%d\n", slot, numS3Images);
    } else {
      out.printf("ERR:s3 delete failed\n");
    }

  } else if (strncmp(cmd, "SWAP_S3_IMG:", 12) == 0) {
    int a, b;
    if (sscanf(cmd + 12, "%d,%d", &a, &b) != 2) {
      out.printf("ERR:usage SWAP_S3_IMG:A,B\n");
      return;
    }
    if (a < 0 || a >= numS3Images || b < 0 || b >= numS3Images) {
      out.printf("ERR:invalid slots (0-%d)\n", numS3Images - 1);
      return;
    }

    SwapPayload sp{(uint8_t)a, (uint8_t)b};
    stS3.txObj(sp);
    stS3.sendData(sizeof(sp), PKT_SWAP_IMAGES);

    if (waitStResponse(stS3, PKT_RESP_SWAP_OK, 3000)) {
      out.printf("OK:S3_SWAPPED=%d,%d\n", a, b);
    } else {
      out.printf("ERR:s3 swap failed\n");
    }

  // ── ESP32 image store commands ────────────────────────────

  } else if (strncmp(cmd, "STORE_RP_IMG:", 13) == 0) {
    int slot = atoi(cmd + 13);
    if (slot < 0 || slot > MAX_STORE_IMAGES - 1) {
      out.printf("ERR:invalid slot\n");
      return;
    }
    out.printf("OK:STORE_MODE\n");
    out.flush();
    enterStoreMode(false, slot);

  } else if (strncmp(cmd, "STORE_S3_IMG:", 13) == 0) {
    int slot = atoi(cmd + 13);
    if (slot < 0 || slot > MAX_STORE_IMAGES - 1) {
      out.printf("ERR:invalid slot\n");
      return;
    }
    out.printf("OK:STORE_MODE\n");
    out.flush();
    enterStoreMode(true, slot);

  } else if (strncmp(cmd, "STORE_S3_PNG:", 13) == 0) {
    int slot = atoi(cmd + 13);
    if (slot < 0 || slot > MAX_STORE_IMAGES - 1) {
      out.printf("ERR:invalid slot\n");
      return;
    }
    out.printf("OK:STORE_MODE\n");
    out.flush();
    enterStoreMode(true, slot, true);

  } else if (strncmp(cmd, "STORE_LABEL:", 12) == 0) {
    int slot;
    char name[MAX_LABEL_LEN + 1] = {0};
    if (sscanf(cmd + 12, "%d=%32[^\n]", &slot, name) >= 1) {
      if (slot >= 0 && slot < espNumImages) {
        strncpy(espLabels[slot], name, MAX_LABEL_LEN);
        espLabels[slot][MAX_LABEL_LEN] = '\0';
        saveEspLabels();
        // Forward to both devices
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); stSendText(stRP, lbuf); }
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); stSendText(stS3, lbuf); }
        out.printf("OK:STORE_LABEL=%d:%s\n", slot, name);
      } else {
        out.printf("ERR:invalid slot\n");
      }
    }

  } else if (strncmp(cmd, "DELETE_STORE_IMG:", 16) == 0) {
    int slot = atoi(cmd + 16);
    if (slot < 0 || slot >= espNumImages) {
      out.printf("ERR:invalid slot (0-%d)\n", espNumImages - 1);
      return;
    }
    if (espNumImages <= 1) {
      out.printf("ERR:cannot delete last image\n");
      return;
    }

    // Delete from ESP32 LittleFS (RGB565 + PNG)
    LittleFS.remove(espRpPath(slot));
    LittleFS.remove(espS3Path(slot));
    LittleFS.remove(espS3PngPath(slot));

    // Shift remaining slots down
    for (int i = slot + 1; i < espNumImages; i++) {
      LittleFS.rename(espRpPath(i), espRpPath(i - 1));
      LittleFS.rename(espS3Path(i), espS3Path(i - 1));
      String pngFrom = espS3PngPath(i);
      if (LittleFS.exists(pngFrom)) {
        LittleFS.rename(pngFrom, espS3PngPath(i - 1));
      }
      strncpy(espLabels[i - 1], espLabels[i], MAX_LABEL_LEN);
    }
    espNumImages--;
    espLabels[espNumImages][0] = '\0';
    saveEspMeta();
    saveEspLabels();

    // Forward delete to RP2040 (SerialTransfer)
    {
      SlotPayload sp{(uint8_t)slot};
      stRP.txObj(sp);
      stRP.sendData(sizeof(sp), PKT_DELETE_IMAGE);
      ResponsePayload rp;
      if (waitStResponse(stRP, PKT_RESP_DELETE_OK, 3000, &rp))
        numImages = rp.value;
    }
    // Forward delete to S3 (SerialTransfer)
    {
      SlotPayload sp{(uint8_t)slot};
      stS3.txObj(sp);
      stS3.sendData(sizeof(sp), PKT_DELETE_IMAGE);
      ResponsePayload rp;
      if (waitStResponse(stS3, PKT_RESP_DELETE_OK, 3000, &rp))
        numS3Images = rp.value;
    }

    // Adjust flavor image references
    if (flavor1Image == slot) flavor1Image = 0;
    else if (flavor1Image > slot) flavor1Image--;
    if (flavor2Image == slot) flavor2Image = 0;
    else if (flavor2Image > slot) flavor2Image--;
    if (flavor1Image >= espNumImages) flavor1Image = 0;
    if (flavor2Image >= espNumImages) flavor2Image = 0;
    sendMapToRP();
    out.printf("OK:STORE_DELETED=%d,NUM_IMAGES=%d\n", slot, espNumImages);

  } else if (strcmp(cmd, "LIST_STORE") == 0) {
    out.printf("STORE_COUNT:%d\n", espNumImages);
    for (uint8_t i = 0; i < espNumImages; i++) {
      const char *lbl = espLabels[i][0] ? espLabels[i] : "";
      bool rpOk = LittleFS.exists(espRpPath(i));
      bool s3Ok = LittleFS.exists(espS3Path(i));
      bool pngOk = LittleFS.exists(espS3PngPath(i));
      out.printf("STORE:%d:%s:rp=%s:s3=%s:png=%s\n", i, lbl,
                 rpOk ? "ok" : "missing", s3Ok ? "ok" : "missing",
                 pngOk ? "ok" : "missing");
    }
    out.println("END");

  } else if (strcmp(cmd, "PUSH_TO_DEVICES") == 0) {
    if (espNumImages == 0) {
      out.printf("ERR:store empty\n");
      return;
    }
    out.printf("OK:PUSHING %d images\n", espNumImages);
    out.flush();
    bool rpOk = pushAllToDevice(DEVICE_RP2040);
    bool s3Ok = pushAllToDevice(DEVICE_S3);
    if (rpOk) numImages = espNumImages;
    if (s3Ok) numS3Images = espNumImages;
    sendMapToRP();
    out.printf("OK:PUSH_DONE rp=%s s3=%s\n",
               rpOk ? "ok" : "fail", s3Ok ? "ok" : "fail");

  } else if (strcmp(cmd, "SYNC_DEVICES") == 0) {
    if (espNumImages == 0) {
      out.printf("ERR:store empty\n");
      return;
    }
    out.printf("OK:SYNCING\n");
    out.flush();
    bool rpPushed = false, s3Pushed = false;
    if (numImages != espNumImages) {
      rpPushed = pushAllToDevice(DEVICE_RP2040);
      if (rpPushed) numImages = espNumImages;
    }
    if (numS3Images != espNumImages) {
      s3Pushed = pushAllToDevice(DEVICE_S3);
      if (s3Pushed) numS3Images = espNumImages;
    }
    sendMapToRP();
    out.printf("OK:SYNC_DONE rp=%s s3=%s\n",
               rpPushed ? "pushed" : "in_sync",
               s3Pushed ? "pushed" : "in_sync");

  } else if (strcmp(cmd, "FACTORY_RESET") == 0) {
    out.printf("OK:RESETTING\n");
    out.flush();

    // Apply compiled-in factory defaults (sets espNumImages, saves meta + labels)
    applyFactoryDefaults();

    // Delete user config so defaults persist across reboot
    if (LittleFS.exists(USER_CONFIG_PATH)) {
      LittleFS.remove(USER_CONFIG_PATH);
    }

    // Force push to both devices
    if (espNumImages > 0) {
      bool rpOk = pushAllToDevice(DEVICE_RP2040);
      bool s3Ok = pushAllToDevice(DEVICE_S3);
      if (rpOk) { numImages = espNumImages; pushLabelsToDevice(DEVICE_RP2040); }
      if (s3Ok) { numS3Images = espNumImages; pushLabelsToDevice(DEVICE_S3); }
      sendMapToRP();
      out.printf("OK:FACTORY_RESET rp=%s s3=%s\n",
                 rpOk ? "ok" : "fail", s3Ok ? "ok" : "fail");
    } else {
      sendMapToRP();
      out.printf("OK:FACTORY_RESET (no images in store)\n");
    }

  } else if (strncmp(cmd, "PUSH_IMG:", 9) == 0) {
    // PUSH_IMG:slot:target — push a single stored image to a device
    int slot;
    char target[8] = {0};
    if (sscanf(cmd + 9, "%d:%7s", &slot, target) != 2) {
      out.printf("ERR:usage PUSH_IMG:slot:target\n");
      return;
    }
    if (slot < 0 || slot >= espNumImages) {
      out.printf("ERR:invalid slot (0-%d)\n", espNumImages - 1);
      return;
    }

    bool rpOk = true, s3Ok = true;
    if (strcmp(target, "rp2040") == 0 || strcmp(target, "both") == 0) {
      rpOk = pushImageToDevice(DEVICE_RP2040, slot);
      if (rpOk) { numImages = max(numImages, (uint8_t)(slot + 1)); pushLabelsToDevice(DEVICE_RP2040); sendMapToRP(); }
    }
    if (strcmp(target, "s3") == 0 || strcmp(target, "both") == 0) {
      s3Ok = pushImageToDevice(DEVICE_S3, slot);
      if (s3Ok) {
        numS3Images = max(numS3Images, (uint8_t)(slot + 1));
        pushPngToS3(slot);
        pushLabelsToDevice(DEVICE_S3);
      }
    }
    out.printf("OK:PUSH_IMG=%d rp=%s s3=%s\n", slot,
               rpOk ? "ok" : "fail", s3Ok ? "ok" : "fail");
  }
}

// Buffer incoming bytes into lines, dispatch complete lines to processConfigCommand
#define CONFIG_BUF_SIZE 64

void checkConfigStream(Stream &stream, char *buf, uint8_t &pos) {
  while (stream.available()) {
    char c = stream.read();
    if (c == '\n' || c == '\r') {
      if (pos > 0) {
        buf[pos] = '\0';
        processConfigCommand(buf, stream);
        pos = 0;
      }
    } else if (pos < CONFIG_BUF_SIZE - 1) {
      buf[pos++] = c;
    }
  }
}

// Stream wrapper that routes printf/println output to SerialTransfer PKT_TEXT packets.
// Each line (terminated by \n) is sent as a separate packet. This lets processConfigCommand
// work unchanged — all out.printf() calls automatically become stSendText() calls.
class StStream : public Stream {
  SerialTransfer &_st;
  char _buf[256];
  uint8_t _pos;
public:
  StStream(SerialTransfer &st) : _st(st), _pos(0) {}
  size_t write(uint8_t c) override {
    if (c == '\n' || _pos >= 254) {
      if (_pos > 0) { _buf[_pos] = '\0'; stSendText(_st, _buf); _pos = 0; }
    } else if (c != '\r') {
      _buf[_pos++] = c;
    }
    return 1;
  }
  size_t write(const uint8_t *buffer, size_t size) override {
    for (size_t i = 0; i < size; i++) write(buffer[i]);
    return size;
  }
  int available() override { return 0; }
  int read() override { return -1; }
  int peek() override { return -1; }
  void flush() override { if (_pos > 0) { _buf[_pos] = '\0'; stSendText(_st, _buf); _pos = 0; } }
};

char configBuf0[CONFIG_BUF_SIZE];  // USB Serial
uint8_t configPos0 = 0;

void checkConfigUART() {
  checkConfigStream(Serial, configBuf0, configPos0);
  // Poll stS3 for PKT_TEXT commands from S3
  if (stS3.available()) {
    if (stS3.currentPacketID() == PKT_TEXT) {
      uint16_t len = stS3.bytesRead;
      char cmd[CONFIG_BUF_SIZE];
      uint16_t copyLen = (len < CONFIG_BUF_SIZE - 1) ? len : CONFIG_BUF_SIZE - 1;
      memcpy(cmd, stS3.packet.rxBuff, copyLen);
      cmd[copyLen] = '\0';
      StStream s3out(stS3);
      processConfigCommand(cmd, s3out);
      s3out.flush();
    }
  }
}

// ════════════════════════════════════════════════════════════
//  Setup
// ════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);

  // Init ESP32 image store (LittleFS) — must come before config loading
  if (!LittleFS.begin(true)) {  // true = format on first use
    Serial.println("WARNING: LittleFS init failed");
  }

  // Detect new firmware → apply factory defaults; otherwise load user config
  firstBoot = checkFirstBoot();
  loadUserConfig();

  // Init all motor pins
  const uint8_t motorPins[] = {
    A_ENA, A_IN1, A_IN2, A_IN3, A_IN4, A_ENB,
    B_ENA, B_IN1, B_IN2, B_IN3, B_IN4, B_ENB
  };
  for (uint8_t pin : motorPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Inputs
  pinMode(FLAVOR_SW_PIN, INPUT_PULLUP);
  pinMode(PRIME_BTN_PIN, INPUT_PULLUP);
  pinMode(FLOW_PIN,      INPUT_PULLUP);

  // LEDs
  pinMode(LED_FLAVOR1, OUTPUT);
  pinMode(LED_FLAVOR2, OUTPUT);

  // Flow meter interrupt
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flowPulse, FALLING);

  // Read initial flavor from switch state
  activeFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;
  digitalWrite(LED_FLAVOR1, activeFlavor == 0 ? HIGH : LOW);
  digitalWrite(LED_FLAVOR2, activeFlavor == 1 ? HIGH : LOW);

  // UART to display board (bidirectional, 38400 baud)
  Serial2.begin(38400, SERIAL_8N1, DISPLAY_RX_PIN, DISPLAY_TX_PIN);
  stRP.begin(Serial2);
  // Wait for RP2040 to boot, init LittleFS, and start UART.
  // First boot seeds 3 images (~88KB writes) which can take several seconds.
  // GP27 (RP2040 TX) is floating until pioSerial.begin() — noise on GPIO 35.
  delay(3000);

  // Query with retries (first attempt may still see noise)
  for (int attempt = 0; attempt < 3; attempt++) {
    if (queryImageCount()) break;
    Serial.printf("  retry %d/3...\n", attempt + 1);
    delay(500);
  }
  sendMapToRP();

  // UART to config display (ESP32-S3, bidirectional, 38400 baud)
  Serial1.begin(38400, SERIAL_8N1, CONFIG_RX_PIN, CONFIG_TX_PIN);
  stS3.begin(Serial1);

  // Wait for S3 to boot, init LittleFS, and start UART.
  // First boot seeds 3 images (~345KB writes) which can take several seconds.
  // COBS framing naturally rejects boot noise — no parser reset needed.
  delay(3000);

  // Query S3 image count with retries
  for (int attempt = 0; attempt < 3; attempt++) {
    if (queryS3ImageCount()) break;
    Serial.printf("  S3 retry %d/3...\n", attempt + 1);
    delay(500);
  }

  // Boot sync: force push on first boot, count-based sync otherwise
  if (firstBoot && espNumImages > 0) {
    Serial.printf("First boot — force pushing %d images to both devices\n", espNumImages);
    if (pushAllToDevice(DEVICE_RP2040)) { numImages = espNumImages; pushLabelsToDevice(DEVICE_RP2040); }
    if (pushAllToDevice(DEVICE_S3))     { numS3Images = espNumImages; pushLabelsToDevice(DEVICE_S3); }
  } else {
    bootSync();
  }

  Serial.println("Dual-Flavor Soda Maker ready!");
  Serial.printf("Active flavor: %d\n", activeFlavor + 1);
  Serial.printf("RP2040: %d images, S3: %d images\n", numImages, numS3Images);
  Serial.printf("Sent image mapping to display: %d,%d\n", flavor1Image, flavor2Image);
}

// ════════════════════════════════════════════════════════════
//  Loop
// ════════════════════════════════════════════════════════════

void loop() {
  unsigned long now = millis();

  // ── 1. Read inputs ─────────────────────────────────────────

  // Flavor switch (locked while valve is open)
  uint8_t newFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;
  if (newFlavor != activeFlavor) {
    if (!valveOpen) {
      activeFlavor = newFlavor;
      digitalWrite(LED_FLAVOR1, activeFlavor == 0 ? HIGH : LOW);
      digitalWrite(LED_FLAVOR2, activeFlavor == 1 ? HIGH : LOW);
      Serial.printf("Active flavor: %d\n", activeFlavor + 1);
    } else {
      Serial.println("Cannot toggle flavor while dispensing.");
    }
  }

  // Flow meter (every 50ms)
  if (now - lastFlowCheck >= FLOW_CHECK_INTERVAL_MS) {
    noInterrupts();
    unsigned long count = pulseCount;
    pulseCount = 0;
    interrupts();

    waterFlowing = (count >= FLOW_MIN_PULSES);
    flowPulses = count;
    lastFlowCheck = now;

    // Track readings during active pump cycles for averaging
    if (pumpState == PUMP_ON || pumpState == PUMP_OFF) {
      cyclePulseSum += count;
      cyclePulseReadings++;
      if (count == 0) cycleSawZero = true;
    }
  }

  // Prime button
  primePressed = (digitalRead(PRIME_BTN_PIN) == LOW);

  Flavor& active = flavors[activeFlavor];

  // ── 2. Pump state machine ─────────────────────────────────

  if (primePressed) {
    // Prime overrides cycling: pump runs continuously
    if (pumpState != PUMP_PRIME) {
      motorOn(active.pump, PUMP_SPEED);
      pumpState = PUMP_PRIME;
      phaseStart = now;
    }
  } else if (pumpState == PUMP_PRIME) {
    // Prime released → go idle
    motorOff(active.pump);
    pumpState = PUMP_IDLE;
  } else {
    switch (pumpState) {

      case PUMP_IDLE:
        // Waiting for flow — start a new cycle immediately
        if (flowPulses >= FLOW_MIN_PULSES) {
          computeCycleTiming(flowPulses, active.ratio, cycleOnMs, cycleOffMs);
          cyclePulseSum = 0;
          cyclePulseReadings = 0;
          cycleSawZero = false;
          motorOn(active.pump, PUMP_SPEED);
          pumpState = PUMP_ON;
          phaseStart = now;
          Serial.printf("── CYCLE START ── pulses=%lu → on=%lums off=%lums\n",
                        flowPulses, cycleOnMs, cycleOffMs);
        }
        break;

      case PUMP_ON:
        // On-phase running — wait for it to finish
        if (now - phaseStart >= cycleOnMs) {
          motorOff(active.pump);
          pumpState = PUMP_OFF;
          phaseStart = now;
          Serial.printf("── PUMP OFF  ── (on-phase done, off for %lums)\n", cycleOffMs);
        }
        break;

      case PUMP_OFF:
        // Off-phase running — full cycle complete when done
        if (now - phaseStart >= cycleOffMs) {
          unsigned long avg = (cyclePulseReadings > 0)
                              ? cyclePulseSum / cyclePulseReadings
                              : flowPulses;
          Serial.printf("── CYCLE DONE ── avg=%lu from %lu readings sawZero=%s\n",
                        avg, cyclePulseReadings, cycleSawZero ? "YES" : "NO");

          if (cycleSawZero) {
            // Flow may be stopping — enter cooldown
            pumpState = PUMP_COOLDOWN;
            phaseStart = now;
            Serial.printf("── COOLDOWN  ── pump off for %dms\n", COOLDOWN_MS);
          } else {
            // Start next cycle using the averaged flow rate
            computeCycleTiming(avg, active.ratio, cycleOnMs, cycleOffMs);
            cyclePulseSum = 0;
            cyclePulseReadings = 0;
            cycleSawZero = false;
            motorOn(active.pump, PUMP_SPEED);
            pumpState = PUMP_ON;
            phaseStart = now;
            Serial.printf("── CYCLE START ── avg=%lu → on=%lums off=%lums\n",
                          avg, cycleOnMs, cycleOffMs);
          }
        }
        break;

      case PUMP_COOLDOWN:
        // Pump off, readings discarded, wait for settle time
        if (now - phaseStart >= COOLDOWN_MS) {
          Serial.println("── COOLDOWN DONE ── back to idle");
          pumpState = PUMP_IDLE;
          cyclePulseSum = 0;
          cyclePulseReadings = 0;
          cycleSawZero = false;
        }
        break;

      case PUMP_PRIME:
        break; // handled above
    }
  }

  // ── 3. Valve control ───────────────────────────────────────

  bool shouldValveBeOpen = (pumpState != PUMP_IDLE) || waterFlowing || primePressed;

  if (shouldValveBeOpen && !valveOpen) {
    motorOn(active.valve, 255);
    valveOpen = true;
    blinkState = true;
    lastBlinkToggle = now;
    Serial.printf("Dispensing flavor %d\n", activeFlavor + 1);
  } else if (!shouldValveBeOpen && valveOpen) {
    motorOff(active.pump);    // defensive
    motorOff(active.valve);
    valveOpen = false;
    digitalWrite(LED_FLAVOR1, activeFlavor == 0 ? HIGH : LOW);
    digitalWrite(LED_FLAVOR2, activeFlavor == 1 ? HIGH : LOW);
    Serial.printf("Stopped dispensing flavor %d\n", activeFlavor + 1);
  }

  // ── 4. LED control ─────────────────────────────────────────

  if (valveOpen && (now - lastBlinkToggle >= BLINK_INTERVAL_MS)) {
    blinkState = !blinkState;
    uint8_t ledPin = (activeFlavor == 0) ? LED_FLAVOR1 : LED_FLAVOR2;
    digitalWrite(ledPin, blinkState ? HIGH : LOW);
    lastBlinkToggle = now;
  }

  // ── 5. Periodic display config resend ─────────────────────
  if (now - lastConfigSend >= CONFIG_SEND_INTERVAL_MS) {
    sendMapToRP();
    lastConfigSend = now;
  }

  // ── 6. Config UART commands ─────────────────────────────────
  checkConfigUART();

  delay(10);
}
