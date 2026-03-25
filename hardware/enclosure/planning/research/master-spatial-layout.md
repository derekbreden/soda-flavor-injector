# V1 Master Spatial Layout: Diagonal Interleave

**Synthesized:** 2026-03-24
**Sources:** v1-diagonal-bag-placement.md, v1-cartridge-dock-placement.md, v1-hopper-integration.md, diagonal-interleave.md (Vision 1), access-architecture.md, fitting-alternatives.md, cascade-matrix.md, 2l-bags-at-300mm-depth.md

This is the master reference for the Vision 1 diagonal interleave layout. It defines exact component positions, clearances, tube routing, and structural members for the unified product layout: **280W x 300D x 400H with 2L bags at 35 degrees**. Where research is conclusive, positions are stated as coordinates. Where physical testing is needed, ranges are given with the uncertainty flagged.

**Key correction (2026-03-24):** Prior versions of this document carried two layout variants (V1-A for 1L bags at 300mm depth, V1-B for 2L bags at 350mm depth). The V1-B variant existed because the rigid-body rectangle model predicted 333mm of depth for 2L bags at 35 degrees, which did not fit in 292mm interior depth. The corrected lens-shaped bag model (see `2l-bags-at-300mm-depth.md`, `2l-rigid-body-geometry.svg`) shows 2L bags actually consume ~296mm of depth at 35 degrees without back-wall mounting, or ~267mm with back-wall mounting. The rigid model overstated depth by ~37mm. Since 2L bags fit at 300mm depth, V1-B's reason for existing is gone. This document now presents a single unified layout.

---

## Coordinate System

**Origin:** Interior front-bottom-left corner.

- **X axis (width):** Left wall = 0, right wall = interior width. Positive rightward.
- **Y axis (depth):** Front wall = 0, back wall = interior depth. Positive rearward.
- **Z axis (height):** Floor = 0, ceiling = interior height. Positive upward.

All dimensions are interior (exterior minus 8mm per axis: 4mm walls on all sides).

---

## 1. Enclosure

| Parameter | Value |
|-----------|-------|
| Exterior | 280W x 300D x 400H mm |
| Interior | 272W x 292D x 392H mm |
| Interior volume | 31.1 liters |
| Wall thickness | 4mm all sides |

---

## 2. Bag Configuration

| Parameter | Value |
|-----------|-------|
| Bag model | Platypus 2.0L |
| Bag dimensions | 350mm long x 190mm wide |
| Stack thickness (peak) | 80mm (two bags stacked, ~40mm each at center) |
| Mounting angle | 35 degrees from horizontal |
| Mounting method | Back-wall mount: sealed end pinned flat to back wall, cap end at front-low |
| Support | 3D-printed profiled cradle underneath; gravity positions bag |
| Effective depth consumed | ~267mm (with back-wall mounting) |
| Height consumed by bag zone | ~267mm (cradle to sealed-end pin) |
| Remaining height above bags | ~125mm (for electronics + hopper) |

### 2a. Why the Depth Works

The rigid-body formula (`L cos theta + T sin theta = 333mm`) overstates depth by ~37mm because it treats the bag as a constant-thickness rectangle. Real Platypus bags are lens-shaped: ~2mm at the sealed end, ~80mm stacked at center, ~30mm at the cap. The actual bag envelope at 35 degrees consumes ~296mm of depth without back-wall mounting.

With back-wall mounting (sealed end pinned flat to the back wall), the effective depth drops further to ~267mm, leaving a **25mm margin** in the 292mm interior. See `2l-bags-at-300mm-depth.md` for the full derivation and `2l-rigid-body-geometry.svg` for a visual comparison.

### 2b. Bag Slab Position Derivation

The sealed end is pinned flat against the back wall at the top. The cap/connector end hangs at the front-low position. With back-wall mounting, the effective depth envelope spans from Y = 292 (back wall) to approximately Y = 25 (267mm forward from the back wall).

The bag zone occupies height from approximately Z = 125 (bottom of cradle at the front-low cap end) to Z = 392 (sealed end pinned at back wall near ceiling).

At the cap end (front-low), the bag stack center is at approximately:

    Y_cap ≈ 25mm (from front wall)
    Z_cap ≈ 125mm (bottom of cradle)

At the sealed end (back-high), the bag is pinned flat:

    Y_sealed = 292mm (back wall)
    Z_sealed ≈ 392mm (near ceiling)

The profiled cradle follows the lens shape of the bags, providing continuous support along the underside. The cradle is wider/deeper at the bag center and tapers toward both ends.

At cartridge height (Z = 80mm), the entire floor area is available -- the bags are well above this zone at their lowest point (~Z = 125mm at the front).

### 2c. Side-View Cross-Section (Height Z vs. Depth Y)

Viewing from the right side. Width is into the page.

