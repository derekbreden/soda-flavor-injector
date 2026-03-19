#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <uart_st.h>
#include <uart_queue.h>
#include "fw_version.h"

// ════════════════════════════════════════════════════════════
//  Dual-Flavor Soda Maker
// ════════════════════════════════════════════════════════════

// ── L298N Board A (flavor 1) ──
#define A_ENA  33    // pump PWM
#define A_IN1  25    // pump direction
#define A_IN2  26    // pump direction
#define A_ENB  12    // dispensing solenoid valve

// ── L298N Board B (flavor 2) ──
#define B_ENA  19    // pump PWM
#define B_IN1  18    // pump direction
#define B_IN2   5    // pump direction
#define B_ENB   4    // dispensing solenoid valve

// ── L298N Board C (clean solenoids) ──
#define CLEAN_SOL1_PIN 27   // clean solenoid flavor 1, L298N #3 Channel A ENA
#define CLEAN_SOL2_PIN 17   // clean solenoid flavor 2, L298N #3 Channel B ENB

// ── Inputs ──
#define FLAVOR_SW_PIN   13   // latching toggle: flavor select (air switch)
#define FLOW_PIN        23   // flow meter pulse input

// ── Per-flavor config (runtime, persisted in LittleFS) ──
// Ratio: flavoring to water in 1:X. Lower = more flavor.
//   6  = maximum strength (traditional BIB, e.g. Coke syrup)
//  20  = tuned for SodaStream concentrates
//  24  = minimum strength (hard limit floor)
// Image: index into the RP2040's LittleFS image store
uint8_t numRpImages = 0;  // updated at boot via QUERY_COUNT to RP2040
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
extern const char factory_manifest_start[] asm("_binary_images_factory_manifest_json_start");

// USB binary store protocol constants (upload_image.py → ESP32 LittleFS)
#define CMD_UPLOAD_START  0x01
#define CMD_CHUNK_DATA    0x02
#define CMD_UPLOAD_DONE   0x03
#define RESP_READY        0x10
#define RESP_CHUNK_OK     0x11
#define RESP_UPLOAD_OK    0x12

uint8_t numEspImages = 0;
uint8_t factoryImageCount = 0;  // slots 0..factoryImageCount-1 are protected
char espLabels[MAX_STORE_IMAGES][MAX_LABEL_LEN + 1];

// ── S3-initiated upload state (BLE phone → S3 → ESP32 via SerialTransfer) ──
static struct {
  bool active = false;
  uint8_t slot;
  uint8_t fileType;  // 0=s3_rgb, 1=png, 2=rp_rgb
  uint32_t expectedSize;
  uint32_t receivedBytes;
  uint8_t nextSeq;
  uint32_t runningCrc32;
  unsigned long lastChunkTime;
  File file;
} s3Upload;

uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;

// ── Display UART (ESP32 ↔ RP2040, bidirectional) ──
#define DISPLAY_TX_PIN          32     // UART TX to RP2040 display
#define DISPLAY_RX_PIN          35     // UART RX from RP2040 (input-only GPIO)
#define CONFIG_SEND_INTERVAL_MS 30000  // resend image mapping every 30s

SerialTransfer stRP;  // SerialTransfer on Serial2 (RP2040 link)
SerialTransfer stS3;  // SerialTransfer on Serial1 (S3 link)
UartLink linkRP;      // Non-blocking state machine for RP2040 link
UartLink linkS3;      // Non-blocking state machine for S3 link

void sendMapToRP() {
  char buf[20];
  snprintf(buf, sizeof(buf), "MAP:%d,%d", flavor1Image, flavor2Image);
  if (linkRP.st) {
    linkRP.queueText(buf, false, UARTLINK_PRI_HIGH);
  } else {
    stSendText(stRP, buf);  // pre-init (setup)
  }
}

// ── Config UART (ESP32 ↔ ESP32-S3, bidirectional) ──
// GPIO 15 for TX: frees GPIO 2 (boot-strap pin that must be LOW to flash).
// GPIO 15 is a strapping pin too but only affects boot log silence — won't block flashing.
#define CONFIG_TX_PIN  15    // UART TX to ESP32-S3
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


// ── Flow detection ──
#define FLOW_CHECK_INTERVAL_MS  50
#define FLOW_MIN_PULSES          1   // minimum to count as flowing
#define FLOW_FULL_PULSES         6   // pulses per interval at full flow
#define COOLDOWN_MS           1000   // settle time after zero detected in cycle

// ════════════════════════════════════════════════════════════
//  Pump and valve abstraction
// ════════════════════════════════════════════════════════════

struct PumpChannel {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
};

struct Flavor {
  PumpChannel pump;
  uint8_t valvePin;
  uint8_t ratio;
};

Flavor flavors[] = {
  // Flavor 1 — Board A
  {
    { A_ENA, A_IN1, A_IN2 },   // pump
    A_ENB,                      // dispensing solenoid valve
    20,                         // ratio (overwritten by loadConfig)
  },
  // Flavor 2 — Board B
  {
    { B_ENA, B_IN1, B_IN2 },   // pump
    B_ENB,                      // dispensing solenoid valve
    20,                         // ratio (overwritten by loadConfig)
  },
};

void pumpOn(const PumpChannel& m, uint8_t speed) {
  digitalWrite(m.in1, HIGH);
  digitalWrite(m.in2, LOW);
  analogWrite(m.ena, speed);
}

void pumpOff(const PumpChannel& m) {
  digitalWrite(m.in1, LOW);
  digitalWrite(m.in2, LOW);
  analogWrite(m.ena, 0);
}

void valveOn(uint8_t pin) { analogWrite(pin, 255); }
void valveOff(uint8_t pin) { analogWrite(pin, 0); }

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

// ── Clean cycle state ──
enum CleanState { CLEAN_IDLE, CLEAN_FILLING, CLEAN_FLUSHING };
CleanState cleanState = CLEAN_IDLE;
uint8_t    cleanFlavor = 0;        // 0 or 1 (index into flavors[])
uint8_t    cleanCycle  = 0;        // current cycle (0-based)
unsigned long cleanPhaseStart = 0;

#define CLEAN_CYCLES     3      // number of fill+flush rounds
#define CLEAN_FILL_MS   10000   // 10 seconds fill
#define CLEAN_FLUSH_MS  15000   // 15 seconds flush

// ── Prime state (software-controlled via UART/BLE) ──
bool primeActive = false;
uint8_t primeFlavor = 0;           // 0-based index into flavors[]
unsigned long primeStartMs = 0;
unsigned long lastPrimeTickMs = 0;
#define PRIME_TIMEOUT_MS  2000     // auto-stop if no tick within 2s
#define PRIME_MAX_MS     60000     // hard ceiling: 60s total

// ── Valve ──
bool valveOpen                   = false;
unsigned long lastFlowCheck      = 0;
unsigned long lastConfigSend     = 0;

// ── Usage statistics ──
struct StatsAccum {
  uint32_t flow_sum;       // sum of pulse readings (0-6 each, every 50ms when flowing)
  uint32_t flow_count;     // number of 50ms readings that contributed
  uint32_t burst_sum_ms;   // sum of pump burst durations in ms
  uint32_t burst_count;    // number of bursts
};

struct StatsBucket {
  uint32_t seq_hour;       // seqHour key (monotonic hour counter)
  uint32_t flow_sum, flow_count, burst_sum_ms, burst_count;
};

StatsAccum currentHour[2] = {};   // per-flavor accumulators for current hour
unsigned long lastStatsFlush = 0; // last 60s flush

// Stats persistence
#define HOURLY_RING_SIZE   720   // 30 days of hourly buckets
#define RTC_EPOCH_PATH     "/stats/rtc_epoch.bin"

RTC_DS3231 rtc;
uint32_t rtcEpoch = 0;   // RTC timestamp when seqHour was 0 (persisted)
uint32_t seqHour = 0;    // derived: (rtcNow - rtcEpoch) / 3600

static const char *statsHourlyPath(uint8_t flavor) {
  return flavor == 0 ? "/stats/f0_hourly.bin" : "/stats/f1_hourly.bin";
}

// Live chart push (ESP32 → S3 → BLE → iOS, no polling)
uint8_t statsSubscribeCount = 0;
unsigned long lastChartPush = 0;
unsigned long lastChartAck = 0;
const unsigned long CHART_ACK_TIMEOUT = 10000;
uint32_t lastPushedFlowSum[2] = {};

// ════════════════════════════════════════════════════════════
//  Factory defaults & LittleFS config persistence
// ════════════════════════════════════════════════════════════

// Forward declarations (defined after image store section)
void saveEspMeta();
void saveEspLabels();

