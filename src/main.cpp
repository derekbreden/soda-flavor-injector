#include <Arduino.h>

// ════════════════════════════════════════════════════════════
//  Phase 2 – Dual-Flavor Soda Maker
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

// ── Pump duty cycling ──
//   1 pulse → 100 on / 600 off
//   6 pulse → 200 on / 300 off
//   on  = 80 + 20 * pulses  (clamped 1–6)
//   off = 660 - 60 * pulses (clamped 1–6)
#define PUMP_ON_BASE_MS    80
#define PUMP_ON_PER_PULSE  20
#define PUMP_OFF_BASE_MS  660
#define PUMP_OFF_PER_PULSE 60
#define PUMP_SPEED        255

// ── LED blink while dispensing ──
#define BLINK_INTERVAL_MS  50

// ── Flow detection ──
#define FLOW_CHECK_INTERVAL_MS  50
#define FLOW_MIN_PULSES          1   // minimum to count as flowing
#define FLOW_FULL_PULSES         6   // pulses per interval at full flow
#define COOLDOWN_MS            500   // settle time after zero detected in cycle

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

uint8_t activeFlavor = 0;         // 0 or 1
bool dispensing     = false;      // true when pump+valve are active
unsigned long lastFlowCheck  = 0;
bool waterFlowing    = false;
unsigned long lastBlinkToggle = 0;
bool blinkState      = true;
unsigned long currentFlowPulses = 0;  // latest pulse count from flow check

// ── Pump cycle state ──
// A "cycle" = one ON phase + one OFF phase, timing locked at cycle start
// COOLDOWN = saw a zero during cycle, pump off for 500ms, readings discarded
enum CyclePhase { CYCLE_IDLE, CYCLE_ON, CYCLE_OFF, CYCLE_COOLDOWN };
CyclePhase cyclePhase          = CYCLE_IDLE;
unsigned long phaseStart       = 0;     // when current phase began
unsigned long cycleOnMs        = 0;     // locked for entire cycle
unsigned long cycleOffMs       = 0;     // locked for entire cycle
unsigned long cyclePulseSum    = 0;     // accumulated during cycle
unsigned long cyclePulseReadings = 0;   // readings taken during cycle
bool cycleSawZero              = false; // any 0 reading during this cycle?

// ── Flow rate logging ──
unsigned long flowLogWindow    = 0;   // pulses accumulated over 1s window
unsigned long lastFlowLog      = 0;   // timestamp of last 1s log
#define FLOW_LOG_INTERVAL_MS  1000

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

  Serial.println("Phase 2 – Dual-Flavor Soda Maker ready!");
  Serial.printf("Active flavor: %d\n", activeFlavor + 1);
}

// ════════════════════════════════════════════════════════════
//  Loop
// ════════════════════════════════════════════════════════════