```
Z(mm)
392 +-----------------------------------------------------+
    |                                   [sealed end pinned |
    |[HOPPER]  [ELECTRONICS]             flat to back wall]|
    |funnel+    ESP32,drivers  \\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |board     above bags       \\\\\\\\\\\\\\\\\\\\\\\\\\=|
    |                      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
267 |                 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |            \\\\\\\\ bag slab (2x 2L) \\\\\\\\\\\\\\  |
    |         \\\\\\\\ lens profile, 80mm peak \\\\\\\\\\  |
    |      \\\\\\\\\\\\  on profiled cradle  \\\\\\\\\\\\  |
    |   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
125 | cap end \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    | (front-low)                                          |
    |[8x VALVES]                                           |
 84 | +----------+                                         |
    | |CARTRIDGE |                                         |
    | |pumps only|=========================================|
    | |150x130x80|  tubes along floor    [BACK PANEL:     |
    | +----------+                        water,soda,pwr]  |
  0 +-----------------------------------------------------+
    0  (front)                                    292 (back)
                         Y (depth, mm)
```

### 2d. Front-View Cross-Section (Height Z vs. Width X)

Viewing from the front. Depth is into the page.

```
Z(mm)
392 +---------------------------------------------+
    |          +------------------+                |
    |          |  HOPPER FUNNEL   |                |
322 |          |  (100mm dia)     |                |
    |          +--------+---------+                |
    |                   | tubing                   |
    |                   |                          |
    |      bag slab extends into page              |
    |      (190mm wide, centered)                  |
    |      on profiled cradle                      |
    |                                              |
    |                                              |
    |                                              |
 84 | +---------------------------------------+    |
    | | 8x NC SOLENOID  |   CARTRIDGE SLOT  |    |
    | | VALVES (4/side)  |  (150mm, centered)|    |
    | |                  |   front opening   |    |
  0 +-+---------------------------------------+----+
    0                  136                       272
                      X (width, mm)
```

Note: Displays are not shown in the front-view cross-section because they are **detachable 50mm magnetic pucks** connected via retractable flat Cat6 cable (1m). They can be placed anywhere convenient -- on the front face, on the cabinet door, or on the countertop. They communicate via UART protocol.

### 2e. Top-View Cross-Section (Width X vs. Depth Y)

Viewing from above. Height is into the page.

```
Y(mm)
292 +---------------------------------------------+
    |      [BACK PANEL: water in, soda in/out,    |
    |       power, flavor tube exits]              |
    |       sealed ends pinned to back wall        |
    |                                              |
    |      bag slab (190mm wide, centered)         |
    |      extends diagonally into page            |
    |      from Y~292 (sealed, high Z)             |
    |      to Y~25 (cap end, low Z)                |
    |                                              |
    |======== tubes along floor ========           |
    |                                              |
165 | +----------------------------------+         |
    | |    DOCK BACK WALL (fittings)     |         |
    | +----------------------------------+         |
130 |                                              |
    | +----------------------------------+         |
    | |         CARTRIDGE                |         |
    | |      (150W x 130D x 80H)        |         |
    | |    2x Kamoer pumps only          |         |
    | +----------------------------------+         |
  0 +---------------------------------------------+
    0          41              231              272
                      X (width, mm)
```

---

## 3. Component Position Table

Origin at interior front-bottom-left corner. All values in mm.

| Component | X range (width) | Y range (depth) | Z range (height) | Dimensions (WxDxH) | Notes |
|-----------|----------------|-----------------|-------------------|---------------------|-------|
| Bag slab (2x 2L) | 41-231 | 25-292 | ~125-392 (varies diag.) | 190W x 267D x 267H bounding | Lens profile on profiled cradle; sealed end pinned to back wall |
| Cartridge (pumps only) | 61-211 | 0-130 | 0-80 | 150W x 130D x 80H | Contains ONLY 2x Kamoer peristaltic pumps. No valves, no drivers, no electronics. Front-bottom dock. |
| Dock back wall | 61-211 | 130-165 | 0-84 | 150W x 35D x 84H | JG fittings, pogo pins |
| Lever / handle zone | 61-211 | 0-20 | 80-124 | 150W x 20D x 44H | Cartridge pull handle |
| Hopper funnel | 86-186 | 0-80 | 322-392 | 100dia x 70H | Constrained to Y=0-80 for bag clearance |
| Hopper lid zone | 86-186 | 0-80 | 392 (top surface) | 100dia x 10H | Hinged or removable |
| Electronics zone | 41-231 | 200-292 | 275-392 | ~190W x 92D x ~117H | Above rear portion of bags. ESP32, motor drivers, fuse block. Space shared with upper bag surface -- available height varies by depth. |
| Solenoid valves (8x NC two-way) | 0-60 + 212-272 | 80-160 | 0-70 | ~30W x 40D x 35H each, 4 per side | 4 valves flanking each side of cartridge. 8 two-way NC: simple, cheap, Amazon Prime available, independent "all closed" position. |
| Displays (2x round) | Detachable | Detachable | Detachable | 50mm dia magnetic pucks | Retractable flat Cat6 cable (1m), UART protocol. Mount anywhere. |
| Drip tray | 0-272 | 0-50 | 0-4 | 272W x 50D x 4H | Removable, beneath cartridge area |
| Tube channels (floor) | 41-231 | 130-267 | 0-10 | 190W x 137D x 10H | Printed U-channels for tube routing |
| Back panel | 0-272 | 288-292 | 0-392 | 272W x 4D x 392H | All external connections: water in, soda in/out, power, flavor tube exits |

