# Enclosure Architecture

The enclosure is the main body of a commercial under-sink flavor injection system. It houses two permanent 2L flavor bags on a diagonal cradle, a removable pump cartridge, a hopper funnel, 10 solenoid valves, displays, and all electronics. It sits in a side zone of an under-sink cabinet beside the plumbing center zone, paired with a companion carbonated water machine (Lilium, 220mm wide).

---

## 1. Enclosure Dimensions

| Parameter | Value |
|-----------|-------|
| Exterior | **220W x 300D x 400H mm** |
| Interior (4mm walls) | 212W x 292D x 392H mm |
| Interior (3mm walls) | 214W x 294D x 394H mm |
| Interior volume | ~24.3 liters (at 4mm walls) |
| Footprint | 660 cm2 |

The 220mm width matches the companion Lilium carbonator. It is driven by product-line aesthetics, not internal need. The 190mm bags plus a 200mm cradle fit within 212mm interior with 6mm clearance per side to the enclosure wall.

The 300mm depth is an intentional constraint. Under-sink cabinets offer 480-510mm of depth, so this leaves the unit well within available space. 2L bags fit at 35 degrees with corrected lens-shaped geometry (see Section 2).

The 400mm height fits typical under-sink clearance (380-420mm below the sink bowl).

Wall thickness is not yet decided. 3mm and 4mm are both viable. 3mm walls with internal ribbing are standard for ABS enclosures of this size, gain 2mm of interior per axis, and reduce weight. 4mm walls provide more rigidity without ribbing. The dimensional analysis in this document uses 4mm (worst case for interior space).

See `research/enclosure-width-reduction.md` for the full width feasibility study.

---

## 2. Diagonal Bag Layout

Two 2L Platypus bags sit stacked on a profiled 3D-printed cradle at 35 degrees from horizontal.

### 2a. Bag Shape

The bags are **lens-shaped**, not constant-thickness rectangles. Cross-sectional thickness varies along the 350mm length:

| Position | Single Bag | Two-Bag Stack |
|----------|-----------|---------------|
| Sealed end (top) | ~1mm | ~2mm |
| 25% along length | ~15mm | ~30mm |
| Center (50%) | ~40mm | ~80mm |
| 75% along length | ~25mm | ~50mm |
| Cap/connector end | ~15mm | ~30mm |

The rigid-body formula (`L cos θ + T sin θ = 333mm`) overstates depth by ~37mm because it assumes a constant 80mm thickness across the full 350mm length. The actual lens-shaped profile at 35 degrees consumes ~296mm of depth without back-wall mounting, or ~267mm with back-wall mounting.

### 2b. Mounting

The sealed end is pinned flat against the back wall at the highest point (near Z=392). The cap/connector end hangs at the front-low position (approximately Y=25, Z=125). Gravity and liquid weight position the bag naturally into the cradle.

```
    SIDE VIEW: Height (Z) vs. Depth (Y)

Z(mm)
392 +-----------------------------------------------------+
    |                                   [sealed end pinned |
    |[HOPPER]  [ELECTRONICS]             flat to back wall]|
    |funnel+    ESP32,drivers  \\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |board     above bags       \\\\\\\\\\\\\\\\\\\\\\\\\\=|
    |                      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
267 |                 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |            \\\\\\\\ bag slab (2x 2L) \\\\\\\\\\\\\\  |
    |         \\\\\\\\ lens profile, 80mm peak \\\\\\\\\\  |
    |      \\\\\\\\\\\\  on profiled cradle  \\\\\\\\\\\\  |
    |   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
125 | cap end \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    | (front-low)                                          |
    |              [VALVE RACK]                             |
 84 |              10x solenoids                            |
    | +----------+ behind dock                              |
    | |CARTRIDGE |+--------+                                |
    | |pumps only||VALVES  |=======tubes along floor=======|
    | |148x130x80||         |               [BACK PANEL:   |
    | +----------++--------+                water,soda,pwr]|
  0 +-----------------------------------------------------+
    0  (front)              165  240             292 (back)
                         Y (depth, mm)
```

### 2c. Depth Budget

