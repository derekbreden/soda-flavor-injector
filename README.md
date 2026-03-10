# Soda Flavor Injector

ESP32 + RP2040 project that injects flavoring concentrate into cold carbonated water from an under-counter carbonator. Peristaltic pumps duty-cycle based on real-time flow meter readings to maintain consistent flavor strength. A round LCD display shows which flavor is selected.

<p align="center">
  <img src="docs/photos/display-pepsi-cherry.jpg" width="360" alt="Front panel showing LCD display with Diet Pepsi Cherry logo and air switch buttons">
  <img src="docs/photos/display-mountain-dew.jpg" width="360" alt="Front panel showing LCD display with Diet Mountain Dew logo and air switch buttons">
</p>
<p align="center"><em>The RP2040 round LCD shows the active flavor. An air switch button toggles between flavors.</em></p>

## How It Works

Cold carbonated water flows from an under-counter carbonator through a dispenser faucet. When you open the faucet, a flow meter detects water movement and the system automatically kicks in:

1. A solenoid valve opens (it stays closed between uses to prevent backflow and keep the lines primed)
2. A peristaltic pump injects concentrate from a collapsible reservoir
3. The pump duty-cycles on/off proportionally to the detected flow rate
4. Flavored water exits through a multi-tube dispensing nozzle at the faucet

A toggle switch (an air switch) selects between two flavors. The small LCD display updates to show which flavor is active.

<p align="center">
  <img src="docs/photos/countertop-annotated.jpg" width="500" alt="Kitchen countertop showing soda dispenser faucet and flavor toggle switch">
</p>
<p align="center"><em>The countertop: a dedicated dispenser faucet and a flavor toggle air switch.</em></p>

### Under the Counter

Everything lives inside the sink cabinet:

<p align="center">
  <img src="docs/photos/under-cabinet.jpg" width="600" alt="Under-cabinet view showing CO2 tank, carbonator, concentrate bag, and control panel">
</p>
<p align="center"><em>Left to right: CO2 tank with dual-gauge regulator, Lilium carbonator, Platypus bag filled with concentrate, and the control panel with pumps and valves.</em></p>

The dispenser faucet has silicone tubes running through the gooseneck alongside the water line, delivering concentrate right at the nozzle:

<p align="center">
  <img src="docs/photos/faucet-side.jpg" width="400" alt="Side view of dispenser faucet with silicone tubes bundled along the gooseneck">
  <img src="docs/photos/faucet-nozzle.jpg" width="300" alt="Close-up of faucet nozzle showing multi-tube dispensing design">
</p>

### Architecture

The system runs on two microcontrollers:

- **ESP32** - Main controller. Reads the flow meter, drives pumps and valves via L298N motor drivers, manages the pump state machine, and sends display configuration over UART.
- **RP2040** (Waveshare RP2040-LCD-0.99) - Display controller. Shows the selected flavor logo on a 128x115 round LCD. Reads the same physical toggle switch for instant visual feedback.

```
                        ┌─────────────────────┐
  Carbonated Water ───→ │ Flow Meter (GPIO 23) │
                        └──────────┬──────────┘
                                   │ pulses
                        ┌──────────▼──────────┐
                        │   ESP32 Controller   │
                        │                      │
                        │  Pump State Machine   │
                        │  IDLE → ON → OFF ──→ │──(cycle repeats)
                        │    └── COOLDOWN       │
                        │    └── PRIME (manual)  │
                        └──┬────────┬────────┬─┘
                           │        │        │
                    UART TX│   L298N A  L298N B
                    9600   │   ┌────┴┐  ┌───┴──┐
                           │   │Pump1│  │Pump2 │
              ┌────────────▼┐  │Valve│  │Valve │
              │ RP2040 LCD  │  └─────┘  └──────┘
              │ 128x115 px  │
              │ flavor logo │
              └─────────────┘
```

### Pump Control

The pump doesn't just run at a fixed speed. It duty-cycles (on/off/on/off) with timing that adapts to how fast water is flowing:

| Flow Rate | On Time | Off Time | Duty Cycle |
|-----------|---------|----------|------------|
| Slow (1 pulse/50ms) | 50ms | 600ms | ~8% |
| Full (6 pulses/50ms) | 200ms | 300ms | ~40% |

This is further scaled by a per-flavor **ratio** parameter:
- `FLAVOR_RATIO = 20` — tuned for SodaStream concentrates (1:20 concentrate-to-water)
- `FLAVOR_RATIO = 6` — for bag-in-box syrup (traditional fountain ratio)

## Parts List