**Notes on the bag slab coordinates:** The bag slab is a diagonal lens-shaped profile, not an axis-aligned box. The X range (41-231) is the centered width of 190mm bags in a 272mm interior. The Y and Z ranges reflect the bounding envelope of the back-wall-mounted diagonal: sealed end pinned flat at back wall (Y=292, Z~392), cap end hanging at front-low (Y~25, Z~125). The profiled cradle supports the underside continuously.

---

## 4. Clearance Analysis

| Component Pair | Clearance | Assessment |
|----------------|-----------|------------|
| Cartridge top (Z=80) to bottom of bag cradle at Y=130 | Bag cradle at Y=130 is at approximately Z=233. Gap = 233 - 80 = **153mm** | Very generous |
| Cartridge rear (Y=165 incl. dock wall) to bag cap end (Y~25) | Bag cap is at front, cartridge at front. Vertical separation at Y=130: **153mm** above cartridge. | Generous |
| Hopper rear edge (Y=80) to bag upper surface at Y=80 | Bag upper surface at Y=80: approximately Z = 392 - (292-80)*tan(35) = 392 - 212*0.700 = 392 - 148 = **244mm**. Hopper bottom at Z=322. Gap = 322 - 244 = **78mm**. | Comfortable |
| Electronics zone (Z=275+) to bag upper surface at Y=200 | Bag upper at Y=200: Z = 392 - (292-200)*0.700 = 392 - 64 = **328mm**. Electronics start at Z=275. Electronics are above bags at Y>200 where bag surface drops below 275. | Electronics share airspace above rear bag section |
| Solenoid valves (sides, Z=0-70) to cartridge (center, Z=0-80) | Valves at X=0-60 and X=212-272; cartridge at X=61-211. **1mm lateral gap** minimum. | Adequate; separated in X |
| Bag width (190mm) to enclosure walls | (272 - 190) / 2 = **41mm** per side | Adequate for cradle mounting and tubing |
| Back panel to bag sealed end | Sealed end pinned directly to back wall. **0mm by design.** | By design |

**Identified tight clearances:**
1. Solenoid valves to cartridge: 1mm lateral gap. Valves and cartridge do not overlap in X, but tolerances must be managed.
2. Bag upper surface near hopper: 78mm gap at Y=80 is comfortable. Hopper must stay within Y=0-80 as designed.

**No interferences detected** with the constraints above.

---

## 5. Valve Architecture

The system uses **8 two-way normally-closed (NC) solenoid valves**, arranged as 4 per pump. All valves are in the main enclosure body (permanent), not in the cartridge.

| Valve | Function | Location |
|-------|----------|----------|
| V1-A | Flavor 1: bag to pump inlet | Left bank (X=0-60) |
| V1-B | Flavor 1: pump outlet to dispense nozzle | Left bank (X=0-60) |
| V1-C | Flavor 1: hopper to pump inlet | Left bank (X=0-60) |
| V1-D | Flavor 1: pump outlet to bag (refill) | Left bank (X=0-60) |
| V2-A | Flavor 2: bag to pump inlet | Right bank (X=212-272) |
| V2-B | Flavor 2: pump outlet to dispense nozzle | Right bank (X=212-272) |
| V2-C | Flavor 2: hopper to pump inlet | Right bank (X=212-272) |
| V2-D | Flavor 2: pump outlet to bag (refill) | Right bank (X=212-272) |

**Why 8 two-way NC instead of 4 three-way:** Simple, cheap, Amazon Prime available, and provides an independent "all closed" default position. With NC valves, power loss means all fluid paths are sealed. Three-way valves would always have one path open.

---

## 6. Cartridge Design

The cartridge is a front-bottom dock module containing **only 2 Kamoer peristaltic pumps**. No valves, no drivers, no electronics.

| Parameter | Value |
|-----------|-------|
| Dimensions | 150W x 130D x 80H mm |
| Contents | 2x Kamoer peristaltic pumps |
| Electrical connection | Pogo pins on dock ceiling contact pads on cartridge top (motor power only) |
| Fluid connections | 4x JG 1/4" push-to-connect fittings on dock back wall (2 inlet, 2 outlet) |
| Insertion | Front-bottom, slide along floor rails |
| Replacement interval | Every 18-36 months (pump tube wear) |

All valve control, motor driving, and intelligence remain in the main enclosure permanently.

---

## 7. Back Panel

All external connections route through the back panel (Y = 288-292mm):

| Connection | Type | Position |
|------------|------|----------|
| Water in | 1/4" push-fit | Back panel, lower section |
| Soda water in | 1/4" push-fit | Back panel, lower section |
| Soda water out (to tap) | 1/4" push-fit | Back panel, lower section |
| Power | DC barrel jack or IEC C14 | Back panel, upper section |
| Flavor tube exits (2x) | 1/4" push-fit or barb | Back panel, lower section |

