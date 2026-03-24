# Enclosure Layout & Spatial Planning — Definitive Design

This is the single reference document for the complete enclosure layout. It integrates findings from all enclosure research (hopper, front face, back panel, routing) and all cartridge research (mating face, release plate, cam lever, envelope, electrical, guides, pump mounting, ergonomics, dock mounting, workflow, release alternatives). The goal is a document with enough specificity to start CAD work.

**Design philosophy:** This should look like a product, not a science project. Dark navy PETG (#1a1a2e theme), consistent with the iOS app, S3 display UI, and app icon. Under a kitchen sink, it should read as a premium appliance — like a bean-to-cup coffee machine.

---

## 1. Confirmed Design Decisions

These are settled. Prior research explored alternatives; this section records only the winners and their key parameters.

| Decision | Choice | Source |
|----------|--------|--------|
| Form factor | Tall tower, vertical emphasis | layout-spatial-planning.md (original) scored highest |
| Layer order (top to bottom) | Electronics + hopper > dock + valves > bags | Gravity flow optimization, thermal isolation |
| Bag orientation | Hanging vertically, connector at bottom | hopper-and-bag-management.md Section 5 |
| Bag size | Smaller than 2L Platypus — 10-12" of vertical bag zone is sufficient | User confirmation |
| Cartridge loading | Front-loading, horizontal slide-in | front-face-interaction-design.md, under-cabinet-ergonomics.md |
| Release mechanism | Cartridge-side release plate + eccentric cam lever on cartridge front face | mating-face.md, release-mechanism-alternatives.md |
| Mating face port layout | 2x2 grid, 15mm center-to-center | mating-face.md — compact, best tilt resistance |
| Display arrangement | Stacked totem (RP2040 on top, S3 below) on front-left | front-face-interaction-design.md Layout F |
| Display retention | Magnetic pop-out holders (N52 neodymium) | front-face-interaction-design.md Section 1a |
| Display cable | Magnetic breakaway pogo connector + optional coiled extension | front-face-interaction-design.md Section 1b |
| Cartridge face | Flush with front panel, color-matched navy PETG | front-face-interaction-design.md Layout F |
| Hopper location | Top of enclosure, two funnels (one per flavor) | hopper-and-bag-management.md Section 1 |
| Hopper fill method | **Pump-assisted** (NOT gravity — user override) | User confirmation |
| Back panel connections | Water fittings at bottom, power at top, 90-degree elbows outside | back-panel-and-routing.md Section 1 |
| Capacitive sensing | FDC1004 (4-channel I2C) with copper tape electrodes | hopper-and-bag-management.md Section 4 |
| GPIO expansion | I2C expander (MCP23017) — routine task | User confirmation |
| Tubing | Mostly silicone; few transition points need hard tubing | User confirmation |
| John Guest fitting spacing | 15mm center-to-center confirmed adequate | User confirmation |

---

## 2. Revised Dimensions

The original research estimated 250W x 200D x 450H mm based on 2L Platypus bags requiring 14"+ of bag zone. With confirmed smaller bags (10-12" bag zone), pump-assisted hopper filling, and refined component stacking, the dimensions shrink.

### 2a. Component Stack (Side View, Top to Bottom)

