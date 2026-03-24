# Dock Integration Within the Enclosure

The dock is not a standalone bracket screwed to a cabinet wall. It is part of a self-contained enclosure — a tower (~280 x 250 x 400mm) that sits inside the cabinet as a single unit. The dock is structurally integrated into the enclosure.

The question: **where on/in the enclosure does the cartridge dock sit, and how does it integrate structurally with the enclosure frame, fluid routing, and electrical routing?**

**Established parameters from other research:**

| Parameter | Value | Source |
|---|---|---|
| Enclosure (front-loading tower) | ~280W x 250D x 400H mm | layout-spatial-planning.md |
| Cartridge envelope | ~150W x 80H x 130D mm, ~820g | cartridge-envelope.md |
| Dock estimate (housing) | ~180W x 130H x 130D mm, ~500g | layout-spatial-planning.md |
| Lever clearance above dock | ~40mm | layout-spatial-planning.md |
| Dock + lever vertical extent | ~120mm (80mm cartridge + 40mm lever) | layout-spatial-planning.md |
| Tube fittings (dock wall) | 4x John Guest 1/4" push-connect | collet-release.md |
| Electrical contacts (dock) | 3x pogo pins | electrical-mating.md |
| Fitting C-C spacing | 15mm recommended | mating-face.md |
| John Guest fitting body OD | ~12.7mm | collet-release.md |
| Guide rail clearance (FDM) | 0.3-0.5mm per side | guide-alignment.md |
| Cam lever eccentricity | 1-1.5mm for 2-3mm stroke | cam-lever.md |
| Pump vibration frequency | 10-17 Hz (roller), plus motor HF | pump-mounting.md |
| Bag zone height | ~176mm (two 1L bags at 18-20 deg incline) | bag-zone-geometry.md |
| Bag mounting | Two-point stretch, 18-20 deg incline | incline-bag-mounting.md |
| Hopper fill method | Pump-assisted (reversed main pump) | pump-assisted-filling.md |
| FDC1004 capacitive sensing | Confirmed working | confirmed |

---

## 1. The Enclosure's Layered Architecture

The front-loading tower layout divides the enclosure into three horizontal zones stacked vertically. The dock sits in the middle zone.

```
    FRONT VIEW                           SIDE VIEW (cross-section)

    ┌─────────────────────┐             ┌─────────────────┐
    │   HOPPER INLET      │             │   HOPPER        │
    │   (pump-assisted)   │ ← top cap   │   INLET         │
    ├─────────────────────┤             ├─────────────────┤
    │ [S3]         [RP]   │             │                 │
    │   ESP32, L298N x3   │             │  ELECTRONICS    │  ~90mm
    │   RTC, fuse block   │ ZONE A      │  zone           │
    ├═════════════════════┤ ─ ─ ─ ─ ─  ├═════════════════┤ ← ELECTRONICS SHELF (~310mm)
    │ ┌─────────────────┐ │             │  ┌───────────┐  │
    │ │  CARTRIDGE     ◄│ │             │  │ CARTRIDGE │  │
    │ │  DOCK           │ │ ZONE B      │  │ DOCK      │  │  ~120mm
    │ │  (slide-in)     │ │             │  │           │  │  (incl lever)
    │ └─────────────────┘ │             │  └───────────┘  │
    │   Solenoid valves   │             │  VALVES         │
    │   Flow meter        │             │  FLOW METER     │
    ├═════════════════════┤ ─ ─ ─ ─ ─  ├═════════════════┤ ← DOCK SHELF (~180mm)
    │                     │             │        *  sealed│
    │   Platypus bags     │             │       / BAG 2   │
    │   (2x 1L, incline   │ ZONE C      │      / 18 deg  │  ~176mm
    │    mount 18-20 deg, │             │  * conn        │
    │    connector low,   │             │        *  sealed│
    │    sealed end high) │             │       / BAG 1   │
    │                     │             │  * conn        │
    └─────────────────────┘             └─────────────────┘
         280mm                               250mm
```

**Zone A (top, ~90mm, 310-400mm):** Electronics. ESP32, three L298N motor drivers, RTC module, MCP23017, fuse block, DIN rail, wiring. Displays mount flush on the front face. Hopper inlet on top cap. Vent louvers on side walls for passive convection.