This keeps the front face clean (only cartridge slot and hopper access) and all plumbing/wiring accessible from the rear when the enclosure is pulled forward on its slide-out tray.

---

## 8. Tube Routing Map

### 8a. Fluid Line Topology

Each flavor line has the following components in series, controlled by 4 NC solenoid valves:

**Dispensing mode (V-A and V-B open):**
```
BAG --> valve V-A --> pump (in cartridge) --> valve V-B --> dispensing nozzle (exits back panel)
```

**Refill mode (V-C and V-D open):**
```
HOPPER FUNNEL --> valve V-C --> pump (in cartridge, reversed) --> valve V-D --> BAG
```

**Idle (all valves closed):** All 8 NC valves closed. No fluid path open. Safe power-loss state.

The cartridge contains only the two pumps. All valves are permanent in the enclosure body.

### 8b. Tube Routing Paths

**Segment 1: Bag connector to valve V-A (bag inlet valve)**

- Bag connectors at approximately (136, 25, 125) -- centered in X, at cap end (front-low)
- Valve V-A mounted on enclosure side walls at approximately (30, 120, 30) and (242, 120, 30)
- Path: from cap end along floor, lateral to valve bank
- Length: ~120mm per line

**Segment 2: Valve V-A outlet to cartridge inlet (dock back wall)**

- Valve V-A at (30, 120, 30) and (242, 120, 30)
- Dock fittings at approximately (100, 135, 40) and (172, 135, 40)
- Path: short run from valve to dock fitting
- Length: ~80mm per line

**Segment 3: Cartridge outlet (dock back wall) to valve V-B (dispense valve)**

- Dock outlet fittings at approximately (100, 135, 55) and (172, 135, 55)
- Valve V-B at approximately (30, 140, 30) and (242, 140, 30)
- Path: lateral run from dock wall to valve bank
- Length: ~80mm per line

**Segment 4: Valve V-B outlet to dispensing nozzle (exits back panel)**

- Valve V-B at (30, 140, 30) and (242, 140, 30)
- Nozzle exits through back panel at (30, 292, 30) and (242, 292, 30)
- Path: straight back along floor to back panel
- Length: ~150mm per line

**Segment 5: Hopper funnel to valve V-C (hopper inlet valve)**

- Hopper funnel outlet at approximately (136, 50, 322)
- Valve V-C at approximately (30, 100, 30) and (242, 100, 30)
- Path: vertical drop from funnel down front wall to floor, then lateral to valve
- Length: ~350mm per line (long vertical run)

**Segment 6: Valve V-D (refill outlet) to bag**

- Valve V-D at approximately (30, 80, 30) and (242, 80, 30)
- Bag connector at approximately (136, 25, 125)
- Path: from valve bank to cap end
- Length: ~120mm per line

### 8c. Summary: Dead Volume per Flavor Line (Dispensing Path)

| Segment | Length (mm) | ID (mm) | Dead Volume (ml) |
|---------|-------------|---------|-------------------|
| Bag to valve V-A | 120 | 6.35 | 3.8 |
| Valve V-A to cartridge inlet | 80 | 6.35 | 2.5 |
| Cartridge outlet to valve V-B | 80 | 6.35 | 2.5 |
| Valve V-B to nozzle (back panel) | 150 | 6.35 | 4.8 |
| **Total dispensing path** | **430** | | **13.6** |

Total dead volume per flavor line (dispensing): ~13.6ml.
Total dead volume (both lines, dispensing): ~27.2ml.

### 8d. Tube Routing Design Notes

- All floor-level tubes run in printed channels (U-profile, 10mm wide x 8mm deep) to prevent kinking and keep the floor organized.
- Tubes cross under the bag cradle in the large front-bottom void. They never need to route over or around the bags.
- The hopper fill tubes are the longest runs in the system (~350mm vertical drop from ceiling to floor). These tubes should be silicone (flexible, food-grade) routed along the front wall interior, clipped at 100mm intervals.
- All flavor tube exits route through the back panel, keeping the front face clean.

---

## 9. Access and Serviceability

### 9a. Hopper Refill (Weekly)

**Sequence:**
1. Open cabinet door
2. Pull slide-out tray forward ~200-300mm (enclosure comes to cabinet door opening under room light)
3. Flip hinged top lid (or remove top panel with magnets/snap-fits). Lid is ~40mm tall, hinges at rear edge. At 90-degree open, total height = 400 + 35mm (tray) + 40mm (lid) = 475mm. Fits within standard 500-600mm cabinet clearance.
4. Select flavor on detachable display (tap left or right display puck, or single button press)
5. Pour concentrate from bottle into silicone funnel. Funnel holds 200-300ml buffer. Pump runs in reverse, pulling syrup from funnel through valve V-C into the bag via valve V-D. Pour rate matches pump rate (~200-400 ml/min). For 2L refill: 5-10 minutes of pouring.
6. Display shows progress. When bag-side capacitive sensor detects full, pump stops and display shows "Complete."
7. Optionally pour 50ml of clean water to flush the hopper line.
8. Remove silicone funnel, rinse in sink (30 seconds). Replace.
9. Close lid, push tray back, close cabinet door.

