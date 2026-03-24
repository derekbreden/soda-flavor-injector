# Enclosure Layout & Spatial Planning — Integrated Master Document

The soda flavor injector enclosure is a self-contained, desktop-PC-tower-sized unit that sits inside an under-sink kitchen cabinet. It houses every component in the system: flavor bags, pump cartridge dock, solenoid valves, electronics, hopper funnels, plumbing, and displays. This document is the definitive spatial plan, integrating findings from all cartridge, hopper, front face, and back panel research into a single coherent layout.

Terminology: **enclosure** = the product (the self-contained unit). **Cabinet** = the kitchen sink cabinet it sits inside.

---

## 1. Design Principles

### 1a. Layered Architecture

The enclosure follows a strict vertical layering driven by physics and interaction frequency:

```
    TOP:     Electronics zone (heat rises, away from water)
    MIDDLE:  Dock + valves zone (user access, cartridge change)
    BOTTOM:  Bags zone (gravity feeds pump inlets)
```

This ordering satisfies three simultaneous constraints:
1. **Thermal**: Heat from L298N motor drivers (~6-12W peak) and ESP32 rises naturally to the top and vents through side louvers
2. **Water safety**: Electronics above fluids means leaks never reach PCBs
3. **Gravity flow**: Bags below pumps pre-prime the pump inlets, eliminating cavitation and ensuring complete bag drainage

### 1b. Front-Loading Cartridge

The pump cartridge loads from the front face, following the CD drive / blade server pattern. A cam lever on the cartridge's front face doubles as the extraction handle. The user reaches into the cabinet, flips the lever, and pulls the cartridge straight out in a single motion with one hand.

### 1c. Interaction Frequency Hierarchy

| Interaction | Frequency | Access Point |
|---|---|---|
| Hopper refill (pour concentrate) | Weekly+ | Front-top corner of enclosure |
| Glance at flavor display | Daily | Front face, upper area |
| S3 config adjustment | Weekly | Front face, below flavor display |
| Cartridge change | Monthly-quarterly | Front face, center slot |
| Back panel connections | Once (install) | Back panel |

The most frequent interaction (hopper) gets the most accessible position. The least frequent (back panel) gets the least accessible.

---

## 2. Overall Enclosure Dimensions

### 2a. Target Dimensions

