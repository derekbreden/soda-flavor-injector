# Enclosure Width Reduction: 280mm to 220mm Feasibility Study

**Synthesized:** 2026-03-24
**Sources:** v1-master-spatial-layout.md, valve-architecture.md, 2l-bags-at-300mm-depth.md, requirements.md, cartridge-envelope.md (archive), pump_tray.py, display-and-front-panel.md

The companion carbonated water machine (Lilium) is 220mm wide. The current enclosure is 280mm wide. This document investigates whether the enclosure can shrink to 220mm exterior width to match the Lilium, or whether 240mm is a viable fallback.

**Bottom line up front:** 220mm exterior width (212mm interior at 4mm walls) is feasible. No hard blockers exist. The bags fit with 11mm per side. The cartridge narrows to ~148mm (already designed at this width). The 8 solenoid valves must relocate from side banks into the depth dimension -- behind and above the cartridge is the strongest option, using the large front-bottom triangular void that extends 267mm deep and 200mm tall. Electronics, hopper, displays, and back panel all fit. **220mm is recommended as the target width, with high confidence.**

---

## 1. Component Width Inventory

Every component that consumes width, with actual dimensions from existing research.

| Component | Width (mm) | Source | Notes |
|-----------|-----------|--------|-------|
| 2L Platypus bag | 190 | bag-dimensions-survey.md | Flat flexible pouch, 350mm x 190mm |
| Bag cradle (profiled) | ~200 | 2l-bags-at-300mm-depth.md sec 4d | 190mm bag + 5mm margin per side |
| Kamoer KPHM400 pump (single) | 68.6 | Kamoer product page, pump_tray.py | 115.6L x 68.6W x 62.7H mm |
| Two pumps side-by-side | 142.2 | pump_tray.py | 2 x 68.6 + 5mm inter-pump gap |
| Cartridge (current spec) | 150 | v1-master-spatial-layout.md | 150W x 130D x 80H mm exterior |
| Cartridge (calculated envelope) | 148 | pump_tray.py, cartridge-envelope.md | 2 x 68.6 + 5mm gap + 6mm walls |
| Solenoid valve (NC, RO style) | 30-35 dia | valve-architecture.md sec 5a | Cylindrical body, 50-55mm long |
| L298N motor driver PCB | ~43 | v1-master-spatial-layout.md | 43 x 43 x 27mm |
| ESP32-S3 dev board | ~50 x 25 | v1-master-spatial-layout.md sec 10 | Typical ESP32-S3-DevKitC |
| MCP23017 breakout | ~25 x 25 | Standard Adafruit/generic board | DIP-28 or breakout |
| Hopper funnel | 100 dia | v1-master-spatial-layout.md | 100mm diameter opening |
| Display reel (single) | ~80 dia | display-and-front-panel.md sec 1.1 | 80mm dia x 25mm deep spool |
| Two display reels | 160 (side-by-side) or 80 (stacked) | display-and-front-panel.md | Side-by-side or vertical stack |
| IEC C14 power inlet | ~48 x 28 | Standard IEC dimensions | Panel-mount cutout |
| 1/4" push-fit fitting | ~25 dia body | Standard RO fitting | 5 fittings on back panel |
| Flow meter (inline) | ~30 dia | Typical 1/4" inline flow sensor | Mounted on back panel or floor |

---

## 2. Width Budget at 220mm

### 2a. Interior Space

| Wall Thickness | Exterior | Interior | Per-Side Margin (around 190mm bag) |
|---------------|----------|----------|------------------------------------|
| 4mm | 220mm | 212mm | 11.0mm |
| 3mm | 220mm | 214mm | 12.0mm |
| 3.5mm (compromise) | 220mm | 213mm | 11.5mm |

At 4mm walls: **212mm interior, 11mm per side around the 190mm bags.**

### 2b. Bags: FITS

