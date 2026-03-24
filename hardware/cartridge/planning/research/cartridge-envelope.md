# Cartridge Envelope — Dimensions, Internal Layout, and Enclosure Fit

The cartridge is a front-loading, slide-in module that carries two Kamoer KPHM400 peristaltic pumps, internal tubing, a release plate, a cam lever, and electrical pads. It slides into a dedicated slot in the enclosure — a self-contained, desktop-PC-tower-sized unit sitting on the floor of the kitchen sink cabinet. The interaction model is a blade server ejector or a CD drive in a PC tower: lever on the front face, one-handed operation, slide in until it clicks, flip lever to release.

This is the definitive reference for cartridge physical dimensions, internal component placement, and how the cartridge fits within the enclosure slot. It integrates constraints from every other research document in the cartridge and enclosure design set.

---

## 1. Terminology

| Term | Definition |
|------|-----------|
| **Enclosure** | The self-contained product housing (~desktop PC tower size). Holds bags, pumps, electronics, valves, displays, hopper. Sits on the cabinet floor. |
| **Cabinet** | The kitchen sink cabinet the enclosure sits inside. |
| **Cartridge** | The removable module. Contains 2 pumps, internal tubing, release plate, cam lever, electrical pads. Slides into the enclosure slot. |
| **Dock** | The passive receiving structure at the back of the enclosure slot. Contains John Guest fittings, pogo pins, alignment sockets, and guide rails. No moving parts. |
| **Mating face** | The rear wall of the cartridge (enters the dock first). All fluid and mechanical connections cross this boundary. |
| **Front face** | The user-facing end of the cartridge. The lever handle lives here. Flush with the enclosure front panel when docked. |

---

## 2. Coordinate System

Consistent with all enclosure and cartridge research documents:

- **Width (W)**: Left-right as viewed from the enclosure front.
- **Height (H)**: Floor-to-top (gravity direction).
- **Depth (D)**: Front-to-back (insertion axis). Mating face is at the rear; front face with lever is at the front.

---

## 3. Enclosure Context

The enclosure is the constraining boundary. The cartridge does not float in open under-sink space — it lives inside a defined slot.

### 3a. Enclosure Dimensions

From layout-spatial-planning.md, the enclosure layouts under consideration:

| Layout | W x D x H (mm) | Notes |
|--------|----------------|-------|
| Tall Tower | 250 x 200 x 450 | Smallest footprint, bags hang vertically above dock |
| Cube | 300 x 300 x 300 | Balanced proportions, bags side by side |
| Front-Loading Tower | 280 x 250 x 400 | All interaction from front face |

All variants share common traits: cartridge slot on the front face at mid-height, bags above, electronics and valves below.

### 3b. Cartridge Slot Allocation

The cartridge slot is one zone in the enclosure's internal layout. Using the Tall Tower (the tightest-fitting variant) as the constraining case:

| Dimension | Enclosure Interior | Dock Structure | Available for Cartridge |
|-----------|-------------------|----------------|------------------------|
| Width | ~242 mm | ~20 mm (walls, rails) | ~220 mm |
| Height | ~130 mm (zone) | ~45 mm (lever clearance + floor) | ~85 mm |
| Depth | ~192 mm | ~40-50 mm (back wall, fittings, tube routing) | ~142-152 mm |

Height is the tightest constraint. Width is generous. Depth is moderate.

### 3c. Front-Loading Implications

1. The cartridge front face is flush with the enclosure front panel when docked.
2. The lever is on the front face (not the top). It swings in the vertical plane.
3. The insertion axis is the depth axis. The mating face enters the dock first.
4. The dock back wall holds John Guest fittings, alignment sockets, and pogo pins.

---

## 4. Pump Dimensions — Kamoer KPHM400

Each pump is a Kamoer KPHM400-SW3B25: 12V DC brushed motor, 3 rollers, 400 ml/min, BPT tubing (4.8 mm ID x 8.0 mm OD).