**280W x 250D x 400H mm (11" x 10" x 16")**

This is a front-loading tower optimized for the vertical stack. All user interactions happen on the front face.

| Dimension | Value | Rationale |
|---|---|---|
| Width | 280mm (11") | Fits comfortably in the 250-350mm side zones next to under-sink plumbing |
| Depth | 250mm (10") | Allows 75-100mm clearance behind the back panel for tube bends when pushed ~150mm from the cabinet back wall |
| Height | 400mm (16") | Accommodates the full vertical stack: bags (bottom) + dock (middle) + electronics (top) + hopper inlet |
| Gross internal volume | ~28L | With 4mm wall thickness on all sides |
| Estimated weight (loaded, 2 bags full) | ~8-9 kg (18-20 lbs) | 2x ~2kg bags + ~2kg pumps/cartridge + ~1kg electronics/valves + ~1-1.5kg enclosure |

### 2b. Under-Sink Fit

Standard US kitchen sink base cabinets (36" most common) have two usable zones on either side of the P-trap and drain:

| Zone Parameter | Typical Value |
|---|---|
| Zone width | 250-350mm (10-14") |
| Zone depth | 400-500mm (16-20") |
| Zone height | 500-600mm (20-24") |

The 280 x 250 x 400mm enclosure fits comfortably in most side zones with clearance on all sides. The 400mm height leaves 100-200mm of headroom above the enclosure for hopper access (pouring). Floor footprint is 700 cm^2 -- smaller than a standard box of cereal laid flat.

### 2c. Bag Dimensions -- Corrected

Platypus bags are realistically **10-12" (250-300mm) tall**, not 14"+. 2L capacity is not a hard requirement -- smaller bags that fit the enclosure are acceptable.

| Parameter | Value |
|---|---|
| Bag height (realistic) | 250-300mm (10-12") |
| Bag width | ~190mm (7.5") |
| Bag thickness (full) | ~50-60mm (2-2.4") |
| Two bags side by side | 250-300mm H x 380mm W x 60mm D |
| Two bags stacked (front to back) | 250-300mm H x 190mm W x 120mm D |

---

## 3. Zone Breakdown with Specific Measurements

### 3a. Zone Overview (Side Cross-Section)

```
    ┌──────────────────────────────────────────────────┐
    │ ╔═HOPPER═╗                                       │ ← 400mm
    │ ║ funnel ║   ESP32, L298N x3, RTC, DIN rail     │
    │ ╚════════╝   Fuse block, power distribution      │
    │ ─────────── DRIP SHELF (solid barrier) ──────── │ ← ~310mm
    │                                                  │
    │  ┌──────────────┐                                │
    │  │  CARTRIDGE   │  Solenoids x4, needle valve    │
    │  │  DOCK        │  Tees x5, flow meter           │
    │  │  (cartridge  │  T-MERGE, T-INJECT             │
    │  │   slides in  │  Internal tube routing          │
    │  │   from front)│                                │ ← ~170mm
    │  └──────────────┘                                │
    │                                                  │
    │  ┌──────────────────────────────────┐            │
    │  │  BAG 1  (tilted ~5-10° toward    │            │
    │  │         connector for drainage)  │            │
    │  │  BAG 2  (behind or beside)       │            │
    │  └──────────────────────────────────┘            │
    │  ═══════════ DRIP TRAY ═══════════════           │ ← 0mm (floor)
    └──────────────────────────────────────────────────┘
      FRONT                                    BACK
           ←─────── 250mm depth ───────→
```

### 3b. Electronics Zone (Top, ~310-400mm height)

**Dimensions: 280W x 250D x ~90H mm**

| Component | Dimensions (mm) | Mount | Notes |
|---|---|---|---|
| ESP32-DevKitC on DIN rail breakout | 110 x 90 x 35 | Upper rear wall, DIN rail | Central to all wire routing |
| L298N motor driver x2 (pumps/valves) | 43 x 43 x 27 each | Upper side walls, near dock | Heatsink tabs face outward for convection |
| L298N motor driver x1 (clean solenoids) | 43 x 43 x 27 | Upper rear wall, near ESP32 | Short wire runs to clean solenoids below |
| DS3231 RTC module | 38 x 22 x 14 | Adjacent to ESP32, on DIN rail | I2C bus, <200mm from ESP32 |
| 12V fuse block + terminal block | ~60 x 30 x 20 | DIN rail, beside ESP32 | 3A blade fuse, screw terminal distribution |
| MCP23017 I2C expander | ~28 x 28 x 5 | Near ESP32 (I2C bus) | GPIO expansion for hopper solenoids, future sensors |

**Total DIN rail length: ~200mm (8")**

The electronics zone is separated from the plumbing zone below by a **solid drip shelf** -- a horizontal barrier that catches any upward spray and routes drips to a visible indicator at the front. Vent slots on the enclosure side walls (horizontal louvers, not top or bottom) allow passive airflow through this zone.

**Thermal load**: 2W sustained (ESP32 + PSU losses), 33W peak during dispensing (seconds at a time). Temperature rise in a sealed cabinet: <1C above ambient sustained, brief spikes under 10C. No forced ventilation needed.

### 3c. Dock + Valves Zone (Middle, ~100-310mm height)

**Dimensions: 280W x 250D x ~210H mm**

This zone contains the cartridge dock, all solenoid valves, inline components, and the plumbing interconnects.

#### Cartridge Dock

| Parameter | Value | Source |
|---|---|---|
| Cartridge envelope | 140W x 90H x 100D mm | cartridge-envelope.md |
| Cartridge weight | ~940g (2.1 lbs) | cartridge-envelope.md |
| Dock housing | ~180W x 130H x 130D mm | Includes guide rails, fitting holders |
| Pump dimensions (each, Kamoer KPHM400) | 115.6 x 68.6 x 62.7 mm | pump-mounting.md |
| Pump arrangement | Side-by-side, motors same direction | cartridge-envelope.md |
| Mating face: 4 tube stubs (2x2 grid) | 15mm center-to-center | mating-face.md |
| Release plate | 33.5 x 33.5 x 6mm, 3mm travel | release-plate.md |
| Cam lever | On cartridge front face, ~80mm handle | cam-lever.md, front-face-interaction-design.md |
| Electrical contacts | 3 pogo pins (dock side), flat pads (cartridge) | electrical-mating.md |

The dock mounts inside the enclosure with the cartridge slot opening on the front face. The lever on the cartridge front face swings horizontally to release/lock. The lever doubles as the extraction handle -- the user swings the lever and pulls the cartridge out in one motion.

The 4 John Guest fittings in the dock wall use a 2x2 grid at 15mm center-to-center spacing. This compact arrangement (33.5mm square port zone) keeps the mating face small and provides the best tilt resistance for the release plate (10.6mm maximum moment arm vs 22.5mm for a linear arrangement).

The release plate, cam, and lever are all part of the cartridge (the removable part). The dock is entirely passive: fittings in a wall, guide rails, alignment pins, pogo pins. This follows the universal prior art pattern from server blade ejectors and battery packs.

#### Solenoid Valves and Inline Components

| Component | Dimensions (mm) | Qty | Notes |
|---|---|---|---|
| Beduan 12V solenoid valve (dispensing) | ~80 x 35 x 45 | 2 | SV-D1, SV-D2 (per-flavor backflow prevention) |
| Beduan 12V solenoid valve (clean cycle) | ~80 x 35 x 45 | 2 | SV-C1, SV-C2 (already in design) |
| Needle valve | ~55 x 25 x 25 | 1 | Clean water flow restriction |
| DIGITEN flow meter | 65 x 30 x 28 | 1 | Inline with carbonated water |
| 1/4" push-connect tees | ~25 x 25 x 15 each | 5 | T-CLEAN, T1, T2, T-MERGE, T-INJECT |

Solenoid valves mount vertically (preferred for draining) on brackets inside the enclosure, accessible through the removable back panel. The 4 solenoids occupy approximately 160mm of linear space when arranged in a row.

### 3d. Bag Zone (Bottom, 0-~100mm height)

**Dimensions: 280W x 250D x ~100H mm**

Two Platypus bags sit on tilted cradles (5-10 degree incline toward the connector end) so liquid drains completely to the outlet. Bags are arranged side by side or stacked front-to-back depending on final bag dimensions.

| Parameter | Value |
|---|---|
| Bag height (hanging or tilted) | 250-300mm |
| Bag zone height allocation | ~100mm (bags lay on inclined cradles) |
| Bag connector position | At lowest point of tilted cradle |
| Tube from bag to tee (soft silicone) | ~300mm each |

The bottom of the enclosure has an integral **drip tray** (10-15mm deep) that catches any bag leaks or cartridge drips. Bags connect to the plumbing via soft silicone tubing with a zip-tie joint at the Platypus screw-cap port.

**Gravity flow**: With bags at ~50-100mm elevation and pump inlets at ~150-200mm (inside the dock), the head height is modest but sufficient for pre-priming. The peristaltic pumps create their own suction, and gravity assist eliminates air gaps in the inlet tubing.

---

## 4. Front Face Layout

### 4a. Master Front Face Plan

The front face is the only user-facing surface. All interactions happen here.

```
    ┌──────────────────────────────────────────────────┐
    │                                                  │
    │   ○ RP2040 (28mm visible)   ╔═══HOPPER════╗     │  ← front-top corner
    │   flavor display            ║  funnel x2  ║     │     most accessible
    │                             ║  (with caps)║     │     position
    │   ○ S3 (33mm visible)      ╚══════════════╝     │
    │   config display                                 │
    │   (48mm with knob)                               │
    │                                                  │
    │   ┌────────────────────────────────────────┐     │
    │   │                                        │     │
    │   │         CARTRIDGE SLOT                 │     │
    │   │         (155 x 105mm with chamfer)     │     │
    │   │                                        │     │
    │   │    ═══════════╗  ← lever/handle        │     │
    │   └────────────────────────────────────────┘     │
    │                              ● status LED        │
    │                                                  │
    │   ● power LED                                    │
    │                                                  │
    └──────────────────────────────────────────────────┘
                        280mm
```

### 4b. Display Holders

Displays are detachable with **magnetic pogo-pin breakaway connectors** (Adafruit 4-pin, $4.95/pair). When docked, the displays sit flush in magnetic retention sockets. When popped out for external mounting (cabinet door, shelf), the magnetic connector detaches cleanly.

| Display | Visible Diameter | Module Size | Retention | Cable |
|---|---|---|---|---|
| RP2040 (flavor) | 28mm | ~33mm dia x 12mm | 3x 6x3mm N52 magnets + steel discs | 4-pin magnetic pogo breakaway |
| S3 (config) | 33mm (48mm with knob) | 48 x 48 x 33mm | 4x 6x3mm N52 magnets + steel discs | 4-pin magnetic pogo breakaway |

Display arrangement: RP2040 on top (smaller, glance-only, highest visual priority), S3 below (larger, interactive, secondary). This vertical "totem" is ~48mm wide, leaving room for the cartridge slot beside it.

Internal wiring from each magnetic pogo connector to the ESP32 UART headers is permanent (soldered inside the enclosure). For external mounting, the user attaches an optional 1m coiled 4-conductor extension cable with magnetic connectors on both ends.

### 4c. Hopper Access

The hopper is at the **front-top corner** of the enclosure -- the most accessible position for the most frequent interaction. Two funnels (one per flavor, no cross-contamination) with snap-on caps.

| Parameter | Value |
|---|---|
| Funnel opening | 75-100mm (3-4") each |
| Funnel depth | 50-75mm (2-3"), buffers ~200-400ml |
| Material | Food-grade silicone (translucent, dishwasher safe) |
| Cap | Silicone plug with lanyard (insect/dust protection) |
| Fill method | Pump-assisted via hopper solenoid valves |

**Gravity fill does NOT work reliably.** The single Platypus bag opening serves all fluid paths, and gravity alone through narrow tubing is too slow and cannot manage air properly. Pump-assisted hopper filling is required, using hopper solenoid valves (Beduan 12V NC, one per flavor) controlled by the ESP32 via MCP23017 I2C expander + MOSFET drivers.

**FDC1004 capacitive sensing** reliably detects liquid/air through tubing walls -- confirmed working. This enables automatic detection of empty funnels, air intrusion in lines, and bag fill level monitoring. GPIO exhaustion is solved by the MCP23017 I2C expander -- this is a routine task, not a design concern.

### 4d. Cartridge Slot

| Parameter | Value |
|---|---|
| Slot opening (with chamfer) | 155W x 105H mm |
| Chamfer | 5mm at 45 degrees on all edges (guide funnel + visual frame) |
| Lever position | On cartridge front face, swings horizontally |
| Lever length | ~80mm (doubles as extraction handle) |
| Status LED | Above slot (green = locked, amber = unlocked, off = absent) |

The slot has a recessed border (2mm step-down) that frames it visually. No door -- the cartridge face itself fills the opening and becomes part of the front face aesthetic.

### 4e. Surface Finish

Dark navy theme (#1a1a2e) throughout -- matching the iOS app, S3 display UI, and app icon. Achieved via:
- PETG filament in dark navy (or matte black with navy accent panels)
- Matte finish (hides FDM layer lines better than gloss)
- Chamfered cartridge slot edges could have a subtle accent (matte silver or gloss navy)
- Minimal labeling: no text on front face, status communicated through LEDs and displays
- Small debossed logo in bottom corner if desired

---

## 5. Back Panel Layout

### 5a. Connection Inventory

Six connection types cross the back panel:

| # | Connection | Fitting | Protrusion (outside) |
|---|---|---|---|
| 1 | Tap water inlet | John Guest PP1208W bulkhead 1/4" + 90-degree elbow | ~55mm |
| 2 | Carbonated water inlet | John Guest PP1208W bulkhead 1/4" + 90-degree elbow | ~55mm |
| 3 | Carbonated water outlet | John Guest PP1208W bulkhead 1/4" + 90-degree elbow | ~55mm |
| 4 | 12V DC power | 5.5x2.1mm barrel jack, panel mount | ~15mm |
| 5 | Air switch tube | Rubber grommet, 3-4mm ID | Flush |
| 6 | USB port (optional) | Panel-mount Micro-B extension | ~10mm |

### 5b. Back Panel Layout (viewed from outside)

```
    ┌───────────── 280mm (11") ────────────────────┐
    │                                               │
    │   [USB]                            [12V DC]   │  ← TOP ZONE (dry)
    │                                               │
    │                     [AIR]                     │  ← MID (air switch)
    │                                               │
    │  ───────────── drip dam ridge ──────────────  │
    │                                               │
    │   [TAP IN]      [CARB IN]      [CARB OUT]    │  ← BOTTOM ZONE (wet)
    │    (blue)        (green)        (white)       │
    │                                               │
    └───────────────────────────────────────────────┘
```

Water connections at the bottom (drips fall away from electronics), power and signal at the top. A 3D-printed drip dam ridge separates wet and dry zones. Labels are embossed/debossed into the panel surface (tactile in the dark, humidity-resistant). Color-coded snap-on rings around bulkhead fitting nuts.

90-degree elbows on the outside of all three water bulkhead fittings (~$1.50 each from John Guest) allow tubes to exit parallel to the cabinet wall, preventing kinks when the enclosure is pushed back. The enclosure can sit as close as **60mm (2.4")** from the cabinet wall with no kink risk.

The back panel is **removable** (4-6 M3 screws into heat-set inserts) for internal plumbing access without disconnecting external tubes.

### 5c. Clearance Behind Enclosure

The enclosure needs 75-100mm between the back panel and the cabinet wall:
- 55mm fitting + elbow protrusion
- 20-30mm tube bend clearance

Position the enclosure ~150mm from the cabinet back wall, leaving ~100mm behind the back panel (including the 4mm panel thickness). This is well within the 400-500mm cabinet depth.

---

## 6. Internal Plumbing

### 6a. Complete System Plumbing

```
                      ┌── Bag 1 ──── T1 ──── SV-D1 ──── DOCK IN 1 ──── [P1] ──── DOCK OUT 1 ─┐
                      │              ↑                                                          │
TAP IN ── T-CLEAN ── NV ─┤         SV-C1                                                      ├── T-MERGE ── T-INJECT ── CARB OUT
                      │              ↑                                                          │                ↑
                      └── Bag 2 ──── T2 ──── SV-D2 ──── DOCK IN 2 ──── [P2] ──── DOCK OUT 2 ─┘          CARB IN ── FM
```

Plus per-flavor hopper lines:
```
Hopper 1 → [HOPPER VALVE 1] → T1 (shared tee with bag, clean, dispensing)
Hopper 2 → [HOPPER VALVE 2] → T2
```

### 6b. Operating Modes

| Mode | Hopper Valve | Clean Solenoid | Dispensing Solenoid | Pump | Flow Path |
|------|-------------|----------------|--------------------|----- |-----------|
| **Idle** | CLOSED | CLOSED | CLOSED | OFF | No flow |
| **Dispensing** | CLOSED | CLOSED | OPEN | FORWARD | Bag -> disp. sol. -> pump -> injection tee -> faucet |
| **Hopper Refill** | OPEN | CLOSED | CLOSED | OFF (pump-assisted) | Hopper -> valve -> tee -> bag |
| **Clean Fill** | CLOSED | OPEN | CLOSED | OFF | Tap water -> clean sol. -> tee -> bag |
| **Clean Flush** | CLOSED | CLOSED | OPEN | FORWARD | Bag -> disp. sol. -> pump -> faucet |

### 6c. Fitting and Tube Inventory

| Category | Count |
|---|---|
| Push-connect tees | 5 (T-CLEAN, T1, T2, T-MERGE, T-INJECT) |
| Bulkhead fittings (back panel) | 3 |
| 90-degree elbows (external strain relief) | 3 |
| Dock wall fittings | 4 |
| **Total push-connect fittings** | **~15** |
| Solenoid valves (Beduan 12V NC) | 4 (2 dispensing + 2 clean) |
| Hopper solenoid valves (future) | 2 |
| Needle valve | 1 |
| Flow meter | 1 |

### 6d. Tubing Totals

| Type | Length |
|---|---|
| 1/4" OD hard tubing | ~1,200mm (4 ft) |
| Soft silicone/BPT tubing | ~1,100mm (3.6 ft) |
| **Total tubing** | **~2,300mm (7.5 ft)** |

Hard-to-soft transitions: ~8 locations, each using ~30mm of hard tube as a stub inserted into soft tube, secured with a zip tie.

### 6e. Routing Strategy

Tubes route along enclosure walls using printed C-clips and zip-tie anchors. The key routing constraint is minimum bend radius: 30-40mm for hard tubing (cold bend), 15-20mm for soft silicone/BPT.

**Carbonated water main line**: U-shaped path from CARB IN (back panel) forward to flow meter and injection tee, then back to CARB OUT. The injection tee sits near the dock outlets.

**Clean water supply**: From TAP IN through T-CLEAN, splitting to both clean solenoids via the needle valve. Short runs (~100mm each between components).

**Flavor lines**: Soft silicone from each bag (~300mm) routes to the flavor tees. These are the most flexible tubes and can route around obstacles.

---

## 7. Electrical Layout

### 7a. Power Budget

| Condition | Current at 12V | Power |
|---|---|---|
| Idle (ESP32 + PSU losses) | ~0.15A | ~2W |
| Typical dispensing (1 pump + 1 solenoid) | ~1.3A | ~16W |
| Peak (1 pump + 2 solenoids + ESP32 + displays) | ~2.0A | ~24W |

A **12V 3A (36W) power supply** provides comfortable headroom. The existing 12V 2A adapter is borderline for peak loads.

### 7b. Power Distribution

```
12V IN (back panel barrel jack)
     │
     ▼
[3A BLADE FUSE + TERMINAL BLOCK]
     │
     ├── L298N Board A (pumps 1/2 + dispensing valves)
     ├── L298N Board B (pump power routing)
     ├── L298N Board C (clean solenoids)
     └── ESP32 VIN
```

### 7c. Wire Routing

Signal wires (ESP32 to L298N, UART to displays, I2C to RTC/MCP23017) route through printed wire channels on the upper enclosure walls. Motor power wires (L298N outputs) run separately and cross signal wires at 90 degrees only.

Cat6 cable (using 4 of 8 conductors) connects each display's magnetic pogo connector to the ESP32 UART headers. The magnetic breakaway happens at the front-face display holder; the internal cable routing is permanent.

---

## 8. Thermal Management

### 8a. Heat Sources

| Component | Power | Duty |
|---|---|---|
| L298N motor drivers (x2 active) | 6-12W | Brief bursts during dispensing (~15s per glass) |
| Kamoer pumps (x1 active) | ~7W | Same as above |
| Solenoid valves (x1-2 active) | 5.5-11W | Same as above |
| ESP32 | 0.5W | Continuous |
| PSU losses | 1-2W | Continuous |
| **Peak** | **~33W** | **Seconds at a time** |
| **Sustained** | **~2W** | **ESP32 + PSU only** |

### 8b. Thermal Strategy

No forced ventilation needed. Passive ventilation via horizontal louver slots on the enclosure side walls (near the top, in the electronics zone) provides adequate airflow. The under-sink cabinet is semi-enclosed; temperatures run 2-5C above room temperature from hot water pipes. All components are rated for 0-40C or better.

**L298N placement**: Upper portion of electronics zone, heatsink tabs facing outward toward vent slots. Heat rises naturally and exits.

**Pump heat**: Contained inside the cartridge during operation. The dock provides some thermal mass. When the cartridge is removed, pump heat exits with it.

---

## 9. Water Protection

### 9a. Protection Architecture

1. **Enclosure shell**: Solid on all sides. Top surface slightly crowned to shed external drips from cabinet plumbing above.
2. **Internal drip shelf**: Solid horizontal barrier between electronics (top) and plumbing (middle). Sloped to drain toward a visible indicator at the front edge.
3. **Drip tray**: Integral to enclosure bottom. 10-15mm deep. Catches bag leaks or cartridge drips.
4. **Vent slots**: Side walls only (horizontal louvers that reject drips while allowing airflow). No vents on top or bottom.
5. **Cartridge dock drip channel**: Below the fitting openings in the dock, routes to the front face or to the drip shelf above the bag zone.

### 9b. Cartridge Change Drip Management

When the user removes the cartridge, residual fluid in the tubes (~2-5ml per tube) can drip. The dock has a drip channel below the fitting openings that routes to the front (visible) or to a small catch tray. The front face of the dock has a slight lip to prevent drips from running down the enclosure front.

---

## 10. Component Placement Summary

### 10a. Spatial Map (Front View)

```
    ┌──────────────────────────────────────────────────┐
    │                                                  │
    │   ○ RP2040        ╔══HOPPER x2══╗               │  390-400mm
    │                   ║  funnels     ║               │
    │   ○ S3            ╚══════════════╝               │
    │                                                  │  ~310mm (drip shelf)
    │  ─────────────────────────────────────────────── │
    │                                                  │
    │   ┌────────────────────────────────────────┐     │
    │   │         CARTRIDGE SLOT                 │     │
    │   │         lever / handle                 │     │  ~170-280mm
    │   └────────────────────────────────────────┘     │
    │                              ● status LED        │
    │                                                  │  ~100mm
    │   (bags behind this wall, not visible)           │
    │                                                  │
    │   ● power LED                                    │  0mm (floor)
    └──────────────────────────────────────────────────┘
                        280mm
```

### 10b. Spatial Map (Side Cross-Section)

```
    ┌──────────────────────────────────────────────────┐
    │  Hopper funnels    │  ESP32 + DIN rail           │  400mm
    │  (front-top corner)│  L298N x3, RTC              │
    │                    │  MCP23017, fuse block        │
    │  Display holders   │                              │  310mm
    │════════ DRIP SHELF (solid horizontal barrier) ═══│
    │                    │                              │
    │  CARTRIDGE DOCK    │  Solenoids x4               │
    │  (slot opening     │  Needle valve                │
    │   on front face)   │  Tees, flow meter            │  170mm
    │                    │  Tube routing zone            │
    │                    │                              │
    │  ──────────────────│──────────────────────────── │  100mm
    │  Bag 1 (tilted     │  Bag 2 (beside or behind)   │
    │  cradle, drain fwd)│                              │
    │════════════════ DRIP TRAY ═══════════════════════│  0mm
    └──────────────────────────────────────────────────┘
      FRONT                                      BACK
                   ←── 250mm depth ──→
```

### 10c. Spatial Map (Top View, Looking Down)

```
    ┌──────────────────────────────────────────────────┐
    │                                                  │
    │  ┌──HOPPER──┐   ┌──────────────────────────┐    │
    │  │ funnel 1 │   │  ESP32 + DIN rail        │    │
    │  │ funnel 2 │   │  L298N boards            │    │
    │  └──────────┘   │  fuse + terminal blocks  │    │
    │                 └──────────────────────────┘    │
    │  ┌──DISPLAYS─┐                                  │
    │  │ RP2040    │  ┌────────────────────────┐      │
    │  │ S3        │  │  DOCK + CARTRIDGE      │      │
    │  └───────────┘  │  (extends full depth)  │      │
    │                 │  SOLENOIDS beside dock  │      │
    │                 └────────────────────────┘      │
    │                                                  │
    │  ┌──────────────────────────────────────────┐    │
    │  │  BAG 1              BAG 2                │    │
    │  └──────────────────────────────────────────┘    │
    │                                                  │
    │  ──────── BACK PANEL ────────────────────────── │
    │  [TAP] [CARB IN] [CARB OUT]    [12V] [USB] [AIR]│
    └──────────────────────────────────────────────────┘
      ▲ FRONT                               BACK ▲
                   ←── 250mm depth ──→
```

---

## 11. Removable Panels and Service Access

| Panel | Attachment | Purpose |
|---|---|---|
| **Back panel** | 4-6 M3 screws into heat-set inserts | Plumbing access without disconnecting external tubes |
| **Top panel** | 4 M3 screws or snap clips | Visual inspection, bag replacement, electronics access |
| **Front face** | Fixed (displays + cartridge slot) | Not removable -- user interaction surface |
| **Side panels** | Fixed (structural, vent slots) | Not removable |
| **Bottom** | Fixed (drip tray integral) | Not removable |

---

## 12. Key Subsystem Integration Points

### 12a. Cartridge Dock to Enclosure

The dock is permanently mounted inside the enclosure. Its front opening aligns with the cartridge slot on the front face. The dock's 4 John Guest fittings face inward (toward the cartridge). The dock's pogo pins are on the top face of the dock cavity (moisture below, electrical above, separated by a dam).

Tubes from the dock fittings route to the solenoid valves (dispensing solenoids between tees and dock, ~150mm soft silicone + hard transition). Dock outlets route to T-MERGE and then T-INJECT (~80mm + 50mm hard tube).

### 12b. Hopper to Bags

Each hopper funnel connects via tubing (~300mm) to a hopper solenoid valve, then to the shared tee (T1 or T2) that also connects the bag, clean solenoid, and dispensing solenoid. During refill, the dispensing solenoid is closed, isolating the tee -- all flow from the hopper goes into the bag.

Air management during refill: air displaced from the bag exits back through the tee and up through the hopper line (which is open to atmosphere via the funnel). FDC1004 capacitive sensing on the hopper line detects when the funnel is empty and the fill is complete.

### 12c. Bags to Dock

Soft silicone tubing (~300mm) from each bag's screw-cap port routes to T1/T2, then through the dispensing solenoid to the dock inlet fitting. The bag connector is at the lowest point of a tilted cradle, ensuring gravity drains the bag completely.

### 12d. Electronics to Dock

3 pogo pins on the dock carry GND, Motor A (12V), Motor B (12V) to the cartridge. Wired from L298N motor output terminals (~150mm 18AWG stranded). The pogo pins are on the top face of the dock cavity with a printed dam between the electrical and fluid zones.

### 12e. Back Panel to Internal Plumbing

Three bulkhead fittings on the back panel connect to internal plumbing via short hard-tube runs:
- TAP IN -> 150mm -> T-CLEAN
- CARB IN -> 100mm -> flow meter -> 80mm -> T-INJECT
- CARB OUT <- 100mm <- T-INJECT

---

## 13. Bill of Materials -- Spatial Components

| Component | Qty | Est. Unit Cost | Notes |
|---|---|---|---|
| John Guest PP1208W bulkhead 1/4" | 3 | $5 | Back panel |
| John Guest PP0308W 90-degree elbow | 3 | $1.50 | External strain relief |
| John Guest union tee 1/4" | 5 | $3 | T-CLEAN, T1, T2, T-MERGE, T-INJECT |
| Beduan 12V NC solenoid valve 1/4" | 4 (+2 hopper) | $9 | Dispensing x2, clean x2, hopper x2 |
| Needle valve (YKEBVPW) | 1 | $8 | Clean water flow restriction |
| DIGITEN flow meter | 1 | $10 | Carbonated water line |
| Adafruit 4-pin magnetic pogo connector | 2 pairs | $5/pair | Display breakaway connectors |
| 6x3mm N52 neodymium disc magnets | ~14 | $0.15 | Display retention (7 per pair x 2 displays) |
| MCP23017 I2C expander | 1 | $2 | GPIO expansion |
| 12V 3A power supply | 1 | $12 | External barrel jack PSU |
| 3A blade fuse + holder | 1 | $3 | Input protection |
| DIN rail + terminal blocks | 1 set | $8 | Electronics mounting |
| 1/4" OD hard tubing | ~1.2m | From ice maker kit | Internal routing |
| Soft silicone tubing | ~1.1m | From existing spool | Flexible sections |
| Heat-set M3 brass inserts | ~30 | $0.05 | Panel mounting, dock mounting, cartridge bosses |
| 3D printed PETG enclosure panels | ~5 pieces | Filament cost | Front face, back panel, top, 2 sides, bottom/drip tray |

---

## 14. Open Questions

1. **Exact bag dimensions**: Measure actual Platypus bags in hand to confirm the 10-12" realistic height and whether they fit the 280mm enclosure width when side by side.
2. **Hopper pump feasibility**: Validate pump-assisted hopper fill with the shared-tee plumbing topology. Test whether the solenoid isolation is sufficient to direct all flow into the bag.
3. **Cartridge dock mockup**: Build a cardboard or foam-core mockup of the cartridge slot at the proposed height (~170-280mm from enclosure floor) and test insertion/removal ergonomics in the actual cabinet.
4. **Cabinet measurement**: Measure the actual under-sink cabinet where this will be installed. Confirm the side zone width accommodates 280mm, and the usable height accommodates 400mm plus hopper clearance.
5. **Thermal validation**: After assembly, measure temperatures at L298N heatsinks and ESP32 during a sustained dispensing session (20 glasses in an hour) to confirm passive ventilation is adequate.

---

## Sources

### Component Dimensions
- [Kamoer KPHM400 Datasheet (Amazon PDF)](https://m.media-amazon.com/images/I/A1at7U9PyNL.pdf) -- Pump dimensions, mounting pattern
- [Kamoer KPHM400 Amazon Listing](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D) -- Weight
- [Platypus Platy 2L Bottle (Cascade Designs)](https://cascadedesigns.com/products/platy-2l-bottle) -- Bag dimensions
- [ESP32-DevKitC V4 User Guide (Espressif)](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html)
- [L298N Motor Driver Module (Components101)](https://components101.com/modules/l293n-motor-driver-module)
- [Elecrow CrowPanel 1.28" Rotary Display](https://www.elecrow.com/crowpanel-1-28inch-hmi-esp32-rotary-display-240-240-ips-round-touch-knob-screen.html)
- [Waveshare RP2040-LCD-0.99-B Wiki](https://www.waveshare.com/wiki/RP2040-LCD-0.99-B)
- [Adafruit DIY Magnetic Connector 4-Pin (Product 5358)](https://www.adafruit.com/product/5358)

### Cartridge Design (Prior Research)
- [mating-face.md](../../cartridge/planning/research/mating-face.md) -- Fitting layout, release plate on cartridge, lever placement
- [collet-release.md](../../cartridge/planning/research/collet-release.md) -- Collet mechanics, stepped bore geometry
- [release-plate.md](../../cartridge/planning/research/release-plate.md) -- Plate dimensions, guide features, compliance
- [cam-lever.md](../../cartridge/planning/research/cam-lever.md) -- Eccentric cam mechanism, prior art survey
- [electrical-mating.md](../../cartridge/planning/research/electrical-mating.md) -- Pogo pin contacts, moisture mitigation
- [guide-alignment.md](../../cartridge/planning/research/guide-alignment.md) -- Rail slides, tapered pin alignment
- [pump-mounting.md](../../cartridge/planning/research/pump-mounting.md) -- Pump mounting, vibration, tube routing
- [cartridge-envelope.md](../../cartridge/planning/research/cartridge-envelope.md) -- Envelope sizing, pump arrangements
- [release-mechanism-alternatives.md](../../cartridge/planning/research/release-mechanism-alternatives.md) -- Full solution space exploration
- [under-cabinet-ergonomics.md](../../cartridge/planning/research/under-cabinet-ergonomics.md) -- Reach zones, sight lines, body positioning
- [dock-mounting-strategies.md](../../cartridge/planning/research/dock-mounting-strategies.md) -- Mounting location survey
- [cartridge-change-workflow.md](../../cartridge/planning/research/cartridge-change-workflow.md) -- Complete UX walkthrough

### Enclosure Design (Prior Research)
- [hopper-and-bag-management.md](hopper-and-bag-management.md) -- Hopper system, air management, bag mounting
- [front-face-interaction-design.md](front-face-interaction-design.md) -- Display holders, cartridge slot, visual hierarchy
- [back-panel-and-routing.md](back-panel-and-routing.md) -- Back panel connections, internal routing, power distribution

### Under-Sink Environment
- [Sink Base Cabinet Dimensions (Casta Cabinetry)](https://castacabinetry.com/post/sink-base-cabinet-dimensions/)
- [Kitchen Cabinet Sizes (Kitchen Cabinet Kings)](https://kitchencabinetkings.com/guides/kitchen-cabinet-sizes)
