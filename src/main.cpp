#include <Arduino.h>

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
#define LED_FLAVOR2     15   // lit when flavor 2 is selected (blinks while dispensing)

// ── Pump hard limits (physical constraints) ──
#define PUMP_ON_MIN_MS     50   // below this, pump doesn't reliably dispense
#define PUMP_OFF_MAX_MS  1000   // above this, reaction time is too slow
#define PUMP_SPEED        255

// ── Pump duty cycling (recipe tuning) ──
//   1 pulse →  50 on / 600 off
//   6 pulse → 200 on / 300 off
//   on  = 20 + 30 * pulses  (clamped 1–6, then clamped to hard limits)
//   off = 660 - 60 * pulses (clamped 1–6, then clamped to hard limits)
#define PUMP_ON_BASE_MS    20
#define PUMP_ON_PER_PULSE  30
#define PUMP_OFF_BASE_MS  660
#define PUMP_OFF_PER_PULSE 60

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
};

Flavor flavors[] = {
  // Flavor 1 — Board A
  {
    { A_ENA, A_IN1, A_IN2 },   // pump
    { A_ENB, A_IN3, A_IN4 },   // valve
  },
  // Flavor 2 — Board B
  {
    { B_ENA, B_IN1, B_IN2 },   // pump
    { B_ENB, B_IN3, B_IN4 },   // valve
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
//  State
// ════════════════════════════════════════════════════════════

uint8_t activeFlavor   = 0;       // 0 or 1
bool    waterFlowing   = false;   // true if latest 50ms reading >= FLOW_MIN_PULSES
unsigned long flowPulses = 0;     // pulse count from latest 50ms check
bool    primePressed   = false;

// ── Pump state machine ──
// A "cycle" = one ON phase + one OFF phase, timing locked at cycle start.
// PRIME = manual override, pump runs continuously.
// COOLDOWN = saw a zero during cycle, pump off for 500ms, readings discarded.
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

// ════════════════════════════════════════════════════════════
//  Setup
// ════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);

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

  Serial.println("Dual-Flavor Soda Maker ready!");
  Serial.printf("Active flavor: %d\n", activeFlavor + 1);
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
          unsigned long clamped = constrain(flowPulses, FLOW_MIN_PULSES, FLOW_FULL_PULSES);
          cycleOnMs  = max((unsigned long)PUMP_ON_MIN_MS, PUMP_ON_BASE_MS  + PUMP_ON_PER_PULSE  * clamped);
          cycleOffMs = min((unsigned long)PUMP_OFF_MAX_MS, PUMP_OFF_BASE_MS - PUMP_OFF_PER_PULSE * clamped);
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
            unsigned long clamped = constrain(avg, FLOW_MIN_PULSES, FLOW_FULL_PULSES);
            cycleOnMs  = max((unsigned long)PUMP_ON_MIN_MS, PUMP_ON_BASE_MS  + PUMP_ON_PER_PULSE  * clamped);
            cycleOffMs = min((unsigned long)PUMP_OFF_MAX_MS, PUMP_OFF_BASE_MS - PUMP_OFF_PER_PULSE * clamped);
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

  delay(10);
}