| Parameter | Value | Source |
|-----------|-------|--------|
| Overall size (L x W x H) | 115.6 x 68.6 x 62.7 mm | Kamoer product page (verified) |
| Weight | ~306 g | Kamoer specs (verified) |
| Motor type | 12V DC brushed | Model suffix SW |
| Power | 10 W (~0.83 A at 12V) | Verified |
| Noise | <=65 dB | Verified |
| Tube | BPT 25# (4.8 mm ID x 8.0 mm OD) | Model suffix B25 |

The pump head (with inlet/outlet barbs on top) is at one end of the 115.6 mm length. The DC motor extends from the opposite end. The mounting bracket (M3 holes, straight plate) is perpendicular to the motor axis at the motor end.

### Inlet/Outlet Barbs

| Parameter | Value |
|-----------|-------|
| Port orientation | Vertical (upward from pump head top face) |
| Port spacing (C-C) | ~20-25 mm (estimated from drawing) |
| Barb OD | ~8 mm (matches BPT tubing) |
| Barb protrusion above pump body | ~15-20 mm |

The barbs exit perpendicular to the motor axis. When the pump is mounted horizontally with the motor axis along the depth axis, the barbs point straight up.

---

## 5. Pump Arrangement

### 5a. Why Side-by-Side Is the Only Viable Option

The enclosure's height constraint (~85 mm available) eliminates every arrangement except side-by-side:

| Arrangement | W (mm) | H (mm) | D (mm) | Fits? | Reason |
|-------------|--------|--------|--------|-------|--------|
| **Side-by-side (motors along depth)** | ~149 | ~79 | ~130 | **Yes** | Only arrangement that fits height |
| Stacked vertically | ~69 | ~146 | ~122 | No | 60 mm too tall |
| Inline (end-to-end) | ~69 | ~79 | ~237 | No | 85+ mm too deep |
| Perpendicular | ~185 | ~79 | ~185 | No | Too deep |
| Motors sideways | ~238 | ~79 | ~75 | No | 18 mm too wide |
| Head-to-tail | ~149 | ~79 | ~130 | Marginal | Same as side-by-side but worse tube routing |

The ranking did not change because a different arrangement became better — it changed because the enclosure eliminated all alternatives. Side-by-side is not just the best option; it is the only option that physically fits.

### 5b. Side-by-Side Layout

Both pumps sit side by side with motor axes parallel, pointing toward the rear of the enclosure (away from the user). Pump heads face the mating face (dock side). Motors face the front.

```
    TOP VIEW (looking down, front at bottom)

    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │   ┌────────────────────┐   ┌────────────────────┐   │
    │   │  Motor 1 ────────► │   │  Motor 2 ────────► │   │
    │   │        Pump Head 1 │   │        Pump Head 2 │   │
    │   └────────────────────┘   └────────────────────┘   │
    │                                                     │
    └─────────────────────────────────────────────────────┘
    ▲ front face (lever)                  mating face ▲
      (user side)                         (dock side)


    FRONT VIEW (looking at the user-facing front face)

    ┌─────────────────────────────────────────────────────┐
    │   ↑barbs     ↑barbs       ↑barbs     ↑barbs       │
    │   ┌──────────────────┐    ┌──────────────────┐      │
    │   │    Pump 1        │    │    Pump 2        │      │
    │   │    62.7mm tall   │    │    62.7mm tall   │      │
    │   └──────────────────┘    └──────────────────┘      │
    └─────────────────────────────────────────────────────┘
         ←── 68.6mm ───→         ←── 68.6mm ───→
```

---

## 6. Envelope Dimensions

### 6a. Dimension Buildup

| Component | W (mm) | H (mm) | D (mm) |
|-----------|--------|--------|--------|
| 2x pumps side by side | 137.2 | 62.7 | 115.6 |
| Center gap between pumps | +5 | — | — |
| Tube routing above pumps | — | +10 | — |
| Cam housing at front face | — | — | +5 |
| Housing walls (3 mm/side) | +6 | +6 | +6 |
| **Calculated envelope** | **148** | **79** | **127** |
| **Target envelope (rounded)** | **150** | **80** | **130** |