// Parse compiled-in factory_manifest.json → set runtime config + labels + store count
void applyFactoryDefaults() {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, factory_manifest_start);
  if (err) {
    Serial.printf("WARNING: factory_manifest.json parse failed: %s\n", err.c_str());
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
  numEspImages = count;
  factoryImageCount = count;
  saveEspMeta();
  saveEspLabels();

  Serial.printf("Factory defaults applied: F1 ratio=%d image=%d, F2 ratio=%d image=%d, %d images (%d factory)\n",
                flavor1Ratio, flavor1Image, flavor2Ratio, flavor2Image, count, count);
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
//  CRC-16/CCITT for USB binary store protocol
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
//  Query RP2040 image count (binary protocol)
// ════════════════════════════════════════════════════════════

bool queryImageCount() {
  stRP.packet.txBuff[0] = 0;
  stRP.sendData(1, PKT_QUERY_COUNT);

  unsigned long start = millis();
  while (millis() - start < 500) {
    if (stRP.available()) {
      if (stRP.currentPacketID() == PKT_RESP_COUNT) {
        ResponsePayload resp;
        stRP.rxObj(resp);
        numRpImages = resp.value;
        Serial.printf("RP2040 reports %d images\n", numRpImages);
        return true;
      }
    }
  }
  return false;
}



// ════════════════════════════════════════════════════════════
//  Query S3 image count via SerialTransfer
// ════════════════════════════════════════════════════════════

bool queryS3ImageCount() {
  stS3.packet.txBuff[0] = 0;
  stS3.sendData(1, PKT_QUERY_COUNT);

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
    numEspImages = f.parseInt();
    f.close();
  }
}