**Tool-free.** No tools required for any step.

### 9b. Cartridge Swap (Every 18-36 Months)

**Sequence:**
1. Open cabinet door
2. Pull slide-out tray forward
3. Cartridge slot is at the bottom of the front panel (Z=0 to Z=84mm, or Z=0 to Z=124mm with lever)

4. Flip cam lever on cartridge front face to release position
5. Pull cartridge forward -- release plate depresses all 4 JG collet rings simultaneously
6. Slide old cartridge out along floor rails. Set on cabinet floor.
7. Slide new cartridge in along floor rails; JG collets grip automatically on tube stub insertion
8. Flip cam lever to locked position
9. Pogo pins on dock ceiling make electrical contact with cartridge top pads automatically.
10. Push tray back, close door.

**Tool-free.** The firmware enforces a clean cycle before removal, so any residual drip is water only.

### 9c. Front Face Layout (Bottom to Top)

```
Z(mm)  FRONT PANEL
392 +--------------------------------------+
    |        [ LID / TOP ACCESS ]          | <- hinged or removable
    |     (hopper funnel beneath)          |
322 +--------------------------------------+
    |                                      |
    |         product logo                 |
    |    (displays are detachable pucks,   |
    |     not mounted on front panel)      |
    |                                      |
    |         (blank panel area)           |
    |                                      |
124 +--------------------------------------+
    |     (lever clearance zone if JG)     | <- cam lever swing
 84 +--------------------------------------+
    |  +------------------------------+    |
    |  |     CARTRIDGE SLOT           |    | <- 150mm wide opening
    |  |   (pull handle or cam lever) |    |
    |  +------------------------------+    |
  0 +--------------------------------------+
```

The front face has three visual zones:
1. **Bottom (0-124mm):** Cartridge maintenance zone. Infrequently accessed. Slot can be a contrasting color or have a subtle pull handle.
2. **Middle (124-322mm):** Branding zone. No fixed displays -- the two round display pucks are detachable magnetic units on retractable Cat6 cables. User places them wherever convenient.
3. **Top (322-392mm):** Hopper access zone. Lid or panel that opens for refilling.

### 9d. Components Requiring Tools

**None for user-facing operations.** All user tasks (hopper refill, cartridge swap) are tool-free.

Internal service (bag replacement, electronics repair) requires disassembly of the enclosure shell, which uses screws. These are manufacturing/repair operations, not user operations. Bags are permanent and never replaced by the user.

---

## 10. Structural Members

### 10a. Profiled Bag Cradle

**Purpose:** Support the diagonal lens-shaped bag slab, prevent sag, maintain consistent drainage angle, and conform to the actual bag profile rather than assuming a flat rectangle.

**Design:** A 3D-printed PETG cradle that follows the lens-shaped cross-section of the stacked bags. The cradle is widest/deepest at the bag center and tapers toward both ends. It runs the full 190mm width of the bags.

| Parameter | Value |
|-----------|-------|
| Cradle length (along incline) | ~350mm (bag length) |
| Cradle width | 190mm (2L bag width) |
| Cradle profile | Lens-shaped: ~2mm deep at sealed end, ~40mm deep at center, ~15mm at cap end |
| Material | PETG (3D printed for prototype) |
| Mount points | Back wall (sealed end pin, Z~380-392) and front wall bracket (Z~125) |
| Load capacity needed | 4.4kg (two full 2L bags + liquid) |

A thin PETG separator sheet (1-2mm) between the two stacked bags prevents them from sticking.

**Attachment:** Cradle anchors to the back wall via the sealed-end pin mount (heat-set inserts) and to the side walls at one or two intermediate points to prevent lateral shift. The front-low end rests on a bracket attached to the side walls.

### 10b. Cartridge Guide Rails

**Purpose:** Guide the cartridge during insertion/removal, carry cartridge weight, prevent lateral wobble.

**Design:** Two parallel floor rails (2mm tall x 3mm wide, full slot depth of 130mm) on the enclosure floor. Two side wall guides (1.5mm wide rails, 0.3-0.5mm clearance per side) on the cartridge slot side walls.

| Parameter | Value |
|-----------|-------|
| Floor rail length | 130mm (cartridge depth) |
| Floor rail height | 2mm |
| Side guide length | 130mm |
| Chamfer at entrance | 5mm at 30-degree taper for blind insertion |
| Material | Integral to enclosure shell (injection molded PP/ABS) |

### 10c. Electronics Shelf

**Purpose:** Mount the ESP32, motor driver board, fuse block, and wiring in the upper-rear zone above the bags.

**Design:** A horizontal shelf or L-bracket in the upper-rear area. The shelf is screwed to the back wall and one side wall. It sits above the rear portion of the bag slab where the bags are thinnest (near the sealed end).