| Zone | Contents | Height (mm) | Notes |
|------|----------|-------------|-------|
| **Top cap + hopper funnels** | 2x silicone funnels (75-100mm opening), lids, FDC1004 board | 60 | Funnels sit in recesses in the top panel |
| **Electronics shelf** | ESP32 on DIN rail breakout, L298N x3, DS3231 RTC, power distribution, FDC1004 | 50 | Drip shield below this shelf |
| **Drip barrier** | Solid horizontal shelf, sloped to front drain indicator | 5 | Separates dry electronics from wet plumbing |
| **Dock + valve zone** | Cartridge dock (~130mm deep), 4x solenoid valves, 2x hopper solenoids, needle valve, tees, flow meter | 130 | Dock opening on front face; valves and plumbing behind/beside dock |
| **Bag zone** | 2x bags hanging vertically, connector at bottom | 280 (11") | 10-12" confirmed sufficient; using 280mm (11") |
| **Drip tray + feet** | Removable drip tray, 4x rubber feet | 25 | Catches any bag leaks; elevates enclosure off cabinet floor |
| **Total** | | **550** | **~21.7" (was 450mm / 18" in original)** |

Wait — 550mm is taller than the original 450mm estimate. The original had bags ABOVE the dock and electronics BELOW. Let me re-examine the layer order.

### 2b. Revised Layer Order: Electronics Top, Dock Middle, Bags Bottom

The gravity-optimal layout puts bags above pumps. But the original tall tower placed bags in the upper portion and electronics at the bottom. The routing research (back-panel-and-routing.md Section 3a) established the preferred layout as electronics top, dock middle, bags bottom — which contradicts gravity optimization.

**Resolution:** Bags below the dock still drain downward through tubing to the dock fittings. The key gravity requirement is that the bag connector (at the bottom of the hanging bag) is connected to the pump inlet via tubing, and the bag is higher than the pump inlet so gravity pre-primes the line. With bags in the bottom zone and the dock in the middle, the bags hang BELOW the dock — meaning the pump inlets (in the dock/cartridge) are ABOVE the bags. This means the pump must pull liquid upward, working against gravity.

**This is problematic.** The hopper research (Section 4b of the original layout doc) specifically concluded: "Bags should be above the pump cartridge. A minimum of 50-100mm of elevation difference provides meaningful gravity assist."

**Corrected layer order (matching gravity requirements):**

| Position | Zone | Rationale |
|----------|------|-----------|
| Top | Hopper funnels + electronics | Heat rises to vent slots; electronics furthest from water; hopper connects downward to plumbing |
| Upper-middle | Bags (hanging, connector at bottom) | Gravity pre-primes pump inlet lines |
| Lower-middle | Cartridge dock + solenoid valves + plumbing manifold | Pump inlets below bags; all valve plumbing concentrated here |
| Bottom | Drip tray + feet | Catches leaks |

This matches the original tall tower layout (Section 3a of the original doc) which scored highest.

### 2c. Final Component Stack

| Zone | Contents | Height (mm) | Notes |
|------|----------|-------------|-------|
| **Top cap + hopper** | 2x funnels in recesses, snap-on lids | 55 | Funnels 75-100mm opening, 50-75mm deep |
| **Electronics shelf** | ESP32/DIN rail, L298N x3, RTC, FDC1004, power block, vent slots | 50 | Mounted to rear wall; USB port accessible from back |
| **Drip barrier** | Solid shelf, sloped forward | 5 | Separates electronics from bag zone |
| **Bag zone** | 2x bags side-by-side (or front/back), hanging vertically | 280 | 11" bag zone; bags hang from hooks on underside of drip barrier |
| **Dock + plumbing zone** | Cartridge dock, solenoid valves x6, needle valve, tees x5+, flow meter, hopper solenoids x2 | 130 | Dock faces front; valves mount behind/beside dock |
| **Drip tray + feet** | Removable tray, rubber feet | 25 | |
| **Total height** | | **545 mm (21.5")** | |

This is taller than the original 450mm estimate. Under-sink cabinets have 500-710mm of usable height (under-cabinet-ergonomics.md). A 545mm enclosure fits, but tightly — the hopper funnels on top must be reachable, which requires ~100-150mm of clearance above the funnels for pouring.

**Problem:** 545mm enclosure + 100mm pouring clearance = 645mm minimum. The lower end of cabinet height (500mm) won't work. The upper end (710mm) has 165mm margin — fine.

**Optimization: reduce bag zone height.** The user confirmed 10-12" is realistic. Using 10" (254mm) instead of 11" (280mm) saves 26mm. Also, the hopper funnels can be shallower (40mm instead of 55mm) since pump-assisted filling means the funnel is just a pour target, not a gravity reservoir.

### 2d. Optimized Dimensions

| Zone | Height (mm) |
|------|-------------|
| Top cap + hopper funnels | 45 |
| Electronics shelf | 45 |
| Drip barrier | 5 |
| Bag zone | 255 (10") |
| Dock + plumbing zone | 130 |
| Drip tray + feet | 25 |
| **Total height** | **505 mm (19.9")** |

**Width:** The cartridge dock is ~180mm wide (cartridge 140mm + dock walls). Two bags side by side when full are each ~60mm thick — too wide. Two bags front-to-back use ~190mm width (one bag width) and ~120mm depth. The electronics shelf needs ~110mm width (DIN rail). The front face needs ~230mm for the display totem + cartridge slot side by side (front-face-interaction-design.md Layout C/F).

**Target width: 240 mm (9.4")**

**Depth:** The cartridge dock is ~130mm deep. Bags behind the dock would extend the depth significantly. Bags beside the dock (in the zone above it) don't affect dock depth. The back panel needs 60mm clearance behind it for 90-degree elbows and tube bends.

**Target depth: 220 mm (8.7")** — enclosure body is 220mm, plus 60mm behind for tube clearance = 280mm total cabinet depth used.

### 2e. Final Enclosure Dimensions

| Dimension | Value | Imperial |
|-----------|-------|----------|
| **Width** | 240 mm | 9.4" |
| **Depth** | 220 mm | 8.7" |
| **Height** | 505 mm | 19.9" |
| **Internal volume** | ~26.7 L | |
| **Footprint** | 528 cm^2 | 82 in^2 |
| **Clearance behind** | 60 mm | 2.4" |
| **Clearance above** | 100 mm minimum | 4" (for pouring) |
| **Total cabinet space** | 240W x 280D x 605H mm | 9.4" x 11" x 23.8" |

This fits in most under-sink side zones (10-14" wide per the ergonomics research) and requires ~24" of vertical space (available in cabinets with 28-30" usable height, which is standard).

---

## 3. Zone-by-Zone Design

### 3a. Top Zone: Hopper Funnels + Electronics (0-95mm from top)

```
    TOP VIEW
    ┌──────────────────────────────────┐
    │  ┌────────┐      ┌────────┐     │
    │  │Funnel 1│      │Funnel 2│     │
    │  │ ~80mm  │      │ ~80mm  │     │
    │  └────────┘      └────────┘     │
    │                                  │
    │  Silicone lids (snap-on, hinged) │
    └──────────────────────────────────┘
              240mm wide

    SIDE CROSS-SECTION (top 95mm)
    ┌──────────────────────────────────────┐
    │  ╔══funnel══╗    ╔══funnel══╗        │  45mm: hopper cap
    │  ║  drain   ║    ║  drain   ║        │
    │  ╚════╤═════╝    ╚════╤═════╝        │
    │───────┤───────────────┤──────────────│
    │  [FDC1004]  ESP32/DIN  L298N x3      │  45mm: electronics shelf
    │  [RTC]      [fuse]     [MCP23017]    │
    │─────────────────────────────────────│
    │  DRIP BARRIER (solid shelf)          │  5mm
    └──────────────────────────────────────┘
```

**Hopper funnel details:**
- 2x food-grade silicone funnels, ~80mm opening diameter, ~40mm deep
- Funnel outlets connect to 1/4" OD tubing via barb fittings
- Each funnel has a snap-on silicone lid (hinged to prevent loss)
- Funnels sit in 3D-printed recesses with a raised moat (5mm deep, 10mm wide) around each to catch drips
- Spacing: ~120mm center-to-center (fits within 240mm width with margins)

**Electronics shelf:**
- ESP32 on DIN rail breakout (~110 x 90 x 35mm) mounts to the rear wall
- 3x L298N boards (43 x 43 x 27mm each) mount to side walls, heatsink tabs facing outward toward vent slots
- DS3231 RTC piggybacks on ESP32 (I2C bus, ~100mm wire)
- FDC1004 breakout board mounts near hopper funnel drain tubes (short electrode wire runs)
- MCP23017 I2C expander for GPIO expansion
- Power distribution: fused terminal block on DIN rail, 12V bus
- All wiring in printed channels along upper walls

**Vent slots:**
- Horizontal louvers on both side panels, in the upper 50mm
- Louvers angled downward (reject drips from above while allowing convection)
- Passive ventilation sufficient for ~2W sustained / 33W peak thermal load

**Hopper tubing routing:** Funnel drain tubes route downward through the drip barrier (sealed pass-throughs) into the bag zone, then continue down to the dock/plumbing zone where they connect to hopper solenoid valves. Total run: ~400mm per funnel.

### 3b. Bag Zone (95-350mm from top, 255mm tall)

```
    FRONT VIEW (bag zone)
    ┌──────────────────────────────────┐
    │                                  │
    │   ╔══════════╗  ╔══════════╗    │
    │   ║  BAG 1   ║  ║  BAG 2   ║    │
    │   ║ (hanging)║  ║ (hanging)║    │
    │   ║          ║  ║          ║    │
    │   ║  liquid  ║  ║  liquid  ║    │
    │   ║  pools   ║  ║  pools   ║    │
    │   ║  here ↓  ║  ║  here ↓  ║    │
    │   ╚════╤═════╝  ╚════╤═════╝    │
    │        │              │          │
    │     tubing          tubing       │
    │     to dock         to dock      │
    └──────────────────────────────────┘
```

**Bag mounting:**
- Bags hang from hooks/clips on the underside of the drip barrier shelf
- Each bag occupies roughly 190mm wide x 60mm deep (full) x 255mm tall
- Two bags side by side: 190mm total width (staggered or front-to-back if width is tight)
- With 240mm enclosure width, two bags side by side (each ~100mm wide at half-full) fit with margin
- Bags hang inverted (connector at bottom) for gravity drainage
- Inlet cap (Platypus Drink Tube Kit) rigidly secured — confirmed to work for reliable bag collapse

**Bag flattening:** Start with gravity alone (Phase 1). If the last 20% sputters, add elastic frame (Phase 2: two 3D-printed plates + elastic bands per bag).

**Bag-to-dock tubing:** Soft silicone tubing (~300mm per bag) routes from the bag connector downward to the tee fittings in the dock/plumbing zone. Transition to hard tube at the push-connect fitting junction.

**Access:** Bags are accessed by removing the top panel (which lifts off after removing 4-6 screws into heat-set inserts). The user unhooks the old bag, threads a new bag onto the tubing adapter, and rehooks it.

### 3c. Dock + Plumbing Zone (350-480mm from top, 130mm tall)

This is the most densely packed zone. It contains the cartridge dock, all solenoid valves, the needle valve, all tee fittings, the flow meter, and the hopper solenoid valves.

```
    FRONT VIEW (dock + plumbing zone)
    ┌──────────────────────────────────────────────┐
    │                                              │
    │  ○ RP2040     ╔══════════════════════════╗   │
    │  (40mm)       ║                          ║   │
    │               ║   CARTRIDGE FACE         ║   │
    │  ○ S3         ║   (flush, navy finish)   ║   │
    │  (55mm)       ║   lever ═══╗            ║   │
    │               ╚══════════════════════════╝   │
    │  ● power                    ● status LED     │
    └──────────────────────────────────────────────┘
                    240mm wide
```

**Cartridge dock:**
- Dock opening on front face: ~155 x 105mm (with chamfered entrance)
- Dock depth: ~130mm (matches cartridge depth of ~100mm + 30mm for fittings and clearance)
- 4x John Guest fittings in dock rear wall, 2x2 grid at 15mm center-to-center
- 3x pogo pins on dock top face (12V GND, Motor A, Motor B)
- Guide rails: printed PETG, 0.3-0.5mm clearance per side
- 2x tapered alignment pins (15-20 degree taper, 8-10mm base) for final ~1mm alignment

**Cam lever clearance:** The lever on the cartridge front face swings 180 degrees. When the cartridge is inserted and the lever is in the locked position, it folds flat against the cartridge face (within the slot opening). When unlocked, it swings to the side, protruding ~50-80mm. This is within the enclosure width and does not require clearance above the slot.

**Valve mounting (behind and beside dock):**

```
    TOP VIEW (dock + plumbing zone)
    ┌──────────────────────────────────────────────┐
    │                                              │
    │  [SV-D1] [SV-D2]   ┌──────────────────┐    │
    │  [SV-C1] [SV-C2]   │   DOCK CAVITY    │    │
    │  [SV-H1] [SV-H2]   │   (cartridge     │    │
    │  [NV]               │    slides in     │    │
    │  [FM]     [T1-T5]   │    from front)   │    │
    │  [T-INJECT]         └──────────────────┘    │
    │  [T-MERGE]                                   │
    │  [T-CLEAN]                                   │
    │                                              │
    └──────────────────────────────────────────────┘
      ▲ BACK                              FRONT ▲
```

- 6x solenoid valves (2 dispensing, 2 clean, 2 hopper): mount vertically on the rear/side walls of this zone, behind the dock
- Needle valve: inline with clean water supply
- 5x tee fittings + 2x merge/inject tees: clustered in the plumbing manifold area behind the dock
- Flow meter: inline with carbonated water, near the injection tee
- Hopper solenoid valves (2x): connect funnel drain tubes (from above) to the bag tee junctions

**Display holders:** The display totem (RP2040 on top, S3 below) mounts to the front panel to the left of the cartridge slot. Magnetic retention sockets are printed into the front panel. Internal wiring (magnetic pogo connector to ESP32 UART) routes through the left wall upward to the electronics shelf.

### 3d. Bottom Zone: Drip Tray + Feet (480-505mm from top, 25mm)

- Removable drip tray: ~220 x 200 x 15mm, slides out from the front
- 4x rubber feet (10mm tall) elevate the enclosure off the cabinet floor
- The tray catches any drips from bag leaks, cartridge swaps, or fitting seepage
- A visible drip indicator at the front edge shows if the tray has collected water

---

## 4. Pump-Assisted Hopper Integration

The original hopper research recommended gravity filling. The user overrides this: **pumping is necessary to keep air out of the bags.** This changes the hopper fluid path significantly.

### 4a. Why Gravity Fill Doesn't Work

The hopper research identified a fundamental problem: the bag has a single opening, and air/liquid counter-flow through a 4.5mm ID tube is slow and unreliable. Gravity fill through the shared tee produces 8-15 minute fill times and risks vapor locks. Pump-assisted filling actively moves concentrate into the bag and forces air out.

### 4b. Pump-Assisted Fill Fluid Path

The cartridge's peristaltic pumps are the only pumps in the system. To fill a bag, the pump must pull concentrate from the hopper and push it into the bag. The plumbing must support this.

**Key constraint:** The bag has one opening. The pump is between the bag and the dispensing point. The hopper connects at a tee junction near the bag.

**Revised plumbing per flavor line:**

```
                              ┌─── Bag (single opening, connector at bottom)
                              │
  Funnel → [SEN-A] → [SV-H] ─┤
                              │
  Tap Water → [NV] → [SV-C] ─┘
                              │
                        ┌─────┘
                        │
                 [SV-D] (dispensing solenoid)
                        │
                 [DOCK INLET]
                        │
                 ══════════════
                 ║ PUMP (in   ║
                 ║ cartridge) ║
                 ══════════════
                        │
                 [DOCK OUTLET]
                        │
                 → T-INJECT → dispensing point
```

**Fill mode (pump-assisted):**

The pump runs in reverse (pulling from its outlet side, pushing through its inlet side). The dispensing solenoid is OPEN so the pump can pull from the bag side. The hopper solenoid is OPEN to provide the concentrate source.

```
  Funnel → [SV-H OPEN] → TEE → [SV-D OPEN] → [PUMP REVERSED] → DOCK OUTLET → T-INJECT → dispensing point
                           │
                      Bag (fills via TEE as pump creates suction)
```

Actually, this has the same problem identified in the hopper research (Section 2, Option A/B) — the pump pulls from the tee, which has two paths: the hopper and the bag. Some concentrate goes through the pump and out the dispensing point (wasted), and some enters the bag.

**Better approach: pump runs forward, hopper gravity-feeds into the tee while pump draws from the bag side:**

No — the user explicitly stated gravity doesn't work. We need a dedicated fill approach.

**The practical pump-assisted approach:**

1. Hopper solenoid OPEN, dispensing solenoid CLOSED, clean solenoid CLOSED
2. Pump runs in REVERSE at low speed
3. Pump pulls from its outlet side (the dispensing point / T-INJECT side) — but dispensing solenoid is closed, so this path is blocked
4. Instead: pump pushes backward through its inlet → through the dock inlet fitting → through the dispensing solenoid (CLOSED — blocked)

This doesn't work either with a closed dispensing solenoid. Let me reconsider.

**Working approach:** Accept that some concentrate passes through the system during fill, or use a separate dedicated fill path.

The simplest working approach: during fill mode, the hopper solenoid opens and the dispensing solenoid opens. The pump runs FORWARD (normal direction: bag side → pump → dispensing point). The pump creates suction on the bag/tee side, which pulls concentrate from the hopper through the hopper solenoid into the tee. Most concentrate enters the bag (path of least resistance when the bag is collapsed/empty), but some gets pulled through the dispensing solenoid into the pump and out. This is a "flush and fill" approach.

**Alternatively — and this is the cleanest solution — add a small dedicated fill pump.** A second, smaller peristaltic pump (or even a small DC diaphragm pump) is dedicated to hopper filling. It sits in the enclosure (not the cartridge) and pumps concentrate from the hopper funnel directly into the bag through a dedicated fill tube that connects at the same tee junction.

However, the system's architecture uses the existing pump for everything. Adding a dedicated fill pump changes the BOM significantly.

**The approach the user has confirmed works:** FDC1004 capacitive sensing detects liquid vs air. The existing pump runs in the normal direction with the hopper solenoid open and dispensing solenoid open. Concentrate enters through the tee, fills the bag, and the pump draws a small amount through the dispensing line. The FDC1004 sensor on the hopper line detects when the funnel is empty. The pump stops. Some concentrate is lost out the dispensing point, but this is an acceptable tradeoff for reliable air-free filling.

### 4c. Hopper Tubing Routing in the Layered Layout

The hopper funnels are at the TOP of the enclosure (in the top cap zone). The hopper solenoid valves are in the DOCK/PLUMBING zone (middle). The tubing must route from top to middle.

**Route:**
1. Funnel outlet → short barb fitting → 1/4" tubing
2. Tubing passes through sealed grommet in the drip barrier shelf
3. Tubing runs vertically down through the bag zone (along the rear wall, secured with printed clips every 50mm)
4. Tubing passes through the bag zone without interference (bags hang from the front, tubing runs along the back)
5. Tubing enters the dock/plumbing zone and connects to: FDC1004 copper tape electrode (SEN-A) → hopper solenoid valve (SV-H) → tee junction (T1 or T2)

**Total hopper tube run per flavor:** ~400mm (top to middle of enclosure)

This routing works naturally in the layered layout. The tubing runs along the rear wall, physically separated from the bags and the cartridge dock.

### 4d. Revised Valve State Table

| Mode | Hopper Sol | Clean Sol | Disp Sol | Pump | Flow Path |
|------|-----------|-----------|----------|------|-----------|
| Idle | CLOSED | CLOSED | CLOSED | OFF | No flow |
| Dispensing | CLOSED | CLOSED | OPEN | FORWARD | Bag → tee → disp sol → pump → tap |
| Hopper Refill | OPEN | CLOSED | OPEN | FORWARD | Hopper → tee → bag (fills) + tee → disp sol → pump → tap (small loss) |
| Clean Fill | CLOSED | OPEN | CLOSED | OFF | Tap water → clean sol → tee → bag |
| Clean Flush | CLOSED | CLOSED | OPEN | FORWARD | Bag → tee → disp sol → pump → tap |
| Hopper Line Flush | OPEN | CLOSED | OPEN | FORWARD | Flushes stale concentrate from hopper tubing |

---

## 5. Cross-Section Diagrams

### 5a. Side View Cross-Section (Primary View)

```
    ← FRONT                                                  BACK →

    ┌──────────────────────────────────────────────────────────────┐
    │  ╔═══FUNNEL 1═══╗    ╔═══FUNNEL 2═══╗   [lid]  [lid]       │ 45mm
    │  ╚═══════╤═══════╝    ╚═══════╤═══════╝                     │ HOPPER
    │──────────┤────────────────────┤─────────────────────────────│
    │          │   [ESP32/DIN]  [L298N] [L298N] [L298N]  [12V]   │ 45mm
    │          │   [RTC] [FDC1004] [MCP23017]  [FUSE]            │ ELECTRONICS
    │══════════╪════════════════════╪══════════════════════════════│ 5mm DRIP BARRIER
    │          │hopper tube 1      │hopper tube 2                 │
    │  ╔═══╤══╪═══╗   ╔═══╤══╪═══╗                               │
    │  ║BAG│  │   ║   ║BAG│  │   ║    [hopper tubes route        │
    │  ║ 1 │  │   ║   ║ 2 │  │   ║     along rear wall]          │ 255mm
    │  ║   │  │   ║   ║   │  │   ║                                │ BAG ZONE
    │  ║   ↓  │   ║   ║   ↓  │   ║                                │
    │  ╚═══╤══╪═══╝   ╚═══╤══╪═══╝                               │
    │──────┤──┤────────────┤──┤───────────────────────────────────│
    │  [Display]  ╔════════════════╗   [SV-D1][SV-D2]             │
    │  [totem ]   ║   CARTRIDGE   ║   [SV-C1][SV-C2]             │ 130mm
    │  [RP2040]   ║   DOCK        ║   [SV-H1][SV-H2]             │ DOCK +
    │  [S3    ]   ║   ← slide in  ║   [NV] [FM]                  │ PLUMBING
    │  [      ]   ╚════════════════╝   [T1-T5] [T-MERGE]         │
    │  [LEDs  ]                        [T-INJECT] [T-CLEAN]      │
    │─────────────────────────────────────────────────────────────│
    │  [DRIP TRAY — removable, slides out front]                  │ 25mm
    │  ▓▓▓▓▓▓▓ rubber feet ▓▓▓▓▓▓▓                              │ FEET
    └──────────────────────────────────────────────────────────────┘
                               220mm deep
```

### 5b. Front View

```
    ┌──────────────────────────────────────────────┐
    │         [lid 1]        [lid 2]               │  Hopper lids visible on top
    │                                              │
    │  ═══ vent louvers (both sides) ═══           │  Side vents (not visible from front)
    │                                              │
    │                                              │  (electronics hidden behind top panel)
    │                                              │
    │         (bag zone — no front features)       │
    │                                              │
    │  ○ RP2040     ╔══════════════════════════╗   │
    │  (33mm vis)   ║                          ║   │
    │               ║   CARTRIDGE FACE         ║   │
    │  ○ S3         ║   (flush, navy,          ║   │  Dock + plumbing zone
    │  (48mm w/     ║    color-matched)        ║   │
    │   knob)       ║   ═══lever═══╗          ║   │
    │               ╚══════════════════════════╝   │
    │  ● power LED              ● status LED       │
    │                                              │
    │  [═══════ drip tray front edge ═══════]      │
    └──────────────────────────────────────────────┘
                    240mm wide
```

**Front face dimensions:**
- Display totem: ~48mm wide x ~93mm tall, left side, vertically centered on the dock zone
- Cartridge slot: ~155 x 105mm (with chamfer), right of center
- Total active front face area: dock zone height (130mm) plus some margin above/below
- Gap between display totem and cartridge slot: ~20mm

### 5c. Back Panel View (From Outside)

```
    ┌──────────────────────────────────────────────┐
    │                                              │
    │   [USB]                           [12V DC]   │  TOP ZONE (dry)
    │                                              │
    │                     [AIR SWITCH]             │  MID (pneumatic grommet)
    │                                              │
    │  ─────────────────────────────────────────── │  Drip dam ridge
    │                                              │
    │   [TAP IN]      [CARB IN]      [CARB OUT]   │  BOTTOM ZONE (wet)
    │    (blue)        (green)        (white)      │
    │    + 90° elbow   + 90° elbow   + 90° elbow  │
    │                                              │
    └──────────────────────────────────────────────┘
                    240mm wide

    Labels: embossed/debossed into panel surface (not stickers)
    Color rings: printed snap-on collars around bulkhead nuts
    Panel: removable, 4-6 M3 screws into heat-set inserts, 4mm thick
```

**Fitting spacing:** 40mm center-to-center between the three water fittings. Water zone spans ~80mm + margins at the bottom of the panel. Power and USB at the top, separated by the drip dam.

---

## 6. Internal Plumbing Summary

From the routing research (back-panel-and-routing.md Section 2c):

| Category | Count | Details |
|----------|-------|---------|
| Push-connect tees | 5 | T-CLEAN, T1, T2, T-MERGE, T-INJECT |
| Bulkhead fittings | 3 | TAP IN, CARB IN, CARB OUT |
| 90-degree elbows | 3 | External strain relief on back panel |
| Dock wall fittings | 4 | 2 inlets, 2 outlets |
| Solenoid valves | 6 | 2 dispensing, 2 clean, 2 hopper |
| Needle valve | 1 | Clean water flow restriction |
| Flow meter | 1 | Inline with carbonated water |
| **Total push-connect fittings** | **15** | Plus fittings integral to solenoid valves |
| **Total hard tubing** | **~1,200mm (4 ft)** | |
| **Total soft silicone** | **~1,500mm (5 ft)** | Including hopper runs (~800mm for 2 lines) |
| **Grand total tubing** | **~2,700mm (9 ft)** | |
| **Hard-to-soft transitions** | **~10** | Each uses ~30mm hard stub + zip tie |

Most tubing is silicone (user confirmed). Hard tubing is used only at push-connect fitting interfaces and short rigid runs between closely spaced fittings.

---

## 7. Weight Estimate

| Component | Weight (g) | Notes |
|-----------|-----------|-------|
| 2x Platypus bags (full) | 4,000 | ~2 kg each (water weight) |
| Cartridge (assembled, with pumps) | 940 | From cartridge-envelope.md |
| Dock housing | 500 | Estimated |
| 6x solenoid valves | 720 | 120g each |
| 3x L298N boards | 90 | 30g each |
| ESP32 + DIN rail breakout | 80 | |
| Flow meter | 50 | |
| Needle valve | 60 | |
| DS3231 RTC | 8 | |
| FDC1004 + MCP23017 boards | 20 | |
| 2x displays | 80 | S3 50g + RP2040 30g |
| 15x push-connect fittings | 150 | ~10g each estimated |
| Tubing (2.7m) | 80 | Silicone + hard tube |
| Wiring harness | 50 | |
| 12V power supply (if internal) | 100 | |
| **Enclosure structure (PETG)** | **800** | See Section 8 BOM |
| Drip tray | 50 | |
| Hopper funnels + lids | 60 | Silicone |
| Rubber feet, screws, magnets | 50 | |
| **Total (bags full)** | **~7,890 g (17.4 lbs)** | |
| **Total (bags empty)** | **~3,950 g (8.7 lbs)** | |

The center of gravity shifts as bags empty. With bags in the lower half of the enclosure and full of liquid, the CG is low and stable. A 240mm wide base is stable at this weight.

---

## 8. Enclosure Structure Bill of Materials

This covers the box itself — panels, fasteners, rails, vents. Not the electronics, plumbing, or cartridge.

| Part | Material | Qty | Dimensions (mm) | Est. Weight (g) | Notes |
|------|----------|-----|-----------------|-----------------|-------|
| **Front panel** | Navy PETG, 0.1mm layer height | 1 | 240 x 505 x 5 | 150 | Separate piece, sanded + clear coat. Display holder sockets and cartridge slot integrated. |
| **Back panel** | Navy PETG | 1 | 240 x 505 x 4 | 120 | Removable (6x M3 screws). Bulkhead fitting holes, barrel jack, grommet. |
| **Left side panel** | Navy PETG | 1 | 220 x 505 x 3 | 85 | Vent louvers in upper 50mm. Fixed (glued or screwed to frame). |
| **Right side panel** | Navy PETG | 1 | 220 x 505 x 3 | 85 | Mirror of left. |
| **Top panel** | Navy PETG | 1 | 240 x 220 x 4 | 55 | Removable (4x M3 screws). Funnel recesses and moats molded in. |
| **Bottom plate** | Navy PETG | 1 | 240 x 220 x 4 | 55 | Drip tray rails, rubber foot mounts. |
| **Drip barrier shelf** | PETG (any color, internal) | 1 | 230 x 210 x 5 | 60 | Sloped forward 3-5 degrees. Sealed hopper tube pass-throughs. |
| **Electronics mounting rails** | PETG | 2 | DIN rail + brackets, ~150mm | 20 | For ESP32 breakout, fuse holder, terminal blocks |
| **Cartridge dock structure** | PETG | 1 | ~180 x 130 x 130 | 120 | Guide rails, fitting mount wall, pogo pin mount, alignment pins |
| **Internal valve mounting bracket** | PETG | 1 | ~200 x 100 x 40 | 30 | Holds 6 solenoid valves vertically |
| **Display holder sockets (x2)** | PETG + embedded magnets | 2 | 55mm dia x 15mm, 40mm dia x 10mm | 15 | Magnetic retention rings. Printed into front panel or as inserts. |
| **Bag hooks (x2)** | PETG | 2 | ~30 x 20 x 15 | 5 | Mount to underside of drip barrier shelf |
| **Wire channels** | PETG | 4 | ~150mm lengths, U-channel with snap lids | 10 | Route wires along upper walls |
| **Drip tray** | PETG | 1 | 220 x 200 x 15 | 50 | Removable, slides on rails from front |
| **Heat-set inserts (M3)** | Brass | 20 | M3 x 4mm | 10 | For all screw-mounted panels |
| **M3 x 8mm socket head screws** | Stainless | 20 | — | 10 | Panel mounting |
| **6x3mm N52 neodymium disc magnets** | — | 10 | 6mm dia x 3mm | 5 | 4 for S3 holder, 3 for RP2040 holder, 3 spare |
| **Steel discs (magnet targets)** | — | 7 | 6mm dia x 1mm | 3 | In display adapter housings |
| **Rubber feet** | Neoprene or silicone | 4 | 20mm dia x 10mm | 8 | Self-adhesive |
| **PETG filament (total)** | Navy PETG (Atomic Filament or similar) | ~600g | — | — | Estimated print material for all structural parts |
| | | | | **Total: ~800g** | |

**Estimated PETG cost:** ~600g at $30/kg = ~$18 in filament
**Estimated hardware cost:** ~$15 (magnets, inserts, screws, feet)
**Total enclosure structure cost:** ~$33

---

## 9. Eliminated Alternatives (Brief)

These were explored in the original research and rejected. Recorded here for context.

| Layout | Why Eliminated |
|--------|---------------|
| **Wide/Short (400W x 220D x 250H)** | 400mm width doesn't fit most under-sink side zones (250-350mm) |
| **Cube (300 x 300 x 300)** | Internal routing too dense; 300mm wide is tight for dock + bags side by side |
| **L-Shape / Stepped** | Complex manufacturing, lateral tube runs, doesn't fit rectangular cabinet zones |
| **Deep Shelf (250W x 450D x 280H)** | Bags in rear are inaccessible; 450mm depth reaches too far into cabinet |
| **Front-Loading Tower with hopper door** | Front hopper adds mechanism complexity; top hopper is simpler and more natural |
| **Top-loading cartridge** | Contradicts front-access design goal; requires reaching over enclosure |
| **Gravity hopper fill** | Too slow (8-15 min), air counter-flow problems, vapor lock risk. User override: pump-assisted required. |
| **Dock-side release mechanism** | Chicken-and-egg problem: collets must release BEFORE tubes move, but dock ramp requires tube movement FIRST |
| **CPC quick-disconnect couplings** | $10-15 per coupling vs $1 for John Guest fittings; overkill for this swap frequency |

---

## 10. Open Items for CAD

1. **Measure actual pump mounting holes** — the KPHM400 bracket hole pattern must be confirmed from physical pumps before dock/cartridge CAD
2. **Bag size selection** — the Platypus bags don't need to be 2L. Confirm the specific bag size and measure actual dimensions when hung inverted
3. **Hopper fill pump path validation** — build the plumbing on the bench, test pump-assisted fill with all solenoid states, measure concentrate loss out the dispensing point
4. **Cabinet measurement** — measure the actual installation cabinet: zone width, depth, height (accounting for sink bowl, disposal, supply lines)
5. **Cardboard mockup** — build a 240 x 220 x 505mm box, place it in the cabinet, verify hopper reach, cartridge slot height, and back panel access
6. **FDC1004 electrode testing** — verify capacitance change through silicone tubing with sugar syrup vs air
7. **Release plate single-bore test** — 3D print a single stepped-bore sleeve, test against a John Guest fitting, validate dimensions before scaling to 4 bores

---

## Sources

### Enclosure Research (This Project)
- [hopper-and-bag-management.md](hopper-and-bag-management.md) — Hopper design, bag mounting, air management, capacitive sensing, fill paths
- [front-face-interaction-design.md](front-face-interaction-design.md) — Display holders, cable management, cartridge slot, front layout options, materials
- [back-panel-and-routing.md](back-panel-and-routing.md) — Back panel layout, internal fluid routing, electrical routing, plumbing inventory

### Cartridge Research (This Project)
- [mating-face.md](../../cartridge/planning/research/mating-face.md) — Port arrangement, release plate integration, dock simplicity
- [collet-release.md](../../cartridge/planning/research/collet-release.md) — John Guest collet mechanics, release tool geometry, failure modes
- [release-plate.md](../../cartridge/planning/research/release-plate.md) — Stepped bore geometry, spacing, compliance, material, print strategy
- [cam-lever.md](../../cartridge/planning/research/cam-lever.md) — Eccentric cam, over-center locking, lever sizing
- [electrical-mating.md](../../cartridge/planning/research/electrical-mating.md) — Pogo pins, 3 contacts, moisture separation
- [guide-alignment.md](../../cartridge/planning/research/guide-alignment.md) — Tapered pins, rail systems, FDM tolerances
- [cartridge-envelope.md](../../cartridge/planning/research/cartridge-envelope.md) — Pump dimensions, arrangement options, target envelope
- [pump-mounting.md](../../cartridge/planning/research/pump-mounting.md) — Mounting holes, vibration, tube exits, bracket options
- [under-cabinet-ergonomics.md](../../cartridge/planning/research/under-cabinet-ergonomics.md) — Cabinet zones, reach depth, sight lines, body positions
- [release-mechanism-alternatives.md](../../cartridge/planning/research/release-mechanism-alternatives.md) — Full solution space: hand disconnect, CPC couplings, dock-side mechanisms
- [dock-mounting-strategies.md](../../cartridge/planning/research/dock-mounting-strategies.md) — Wall mount vs floor, water filter prior art, scoring
- [cartridge-change-workflow.md](../../cartridge/planning/research/cartridge-change-workflow.md) — Step-by-step UX analysis, failure scenarios, time estimates

### External Sources
- [Kamoer KPHM400 Datasheet](https://m.media-amazon.com/images/I/A1at7U9PyNL.pdf) — Pump dimensions
- [Platypus Platy 2L Bottle](https://cascadedesigns.com/products/platy-2l-bottle) — Bag dimensions
- [ESP32-DevKitC V4 User Guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html) — PCB dimensions
- [Sink Base Cabinet Dimensions](https://castacabinetry.com/post/sink-base-cabinet-dimensions/) — Standard cabinet measurements
- [Under-Cabinet Ergonomics Research](../../cartridge/planning/research/under-cabinet-ergonomics.md) — Detailed zone analysis
