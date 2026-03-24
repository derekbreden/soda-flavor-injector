# Hopper System and Bag Management Research

## Table of Contents

1. [Hopper/Funnel Design](#1-hopperfunnel-design)
2. [Hopper-to-Bag Fluid Path](#2-hopper-to-bag-fluid-path)
3. [Air Management](#3-air-management)
4. [Capacitive Liquid/Air Detection](#4-capacitive-liquidair-detection)
5. [Bag Mounting](#5-bag-mounting)
6. [Overfill and Spill Containment](#6-overfill-and-spill-containment)
7. [The Platypus Bottle Opening as a Design Constraint](#7-the-platypus-bottle-opening-as-a-design-constraint)
8. [Recommendation](#8-recommendation)
9. [Sources](#9-sources)

---

## 1. Hopper/Funnel Design

### 1a. Location Options

The enclosure sits under a kitchen sink, roughly desktop PC tower size. The user opens the cabinet door and interacts with the hopper to refill flavor concentrate. This happens weekly or more -- it must be the most accessible interaction point on the enclosure.

**Top of enclosure:**
```
    ┌──────────┐  ← cabinet shelf above
    │ ┌──┐     │
    │ │FN│     │  ← funnel on top
    │ └──┘     │
    │          │
    │ enclosure│
    │          │
    └──────────┘
    ─── cabinet floor ───
```
- Gravity assists: concentrate flows downward naturally
- Most natural pour point (tip bottle and pour down)
- Problem: if there's a shelf or plumbing above, clearance may be tight
- Problem: user must reach over/behind the enclosure if it's pushed to the back of the cabinet
- Splash risk: low (pouring downward into a funnel)
- Best if the enclosure is pulled forward to the cabinet door opening

**Front-top corner:**
```
    ┌──────────┐
    │     ┌──┐ │
    │     │FN│←│  ← funnel at front-top edge
    │     └──┘ │
    │          │
    │ enclosure│
    └──────────┘
```
- Accessible when reaching into cabinet (hand goes straight to the front-top)
- Gravity still assists (funnel is high)
- Slight angle for pouring -- user tilts bottle toward the front corner
- Splash risk: moderate (angled pour could overshoot the funnel)
- Good compromise between accessibility and gravity advantage

**Front face with door/flap:**
```
    ┌──────────┐
    │          │
    │  ┌────┐  │  ← door/flap opens to reveal funnel
    │  │ FN │  │
    │  └────┘  │
    │          │
    └──────────┘
```
- Most accessible (directly facing the user when cabinet door opens)
- No gravity advantage -- funnel is at mid-height or lower
- Requires pump-assisted filling (gravity alone won't push concentrate up into a bag above the funnel)
- Adds mechanical complexity (hinge, latch, seal for the door)
- Splash risk: high (pouring horizontally into a front-facing funnel is awkward)

**Side of enclosure:**
- Only practical if enclosure is accessible from the side
- Under-sink cabinets typically have enclosures pushed against a side wall
- Unlikely to be accessible -- skip this option

**Verdict:** Top of enclosure or front-top corner are the only practical locations. Top is simplest if vertical clearance exists (need ~150mm above enclosure for pouring). Front-top corner if the enclosure is pushed to the back of the cabinet.

### 1b. Funnel Design

**Opening diameter:**
- SodaStream syrup bottles (440ml) have a narrow pour spout, approximately 25mm diameter opening
- Bag-in-box syrups (5 gallon) are poured from larger containers or dispensed via tap
- Generic concentrate bottles vary from 25-50mm pour openings
- A funnel opening of **75-100mm (3-4 inches)** catches pours from any of these sources comfortably
- Standard wide-mouth canning funnels are 80-115mm (3.25-4.5") at the top -- this is the right range

**Funnel depth:**
- Deeper funnel catches splashes and accommodates fast pours
- SodaStream bottles hold 440ml; the funnel doesn't need to hold the full bottle, just buffer a fast pour
- A depth of **50-75mm (2-3 inches)** is sufficient, giving a working volume of ~200-400ml depending on diameter
- This prevents overflow if the user pours faster than the pump can pull concentrate through

**Lid/cap when not in use:**
- Absolutely needed. The enclosure is under a sink -- insects, dust, cleaning spray overspray
- A simple twist-off or snap-on cap works
- Could be hinged to the enclosure so it doesn't get lost
- Silicone plug or cap with a lanyard is the simplest approach
- For pump-assisted filling, the cap should include a one-way duckbill valve that lets air in as concentrate drains, keeping the hopper sealed during refill

**Material considerations:**
- **Food-grade silicone** is the best choice: flexible (absorbs impacts), easy to clean, dishwasher safe, transparent/translucent options available, temperature resistant
- **Stainless steel** works but is opaque (user can't see if concentrate is still draining) and heavier
- **PETG (3D printed)** for prototyping -- FDA-compliant PETG filament exists, but surface finish traps residue in layer lines. Acceptable for prototyping, not ideal for production
- Transparent or translucent material is strongly preferred so the user can see the concentrate level in the funnel

**Funnel outlet:**
- The funnel must drain to tubing, not directly into the bag
- The funnel outlet should be a barb fitting (matching the 1/4" OD tubing ecosystem) or a short section of integrated tubing
- A small screen/filter at the funnel outlet catches debris (but adds cleaning burden)
- The funnel outlet connects via tubing to TEE2 on the pump outlet side (see Section 2)

### 1c. Dual Funnels vs Single Funnel

| Approach | Pros | Cons |
|----------|------|------|
| **Two funnels** (one per flavor) | No valve switching, no cross-contamination risk, can refill both simultaneously | 2x enclosure real estate, 2x cleaning |
| **One funnel + selector valve** | Saves space, one cleaning point | Valve adds complexity, cross-contamination risk between flavors if funnel/valve not rinsed, electronic valve needed for automation |
| **One funnel + manual tube swap** | Simplest plumbing | User must reach in and swap tubes -- defeats the purpose of easy refilling |

**Analysis:**

Cross-contamination between flavors through a shared funnel is a real concern. If the user refills cola and then switches to lemon-lime, cola residue in the funnel contaminates the lemon-lime. The funnel would need flushing between flavors.

With two funnels, each funnel is dedicated to one flavor. No flushing needed between refills. The only downside is space -- two 100mm diameter funnels need ~250mm (10") of enclosure top width, which is achievable in a desktop-tower-size enclosure.

**Verdict:** Two funnels. The space cost is small, the UX improvement is large (no flavor mixing, no valve switching, no flushing), and it aligns with the two-bag, two-pump architecture.

---

## 2. Hopper-to-Bag Fluid Path

### 2a. The Dip Tube: Why the Bag Is a Sealed Vessel

The Platypus Drink Tube Kit threads a sealed cap onto the bag's 28mm opening. A dip tube (1/4" ID, 6.35mm polyurethane tube) extends from the cap into the bag interior. The bag is completely sealed except for flow through this dip tube. There is no "open pouch" -- fluid entering the tubing MUST travel through the dip tube into the bag. It cannot bypass the bag.

This is proven by two existing operations:
- **Priming** pulls air OUT of the bag through the dip tube (suction propagates through the sealed path)
- **Clean fill** pushes water INTO the bag through the dip tube (pressure drives fluid through the sealed path)

Both demonstrate that the sealed dip tube path works bidirectionally. See [dip-tube-analysis.md](dip-tube-analysis.md) for detailed specifications and flow dynamics.

### 2b. Pump-Assisted Filling (Recommended)

The Kamoer KPHM400 peristaltic pumps are reversible DC motors driven by L298N H-bridges. Reversing the pump is a 3-line firmware change (swap IN1/IN2). By connecting the hopper downstream of the pump (at the pump outlet via a tee), reversing the pump during refill pulls concentrate from the hopper and pushes it through the dispensing solenoid, through TEE1, through the dip tube, and into the bag.

**Topology:**

```
COMPLETE TOPOLOGY (per flavor line):

                              ┌─── Bag (sealed, dip tube)
                              │
                            TEE1 ← [CLEAN SOL] ← needle valve ← tap water
                              │
                       [DISP SOL]
                              │
                           [PUMP]
                              │
                            TEE2
                           /     \
              [HOPPER SOL]     [CHECK VALVE]
                    │                │
                 hopper        dispensing point
```

### 2c. How Refill Mode Works

1. Dispensing solenoid: **OPEN** (pump pushes fluid through it toward the bag)
2. Hopper valve: **OPEN**
3. Check valve at dispensing point: passively **CLOSED** (blocks air from entering through the dispensing point)
4. Clean solenoid: **CLOSED**
5. Pump: **REVERSED**

Flow path:
```
Hopper → [hopper valve OPEN] → TEE2 → [pump REVERSED] → [disp solenoid OPEN] → TEE1 → Bag (via dip tube)
```

The reversed pump creates suction on the TEE2 side (pulling from hopper) and positive pressure on the dispensing solenoid side (pushing toward TEE1 and into the bag). The check valve at the dispensing point prevents the pump from pulling air through the dispensing point instead of concentrate from the hopper.

### 2d. Why a Check Valve at the Dispensing Point

During refill with the pump reversed, the pump's inlet is at TEE2, which has two branches: the hopper (valve open) and the dispensing point. Without a check valve, the pump would pull air from the dispensing point (open to atmosphere) instead of concentrate from the hopper. A 1/4" inline check valve oriented to allow flow OUT (toward dispensing point) but block flow IN solves this.

During normal dispensing, the pump runs forward and pushes concentrate through the check valve to the dispensing point. The check valve opens in the forward direction with low cracking pressure (0.5-2 PSI) -- well within the Kamoer pump's capability (~8 PSI). The check valve is transparent to normal dispensing.

### 2e. Alternatives Considered

**Option A: Hopper tees into the pump outlet side (the recommended approach)** -- described above. Lowest cost ($6-10 for check valves), uses existing pumps, fastest fill time (400 ml/min pump at 2-5 PSI).

**Option B: Dedicated fill pump per line** -- a small peristaltic pump on each hopper line pushes concentrate from the hopper into the bag through the bag-side tee (TEE1). Works, but adds 2 pumps ($10-24), 2 drivers, 2 GPIOs, and enclosure space. All for a function that runs a few minutes per week.

**Option C: Gravity fill through the bag-side tee** -- hopper connects at TEE1 (bag side). Dispensing solenoid closed, hopper valve open, pump off. Gravity drains concentrate from the hopper into the bag. This was the original recommended approach, but has three disadvantages:
1. **Slow:** 15-30 minutes for 1L (counter-flow limited through the dip tube's 6.35mm bore) vs 5-12 minutes with pump assist
2. **Open air system:** Every refill exchanges the bag atmosphere with room air through the open funnel. Over months of weekly refills, this introduces repeated microbial exposure and volatile flavor loss
3. **Counter-flow bottleneck:** With only ~0.5 PSI of gravity driving pressure, the air counter-flow through the dip tube is the primary flow limiter. The pump's 2-5 PSI makes a material difference

Gravity fill remains an acceptable fallback if pump reversal proves problematic in testing.

### 2f. Complete Plumbing Diagram

```
COMPLETE SYSTEM (per flavor line):

         [Funnel]
            │
         [SEN-A]           ← capacitive sensor (hopper empty detect)
            │
       [HOPPER SOL]        ← Beduan 12V NC solenoid
            │
            │              ┌─── Bag (sealed, dip tube inside)
            │              │
            │            TEE1 ← [CLEAN SOL] ← needle valve ← tap water
            │              │
            │        [DISP SOL]
            │              │
            │           [PUMP]
            │              │
            └─────────── TEE2
                           │
                     [CHECK VALVE]   ← allows flow OUT only (toward dispensing point)
                           │
                        [SEN-B]      ← capacitive sensor (primed/purged detect)
                           │
                    dispensing point
```

### 2g. Valve State Table

| Mode | Hopper Sol | Clean Sol | Disp Sol | Check Valve | Pump | Flow Path |
|------|-----------|-----------|----------|-------------|------|-----------|
| **Idle** | CLOSED | CLOSED | CLOSED | N/A | OFF | No flow |
| **Dispensing** | CLOSED | CLOSED | OPEN | passes flow | FORWARD | Bag → TEE1 → disp sol → pump → TEE2 → check valve → disp point |
| **Hopper Refill** | OPEN | CLOSED | OPEN | blocks air | REVERSE | Hopper → TEE2 → pump(rev) → disp sol → TEE1 → Bag (via dip tube) |
| **Clean Fill** | CLOSED | OPEN | CLOSED | N/A | OFF | Tap water → clean sol → TEE1 → Bag (water pressure) |
| **Clean Flush** | CLOSED | CLOSED | OPEN | passes flow | FORWARD | Bag → TEE1 → disp sol → pump → TEE2 → check valve → disp point |
| **Prime** | CLOSED | CLOSED | OPEN | passes flow | FORWARD | Bag(air) → TEE1 → disp sol → pump → TEE2 → check valve → disp point |
| **Hopper Line Flush** | OPEN | CLOSED | OPEN | passes flow | FORWARD | Hopper(water) → TEE2 → check valve → disp point |

### 2h. Pump Reversal Implementation

The existing L298N H-bridges support bidirectional motor control natively:

```cpp
// Current forward: IN1=HIGH, IN2=LOW
void pumpOn(const PumpChannel& m, uint8_t speed) {
  digitalWrite(m.in1, HIGH);
  digitalWrite(m.in2, LOW);
  analogWrite(m.ena, speed);
}

// Reverse: swap IN1/IN2
void pumpReverse(const PumpChannel& m, uint8_t speed) {
  digitalWrite(m.in1, LOW);
  digitalWrite(m.in2, HIGH);
  analogWrite(m.ena, speed);
}
```

No hardware changes needed for pump reversal itself.

### 2i. Fill Rate Estimate

With the pump providing 2-5 PSI of driving pressure, concentrate flows through the dip tube (6.35mm ID) and into the bag. The fill rate is faster than gravity (~0.5 PSI) but still limited by counter-current air displacement through the dip tube (see Section 3).

Estimated fill time for 1L: **5-10 minutes** with pump assist (vs 8-15 minutes gravity-only).

### 2j. Sealed Hopper Design

The hopper should be sealable to minimize air exchange with the bag during filling:

```
  [silicone cap with duckbill/check valve]
           │
     ┌─────┴─────┐
     │   FUNNEL   │  ← food-grade silicone or PETG
     │            │
     └─────┬──────┘
           │
     [hopper valve solenoid]
           │
         TEE2
```

The duckbill valve lets air INTO the hopper as concentrate drains (preventing vacuum), but prevents hopper air from flowing outward. This keeps the system sealed during refill -- no ambient air enters the bag side.

**Refill procedure:**
1. Remove silicone cap
2. Pour concentrate into funnel
3. Replace silicone cap (cap has duckbill valve for air inlet)
4. Initiate refill via S3 touchscreen or iOS app
5. Firmware opens hopper valve, reverses pump, opens dispensing solenoid
6. Concentrate flows: hopper → hopper valve → TEE2 → pump(rev) → disp sol → TEE1 → dip tube → bag
7. Capacitive sensor on hopper line detects air (funnel empty) → firmware stops pump, closes valves
8. Done. Bag is filled, minimal air exchange, system sealed.

---

## 3. Air Management

### 3a. The Sealed System

The dip tube + cap creates a sealed bag with a single port. When the dispensing solenoid is closed, the bag interior is completely isolated from atmosphere. No air can enter (freshness preserved), and no liquid can leak out.

```
    SEALED SYSTEM:

    [solenoid valve] ← CLOSED = sealed system
         │
    black silicone tube
         │
    blue PU tube
         │
    ┌────┴────┐
    │   cap   │ ← friction seal
    └────┬────┘
         │ dip tube (6.35mm ID)
    ╔════╧════════╗
    ║ BAG INTERIOR║ ← sealed volume, no air exchange
    ║  (liquid +  ║    with atmosphere when valve closed
    ║   trapped   ║
    ║   air)      ║
    ╚═════════════╝
```

When the dispensing solenoid opens and the pump runs forward:
- The pump creates suction, pulling liquid through the dip tube
- The bag collapses under atmospheric pressure (like squeezing a sealed juice box with a straw)
- No air enters through the dip tube -- the tube is full of liquid
- The bag is never exposed to ambient air during normal dispensing

This sealed behavior is excellent for freshness. Concentrate stays sealed for weeks, similar to a bag-in-box wine system.

### 3b. During Pump-Assisted Refill (Sealed Hopper)

When the pump pushes concentrate into the bag through the dip tube during refill, the bag is a sealed vessel. Displaced air inside the bag has nowhere to go:

1. Concentrate enters via the dip tube opening inside the bag
2. The bag inflates as liquid enters
3. Air inside the bag compresses as the bag fills
4. Eventually, air back-pressure equals the pump's pushing pressure, and filling slows

At 2-5 PSI pump pressure, Boyle's law gives: air compresses from ~1L to ~0.85-0.9L. The bag fills to approximately 85-90% capacity on a single fill cycle. The remaining 10-15% is compressed air trapped in the bag.

**Getting above 90% fill:**

1. **Accept 85-90% fill.** A 1L bag holds 850-900 ml. The user refills slightly more frequently.
2. **Two-phase fill:** Pump concentrate in (compressing air), briefly open the hopper valve and hopper lid to vent the compressed air (it vents back through the dip tube, through tubing, out the hopper), seal and pump again. Each cycle removes more air.
3. **Pre-fill air removal:** Before pouring concentrate, use the prime operation to pull air out of the bag through the dispensing path. The bag starts with less air, so more concentrate fits.

### 3c. During Normal Operation (Dispensing)

As the bag empties during dispensing, the dip tube provides two advantages:

1. **Anti-pinch spine:** The semi-rigid PU tube (3/8" OD, 85-95 Shore A) maintains an open channel inside the bag even when the bag walls collapse. The bag cannot completely occlude the flow path along the tube's length. This makes bag-pinch-related sputtering less likely than with a simple open connector.

2. **High-point drainage:** With the bag mounted on an incline (connector at low end, see Section 5), the dip tube extends upward from the connector into the bag interior. The tube opening is one of the last points to be exposed to air as the bag drains. Air only reaches the dip tube opening when the liquid level drops below it -- near the last 5-10% of bag volume.

The dispensing solenoid seals the line between dispense cycles. No air enters from the bag side while idle.

### 3d. During Clean Cycle

**Fill phase:** Water enters the bag through the dip tube from the clean solenoid (house water pressure through needle valve). The bag is initially collapsed (mostly air). Water exits the dip tube opening inside the bag and cascades downward, contacting more interior surface than if it simply pooled at the connector. This "shower head" effect actually helps cleaning.

Air is trapped above the rising water level. Since the only exit path is back through the dip tube (which is full of incoming water), the air compresses until equilibrium. The bag fills partially -- not to 100% capacity.

**Flush phase:** The pump pulls water + dissolved residue through the dip tube. Trapped air gets pulled through once the water level drops below the tube opening.

**Multi-cycle cleaning:** Each fill+flush cycle removes more residual air and concentrate. Three cycles (CLEAN_CYCLES = 3) provides adequate dilution. The dip tube's ~11 ml dead volume is flushed progressively -- after 3 cycles the dilution factor is roughly (11/1000)^3 = negligible.

### 3e. Dead Volume in the Hopper Tubing

```
Funnel → ~300mm of 1/4" OD tubing → [hopper valve] → ~50mm → TEE2
```

- 1/4" OD tubing with ~4.5mm ID: cross-section area = 15.9 mm^2
- 300mm run: dead volume = ~4.8 ml
- This concentrate remains in the hopper tubing after the funnel empties
- Trapped between the hopper valve and the empty funnel
- Not a significant waste, but could grow bacteria if left for weeks
- Clean cycle should flush this line too (open hopper valve during clean flush -- see "Hopper Line Flush" mode in Section 2g)

---

## 4. Capacitive Liquid/Air Detection

### How It Works

Capacitive liquid detection exploits the dramatic difference in dielectric constant between air (~1) and water-based liquids (~80). A capacitive sensor wrapped around (or clamped onto) a non-metallic tube measures the capacitance of the tube contents. When liquid is present, capacitance is ~80x higher than when air is present. This enormous signal margin makes detection reliable and binary -- no calibration needed for "is there liquid or air?"

Sugar/flavoring concentration doesn't matter for detection. The question is only "liquid vs air," not "what kind of liquid."

### Sensor Options

**Option 1: XKC-Y25-T12V (DFRobot SEN0204) -- for containers/pipes OD >= 11mm**

| Spec | Value |
|------|-------|
| Voltage | 5-24V DC |
| Output | Digital (high/low), 1-50mA |
| Response time | 500ms |
| Wall penetration | Up to 20mm non-metallic |
| Waterproof | IP67 |
| Sensitivity | 4 levels, adjustable via SET button |
| Price | ~$8-12 |

This sensor is designed for containers and large pipes. The 1/4" OD tubing (6.35mm OD) is too small for this sensor. It would work on the funnel/hopper reservoir itself (detecting whether the hopper still has liquid) but not on the tubing.

**Option 2: DFRobot SEN0370 -- for small pipes OD <= 10mm**

| Spec | Value |
|------|-------|
| Voltage | 5-24V DC |
| Output | Digital (switch) |
| Response time | 500ms |
| Tube OD compatibility | <= 10mm |
| Accuracy | +/- 1.5mm |
| Waterproof | IP65 |
| Price | ~$8-12 |

This is the right size for the 1/4" OD tubing (6.35mm). Clamps onto the tube and detects liquid vs air. Digital output -- connects directly to an ESP32 GPIO or MCP23017 input.

Available from: [DFRobot](https://www.dfrobot.com/product-2116.html), DigiKey, Amazon

**Option 3: DFRobot SEN0368 -- for containers/pipes OD >= 11mm**

Similar to SEN0204 but with IP65 rating and 5-12V range. Same limitation: too large for 1/4" tubing. Could work on the hopper funnel wall or a larger reservoir.

**Option 4: FDC1004 (TI) -- 4-channel capacitance-to-digital I2C chip**

| Spec | Value |
|------|-------|
| Interface | I2C (address 0x50) |
| Channels | 4 |
| Measurement range | +/- 15 pF |
| Resolution | 0.5 fF |
| Max offset capacitance | 100 pF |
| Price | ~$15-20 (breakout board from ProtoCentral) |

The FDC1004 is a precision capacitance measurement IC. Instead of a self-contained module with digital output, this requires copper tape electrodes wrapped around the tubing, connected to the FDC1004 inputs. The ESP32 reads capacitance values over I2C and applies threshold logic in firmware.

**Advantages over standalone modules:**
- 4 channels on one chip (exactly 2 flavors x 2 sensing points)
- I2C -- no GPIO pins consumed (shares bus with DS3231 RTC at 0x68)
- More flexible -- electrode placement and size can be customized
- Higher resolution -- can potentially measure partial fill or flow rate, not just binary

**Disadvantages:**
- More assembly work (copper tape electrodes, shielding)
- Requires firmware calibration
- Breakout board adds a PCB to the system

Available from: [ProtoCentral](https://protocentral.com/product/protocentral-fdc1004-capacitance-sensor-breakout-board/), DigiKey, Mouser

### Tube Material Compatibility

Capacitive sensing works through any non-metallic tube wall:
- **Silicone** (current system): excellent. Low dielectric constant (~3-4), thin wall, flexible. Signal passes through easily.
- **BPT** (pump tubing): works fine. Similar dielectric properties to other plastics.
- **Nylon/PE** (Platypus bag material): works fine.
- **PVC/hard tubing**: works fine.
- **Metal tubing**: does NOT work. Metal shields the capacitive field.

All tubing in this system is non-metallic, so capacitive sensing is fully compatible.

### Response Time

All the off-the-shelf modules have ~500ms response time. At pump flow rates of 400 ml/min = 6.67 ml/s, in 500ms the pump moves ~3.3 ml. In 1/4" OD tubing (4.5mm ID, 15.9 mm^2 cross-section), 3.3 ml = 208mm (8.2") of tube length.

This means there's roughly **200mm of "overshoot"** -- the distance fluid or air travels past the sensor before the sensor triggers. This dead volume after the sensor is important for air management:
- Place the sensor 200+mm before the point where air must not enter
- Or: place the sensor, and when it detects "air," the pump has already pushed ~3.3 ml of air past the sensor
- For hopper-empty detection, this is fine (a few ml of air in the hopper line is acceptable, and the hopper valve closes)
- For tap-side priming detection, the 200mm overshoot means the sensor should be placed well before the tap

### Recommended Sensor Placement

```
                                         ┌─── Bag
                                         │
  Funnel → [SENSOR A] → [hopper valve] → TEE2 side (via pump → disp sol → TEE1)

                              [pump]
                                │
                              TEE2
                                │
                     [SENSOR B] → [check valve] → dispensing point
```

- **Sensor A (hopper line):** Detects when the funnel has drained (liquid-to-air transition). Placed between the funnel outlet and the hopper valve. When air is detected, the system knows the refill is complete. The hopper valve can close and the pump stops.
- **Sensor B (dispensing line):** Detects priming status (air-to-liquid = primed) and clean cycle completion (liquid-to-air = line purged). Placed between TEE2 and the dispensing point (after the check valve).

Both sensors need to work on 1/4" OD tubing (6.35mm). The **DFRobot SEN0370** (OD <= 10mm) or **FDC1004 with copper tape** are the right choices.

### Recommended Approach

**Use the FDC1004 breakout board.** Rationale:
- 4 channels covers all sensing points (2 hopper + 2 tap, one per flavor)
- I2C bus already exists (GPIO 21/22 with DS3231 at 0x68)
- Zero additional GPIO pins consumed
- Higher precision than standalone modules
- Aligns with the expansion architecture documented in future-sensing-and-gpio.md
- One PCB, one I2C address, all sensing handled

**Fallback:** If the FDC1004 is too complex to set up initially, use 4x DFRobot SEN0370 modules. They need 4 GPIO inputs, which can come from MCP23017 digital inputs (also I2C, also zero ESP32 GPIO).

---

## 5. Bag Mounting

### 5a. Incline Two-Point Stretch Mount (Recommended)

Each Platypus bag mounts at an 18-20 degree incline between two fixed points: the sealed top end elevated at the rear and the connector/outlet end at the low front. The bag is held taut under mild tension, filling the available space diagonally rather than consuming height vertically.

```
    SIDE VIEW — one bag at 18-20 degrees

    ═══════════ DOCK SHELF (180mm) ═══════════════
                                         * sealed end (high)
                                        /  J-hook + binder clip
                                       /
                      BAG            /    18-20 deg
                                    /
                                   /
                         * connector (low)
                           U-clip on front wall
    ═══════════ FLOOR ════════════════════════════
    FRONT                                    BACK
```

**Key advantages over vertical hanging:**
- Uses diagonal space, dramatically reducing vertical height requirement (needs ~140mm vs 250mm+)
- Bag is held between two fixed points, constraining collapse to thinning-in-place rather than random folding
- The connector end is at the lowest point, so gravity pulls all liquid toward the outlet

**Key advantages over flat cradles:**
- Gravity still assists drainage (sin(20) = 0.34 of gravitational acceleration drives liquid toward the connector)
- The dip tube extends from the low connector upward along the incline, reaching deeper into the bag interior
- Clear and predictable drainage behavior

### 5b. Geometry

At 18-20 degrees with 1L Platypus bags (250mm long):

| Angle | Vertical Rise (mm) | Horizontal Run (mm) | Total Vertical w/ Thickness |
|---|---|---|---|
| 18 deg | 77 | 238 | ~115 |
| 20 deg | 85 | 235 | ~123 |

Available bag zone: 165mm tall (15mm floor to 180mm dock shelf), 242mm deep.

### 5c. Two-Bag Arrangement

Two bags at 18 degrees, stacked vertically, both running front-to-back:

```
    SIDE VIEW — Two 1L bags at 18 degrees, stacked

    ═══════════════ DOCK SHELF (180mm) ═══════════════
                                            ·  ← 11mm clearance
                                          * BAG 2 sealed (169mm top)
                                         /
                              BAG 2     /    18 deg
                                       /
                              * conn. /
                             (92mm)  * ← BAG 2 connector (73mm center)
                     ─ ─ ─ ─ ─ ─ ─ ─ ─  5mm gap
                            * BAG 1 sealed (126mm top)
                           /
                BAG 1     /    18 deg
                         /
                * conn. /
               (49mm)  * ← BAG 1 connector (30mm center)

    ═══════════════ FLOOR (4mm) ═══════════════
    FRONT                                        BACK
    ◄────────────── 238mm run ──────────────────►
```

| Parameter | Value |
|---|---|
| Incline angle | 18-20 degrees from horizontal |
| Bag 1 connector height | ~30mm from floor |
| Bag 1 sealed end height | ~107-115mm from floor |
| Bag 2 connector height | ~73-78mm from floor |
| Bag 2 sealed end height | ~150-163mm from floor |
| Horizontal run (each bag) | 235-238mm |
| Top clearance to dock shelf | 11-17mm |
| Width consumed | 140mm of 272mm (centered, 66mm per side for tubing) |

### 5d. Drainage Behavior

As the bag empties, the remaining liquid collects at the low (connector) end of the incline. The bag collapses from the high (sealed) end downward because:

1. Atmospheric pressure acts uniformly on the bag exterior
2. Liquid weight creates higher internal pressure at the low end
3. The high end loses liquid first (gravity drains it downward along the incline)
4. The bag film at the high end has no liquid behind it, so it collapses inward

The incline mount constrains collapse to thinning (the two faces of the bag coming together) rather than random folding, because the bag is under mild tension between two fixed points. This is the ideal collapse mode.

**The dip tube advantage on incline:** The tube extends from the low connector end upward into the bag interior along the incline. The tube opening is positioned partway up the incline, in the middle of the liquid volume. As the bag drains, the dip tube opening is one of the last points to be exposed to air -- the liquid must drain past the tube opening before air reaches it. This provides a natural buffer: even if the upper portion of the bag has collapsed, the dip tube opening remains submerged in the liquid pool at the lower portion. Air only reaches the tube opening when the bag is nearly empty (last 5-10%).

**The dip tube anti-pinch spine:** Even if the bag walls fully collapse, the semi-rigid PU tube (3/8" OD) holds the bag walls ~9.5mm apart along its length, maintaining a guaranteed minimum flow channel. The bag cannot completely occlude the flow path along the tube.

**Kinking analysis:** On a stretched incline mount, kinks above the liquid line are irrelevant (no liquid trapped above). Kinks below the liquid line are self-healing (liquid weight pushes past the fold). The stretched mount prevents severe kinking because the bag material is under mild tension along its length axis, resisting lateral folding.

### 5e. Mounting Hardware

**Connector end (low mount):** A 3D-printed snap-fit U-clip on the enclosure front wall interior. The 28mm threaded cap sits in the clip, held by friction and gravity. Quick, one-handed installation.

**Sealed end (high mount):** A 50mm binder clip (~$0.25) grips the heat-sealed seam. The clip's handles fold up and hook onto a 3D-printed J-hook on the rear wall or dock shelf underside.

**Installation sequence:**
1. Attach binder clip to bag's sealed seam
2. Hang binder clip on J-hook at rear
3. Route connector down toward front
4. Push connector/cap into U-clip on front wall
5. Connect tubing

**Estimated installation time: 15-30 seconds per bag.**

### 5f. Alternatives Considered

The following mechanical solutions for bag collapse were analyzed in prior research and are superseded by the incline mount approach:

| Solution | Why Superseded |
|---|---|
| Gravity only (vertical hanging) | Requires 250mm+ vertical space; 400mm enclosure can't fit bags vertically without folding |
| Elastic frame / bag squeezer | The incline two-point stretch provides equivalent constraint without springs or elastic bands |
| Roller / wiper bar | The incline provides natural top-down drainage; a roller adds mechanical complexity with no benefit |
| Rigid channel / cradle | Could complement the incline but adds bulk and complicates bag installation |
| Bottom weight | Puts connector at top, which is terrible for air management |

The incline mount effectively combines "gravity alone" with "elastic frame" -- the bag is constrained by its mounting points, and gravity drives drainage in the right direction.

See [incline-bag-mounting.md](incline-bag-mounting.md) for detailed geometry, clearance analysis, and tubing routing.

---

## 6. Overfill and Spill Containment

### Overfill Scenarios

**User pours too much concentrate into the hopper:**
- The funnel has a fixed volume (~200-400 ml depending on size)
- If the user pours more than the funnel can hold, it overflows out the top
- Mitigation: clear/translucent funnel so user can see the level. Funnel lip to catch minor overflows. User instruction: "pour slowly, wait for funnel to drain."

**Bag is already mostly full and user overfills:**
- Pump reverses and pushes concentrate into the bag through the dip tube
- The bag has a maximum capacity of 1L
- If the bag is nearly full, back-pressure from compressed air inside the sealed bag increases rapidly
- The pump stalls against the back-pressure -- no catastrophic failure
- The capacitive sensor on the hopper line can detect that concentrate has stopped flowing (sensor stays "liquid" for an extended period), signaling "bag full"

**Pump fails to stop (sensor failure):**
- If the pump continues running against a full bag, it simply stalls against back-pressure
- The sealed bag cannot rupture from pump pressure (peristaltic pumps generate 2-5 PSI; the bag and tubing joints tolerate this)
- Worst case: the pump runs until firmware timeout (clean cycle model -- REFILL_TIMEOUT_MS)

### Spill Containment

**Around the hopper:**
- The funnel area on top of the enclosure should have a raised lip or drip ring
- If concentrate spills while pouring, it pools in the lip instead of running down the enclosure side
- A small moat around the funnel base (5mm deep, 10mm wide) catches most drips
- Hopper overflow runs down the OUTSIDE of the enclosure, not into the interior

**Inside the enclosure:**
- The sealed fluid path means no liquid is exposed to the enclosure interior during normal operation
- The only realistic internal leak scenarios are assembly defects (loose zip tie at bag connector) or catastrophic failures (bag rupture, fitting blowout)
- Assembly defects are prevented at the source: use hose clamps instead of zip ties for production
- Catastrophic failures (bag rupture = up to 1L of sticky syrup) overwhelm any internal drip containment

**External secondary containment:**
- Place a removable silicone mat or tray on the cabinet floor beneath the enclosure
- This matches industry practice for under-sink water filtration systems
- More effective and cheaper than an internal drip tray, which costs 15mm of critical vertical space

See [drip-tray-shelf-analysis.md](drip-tray-shelf-analysis.md) for detailed analysis of every liquid scenario and why internal drip trays are not justified for this system.

### Spill Detection

- A simple water/leak sensor on the cabinet floor could alert the user
- Low-priority feature -- could be a future MCP23017 input or FDC1004 channel

---

## 7. The Platypus Bottle Opening as a Design Constraint

### Thread Specifications

The Platypus bottle uses a **28mm thread**, the same standard used by most plastic soda bottles worldwide (Pepsi products, Smart Water, Evernew, etc.). This is a common "28mm" or "28-410" thread finish in the packaging industry.

- Outer diameter of the thread: ~28mm
- Opening inner diameter: approximately 21-22mm (the actual hole through which liquid flows)
- Thread pitch: described as 5/32" by some sources

### Available Caps and Adapters

- **Platypus Closure Cap**: standard screw-on cap (replacement available from Cascade Designs)
- **Platypus Drink Tube Kit**: threaded cap with integrated dip tube (1/4" ID PU tube passes through the cap's central bore). This is the current connection method.
- **28mm to barb adapters**: available from various sources since 28mm is a standard size. However, the Platypus thread reportedly has a 5/32" pitch, which may differ slightly from other "28mm" bottles.
- **Sawyer filter adapters**: Sawyer uses a different 4/32" pitch, so Sawyer adapters are NOT compatible with Platypus bottles.

### Flow Analysis Through the Dip Tube

The relevant flow restriction is NOT the 21mm bag opening -- fluid never flows through the raw opening. All fluid passes through the dip tube's 6.35mm ID bore.

**Dispensing flow:** The dip tube (6.35mm ID) is not the bottleneck. The bottleneck is the black silicone tubing at 1/8" ID (3.175mm):

| Tube Segment | ID (mm) | Cross-Section (mm^2) | Relative Flow Capacity |
|---|---|---|---|
| Blue PU dip tube | 6.35 | 31.7 | 4x silicone |
| Black silicone tube | 3.175 | 7.9 | 1x (bottleneck) |
| Platypus bag opening | ~21 | ~346 | 44x silicone (irrelevant -- fluid goes through dip tube) |

The dip tube contributes less than 4% of total flow resistance during dispensing.

**Filling flow (pump-assisted):** During refill with the pump reversed, concentrate enters the dip tube from below and exits at the tube opening inside the bag. The 6.35mm bore is the primary flow restriction for filling. At 2-5 PSI pump pressure, the flow rate through the tube is adequate (estimated 100-200 ml/min accounting for counter-flow air effects).

**Air management during fill:** With pump-assisted filling through a sealed hopper, air displaced from the bag compresses in place (see Section 3b). There is no counter-current air flow through the dip tube during pump-assisted fill because the system is sealed -- air has no exit path. This eliminates the counter-flow bottleneck that limits gravity fill.

With an unsealed hopper (gravity fill fallback), air must counter-flow through the 6.35mm dip tube against incoming concentrate. In a 6.35mm tube, liquid fills the entire cross-section, blocking air passage. Fill proceeds in slow slug-flow mode. Estimated gravity fill time for 1L: 8-15 minutes.

### Could a Different Bag Be Used?

If the Platypus bag + dip tube proves too restrictive:

**Wider-mouth collapsible bags:**
- CNOC Vecto (28mm thread, same standard) -- similar constraints
- Custom bag with two ports (fill port + vent port) -- solves the air counter-flow problem but requires custom fabrication
- Medical-style IV bags -- have spike ports designed for controlled flow, but smaller capacity (typically 500ml-1L)

**Custom reservoir instead of a bag:**
- A rigid collapsible container (accordion/bellows style) with separate fill and drain ports
- More complex to source/fabricate, but eliminates the single-opening constraint
- Could be 3D printed (food-safe PETG bellows)

**Verdict:** Stick with the Platypus 1L bags and the Drink Tube Kit. The dip tube's sealed path provides significant advantages (freshness, anti-pinch spine, high-point drainage) that outweigh the fill-rate limitation. Pump-assisted filling through the sealed hopper largely mitigates the fill time concern.

---

## 8. Recommendation

### Recommended Filling Architecture

**Pump-assisted filling through TEE2 (pump outlet side), pump reversed.** The hopper connects at TEE2 between the pump outlet and the dispensing point. A check valve at the dispensing point prevents air ingress during refill. The sealed hopper (duckbill valve cap) minimizes air exchange with the bag.

### Recommended Bag Mounting

**Incline two-point stretch at 18-20 degrees from horizontal.** Connector at front-low, sealed end at rear-high. Two bags stacked vertically within the 165mm bag zone. U-clip at front wall, binder clip + J-hook at rear.

### Recommended Hopper Location

**Top of enclosure, one funnel per flavor.** Two funnels, each ~75-100mm diameter, with snap-on silicone lids (duckbill valve for air inlet during refill). Positioned so the user can reach them when the cabinet door is open.

### Recommended Plumbing

```
COMPLETE SYSTEM (2 flavor lines):

         [Funnel 1]         [Funnel 2]
             │                    │
         [SEN-A1]            [SEN-A2]       ← capacitive sensor (hopper empty detect)
             │                    │
        [HOPPER SOL 1]      [HOPPER SOL 2]  ← Beduan 12V NC solenoid
             │                    │
             │    ┌── BAG 1 ──┐  │    ┌── BAG 2 ──┐
             │    │ (dip tube) │  │    │ (dip tube) │
             │    └─── TEE1a ──┘  │    └─── TEE1b ──┘
             │         │          │         │
             │  [CLEAN SOL 1]     │  [CLEAN SOL 2]
             │      │             │      │
             │   needle valve     │   needle valve
             │      │             │      │
             │   tap water ───────┘   tap water
             │                        │
             │  [DISP SOL 1]     [DISP SOL 2]
             │      │                  │
             │   [PUMP 1]          [PUMP 2]
             │      │                  │
             └── TEE2a                TEE2b ──┘
                  │                    │
           [CHECK VALVE 1]      [CHECK VALVE 2]
                  │                    │
             [SEN-B1]            [SEN-B2]       ← capacitive sensor (primed/purged detect)
                  │                    │
           dispensing point      dispensing point
```

### Valve States for All Operating Modes

| Mode | Hopper Sol | Clean Sol | Disp Sol | Check Valve | Pump | Notes |
|------|-----------|-----------|----------|-------------|------|-------|
| Idle | CLOSED | CLOSED | CLOSED | N/A | OFF | |
| Dispensing | CLOSED | CLOSED | OPEN | passes flow | FWD | Normal operation |
| Hopper Refill | OPEN | CLOSED | OPEN | blocks air | REV | Pump reversed, sealed hopper |
| Clean Fill | CLOSED | OPEN | CLOSED | N/A | OFF | Water pressure fills bag |
| Clean Flush | CLOSED | CLOSED | OPEN | passes flow | FWD | Pump empties bag to dispensing point |
| Clean Air Purge | CLOSED | CLOSED | OPEN | passes flow | FWD | Pump runs on empty bag, blows air through line |
| Hopper Line Flush | OPEN | CLOSED | OPEN | passes flow | FWD | During clean cycle, flush hopper tubing too |
| Prime | CLOSED | CLOSED | OPEN | passes flow | FWD | Pull air out of system |

### Hardware Shopping List (New Items)

| Item | Qty | Est. Price | Source |
|------|-----|-----------|--------|
| Beduan 12V 1/4" NC solenoid valve (hopper) | 2 | $18 | Amazon (B07NWCQJK9) |
| 1/4" inline check valve (push-connect) | 2 | $6-10 | Amazon |
| FDC1004 breakout board (ProtoCentral) | 1 | $18 | ProtoCentral / DigiKey |
| IRLZ44N N-channel MOSFET (hopper sol. driver) | 2 | $2 | Amazon/DigiKey |
| Food-grade silicone funnel, ~100mm opening | 2 | $10 | Amazon |
| Silicone plug/cap for funnels (w/ duckbill valve) | 2 | $5 | Amazon |
| Copper tape (for FDC1004 electrodes) | 1 roll | $8 | Amazon |
| TEE fittings 1/4" push-connect | 2 | $0 | Already in hand (ice maker kit) |
| 1/4" OD tubing runs | ~2m | $0 | Already in hand |
| 50mm binder clips (bag sealed end mount) | 4 | $1 | Already in hand |
| **Total** | | **~$68-73** | |

### GPIO Impact

- 2 additional solenoid valves (hopper, one per flavor) need 2 GPIO outputs
- Current free output GPIOs: 14, 16 (only 2 available)
- After hopper solenoids, the ESP32 has ZERO free output GPIOs
- Future expansion (capacitive sensing, additional controls) MUST go through I2C expander (MCP23017)
- Alternative: drive hopper solenoids from MCP23017 from the start, preserving GPIO 14 and 16 as reserve

**Hopper solenoid driver:** Individual logic-level N-channel MOSFETs (IRLZ44N), one per hopper solenoid. Gate to GPIO, drain to solenoid, source to GND, flyback diode across solenoid. No L298N board needed -- these are simple on/off loads.

### Open Questions Requiring Physical Testing

1. **Pump reversal fluid tightness:** When the Kamoer pump reverses, does the roller compression seal work identically in both directions? (Peristaltic pumps are symmetric, but test to confirm.)

2. **Check valve cracking pressure impact on dispensing:** Does a 1/4" inline check valve add noticeable back-pressure during normal pump dispensing?

3. **Counter-flow air escape during pump fill:** With the pump pushing at 2-5 PSI, does air escape the bag or does it compress and stall? What fill percentage is achieved per cycle?

4. **Incline mount drainage test:** Mount a filled 1L Platypus bag at 18-20 degrees, run the pump. At what remaining volume does air appear? Target: clean drainage to last 5-10%.

5. **Sealed hopper air inlet:** Does a silicone duckbill valve on the hopper cap reliably allow air IN as concentrate drains?

6. **Hopper solenoid food safety:** Sugar syrup sitting in a closed solenoid valve between uses -- does it crystallize and jam? May need periodic flushing.

7. **FDC1004 sensitivity through silicone tubing:** How much capacitance change when 1/4" silicone tubing transitions from air to sugar syrup?

8. **Bag attachment seal:** Under pump suction (~0.5 psi) and pump push during refill (~2-5 psi), is the Platypus Drink Tube Kit cap friction fit adequate, or does it need a hose clamp reinforcement?

9. **TEE2 routing within the enclosure:** Does TEE2 fit in the available space between the pump outlet and the dispensing point tubing run? If the pump is inside a cartridge, TEE2 would be immediately outside the cartridge dock.

---

## 9. Sources

### Platypus Bag Specifications
- [Platypus Platy 2L Bottle - Cascade Designs](https://cascadedesigns.com/products/platy-2l-bottle)
- [Platypus Platy 2L - REI](https://www.rei.com/product/820769/platypus-platy-water-bottle-70-fl-oz)
- [Platypus Platy 2L - Garage Grown Gear](https://www.garagegrowngear.com/products/platy-2l-bottle-collapsible-bottle-by-platypus)
- [Platypus Drink Tube Kit - Cascade Designs](https://cascadedesigns.com/products/drink-tube-kit)
- [Platypus Replacement Closure Cap - Cascade Designs](https://cascadedesigns.com/products/closure-cap)
- [Platypus Platy 2L - Amazon](https://www.amazon.com/Platypus-Platy-2-Liter-Flexible-Bottle/dp/B0BX4YQ8C5)

### Dip Tube and Sealed Path Analysis
- [dip-tube-analysis.md](dip-tube-analysis.md) — Detailed analysis of the Platypus Drink Tube Kit's dip tube, cap seal, and flow dynamics
- [pump-assisted-filling.md](pump-assisted-filling.md) — Pump reversal topology, sealed hopper, air management

### Bag Mounting
- [incline-bag-mounting.md](incline-bag-mounting.md) — Two-point stretch mount geometry, drainage analysis, mounting hardware

### Spill Containment
- [drip-tray-shelf-analysis.md](drip-tray-shelf-analysis.md) — Analysis of every liquid scenario; drip tray removed, drip shelf redesigned as open electronics shelf

### Capacitive Liquid Detection
- [DFRobot SEN0370 - Small Pipe Diameter Level Sensor (OD <= 10mm)](https://wiki.dfrobot.com/Small_Pipe_Diameter_Level_Sensor_SKU_SEN0370)
- [DFRobot SEN0368 - Non-contact Capacitive Liquid Level Sensor (OD >= 11mm)](https://wiki.dfrobot.com/Non_Contact_Capacitive_Liquid_Level_Sensor_SKU_SEN0368)
- [DFRobot SEN0204 - XKC-Y25-T12V Liquid Level Sensor](https://wiki.dfrobot.com/Non-contact_Liquid_Level_Sensor_XKC-Y25-T12V_SKU__SEN0204)
- [FDC1004 Datasheet - Texas Instruments](https://www.ti.com/lit/ds/symlink/fdc1004.pdf)
- [FDC1004 Application Note - TI](https://www.ti.com/lit/an/snoa927a/snoa927a.pdf)
- [ProtoCentral FDC1004 Breakout Board](https://protocentral.com/product/protocentral-fdc1004-capacitance-sensor-breakout-board/)
- [FDC1004 Liquid Level Sensing Project - Hackster.io](https://www.hackster.io/team-protocentral/non-contact-capacitive-liquid-level-sensing-using-fdc1004-9333c7)
- [FDC1004 Arduino Library - GitHub](https://github.com/Protocentral/ProtoCentral_fdc1004_breakout)
- [XKC-Y25-T12V on Amazon](https://www.amazon.com/LYYNHG-Detector-Manufacturer-Induction-XKC-Y25-V/dp/B0CW3CTCFB)
- [DFRobot SEN0370 - Small Pipe Sensor Product Page](https://www.dfrobot.com/product-2116.html)

### Bag-in-Box and Collapsible Bag Systems
- [Bag-in-Box - Wikipedia](https://en.wikipedia.org/wiki/Bag-in-box)
- [Bag-in-Box Soda System Explained - BN Pack](https://bnpack.com/bag-in-box-soda-system/)
- [Bag-in-Box System Overview - BN Pack](https://bnpack.com/bag-in-box-system/)
- [Bag-in-Box Benefits - BeverageFactory.com](https://www.beveragefactory.com/blog/coffee-and-tea/benefits-of-bag-in-box-beverage-dispensing/)
- [Boxxle Bag-in-Box Wine Dispenser Review](https://the-gadgeteer.com/2013/08/14/boxxle-bag-in-box-wine-dispenser-review/)

### IV Bag and Medical Infusion
- [IV Therapy Management - NCBI Bookshelf](https://www.ncbi.nlm.nih.gov/books/NBK593209/)
- [Gravity Infusion Guide - Coram HC](https://www.coramhc.com/sites/default/files/2024-04/Coram%20Gravity%20Infusion%20Administration%20Guide.pdf)
- [Gravity Feeding Method - Memorial Sloan Kettering](https://www.mskcc.org/cancer-care/patient-education/tube-feeding-using-gravity-method)

### Bottle Threading Standards
- [Water Bottle Cap Thread Sizes and Dimensions](https://www.waterbottle.tech/water-bottle-cap-and-neck-finishes-thread-sizes-dimensions/)
- [Guide to Neck Finishes - The Cary Company](https://www.thecarycompany.com/insights/guides/guide-to-neck-finishes)
- [Bottle Neck Thread Guide - Paramount Global](https://www.paramountglobal.com/knowledge/bottle-neck-thread-finish/)
- [Platypus Threading Discussion - Backpacking Light](https://backpackinglight.com/forums/topic/44665/)
- [Let's Talk About Threads - Minimal Gear](https://minimalgear.com/blogs/blog/lets-talk-about-threads)

### Peristaltic Pumps
- [Peristaltic Pump - Wikipedia](https://en.wikipedia.org/wiki/Peristaltic_pump)
- [How to Set Up and Prime a Peristaltic Pump - Cole-Parmer](https://www.coleparmer.com/blog/how-to-set-up-and-prime-a-peristaltic-pump/)

### Funnel / Food-Safe Materials
- [Impresa Silicone Wide Mouth Funnel - Amazon](https://www.amazon.com/Pack-Squeeze-Bottle-Funnel-Dressing/dp/B07XVQKC3X)
- [Farm to Table Silicone Funnel Set - Amazon](https://www.amazon.com/Farm-Table-Canning-Silicone-3-Piece/dp/B001UAREYE)

### SodaStream Syrup
- [SodaStream Syrup Measuring - sodastreamstuff.blogspot.com](http://sodastreamstuff.blogspot.com/2013/02/measuring-perfect-amount-of-sodastream.html)
- [Syrup FAQ - Soda Centre](https://www.sodacentre.com/pages/syrup-faq)