| Parameter | Value |
|-----------|-------|
| Shelf position | Z=275, Y=200-292, X=41-231 |
| Shelf dimensions | ~190W x 92D mm |
| Available height above shelf | ~117mm (to ceiling at Z=392) |
| Material | 3mm PETG sheet (prototype), injection molded PP/ABS (production) |
| Mounting | 2 screws into heat-set inserts on back wall + 1 screw into each side wall |

Components mounted on the shelf: ESP32 dev board (~50x25mm), motor driver (~40x25mm), fuse holder (~30x15mm), terminal blocks for solenoid and display wiring. All components fit within the ~190x92mm footprint at well under 117mm stack height.

### 10d. Hopper Funnel Mount

**Purpose:** Hold the removable silicone funnel in a fixed position at the top of the enclosure.

**Design:** A rigid PP or ABS mounting ring permanently attached to the enclosure structure just below the lid line. The silicone funnel press-fits into the ring. The ring has a 15mm outlet hole connecting to hopper tubing.

| Parameter | Value |
|-----------|-------|
| Ring outer diameter | 110mm |
| Ring inner diameter | 100mm (matches funnel) |
| Ring height | 15mm |
| Position | Centered at X=136, Y=40, Z=315-330 |
| Attachment | 2 screws into front wall or integral to enclosure molding |

### 10e. Dock Back Wall

**Purpose:** Hold JG fluid fittings, alignment features, and pogo pin housing.

**Design:** A vertical wall spanning the cartridge slot width, at the rear of the slot. Contains 4 fluid fitting bores (2x2 grid) and a pogo pin housing on its upper surface.

| Parameter | Value |
|-----------|-------|
| Wall position | Y=130-165 |
| Wall dimensions | 150W x 35D x 84H mm |
| Material | PP or ABS (injection molded) |
| Fitting bores | 4x 12.7mm (JG), in 2x2 grid with 30mm center spacing |
| Pogo pin housing | 2-pin block on top surface, Z=80-84mm (motor power only) |

### 10f. Material Summary

| Component | Prototype Material | Production Material | Notes |
|-----------|-------------------|---------------------|-------|
| Enclosure shell | 3D printed PETG | Injection molded PP or ABS | 4mm walls |
| Bag cradle | 3D printed PETG | Injection molded PP or stainless frame | Profiled to lens shape, must handle 4.4kg at 35 deg |
| Cartridge guide rails | Integral to shell | Integral to shell | PP/ABS |
| Electronics shelf | 3mm PETG sheet | Molded as part of back wall | PP/ABS |
| Hopper funnel | Food-grade silicone (Shore 40-60A) | Same | Removable, dishwasher safe |
| Funnel mount ring | 3D printed PETG | Injection molded PP | Rigid |
| Dock back wall | 3D printed PETG | Injection molded PP or ABS | Holds fittings under load |
| Drip tray | 3D printed PETG | Injection molded PP | Removable, food-grade |
| Bag separator | 1-2mm PETG sheet | Thin PP sheet | Between stacked bags |

### 10g. Enclosure Wall Attachment

All internal structural members attach to the enclosure walls via one of:
- **Heat-set brass inserts** (M3, 5mm long): For prototype PETG enclosures. Press in with soldering iron. Reusable.
- **Boss-and-screw** (integral molded bosses): For production injection molded enclosures. Bosses on inner wall surfaces receive self-tapping screws.
- **Snap-fit tabs**: For the drip tray and hopper funnel mount (removable by design).

---

## 11. Thermal and Ventilation

### 11a. Heat Sources

| Component | Power Dissipation | Location |
|-----------|-------------------|----------|
| ESP32 | ~0.5W continuous, ~1W peak (WiFi) | Electronics shelf, upper-rear |
| Motor drivers (2x) | ~0.5W each during operation (intermittent) | Electronics shelf |
| Solenoid valves (8x NC) | ~2-4W each when energized (intermittent, <30s at a time) | Side banks, front-bottom |
| Kamoer pumps (2x, in cartridge) | ~10W each at peak, ~5W sustained | Cartridge, front-bottom |

**Total sustained heat during dispensing:** ~14-18W (both pumps + up to 4 solenoids + ESP32 + drivers).
**Idle heat:** ~0.5-1W (ESP32 only).

### 11b. Thermal Path

Heat from electronics (upper-rear) rises naturally upward and exits through the enclosure top. The under-sink cabinet provides natural convection -- warm air rises out the top of the cabinet, cooler air enters from below.

Heat from the cartridge pumps (front-bottom) is intermittent (30-60 seconds per dispensing event) and dissipates through the cartridge shell and surrounding air. The large front-bottom void provides ample air volume for convective cooling.

Solenoid valve heat is intermittent and low total energy. Not a concern.

### 11c. Ventilation