With back-wall mounting, the effective depth is ~267mm, leaving a **25mm margin** in the 292mm interior. The depth problem is solved. Without back-wall mounting, the depth is ~296mm (4mm over at 4mm walls), closable by any combination of bag compression, wall thinning, or slight angle increase.

### 2d. Height Budget

Height consumed by the bag zone: ~267mm (from Z=125 at the cap end to Z=392 at the sealed end). Remaining height above bags: **~125mm** for electronics and hopper.

### 2e. Cradle

The cradle is profiled to match the lens-shaped cross-section of the stacked bags -- deepest at center (~40mm channel), tapering toward both ends. It distributes load, prevents sag, guides the bag shape, and provides the mounting structure. Material: PETG (food-safe, contacts bag exterior only). Approximate dimensions: ~350mm along the diagonal, ~200mm wide (190mm bag + 5mm margin per side).

### 2f. Permanence

The bags are permanent -- installed once during manufacturing, refilled via the hopper, never touched by the user.

See `research/2l-bags-at-300mm-depth.md` for the full depth analysis.

---

## 3. Fluid Architecture -- The 10-Valve Topology

This is the heart of the system. Ten 2-way normally-closed solenoid valves route fluid between the hopper, bags, pumps, tap water supply, dip tubes, and dispensing lines.

### 3a. Nodes

```
b1, b2 = bag bottom outlets (main fluid port P1)
t1, t2 = bag dip tubes (air bleed port P2, tube runs to top of bag)
i1, i2 = pump inlets
o1, o2 = pump outlets
p1, p2 = pumps (Kamoer peristaltic, in removable cartridge)
h      = hopper (shared, single funnel)
h1, h2 = hopper branches (one per pump line)
d1, d2 = dispenser lines (silicone tubes to faucet)
w      = tap water inlet
w1, w2 = tap water branches
```

### 3b. The 10 Valves

All 2-way normally-closed solenoid, 12V DC.

| Valve | Position | Function |
|-------|----------|----------|
| v1 | o1 → d1 | Dispense line 1, keeps flavor primed to faucet |
| v2 | o2 → d2 | Dispense line 2 |
| v3 | w1 → b1 | Tap water fill into bag 1 (pressure-fed, no pump) |
| v4 | w2 → b2 | Tap water fill into bag 2 |
| v5 | h1 → i1 | Hopper to pump 1 inlet (flavor fill + air fill) |
| v6 | o1 → b1 | Pump 1 outlet to bag 1 (reverse fill into bag) |
| v7 | h2 → i2 | Hopper to pump 2 inlet |
| v8 | o2 → b2 | Pump 2 outlet to bag 2 |
| v9 | t1 → i1 | Dip tube 1 to pump 1 inlet (air evacuation) |
| v10 | t2 → i2 | Dip tube 2 to pump 2 inlet |

**Critical: b1→i1 and b2→i2 are DIRECT connections with NO valve.** The bag bottom port connects permanently to the pump inlet. This is what keeps the valve count at 10 instead of 12. The pump always has access to the bag liquid; valves control where the pump pushes or what else feeds the inlet.

### 3c. ASCII Schematic

```
                     TAP WATER (w)
                          │
                    ┌─────┴─────┐
                   w1           w2
                    │            │
                   v3           v4
                    │            │
                   b1           b2
                  ╱    ╲      ╱    ╲
              (direct)  v6  (direct)  v8
                │        │    │        │
               i1       o1   i2       o2
                │    ┌───┘    │    ┌───┘
               p1    │       p2    │
                │    │        │    │
               o1    │       o2    │
              ╱  ╲   │      ╱  ╲   │
            v1   v6  │    v2   v8  │
             │       │     │       │
            d1      b1    d2      b2

    HOPPER (h) ────┬──────────┐
                  h1         h2
                   │          │
                  v5         v7
                   │          │
                  i1         i2

    DIP TUBES:
                  t1         t2
                   │          │
                  v9         v10
                   │          │
                  i1         i2
```

### 3d. Operating Modes

