#include <Arduino.h>
#include <Preferences.h>

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

// ── Per-flavor config (runtime, persisted in NVS) ──
// Ratio: flavoring to water in 1:X. Lower = more flavor.
//   6  = maximum strength (traditional BIB, e.g. Coke syrup)
//  20  = tuned for SodaStream concentrates
//  24  = minimum strength (hard limit floor)
// Image: index into the RP2040's LittleFS image store
uint8_t numImages = 3;  // updated at boot via QUERY_COUNT to RP2040

uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;

Preferences prefs;

// ── Display UART (ESP32 ↔ RP2040, bidirectional) ──
#define DISPLAY_TX_PIN          32     // UART TX to RP2040 display
#define DISPLAY_RX_PIN          35     // UART RX from RP2040 (input-only GPIO)
#define CONFIG_SEND_INTERVAL_MS 30000  // resend image mapping every 30s

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
//  NVS config persistence
// ════════════════════════════════════════════════════════════

void loadConfig() {
  prefs.begin("soda", true);  // read-only
  flavor1Ratio = prefs.getUChar("f1ratio", 20);
  flavor2Ratio = prefs.getUChar("f2ratio", 20);
  flavor1Image = prefs.getUChar("f1image", 0);
  flavor2Image = prefs.getUChar("f2image", 1);
  prefs.end();

  // Apply to flavor structs
  flavors[0].ratio = flavor1Ratio;
  flavors[1].ratio = flavor2Ratio;

  Serial.printf("Config loaded: F1 ratio=%d image=%d, F2 ratio=%d image=%d\n",
                flavor1Ratio, flavor1Image, flavor2Ratio, flavor2Image);
}

void saveConfig() {
  prefs.begin("soda", false);  // read-write
  prefs.putUChar("f1ratio", flavor1Ratio);
  prefs.putUChar("f2ratio", flavor2Ratio);
  prefs.putUChar("f1image", flavor1Image);
  prefs.putUChar("f2image", flavor2Image);
  prefs.end();
  Serial.println("Config saved to NVS");
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

// ════════════════════════════════════════════════════════════
//  Query RP2040 image count (binary protocol)
// ════════════════════════════════════════════════════════════

bool queryImageCount() {
  // Drain any stale/noise bytes from RX buffer.
  // GPIO 35 has no pull-up; line floats before RP2040 inits its TX pin.
  while (Serial2.available()) Serial2.read();

  // Send QUERY_COUNT: STX STX 0x04 0x00 CRC16
  uint8_t msg[6];
  msg[0] = 0x02; msg[1] = 0x02;
  msg[2] = 0x04; msg[3] = 0x00;
  uint16_t crc = crc16(msg, 4);
  msg[4] = crc & 0xFF;
  msg[5] = (crc >> 8) & 0xFF;
  Serial2.write(msg, 6);
  Serial2.flush();  // wait for TX to complete before reading

  // Wait up to 500ms for 6-byte response
  unsigned long start = millis();
  uint8_t resp[6];
  uint8_t pos = 0;
  while (millis() - start < 500) {
    if (Serial2.available()) {
      resp[pos++] = Serial2.read();
      if (pos == 6) {
        if (resp[0] == 0x02 && resp[1] == 0x02 && resp[2] == 0x14) {
          numImages = resp[3];
          Serial.printf("RP2040 reports %d images\n", numImages);
          return true;
        }
        return false;  // got 6 bytes but not valid response
      }
    }
  }
  return false;
}

// ════════════════════════════════════════════════════════════
//  Upload bridge mode: transparent USB <-> display UART
// ════════════════════════════════════════════════════════════

void enterUploadMode() {
  Serial.println("Entering upload bridge mode...");

  unsigned long lastActivity = millis();
  while (true) {
    // USB -> Display UART
    while (Serial.available() && Serial2.availableForWrite()) {
      Serial2.write(Serial.read());
      lastActivity = millis();
    }
    // Display UART -> USB
    while (Serial2.available() && Serial.availableForWrite()) {
      Serial.write(Serial2.read());
      lastActivity = millis();
    }
    // Exit after 5 seconds of inactivity
    if (millis() - lastActivity > 5000) {
      break;
    }
  }

  Serial.println("Upload bridge mode ended");
  delay(100);
  queryImageCount();
  Serial.printf("numImages now %d\n", numImages);
}

// ════════════════════════════════════════════════════════════
//  Config UART command parser
// ════════════════════════════════════════════════════════════

void sendConfigResponse(Stream &out) {
  out.printf("CONFIG:F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d,numImages=%d\n",
             flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image, numImages);
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
      } else if (strcmp(key, "F1_IMAGE") == 0 && val >= 0 && val < numImages) {
        flavor1Image = val; ok = true;
        Serial2.printf("MAP:%d,%d\n", flavor1Image, flavor2Image);
      } else if (strcmp(key, "F2_IMAGE") == 0 && val >= 0 && val < numImages) {
        flavor2Image = val; ok = true;
        Serial2.printf("MAP:%d,%d\n", flavor1Image, flavor2Image);
      }

      if (ok) {
        out.printf("OK:%s=%d\n", key, val);
        Serial.printf("Config SET: %s=%d\n", key, val);
      } else {
        out.printf("ERR:%s out of range\n", key);
      }
    }

  } else if (strcmp(cmd, "SAVE") == 0) {
    saveConfig();
    out.printf("OK:SAVED\n");

  } else if (strncmp(cmd, "UPLOAD_IMG:", 11) == 0) {
    int slot = atoi(cmd + 11);
    if (slot < 0 || slot > 99) {
      out.printf("ERR:invalid slot\n");
      return;
    }
    out.printf("OK:BRIDGE_MODE\n");
    out.flush();
    enterUploadMode();

  } else if (strcmp(cmd, "QUERY_IMAGES") == 0) {
    queryImageCount();
    out.printf("OK:NUM_IMAGES=%d\n", numImages);
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

char configBuf0[CONFIG_BUF_SIZE];  // USB Serial
uint8_t configPos0 = 0;
char configBuf1[CONFIG_BUF_SIZE];  // Serial1 (S3)
uint8_t configPos1 = 0;

void checkConfigUART() {
  checkConfigStream(Serial,  configBuf0, configPos0);
  checkConfigStream(Serial1, configBuf1, configPos1);
}

// ════════════════════════════════════════════════════════════
//  Setup
// ════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  loadConfig();

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
  Serial2.printf("MAP:%d,%d\n", flavor1Image, flavor2Image);

  // UART to config display (ESP32-S3, bidirectional, 9600 baud)
  Serial1.begin(9600, SERIAL_8N1, CONFIG_RX_PIN, CONFIG_TX_PIN);

  Serial.println("Dual-Flavor Soda Maker ready!");
  Serial.printf("Active flavor: %d\n", activeFlavor + 1);
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
    Serial2.printf("MAP:%d,%d\n", flavor1Image, flavor2Image);
    lastConfigSend = now;
  }

  // ── 6. Config UART commands ─────────────────────────────────
  checkConfigUART();

  delay(10);
}