**Zone B (middle, ~120mm, 186-306mm):** The dock, cartridge, solenoid valves, flow meter, needle valve, tees, check valves. This is the densest zone. The cartridge occupies 80mm of height, with 40mm of lever clearance above it. Valves sit beside the dock on the shelf.

**Zone C (bottom, ~176mm, 4-180mm):** Bags. Two 1L Platypus bags mount at an 18-20 degree incline with two-point stretch (connector low at front, sealed end high at rear). Bags stack vertically in the same depth footprint. The incline mount constrains collapse to thinning-in-place, and the dip tube extends upward along the incline to remain submerged until the bag is nearly empty. See [incline-bag-mounting.md](../../../enclosure/research/incline-bag-mounting.md) for full geometry.

---

## 2. The Dock as a Structural Shelf

The dock is not just a receptacle for the cartridge. It is a horizontal structural member that divides Zone B from Zone C. It functions as a shelf.

### Why a Shelf, Not a Floating Sub-Assembly

A dock bolted to the front face and cantilevered inward would rely on the front panel for all structural load. The cartridge weighs ~820g, the dock itself ~500g, and insertion force adds ~20N of momentary push. A cantilevered mount from a 3D printed front panel would flex visibly.

Making the dock a shelf that spans the full depth of the enclosure (front wall to back wall) distributes load to both walls. The dock shelf also:

- Creates a physical barrier between the fluid-handling zone (B) and the bag zone (C)
- Provides a mounting surface for solenoid valves (on or below the shelf)
- Routes all four fluid lines through defined holes in the shelf floor
- Gives the enclosure lateral rigidity (a horizontal member tying the side panels together)

### Dock Shelf Construction

```
    TOP VIEW — dock shelf in enclosure (looking down)

    ┌──────────────────────────────── 280mm ──────────────────────────────┐
    │                                                                      │
    │  ┌──────────────── DOCK SHELF ────────────────────────────────────┐ │
    │  │                                                                │ │
    │  │  ┌─────────────────────────┐    ┌────────────────────────┐    │ │
    │  │  │   CARTRIDGE CAVITY      │    │   FITTING WALL         │    │ │
    │  │  │   (open to front face)  │    │   (4x JG fittings)     │    │ │
    │  │  │   150W x 130D x 80H    │    │   faces back of        │    │ │
    │  │  │                         │    │   enclosure             │    │ │
    │  │  │   guide rails on sides  │    │                        │    │ │
    │  │  └─────────────────────────┘    └────────────────────────┘    │ │
    │  │                                                                │ │
    │  │  (shelf solid floor below cartridge cavity — fluid pass-      │ │
    │  │   throughs are holes in the fitting wall, not the floor)      │ │
    │  │                                                                │ │
    │  └────────────────────────────────────────────────────────────────┘ │
    │                                                                      │
    │  ▲ front face                                          back panel ▲  │
    │                                                                      │
    └──────────────────────────────────────────────────────────────────────┘
```

The dock shelf is a single 3D printed piece (or a 2-part assembly: shelf floor + fitting wall) that drops into the enclosure and screws to the side walls via heat-set inserts. The shelf spans the full 242mm depth and most of the 272mm width (interior dimensions after 4mm walls).

### Shelf Dimensions

| Parameter | Value | Notes |
|---|---|---|
| Shelf width | ~264mm | 272mm enclosure interior minus ~4mm clearance per side |
| Shelf depth | ~234mm | 242mm enclosure interior minus clearance |
| Shelf thickness (floor) | 6mm | Structural minimum for PETG span, supports cartridge weight |
| Fitting wall height | ~100mm | Enough to house JG fittings + pogo pins + guide rail attachment |
| Fitting wall thickness | 6mm | Bulkhead fittings need ~4mm panel minimum |
| Total shelf + wall height | ~106mm | Floor + vertical wall at the back |

---

## 3. Cartridge Slot Position Within the Enclosure

### Height Above Enclosure Floor

The cartridge slot opening is on the front face of the enclosure, within Zone B (dock/valves).