void loop() {
  unsigned long now = millis();

  // ── 1. Read flavor switch state directly ──
  uint8_t newFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;
  if (newFlavor != activeFlavor) {
    if (!dispensing) {
      activeFlavor = newFlavor;
      digitalWrite(LED_FLAVOR1, activeFlavor == 0 ? HIGH : LOW);
      digitalWrite(LED_FLAVOR2, activeFlavor == 1 ? HIGH : LOW);
      Serial.printf("Active flavor: %d\n", activeFlavor + 1);
    } else {
      Serial.println("Cannot toggle flavor while dispensing.");
    }
  }

  // ── 2. Check flow meter ──
  if (now - lastFlowCheck >= FLOW_CHECK_INTERVAL_MS) {
    noInterrupts();
    unsigned long count = pulseCount;
    pulseCount = 0;
    interrupts();

    waterFlowing = (count >= FLOW_MIN_PULSES);
    currentFlowPulses = count;
    lastFlowCheck = now;

    // Only accumulate during active cycle phases (not idle/cooldown)
    if (cyclePhase == CYCLE_ON || cyclePhase == CYCLE_OFF) {
      cyclePulseSum += count;
      cyclePulseReadings++;
      if (count == 0) cycleSawZero = true;
    }

    // Accumulate for 1s summary log
    flowLogWindow += count;

    Serial.printf("[50ms] pulses=%lu\n", count);
  }

  // ── Flow rate summary (once per second) ──
  if (now - lastFlowLog >= FLOW_LOG_INTERVAL_MS) {
    Serial.printf("[1sec] pulses=%lu flowing=%s\n",
                  flowLogWindow, waterFlowing ? "YES" : "NO");
    flowLogWindow = 0;
    lastFlowLog = now;
  }

  // ── 3. Determine if we should be dispensing ──
  bool primePressed = (digitalRead(PRIME_BTN_PIN) == LOW);
  bool inCycle = (cyclePhase != CYCLE_IDLE);
  bool shouldDispense = waterFlowing || primePressed || inCycle;

  Flavor& active = flavors[activeFlavor];

  if (shouldDispense) {
    // ── Start dispensing (valve on, LED blinking) ──
    if (!dispensing) {
      dispensing = true;
      motorOn(active.valve, 255);
      blinkState = true;
      lastBlinkToggle = now;
      cyclePhase = CYCLE_IDLE;
      Serial.printf("Dispensing flavor %d\n", activeFlavor + 1);
    }

    // ── Pump cycle state machine ──
    if (primePressed) {
      // Prime mode: continuous pump, no cycling
      if (cyclePhase != CYCLE_ON) {
        motorOn(active.pump, PUMP_SPEED);
        cyclePhase = CYCLE_ON;
        phaseStart = now;
      }
    } else {
      switch (cyclePhase) {

        case CYCLE_IDLE:
          // Waiting for flow — start a new cycle immediately
          if (currentFlowPulses >= FLOW_MIN_PULSES) {
            unsigned long clamped = constrain(currentFlowPulses, FLOW_MIN_PULSES, FLOW_FULL_PULSES);
            cycleOnMs  = PUMP_ON_BASE_MS  + PUMP_ON_PER_PULSE  * clamped;
            cycleOffMs = PUMP_OFF_BASE_MS - PUMP_OFF_PER_PULSE * clamped;
            cyclePulseSum = 0;
            cyclePulseReadings = 0;
            cycleSawZero = false;
            motorOn(active.pump, PUMP_SPEED);
            cyclePhase = CYCLE_ON;
            phaseStart = now;
            Serial.printf("── CYCLE START ── pulses=%lu → on=%lums off=%lums\n",
                          currentFlowPulses, cycleOnMs, cycleOffMs);
          }
          break;

        case CYCLE_ON:
          // On-phase running — wait for it to finish
          if (now - phaseStart >= cycleOnMs) {
            motorOff(active.pump);
            cyclePhase = CYCLE_OFF;
            phaseStart = now;
            Serial.printf("── PUMP OFF  ── (on-phase done, off for %lums)\n", cycleOffMs);
          }
          break;

        case CYCLE_OFF:
          // Off-phase running — wait for it to finish = full cycle complete
          if (now - phaseStart >= cycleOffMs) {
            unsigned long avg = (cyclePulseReadings > 0)
                                ? cyclePulseSum / cyclePulseReadings
                                : currentFlowPulses;
            Serial.printf("── CYCLE DONE ── avg=%lu from %lu readings sawZero=%s\n",
                          avg, cyclePulseReadings, cycleSawZero ? "YES" : "NO");

            if (cycleSawZero) {
              // Flow may be stopping — enter cooldown
              motorOff(active.pump);
              cyclePhase = CYCLE_COOLDOWN;
              phaseStart = now;
              Serial.printf("── COOLDOWN  ── pump off for %dms\n", COOLDOWN_MS);
            } else {
              // Start next cycle using the averaged flow rate
              unsigned long clamped = constrain(avg, FLOW_MIN_PULSES, FLOW_FULL_PULSES);
              cycleOnMs  = PUMP_ON_BASE_MS  + PUMP_ON_PER_PULSE  * clamped;
              cycleOffMs = PUMP_OFF_BASE_MS - PUMP_OFF_PER_PULSE * clamped;
              cyclePulseSum = 0;
              cyclePulseReadings = 0;
              cycleSawZero = false;
              motorOn(active.pump, PUMP_SPEED);
              cyclePhase = CYCLE_ON;
              phaseStart = now;
              Serial.printf("── CYCLE START ── avg=%lu → on=%lums off=%lums\n",
                            avg, cycleOnMs, cycleOffMs);
            }
          }
          break;

        case CYCLE_COOLDOWN:
          // Pump off, readings discarded, wait for settle time
          if (now - phaseStart >= COOLDOWN_MS) {
            Serial.println("── COOLDOWN DONE ── back to idle");
            cyclePhase = CYCLE_IDLE;
            cyclePulseSum = 0;
            cyclePulseReadings = 0;
            cycleSawZero = false;
          }
          break;
      }
    }

    // ── Blink active flavor LED ──
    if (now - lastBlinkToggle >= BLINK_INTERVAL_MS) {
      blinkState = !blinkState;
      uint8_t ledPin = (activeFlavor == 0) ? LED_FLAVOR1 : LED_FLAVOR2;
      digitalWrite(ledPin, blinkState ? HIGH : LOW);
      lastBlinkToggle = now;
    }
  } else if (dispensing) {
    // ── Stopped ──
    motorOff(active.pump);
    motorOff(active.valve);
    dispensing = false;
    cyclePhase = CYCLE_IDLE;
    // Restore solid LED for active flavor
    digitalWrite(LED_FLAVOR1, activeFlavor == 0 ? HIGH : LOW);
    digitalWrite(LED_FLAVOR2, activeFlavor == 1 ? HIGH : LOW);
    Serial.printf("Stopped dispensing flavor %d\n", activeFlavor + 1);
  }

  delay(10);
}