The tube routing zone above the pumps is only 10 mm (not the 20 mm assumed in the original analysis). This is achievable because silicone tubing bends at 10-15 mm radius — the barbs point up and the tubes immediately curve rearward without needing the full 15-20 mm of vertical clearance that the original BPT-based estimate required.

### 6b. Target Envelope

| Metric | Value |
|--------|-------|
| **Dimensions** | **150 x 80 x 130 mm (W x H x D)** |
| Imperial | ~5.9" x 3.1" x 5.1" |
| Volume | ~1.56 L |
| Weight | ~810 g (~1.8 lbs) |

### 6c. Fit Check Against Enclosure Slot

| Dimension | Cartridge | Available | Margin |
|-----------|-----------|-----------|--------|
| Width | 150 mm | ~220 mm | +70 mm (comfortable) |
| Height | 80 mm | ~85 mm | +5 mm (tight, adequate) |
| Depth | 130 mm | ~142-152 mm | +12-22 mm (workable) |

Height is the tightest dimension. The 5 mm margin must accommodate guide rail clearance (0.3-0.5 mm per side) and any fabrication tolerance on the pumps. If the pump's 62.7 mm height proves larger than spec, there is limited room to absorb it.

---

## 7. Internal Layout

### 7a. Side Cross-Section

```
    SIDE VIEW (looking from the left side of the cartridge)

    ← FRONT (lever, user)                       BACK (mating face, dock) →

    ┌──────────────────────────────────────────────────────────────────┐
    │  tube routing zone (10mm)                                       │
    │  ┌────────────────────────────────────────────────────┐  plate  │
    │  │                                                    │  ┌──┐   │
    │  │   PUMP (62.7mm H x 115.6mm D)                     │  │  │→  │
    │  │   Motor ──────────────────────────► Pump Head      │  │  │   │
    │  │                                                    │  └──┘   │
    │  └────────────────────────────────────────────────────┘  stubs  │
    │  ┌──┐                                                           │
    │  │  │ cam + lever pivot                                         │
    │  └──┘                                                           │
    └──────────────────────────────────────────────────────────────────┘
         5mm cam   ←─── 115.6mm pump ────→   6mm plate + 3mm travel
              ←──────────── 130mm total ───────────────→
```

### 7b. Mating Face Layout

The mating face carries all connections crossing the cartridge-dock boundary. From mating-face.md and release-plate.md:

```
    MATING FACE (looking at the rear of the cartridge)

    ┌─────────────────────────────────────────────────────────┐
    │                                                         │
    │  ○ alignment pin                     alignment pin ○    │
    │                                                         │
    │             ○ (P1-IN)       ○ (P2-IN)                   │
    │                  15mm C-C                               │
    │             ○ (P1-OUT)      ○ (P2-OUT)                  │
    │                                                         │
    │  ○ dowel pin                         dowel pin ○        │
    │                                                         │
    └─────────────────────────────────────────────────────────┘
              150mm wide x 80mm tall
```

| Feature | Specification | Source |
|---------|---------------|--------|
| 4 tube stubs | 6.35 mm OD hard tubing (nylon), 2x2 grid at 15 mm C-C | mating-face.md |
| Release plate | 6 mm thick, 4 stepped bores (8.0/10.5/12.5 mm), slides on dowel pins | release-plate.md |
| Plate travel | 3 mm (min 2.5 mm) | collet-release.md |
| Total actuation force | 12-20 N (4 fittings x 3-5 N each) | collet-release.md |
| Alignment pins | 2x tapered pins, 15-20 deg taper, 8-10 mm base | guide-alignment.md |
| Plate guide pins | 4x steel dowel pins, 3 mm dia, press-fit into cartridge body wall | release-plate.md |
| Tube stub protrusion | ~24 mm past cartridge body wall when plate retracted | mating-face.md |