```
    FRONT VIEW — vertical position of cartridge slot

    ┌─────────────────────────┐ ← 400mm (top of enclosure)
    │  HOPPER                 │
    │  ───────────────────    │ ~396mm
    │  [S3]          [RP]    │
    │  ESP32, L298N, DIN     │ ~310mm
    │  ═══════════════════    │ ← electronics shelf
    │  ┌───── lever ──────┐  │ ~306mm
    │  │                  │  │
    │  │ ╔═══════════════╗│  │ ~266mm  ← top of cartridge slot
    │  │ ║  CARTRIDGE    ║│  │
    │  │ ║  SLOT         ║│  │ ~186mm  ← bottom of cartridge slot
    │  │ ╚═══════════════╝│  │
    │  └──────────────────┘  │
    │  ═══════════════════   │ ← dock shelf floor (~180mm)
    │                        │
    │  bags (incline mount)  │ ~176mm bag zone
    │                        │
    │                        │
    └────────────────────────┘ ← 0mm (enclosure floor)
```

| Reference Point | Height from enclosure floor |
|---|---|
| Enclosure floor | 0mm |
| Floor panel top | 4mm |
| Top of bag zone (dock shelf floor) | ~180mm |
| Bottom of cartridge slot opening | ~186mm |
| Top of cartridge slot opening | ~266mm |
| Top of lever swing | ~306mm |
| Electronics shelf | ~310mm |
| Displays | ~350mm |
| Top of enclosure | 400mm |