| Mode | Open Valves | Path |
|------|-------------|------|
| Dispense flavor 1 | v1 | b1→i1→p1→o1→v1→d1 |
| Dispense flavor 2 | v2 | b2→i2→p2→o2→v2→d2 |
| Fill bag 1 from hopper | v5, v6 | h→h1→v5→i1→p1→o1→v6→b1 |
| Fill bag 2 from hopper | v7, v8 | h→h2→v7→i2→p2→o2→v8→b2 |
| Fill bag 1 with tap water | v3 | w→w1→v3→b1 (gravity/pressure, no pump) |
| Fill bag 2 with tap water | v4 | w→w2→v4→b2 |
| Pump out bag 1 (water/air) | v1 | b1→i1→p1→o1→v1→d1 |
| Fill bag 1 with air (dry) | v5, v6 | h→h1→v5→i1→p1→o1→v6→b1 |
| Evacuate air via dip tube 1 | v9, v1 | t1→v9→i1→p1→o1→v1→d1 |
| Evacuate air via dip tube 2 | v10, v2 | t2→v10→i2→p2→o2→v2→d2 |
| **Idle** | **none** | **All NC valves closed, everything sealed** |

### 3e. Why 2-Way NC, Not 3-Way

3-way valves toggle between port A and port B but have no "off" position. The idle state (all sealed) is essential for preventing backflow, keeping lines primed, and handling error states. A 3-way valve with center-off does not exist as a standard product in the required form factor: small, cheap, food-safe, 1/4" barb, 12V DC. See `research/valve-architecture.md` for the full analysis.

### 3f. Valve Hardware

Beduan 12V NC solenoid, 1/4" quick-connect (Amazon B07NWCQJK9), approximately $9 each, approximately $90 for 10. Cylindrical body, ~30-35mm diameter x 50-55mm long. Designed for RO water systems -- food-grade, mass-produced, Amazon Prime eligible.

GPIO via MCP23017 I2C expander. 10 valve outputs use GPB0-GPB7 (8 pins) plus GPA0-GPA1 (2 pins).

### 3g. Valve Physical Placement

At 220mm width (212mm interior), side-bank valve placement beside the cartridge does not work -- only 31mm per side, too narrow for a 35mm-diameter valve. The valves relocate to a rack behind the cartridge dock, using the depth dimension. Two rows of 5 valves stacked vertically (Z=0-35 and Z=40-75), centered in width, occupying Y=165-240. This zone has generous clearance to the bags above.

See `research/enclosure-width-reduction.md` for the valve relocation analysis.

---

## 4. Two-Port Cap and Dip Tube

Each bag has a modified 28mm Platypus cap with **two ports**:

- **P1 (main fluid port):** Short stub or flush with cap interior. At the bottom where liquid pools. Connects to the permanent b→i line.
- **P2 (dip tube port):** 1/4" hard PE tube running the full length of the bag to the sealed end (highest point) for air evacuation during the prime cycle.

### 4a. Cap Fitting Approach

Two full-size John Guest PP1208W bulkhead fittings (each requiring a 15.9mm mounting hole) do not fit side-by-side in a 28mm cap (31.8mm combined > 24mm usable diameter). The solution is one JG bulkhead for P1 (quick-connect for serviceability) plus one barb fitting for P2 (smaller footprint, permanently installed, rarely disconnected).

A custom cap (3D-printed or machined, 28mm thread) accommodates both fittings.

### 4b. Dip Tube Tip Piece

At the top of the dip tube sits a 3D-printed air collection bar spanning the full bag width (~185mm):

- **Ship-in-a-bottle assembly:** The bar feeds through the 28mm cap opening lengthwise (cross-section 22mm x 14mm, diagonal 26.1mm), then rotates 90 degrees inside the bag to span its width.
- **Central socket** grips the 1/4" hard dip tube.
- **Air channel ribs** on the top and bottom faces prevent the bag film from sealing flat against the bar, maintaining air pathways to the central bore.
- **Only fluid-contact 3D-printed part** in the system. Material options: FDM PETG, SLS nylon, SLA resin -- food safety drives the choice.

The tip piece wedges between the bag's heat-sealed side seams, preventing lateral movement.

See `research/dip-tube-analysis.md` for the two-port architecture and `research/dip-tube-tip-design.md` for the tip piece design.

---

## 5. Pump Cartridge

A front-bottom dock, below the diagonal bags, holds the only user-replaceable module.