15 mm center-to-center for John Guest fittings is confirmed workable. The tube port zone occupies ~33.5 x 33.5 mm including release plate margins — compact enough to center on the mating face with room for alignment features at the corners.

### 7c. Electrical Contacts

3 flat nickel-plated brass pads (~8 x 5 mm each, 10 mm C-C) on the **top face** of the cartridge. Contacted by 3 spring-loaded pogo pins mounted in the dock ceiling when the cartridge is fully inserted. Separating electrical contacts from the water fittings (which are on the mating face) eliminates moisture cross-contamination.

| Contact | Function | Max Current |
|---------|----------|-------------|
| GND | Common ground, both motors | ~1.7 A |
| Motor A+ | Pump 1 positive, 12V | ~0.85 A |
| Motor B+ | Pump 2 positive, 12V | ~0.85 A |

---

## 8. Front Face and Lever

### 8a. Lever Design

The eccentric cam lever is on the front face of the cartridge. It doubles as the extraction handle.

| Parameter | Value | Source |
|-----------|-------|--------|
| Cam eccentricity | 1.0-1.5 mm | cam-lever.md |
| Stroke | 2.0-3.0 mm | 2x eccentricity |
| Lever handle length | 60-80 mm | cam-lever.md |
| Lever swing | ~180 degrees (over-center) | cam-lever.md |
| Mechanical advantage | ~10:1 or greater | cam-lever.md |

The lever pivot is near the top of the front face. In the locked position, the handle lies flat against the front face, pointing downward. The over-center cam provides self-locking — vibration pushes it further closed, not open.

**Release sequence**: Flip lever upward (cam goes over-center, drives release plate forward via push rod through cartridge body, plate engages all 4 collets simultaneously) -> grip lever handle -> slide cartridge forward out of dock.

### 8b. Push Rod

A rigid rod (~3 mm steel or 4-5 mm PETG) transmits the cam's output from the front (lever) to the rear (release plate). Rod length: ~118 mm (130 mm cartridge depth minus front and rear wall thicknesses). The rod runs through the cartridge interior alongside the pumps, in the center gap between them.

### 8c. Flush Mounting

When docked, the cartridge front face is flush with the enclosure front panel. A 1-2 mm recessed bezel around the slot opening provides visual framing and absorbs tolerance. The lever handle is visible but does not protrude past the enclosure surface.

```
    FRONT FACE (user-facing)

    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │               ● pivot                               │
    │               ╠═══════════════════╗                  │
    │               ║  LEVER HANDLE    ║                  │
    │               ╚═══════════════════╝                  │
    │                                                     │
    └─────────────────────────────────────────────────────┘
             150mm wide x 80mm tall

    Lever flips upward to release.
    Lever handle serves as pull grip for extraction.
```

---

## 9. Depth Budget

The enclosure interior depth (~192 mm for the Tall Tower) is consumed by three zones:

```
    ← FRONT                                                BACK →

    ┌────────────┬───────────────────────────┬────────────────────┐
    │ cartridge  │     dock back wall        │  tube routing to   │
    │  body      │     + fittings            │  valves & back     │
    │  130mm     │     ~35mm                 │  panel ~22-27mm    │
    └────────────┴───────────────────────────┴────────────────────┘
                          ~192mm total
```

| Zone | Depth | Contents |
|------|-------|---------|
| Cartridge body | 130 mm | Pumps, tubing, plate, cam housing |
| Dock back wall + fittings | ~35 mm | 5 mm wall, ~25 mm JG fitting depth, ~5 mm alignment pin protrusion |
| Tube routing behind dock | ~22-27 mm | Silicone tubing from JG fittings to vertical runs toward valves and back panel |
| **Total** | **~187-192 mm** | Fits within 192 mm interior |

This is a tight fit. Compact right-angle barb fittings behind the dock wall help minimize the tube routing depth.