The 190mm-wide bags sit centered in 212mm interior. The 200mm cradle (190mm + 5mm margin per side) fits with 6mm clearance per side to the enclosure wall. This is tight but adequate:

- The cradle does not need significant wall structure beyond the bag. A 2-3mm PETG lip on each side of the cradle channel is sufficient to keep the bags from sliding laterally.
- Tube routing along the sides is constrained but possible. The dip tube exits from the bag connector at the front-low cap end (centered, not at the side), so tubes route downward from center, not outward to the sides.
- The 6mm between cradle edge and enclosure wall accommodates the cradle mounting brackets (1-2mm standoff screws or snap-fit tabs into wall ribs).

**Verdict: 190mm bags in 212mm interior work. Not luxurious, but no interference.**

### 2c. Cartridge: FITS

The cartridge envelope from pump_tray.py is already 148mm wide (2 x 68.6mm pumps + 5mm gap + 6mm shell walls). The current spec of 150mm is rounded up. At 212mm interior:

- Cartridge width: 148mm (or 150mm with rounding)
- Side clearance: (212 - 148) / 2 = **32mm per side** (with 148mm cartridge)
- Side clearance: (212 - 150) / 2 = **31mm per side** (with 150mm cartridge)

This is ample. The cartridge fits easily. The guide rails and dock structure need only a few mm per side.

**But here is the critical change:** At 280mm width, the current layout places 4 valves in each 60mm side bank flanking the cartridge (X=0-60 and X=212-272). At 220mm width with 31mm per side, there is NOT enough room for solenoid valves beside the cartridge. A valve is 30-35mm diameter and needs ~40mm of width including tube quick-connects. 31mm is too narrow.

**The side-bank valve layout is eliminated at 220mm width. Valves must relocate.**

### 2d. Solenoid Valves: MUST RELOCATE (see Section 3)