**Ventilation slots (recommended):**
- **Top panel:** 4-6 slots (3mm x 30mm each) along the rear edge of the top panel, above the electronics shelf. Warm air exits here.
- **Bottom panel:** 4-6 matching slots along the rear of the enclosure floor, behind the cartridge slot. Cool air enters here. These also provide drainage for any condensation or minor spills.
- **Slot orientation:** Slots run left-right (parallel to the width axis) to minimize dust ingress while allowing vertical airflow.

**No fan required.** The peak sustained power (~18W) in a 31L enclosure produces a negligible temperature rise. ESP32 operates reliably up to 85C; ambient under-sink temperature is 20-30C. Even with zero ventilation, the internal temperature rise from 18W intermittent load would be <5C.

### 11d. Proximity of Electronics to Fluid

| Electronics Component | Nearest Fluid Component | Distance | Risk |
|----------------------|------------------------|----------|------|
| ESP32 (upper-rear) | Bag slab (diagonal, nearest at back wall where bags are flat film) | ~0mm vertical at back wall but bags are sealed flat film there | Very low. Bags are sealed, permanently mounted. No splash risk. |
| Motor drivers (upper-rear) | Same as ESP32 | Same | Same |
| Solenoid valves (side banks) | Cartridge fittings (dock back wall) | 30-60mm | Moderate. Solenoids are IP65 rated bodies, water resistant. A drip during cartridge swap could reach them but does no damage. |
| Pogo pins (dock ceiling) | JG fittings (dock back wall) | 30-40mm | Managed by physical separation. Pogo pins face down from ceiling; fittings face rearward from wall. Gravity pulls drips away from pins. |

---

## 12. Conflicts and Unresolved Questions

### 12a. Resolved from Prior Versions

**1. 2L bag depth at 35 degrees (RESOLVED):**
The rigid-body model predicted 333mm, which did not fit in 292mm interior. The corrected lens-shaped bag model shows ~296mm without back-wall mounting, ~267mm with back-wall mounting. 2L bags fit at 35 degrees in the 300mm enclosure with 25mm margin. See `2l-bags-at-300mm-depth.md`.

**2. Two variants collapsed to one (RESOLVED):**
V1-A (1L at 300mm depth) and V1-B (2L at 350mm depth) have been collapsed into a single unified layout: 280W x 300D x 400H with 2L bags at 35 degrees. The deeper enclosure variant is no longer needed.

**3. Valve architecture (RESOLVED):**
8 two-way NC solenoid valves (4 per pump), not 4 three-way. Simpler, cheaper, provides "all closed" default.

### 12b. What Needs Physical Prototyping

1. **Bag deformation on profiled cradle at 35 degrees.** Does the lens-shaped bag profile match the cradle well enough? Does the bag bulge beyond the cradle edges when full? This determines whether the 25mm depth margin (with back-wall mounting) is sufficient.

2. **Back-wall mounting reliability.** Does the sealed end stay pinned to the back wall under liquid load? What pin/clamp design works best for the flat film sealed end?

3. **Bag sag on cradle.** How well does the profiled cradle support the bag center when full? If the cradle shape does not match the actual filled bag profile closely enough, drainage may be inconsistent.

4. **Dip tube behavior during drainage at 35 degrees.** At what fill level does air first reach the dip tube opening? This determines the practical residual volume.

5. **Dip tube behavior during refilling.** Can the pump push concentrate uphill through the dip tube into an angled bag? Air must counter-flow downward past the rising concentrate. At 35 deg for 2L, the hydrostatic head is manageable but air displacement needs testing.

6. **Hopper funnel position.** Can the user comfortably pour into a funnel at Z=322-392mm when the enclosure is on a slide-out tray at cabinet floor level (~200mm below countertop)? Total pour height is approximately 200mm (cabinet floor) + 35mm (tray) + 322mm (funnel bottom) = 557mm above floor, or roughly at waist height for a standing adult.

7. **Cartridge floor-level insertion.** Is sliding a cartridge along the cabinet floor into a bottom-mounted slot ergonomically acceptable?

8. **8-valve solenoid bank packaging.** Do 4 two-way NC solenoids fit in each side bank (X=0-60 and X=212-272)? Stack vertically (2 high) if needed.

### 12c. Open Design Decisions

1. **Cam lever release plate geometry.** The stepped bore dimensions and push rod routing for the JG release plate need physical prototyping to confirm reliable collet engagement.

2. **One hopper funnel vs two.** Single funnel with firmware-controlled valve selection is recommended. Two physical funnels are simpler but use more space. Decision depends on firmware UI readiness.

3. **Hopper capacity: 200ml vs 300ml.** 200-300ml is recommended as a funnel (user pours while pump drains). Space allows up to ~300ml given the 78mm clearance above the bag surface at Y=80.

4. **Slide-out tray: included or BYO?** Recommend specifying compatibility with standard under-sink slide-out trays ($15-35) and letting the user provide their own.

5. **Display placement convention.** The detachable magnetic pucks can go anywhere within 1m cable reach. Should the product suggest a default location (front panel, cabinet door, countertop) in the user guide?

---