| Parameter | Value |
|-----------|-------|
| Contents | 2 Kamoer KPHM400 peristaltic pumps (one per flavor line) |
| Envelope | ~148W x 130D x 80H mm |
| Replacement interval | 18-36 months (pump tube wear) |
| Fluid connections | 4 (2 inlets + 2 outlets), via CPC or John Guest fittings at the dock back wall |
| Electrical connections | 3-6 pogo pins (dock ceiling) onto flat pads (cartridge top face): ground, motor A 12V, motor B 12V, optional ID and temp |

The cartridge contains **no valves, no drivers, no electronics** -- only passive contact pads and the two pumps. L298N motor drivers live in the main body. The cartridge is the simplest possible replaceable module.

Front-loading: user opens front panel, slides cartridge out along floor rails, slides new one in. Guide rails and chamfered slot entrance accept blind insertion in a dark cabinet.

See `cartridge/planning/requirements.md` for the full cartridge spec.

---

## 6. Hopper

A single funnel sits at the top-front of the enclosure, in the ~125mm vertical zone above the bags at 35 degrees.

| Parameter | Value |
|-----------|-------|
| Capacity | ~200-300ml (funnel, not reservoir) |
| Opening diameter | ~100mm |
| Serves | Both flavors -- firmware-controlled valve selection (v5/v7) determines which bag receives the pour |
| Drain | Pump-assisted (pump pulls from hopper through v5 or v7) |
| Material | Removable silicone insert, dishwasher-safe |
| Refill frequency | Weekly (families), monthly (moderate users) |

The hopper is the primary user interaction. The user pours concentrated flavor syrup into the funnel, selects the target bag via display or button, and the pump drains the hopper into the selected bag. A single hopper for both flavors eliminates redundant hardware; the user simply rinses between flavor changes if cross-contamination matters.

See `research/hopper-integration.md` for the integration analysis.

---

## 7. Displays

Two round displays, detachable via retractable flat cat6 cable (1m).

| Parameter | Value |
|-----------|-------|
| Form factor | ~50mm magnetic pucks with kickstand |
| Tether | Flat cat6 cable on retractable spool (~55mm dia, ~22mm deep per reel) |
| Protocol | UART (already in firmware), trivial over 1m cat6 |
| Display 1 | Driven by ESP32-S3 (Serial1) |
| Display 2 | Driven by RP2040 (Serial2) |
| Docked position | Flush on front panel, reels retracted |
| Extended position | Countertop, fridge (magnetic), cabinet door -- wherever the user wants |

The retractable tether is a signature UX feature. Cat6 provides power + UART data over a single cable. The pull-to-lock, pull-to-release spool mechanism is the same as retractable badge reels.

Two reels fit side-by-side (110mm) within the 212mm interior width, or stack vertically if width is tight.

See `research/display-and-front-panel.md` for the full analysis.

---

## 8. Back Panel

All external connections route through the back. The installer connects everything once during setup and never touches it again.

### 8a. Connections

| Connection | Type | Position (viewed from rear) |
|------------|------|----------------------------|
| Tap water inlet | 1/4" push-to-connect bulkhead | Lower zone, left |
| Carbonated soda water inlet | 1/4" push-to-connect bulkhead | Lower zone, center |
| Carbonated soda water outlet | 1/4" push-to-connect bulkhead | Lower zone, center-right |
| 120V AC power | IEC C14 panel-mount inlet (with fuse) | Upper zone, right |
| Flavor line 1 exit | Silicone tube pass-through (PG7/PG9 gland) | Mid zone, left |
| Flavor line 2 exit | Silicone tube pass-through (PG7/PG9 gland) | Mid zone, right |

At 212mm interior width, the back panel uses a two-row fitting arrangement. Water and flavor fittings on the lower/mid rows, power on the upper row.

### 8b. Soda Water Path

Soda water passes straight through the enclosure via a flow meter (DIGITEN, inline, pulse output). **No flavor mixing happens inside the enclosure on the soda water line.** The flow meter measures volume dispensed; firmware synchronizes flavor dosing on the separate dispense lines (d1, d2) to match soda flow.

See `research/back-panel-layout.md` for the full layout.

---