---

## 10. Tubing Routing Inside the Cartridge

### 10a. Routing Strategy

Silicone tubing (4.8 x 8.0 mm) handles all internal routing from pump barbs through bends. Hard 1/4" OD tubing (nylon/polyethylene) is used only for the tube stubs that protrude through the mating face and engage the John Guest fittings. Silicone cannot hold in push-to-connect fittings — it deforms and slips out.

### 10b. Tube Path (Per Pump)

1. **Pump barbs** (top of pump head, pointing up)
2. **Silicone tubing** bends 90 degrees rearward toward mating face (10-15 mm bend radius)
3. **Barb reducer fitting** transitions from silicone (8 mm OD) to hard 1/4" OD tubing
4. **Hard tube stub** passes through cartridge body wall (3 mm), through release plate clearance hole (6 mm + 3 mm travel gap), and protrudes ~15 mm into the dock fitting
5. **Total stub length from inside wall: ~30 mm**

### 10c. Strain Relief

Printed C-clips integrated into the pump tray and cartridge walls anchor tubing at two points per run: one near the pump barb, one near the barb reducer. This prevents insertion forces from reaching pump connections.

### 10d. Minimum Bend Radii

| Tubing Type | Min Bend Radius | Application |
|-------------|----------------|-------------|
| Silicone (4.8x8.0 mm) | 10-15 mm | Internal cartridge routing |
| Hard nylon (1/4" OD) | 30-40 mm cold, 15-20 mm heat-formed | Only used as straight stubs — no bending needed |

---

## 11. Cartridge Body Construction

From pump-mounting.md, the recommended approach is a tray + shell assembly:

### 11a. Three-Piece Design

1. **Pump tray**: Flat PETG plate (~140 x 116 x 6 mm) with heat-set M3 insert bosses for pump mounting brackets. Includes printed tube clips and wire routing channel. Prints flat (horizontal) for maximum screw boss strength.

2. **Outer shell**: Rectangular box with guide rail grooves on exterior walls, mating face wall with tube pass-throughs and dowel pin bosses, front face wall with cam lever pivot mount.

3. **Lid**: Flat plate closing one side. Secured with screws. Provides access for pump installation and wiring.

### 11b. Guide Rails

From guide-alignment.md: rectangular profile rails, 0.3-0.5 mm clearance per side in PETG. Grooves cut into the cartridge exterior walls mate with rails protruding from the dock/slot interior walls. Rails run the full slot depth for smooth insertion.

### 11c. Pump Mounting

From pump-mounting.md: heat-set M3 brass inserts in the PETG pump tray are the primary recommendation. Rubber grommet isolators (standard neoprene, ~6-8 mm OD) on the mounting screws reduce vibration transmission. The pump mounting bracket is perpendicular to the motor axis — bolts go through the bracket into the tray.

Exact mounting hole pattern must be measured from the physical pump (inferred ~55-65 x 40-50 mm based on KK series proportional scaling).

---

## 12. Weight Estimate

| Component | Weight | Qty | Subtotal |
|-----------|--------|-----|----------|
| Kamoer KPHM400 pump | ~306 g | 2 | 612 g |
| Silicone tubing (internal, ~400 mm) | ~15 g | 1 | 15 g |
| Hard tube stubs (4x, ~30 mm each) | ~2 g | 4 | 8 g |
| 3D printed PETG housing | ~100-120 g | 1 | ~110 g |
| Barb reducer fittings | ~5 g | 4 | 20 g |
| Release plate (PETG, 6 mm thick) | ~10 g | 1 | 10 g |
| Cam lever + push rod | ~15 g | 1 | 15 g |
| Brass electrical pads (3x) | ~5 g | 3 | 15 g |
| Steel dowel pins + misc hardware | ~15 g | — | 15 g |
| **Total** | | | **~820 g (~1.8 lbs)** |

Comfortable for one-handed handling. Center of gravity is dominated by the two pumps (75% of total weight), distributed symmetrically left-right. The lever handle provides a natural grip point.

---

## 13. Relationship to Other Enclosure Systems

### 13a. Bags

Bags (Platypus or similar, 10-12" realistic size) hang or sit in the zone above the cartridge slot. Bag outlets connect via silicone tubing to the dock's inlet-side John Guest fittings. The elevation difference provides gravity priming for pump inlets.

Hopper filling is pump-assisted — gravity fill does not work. The peristaltic pumps run in reverse to pull concentrate from the hopper funnel through the tubing into the bags. This is handled by the existing firmware (PRIME/CLEAN modes run pumps in reverse).

### 13b. Solenoid Valves and Flow Path

Solenoid valves mount below the dock zone. Tubing routes from the dock's outlet-side John Guest fittings downward to the valves, then to the flow meter and out the back panel.

### 13c. Electronics

The ESP32, L298N motor drivers, RTC, and other electronics mount above or below the dock zone (layout dependent). The dock's pogo pins connect by wire to the L298N motor driver outputs.

### 13d. Back Panel

All external connections (tap water, carbonated water in/out, 12V power, air switch tube, USB) enter through the enclosure back panel. Internal tubing routes from these connections through the enclosure to the dock fittings and valves.

### 13e. Capacitive Sensing and GPIO

FDC1004 capacitive sensing is confirmed working for liquid/air detection in tubing. Sensing electrodes are on external tubing, not inside the cartridge — no impact on cartridge dimensions.

GPIO exhaustion is solved by MCP23017 I2C expander. This is routine and does not affect the cartridge's physical design.

---

## 14. Open Questions for Physical Verification

Before finalizing CAD:

1. **Exact pump mounting hole pattern**: Measure center-to-center distances on the KPHM400-SW3B25 bracket. Record whether 2 or 4 holes.
2. **Tube exit positions on pump head**: Distance between inlet and outlet barbs, and their position relative to the mounting bracket.
3. **Motor protrusion past bracket**: How far the motor body extends behind the mounting plate. Determines tray-to-front-wall clearance.
4. **Overall pump envelope with tubes**: Full extent including barbs and factory BPT tube stubs.
5. **Lever handle ergonomics**: Test in the physical enclosure — must be comfortable when reaching into a sink cabinet.
6. **Hopper-to-bag plumbing through dock**: The pump-assisted hopper fill path runs through the cartridge pumps in reverse. The dock fittings must be plumbed to support both dispensing (bag -> pump -> faucet) and refilling (hopper -> pump -> bag) modes.

---

## Sources

- [Kamoer KPHM400 Official Product Page](https://www.kamoer.com/us/product/detail.html?id=10014) — Verified dimensions: 115.6 x 68.6 x 62.7 mm, 306 g, 10 W
- [Kamoer KPHM400 Amazon Listing (B09MS6C91D)](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D) — Product specifications
- [Kamoer KK Series Product Manual (Amazon PDF)](https://m.media-amazon.com/images/I/91kVMb3kOxL.pdf) — Mounting bracket patterns, pump head geometry
- [KPHM400 Data Sheet — DirectIndustry](https://pdf.directindustry.com/pdf/kamoer-fluid-tech-shanghai-co-ltd/kphm400-peristaltic-pump-data-sheet/242598-1017430.html) — Dimensional drawing
- [Silicone Tubing Bend Radius — Zeus Inc.](https://www.zeusinc.com/resources/summary-material-properties/bend-radius/) — Bend radius data
- Related research: mating-face.md, collet-release.md, release-plate.md, cam-lever.md, electrical-mating.md, guide-alignment.md, pump-mounting.md, release-mechanism-alternatives.md, under-cabinet-ergonomics.md, dock-mounting-strategies.md, cartridge-change-workflow.md, layout-spatial-planning.md, hopper-and-bag-management.md, front-face-interaction-design.md, back-panel-and-routing.md