The cartridge slot center is at roughly **226mm** from the enclosure floor. If the enclosure sits on the cabinet floor (~0mm), the slot center is 226mm (~9") up — a comfortable reach height under a sink where the user is crouching or kneeling.

### Distance From Front Face

The cartridge slides in from the front face. The slot opening is flush with the front panel. The dock cavity extends ~165mm into the enclosure (130mm cartridge depth + ~35mm for the fitting wall behind the cartridge).

```
    SIDE CROSS-SECTION — dock zone

    FRONT FACE                                          BACK PANEL
    │                                                         │
    │  ┌─ cartridge ──────────────┐  ┌─ fitting wall ─┐      │
    │  │                          │  │  ○ ○  JG fittings│     │
    │  │  150W x 80H x 130D      │  │  ○ ○  (facing    │     │
    │  │  (slide-in space)        │  │       back)      │     │
    │  │                          │  │  • • • pogo pins │     │
    │  └──────────────────────────┘  └─────────────────┘      │
    │                                                         │
    │◄──────── 130mm ──────────────►◄── 35mm ──►◄── 77mm ──►│
    │    cartridge depth            fitting wall  remaining   │
    │                               + fittings   depth for   │
    │                                            tube runs   │
    │                                                         │
    ├────────────────────── 250mm total depth ────────────────┤
```

Behind the fitting wall, there is approximately 77mm of depth for tube routing from the fittings down through the shelf floor to the bag zone below, and up to the valves mounted on or under the shelf. This is more than adequate for 1/4" tubing with 90-degree bends (minimum bend radius ~20mm for silicone, ~30mm for hard tubing).

---

## 4. Dock-to-Enclosure Fluid Routing

The dock's fitting wall has 4 John Guest 1/4" push-to-connect fittings (in a 2x2 grid at 15mm center-to-center). The fittings face the BACK of the enclosure — the cartridge slides in from the front and the tube stubs push into the fittings from the front side.

On the back side of the fitting wall, 4 hard tubes exit the fittings and route to their destinations:

```
    BACK VIEW of fitting wall (viewed from inside the enclosure, looking toward front)

    ┌─────────────────────────────┐
    │                             │
    │      ○ P1-IN    ○ P2-IN    │  ← inlet fittings (from bags)
    │                             │
    │      ○ P1-OUT   ○ P2-OUT   │  ← outlet fittings (to carbonated water)
    │                             │
    │      • GND  • A+  • B+     │  ← pogo pins (above or below fittings)
    │                             │
    └─────────────────────────────┘
```

### Inlet Routing (Bags to Dock)

Two inlet lines carry flavor concentrate from the bags (Zone C, below) to the dock fittings:

```
    SIDE CROSS-SECTION — inlet tube path

                        ┌─── fitting wall
                        │
    Bag 1 outlet  ──────┤  ┌─ solenoid valve SV-D1
    (Zone C, below)     │  │
         │              │  │
         ~~ soft tube   │  │  hard tube up
         │              │  │  through shelf
         └── through ───┘──┘── into JG fitting P1-IN
              shelf hole
```

Each inlet line runs:
1. From bag dip tube connector (low point of incline mount, near front wall) through a **soft silicone tube** (~200-250mm)
2. Through a **tee fitting** (splits to clean cycle solenoid)
3. Through the **dispensing solenoid valve** (SV-D1 or SV-D2)
4. Through a **hole in the dock shelf floor** (or around the shelf edge)
5. Up the back of the fitting wall into the **John Guest inlet fitting**

Most of this tubing can be silicone. The only hard tubing transitions needed are at the John Guest fitting connections (push-connect requires hard 1/4" OD tubing) and at a few transition points near the valves.

**FDC1004 sensor placement:** The inlet tube runs between the bag and the dock are the right location for capacitive liquid/air detection (FDC1004 confirmed working). A sensor breakout clips or zip-ties to the silicone tube in the dock zone, with the I2C lines running up to the ESP32 in Zone A. This detects empty bags and air ingestion before the pump runs dry.

### Outlet Routing (Dock to Carbonated Water Line)

Two outlet lines carry flavored concentrate from the dock fittings to the carbonated water main line (which exits via the back panel):

```
    Dock P1-OUT fitting → hard tube → joins carbonated water line
    Dock P2-OUT fitting → hard tube → joins carbonated water line
```

The outlet tubes route from the back of the fitting wall, through the shelf or around its edge, and across to the back panel area where they join the carbonated water stream via tee fittings. These runs are mostly horizontal, ~100-150mm long.

### Hopper Connection (Pump Outlet Side)

The hopper connects downstream of the pump (between pump outlet and dispensing point), not at the bag-side tee. During refill, the pump reverses direction to pull concentrate from the hopper funnel and push it through the dip tube into the bag. This corrects an earlier topology assumption. A check valve at the dispensing point prevents air ingestion during refill.

### Tube Pass-Through Design

The dock shelf floor needs holes for tubes to pass between Zone B and Zone C. These are simple round holes (~10mm diameter for 1/4" OD tubing with clearance) drilled or printed into the shelf. A rubber grommet in each hole provides strain relief and prevents rattling.

| Pass-through | Purpose | Location in shelf |
|---|---|---|
| Inlet 1 | Bag 1 to SV-D1 to dock | Near fitting wall, left side |
| Inlet 2 | Bag 2 to SV-D2 to dock | Near fitting wall, right side |
| Clean 1 | Clean solenoid to tee on inlet 1 | Near left valve cluster |
| Clean 2 | Clean solenoid to tee on inlet 2 | Near right valve cluster |

Outlet tubes do not need to pass through the shelf — they route behind the fitting wall and across to the back panel within Zone B.

---

## 5. Dock-to-Enclosure Electrical Routing

The dock has 3 pogo pins that contact flat pads on the cartridge. These pogo pins connect to the L298N motor driver outputs in Zone A (electronics, above the dock).

### Wire Path

```
    FRONT CROSS-SECTION — electrical routing

    ┌─────────────────────────────────┐
    │  ┌─────── ZONE A ────────────┐  │
    │  │  L298N #1     L298N #2    │  │
    │  │    │ motor out   │        │  │
    │  │    └──── wires ──┘        │  │
    │  │          │                │  │
    │  │          │ (through shelf │  │
    │  │          │  or along      │  │
    │  │          │  enclosure     │  │
    │  │          │  wall)         │  │
    │  └──────────┼────────────────┘  │
    │  ┌──────────┼── ZONE B ──────┐  │
    │  │          │                │  │
    │  │          ▼                │  │
    │  │    pogo pin block         │  │
    │  │    (on fitting wall)      │  │
    │  │                           │  │
    │  └───────────────────────────┘  │
    └─────────────────────────────────┘
```

Three wires (GND, Motor A+, Motor B+) route from the L298N screw terminals down to the pogo pin block on the fitting wall. The wires run along the interior side wall of the enclosure or through a channel printed into the dock shelf. The total wire run is approximately 150-200mm.

### Wire Specifications

| Wire | Gauge | Max Current | Notes |
|---|---|---|---|
| GND (common) | 20 AWG stranded | ~1.7A (both pumps) | Silicone jacket for flexibility |
| Motor A+ | 22 AWG stranded | ~0.85A | Silicone jacket |
| Motor B+ | 22 AWG stranded | ~0.85A | Silicone jacket |

Wires are secured to the enclosure wall with printed clips or a small adhesive cable channel. A JST connector at the pogo pin block allows the dock shelf to be removed for maintenance without desoldering.

### Moisture Separation

The pogo pins are on the fitting wall, close to the John Guest fittings. Per electrical-mating.md, electrical contacts should be separated from water connections by at least 10-20mm with a dam/ridge between them. On the fitting wall:

- Pogo pins mount **above** the John Guest fittings (water drips down, not up)
- A printed ridge (~3mm tall) runs horizontally between the pogo pin zone and the fitting zone
- The fitting wall is oriented vertically, so any drip from a leaking fitting falls straight down, away from the pogo pins

---

## 6. Structural Requirements

### Rigidity for Alignment

The cartridge's 4 tube stubs must hit the 4 John Guest fittings within ~1mm (per guide-alignment.md). This precision depends on:

1. **Guide rails** maintaining cartridge lateral and vertical position during insertion
2. **Fitting wall** holding the 4 fittings in fixed positions relative to the guide rails
3. **The dock shelf** not flexing under insertion force (~20N)

The guide rails are part of the dock shelf assembly, attached to the shelf's side walls (or printed integrally). Because the rails and fitting wall are on the same rigid part, their relative positions are determined at print time — no installation alignment required.

### Material and Thickness

| Component | Material | Minimum Thickness | Notes |
|---|---|---|---|
| Dock shelf floor | PETG | 6mm | Spans ~234mm depth; must not flex under ~1.3kg cartridge + dock weight |
| Fitting wall | PETG | 6mm | Holds bulkhead fittings (hex nut clamps against 4mm+ panel) |
| Guide rail features | PETG | 4mm walls | FDM sliding surfaces, 0.3-0.5mm clearance per side |
| Shelf-to-enclosure attachment | M3 heat-set inserts | 2.5mm wall around insert | 4-6 screws into enclosure side walls |

### Infill and Print Orientation

The dock shelf should be printed with the floor horizontal (flat on the build plate) for maximum strength across the span. The fitting wall can be printed as a separate piece (vertical orientation for best dimensional accuracy on the fitting holes) and attached to the shelf with screws or a dovetail joint.

**Recommended infill:**
- Shelf floor: 40-60% infill (structural span)
- Fitting wall: 60-100% infill (fitting mounting area needs full density for nut clamping force)
- Guide rail areas: 4+ perimeters, 40% infill minimum

### How Guide Rails Attach to the Enclosure

The guide rails are part of the dock shelf, not the enclosure walls. This is important: if the rails were on the enclosure walls and the fittings were on the dock shelf, any misalignment between the shelf and the walls would propagate to the cartridge-to-fitting alignment.

By keeping rails and fittings on the same part (the dock shelf), the critical alignment chain is:

```
    Rail → shelf → fitting wall → fitting position
    (all one rigid piece — no inter-part tolerance stack-up)
```

The shelf mounts to the enclosure walls with enough clearance (~0.5mm) that minor enclosure dimensional variation does not stress the shelf. The shelf is located vertically by resting on printed ledges inside the enclosure walls.

---

## 7. Vibration Isolation

### The Problem

The cartridge contains 2 running Kamoer KPHM400 peristaltic pumps. Each pump produces:
- **Low-frequency pulsation** at 10-17 Hz (3 rollers x RPM/60) — this is felt as a mechanical thump
- **High-frequency motor vibration** from the DC brushed motor (commutator noise, rotor imbalance)
- **Combined noise** rated at <=65 dB per pump

The vibration transmission path is: pump → cartridge body → guide rails → dock shelf → enclosure walls → cabinet shelf.

### Where to Isolate

There are three possible isolation points:

**Point A: Pump-to-cartridge (inside the cartridge)**
Rubber grommet isolators on the pump mounting screws within the cartridge body. Per pump-mounting.md, this reduces transmission by 60-80% above 30 Hz. This is already the recommended approach for the cartridge design.

**Point B: Cartridge-to-dock (at the guide rails)**
The sliding fit between the cartridge rails and dock rails already provides slight decoupling — the 0.3-0.5mm clearance per side means the cartridge is not rigidly press-fit into the dock. However, the cam lever locks the cartridge firmly, which re-couples vibration.

A thin elastomeric pad (1-2mm silicone or neoprene) on the dock rail surfaces could provide isolation at this interface. The risk: the pad compresses under cam lever force and changes the alignment tolerance. Silicone with 30-40 Shore A durometer compresses ~0.3mm under the cartridge's weight, which is within the alignment budget but leaves less margin.

**Point C: Enclosure-to-cabinet (at the enclosure feet)**
Rubber feet on the bottom of the enclosure, isolating the entire unit from the cabinet shelf. This is the simplest and most effective approach for the cabinet. Standard rubber bumper feet (12-15mm diameter, 6-8mm tall, Shore 40-60A) from McMaster or Amazon provide broadband isolation.

### Recommendation

Use **two isolation points**:

1. **Rubber grommets on pump mounting screws** (Point A, inside the cartridge). This is the primary isolation — it reduces vibration at the source before it enters the dock.

2. **Rubber feet on the enclosure** (Point C). This prevents whatever residual vibration passes through the dock from transmitting to the cabinet and countertop. Four rubber feet at the enclosure corners are cheap and effective.

Point B (cartridge-to-dock rail isolation) is unnecessary if Points A and C are implemented. Adding compliant material at the rail interface risks degrading the alignment precision that the rigid dock shelf is designed to provide.

### Noise Amplification

The dock shelf is a flat span of PETG — a potential sounding board. If the shelf resonates at or near the pump pulsation frequency (10-17 Hz), it amplifies noise.

Mitigation:
- The shelf's natural frequency should be well above 17 Hz. For a 6mm thick PETG shelf spanning 234mm with ~1.5 kg of load, the natural frequency is roughly 25-40 Hz (depends on boundary conditions) — comfortably above the excitation range.
- If noise is objectionable in practice, adding a thin damping layer (adhesive-backed neoprene or butyl sheet) to the underside of the shelf floor converts vibration energy to heat. This is a post-hoc fix, not something to design in from the start.

---

## 8. Dimensioned Dock Cross-Section Within the Enclosure

### Front-to-Back Cross-Section (Side View, Cut Through Center)

```
    ┌──────────────────────────────────────────────────────────┐
    │  FRONT PANEL                                  BACK PANEL │
    │  │                                                   │   │
    │  │  ╔════════════════════════╗                        │   │  ← Zone A (electronics)
    │  │  ║ ESP32, L298N x3,      ║                        │   │     ~90mm
    │  │  ║ RTC, fuse, DIN rail   ║                        │   │
    │  │  ╚════════════════════════╝                        │   │
    │  │  ═══════════════════════════════════════════════   │   │  ← electronics shelf (~310mm)
    │  │                                                   │   │
    │  │  ┌── cartridge ──────────────┐  ┌─ fitting wall ┐ │   │
    │  │  │                           │  │               │ │   │
    │  │  │  P1  P2  (pumps)         │  │ ○ ○  JG fitt. │ │   │     ~80mm cartridge height
    │  │  │                           │  │ ○ ○           │ │   │
    │  │  │  cam lever above          │  │ • • • pogos   │ │   │
    │  │  │                           │  │               │ │   │
    │  │  └───────────────────────────┘  └───────────────┘ │   │
    │  │  ════════════════════════════════════════════════  │   │  ← dock shelf floor (6mm PETG, ~180mm)
    │  │       ↕ tube pass-throughs (10mm holes)           │   │
    │  │                                                   │   │
    │  │                            * sealed end (high)    │   │
    │  │                           /  BAG 2                │   │     ~176mm bag zone
    │  │                     * conn/                       │   │     (two 1L bags at
    │  │                         /                         │   │      18-20 deg incline)
    │  │                 * sealed end (high)                │   │
    │  │                /  BAG 1                            │   │
    │  │          * conn/                                   │   │
    │  │                                                   │   │
    │  └───────────────────────────────────────────────────┘   │
    └──────────────────────────────────────────────────────────┘
    │◄──── 250mm depth ────►│
```

### Top-Down Cross-Section (Plan View at Dock Shelf Height)

```
    ┌────────────────────────── 280mm ──────────────────────────┐
    │                                                            │
    │  FRONT FACE                                    BACK PANEL  │
    │  │                                                    │    │
    │  │   ┌──── guide rail (left) ──────────────────┐     │    │
    │  │   │                                          │     │    │
    │  │   │  ┌─ cartridge cavity ──────────┐        │     │    │
    │  │   │  │                              │ fitting│     │    │
    │  │   │  │   150mm wide x 130mm deep   │  wall  │     │    │
    │  │   │  │                              │  (6mm) │     │    │
    │  │   │  │   open to front face         │  ○ ○   │     │    │
    │  │   │  │   (cartridge slides in)      │  ○ ○   │     │    │
    │  │   │  │                              │  15mm  │     │    │
    │  │   │  │                              │  C-C   │     │    │
    │  │   │  └──────────────────────────────┘        │     │    │
    │  │   │                                          │     │    │
    │  │   └──── guide rail (right) ─────────────────┘     │    │
    │  │                                                    │    │
    │  │   ┌─ valve area ──────────────────────────┐       │    │
    │  │   │  SV-D1   SV-C1   NV   SV-C2   SV-D2  │       │    │
    │  │   └───────────────────────────────────────┘       │    │
    │  │                                                    │    │
    │  │◄── 130mm ──►◄─ 35mm ─►◄────── 77mm ──────►       │    │
    │  │  cartridge   fitting   tube routing space          │    │
    │  │  depth        wall                                 │    │
    │                                                            │
    └────────────────────────────────────────────────────────────┘
```

### Front Face Elevation (Cartridge Slot Detail)

```
    ┌────────────────── 280mm ──────────────────┐
    │                                            │
    │     ┌──────────────────────────────┐       │
    │     │     lever swing zone         │       │  ← ~40mm above slot
    │     │     (clearance, no           │       │
    │     │      obstruction)            │       │
    │     ├──────────────────────────────┤       │
    │     │  ┌────────────────────────┐  │       │
    │     │  │                        │  │       │
    │     │  │   CARTRIDGE SLOT       │  │       │  80mm tall
    │     │  │   150mm x 80mm         │  │       │
    │     │  │                        │  │       │
    │     │  └────────────────────────┘  │       │
    │     │      180mm (dock width)      │       │
    │     └──────────────────────────────┘       │
    │                                            │
    └────────────────────────────────────────────┘
```

The cartridge slot is centered horizontally on the front face. The 150mm slot width within a 280mm enclosure leaves 65mm on each side — enough for the enclosure wall structure (4mm), guide rail housing (20mm on each side), and margin.

---

## 9. The Enclosure-in-Cabinet Question

Where does the enclosure box sit?

### Placement

The enclosure is a self-contained tower that sits on the cabinet floor, in one of the two usable zones flanking the P-trap/drain:

```
    TOP VIEW — enclosure in cabinet

    ┌────────────────────────────────────────────────────────────┐
    │                       BACK WALL                            │
    │                                                            │
    │        hot shutoff ●          ● cold shutoff               │
    │                    │          │                             │
    │                    │  drain   │                             │
    │                    │  pipe    │                             │
    │                    │    │     │                             │
    │   ┌─────────┐     │  ┌─┴─┐   │                             │
    │   │ENCLOSURE│     │  │P- │   │                             │
    │   │ 280x250 │     │  │trap│  │                             │
    │   │         │     │  │   │   │                             │
    │   │ front   │     │  └───┘   │                             │
    │   │ face →  │     │          │                             │
    │   └─────────┘     │          │                             │
    │                                                            │
    │                      FRONT OPENING                         │
    └────────────────────────────────────────────────────────────┘
```

The 280 x 250mm footprint fits comfortably in the 250-350mm wide side zones. The enclosure front face should be oriented toward the cabinet opening for cartridge and display access.

### Orientation

The enclosure front face must face the cabinet door opening. The back panel faces the cabinet back wall (or side wall). This gives direct access to:
- Cartridge slot (front face, mid-height)
- Displays (front face, upper area)
- Hopper (top of enclosure)

### Clearance Behind for Back Panel Connections

Per back-panel-and-routing.md, the back panel has 3 John Guest bulkhead fittings with 90-degree elbows, a barrel jack, an air switch grommet, and an optional USB port. With 90-degree elbows on the water fittings, the enclosure can sit as close as **60mm (2.4 inches)** from the cabinet wall.

In a 22" deep cabinet, the enclosure (250mm / 10" deep) plus 60mm (2.4") rear clearance uses ~310mm (12.2"), leaving ~250mm (10") of clearance in front. More than enough for the cartridge to slide out and for the user to operate the lever.

### Anti-Tip Considerations

The enclosure with two full 1L bags weighs approximately 7-8 kg (15-18 lbs), with the bags near the bottom (Zone C). This gives a low center of gravity — the tower is bottom-heavy when the bags are full. As bags empty, the center of gravity shifts upward, but the enclosure becomes lighter overall.

Rubber feet with a wide stance (placed at the corners of the 280 x 250mm base) provide adequate stability. For extra security, a single L-bracket screwed to the cabinet wall and hooked over the enclosure's back panel prevents the unit from tipping forward during cartridge insertion (when the user pushes the cartridge in, the force vector could tip the enclosure backward, but the wall is right there).

---

## 10. Open Questions

1. **Dock shelf as one piece or two?** The shelf floor and fitting wall could be one monolithic print (simpler assembly, better alignment) or two pieces joined with screws (easier to print, allows fitting wall replacement). The fitting wall takes the most wear (fitting insertion/removal cycles) and might benefit from being replaceable.

2. **Valve mounting**: Do solenoid valves mount directly to the underside of the dock shelf, or on a separate bracket below? Mounting to the shelf keeps tube runs short but adds weight and vibration sources to the alignment-critical shelf. A separate valve bracket below the shelf isolates valve mass from the dock.

3. **Shelf removability**: Should the dock shelf be removable for maintenance (accessing bags, replacing fittings)? If so, the electrical wiring and plumbing to the shelf need quick-disconnect points (JST connectors for wires, John Guest fittings for tubes).

4. **Cartridge detection**: A microswitch or hall sensor on the fitting wall can detect whether a cartridge is fully docked. This allows the ESP32 to disable pump drive when no cartridge is present, preventing dry-running the L298N into open pogo pins. GPIO for this sensor (and any future lever-position sensor) can come from the MCP23017 I/O expander -- GPIO exhaustion on the ESP32 is a solved problem.

---

## References

- [layout-spatial-planning.md](../../../enclosure/research/layout-spatial-planning.md) — Enclosure dimensions, zone layout, front-loading tower architecture
- [back-panel-and-routing.md](../../../enclosure/research/back-panel-and-routing.md) — Back panel connections, internal plumbing diagram, clearance behind enclosure
- [incline-bag-mounting.md](../../../enclosure/research/incline-bag-mounting.md) — 18-20 degree incline mount, two-point stretch, drainage analysis
- [bag-zone-geometry.md](../../bag-zone-geometry.md) — Bag zone dimensions, vertical space budget, incline stacking geometry
- [pump-assisted-filling.md](../../../enclosure/research/pump-assisted-filling.md) — Pump reversal fill method, topology correction, hopper connection
- [drip-tray-shelf-analysis.md](../../../enclosure/research/drip-tray-shelf-analysis.md) — Drip tray removal justification, electronics shelf redesignation
- [front-face-interaction-design.md](../../../enclosure/research/front-face-interaction-design.md) — Display mounting, cartridge slot on front face
- [cartridge-envelope.md](cartridge-envelope.md) — Cartridge dimensions, weight, pump arrangement
- [mating-face.md](mating-face.md) — Tube port layout, fitting spacing, release plate geometry
- [collet-release.md](collet-release.md) — John Guest collet mechanics, release plate bore dimensions
- [release-plate.md](release-plate.md) — Release plate design, cam-to-plate interface
- [cam-lever.md](cam-lever.md) — Eccentric cam mechanism, lever clearance requirements
- [electrical-mating.md](electrical-mating.md) — Pogo pin contacts, moisture separation strategy
- [guide-alignment.md](guide-alignment.md) — Guide rail design, FDM tolerances, alignment chain
- [pump-mounting.md](pump-mounting.md) — Vibration analysis, rubber grommet isolation, pump dimensions
- [under-cabinet-ergonomics.md](under-cabinet-ergonomics.md) — Cabinet anatomy, reach ergonomics
