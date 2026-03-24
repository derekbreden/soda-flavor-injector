# Hopper System and Bag Management Research

## Table of Contents

1. [Hopper/Funnel Design](#1-hopperfunnel-design)
2. [Hopper-to-Bag Fluid Path](#2-hopper-to-bag-fluid-path)
3. [Air Management](#3-air-management)
4. [Capacitive Liquid/Air Detection](#4-capacitive-liquidair-detection)
5. [Bag Mounting and the Flattening Problem](#5-bag-mounting-and-the-flattening-problem)
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

**Material considerations:**
- **Food-grade silicone** is the best choice: flexible (absorbs impacts), easy to clean, dishwasher safe, transparent/translucent options available, temperature resistant
- **Stainless steel** works but is opaque (user can't see if concentrate is still draining) and heavier
- **PETG (3D printed)** for prototyping -- FDA-compliant PETG filament exists, but surface finish traps residue in layer lines. Acceptable for prototyping, not ideal for production
- Transparent or translucent material is strongly preferred so the user can see the concentrate level in the funnel

**Funnel outlet:**
- The funnel must drain to tubing, not directly into the bag
- The funnel outlet should be a barb fitting (matching the 1/4" OD tubing ecosystem) or a short section of integrated tubing
- A small screen/filter at the funnel outlet catches debris (but adds cleaning burden)
- The funnel outlet connects to the pump's intake path for pump-assisted filling (see Section 2)

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

### 2a. Pump-Assisted Filling (Preferred)

The Kamoer KPHM400 peristaltic pumps are reversible DC motors. Running the pump in reverse pulls fluid from the hopper side and pushes it through the tubing into the bag.

**The key question: where does the hopper connect to the existing plumbing?**

The current dispensing path is:
```
Bag → [dispensing solenoid] → [pump] → dispensing point (faucet)
```

To pump concentrate FROM the hopper INTO the bag, we need the pump to pull from the hopper. The pump can't pull from both the bag (dispensing) and the hopper (refilling) through the same inlet without a valve or tee.

**Option A: Hopper tees into the pump outlet side**

```
DISPENSING MODE (pump forward):
  Bag → [disp. solenoid OPEN] → [pump →] → dispensing point

REFILL MODE (pump reverse):
  Hopper → [hopper valve OPEN] → TEE → [← pump] → [disp. solenoid OPEN] → Bag
                                   |
                            dispensing point (dead end, no flow)
```

In this arrangement:
- A TEE is placed between the pump outlet and the dispensing point
- The hopper line connects to this TEE through a hopper valve (solenoid or manual)
- During refill: hopper valve OPEN, pump runs in REVERSE, dispensing solenoid OPEN
- Concentrate flows: hopper → hopper valve → tee → pump (reversed) → dispensing solenoid → bag
- The dispensing point is a dead-end during refill (no pressure driving flow that direction)

Problem: the dispensing point is open to atmosphere. During reverse pumping, could air be pulled in through the dispensing point tee instead of concentrate from the hopper? The peristaltic pump creates suction on its inlet side. If the tee is between the pump and the dispensing point, the pump would try to pull from both the hopper line AND the dispensing point. Air from the dispensing point could enter the pump.

Fix: add a check valve or solenoid on the dispensing point line, or redesign the tee placement.

**Option B: Hopper tees into the pump inlet side (between dispensing solenoid and pump)**

```
DISPENSING MODE (pump forward):
  Bag → [disp. solenoid OPEN] → TEE → [pump →] → dispensing point
                                  |
                    Hopper → [hopper valve CLOSED]

REFILL MODE (pump reverse):
  Hopper → [hopper valve OPEN] → TEE → [← pump] → dispensing point
                                  |
                    Bag ← [disp. solenoid OPEN] ← (reverse flow through solenoid)
```

Wait -- this doesn't work cleanly. When the pump reverses, it pushes fluid back through the dispensing solenoid into the bag, but it's also pulling from the tee. The tee has two sources: the hopper (through the hopper valve) and the bag (through the dispensing solenoid). We need only the hopper to feed.

Fix: during refill, dispensing solenoid CLOSED, hopper valve OPEN:
```
REFILL MODE (pump reverse):
  Hopper → [hopper valve OPEN] → TEE → [← pump reversed] → dispensing point (waste/drip)
                                  |
                    Bag ← [disp. solenoid CLOSED] (blocked)
```

But now the reversed pump pushes fluid out the dispensing point, not into the bag. That's backwards.

**Option C: Hopper connects to the bag directly, pump pulls from bag side**

```
                    ┌──────────────────────────────┐
                    │                              │
  Hopper → [hopper valve] → TEE → Bag → [disp. solenoid] → [pump] → dispensing point
                              |
                    clean water → [clean solenoid]
```

The hopper tee is on the BAG side of the plumbing, between the bag and the dispensing solenoid. This is the same location as the existing clean cycle tee.

During refill: gravity and/or pump assistance move concentrate from the hopper through the tee into the bag. The pump doesn't need to reverse -- concentrate enters the bag through the same port that the bag drains from.

But wait -- the bag has only ONE opening (the Platypus screw-cap port). If the bag is connected to the dispensing plumbing through this port, the hopper must also connect through this same port. This means the hopper, the clean water, and the dispensing path all share the same tee point near the bag.

```
                              ┌─── Bag (single opening)
                              │
  Hopper → [hopper valve] → TEE ← clean water → [clean solenoid]
                              │
                    [disp. solenoid] → [pump] → dispensing point
```

**Refill mode (gravity-fed, no pump):**
- Hopper valve OPEN, dispensing solenoid CLOSED, clean solenoid CLOSED, pump OFF
- Concentrate flows by gravity from hopper through the tee into the bag
- Works only if hopper is above the bag

**Refill mode (pump-assisted):**
- Hopper valve OPEN, dispensing solenoid OPEN, clean solenoid CLOSED, pump REVERSED
- Pump creates suction on the dispensing solenoid side, pulling concentrate from the hopper through the tee, past the bag port, through the dispensing solenoid, into the pump
- But this pulls concentrate THROUGH the bag and into the pump/dispensing line, not INTO the bag
- The bag is not a sealed dead-end -- it's an open pouch connected at one point. Fluid entering the bag opening can flow through the bag or past it depending on path resistance.

This is the fundamental problem: **the bag has one opening, and that opening serves all fluid paths**. You can't simultaneously pump fluid through the tee (past the bag) and expect it to fill the bag. The fluid takes the path of least resistance, which is through the tubing, not into the floppy bag.

**Option D: Gravity fill through the shared tee (recommended)**

The simplest approach that actually works:

```
                              ┌─── Bag (single opening, connector at bottom of hanging bag)
                              │
  Hopper → [hopper valve] → TEE ← clean water → [clean solenoid]
                              │
                    [disp. solenoid] → [pump] → dispensing point
```

**Refill mode:**
- Hopper valve OPEN, dispensing solenoid CLOSED, clean solenoid CLOSED, pump OFF
- With dispensing solenoid CLOSED, the only path from the tee is into the bag
- Concentrate flows from hopper, through hopper valve, through tee, into bag
- Flow is driven by gravity (hopper must be above the bag) and/or head pressure from the concentrate column in the hopper/funnel

**This works because:**
1. The dispensing solenoid blocks the path to the pump, so all flow goes into the bag
2. Gravity provides the driving force (hopper is on top of enclosure, bag hangs below)
3. No pump reversal needed
4. Air displaced from the bag exits back through the bag opening, up through the tee, and out the hopper (which is open to atmosphere during refill)

**What about pump-assisted filling?**

If gravity alone is too slow (narrow Platypus opening restricts flow), the pump could assist:
- Hopper valve OPEN, dispensing solenoid OPEN, clean solenoid CLOSED
- Pump runs FORWARD (normal direction: bag → pump → dispensing point)
- Pump creates suction on the bag side of the dispensing solenoid
- This suction propagates through the tee, pulling concentrate from the hopper into the bag AND through the dispensing solenoid into the pump
- The pump pushes a mix of old bag contents and new concentrate out the dispensing point

This is essentially a "flush and fill" -- not a pure bag fill. The pump accelerates flow through the tee but some concentrate goes into the bag and some goes through the pump. Not ideal because you waste some concentrate out the dispensing point.

**True pump-assisted fill requires a second connection to the bag**, which the Platypus design doesn't support (single opening). So gravity fill through the shared tee with the dispensing solenoid closed is the practical answer.

### 2b. Gravity-Fed Filling (Primary Approach)

Given the analysis above, gravity-fed filling is the recommended primary approach.

**Height differential calculation:**

- Hopper is on top of the enclosure
- Bag hangs vertically inside the enclosure, connector at bottom
- Enclosure height: ~450mm (18", typical PC tower)
- Hopper funnel bottom: ~450mm from cabinet floor
- Bag connector (bottom of hanging bag): ~100mm from cabinet floor (bag hangs from near the top)
- Height differential: ~350mm (14")
- Hydrostatic pressure at bag: 350mm x 9.81 m/s^2 x 1050 kg/m^3 (sugar syrup density) = ~3.6 kPa = ~0.5 psi

This is low pressure, but adequate for flow through tubing. Flow rate depends on the tubing restriction and the Platypus opening (see Section 7).

**Flow rate estimate:**

Using Poiseuille's law for laminar flow through the narrowest restriction (assumed to be the 1/4" OD tubing = ~4.5mm ID):
- Pressure: ~3600 Pa
- Tube length: ~500mm (total tubing run from hopper to bag)
- Tube ID: 4.5mm (radius 2.25mm)
- Viscosity of sugar syrup: ~5 mPa.s (5x water, typical for concentrated syrup)

Q = (pi * r^4 * dP) / (8 * mu * L)
Q = (3.14 * (0.00225)^4 * 3600) / (8 * 0.005 * 0.5)
Q = (3.14 * 2.56e-11 * 3600) / (0.02)
Q = 2.89e-7 / 0.02
Q = 1.45e-5 m^3/s = 14.5 ml/s = ~870 ml/min

This is a generous estimate (assumes no fittings, no bends, ideal conditions). Real flow rate will be lower due to fitting losses, bends, and the Platypus opening restriction. A realistic estimate is **200-500 ml/min**, meaning a 2L refill takes **4-10 minutes by gravity alone**.

This is acceptable. The user pours concentrate into the funnel and walks away. The system drains by gravity. A capacitive sensor on the hopper line detects when the funnel is empty (see Section 4).

### 2c. Hybrid Approach

Not needed given the analysis. Gravity fill with solenoid isolation is simple and effective. The pump's reversibility is a future option if gravity proves too slow, but adds plumbing complexity (need a path that doesn't waste concentrate out the dispensing point).

### 2d. Complete Plumbing Diagram

```
COMPLETE SYSTEM PLUMBING (per flavor line):

                                    ┌─── Platypus Bag
                                    │     (hanging vertically,
                                    │      connector at bottom)
                                    │
  Funnel/Hopper ──→ [HOPPER VALVE] ─┤
                                    │
  Tap Water ──→ [NEEDLE VALVE] ──→ [CLEAN SOLENOID] ─┘
                                    │
                              ┌─────┘
                              │
                       [DISPENSING SOLENOID]
                              │
                           [PUMP]
                              │
                        Dispensing Point
                         (faucet tap)
```

Note: the hopper valve, clean solenoid, and bag all connect at a single TEE junction. The "TEE" in the diagram above is represented by the branching point.

**Valve State Table:**

| Mode | Hopper Valve | Clean Solenoid | Dispensing Solenoid | Pump | Flow Path |
|------|-------------|----------------|--------------------|----- |-----------|
| **Idle** | CLOSED | CLOSED | CLOSED | OFF | No flow |
| **Dispensing** | CLOSED | CLOSED | OPEN | FORWARD | Bag → disp. sol. → pump → tap |
| **Hopper Refill** | OPEN | CLOSED | CLOSED | OFF | Hopper → hopper valve → tee → bag (gravity) |
| **Clean Fill** | CLOSED | OPEN | CLOSED | OFF | Tap water → clean sol. → tee → bag |
| **Clean Flush** | CLOSED | CLOSED | OPEN | FORWARD | Bag → disp. sol. → pump → tap |

**New hardware needed per flavor line:**
- 1x hopper solenoid valve (Beduan 12V 1/4" NC, same as existing) -- $8.99
- 1x TEE fitting (from ice maker kit, already in hand)
- 1x funnel with tubing adapter
- Tubing run from funnel to hopper solenoid (~300mm)

**GPIO impact:**
- 2 additional solenoid valves (one per flavor) need 2 GPIO outputs
- Current free output GPIOs: 14, 16 (only 2 available!)
- These are the last 2 free output-capable GPIOs on the ESP32
- After hopper solenoids, the ESP32 has ZERO free output GPIOs
- Future expansion (capacitive sensing, additional controls) MUST go through I2C expander (MCP23017)
- Alternative: drive hopper solenoids from MCP23017 from the start, preserving GPIO 14 and 16 as reserve

**Hopper solenoid GPIO assignment:**
- GPIO 14: Hopper solenoid flavor 1
- GPIO 16: Hopper solenoid flavor 2
- Both driven via L298N #3 (which already has 2 channels for clean solenoids) -- but L298N #3 only has 2 channels and they're both used for clean solenoids
- Need L298N #4, OR drive hopper solenoids with MOSFETs (simpler for on/off), OR use MCP23017 + MOSFETs

**Simplest driver approach:** Individual logic-level N-channel MOSFETs (IRLZ44N or similar), one per hopper solenoid. Gate to GPIO, drain to solenoid, source to GND, flyback diode across solenoid. No L298N board needed -- these are simple on/off loads.

---

## 3. Air Management

### 3a. During Hopper Refilling

**The scenario:** User pours concentrate into the funnel. Gravity drains it through tubing into the bag. When the funnel empties, air enters the fill line.

**Where does the air go?**

With the dispensing solenoid CLOSED and the hopper valve OPEN, the only path from the tee is into the bag. As the funnel empties:
1. Concentrate finishes draining from the funnel
2. Air enters the hopper tubing
3. Air travels down to the tee
4. Air could enter the bag through the connector

**Why this is actually OK for gravity-fed filling:**

In gravity-fed mode with no pump, air entry is self-limiting. Once the funnel is empty, there's no pressure driving air into the bag. The air column in the hopper tubing just sits there. A small bubble might enter the bag opening, but there's no force pushing it deep into the bag.

**The real air concern is in the tubing between the tee and the dispensing solenoid.** This tubing segment is always full of concentrate during normal operation. After a hopper refill:
- The hopper valve closes
- The dispensing solenoid opens for the next dispense cycle
- The pump pulls concentrate from the bag through the tee

As long as the tee-to-bag path is filled with liquid, the pump draws from the bag normally. The hopper tubing (between hopper valve and tee) contains some air after the funnel drains, but the hopper valve is closed, so this air is isolated.

**Dead volume in the hopper tubing:**

```
Funnel → ~300mm of 1/4" OD tubing → [hopper valve] → ~50mm → TEE
```

- 1/4" OD tubing with ~4.5mm ID: cross-section area = 15.9 mm^2
- 300mm run: dead volume = 15.9 * 300 = 4,770 mm^3 = ~4.8 ml
- This 4.8 ml of concentrate remains in the hopper tubing after the funnel empties
- It's trapped between the hopper valve and the empty funnel
- Not a significant waste, but could grow bacteria if left for weeks
- Clean cycle should flush this line too (open hopper valve during clean flush)

### 3b. During Normal Operation (Bag Emptying)

As the bag empties during dispensing, it collapses. The key concern:

**If the bag collapses unevenly, a fold or pinch can trap liquid above the fold while air sits at the connector.** The pump then pulls air.

This is addressed in detail in Section 5 (bag flattening). In summary:
- Bag must hang vertically with connector at the bottom
- Gravity keeps liquid pooled at the connector
- Mechanical assistance (elastic frame, roller, or guide channel) ensures top-down collapse
- The dispensing solenoid is between the bag and pump -- when closed, it seals the line. No air enters from the bag side while idle.

**Trapped fluid between bag connector and dispensing solenoid:**

```
Bag connector → ~50mm tubing → TEE → ~100mm tubing → [dispensing solenoid]
```

This ~150mm segment (~2.4 ml) is always filled with concentrate during operation. When the dispensing solenoid closes between dispense cycles, this segment is sealed. No air can enter from either side (solenoid sealed on one side, bag full of liquid on the other).

When the bag runs very low, air can enter this segment. The capacitive sensor at the tap (see future-sensing-and-gpio.md) would detect this.

### 3c. During Clean Cycle

**Fill phase (water enters bag):**
- Clean solenoid OPEN, dispensing solenoid CLOSED, pump OFF
- Tap water flows through the tee into the bag
- The bag is initially collapsed (empty from previous use or cleaning)
- As water enters, it inflates the bag
- **Air escape:** The bag was collapsed, meaning it contained mostly air. As water enters through the bottom connector, it pushes air upward. But the air has nowhere to go -- the bag is sealed except for the bottom connector, which is where the water is entering.

This is a real problem. In a sealed collapsible bag with one opening at the bottom:
- Water enters from the bottom
- Air is trapped above the water
- The bag inflates but the air can't escape
- Eventually the water pressure and air pressure equalize and filling stops, with the bag partially full of water and partially full of compressed air

**Solutions:**

1. **Accept partial fill:** The bag fills until back-pressure from trapped air equals the water supply pressure. With typical house water pressure (40-60 psi) through a needle valve (reducing to ~1-5 psi at the bag), the air compresses significantly and the bag fills mostly full. The remaining air pocket is flushed out during the flush phase (pump pulls water + air out).

2. **Bag orientation with connector at top:** If the connector were at the top, water would fill from the top and air would escape past the entering water stream. But connector-at-top is terrible for dispensing (air at the outlet). Not viable.

3. **Two-port bag:** A bag with two openings (fill at bottom, vent at top) solves this perfectly. The Platypus bag has only one opening. A custom bag or modified bag could work -- drill/heat-seal a small vent hole at the top with a check valve. But this modifies a commercial product and adds complexity.

4. **Fill-drain-repeat:** Fill the bag partially (air trapped), then flush (pump pulls water + air out through the dispensing line), then fill again. Each cycle removes more air. This is essentially what the existing multi-cycle clean already does -- the CLEAN_CYCLES constant (default 3) serves this purpose.

**Verdict:** The existing multi-cycle clean approach handles this adequately. Each fill+flush cycle removes more residual air. Three cycles is a reasonable starting point. The trapped air during filling is annoying but not harmful -- it just means each fill cycle doesn't fill the bag to 100% capacity.

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
  Funnel → [SENSOR A] → [hopper valve] → TEE ← [clean solenoid] ← tap water
                                         │
                                   [disp. solenoid]
                                         │
                                      [pump]
                                         │
                               [SENSOR B] → dispensing point
```

- **Sensor A (hopper line):** Detects when the funnel has drained (liquid-to-air transition). Placed between the funnel outlet and the hopper valve. When air is detected, the system knows the refill is complete. The hopper valve can close.
- **Sensor B (dispensing line):** Detects priming status (air-to-liquid = primed) and clean cycle completion (liquid-to-air = line purged). Placed between the pump and the dispensing point.

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

## 5. Bag Mounting and the Flattening Problem

### 5a. The Problem in Detail

A 2L Platypus bag is soft-sided with no internal structure. As the peristaltic pump draws liquid out, the bag must collapse. The pump creates suction at the bag outlet, and atmospheric pressure on the outside of the bag provides the collapsing force.

The failure mode:
1. Bag hangs vertically, connector at bottom, liquid inside
2. Pump draws liquid out through the bottom connector
3. Bag starts collapsing -- the top portion (now empty) folds inward
4. If a fold or crease forms BELOW the remaining liquid level, liquid gets trapped above the fold
5. The pump now pulls air from below the fold instead of liquid from above
6. Pump sputters (alternating air and liquid)
7. Inconsistent flavor injection into the soda water

The goal: **the bag must collapse continuously from the top downward**, so remaining liquid always pools at the bottom near the connector.

### 5b. Platypus 2L Bag Physical Properties

**Material:** Nylon/polyethylene laminate film. Spout is polyethylene. Cap is polypropylene. BPA-free, BPS-free, phthalate-free.

**Dimensions (when full):**
- Width: 190mm (7.5")
- Height: 350mm (13.8")
- Depth when full: approximately 60-80mm (2.5-3") -- the bag puffs out when filled
- Weight empty: 37g (1.3 oz)
- Weight full (2L water): ~2037g (~4.5 lbs)
- Weight full (2L sugar syrup, ~1.05 g/ml): ~2137g (~4.7 lbs)

**Shape:** Rectangular pouch with heat-sealed edges. No gussets (flat when empty). The opening/connector is at the top-center of one of the narrow ends. When hanging by the bottom (connector down), the bag hangs like a rectangular flag.

**Connector location:** The threaded screw-cap opening is at the TOP of the bag as designed (for drinking). In our application, with connector at the bottom for gravity-assisted drainage, the bag hangs UPSIDE DOWN relative to its intended orientation.

**Thread:** Standard 28mm water bottle thread, same as most plastic soda bottles. Compatible with 28mm caps, adapters, and the Platypus Drink Tube Kit.

**Natural collapse behavior:** When emptied slowly (gravity drain or gentle suction), the bag tends to fold inward from the sides, creating a somewhat random collapse pattern. The thin nylon/PE film has no inherent stiffness, so it folds wherever the material stress concentrates. Without mechanical guidance, the collapse is unpredictable.

### 5c. Bag Orientation Analysis

**Hanging vertically, connector at bottom (RECOMMENDED):**
```
    ┌── clip/hanger
    │
    ╔════════════╗  ← bag top (sealed end)
    ║            ║
    ║   liquid   ║  ← liquid pools at bottom due to gravity
    ║            ║
    ╚════╤═══════╝  ← bag bottom (connector end)
         │
       tubing → to plumbing
```
- Gravity pulls liquid to the connector (bottom)
- As liquid is removed, the top of the bag collapses first (least hydrostatic pressure at top)
- Natural top-down collapse behavior
- Most reliable orientation for preventing air at the outlet
- This is how IV bags are hung (connector at bottom, drip by gravity)
- This is how bag-in-box systems work (tap at bottom, bag collapses from top)

**Hanging vertically, connector at top:**
- Liquid pools at the bottom, AWAY from the connector
- Pump must suck liquid up from the bottom of the bag through the entire bag height
- Air collects at the top near the connector first
- Terrible for preventing air ingestion. Reject.

**Laying flat:**
- Liquid spreads across the entire bag surface area
- Air pockets can form anywhere as the bag empties
- No gravitational bias to keep liquid at the outlet
- Only viable with positive pressure squeezing the bag uniformly (like a blood pressure cuff). Unnecessarily complex. Reject.

**Verdict:** Hang vertically, connector at bottom. This is the standard approach used by IV bags, bag-in-box wine dispensers, and hydration pack bladders.

### 5d. Mechanical Solutions for Controlled Flattening

#### Solution 1: Gravity Alone (No Mechanism)

```
    ┌── hook/clip
    │
    ╔════════════╗
    ║  (collapses║  ← top collapses inward as liquid drains
    ║   inward)  ║
    ║────────────║  ← liquid level drops
    ║   liquid   ║
    ╚════╤═══════╝
         │
       tubing
```

- The bag hangs freely from a hook or clip at the top
- Gravity pulls liquid down; atmospheric pressure collapses the empty top portion
- No mechanical parts

**Pros:** Simplest possible solution. Zero moving parts. Easy bag installation (hang on hook).
**Cons:** No guarantee of orderly collapse. The bag film could fold sideways, crease in the middle, or stick to itself in ways that trap liquid. Works fine for the first 80% of the bag; the last 20% is where problems occur as the bag becomes very floppy.
**Verdict:** Try this first. If it works for the first 1.5L (75% of bag) and only sputters in the last 0.5L, it might be "good enough" with a low-level warning to refill when the bag is at ~25%.

#### Solution 2: Elastic Frame / Bag Squeezer

```
    SIDE VIEW (cross-section):

    ┌── hook
    │
    │  spring    │
    │  ←────→    │
    │ ┌──────┐   │
    │ │ plate │   │  ← two plates with springs/elastic
    │ │      │   │     pressing inward from both sides
    │ │ BAG  │   │
    │ │      │   │
    │ │ plate │   │
    │ └──────┘   │
    │  ←────→    │
    │  spring    │
```

Two flat plates (3D printed or sheet material) on either side of the bag, connected by elastic bands or springs. The plates press inward gently, encouraging the bag to flatten from the top (where there's less liquid weight resisting the squeeze).

**Pros:**
- Forces controlled collapse from top down (springs provide constant inward pressure; liquid weight at bottom resists collapse there)
- Simple to 3D print (two flat plates + elastic bands)
- Adjustable pressure by changing elastic band tension

**Cons:**
- User must thread the bag between the plates during installation
- If the elastic is too tight, it could squeeze the bag and force liquid out the connector
- If too loose, it doesn't help
- Need to accommodate bags from full (60-80mm thick) to empty (flat)

**Complexity:** Low-medium. Two 3D printed plates, 2-4 elastic bands. Easy to prototype.
**Bag installation:** Slide bag between plates. Plates spread apart to accommodate full bag, squeeze together as bag empties.
**Space:** ~200mm wide x 360mm tall x 100mm deep per bag.

#### Solution 3: Roller / Wiper Bar

```
    FRONT VIEW:

    ┌── hook
    │
    ╔════════════╗  ← bag
    ║            ║
    ║  ══════    ║  ← weighted roller bar sits on top of liquid level
    ║  ~~~~~~~~  ║     rolls downward as bag empties
    ║   liquid   ║
    ╚════╤═══════╝
         │
       tubing
```

A weighted bar or roller sits on top of the bag (on the liquid surface). As liquid is pumped out, the bar descends, pressing the empty bag flat above it. Similar to a toothpaste tube squeezer.

**Pros:**
- Mechanically forces top-down collapse
- The weight of the bar provides consistent downward pressure
- Very effective at squeezing out the last bit of liquid
- Prior art: toothpaste tube squeezers, ratchet-style tube winders

**Cons:**
- The roller/bar must be guided so it doesn't tilt or jam
- Need guide rails on both sides of the bag
- Must be reset to the top when installing a new (full) bag
- More mechanical complexity than elastic frame
- Risk: if the bar gets stuck on a fold in the bag, it stops descending and the bag crumples below it

**Complexity:** Medium-high. Guide rails, weighted bar, possibly ratchet mechanism.
**Bag installation:** Reset roller to top, hang bag, ensure roller sits evenly on bag surface.
**Space:** ~200mm wide x 360mm tall x 80mm deep per bag (guide rails add width).

#### Solution 4: Channel / Cradle

```
    FRONT VIEW:

    ┌── hook
    │
    ┌────────────┐
    │╔══════════╗│  ← bag sits inside a shaped channel
    ││          ││     channel tapers: wide at bottom, narrow at top
    ││  liquid  ││
    ││          ││
    │╚════╤═════╝│
    └─────│──────┘
          │
        tubing
```

The bag sits inside a rigid channel or cradle that constrains its shape. The channel is wider at the bottom (where the full bag bulges) and narrower at the top (encouraging collapse). As the bag empties, it naturally pulls away from the wider bottom sections and collapses into the narrower top.

**Pros:**
- Passive -- no springs or moving parts
- The channel shape predetermines the collapse pattern
- Rigid structure provides mounting point for the bag

**Cons:**
- The channel must be precisely shaped for the Platypus bag dimensions
- A full bag might not fit if the channel is too narrow
- An empty bag might not collapse properly if the channel is too wide
- Less adaptable to different bag sizes

**Complexity:** Medium. 3D printed channel/cradle, sized for Platypus 2L.
**Bag installation:** Drop bag into channel, connect tubing at bottom.
**Space:** ~200mm wide x 360mm tall x 100mm deep per bag.

#### Solution 5: Top-Suspended with Bottom Weight

```
    ┌── hook at bag BOTTOM (which is now at top, connector pointing up)
    │
    ╔════╤═══════╗  ← bag suspended from its connector end (top = connector)
    ║    │       ║
    ║   liquid   ║  ← liquid pools in the hanging bottom
    ║            ║
    ╚════════════╝  ← bottom of bag (sealed end) hangs down
         │
       weight (optional)
```

Wait -- this puts the connector at the top, which we rejected. Unless we use a siphon or the pump creates enough suction to pull liquid upward from the bag bottom through the connector at the top. But this defeats the gravity advantage and risks air at the connector. Not recommended.

### 5e. Prior Art

**IV bags (medical):**
- Hung vertically from an IV pole, connector/drip port at the bottom
- Gravity feeds liquid down through the drip chamber and IV line
- The bag collapses naturally as it empties -- no mechanical assistance
- IV bags are made of PVC or non-PVC flexible film, similar flexibility to Platypus bags
- IV bags have a wider, flatter form factor than Platypus bottles
- Air is managed by the drip chamber (air separates from liquid before entering the line)
- Key lesson: **gravity + vertical hanging + connector at bottom works for medical applications**, but IV infusion rates are slow (50-200 ml/hr). Peristaltic pump rates (400 ml/min) are much faster and create more suction, which may cause more aggressive bag collapse.

**Bag-in-box (wine, soda syrup):**
- Inner bag sits inside a rigid box
- Tap/spigot at the bottom of the box
- As liquid is dispensed, the bag collapses inside the box
- The box prevents the bag from expanding outward, so all collapse is inward
- Air cannot enter because the system is sealed -- the bag collapses under atmospheric pressure
- No mechanical assistance -- just gravity and atmospheric pressure
- Key lesson: **the rigid box constrains the bag's collapse pattern**. Without the box, the bag could balloon outward instead of collapsing inward. This suggests that the "channel/cradle" approach (Solution 4) or some form of rigid containment helps.

**Hydration pack bladders (CamelBak, Platypus reservoirs):**
- Hang inside a backpack, connector at bottom via drink tube
- Collapse as water is drunk through the bite valve
- No mechanical collapse assistance
- Random collapse patterns are common -- users often feel air pockets when the bladder is nearly empty
- Key lesson: **unassisted collapse works for most of the bladder capacity, but fails for the last 10-20%**. Users accept this because they can squeeze the bladder manually.

**Toothpaste tube squeezers:**
- Ratchet or roller mechanisms force controlled collapse from the bottom up
- Very effective at extracting the last bit of product
- Printables.com has many 3D-printable designs (ratchet, gear, roller)
- Key lesson: **roller/ratchet mechanisms work extremely well for controlled collapse**, but toothpaste tubes are rigid enough to hold their shape. A soft bag would bunch and fold under the roller.

### 5f. Recommended Approach: Start Simple, Add Complexity Only If Needed

**Phase 1: Gravity only (try first)**
- Hang bag vertically, connector at bottom, from a simple hook or clip at the top
- Test with water: fill bag, connect to pump, run pump until bag empties
- Observe collapse pattern: does it collapse from the top? Does the pump sputter?
- If it works for 90%+ of the bag capacity, this is sufficient

**Phase 2: Elastic frame (if gravity alone fails)**
- Two 3D-printed plates on either side of the bag
- Connected by elastic bands (hair ties, rubber bands, or silicone bands)
- Plates are slightly narrower than the bag width, so they press the bag flat from the sides
- The elastic force encourages top-down collapse
- Test and iterate on elastic tension

**Phase 3: Rigid cradle (if elastic frame is insufficient)**
- 3D-print a channel/cradle that contains the bag
- Shape it to be slightly tapered (wider at bottom, narrower at top)
- This constrains the collapse pattern more aggressively than an elastic frame
- Similar to the rigid box in bag-in-box systems

**Key principle:** The simpler the solution, the easier it is for the user to install a new bag. A bare hook is trivial. An elastic frame requires threading the bag between plates. A rigid cradle requires fitting the bag into the channel. Don't add complexity unless testing proves it's needed.

---

## 6. Overfill and Spill Containment

### Overfill Scenarios

**User pours too much concentrate into the hopper:**
- The funnel has a fixed volume (~200-400 ml depending on size)
- If the user pours more than the funnel can hold, it overflows out the top
- Mitigation: clear/translucent funnel so user can see the level. Funnel lip to catch minor overflows. User instruction: "pour slowly, wait for funnel to drain."

**Bag is already mostly full and user overfills:**
- Hopper valve opens, gravity drives concentrate into the bag
- The bag has a maximum capacity of 2L
- If the bag already has 1.5L and the user pours 1L into the hopper, the bag reaches capacity
- Excess concentrate backs up in the tubing and into the funnel
- No overflow as long as the funnel can hold the excess
- The funnel level stops dropping (user sees this and stops pouring)
- With the capacitive sensor on the hopper line: if concentrate stops flowing (sensor stays "liquid" for a long time), firmware could signal "bag full"

**Pump fails to stop (sensor failure):**
- Only relevant for pump-assisted filling (not the recommended gravity-fill approach)
- In gravity-fill mode, there's no pump to fail -- gravity is self-regulating
- The worst case is the hopper valve stays open indefinitely, but this just means the empty funnel sits connected to the bag. No overflow.

### Spill Containment

**Inside the enclosure:**
- Flavor concentrate is sticky sugar syrup. Spills attract insects and can corrode electronics if they reach PCBs.
- A drip tray or basin under the bags is essential
- 3D-printed or purchased shallow tray, ~250mm x 150mm x 25mm deep
- Catches drips from bag connections, leaks from tubing joints, or condensation
- The tray should be removable for cleaning

**Around the hopper:**
- The funnel area on top of the enclosure should have a raised lip or drip ring
- If concentrate spills while pouring, it pools in the lip instead of running down the enclosure side
- A small moat around the funnel base (5mm deep, 10mm wide) catches most drips

**Worst-case spill:**
- A full bag ruptures inside the enclosure: 2L of sticky syrup
- The drip tray catches some, but 2L will overflow a 25mm-deep tray
- Secondary containment: line the bottom of the cabinet with a waterproof mat or tray
- This is a rare catastrophic failure, not a normal operating concern

### Spill Detection

- A simple water/leak sensor on the drip tray could alert the user
- Low-priority feature -- the system doesn't currently have leak detection
- Could be a future MCP23017 input or FDC1004 channel

---

## 7. The Platypus Bottle Opening as a Design Constraint

### Thread Specifications

The Platypus bottle uses a **28mm thread**, the same standard used by most plastic soda bottles worldwide (Pepsi products, Smart Water, Evernew, etc.). This is a common "28mm" or "28-410" thread finish in the packaging industry.

- Outer diameter of the thread: ~28mm
- Opening inner diameter: approximately 21-22mm (the actual hole through which liquid flows)
- Thread pitch: described as 5/32" by some sources

### Available Caps and Adapters

- **Platypus Closure Cap**: standard screw-on cap (replacement available from Cascade Designs)
- **Platypus Drink Tube Kit**: threaded cap with integrated barb fitting for 1/4" drink tube. This is the current connection method in the soda injector system.
- **28mm to barb adapters**: available from various sources since 28mm is a standard size. However, the Platypus thread reportedly has a 5/32" pitch, which may differ slightly from other "28mm" bottles.
- **Sawyer filter adapters**: Sawyer uses a different 4/32" pitch, so Sawyer adapters are NOT compatible with Platypus bottles.

### Flow Rate Through the Opening

The Platypus opening inner diameter of ~21-22mm is not the bottleneck for filling. The bottleneck is the 1/4" OD tubing (4.5mm ID) that connects the bag to the plumbing. Even if the opening were larger, the tubing restricts flow.

However, for gravity-fill INTO the bag (hopper → tee → bag), the concentrate must enter through this same opening. The opening is adequate -- 21mm diameter allows free flow. The tubing restriction is downstream of the bag opening.

**Actual fill rate into the bag is limited by:**
1. Tubing diameter between hopper and tee (~4.5mm ID)
2. Height differential (gravity pressure)
3. Viscosity of the concentrate
4. Air escape rate from the bag (air must exit through the same opening as concentrate enters)

The air escape issue (#4) is the most significant limiter during refill. As concentrate enters the bag from below (through the tee and connector), air must bubble out past the incoming concentrate stream. This counter-flow (liquid down, air up through the same tube) limits the effective fill rate. In a 4.5mm ID tube, liquid and air cannot easily pass each other.

**Mitigation:** The hopper-to-tee tubing run is relatively short (~300-400mm). Once the concentrate column is established in the tube, it pushes air out of the bag in slugs (large air bubbles rising through the tube periodically). This self-regulating process is slow but works. Estimated gravity-fill time for 2L: **8-15 minutes** (accounting for air counter-flow).

### Could a Different Bag Be Used?

If the Platypus opening proves too restrictive:

**Wider-mouth collapsible bags:**
- CNOC Vecto (28mm thread, same standard) -- similar constraints
- Custom bag with two ports (fill port + vent port) -- solves the air counter-flow problem but requires custom fabrication
- Medical-style IV bags -- have spike ports designed for controlled flow, but smaller capacity (typically 500ml-1L)

**Custom reservoir instead of a bag:**
- A rigid collapsible container (accordion/bellows style) with separate fill and drain ports
- More complex to source/fabricate, but eliminates the single-opening constraint
- Could be 3D printed (food-safe PETG bellows)

**Verdict:** Stick with the Platypus 2L bags for now. The 28mm opening is adequate. The fill time of 8-15 minutes is acceptable (user pours and walks away). If the single-opening air counter-flow proves problematic in testing, a custom two-port bag is the next step.

---

## 8. Recommendation

### Recommended Architecture

**Gravity-fed filling through the shared tee junction.** No pump reversal, no additional fluid paths. The hopper solenoid valve isolates the hopper line when not in use.

### Recommended Bag Orientation and Mounting

**Hang vertically, connector at bottom.** Start with a simple hook/clip at the top of the enclosure interior. The bag hangs freely and collapses under gravity and atmospheric pressure as the pump draws liquid out.

### Recommended Flattening Mechanism

**Phase 1: Gravity only.** Test with water first. If the pump sputters in the last 20% of the bag, move to Phase 2.

**Phase 2: Elastic frame.** Two 3D-printed flat plates on either side of the bag, connected by elastic bands. The elastic tension gently presses the bag flat from the sides, encouraging top-down collapse. This is the expected final solution -- bag-in-box systems demonstrate that some form of containment improves collapse behavior.

### Recommended Hopper Location

**Top of enclosure, one funnel per flavor.** Two funnels, each ~75-100mm diameter, with snap-on silicone lids. Positioned so the user can reach them when the cabinet door is open.

### Recommended Plumbing

```
COMPLETE SYSTEM (2 flavor lines):

         [Funnel 1]         [Funnel 2]
             │                    │
         [SEN-A1]            [SEN-A2]       ← capacitive sensor (hopper empty detect)
             │                    │
        [HOPPER SOL 1]      [HOPPER SOL 2]  ← NEW solenoid valves (Beduan 12V NC)
             │                    │
     ┌───── TEE 1 ─────┐ ┌───── TEE 2 ─────┐
     │       │          │ │       │          │
   [BAG 1]  │          │ │    [BAG 2]       │
     │    [CLEAN SOL 1] │ │    [CLEAN SOL 2] │
     │       │          │ │       │          │
     │   needle valve───┘ │   needle valve───┘
     │       │            │       │
     │    tap water ──────┘    tap water
     │                        │
  [DISP SOL 1]           [DISP SOL 2]
     │                        │
  [PUMP 1]                [PUMP 2]
     │                        │
  [SEN-B1]                [SEN-B2]          ← capacitive sensor (primed/purged detect)
     │                        │
  dispensing point        dispensing point
```

### Valve States for All Operating Modes

| Mode | Hopper Sol | Clean Sol | Disp Sol | Pump | Notes |
|------|-----------|-----------|----------|------|-------|
| Idle | CLOSED | CLOSED | CLOSED | OFF | |
| Dispensing | CLOSED | CLOSED | OPEN | FWD | Normal operation |
| Hopper Refill | OPEN | CLOSED | CLOSED | OFF | Gravity-fed, user pours into funnel |
| Clean Fill | CLOSED | OPEN | CLOSED | OFF | Tap water fills bag |
| Clean Flush | CLOSED | CLOSED | OPEN | FWD | Pump empties bag to dispensing point |
| Clean Air Purge | CLOSED | CLOSED | OPEN | FWD | Pump runs on empty bag, blows air through line |
| Hopper Line Flush | OPEN | CLOSED | OPEN | FWD | During clean cycle, flush hopper tubing too |

### Hardware Shopping List (New Items)

| Item | Qty | Est. Price | Source |
|------|-----|-----------|--------|
| Beduan 12V 1/4" NC solenoid valve (hopper) | 2 | $18 | Amazon (B07NWCQJK9) |
| FDC1004 breakout board (ProtoCentral) | 1 | $18 | ProtoCentral / DigiKey |
| IRLZ44N N-channel MOSFET (hopper sol. driver) | 2 | $2 | Amazon/DigiKey |
| Food-grade silicone funnel, ~100mm opening | 2 | $10 | Amazon |
| Silicone plug/cap for funnels | 2 | $5 | Amazon |
| Copper tape (for FDC1004 electrodes) | 1 roll | $8 | Amazon |
| TEE fittings 1/4" push-connect | 2 | $0 | Already in hand (ice maker kit) |
| 1/4" OD tubing runs | ~2m | $0 | Already in hand |
| **Total** | | **~$61** | |

### Open Questions Requiring Physical Testing

1. **Gravity fill rate:** How fast does concentrate actually drain from the hopper into the bag through 1/4" tubing with 350mm height differential? Does the air counter-flow through the single bag opening significantly slow filling?

2. **Bag collapse behavior:** Does the Platypus 2L bag collapse in an orderly top-down fashion when hung vertically with pump suction at the bottom? At what fill level does sputtering begin?

3. **Elastic frame tension:** If the elastic frame (Phase 2) is needed, what tension provides good collapse without squeezing concentrate out the connector?

4. **Funnel drain rate:** With a 100mm funnel draining through a 4.5mm ID tube, how long does it take for 500ml of syrup to drain? Is the funnel volume sufficient as a buffer?

5. **FDC1004 sensitivity through silicone tubing:** How much capacitance change does the FDC1004 detect when 1/4" silicone tubing transitions from air to sugar syrup? Is the signal margin sufficient for reliable detection?

6. **Hopper solenoid valve — food safety:** The Beduan solenoid valves are rated for water. Sugar syrup sitting in a closed solenoid valve between uses: does it crystallize and jam the valve? May need periodic flushing.

7. **Air counter-flow in the bag opening:** When gravity-filling through a single 21mm opening, does air escape upward efficiently enough, or does it create a vapor lock that stalls filling?

8. **Clean cycle with hopper line:** Should the clean cycle flush the hopper tubing too (open hopper valve during flush phase)? This prevents stale concentrate from sitting in the dead volume.

9. **Fill completion detection:** Can the FDC1004 hopper sensor reliably distinguish "funnel empty, draining complete" from "funnel draining slowly"? Or does the user need to manually indicate "done pouring"?

10. **Bag attachment method:** The Platypus Drink Tube Kit cap threads onto the bag. Under pump suction (~0.5 psi), is the thread seal adequate, or does it need PTFE tape or an O-ring?

---

## 9. Sources

### Platypus Bag Specifications
- [Platypus Platy 2L Bottle - Cascade Designs](https://cascadedesigns.com/products/platy-2l-bottle)
- [Platypus Platy 2L - REI](https://www.rei.com/product/820769/platypus-platy-water-bottle-70-fl-oz)
- [Platypus Platy 2L - Garage Grown Gear](https://www.garagegrowngear.com/products/platy-2l-bottle-collapsible-bottle-by-platypus)
- [Platypus Drink Tube Kit - Cascade Designs](https://cascadedesigns.com/products/drink-tube-kit)
- [Platypus Replacement Closure Cap - Cascade Designs](https://cascadedesigns.com/products/closure-cap)
- [Platypus Platy 2L - Amazon](https://www.amazon.com/Platypus-Platy-2-Liter-Flexible-Bottle/dp/B0BX4YQ8C5)

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

### Tube Squeezer / Controlled Collapse Mechanisms
- [Ratchet Toothpaste Tube Squeezer - Printables.com](https://www.printables.com/model/265248-ratchet-toothpaste-tube-squeezer)
- [Toothpaste Squeezer External Gear - MakerWorld](https://makerworld.com/en/models/2423006-toothpaste-squeezer-external-gear)
- [US Patent 5167348 - Tube Squeezer](https://patents.google.com/patent/US5167348A/en)

### Funnel / Food-Safe Materials
- [Impresa Silicone Wide Mouth Funnel - Amazon](https://www.amazon.com/Pack-Squeeze-Bottle-Funnel-Dressing/dp/B07XVQKC3X)
- [Farm to Table Silicone Funnel Set - Amazon](https://www.amazon.com/Farm-Table-Canning-Silicone-3-Piece/dp/B001UAREYE)

### SodaStream Syrup
- [SodaStream Syrup Measuring - sodastreamstuff.blogspot.com](http://sodastreamstuff.blogspot.com/2013/02/measuring-perfect-amount-of-sodastream.html)
- [Syrup FAQ - Soda Centre](https://www.sodacentre.com/pages/syrup-faq)