Nearly everything was sourced from Amazon Prime. The only exception is the carbonated water machine.

### Electronics

| Part | Purpose |
|------|---------|
| [ESP32-DevKitC-32E](https://www.amazon.com/dp/B09MQJWQN2) | Main controller |
| [ESP32 DIN Rail Breakout Board](https://www.amazon.com/dp/B0BW4SJ5X2) | Clean wiring for ESP32 GPIOs |
| [Waveshare RP2040 Round LCD (0.99")](https://www.amazon.com/dp/B0CTSPYND2) | Flavor display (128x115 GC9107) |
| [L298N Dual H-Bridge Motor Driver](https://www.amazon.com/dp/B0C5JCF5RS) x2 | Drive pumps and solenoid valves |
| [12V 2A Power Supply](https://www.amazon.com/dp/B0DZGTTBGZ) | Powers pumps and valves |

### Pumps and Valves

| Part | Purpose |
|------|---------|
| [Kamoer Peristaltic Pump (400ml/min, 12V)](https://www.amazon.com/dp/B09MS6C91D) x2 | Dispense flavor concentrate |
| [Beduan 12V Solenoid Valve (1/4")](https://www.amazon.com/dp/B07NWCQJK9) x2 | Prevent backflow, keep concentrate lines primed |

### Sensors and Switches

| Part | Purpose |
|------|---------|
| [DIGITEN G3/8" Hall Effect Flow Sensor](https://www.amazon.com/dp/B07QQW4C7R) | Measure water flow rate |
| [KRAUS Garbage Disposal Air Switch (Matte Black)](https://www.amazon.com/dp/B096319GMV) | Flavor toggle (countertop safe, no electricity) |
| [7mm Momentary Push Button (12-pack)](https://www.amazon.com/dp/B0F43GYWJ6) | Prime button (behind panel) |

### Plumbing

| Part | Purpose |
|------|---------|
| [Westbrass Cold Water Dispenser Faucet (Matte Black)](https://www.amazon.com/dp/B0BXFW1J38) | Dispensing tap at the counter |
| [Platypus 2L Collapsible Bottle](https://www.amazon.com/dp/B000J2KEGY) x2 | Flavor concentrate reservoirs |
| [Platypus Hydration Drink Tube Kit](https://www.amazon.com/dp/B07N1T6LNW) | Tubing + bite valve for reservoirs |
| [Silicone Tubing (1/8" ID x 1/4" OD)](https://www.amazon.com/dp/B0BM4KQ6RT) | Food-grade tubing for concentrate lines |
| [Waterdrop 15UC-UF Inline Water Filter](https://www.amazon.com/dp/B085G9TZ4L) | Filters water before carbonation |

### Flavor Concentrates

| Part | Notes |
|------|-------|
| [SodaStream Pepsi Wild Cherry Zero Sugar](https://www.amazon.com/dp/B0G4NRDQB8) | Use FLAVOR_RATIO=20 |
| [SodaStream MTN Dew Code Red](https://www.amazon.com/dp/B0CS191QMW) | Use FLAVOR_RATIO=20 |

### Wiring and Connectors

| Part | Purpose |
|------|---------|
| [Dupont Jumper Wires (120-pack M/F, M/M, F/F)](https://www.amazon.com/dp/B0BRTJXND9) | Board-to-board connections |
| [Female Spade Crimp Terminals (60-pack)](https://www.amazon.com/dp/B0B9MZJ2ML) | Motor and valve connections |
| [Male Quick Disconnect Spade Connectors (100-pack)](https://www.amazon.com/dp/B01MZZGAJP) | Motor and valve connections |
| [Pre-wired 12V LEDs (120-pack, 6 colors)](https://www.amazon.com/dp/B07PVVL2S6) | Flavor indicator LEDs |

### Carbonated Water

| Part | Purpose |
|------|---------|
| [Lilium Under-Sink Carbonated Water Dispenser](https://liliumfaucet.com/products/under-sink-carbonated-soda-maker-sparkling-water-dispenser-with-3-way-faucet) | Cold carbonated water source (not from Amazon) |
| [TAPRITE Dual-Gauge CO2 Regulator](https://www.amazon.com/dp/B00L38DRD0) | CO2 pressure regulation |

## Wiring

<p align="center">
  <img src="docs/photos/panel-closeup.jpg" width="500" alt="Control panel showing ESP32 on DIN rail breakout, two L298N motor drivers, peristaltic pumps, and solenoid valves">
</p>
<p align="center"><em>The control panel: ESP32 on a DIN rail breakout board (top), two L298N motor drivers (red boards), two Kamoer peristaltic pumps, and two solenoid valves (bottom).</em></p>

### ESP32 Pin Assignments

**L298N Board A (Flavor 1):**

| Function | GPIO |
|----------|------|
| ENA (pump PWM) | 33 |
| IN1 (pump dir) | 25 |
| IN2 (pump dir) | 26 |
| IN3 (valve dir) | 27 |
| IN4 (valve dir) | 14 |
| ENB (valve PWM) | 12 |

**L298N Board B (Flavor 2):**

| Function | GPIO |
|----------|------|
| ENA (pump PWM) | 19 |
| IN1 (pump dir) | 18 |
| IN2 (pump dir) | 5 |
| IN3 (valve dir) | 17 |
| IN4 (valve dir) | 16 |
| ENB (valve PWM) | 4 |

**Inputs and Outputs:**

| Function | GPIO | Notes |
|----------|------|-------|
| Flavor toggle switch | 13 | Air switch, INPUT_PULLUP |
| Prime button | 22 | Momentary, INPUT_PULLUP |
| Flow meter | 23 | Hall effect, FALLING edge interrupt |
| Flavor 1 LED | 21 | Steady = selected, blink = dispensing |
| Flavor 2 LED | 15 | Steady = selected, blink = dispensing |
| Display UART TX | 32 | 9600 baud to RP2040 |

### RP2040 Pin Assignments

| Function | GPIO | Notes |
|----------|------|-------|
| Flavor toggle switch | 29 | Same physical switch as ESP32 |
| UART RX (from ESP32) | 26 | PIO-based serial, 9600 baud |
| LCD DC | 8 | Fixed on board |
| LCD CS | 9 | Fixed on board |
| LCD CLK | 10 | Fixed on board |
| LCD DIN | 11 | Fixed on board |
| LCD RST | 13 | Fixed on board |
| LCD Backlight | 25 | Fixed on board |

### Inter-Board Communication

The ESP32 sends a one-way UART message to the RP2040 to configure which flavor image maps to which position:

```
Format: MAP:<image0>,<image1>\n
Example: MAP:0,1\n
```

Image indices: 0 = Diet Wild Cherry Pepsi, 1 = Diet Mountain Dew, 2 = Diet Coke

The RP2040 persists this mapping to flash (LittleFS) so it survives power cycles. The ESP32 resends the mapping every 30 seconds for resilience.

## Building and Flashing

This is a [PlatformIO](https://platformio.org/) project with two build environments.

### Flash the ESP32 (main controller)

```bash
pio run -e esp32dev -t upload
```

### Flash the RP2040 (display)

```bash
pio run -e rp2040_display -t upload
```

The RP2040 uses the [earlephilhower Arduino core](https://github.com/earlephilhower/arduino-pico) and the [GFX Library for Arduino](https://github.com/moononournation/Arduino_GFX) for the GC9107 LCD driver.

### Adding a New Flavor Image

1. Create a 128x115 pixel image
2. Convert it to a C header with a `uint16_t[]` array in RGB565 format
3. Add the header as `src_display/flavor<N>_bitmap.h`
4. Include it in `src_display/main.cpp` and add the pointer to the `bitmaps[]` array
5. Update `FLAVOR1_IMAGE` / `FLAVOR2_IMAGE` in `src/main.cpp` to reference the new index

## Configuration

All tuning parameters are `#define`s at the top of `src/main.cpp`.

### Flavor Ratio

The most important parameter. Controls how much concentrate is injected relative to water flow.

```cpp
#define FLAVOR1_RATIO  20   // SodaStream concentrate (1:20)
#define FLAVOR2_RATIO  20   // SodaStream concentrate (1:20)
```

- **6** = maximum strength, for bag-in-box (BIB) syrup
- **20** = tuned for SodaStream concentrates
- **~24** = minimum strength (hard limit)

### Display Image Mapping

```cpp
#define FLAVOR1_IMAGE   0   // 0=Pepsi, 1=Dew, 2=Coke
#define FLAVOR2_IMAGE   1
```

### Advanced Tuning

These control the pump duty cycle shape and generally don't need adjustment:

```cpp
#define PUMP_ON_MIN_MS     50    // minimum pump on-time
#define PUMP_OFF_MAX_MS  1000    // maximum pump off-time
#define SHAPE_ON_BASE     20     // base on-time at minimum flow
#define SHAPE_ON_SLOPE    30     // on-time increase per flow pulse
#define SHAPE_OFF_BASE   660     // base off-time at minimum flow
#define SHAPE_OFF_SLOPE   60     // off-time decrease per flow pulse
```

## License

MIT