## 9. Electronics

All electronics occupy the top-back corner of the enclosure, above and behind the diagonal bags, isolated from fluid paths.

| Component | Function |
|-----------|----------|
| ESP32-S3 | Main controller, drives display 1, controls valves + pumps |
| RP2040 | Drives display 2 |
| MCP23017 | I2C GPIO expander, 10 valve outputs (GPB0-GPB7 + GPA0-GPA1) |
| L298N motor drivers | 12V PWM for pumps (in main body, not cartridge) |
| DIGITEN flow meter | Pulse input for soda water volume |
| PSU | 120V AC → 12V DC (pumps, valves) + 5V/3.3V (logic) |

The PSU sits adjacent to the IEC C14 inlet on the back panel (shortest high-voltage wire run). The 12V rail powers valves (via MOSFET drivers gated by MCP23017 outputs) and pumps (via L298N H-bridge for bidirectional control and PWM speed regulation). The 5V/3.3V rail powers the microcontrollers and display tethers.

---

## 10. Open Questions

1. **Width validation:** 220mm is the target but needs physical prototyping with valve placement and bag-in-cradle lateral fit at 212mm interior.
2. **Wall thickness:** 3mm vs 4mm -- affects interior dimensions by 2mm per axis. 3mm with ribbing is likely sufficient.
3. **CPC vs John Guest** for cartridge fluid connections. CPC ($70 for 4 pairs) gives auto-shutoff and drip-free swap. JG ($8 for 4 fittings) is cheaper but requires a cam lever release mechanism and drips during swap.
4. **Custom cap design** for the two-port bag modification. One JG bulkhead + one barb fitting in a 28mm threaded cap. 3D-printed or machined.
5. **Cradle profile** -- needs physical measurement of actual filled 2L bag cross-section to refine the channel shape.
6. **Back-wall pin method** -- clamp, channel, adhesive, or bracket for holding the flat sealed end against the wall.
7. **Tip piece material** -- FDM PETG vs SLS nylon vs SLA resin for the one food-contact 3D-printed part.
8. **Display reel mechanism** -- retractable flat cat6 spool sizing at 220mm width. Side-by-side vs vertical stacking.
9. **Valve physical placement** -- at 220mm width, side banks do not work. The behind-cartridge rack (Y=165-240, Z=0-75) is the plan but needs prototyping for tube routing density.

---

## Research Index

Every file in `research/` with a summary of what it covers.

| File | Description |
|------|-------------|
| `2l-bags-at-300mm-depth.md` | Corrected lens-shaped bag geometry proving 2L bags fit at 35 degrees in 300mm depth |
| `access-architecture.md` | Comparison of user access approaches (slide-out tray, hinged panel, removable top) |
| `back-panel-layout.md` | External connection placement, fitting types, zone allocation on the rear panel |
| `bag-dimensions-survey.md` | Manufacturer-stated Platypus bag dimensions across all sizes |
| `diagonal-bag-placement.md` | Angle sweeps, enclosure fits, and mounting analysis for diagonal bag layout |
| `diagonal-interleave.md` | The guiding spatial vision -- components share diagonal space instead of horizontal zones |
| `diagonal-risks-and-failure-modes.md` | Adversarial analysis of the diagonal layout with mitigations |
| `dip-tube-analysis.md` | Two-port cap architecture, fluid operations, air management, priming cycles, JG fitting analysis |
| `dip-tube-tip-design.md` | 3D-printed air collection bar: ship-in-a-bottle assembly, cross-section, air channels, materials |
| `display-and-front-panel.md` | Retractable cat6 spool geometry, spring mechanism, locking, cat6 pinout for power + UART |
| `enclosure-width-reduction.md` | Feasibility study for shrinking from 280mm to 220mm width, valve relocation options |
| `hopper-integration.md` | Hopper funnel integration with back-wall-mounted bags at 35 degrees |
| `master-spatial-layout.md` | Synthesized component coordinates and cross-sections for the full diagonal interleave layout |
| `under-sink-constraints.md` | Real cabinet dimensions across US and international markets |
| `valve-architecture.md` | Flow routing topology, valve truth table, 2-way NC vs 3-way analysis, specific valve candidates |