## 13. Comparison: Vision 1 Diagonal vs. Archived Zone-Based Layout

The zone-based (horizontal layer cake) layout was the prior assumption. Components stacked in horizontal slices: bags at the bottom, dock shelf in the middle, cartridge above that, electronics on top.

### 13a. Comparison Table

| Criterion | Vision 1 (Diagonal Interleave) | Zone-Based (Horizontal Layers) |
|-----------|-------------------------------|-------------------------------|
| **Enclosure** | 280x300x400 (33.6L) | 280x250x400 (28.0L) |
| **2L bags feasible?** | Yes, at 35 deg in 300mm depth (lens model) | Only at 55-65 deg in 450H enclosure, with severe height budget pressure |
| **Height budget conflicts** | None. Components share height at different depths. | Severe. Every mm of bag zone steals from dock/electronics zone and vice versa. |
| **Width budget conflicts** | None at the cartridge. Bags and cartridge are at different depths. | None (same -- full width available per zone). |
| **Depth budget** | Bags consume ~267mm (back-wall mounted). Enclosure is 300mm deep. | Bags lie flat, consume less depth (~152mm). Enclosure can be shallower. |
| **Cartridge ergonomics** | Floor-level insertion. User rests cartridge on cabinet floor during alignment. | Mid-height insertion. User holds cartridge in the air while aligning. |
| **Tube routing** | Front-to-back along floor. ~430mm per dispensing line. | Short vertical runs. ~150-200mm per line. |
| **Dead volume** | ~14ml per line (longer tubes) | 6-8ml per line (shorter tubes) |
| **Drainage** | Gravity-assisted at 35 deg. Good (sin=0.574). | Gravity-assisted at 18-45 deg. Range depends on configuration. |
| **Internal structure** | Profiled cradle, no horizontal shelves mid-enclosure. | Horizontal shelves at zone boundaries. Simple stacking. |

### 13b. What Vision 1 Gains

1. **2L bag compatibility without increasing height or depth.** The diagonal uses the enclosure cross-section diagonal. The corrected lens model confirms 2L bags fit at 35 degrees in only 300mm of depth.
2. **Elimination of height budget competition.** Every component has ample space.
3. **Better cartridge ergonomics.** Floor-level insertion is easier than mid-height insertion when crouching under a sink.
4. **Structural simplicity at the dock.** No mid-height shelf cantilevering the dock and cartridge weight. The dock is at the floor, the strongest structural position.

### 13c. What Vision 1 Loses

1. **Compactness in depth.** The 300mm depth is 50mm deeper than the zone layout's 250mm. Under most sinks this is free, but some tight installations may prefer the shallower box.
2. **Tube routing length.** Front-to-back floor runs are longer than the zone layout's short vertical drops. Dead volume increases proportionally (~14ml vs ~7ml per line).
3. **Structural complexity.** A profiled cradle is harder to manufacture than a flat shelf. The 3D geometry requires more careful tolerance management.

### 13d. Worth Adopting from Zone Layout

1. **Horizontal electronics shelf.** Already adopted -- the electronics zone in the upper-rear is effectively a horizontal shelf.
2. **Dock wall fitting layout.** The 2x2 fitting grid, pogo pin placement, and alignment pin geometry transfers directly.
3. **Solenoid valve grouping.** Valves grouped in side banks near the dock area for short tube runs.

---

## Appendix: Derived Geometry Reference

### Bag Envelope at Key Depths (Back-Wall Mounted, 35 deg)

With back-wall mounting, the sealed end is at Y=292 (back wall), Z~392. The cap end is at approximately Y=25, Z~125. The bag height consumed is ~267mm.

The profiled cradle lower surface approximately follows:

    Z_cradle(Y) ≈ 392 - (292 - Y) * tan(35)
                = 392 - (292 - Y) * 0.700

| Depth Y (mm) | Z_cradle (approx) | Available height below cradle | Available height above bag upper surface |
|---------------|-------------------|-------------------------------|----------------------------------------|
| 0 (front wall) | 188 | 188 | 204 |
| 25 (cap end) | 125 | 125 | 267 |
| 50 | 223 | 223 | 169 |
| 100 | 258 | 258 | 134 |
| 130 (cart rear) | 279 | 279 | 113 |
| 150 | 293 | 293 | 99 |
| 200 | 328 | 328 | 64 |
| 250 | 363 | 363 | 29 |
| 292 (back wall) | 392 | 392 | 0 |

Note: These values use the simplified linear model. The actual bag has a lens profile, so the cradle height at any given Y is approximate. The sealed end at the back wall is flat film pinned directly to the wall at Z~392. The "available height above" column shows where electronics and hopper can occupy space.

### Front-Bottom Void

The entire floor area from Z=0 to Z~125 (at the cap end, Y~25) is available for the cartridge, valves, and tube routing. The void is very generous at the front and tapers as depth increases (because the cradle height rises toward the back wall). At Y=130 (cartridge rear face), the cradle is at Z~279, providing 279mm of clearance above the floor -- far more than the 80mm cartridge needs.