Each valve is approximately 30-35mm diameter x 50-55mm long (cylindrical body with 1/4" quick-connect on each end). With quick-connect fittings, the total end-to-end length is approximately 70-80mm.

At 280mm width, the side banks had 60mm of width each -- enough for one valve with its fittings oriented along the depth axis. At 220mm with 31mm per side, this is impossible.

The valves must move into the depth dimension. Section 3 explores placement options.

### 2e. Electronics: FITS

The electronics zone is in the upper-rear, above the bag slab. Current allocation: X=41-231 (190mm wide). At 220mm/212mm interior, the electronics span X=11-201 (190mm centered) or use the full 212mm.

Component widths:
- ESP32-S3 dev board: ~50 x 25mm -- fits easily
- L298N motor driver: ~43 x 43mm -- fits easily (or use smaller DRV8871 boards at ~25 x 20mm)
- MCP23017 breakout: ~25 x 25mm -- fits easily
- 8x MOSFET driver circuits: can be on a single custom PCB ~100 x 30mm, or individual boards
- PSU (12V brick or board): ~80 x 50mm typical for 36W -- fits
- Fuse holder: ~30 x 15mm -- fits

Total electronics width needed: all components can be arranged in 190mm or less. The electronics shelf shrinks from 190mm to about 190mm (or slightly less) -- no change needed.

**Verdict: Electronics fit at 212mm interior with no issues.**

### 2f. Hopper Funnel: FITS

The hopper funnel is 100mm diameter, centered. At 212mm interior, there is 56mm of clearance per side. No issue.

**Verdict: Hopper fits with generous margin.**

### 2g. Display Reels: FITS (with stacking)

At 272mm interior (280mm enclosure), two 80mm reels fit side-by-side (160mm total) with room to spare. At 212mm interior, two reels side-by-side still fit (160mm < 212mm) with 26mm per side. However, this leaves only 26mm between each reel and the wall, which may be tight for the spring mechanism housing.

Alternative: stack the two reels vertically (80mm wide, 50mm deep). This uses less width (80mm vs 160mm) at the cost of more depth and height.

**Verdict: Display reels fit at 212mm. Side-by-side is tight but workable. Vertical stacking is a comfortable alternative.**

### 2h. Back Panel: FITS

Back panel connections (all at Y=288-292):
- 3x 1/4" push-fit water fittings: ~25mm body each, spaced at ~40mm centers = ~120mm span
- IEC C14 power inlet: ~48mm wide
- 2x flavor tube exits (1/4" push-fit or PG7 gland): ~25mm each = 50mm span

Total back panel width needed: ~120 + 20mm gap + 48 + 20mm gap + 50 = ~258mm at the most generous spacing. This exceeds 212mm.

**However**, the fittings do not all need to be on a single horizontal row. With vertical stacking:
- Lower row: 3 water fittings + 2 flavor exits = 5 fittings at ~30mm pitch = 150mm span. Fits in 212mm.
- Upper row: IEC C14 (48mm). Fits in 212mm.

Or:
- Lower row: 5 push-fit fittings at 40mm pitch = 200mm. Fits in 212mm with 6mm per side.
- Upper area: IEC C14. Fits.

**Verdict: Back panel fits at 212mm with two-row arrangement. Single-row is tight but possible with 40mm pitch.**

---

## 3. Valve Relocation Options

The 8 solenoid valves (each ~35mm dia x 55mm body, ~75mm with quick-connects) must move from the side banks into the depth or height dimensions. The front-bottom triangular void is the primary candidate space.

### Front-Bottom Void Geometry Recap

At 35-degree bag angle with back-wall mounting:
- Bag cap end at approximately Y=25, Z=125
- Floor to bag underside at the front wall: ~200mm of height
- Depth of void: extends from front wall (Y=0) to approximately Y=267 where bags reach the floor zone
- Width: full interior (212mm at 220mm enclosure)

The cartridge occupies Y=0-130, Z=0-80, centered in width. That leaves significant space above and behind the cartridge within the triangular void.

### Option A: Valve Rack Behind the Cartridge (RECOMMENDED)

Place all 8 valves in a rack directly behind the cartridge dock back wall, using depth.

**Layout:** Valves oriented with their cylindrical axis along the width (X) direction, stacked in a 4-wide x 2-high grid. Each valve is ~35mm dia x 75mm long (with QC fittings).

```
    TOP VIEW at Z = 30mm (valve center height, row 1)

    Y(mm)
    292 +------------------------------------------+
        |                                          |
        |   (bags overhead on diagonal)            |
        |                                          |
    240 |                                          |
        |                                          |
        |==========================================| <- valve rack
    205 | V1  V2  V3  V4                           | <- row of 4 valves
        | (35mm dia each, side by side along X)    |
        |==========================================|
    165 |                                          |
        |========= DOCK BACK WALL ================|
    130 |                                          |
        | +------CARTRIDGE (148mm wide)------+     |
        | |  Pump 1  |  gap  |  Pump 2      |     |
        | +----------------------------------+     |
      0 +------------------------------------------+
        0         53     106      159      212
                        X (width, mm)
```

**Dimensions check:**
- 4 valves at 35mm diameter + 5mm spacing each = 4 x 40mm = 160mm width. Fits in 212mm (26mm margin per side, or offset to one side).
- Alternatively, 4 valves at 35mm + 2mm spacing = 148mm. Centered at X=32-180.
- Valve body length (along Y axis): 55mm body + 20mm fittings = 75mm per valve
- Two rows of 4: Row 1 at Y=165-240, Row 2 at Y=165-240 but stacked at Z=0-35 and Z=40-75.
- Or: single file of 8, two rows of 4 in the Z direction.

**Revised layout (2 rows of 4, stacked vertically):**

Valves oriented with cylindrical axis along Y (depth), stacked in width:

- Row 1 (Z=0-35): V1, V2, V3, V4 at Y=165-240, spanning X=6-53, X=53-100, X=100-147, X=147-194
- Row 2 (Z=40-75): V5, V6, V7, V8 at same Y range, same X positions

Each valve: 35mm dia, oriented along Y. Four valves across 212mm = 4 x 35mm + 3 x 15mm spacing = 185mm. Fits with 13.5mm margin per side.

**Depth consumed:** Y=165 to Y=240 (75mm, one valve length with fittings). Behind the dock back wall (Y=130-165), the valve rack starts at Y=165. Total depth from front wall: 240mm, well within the 292mm interior. The bag cradle at Y=240 is at Z~356 -- the valves at Z=0-75 are far below the bags.

**Height consumed:** Z=0-75 (two rows of valves). The cartridge is Z=0-80. The valves are at the same height as the cartridge but behind it. No conflict.

**Clearance to bags:** At Y=240, the bag cradle lower surface is at Z = 392 - (292-240) x 0.700 = 392 - 36 = 356mm. Valves top out at Z=75. Gap: **281mm**. No issue.

**Tube routing:** All valves are directly behind the cartridge dock. Tubes from valve quick-connects route forward through the dock back wall to cartridge fittings (short runs, ~35mm). Tubes from valves to bags route upward and rearward to the bag cap end (Y=25, Z=125 -- but wait, the bags are above, so tubes route from Y=165-240 up to the cap end at the front). This requires some routing thought but is fundamentally simple: tubes run along the floor from the valve rack forward to the bag connector zone.

**Verdict: Option A works well. 8 valves fit in a 185mm x 75mm x 75mm block behind the dock. Generous clearance to bags above.**

### Option B: Valves Above the Cartridge, Below the Bag Diagonal

Place valves in the height gap between the cartridge top (Z=80) and the bag underside.

At Y=50 (front area), the bag cradle underside is at approximately Z = 392 - (292-50) x 0.700 = 392 - 169 = 223mm. So the gap between cartridge top (Z=80) and bag underside (Z=223) is **143mm**.

A 2x4 valve array stacked vertically: 2 valves high at 40mm each = 80mm. Fits in 143mm.

**Layout:** 4 valves across in width, 2 valves high. Sitting on top of the cartridge zone (Z=84-164). Width: 4 x 35 + 3 x 15 = 185mm. Fits in 212mm.

**Pros:** Short tube runs to cartridge below. Valves are near the front for serviceability.

**Cons:** Valves sit directly above the cartridge. During cartridge removal, drips from valve connections could fall into the open cartridge slot. Also, this zone is where the cartridge lever/handle operates (Z=80-124 per the master layout). The lever needs clearance.

**Verdict: Option B works geometrically but has ergonomic and drip concerns. Less clean than Option A.**

### Option C: Valves Mounted on Back Wall, Low Section

Mount valves vertically on the back wall interior, below where the bags start (Z=0 to Z~125).

The back wall is at Y=288-292. The back panel components occupy the exterior surface. On the interior side, the valve bodies could mount with their cylindrical axis vertical (Z direction), fittings pointing up and down.

At the back wall, the bag sealed end is pinned at Z~392. Moving down, the bag is flat film for the first ~50mm of length, so it barely protrudes from the wall. At Z=0-75 on the back wall, the bags are far above.

**Layout:** 8 valves in a single row along the width, mounted to the back wall at Y~270-288 (valve body depth ~18mm from wall). Z=0-75 (valve body + fittings).

8 valves at 35mm + 2mm gap each = 296mm. **Does NOT fit in 212mm.**

Alternative: 2 rows of 4 stacked vertically on the back wall.
- Row 1: 4 valves at Z=0-55 (body length). Width: 4 x 37mm = 148mm. Fits.
- Row 2: 4 valves at Z=60-115. Same width. Fits.

**Depth consumed:** Valve body diameter ~35mm projects ~35mm from back wall into the interior. Y=257-292.

**Pros:** Valves are out of the way, near the back panel for clean routing to back panel exits.

**Cons:** Farthest from cartridge -- tube runs are long (~250mm from back wall to dock). Difficult to service. May interfere with tube channels on the floor.

**Verdict: Option C works but has long tube runs and poor serviceability. Not recommended.**

### Option D: Valves in a Floor-Mounted Row, Centered

Mount all 8 valves in a row along the floor, behind the cartridge, with cylindrical axis along the depth (Y) direction.

**Layout:** Single row of 8 along the width. 8 x 35mm = 280mm. Does NOT fit in 212mm.

Two rows of 4: 4 x 37mm = 148mm wide, two rows deep. Y=165-240 (row 1) and Y=240-315 (row 2 -- exceeds 292mm interior). Does not work as two rows in depth.

Alternative two-row in height: Same as Option A. Already covered.

**Verdict: Same as Option A when arranged in 2 rows by height.**

### Summary of Valve Placement Options

| Option | Fits? | Tube Routing | Serviceability | Drip Risk | Recommendation |
|--------|-------|-------------|----------------|-----------|----------------|
| A: Behind cartridge, 2 rows by height | Yes | Good, short runs to dock | Good, accessible when cartridge removed | Low | **RECOMMENDED** |
| B: Above cartridge, below bags | Yes | Very short | Moderate, lever clearance issue | Moderate (above open slot) | Viable fallback |
| C: Back wall, low section | Yes (2x4) | Poor, long runs | Poor, hard to reach | Low | Not recommended |
| D: Floor-mounted row | Same as A | Same as A | Same as A | Same as A | Redundant with A |

---

## 4. Hard Blocker Analysis

| Component | Fits at 220mm? | Blocker? | Notes |
|-----------|---------------|----------|-------|
| 2L Platypus bags (190mm) | Yes, 11mm/side (4mm walls) | **No** | Tight but sufficient for cradle and mounting |
| Cartridge (148mm) | Yes, 32mm/side | **No** | Generous clearance |
| 8 solenoid valves | Yes, if relocated behind cartridge | **No** | Side banks eliminated; depth placement works |
| Electronics (ESP32, drivers, PSU) | Yes | **No** | All components under 100mm wide individually |
| Hopper funnel (100mm dia) | Yes, 56mm/side | **No** | No width constraint |
| Display reels (2x 80mm) | Yes | **No** | Side-by-side (160mm) or stacked (80mm) |
| Back panel connections | Yes, with 2-row layout | **No** | Single row is very tight; two rows preferred |
| Drip tray | Yes (212mm wide) | **No** | Scales to interior width |
| Bag cradle mounting | Yes | **No** | 6mm wall clearance sufficient for brackets |
| Tube routing (side clearance) | Yes | **No** | Tubes route from center (bag cap) downward, not along sides |

**No hard blockers identified at 220mm.**

The tightest constraints are:
1. Bag-to-wall clearance: 11mm per side (4mm walls) or 12mm (3mm walls). Sufficient but leaves no room for error in bag positioning. The profiled cradle must precisely center the bags.
2. Valve relocation: mandatory. The entire valve subsystem architecture changes from side-bank to behind-cartridge placement.
3. Back panel fitting layout: must use two rows instead of one. Still feasible.

---

## 5. 240mm Fallback Evaluation

If 220mm proves too tight during prototyping, 240mm exterior (232mm interior at 4mm walls) provides meaningful relief.

| Parameter | 220mm (212mm int.) | 240mm (232mm int.) | Difference |
|-----------|--------------------|--------------------|------------|
| Bag side clearance | 11mm | 21mm | +10mm per side |
| Cartridge side clearance | 32mm | 42mm | +10mm per side |
| Valve side-bank width | 31mm (too narrow) | 41mm (borderline) | +10mm |
| Back panel single-row | Very tight | Comfortable | Relaxed |
| Display reels side-by-side | 26mm margin/side | 36mm margin/side | +10mm |

At 240mm, the side-bank valve layout becomes borderline possible again. Each side bank would have 41mm -- just enough for a valve (35mm dia) with 3mm clearance per side, IF the valves are oriented with their cylindrical axis along the depth direction and the quick-connect fittings extend in Y, not in X.

However, this is very tight. A single valve body at 35mm in a 41mm bank leaves only 3mm per side for tube routing and mounting brackets. This is fragile from a manufacturing tolerance perspective.

**Recommendation at 240mm:** Still use the behind-cartridge valve placement (Option A). The extra 20mm of width is better spent on bag clearance and back panel layout comfort than on cramming valves back into side banks.

240mm solves no problems that 220mm cannot solve. The constraints at 220mm are all manageable. 240mm is a safety margin, not a requirement.

---

## 6. ASCII Cross-Sections for 220mm Layout (Option A Valves)

### 6a. Side View: Height (Z) vs. Depth (Y)

Width is into the page. Identical to the 280mm layout -- width reduction does not affect the side view.

```
Z(mm)
392 +-----------------------------------------------------+
    |                                   [sealed end pinned |
    |[HOPPER]  [ELECTRONICS]             flat to back wall]|
    |funnel+    ESP32,drivers  \\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |board     above bags       \\\\\\\\\\\\\\\\\\\\\\\\\\=|
    |                      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
267 |                 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |            \\\\\\\\ bag slab (2x 2L) \\\\\\\\\\\\\\  |
    |         \\\\\\\\ lens profile, 80mm peak \\\\\\\\\\  |
    |      \\\\\\\\\\\\  on profiled cradle  \\\\\\\\\\\\  |
    |   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
125 | cap end \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    | (front-low)                                          |
    |              [VALVE RACK]                             |
 84 |              8x solenoids                             |
    | +----------+ behind dock                              |
    | |CARTRIDGE |+--------+                                |
    | |pumps only||VALVES  |=======tubes along floor=======|
    | |148x130x80||4W 2H   |               [BACK PANEL:   |
    | +----------++--------+                water,soda,pwr]|
  0 +-----------------------------------------------------+
    0  (front)              165  240             292 (back)
                         Y (depth, mm)
```

### 6b. Front View: Height (Z) vs. Width (X)

Viewing from the front. Depth is into the page.

```
Z(mm)
392 +---------------------------------------+
    |       +------------------+            |
    |       |  HOPPER FUNNEL   |            |
322 |       |  (100mm dia)     |            |
    |       +--------+---------+            |
    |                | tubing               |
    |                |                      |
    |    bag slab extends into page         |
    |    (190mm wide, centered)             |
    |    on profiled cradle (200mm)         |
    |  6mm|<-- 200mm cradle -->|6mm         |
    |                                       |
    |                                       |
    |                                       |
 84 |                                       |
    | +---------CARTRIDGE (148mm)--------+  |
    | |  Pump 1  |  gap  |  Pump 2      |  |
    | +----------------------------------+  |
  0 +--+----------------------------------+-+
    0  6           106                 206  212
                  X (width, mm)
```

Note: Valves are behind the cartridge in depth (Y=165-240), not visible in the front view.

### 6c. Top View: Width (X) vs. Depth (Y)

Viewing from above. Height is into the page.

```
Y(mm)
292 +---------------------------------------+
    |      [BACK PANEL: water, soda,        |
    |       power, flavor tubes]            |
    |       (2-row fitting layout)          |
    |                                       |
    |    bag slab (190mm wide, centered)    |
    |    extends diagonally into page       |
    |    from Y~292 (sealed, high Z)        |
    |    to Y~25 (cap end, low Z)           |
    |                                       |
240 |==== tubes along floor ================|
    | +----------------------------------+  |
    | |  V1   V2   V3   V4  (Z=0-35)    |  | <- valve rack row 1
    | |  V5   V6   V7   V8  (Z=40-75)   |  | <- valve rack row 2
    | +----------------------------------+  |
165 |====== DOCK BACK WALL ================|
130 |                                       |
    | +----------------------------------+  |
    | |         CARTRIDGE                |  |
    | |      (148W x 130D x 80H)        |  |
    | |    2x Kamoer pumps only          |  |
    | +----------------------------------+  |
  0 +---------------------------------------+
    0     32                    180       212
                  X (width, mm)
```

### 6d. Valve Rack Detail: Front View at Y=200 (cross-section through valve rack)

```
Z(mm)
 75 +---------------------------------------+
    | +--+--+--+--+                         |
    | |V5|V6|V7|V8|  <- row 2 (Z=40-75)    |
 40 | +--+--+--+--+                         |
 35 | +--+--+--+--+                         |
    | |V1|V2|V3|V4|  <- row 1 (Z=0-35)     |
  0 | +--+--+--+--+                         |
    +---------------------------------------+
    0  14  51  88  125  162                 212
                  X (width, mm)

    Each valve: ~35mm dia circle (shown as square in ASCII)
    4 valves + 3 gaps of 2mm = 146mm
    Centered at X = 33-179
```

---

## 7. Cartridge Width at 220mm

The cartridge itself does not need to change. The calculated envelope is already 148mm wide (pump_tray.py), and the enclosure provides 32mm of clearance per side even at 212mm interior width.

If narrowing the cartridge further is desired (for aesthetic proportion or to provide more clearance), the minimum cartridge width is constrained by the two pumps:

| Configuration | Cartridge Width | Notes |
|--------------|----------------|-------|
| Current (2 x 68.6mm + 5mm gap + 6mm walls) | 148mm | Existing design |
| Tighter (2 x 68.6mm + 3mm gap + 4mm walls) | 144mm | Minimum viable |
| Absolute minimum (2 x 68.6mm + 0mm gap + 3mm walls) | 143mm | No inter-pump gap, very thin walls |

The cartridge width is not a constraint at 220mm enclosure width.

---

## 8. Impact on Existing Design

### 8a. What Changes

1. **Valve placement:** From side banks (X=0-60 and X=212-272) to behind-cartridge rack (Y=165-240, Z=0-75).
2. **Tube routing:** Valves are now behind the dock instead of beside it. Tube runs from valves to dock fittings are ~35mm (Y direction through dock wall) instead of ~80mm (X direction across floor). Shorter.
3. **Tube routing from valves to bags:** Tubes from bag-side valves (V2, V4, V6, V8) route from Y=165-240 forward to the bag cap end at Y~25, Z~125. This is a ~200mm run forward and upward. Comparable to the current layout.
4. **Tube routing from valves to hopper:** Tubes from hopper-side valves (V1, V5) route upward from Z=0-75 to the hopper at Z=322. Vertical run of ~250mm. Same as current layout.
5. **Tube routing to back panel:** Dispense line valves (V3, V7) at Y=165-240 route directly back to the back panel at Y=292. Only ~50-75mm. Shorter than the current 150mm.
6. **Back panel:** Two-row fitting arrangement instead of single-row.
7. **Electronics zone:** Slightly narrower (212mm vs 272mm interior) but still adequate.
8. **Display reels:** May need to stack vertically if side-by-side is too tight with spring mechanism housings.
9. **Drip tray:** Narrower (212mm vs 272mm). No functional change.

### 8b. What Does NOT Change

1. Bag configuration (same bags, same angle, same cradle concept)
2. Cartridge design (same pumps, same envelope)
3. Hopper design (same funnel, same diameter)
4. Electronics (same components, same shelf concept)
5. Valve count and architecture (still 8 two-way NC)
6. Fluid topology (same schematic, same valve truth table)
7. Depth and height (still 300D x 400H exterior)
8. Access architecture (still front-loading cartridge, top-access hopper)

---

## 9. Recommendation

**Target 220mm exterior width. High confidence (8/10).**

The 2-point confidence deduction is for:
1. Bag-to-wall clearance (11mm per side) has not been physically prototyped. If the bags bulge laterally more than expected when full, the cradle may press against the walls. Mitigation: the profiled cradle constrains lateral bulge by design.
2. The valve rack behind the cartridge has not been prototyped in this configuration. Tube routing density in the Y=130-240 zone needs validation.

**Why 220mm and not 240mm:**
- 220mm matches the Lilium width exactly -- clean product line aesthetics.
- All components fit at 220mm. 240mm solves no hard problems that 220mm cannot.
- The extra 20mm of width at 240mm would be wasted clearance, not enabling any additional functionality.
- Going to 240mm "just in case" means the product is permanently 20mm wider for no demonstrated reason.

**Why not narrower than 220mm:**
- The 190mm bags in 212mm interior (11mm/side) is already at the practical minimum for a cradle-mounted flexible pouch. Below 210mm interior (~218mm exterior), manufacturing tolerances and bag fill variability become risky.
- There is no product-line reason to go below 220mm.

### 9a. Prototype Priorities for Width Validation

1. **Bag-in-cradle lateral fit test.** Print a 200mm cradle, mount two filled 2L bags, place inside a 212mm-wide test box. Verify the bags do not press against the walls.
2. **Valve rack mockup.** Print a rack that holds 4 valves in a row (148mm span) and verify tube quick-connects can be attached/detached in the Y=165-240 zone behind a cartridge dock wall.
3. **Back panel fitting layout.** Arrange 5 push-fit fittings + 1 IEC C14 in a 212mm-wide panel. Verify tool access for tube insertion.

### 9b. Updated Enclosure Dimensions

| Parameter | Current | Proposed |
|-----------|---------|----------|
| Exterior | 280W x 300D x 400H mm | **220W** x 300D x 400H mm |
| Interior (4mm walls) | 272W x 292D x 392H mm | **212W** x 292D x 392H mm |
| Interior volume | 31.1 liters | **24.3 liters** |
| Footprint | 840 cm2 | **660 cm2** (21% reduction) |

The width reduction from 280mm to 220mm saves 60mm of under-sink width and reduces the footprint by 21%. The enclosure matches the Lilium carbonated water machine width.

---

## 10. Open Questions

1. Does the profiled cradle need lateral wings (vertical walls on its sides) to prevent bag bulge, or is the U-channel sufficient? At 11mm per side, any lateral bag expansion beyond the cradle needs to be contained.
2. Can valve quick-connect fittings be accessed for servicing in the Y=165-240 zone when the cartridge is removed? The dock back wall at Y=130-165 partially obstructs direct front access.
3. Should the dock back wall have pass-through holes for valve tube routing, or should tubes route around the dock wall?
4. Is 3mm wall thickness preferable to 4mm for this narrower enclosure? The 2mm gain (214mm vs 212mm interior) provides 1mm more per side for bag clearance. 3mm with internal ribbing is structurally sound for this form factor.
5. Should the two display reels stack vertically (80mm wide x 50mm deep) instead of side-by-side (160mm wide x 25mm deep) to free width margin at the front panel?

---

## References

- [v1-master-spatial-layout.md](v1-master-spatial-layout.md) -- Current 280mm layout with coordinates
- [valve-architecture.md](valve-architecture.md) -- 8-valve NC architecture, valve dimensions
- [2l-bags-at-300mm-depth.md](2l-bags-at-300mm-depth.md) -- Bag geometry, cradle design, depth analysis
- [requirements.md](../requirements.md) -- Enclosure functional requirements
- [display-and-front-panel.md](display-and-front-panel.md) -- Display reel dimensions
- [cartridge-envelope.md](../../archive-assumes-zone-layout/cartridge-envelope.md) -- Pump dimensions (Kamoer KPHM400: 115.6 x 68.6 x 62.7mm)
- pump_tray.py -- Cartridge envelope calculation (148mm width)