void saveEspMeta() {
  File f = LittleFS.open(ESP_META_PATH, "w");
  if (f) {
    f.println(numEspImages);
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
  for (uint8_t i = 0; i < numEspImages; i++) {
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
        runCrc = uartCrc32Update(runCrc, buf + 6, dataLen);
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
      if (!isPng && slot >= numEspImages) {
        numEspImages = slot + 1;
        saveEspMeta();
      }

      // Send RESP_UPLOAD_OK
      uint8_t resp[6];
      resp[0] = 0x02; resp[1] = 0x02;
      resp[2] = RESP_UPLOAD_OK; resp[3] = numEspImages;
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
//  RP2040 async sync state machine
// ════════════════════════════════════════════════════════════

// Forward declarations
void advanceRpSyncImages();
void onRpSyncUploadDone(UartLink *link, uint8_t slot, bool success);
void onRpSyncDeleteDone(UartLink *link, uint8_t slot, bool success);
void rpSyncSendLabelsAndConfig();

static struct {
  bool active = false;
  uint8_t phase;        // 0=images, 1=deletes, 2=labels+config
  uint8_t slot;
  uint8_t targetCount;
  bool pushAll;
} rpSync;

void startRpSync(bool pushAll) {
  if (rpSync.active) {
    Serial.println("[RP sync] Already active — skipping");
    return;
  }
  if (numEspImages == 0) {
    Serial.println("[RP sync] Store empty — nothing to push");
    return;
  }
  rpSync.active = true;
  rpSync.targetCount = numEspImages;
  rpSync.pushAll = pushAll;

  if (pushAll) {
    Serial.printf("[RP sync] Start: pushing %d images\n", numEspImages);
    rpSync.phase = 0;
    rpSync.slot = 0;
    advanceRpSyncImages();
  } else {
    Serial.printf("[RP sync] Start: labels+config only (%d images in sync)\n", numEspImages);
    rpSync.phase = 2;
    rpSyncSendLabelsAndConfig();
  }
}

void advanceRpSyncImages() {
  if (rpSync.slot >= rpSync.targetCount) {
    // All images done — move to delete phase
    rpSync.phase = 1;
    if (numRpImages > rpSync.targetCount) {
      linkRP.queueDelete(rpSync.targetCount, UARTLINK_PRI_LOW, onRpSyncDeleteDone);
    } else {
      rpSync.phase = 2;
      rpSyncSendLabelsAndConfig();
    }
    return;
  }

  String path = espRpPath(rpSync.slot);
  linkRP.queueUpload(rpSync.slot, path.c_str(), PKT_UPLOAD_START,
                     UARTLINK_PRI_LOW, onRpSyncUploadDone);
}

void onRpSyncUploadDone(UartLink *link, uint8_t slot, bool success) {
  if (!success) {
    Serial.printf("[RP sync] Upload slot %d failed — aborting\n", slot);
    rpSync.active = false;
    return;
  }
  rpSync.slot++;
  advanceRpSyncImages();
}

void onRpSyncDeleteDone(UartLink *link, uint8_t slot, bool success) {
  if (success) numRpImages = link->lastResponseValue;
  if (numRpImages > rpSync.targetCount) {
    linkRP.queueDelete(rpSync.targetCount, UARTLINK_PRI_LOW, onRpSyncDeleteDone);
  } else {
    rpSync.phase = 2;
    rpSyncSendLabelsAndConfig();
  }
}

void rpSyncSendLabelsAndConfig() {
  for (uint8_t i = 0; i < rpSync.targetCount; i++) {
    char lbuf[48];
    snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
    linkRP.queueText(lbuf, false, UARTLINK_PRI_LOW);
  }
  sendMapToRP();

  numRpImages = rpSync.targetCount;
  rpSync.active = false;
  Serial.printf("[RP sync] Complete (%d images)\n", rpSync.targetCount);
}

// ════════════════════════════════════════════════════════════
//  RP2040 deferred command response
// ════════════════════════════════════════════════════════════

#define RP_RESP_NONE 0
#define RP_RESP_USB  1
#define RP_RESP_S3   2

static struct {
  bool pending = false;
  uint8_t target;
  char okMsg[64];
  char errMsg[64];
} rpCmdResp;

void rpCmdSendResponse(bool success) {
  const char *msg = success ? rpCmdResp.okMsg : rpCmdResp.errMsg;
  if (rpCmdResp.target == RP_RESP_USB) {
    Serial.println(msg);
  } else if (rpCmdResp.target == RP_RESP_S3) {
    stSendText(stS3, msg);
  }
  rpCmdResp.pending = false;
}

void onRpDeleteDone(UartLink *link, uint8_t slot, bool success) {
  if (success) {
    numRpImages = link->lastResponseValue;
    if (flavor1Image == slot) flavor1Image = 0;
    else if (flavor1Image > slot) flavor1Image--;
    if (flavor2Image == slot) flavor2Image = 0;
    else if (flavor2Image > slot) flavor2Image--;
    if (flavor1Image >= numRpImages) flavor1Image = 0;
    if (flavor2Image >= numRpImages) flavor2Image = 0;
    sendMapToRP();
  }
  snprintf(rpCmdResp.okMsg, sizeof(rpCmdResp.okMsg),
           "OK:DELETED=%d,NUM_IMAGES=%d", slot, numRpImages);
  rpCmdSendResponse(success);
}

void onRpSwapDone(UartLink *link, uint8_t slotA, uint8_t slotB, bool success) {
  if (success) {
    if (flavor1Image == slotA) flavor1Image = slotB;
    else if (flavor1Image == slotB) flavor1Image = slotA;
    if (flavor2Image == slotA) flavor2Image = slotB;
    else if (flavor2Image == slotB) flavor2Image = slotA;
    sendMapToRP();
  }
  snprintf(rpCmdResp.okMsg, sizeof(rpCmdResp.okMsg),
           "OK:SWAPPED=%d,%d", slotA, slotB);
  rpCmdSendResponse(success);
}

void onRpQueryDone(UartLink *link, uint8_t count, bool success) {
  if (success) numRpImages = count;
  snprintf(rpCmdResp.okMsg, sizeof(rpCmdResp.okMsg),
           "OK:NUM_IMAGES=%d", numRpImages);
  rpCmdSendResponse(success);
}

// S3 async response targets (used by startS3Sync and s3CmdResp)
#ifndef S3_RESP_NONE
#define S3_RESP_NONE 0
#define S3_RESP_USB  1
#define S3_RESP_S3   2
#endif

// Forward declarations (defined earlier/after processConfigCommand)
// startRpSync is defined above; startS3Sync is defined below.
void startS3Sync(bool pushAll, uint8_t respTarget = S3_RESP_NONE, bool rpResult = true);

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

  if (numEspImages == 0) {
    Serial.println("Store empty — skipping boot sync");
    return;
  }

  Serial.printf("Boot sync: store has %d images\n", numEspImages);
  bool rpNeedsPush = (numRpImages != numEspImages);
  startRpSync(rpNeedsPush);
  bool s3NeedsPush = (numS3Images != numEspImages);
  startS3Sync(s3NeedsPush);
}

// ════════════════════════════════════════════════════════════
//  Usage statistics — persistence and rollup
// ════════════════════════════════════════════════════════════

// ── RTC epoch persistence ──

void saveRtcEpoch() {
  const char *tmp = RTC_EPOCH_PATH ".tmp";
  File f = LittleFS.open(tmp, "w");
  if (f) {
    f.write((uint8_t*)&rtcEpoch, sizeof(rtcEpoch));
    f.close();
    LittleFS.remove(RTC_EPOCH_PATH);
    LittleFS.rename(tmp, RTC_EPOCH_PATH);
  }
}

bool loadRtcEpoch() {
  File f = LittleFS.open(RTC_EPOCH_PATH, "r");
  if (f && f.size() == sizeof(rtcEpoch)) {
    f.read((uint8_t*)&rtcEpoch, sizeof(rtcEpoch));
    f.close();
    return true;
  }
  if (f) f.close();
  return false;
}

// Scan both ring buffer files for the highest seq_hour (battery-death recovery)
uint32_t recoverMaxSeqHour() {
  uint32_t maxSH = 0;
  for (int flavor = 0; flavor < 2; flavor++) {
    File f = LittleFS.open(statsHourlyPath(flavor), "r");
    if (!f) continue;
    StatsBucket entry;
    while (f.read((uint8_t*)&entry, sizeof(StatsBucket)) == sizeof(StatsBucket)) {
      if (entry.seq_hour > maxSH) maxSH = entry.seq_hour;
    }
    f.close();
  }
  return maxSH;
}

// Load current hour's accumulator from ring buffer
void loadCurrentHourFromRing() {
  for (int flavor = 0; flavor < 2; flavor++) {
    File f = LittleFS.open(statsHourlyPath(flavor), "r");
    if (!f) continue;
    StatsBucket entry;
    while (f.read((uint8_t*)&entry, sizeof(StatsBucket)) == sizeof(StatsBucket)) {
      if (entry.seq_hour == seqHour) {
        currentHour[flavor].flow_sum = entry.flow_sum;
        currentHour[flavor].flow_count = entry.flow_count;
        currentHour[flavor].burst_sum_ms = entry.burst_sum_ms;
        currentHour[flavor].burst_count = entry.burst_count;
      }
    }
    f.close();
  }
}

// Forward declaration (defined below)
void upsertHourlyBucket(const char *path, const StatsBucket &bucket, int maxEntries);

void flushCurrentHour() {
  for (int f = 0; f < 2; f++) {
    if (currentHour[f].flow_count == 0 && currentHour[f].burst_count == 0) continue;
    StatsBucket bucket;
    bucket.seq_hour = seqHour;
    bucket.flow_sum = currentHour[f].flow_sum;
    bucket.flow_count = currentHour[f].flow_count;
    bucket.burst_sum_ms = currentHour[f].burst_sum_ms;
    bucket.burst_count = currentHour[f].burst_count;
    upsertHourlyBucket(statsHourlyPath(f), bucket, HOURLY_RING_SIZE);
  }
}

// Upsert a bucket into an hourly file: find existing entry by seq_hour and
// overwrite it, or append if new. Each hour has exactly one entry in the file.
// Trims oldest entries if over maxEntries.
void upsertHourlyBucket(const char *path, const StatsBucket &bucket, int maxEntries) {
  // Scan for existing entry with matching seq_hour
  File f = LittleFS.open(path, "r");
  if (f) {
    StatsBucket entry;
    int idx = 0;
    while (f.read((uint8_t*)&entry, sizeof(StatsBucket)) == sizeof(StatsBucket)) {
      if (entry.seq_hour == bucket.seq_hour) {
        f.close();
        // Overwrite in place
        File fw = LittleFS.open(path, "r+");
        if (fw) {
          fw.seek(idx * sizeof(StatsBucket));
          fw.write((uint8_t*)&bucket, sizeof(StatsBucket));
          fw.close();
        }
        return;
      }
      idx++;
    }
    f.close();
  }

  // No existing entry — append
  File fa = LittleFS.open(path, "a");
  if (fa) {
    fa.write((uint8_t*)&bucket, sizeof(StatsBucket));
    fa.close();
  }

  // Trim oldest entries if over limit
  File fc = LittleFS.open(path, "r");
  if (fc) {
    int entryCount = fc.size() / sizeof(StatsBucket);
    if (entryCount > maxEntries) {
      int skip = entryCount - maxEntries;
      Serial.printf("Stats trim: dropping %d oldest entries from %s\n", skip, path);
      File tmp = LittleFS.open("/stats/trim.tmp", "w");
      if (tmp) {
        fc.seek(skip * sizeof(StatsBucket));
        StatsBucket entry;
        for (int i = 0; i < maxEntries; i++) {
          fc.read((uint8_t*)&entry, sizeof(StatsBucket));
          tmp.write((uint8_t*)&entry, sizeof(StatsBucket));
        }
        tmp.close();
        fc.close();
        LittleFS.remove(path);
        LittleFS.rename("/stats/trim.tmp", path);
      } else {
        fc.close();
      }
    } else {
      fc.close();
    }
  }
}


void clearStatsFiles() {
  LittleFS.remove(statsHourlyPath(0));
  LittleFS.remove(statsHourlyPath(1));
  LittleFS.remove(RTC_EPOCH_PATH);
  memset(currentHour, 0, sizeof(currentHour));
  rtcEpoch = rtc.now().unixtime();
  seqHour = 0;
  saveRtcEpoch();
}

// ════════════════════════════════════════════════════════════
//  Clean cycle functions
// ════════════════════════════════════════════════════════════

uint8_t cleanSolPin(uint8_t flavor) {
  return (flavor == 0) ? CLEAN_SOL1_PIN : CLEAN_SOL2_PIN;
}

void broadcastCleanStatus(const char *msg) {
  Serial.println(msg);
  linkS3.queueText(msg, false, UARTLINK_PRI_HIGH);
}

void startCleanFill(uint8_t flavor) {
  Flavor& f = flavors[flavor];
  valveOff(f.valvePin);                      // dispensing solenoid CLOSED
  analogWrite(cleanSolPin(flavor), 255);     // clean solenoid OPEN
  pumpOff(f.pump);                           // pump OFF
  cleanState = CLEAN_FILLING;
  cleanPhaseStart = millis();
  char buf[40];
  snprintf(buf, sizeof(buf), "CLEAN:FILLING:%d:%d/%d", flavor + 1, cleanCycle + 1, CLEAN_CYCLES);
  broadcastCleanStatus(buf);
}

void startCleanFlush(uint8_t flavor) {
  Flavor& f = flavors[flavor];
  analogWrite(cleanSolPin(flavor), 0);       // clean solenoid CLOSED
  valveOn(f.valvePin);                       // dispensing solenoid OPEN
  pumpOn(f.pump, PUMP_SPEED);               // pump ON full speed
  cleanState = CLEAN_FLUSHING;
  cleanPhaseStart = millis();
  char buf[40];
  snprintf(buf, sizeof(buf), "CLEAN:FLUSHING:%d:%d/%d", flavor + 1, cleanCycle + 1, CLEAN_CYCLES);
  broadcastCleanStatus(buf);
}

void finishClean(uint8_t flavor) {
  Flavor& f = flavors[flavor];
  pumpOff(f.pump);
  valveOff(f.valvePin);
  analogWrite(cleanSolPin(flavor), 0);
  cleanState = CLEAN_IDLE;
  char buf[20];
  snprintf(buf, sizeof(buf), "OK:CLEAN:%d", flavor + 1);
  broadcastCleanStatus(buf);
}

void abortClean() {
  Flavor& f = flavors[cleanFlavor];
  pumpOff(f.pump);
  valveOff(f.valvePin);
  analogWrite(cleanSolPin(cleanFlavor), 0);
  cleanState = CLEAN_IDLE;
  broadcastCleanStatus("OK:CLEAN_ABORT");
}

// ════════════════════════════════════════════════════════════
//  Prime functions (software-controlled, heartbeat safety)
// ════════════════════════════════════════════════════════════

void broadcastPrimeStatus(const char *msg) {
  Serial.println(msg);
  linkS3.queueText(msg, false, UARTLINK_PRI_HIGH);
}

void startPrime(uint8_t flavor) {
  Flavor& f = flavors[flavor];
  primeActive = true;
  primeFlavor = flavor;
  primeStartMs = millis();
  lastPrimeTickMs = millis();
  pumpOn(f.pump, PUMP_SPEED);
  valveOn(f.valvePin);
  pumpState = PUMP_PRIME;
  valveOpen = true;
  char buf[20];
  snprintf(buf, sizeof(buf), "PRIME:ACTIVE:%d", flavor + 1);
  broadcastPrimeStatus(buf);
}

void stopPrime(const char *reason) {
  Flavor& f = flavors[primeFlavor];
  pumpOff(f.pump);
  valveOff(f.valvePin);
  primeActive = false;
  pumpState = PUMP_IDLE;
  valveOpen = false;
  broadcastPrimeStatus(reason);
}

// ════════════════════════════════════════════════════════════
//  Config UART command parser
// ════════════════════════════════════════════════════════════

void sendConfigResponse(Stream &out) {
  out.printf("CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d\n",
             flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numEspImages);
}

void abortS3Upload();  // forward decl

// Forward declarations for S3 async sync (defaults on first declaration above)
void startS3Sync(bool pushAll, uint8_t respTarget, bool rpResult);

// S3 async sync state machine
static struct {
  bool active = false;
  uint8_t phase;        // 0=images, 1=deletes, 2=pngs, 3=labels+config
  uint8_t slot;
  uint8_t targetCount;
  bool pushAll;
  uint8_t respTarget;
  bool rpResult;        // stash RP2040 result for combined response
  char respBuf[64];     // deferred response message
} s3Sync;

// Deferred S3 command response (for DELETE_S3_IMG, SWAP_S3_IMG, etc.)
// Completion callbacks send the response via USB or S3.
static struct {
  bool pending = false;
  uint8_t target;       // S3_RESP_USB or S3_RESP_S3
  char okMsg[64];
  char errMsg[64];
} s3CmdResp;

void s3CmdSendResponse(bool success) {
  const char *msg = success ? s3CmdResp.okMsg : s3CmdResp.errMsg;
  if (s3CmdResp.target == S3_RESP_USB) {
    Serial.println(msg);
  } else if (s3CmdResp.target == S3_RESP_S3) {
    stSendText(stS3, msg);
  }
  s3CmdResp.pending = false;
}

void onS3DeleteDone(UartLink *link, uint8_t slot, bool success) {
  if (success) numS3Images = link->lastResponseValue;
  snprintf(s3CmdResp.okMsg, sizeof(s3CmdResp.okMsg),
           "OK:S3_DELETED=%d,NUM_S3_IMAGES=%d", slot, numS3Images);
  s3CmdSendResponse(success);
}

void onS3SwapDone(UartLink *link, uint8_t slotA, uint8_t slotB, bool success) {
  s3CmdSendResponse(success);
}

void onS3QueryDone(UartLink *link, uint8_t count, bool success) {
  if (success) numS3Images = count;
  snprintf(s3CmdResp.okMsg, sizeof(s3CmdResp.okMsg),
           "OK:NUM_S3_IMAGES=%d", numS3Images);
  s3CmdSendResponse(success);
}

void processConfigCommand(const char *cmd, Stream &out) {
  if (strcmp(cmd, "GET_VERSION") == 0) {
    out.printf("VERSION:ESP32=%s\n", FW_VERSION);
    // Query RP2040 for its version and forward (blocking, kept simple)
    if (linkRP.busy()) { out.printf("VERSION:RP2040=busy\n"); }
    else {
    stSendText(stRP, "GET_VERSION");
    unsigned long t = millis();
    while (millis() - t < 1000) {
      if (stRP.available()) {
        if (stRP.currentPacketID() == PKT_TEXT) {
          uint16_t len = stRP.bytesRead;
          char line[64];
          uint16_t copyLen = (len < 63) ? len : 63;
          memcpy(line, stRP.packet.rxBuff, copyLen);
          line[copyLen] = '\0';
          if (strncmp(line, "VERSION:", 8) == 0) {
            out.println(line);
            break;
          }
        }
      }
    }
    } // end busy guard

  } else if (strcmp(cmd, "GET_CHART_DATA") == 0) {
    // Send raw seqHour-keyed data — iOS does the time mapping and chart computation
    for (int f = 0; f < 2; f++) {
      char line[200];
      int pos = snprintf(line, sizeof(line), "CHART_HOURLY:F=%d,SEQ=%lu",
                         f, (unsigned long)seqHour);

      // Read all entries from hourly ring buffer
      File hf = LittleFS.open(statsHourlyPath(f), "r");
      if (hf) {
        StatsBucket entry;
        while (hf.read((uint8_t*)&entry, sizeof(StatsBucket)) == sizeof(StatsBucket)) {
          // Skip current seqHour (stale file entry; live RAM is authoritative)
          if (entry.seq_hour == seqHour) continue;
          if (entry.flow_sum == 0) continue;

          char pair[24];
          int pairLen = snprintf(pair, sizeof(pair), ",%lu:%lu",
                                 (unsigned long)entry.seq_hour, (unsigned long)entry.flow_sum);

          // If this pair would exceed line limit, emit current line and start new one
          if (pos + pairLen >= (int)sizeof(line) - 1) {
            out.printf("%s\n", line);
            pos = snprintf(line, sizeof(line), "CHART_HOURLY:F=%d,SEQ=%lu",
                           f, (unsigned long)seqHour);
          }
          memcpy(line + pos, pair, pairLen + 1);
          pos += pairLen;
        }
        hf.close();
      }

      // Include current hour's live RAM accumulator
      if (currentHour[f].flow_sum > 0) {
        char pair[24];
        int pairLen = snprintf(pair, sizeof(pair), ",%lu:%lu",
                               (unsigned long)seqHour,
                               (unsigned long)currentHour[f].flow_sum);
        if (pos + pairLen >= (int)sizeof(line) - 1) {
          out.printf("%s\n", line);
          pos = snprintf(line, sizeof(line), "CHART_HOURLY:F=%d,SEQ=%lu",
                         f, (unsigned long)seqHour);
        }
        memcpy(line + pos, pair, pairLen + 1);
        pos += pairLen;
      }

      out.printf("%s\n", line);

      // Baseline for live delta computation (unchanged format)
      out.printf("CHART_CUR:F=%d,FS=%lu\n", f, (unsigned long)currentHour[f].flow_sum);
    }

  } else if (strcmp(cmd, "STATS_SUBSCRIBE") == 0) {
    statsSubscribeCount++;
    if (statsSubscribeCount == 1) lastChartAck = millis();
    lastPushedFlowSum[0] = currentHour[0].flow_sum;
    lastPushedFlowSum[1] = currentHour[1].flow_sum;
    Serial.printf("Stats subscribed (count=%d)\n", statsSubscribeCount);
    out.printf("OK:STATS_SUBSCRIBED\n");

  } else if (strcmp(cmd, "STATS_UNSUBSCRIBE") == 0) {
    if (statsSubscribeCount > 0) statsSubscribeCount--;
    if (statsSubscribeCount == 0) lastChartAck = 0;
    Serial.printf("Stats unsubscribed (count=%d)\n", statsSubscribeCount);
    out.printf("OK:STATS_UNSUBSCRIBED\n");

  } else if (strcmp(cmd, "ABORT_S3_UPLOAD") == 0) {
    if (s3Upload.active) abortS3Upload();

  } else if (strcmp(cmd, "BLE_DISCONNECTED") == 0) {
    if (statsSubscribeCount > 0) {
      statsSubscribeCount--;
      if (statsSubscribeCount == 0) lastChartAck = 0;
      Serial.printf("Stats subscriber disconnected (remaining=%d)\n", statsSubscribeCount);
    }

  } else if (strcmp(cmd, "CHART_ACK") == 0) {
    lastChartAck = millis();

  } else if (strcmp(cmd, "GET_CONFIG") == 0) {
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
        if (val >= 0 && val < numEspImages) {
          flavor1Image = val; ok = true;
          sendMapToRP();
        }
      } else if (strcmp(key, "F2_IMAGE") == 0) {
        if (val >= 0 && val < numEspImages) {
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
    {
      char cfgBuf[128];
      snprintf(cfgBuf, sizeof(cfgBuf),
               "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d",
               flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numEspImages);
      linkS3.queueText(cfgBuf, false, UARTLINK_PRI_HIGH);
    }

  } else if (strcmp(cmd, "QUERY_IMAGES") == 0) {
    rpCmdResp.pending = true;
    rpCmdResp.target = (&out == &Serial) ? RP_RESP_USB : RP_RESP_S3;
    strncpy(rpCmdResp.errMsg, "ERR:rp query failed", sizeof(rpCmdResp.errMsg));
    linkRP.queueQuery(UARTLINK_PRI_NORMAL, onRpQueryDone);

  } else if (strcmp(cmd, "LIST_IMAGES") == 0) {
    if (linkRP.busy()) { out.printf("ERR:RP2040 busy, try again later\n"); return; }
    // Send LIST to RP2040 via SerialTransfer, read PKT_TEXT responses (blocking, multi-message)
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
      if (slot >= 0 && slot < numRpImages) {
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); linkRP.queueText(lbuf, false, UARTLINK_PRI_NORMAL); }
        out.printf("OK:LABEL=%d:%s\n", slot, name);
      } else {
        out.printf("ERR:invalid slot\n");
      }
    }

  } else if (strncmp(cmd, "DELETE_IMG:", 11) == 0) {
    int slot = atoi(cmd + 11);
    if (slot < 0 || slot >= numRpImages) {
      out.printf("ERR:invalid slot (0-%d)\n", numRpImages - 1);
      return;
    }
    if (numRpImages <= 1) {
      out.printf("ERR:cannot delete last image\n");
      return;
    }

    rpCmdResp.pending = true;
    rpCmdResp.target = (&out == &Serial) ? RP_RESP_USB : RP_RESP_S3;
    strncpy(rpCmdResp.errMsg, "ERR:delete failed", sizeof(rpCmdResp.errMsg));
    linkRP.queueDelete((uint8_t)slot, UARTLINK_PRI_NORMAL, onRpDeleteDone);

  } else if (strncmp(cmd, "SWAP_IMG:", 9) == 0) {
    int a, b;
    if (sscanf(cmd + 9, "%d,%d", &a, &b) != 2) {
      out.printf("ERR:usage SWAP_IMG:A,B\n");
      return;
    }
    if (a < 0 || a >= numRpImages || b < 0 || b >= numRpImages) {
      out.printf("ERR:invalid slots (0-%d)\n", numRpImages - 1);
      return;
    }

    rpCmdResp.pending = true;
    rpCmdResp.target = (&out == &Serial) ? RP_RESP_USB : RP_RESP_S3;
    strncpy(rpCmdResp.errMsg, "ERR:swap failed", sizeof(rpCmdResp.errMsg));
    linkRP.queueSwap((uint8_t)a, (uint8_t)b, UARTLINK_PRI_NORMAL, onRpSwapDone);

  // ── S3 image management commands ──────────────────────────

  } else if (strcmp(cmd, "QUERY_S3_IMAGES") == 0) {
    s3CmdResp.pending = true;
    s3CmdResp.target = (&out == &Serial) ? S3_RESP_USB : S3_RESP_S3;
    strncpy(s3CmdResp.errMsg, "ERR:s3 query failed", sizeof(s3CmdResp.errMsg));
    linkS3.queueQuery(UARTLINK_PRI_NORMAL, onS3QueryDone);

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
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); linkS3.queueText(lbuf, false, UARTLINK_PRI_NORMAL); }
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

    s3CmdResp.pending = true;
    s3CmdResp.target = (&out == &Serial) ? S3_RESP_USB : S3_RESP_S3;
    strncpy(s3CmdResp.errMsg, "ERR:s3 delete failed", sizeof(s3CmdResp.errMsg));
    linkS3.queueDelete((uint8_t)slot, UARTLINK_PRI_NORMAL, onS3DeleteDone);

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

    s3CmdResp.pending = true;
    s3CmdResp.target = (&out == &Serial) ? S3_RESP_USB : S3_RESP_S3;
    snprintf(s3CmdResp.okMsg, sizeof(s3CmdResp.okMsg), "OK:S3_SWAPPED=%d,%d", a, b);
    strncpy(s3CmdResp.errMsg, "ERR:s3 swap failed", sizeof(s3CmdResp.errMsg));
    linkS3.queueSwap((uint8_t)a, (uint8_t)b, UARTLINK_PRI_NORMAL, onS3SwapDone);

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
      if (slot >= 0 && slot < numEspImages) {
        strncpy(espLabels[slot], name, MAX_LABEL_LEN);
        espLabels[slot][MAX_LABEL_LEN] = '\0';
        saveEspLabels();
        // Forward to both devices
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); linkRP.queueText(lbuf, false, UARTLINK_PRI_NORMAL); }
        { char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", slot, name); linkS3.queueText(lbuf, false, UARTLINK_PRI_NORMAL); }
        out.printf("OK:STORE_LABEL=%d:%s\n", slot, name);
      } else {
        out.printf("ERR:invalid slot\n");
      }
    }

  } else if (strncmp(cmd, "DELETE_STORE_IMG:", 17) == 0) {
    int slot = atoi(cmd + 17);
    if (slot < 0 || slot >= numEspImages) {
      out.printf("ERR:invalid slot (0-%d)\n", numEspImages - 1);
      return;
    }
    if (slot < factoryImageCount) {
      out.printf("ERR:cannot delete factory image (slot %d)\n", slot);
      return;
    }
    if (numEspImages <= factoryImageCount) {
      out.printf("ERR:only factory images remain\n");
      return;
    }

    // Delete from ESP32 LittleFS (RGB565 + PNG)
    LittleFS.remove(espRpPath(slot));
    LittleFS.remove(espS3Path(slot));
    LittleFS.remove(espS3PngPath(slot));

    // Shift remaining slots down
    for (int i = slot + 1; i < numEspImages; i++) {
      LittleFS.rename(espRpPath(i), espRpPath(i - 1));
      LittleFS.rename(espS3Path(i), espS3Path(i - 1));
      String pngFrom = espS3PngPath(i);
      if (LittleFS.exists(pngFrom)) {
        LittleFS.rename(pngFrom, espS3PngPath(i - 1));
      }
      strncpy(espLabels[i - 1], espLabels[i], MAX_LABEL_LEN);
    }
    numEspImages--;
    espLabels[numEspImages][0] = '\0';
    saveEspMeta();
    saveEspLabels();

    // Forward delete to RP2040 (async)
    linkRP.queueDelete((uint8_t)slot, UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t s, bool ok) {
      if (ok) numRpImages = link->lastResponseValue;
    });
    // Forward delete to S3 (async)
    linkS3.queueDelete((uint8_t)slot, UARTLINK_PRI_NORMAL);

    // Adjust flavor image references
    if (flavor1Image == slot) flavor1Image = 0;
    else if (flavor1Image > slot) flavor1Image--;
    if (flavor2Image == slot) flavor2Image = 0;
    else if (flavor2Image > slot) flavor2Image--;
    if (flavor1Image >= numEspImages) flavor1Image = 0;
    if (flavor2Image >= numEspImages) flavor2Image = 0;
    saveUserConfig();
    sendMapToRP();

    // Push updated config to S3 (async, fire-and-forget)
    {
      char cfgBuf[128];
      snprintf(cfgBuf, sizeof(cfgBuf),
               "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d",
               flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numEspImages);
      linkS3.queueText(cfgBuf, false, UARTLINK_PRI_NORMAL);
    }
    out.printf("OK:STORE_DELETED=%d,NUM_IMAGES=%d\n", slot, numEspImages);

  } else if (strcmp(cmd, "LIST_STORE") == 0) {
    out.printf("STORE_COUNT:%d\n", numEspImages);
    for (uint8_t i = 0; i < numEspImages; i++) {
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
    if (numEspImages == 0) {
      out.printf("ERR:store empty\n");
      return;
    }
    out.printf("OK:PUSHING %d images\n", numEspImages);
    out.flush();
    startRpSync(true);
    startS3Sync(true);

  } else if (strcmp(cmd, "SYNC_DEVICES") == 0) {
    if (numEspImages == 0) {
      out.printf("ERR:store empty\n");
      return;
    }
    out.printf("OK:SYNCING\n");
    out.flush();
    bool rpNeedsPush = (numRpImages != numEspImages);
    startRpSync(rpNeedsPush);
    bool s3NeedsPush = (numS3Images != numEspImages);
    startS3Sync(s3NeedsPush);

  } else if (strcmp(cmd, "FACTORY_RESET") == 0) {
    uint8_t oldCount = numEspImages;

    // Reset config + metadata to compiled-in factory defaults (instant)
    applyFactoryDefaults();

    // Delete user config so defaults persist across reboot
    if (LittleFS.exists(USER_CONFIG_PATH)) {
      LittleFS.remove(USER_CONFIG_PATH);
    }

    // Clean up orphaned user-image files from ESP32 store
    for (uint8_t i = factoryImageCount; i < oldCount; i++) {
      LittleFS.remove(espRpPath(i));
      LittleFS.remove(espS3Path(i));
      LittleFS.remove(espS3PngPath(i));
    }

    // Trim excess images from RP2040 (async)
    if (oldCount > numEspImages) {
      for (uint8_t i = oldCount; i > numEspImages; i--) {
        linkRP.queueDelete(i - 1, UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t s, bool ok) {
          if (ok) numRpImages = link->lastResponseValue;
        });
      }
    }

    // Push labels + MAP to RP2040 (async, queued after deletes)
    for (uint8_t i = 0; i < numEspImages; i++) {
      char lbuf[48];
      snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
      linkRP.queueText(lbuf, false, UARTLINK_PRI_NORMAL);
    }
    sendMapToRP();

    // S3: queue deletes + labels + config via async sync
    if (oldCount > numEspImages) {
      for (uint8_t i = oldCount; i > numEspImages; i--) {
        linkS3.queueDelete(i - 1, UARTLINK_PRI_NORMAL);
      }
    }
    // Labels and config via sync (pushAll=false since images are factory and already match)
    startS3Sync(false);

    clearStatsFiles();
    out.printf("OK:FACTORY_RESET\n");

  } else if (strncmp(cmd, "PUSH_IMG:", 9) == 0) {
    // PUSH_IMG:slot:target — push a single stored image to a device
    int slot;
    char target[8] = {0};
    if (sscanf(cmd + 9, "%d:%7s", &slot, target) != 2) {
      out.printf("ERR:usage PUSH_IMG:slot:target\n");
      return;
    }
    if (slot < 0 || slot >= numEspImages) {
      out.printf("ERR:invalid slot (0-%d)\n", numEspImages - 1);
      return;
    }

    if (strcmp(target, "rp2040") == 0 || strcmp(target, "both") == 0) {
      linkRP.queueUpload(slot, espRpPath(slot).c_str(), PKT_UPLOAD_START,
                         UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t s, bool ok) {
        if (ok) numRpImages = max(numRpImages, (uint8_t)(s + 1));
      });
      for (uint8_t i = 0; i < numEspImages; i++) {
        char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
        linkRP.queueText(lbuf, false, UARTLINK_PRI_LOW);
      }
      sendMapToRP();
    }
    if (strcmp(target, "s3") == 0 || strcmp(target, "both") == 0) {
      // S3 push is async
      String s3Path = espS3Path(slot);
      linkS3.queueUpload(slot, s3Path.c_str(), PKT_UPLOAD_START, UARTLINK_PRI_NORMAL);
      String pngPath = espS3PngPath(slot);
      if (LittleFS.exists(pngPath)) {
        linkS3.queueUpload(slot, pngPath.c_str(), PKT_UPLOAD_PNG_START, UARTLINK_PRI_NORMAL);
      }
      for (uint8_t i = 0; i < numEspImages; i++) {
        char lbuf[48];
        snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
        linkS3.queueText(lbuf, false, UARTLINK_PRI_LOW);
      }
      numS3Images = max(numS3Images, (uint8_t)(slot + 1));
    }
    out.printf("OK:PUSH_IMG=%d rp=queued s3=queued\n", slot);

  } else if (strncmp(cmd, "FINALIZE_UPLOAD:", 16) == 0) {
    // FINALIZE_UPLOAD:slot:label — commit a phone-uploaded image
    int slot = -1;
    char label[MAX_LABEL_LEN + 1] = {0};
    if (sscanf(cmd + 16, "%d:%32[^\n]", &slot, label) < 1 || slot < 0 || slot >= MAX_STORE_IMAGES) {
      out.printf("ERR:invalid FINALIZE_UPLOAD\n");
      return;
    }

    // Update metadata
    strncpy(espLabels[slot], label, MAX_LABEL_LEN);
    espLabels[slot][MAX_LABEL_LEN] = '\0';
    if ((uint8_t)slot >= numEspImages) numEspImages = slot + 1;
    saveEspMeta();
    saveEspLabels();

    // Push RP2040 RGB565 to RP2040 (async)
    linkRP.queueUpload(slot, espRpPath(slot).c_str(), PKT_UPLOAD_START,
                       UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t s, bool ok) {
      if (ok) numRpImages = max(numRpImages, (uint8_t)(s + 1));
    });
    numS3Images = max(numS3Images, (uint8_t)(slot + 1));

    // Push labels + MAP to RP2040 (async, queued after upload)
    for (uint8_t i = 0; i < numEspImages; i++) {
      char lbuf[48]; snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
      linkRP.queueText(lbuf, false, UARTLINK_PRI_LOW);
    }
    sendMapToRP();

    // Push labels + config to S3 (async)
    for (uint8_t i = 0; i < numEspImages; i++) {
      char lbuf[48];
      snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
      linkS3.queueText(lbuf, false, UARTLINK_PRI_LOW);
    }
    // Push CONFIG with authoritative store count
    {
      char cfgBuf[128];
      snprintf(cfgBuf, sizeof(cfgBuf),
               "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d",
               flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numEspImages);
      linkS3.queueText(cfgBuf, false, UARTLINK_PRI_NORMAL);
    }

    out.printf("OK:UPLOAD_DONE:%d\n", slot);
    Serial.printf("FINALIZE_UPLOAD: slot %d label=%s rpPush=queued\n", slot, label);

  } else if (strncmp(cmd, "CLEAN:", 6) == 0) {
    int flav = atoi(cmd + 6);
    if (flav < 1 || flav > 2) {
      out.printf("ERR:CLEAN_INVALID\n");
    } else if (cleanState != CLEAN_IDLE || primeActive) {
      out.printf("ERR:CLEAN_BUSY\n");
    } else {
      cleanFlavor = flav - 1;
      cleanCycle = 0;
      startCleanFill(cleanFlavor);
    }

  } else if (strcmp(cmd, "CLEAN_ABORT") == 0) {
    if (cleanState != CLEAN_IDLE) {
      abortClean();
    } else {
      out.printf("OK:CLEAN_ABORT\n");
    }

  } else if (strncmp(cmd, "PRIME_START:", 12) == 0) {
    int flav = atoi(cmd + 12);
    if (flav < 1 || flav > 2) {
      out.printf("ERR:PRIME_INVALID\n");
    } else if (cleanState != CLEAN_IDLE || primeActive) {
      out.printf("ERR:PRIME_BUSY\n");
    } else {
      startPrime(flav - 1);
    }

  } else if (strcmp(cmd, "PRIME_TICK") == 0) {
    if (primeActive) {
      lastPrimeTickMs = millis();
    }

  } else if (strcmp(cmd, "PRIME_STOP") == 0) {
    if (primeActive) {
      stopPrime("OK:PRIME_STOP");
    } else {
      out.printf("OK:PRIME_STOP\n");
    }
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

// ════════════════════════════════════════════════════════════
//  S3-initiated upload handlers (phone → S3 → ESP32 via SerialTransfer)
// ════════════════════════════════════════════════════════════

void abortS3Upload() {
  if (s3Upload.file) s3Upload.file.close();
  LittleFS.remove("/tmp_s3.bin");
  s3Upload.active = false;
  Serial.println("S3 upload aborted");
}

void handleS3UploadStart(uint8_t pktId) {
  UploadStartPayload pl;
  stS3.rxObj(pl);

  if (s3Upload.active) abortS3Upload();

  uint8_t fileType;
  if (pktId == PKT_UPLOAD_START) {
    fileType = 0;  // S3 RGB565
    if (pl.size != S3_IMAGE_BYTES) { stSendResponse(stS3, PKT_ERR_SIZE_MISMATCH, 0); return; }
  } else if (pktId == PKT_UPLOAD_PNG_START) {
    fileType = 1;  // PNG
    if (pl.size == 0 || pl.size > S3_IMAGE_BYTES) { stSendResponse(stS3, PKT_ERR_SIZE_MISMATCH, 0); return; }
  } else {
    fileType = 2;  // RP2040 RGB565
    if (pl.size != RP2040_IMAGE_BYTES) { stSendResponse(stS3, PKT_ERR_SIZE_MISMATCH, 0); return; }
  }

  if (pl.slot >= MAX_STORE_IMAGES) { stSendResponse(stS3, PKT_ERR_SLOT_INVALID, 0); return; }

  LittleFS.remove("/tmp_s3.bin");
  s3Upload.file = LittleFS.open("/tmp_s3.bin", "w");
  if (!s3Upload.file) { stSendResponse(stS3, PKT_ERR_NO_SPACE, 0); return; }

  s3Upload.active = true;
  s3Upload.slot = pl.slot;
  s3Upload.fileType = fileType;
  s3Upload.expectedSize = pl.size;
  s3Upload.receivedBytes = 0;
  s3Upload.nextSeq = 0;
  s3Upload.runningCrc32 = 0;
  s3Upload.lastChunkTime = millis();

  Serial.printf("S3 upload start: slot %d type %d size %lu\n", pl.slot, fileType, pl.size);
  stSendEmptyResponse(stS3, PKT_RESP_READY);
}

void handleS3ChunkData() {
  ChunkDataPayload hdr;
  stS3.rxObj(hdr);
  uint16_t dataLen = stS3.bytesRead - sizeof(hdr);
  const uint8_t *data = stS3.packet.rxBuff + sizeof(hdr);

  if (!s3Upload.active) { stSendResponse(stS3, PKT_ERR_BUSY, 0); return; }
  if (hdr.seq != s3Upload.nextSeq) { stSendResponse(stS3, PKT_ERR_SEQ, s3Upload.nextSeq); return; }
  if (dataLen == 0 || dataLen > STORE_CHUNK_SIZE) { stSendResponse(stS3, PKT_ERR_CRC, 0); return; }

  size_t written = s3Upload.file.write(data, dataLen);
  if (written != dataLen) { stSendResponse(stS3, PKT_ERR_WRITE, 0); abortS3Upload(); return; }

  s3Upload.receivedBytes += dataLen;
  s3Upload.runningCrc32 = uartCrc32Update(s3Upload.runningCrc32, data, dataLen);
  s3Upload.nextSeq = (s3Upload.nextSeq + 1) & 0xFF;
  s3Upload.lastChunkTime = millis();

  // Log every ~12.8KB (every 100 chunks at 128 bytes)
  if (s3Upload.receivedBytes % 12800 < dataLen) {
    Serial.printf("S3 chunk progress: %lu/%lu bytes\n",
                  s3Upload.receivedBytes, s3Upload.expectedSize);
  }

  stSendResponse(stS3, PKT_RESP_CHUNK_OK, s3Upload.nextSeq);
}

void handleS3UploadDone() {
  UploadDonePayload pl;
  stS3.rxObj(pl);

  if (!s3Upload.active) { stSendResponse(stS3, PKT_ERR_BUSY, 0); return; }

  s3Upload.file.close();

  if (s3Upload.receivedBytes != s3Upload.expectedSize) {
    Serial.printf("S3 upload size mismatch: got %lu expected %lu\n",
                  s3Upload.receivedBytes, s3Upload.expectedSize);
    LittleFS.remove("/tmp_s3.bin");
    s3Upload.active = false;
    stSendResponse(stS3, PKT_ERR_SIZE_MISMATCH, 0);
    return;
  }

  if (s3Upload.runningCrc32 != pl.crc32) {
    Serial.printf("S3 upload CRC mismatch: got 0x%08lX expected 0x%08lX\n",
                  s3Upload.runningCrc32, pl.crc32);
    LittleFS.remove("/tmp_s3.bin");
    s3Upload.active = false;
    stSendResponse(stS3, PKT_ERR_CRC32_MISMATCH, 0);
    return;
  }

  String destPath;
  if (s3Upload.fileType == 0)      destPath = espS3Path(s3Upload.slot);
  else if (s3Upload.fileType == 1) destPath = espS3PngPath(s3Upload.slot);
  else                              destPath = espRpPath(s3Upload.slot);

  LittleFS.remove(destPath);
  LittleFS.rename("/tmp_s3.bin", destPath);
  s3Upload.active = false;

  Serial.printf("S3 upload OK: %s slot %d (%lu bytes)\n",
                destPath.c_str(), s3Upload.slot, s3Upload.receivedBytes);
  stSendResponse(stS3, PKT_RESP_UPLOAD_OK, numEspImages);
}

// ════════════════════════════════════════════════════════════
//  RP2040 unsolicited packet handler (called by linkRP.service())
// ════════════════════════════════════════════════════════════

void onRpPacket(UartLink *link, uint8_t pktId) {
  SerialTransfer &st = *link->st;
  switch (pktId) {
    case PKT_DEVICE_READY: {
      ResponsePayload resp;
      st.rxObj(resp);
      Serial.printf("RP2040 DEVICE_READY: reports %d images\n", resp.value);
      numRpImages = resp.value;
      bool needsPush = (numRpImages != numEspImages);
      startRpSync(needsPush);
      break;
    }
    default:
      Serial.printf("RP2040 unexpected packet 0x%02X — discarded\n", pktId);
      break;
  }
}

// ════════════════════════════════════════════════════════════
//  S3 async sync state machine
// ════════════════════════════════════════════════════════════

void s3SyncSendLabelsAndConfig() {
  for (uint8_t i = 0; i < s3Sync.targetCount; i++) {
    char lbuf[48];
    snprintf(lbuf, sizeof(lbuf), "LABEL:%d:%s", i, espLabels[i]);
    linkS3.queueText(lbuf, false, UARTLINK_PRI_LOW);
  }
  char cfgBuf[128];
  snprintf(cfgBuf, sizeof(cfgBuf),
           "CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d",
           flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numEspImages);
  linkS3.queueText(cfgBuf, false, UARTLINK_PRI_LOW);

  numS3Images = s3Sync.targetCount;
  s3Sync.active = false;
  Serial.printf("[S3 sync] Complete (%d images)\n", s3Sync.targetCount);

  if (s3Sync.respTarget == S3_RESP_USB && s3Sync.respBuf[0]) {
    Serial.println(s3Sync.respBuf);
  } else if (s3Sync.respTarget == S3_RESP_S3 && s3Sync.respBuf[0]) {
    stSendText(stS3, s3Sync.respBuf);
  }
}

void onS3SyncPngDone(UartLink *link, uint8_t slot, bool success);

void advanceS3SyncPngs() {
  while (s3Sync.slot < s3Sync.targetCount) {
    String path = espS3PngPath(s3Sync.slot);
    if (LittleFS.exists(path)) {
      linkS3.queueUpload(s3Sync.slot, path.c_str(), PKT_UPLOAD_PNG_START,
                          UARTLINK_PRI_LOW, onS3SyncPngDone);
      return;  // callback will advance
    }
    s3Sync.slot++;  // no PNG for this slot, skip
  }
  // All PNGs done
  s3SyncSendLabelsAndConfig();
}

void onS3SyncPngDone(UartLink *link, uint8_t slot, bool success) {
  if (!success) {
    Serial.printf("[S3 sync] PNG slot %d failed (non-fatal)\n", slot);
  }
  s3Sync.slot++;
  advanceS3SyncPngs();
}

void onS3SyncDeleteDone(UartLink *link, uint8_t slot, bool success) {
  if (success) {
    // More to trim — queue another delete at the same slot
    linkS3.queueDelete(s3Sync.targetCount, UARTLINK_PRI_LOW, onS3SyncDeleteDone);
  } else {
    // Device's count now matches — move to PNGs
    s3Sync.phase = 2;
    s3Sync.slot = 0;
    advanceS3SyncPngs();
  }
}

void onS3SyncUploadDone(UartLink *link, uint8_t slot, bool success);

void advanceS3SyncImages() {
  if (s3Sync.slot < s3Sync.targetCount) {
    String path = espS3Path(s3Sync.slot);
    linkS3.queueUpload(s3Sync.slot, path.c_str(), PKT_UPLOAD_START,
                        UARTLINK_PRI_LOW, onS3SyncUploadDone);
  } else {
    // All images pushed, start deleting extras
    s3Sync.phase = 1;
    linkS3.queueDelete(s3Sync.targetCount, UARTLINK_PRI_LOW, onS3SyncDeleteDone);
  }
}

void onS3SyncUploadDone(UartLink *link, uint8_t slot, bool success) {
  if (!success) {
    Serial.printf("[S3 sync] Upload slot %d failed — aborting sync\n", slot);
    s3Sync.active = false;
    if (s3Sync.respTarget == S3_RESP_USB) {
      Serial.printf("OK:PUSH_DONE rp=%s s3=fail\n", s3Sync.rpResult ? "ok" : "fail");
    } else if (s3Sync.respTarget == S3_RESP_S3) {
      stSendText(stS3, "ERR:sync failed");
    }
    return;
  }
  s3Sync.slot++;
  advanceS3SyncImages();
}

void startS3Sync(bool pushAll, uint8_t respTarget, bool rpResult) {
  if (s3Sync.active) {
    Serial.println("[S3 sync] Already in progress, skipping");
    return;
  }
  if (numEspImages == 0) {
    Serial.println("[S3 sync] Store empty, nothing to push");
    return;
  }

  s3Sync.active = true;
  s3Sync.targetCount = numEspImages;
  s3Sync.pushAll = pushAll;
  s3Sync.respTarget = respTarget;
  s3Sync.rpResult = rpResult;
  s3Sync.respBuf[0] = '\0';

  if (pushAll) {
    Serial.printf("[S3 sync] Starting full push (%d images)\n", numEspImages);
    s3Sync.phase = 0;  // images
    s3Sync.slot = 0;
    advanceS3SyncImages();
  } else {
    Serial.printf("[S3 sync] Starting PNGs+labels+config (%d images)\n", numEspImages);
    s3Sync.phase = 2;  // skip to PNGs
    s3Sync.slot = 0;
    advanceS3SyncPngs();
  }
}

// Unsolicited packet handler for S3 link (called by linkS3.service())
void onS3Packet(UartLink *link, uint8_t pktId) {
  switch (pktId) {
    case PKT_UPLOAD_START:
    case PKT_UPLOAD_PNG_START:
    case PKT_UPLOAD_RP_START:
      handleS3UploadStart(pktId);
      break;
    case PKT_CHUNK_DATA:
      handleS3ChunkData();
      break;
    case PKT_UPLOAD_DONE:
      handleS3UploadDone();
      break;
    case PKT_DEVICE_READY: {
      ResponsePayload resp;
      stS3.rxObj(resp);
      Serial.printf("S3 DEVICE_READY: reports %d images\n", resp.value);
      numS3Images = resp.value;
      bool needsPush = (numS3Images != numEspImages);
      startS3Sync(needsPush);
      break;
    }
    case PKT_TEXT: {
      uint16_t len = stS3.bytesRead;
      char cmd[CONFIG_BUF_SIZE];
      uint16_t copyLen = (len < CONFIG_BUF_SIZE - 1) ? len : CONFIG_BUF_SIZE - 1;
      memcpy(cmd, stS3.packet.rxBuff, copyLen);
      cmd[copyLen] = '\0';
      StStream s3out(stS3);
      processConfigCommand(cmd, s3out);
      s3out.flush();
      break;
    }
  }
}

void checkConfigUART() {
  checkConfigStream(Serial, configBuf0, configPos0);
  linkS3.service();

  // Timeout stale S3 uploads
  if (s3Upload.active && millis() - s3Upload.lastChunkTime > 5000) {
    Serial.printf("S3 upload timeout: %lu/%lu bytes, %d chunks received, gap=%lums\n",
                  s3Upload.receivedBytes, s3Upload.expectedSize,
                  s3Upload.nextSeq, millis() - s3Upload.lastChunkTime);
    abortS3Upload();
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

  LittleFS.mkdir("/stats");

  // ── RTC init (DS3231 on I2C: SDA=21, SCL=22) ──
  Wire.begin(21, 22);
  if (!rtc.begin()) {
    Serial.println("WARNING: DS3231 RTC not found — stats hours will not survive power loss");
  }

  // ── RTC epoch setup ──
  if (rtc.lostPower()) {
    Serial.println("RTC lost power — resetting oscillator");
    rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
    if (loadRtcEpoch()) {
      uint32_t maxSH = recoverMaxSeqHour();
      uint32_t rtcNow = rtc.now().unixtime();
      rtcEpoch = rtcNow - (maxSH * 3600);
      saveRtcEpoch();
      Serial.printf("RTC battery recovery: rebased rtcEpoch=%lu from maxSeqHour=%lu\n",
                    (unsigned long)rtcEpoch, (unsigned long)maxSH);
    } else {
      rtcEpoch = rtc.now().unixtime();
      saveRtcEpoch();
      Serial.println("Fresh RTC epoch set");
    }
  } else if (!loadRtcEpoch()) {
    rtcEpoch = rtc.now().unixtime();
    saveRtcEpoch();
    Serial.println("First boot with RTC — epoch set");
  }

  // Derive seqHour and restore current hour's accumulators from ring buffer
  seqHour = (rtc.now().unixtime() - rtcEpoch) / 3600;
  loadCurrentHourFromRing();
  Serial.printf("RTC stats: rtcEpoch=%lu seqHour=%lu\n",
                (unsigned long)rtcEpoch, (unsigned long)seqHour);

  // Always determine factory image count from compiled-in defaults
  {
    JsonDocument doc;
    if (!deserializeJson(doc, factory_manifest_start)) {
      factoryImageCount = doc["images"].as<JsonArray>().size();
    }
  }

  // Detect new firmware → apply factory defaults; otherwise load user config
  bool firstBoot = checkFirstBoot();
  loadUserConfig();

  // Init all pump/valve/clean pins
  const uint8_t gpioPins[] = {
    A_ENA, A_IN1, A_IN2, A_ENB,
    B_ENA, B_IN1, B_IN2, B_ENB,
    CLEAN_SOL1_PIN, CLEAN_SOL2_PIN
  };
  for (uint8_t pin : gpioPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Inputs
  pinMode(FLAVOR_SW_PIN, INPUT_PULLUP);
  pinMode(FLOW_PIN,      INPUT_PULLUP);

  // Flow meter interrupt
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flowPulse, FALLING);

  // Read initial flavor from switch state
  activeFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;

  // UART to display board (bidirectional, 38400 baud)
  Serial2.begin(38400, SERIAL_8N1, DISPLAY_RX_PIN, DISPLAY_TX_PIN);
  stRP.begin(Serial2);
  // Wait for RP2040 to boot, init LittleFS, and start UART.
  // First boot seeds 3 images (~88KB writes) which can take several seconds.
  // GP27 (RP2040 TX) is floating until pioSerial.begin() — noise on GPIO 35.
  delay(3000);

  // Try to query RP2040 now; if it's not ready yet, PKT_DEVICE_READY will catch up
  for (int attempt = 0; attempt < 3; attempt++) {
    if (queryImageCount()) break;
    Serial.printf("  RP2040 query retry %d/3...\n", attempt + 1);
    delay(500);
  }
  if (numRpImages == 0 && numEspImages > 0) {
    Serial.println("RP2040 not ready yet — will sync on PKT_DEVICE_READY");
  }
  // sendMapToRP uses stSendText pre-init, which is fine here
  sendMapToRP();

  // Init linkRP for non-blocking operations
  linkRP.init(&stRP, "RP2040");
  linkRP.onPacket = onRpPacket;

  // UART to config display (ESP32-S3, bidirectional, 38400 baud)
  Serial1.begin(38400, SERIAL_8N1, CONFIG_RX_PIN, CONFIG_TX_PIN);
  stS3.begin(Serial1);
  linkS3.init(&stS3, "S3");
  linkS3.onPacket = onS3Packet;

  // Wait for S3 to boot, init LittleFS, and start UART.
  // First boot seeds 3 images (~345KB writes) which can take several seconds.
  // COBS framing naturally rejects boot noise — no parser reset needed.
  delay(3000);

  // Try to query S3 now; if it's not ready yet, PKT_DEVICE_READY will catch up
  for (int attempt = 0; attempt < 3; attempt++) {
    if (queryS3ImageCount()) break;
    Serial.printf("  S3 query retry %d/3...\n", attempt + 1);
    delay(500);
  }
  if (numS3Images == 0 && numEspImages > 0) {
    Serial.println("S3 not ready yet — will sync on PKT_DEVICE_READY");
  }

  // Boot sync: force push on first boot, count-based sync otherwise
  if (firstBoot && numEspImages > 0) {
    Serial.printf("First boot — force pushing %d images to both devices\n", numEspImages);
    startRpSync(true);          // RP2040 async
    startS3Sync(true);          // S3 async
  } else {
    bootSync();
  }

  Serial.println("Dual-Flavor Soda Maker ready!");
  Serial.printf("Active flavor: %d\n", activeFlavor + 1);
  Serial.printf("RP2040: %d/%d images, S3: %d/%d images\n", numRpImages, numEspImages, numS3Images, numEspImages);
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

    // Stats: accumulate flow readings when water is flowing
    if (waterFlowing) {
      currentHour[activeFlavor].flow_sum += count;
      currentHour[activeFlavor].flow_count++;

      // Push live chart update to iOS (throttled to 1/sec)
      if (statsSubscribeCount > 0 && (now - lastChartPush >= 1000)) {
        // Safety net: auto-unsubscribe if S3 stopped acking (rebooted/reflashed)
        if (lastChartAck != 0 && (now - lastChartAck) > CHART_ACK_TIMEOUT) {
          statsSubscribeCount = 0;
          lastChartAck = 0;
          Serial.println("Stats unsubscribed (ack timeout)");
        } else {
          uint32_t fs = currentHour[activeFlavor].flow_sum;
          if (fs != lastPushedFlowSum[activeFlavor]) {
            char buf[40];
            snprintf(buf, sizeof(buf), "CHART_LIVE:F=%d,FS=%lu", activeFlavor, (unsigned long)fs);
            linkS3.queueText(buf, false, UARTLINK_PRI_HIGH);
            lastPushedFlowSum[activeFlavor] = fs;
            lastChartPush = now;
          }
        }
      }
    }

    // Track readings during active pump cycles for averaging
    if (pumpState == PUMP_ON || pumpState == PUMP_OFF) {
      cyclePulseSum += count;
      cyclePulseReadings++;
      if (count == 0) cycleSawZero = true;
    }
  }

  // ── Prime timeout / ceiling check ────────────────────────────
  if (primeActive) {
    if (now - lastPrimeTickMs > PRIME_TIMEOUT_MS) {
      stopPrime("OK:PRIME_TIMEOUT");
    } else if (now - primeStartMs > PRIME_MAX_MS) {
      stopPrime("OK:PRIME_TIMEOUT");
    }
  }

  // ── Clean cycle state machine ──────────────────────────────
  if (cleanState != CLEAN_IDLE) {
    if (cleanState == CLEAN_FILLING && (now - cleanPhaseStart >= CLEAN_FILL_MS)) {
      startCleanFlush(cleanFlavor);
    } else if (cleanState == CLEAN_FLUSHING && (now - cleanPhaseStart >= CLEAN_FLUSH_MS)) {
      cleanCycle++;
      if (cleanCycle < CLEAN_CYCLES) {
        startCleanFill(cleanFlavor);
      } else {
        finishClean(cleanFlavor);
      }
    }
  }

  Flavor& active = flavors[activeFlavor];

  // ── 2. Pump state machine ─────────────────────────────────
  // Skip pump/valve control if clean cycle is active on the current flavor
  bool cleaningActiveFlavor = (cleanState != CLEAN_IDLE && cleanFlavor == activeFlavor);

  if (cleaningActiveFlavor) {
    // Clean cycle controls pump and valve directly — skip normal dispensing
  } else if (primeActive) {
    // Software prime controls pump/valve via startPrime/stopPrime — no-op here
  } else if (pumpState == PUMP_PRIME) {
    // Prime was stopped externally → go idle
    pumpOff(active.pump);
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
          pumpOn(active.pump, PUMP_SPEED);
          pumpState = PUMP_ON;
          phaseStart = now;
          Serial.printf("── CYCLE START ── pulses=%lu → on=%lums off=%lums\n",
                        flowPulses, cycleOnMs, cycleOffMs);
        }
        break;

      case PUMP_ON:
        // On-phase running — wait for it to finish
        if (now - phaseStart >= cycleOnMs) {
          pumpOff(active.pump);
          // Stats: accumulate burst duration
          currentHour[activeFlavor].burst_sum_ms += cycleOnMs;
          currentHour[activeFlavor].burst_count++;
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
            pumpOn(active.pump, PUMP_SPEED);
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
  // Clean cycle controls valve directly — skip normal valve logic
  if (!cleaningActiveFlavor) {
    bool shouldValveBeOpen = (pumpState != PUMP_IDLE) || waterFlowing || primeActive;

    if (shouldValveBeOpen && !valveOpen) {
      valveOn(active.valvePin);
      valveOpen = true;
      Serial.printf("Dispensing flavor %d\n", activeFlavor + 1);
    } else if (!shouldValveBeOpen && valveOpen) {
      pumpOff(active.pump);    // defensive
      valveOff(active.valvePin);
      valveOpen = false;
      Serial.printf("Stopped dispensing flavor %d\n", activeFlavor + 1);
    }
  }

  // ── 4. Periodic display config resend ─────────────────────
  if (now - lastConfigSend >= CONFIG_SEND_INTERVAL_MS) {
    sendMapToRP();
    lastConfigSend = now;
  }

  // ── 5. Stats: RTC-based hourly rollover and periodic flush ──────────
  if (now - lastStatsFlush >= 60000) {
    uint32_t rtcNow = rtc.now().unixtime();
    uint32_t newSeqHour = (rtcNow - rtcEpoch) / 3600;

    if (newSeqHour > seqHour) {
      flushCurrentHour();
      seqHour = newSeqHour;
      memset(currentHour, 0, sizeof(currentHour));
    }

    flushCurrentHour();
    lastStatsFlush = now;
  }

  // ── 6. UART commands ─────────────────────────────────────────
  linkRP.service();
  checkConfigUART();

  // ── 7. Periodic device re-sync (safety net) ─────────────────
  static unsigned long lastResyncCheck = 0;
  if (now - lastResyncCheck >= 30000) {
    lastResyncCheck = now;
    if (numEspImages > 0) {
      if (numRpImages != numEspImages && !rpSync.active) {
        Serial.printf("Re-sync check: RP2040 mismatch %d vs %d — re-querying\n", numRpImages, numEspImages);
        linkRP.queueQuery(UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t count, bool success) {
          if (success) {
            numRpImages = count;
            Serial.printf("RP2040 re-query: %d images\n", numRpImages);
            if (numRpImages != numEspImages) {
              startRpSync(true);
            }
          }
        });
      }
      if (numS3Images != numEspImages && !s3Sync.active) {
        Serial.printf("Re-sync check: S3 mismatch %d vs %d — re-querying\n", numS3Images, numEspImages);
        linkS3.queueQuery(UARTLINK_PRI_NORMAL, [](UartLink *link, uint8_t count, bool success) {
          if (success) {
            numS3Images = count;
            Serial.printf("S3 re-query: %d images\n", numS3Images);
            if (numS3Images != numEspImages) {
              startS3Sync(true);
            }
          }
        });
      }
    }
  }

  delay(10);
}
