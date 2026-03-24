# Mating Face Layout — Front-Loading Cartridge in Enclosure

The mating face is the interface between the cartridge and its dock. Every connection crosses this boundary: 4 tube ports, electrical contacts, guide/alignment features, and the release mechanism. This document defines the complete mating face geometry for a front-loading cartridge inside a self-contained enclosure tower.

This is the keystone layout decision. Once the mating face geometry is committed, it constrains the cartridge body dimensions, the dock cavity, the release plate shape, the cam lever placement, and the enclosure front face. Everything else flows from here.

**Context: this is NOT a standalone wall-mounted dock.** The dock is part of an enclosure — a self-contained tower (~250 x 200 x 450mm) that sits inside the under-sink cabinet. The enclosure has a layered internal layout: electronics on top, cartridge dock + valves in the middle, bags + plumbing on the bottom. The cartridge slides in from the front, like a CD drive in a PC tower. The enclosure's front face should look like a product — the cartridge face should be flush and color-matched when docked.

**Critical design principle: the release plate, cam, and lever are all part of the cartridge (the removable part).** The dock is a simple receptacle — fittings in a wall, alignment pins, pogo pins, and guide rails. The lever is on the front face of the cartridge (the face the user sees and grabs), doubling as an extraction handle (blade server pattern). The user operates the lever and removes the cartridge in one motion, from one location, with one hand.

**Established parameters from prior research (not re-derived here):**

| Parameter | Value | Source |
|---|---|---|
| Tube OD | 6.35mm (1/4") | collet-release.md |
| Fitting body OD | ~12.7mm | collet-release.md |
| Collet ring OD | ~11.4mm | collet-release.md |
| Release plate tube hole | 8.0mm | collet-release.md |
| Release plate inner lip | 10.5mm | collet-release.md |
| Release plate outer bore (cradle) | 12.5mm | collet-release.md |
| Plate travel | 3.0mm (min 2.5mm) | collet-release.md |
| Total actuation force (4 fittings) | 12-20N | collet-release.md |
| Plate thickness | 6.0mm (2mm cradle + 2mm lip + 2mm back) | release-plate.md |
| Electrical contacts | 3 pogo pins, dock side | electrical-mating.md |
| Contact pad size | ~8mm x 5mm each | electrical-mating.md |
| Electrical-to-water separation | 10-20mm minimum | electrical-mating.md |
| FDM sliding clearance | 0.3-0.5mm per side | guide-alignment.md |
| Alignment pins | 15-20 deg taper, 8-10mm base | guide-alignment.md |
| Cam eccentricity | 1-1.5mm for 2-3mm stroke | cam-lever.md |
| Pump dimensions (each) | 115.6 x 68.6 x 62.7mm | cartridge-envelope.md |
| Preferred pump arrangement | Side-by-side, motors same direction | cartridge-envelope.md |
| Target cartridge envelope | ~140 x 90 x 100mm (W x H x D) | cartridge-envelope.md |
| Fitting center-to-center spacing | 15mm (verified with parts in hand) | release-plate.md |
| Enclosure dimensions | ~250 x 200 x 450mm (W x D x H) | layout-spatial-planning.md |

**Confirmed facts (parts in hand):**

- 15mm center-to-center is fine for John Guest fittings — verified with physical parts
- Platypus bags collapse reliably as long as the hard inlet cap is rigidly secured in position
- Bag zone is 10-12" of height, not the 14"+ some research assumed
- FDC1004 capacitive sensing reliably detects liquid/air through BPT and silicone tubing
- Most internal tubing can be silicone; only transition points need hard tubing (for push-connect fittings)
- GPIO exhaustion is solved by I2C expander (MCP23017) — routine, not a design concern
- Gravity fill for the hopper does NOT work — pump-assisted filling is required

---

## 1. Enclosure Context — How the Dock Fits In

### The Tower Layout

The enclosure is a front-loading tower. The cartridge dock is one zone within it. The internal layout from top to bottom:

```
    FRONT VIEW (enclosure)              SIDE VIEW (enclosure)

    ┌───────────────────────┐          ┌──────────────────┐
    │  ╔═════HOPPER════╗    │          │  ╔═══HOPPER═══╗  │
    │  ║  (2 funnels)  ║    │          │  ║            ║  │
    │  ╚═══════════════╝    │          │  ╚════════════╝  │
    │                       │          │                  │
    │  [RP2040]   [S3]      │          │  ┌────────────┐  │
    │                       │          │  │  BAGS       │  │
    │  ┌─────────────────┐  │          │  │  (10-12")   │  │
    │  │ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │  │          │  └────────────┘  │
    │  │ ▓  CARTRIDGE  ▓ │  │          │  ┌────────────┐  │
    │  │ ▓  (flush)    ▓ ◄──┤ slide    │  │  DOCK      │  │
    │  │ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │  │ in/out   │  │  +CARTRIDGE│  │
    │  └─────────────────┘  │          │  └────────────┘  │
    │                       │          │  ┌────────────┐  │
    │  ┌─────────────────┐  │          │  │ ELECTRONICS│  │
    │  │  ELECTRONICS    │  │          │  │ VALVES     │  │
    │  │  + VALVES       │  │          │  └────────────┘  │
    │  └─────────────────┘  │          │                  │
    └───────────────────────┘          └──────────────────┘
          ~250mm wide                      ~200mm deep
```

The cartridge slot is the single largest feature on the front face. When the cartridge is fully inserted, its front face is flush with the enclosure surface and becomes part of the product's visual identity.

### What "Front-Loading" Means for the Mating Face

In the original standalone dock design, the mating face was the rear wall of the dock — the cartridge pushed in and the tube stubs engaged fittings in a vertical wall. With front-loading into an enclosure, the geometry is the same but the orientation and context change:

| Aspect | Standalone Dock (old) | Enclosure Front-Loading (current) |
|---|---|---|
| Insertion direction | Horizontal, from cabinet front | Horizontal, from enclosure front |
| Mating face orientation | Vertical wall at back of dock | Vertical wall at back of dock bay |
| Lever location | Top of cartridge | **Front face of cartridge** |
| Electrical contacts | Top of cartridge, dock ceiling | **Reconsidered (see Section 5)** |
| User's view during insertion | Looking at mating face end | Looking at cartridge front face |
| Aesthetic requirement | None (hidden under sink) | **Flush, color-matched, product finish** |

The critical change: the user never sees the mating face (the back wall with fittings). They only see the cartridge's front face (with lever/handle). The mating face design is purely functional. The front face design is functional AND aesthetic.

---

## 2. The Two Faces of the Cartridge

The cartridge has two important faces with distinct roles. Separating their concerns is central to the design.

### Rear Face (Mating Face) — Functional

This is the face that contacts the dock's back wall. It carries:

- 4 tube stubs (pass through the release plate into John Guest fittings)
- The release plate (slides along tube stubs)
- Release plate guide pins (fixed in cartridge body, constrain plate travel)
- Tapered pin sockets (receive dock alignment pins)

The user never directly interacts with or sees this face when the cartridge is docked.

### Front Face (User Face) — Functional + Aesthetic

This is the face the user sees and touches. It carries:

- The cam lever (release mechanism + extraction handle)
- A flush surface that becomes part of the enclosure front face when docked
- Visual cues (color matching, status, lever position indicator)

When docked, this face fills the cartridge slot opening in the enclosure. It should be flush with the surrounding enclosure panel to within ~1mm, color-matched (dark navy to match the enclosure), with the lever folded flat and recessed.

### Relationship Between the Two Faces

The two faces are connected through the cartridge body. The cam lever on the front face drives a push rod through the cartridge body to the release plate on the rear face. This ~100mm push rod (the cartridge depth) transmits the cam's 3mm stroke from front to back.

```
    Side cross-section of cartridge:

    FRONT FACE                                              REAR FACE
    (user sees)                                             (mating face)

    ┌────┐                                                  ┌────────┐
    │    │      push rod (~100mm)                           │  plate │
    │ ●──┤─────────────────────────────────────────────────┤──┤    ├═══► tube stubs
    │cam │                                                  │  plate │    → to fittings
    │    │      cartridge body interior                    │        │
    │lever│      (2 pumps, tubing, wiring)                  │        │
    └────┘                                                  └────────┘
    ↑                                                        ↑
    user operates                                           interfaces with
    from here                                               dock back wall
```

---

## 3. Rear Face (Mating Face) Layout

### Tube Port Arrangement: 2x2 Grid at 15mm C-C

The 2x2 grid remains the strongest arrangement. The compact symmetric footprint halves the maximum moment arm compared to any linear arrangement, directly addressing the plate tilt failure mode. 15mm center-to-center spacing has been verified with physical fittings in hand — the 2.3mm gap between fitting bodies is tight but workable.

```
    Rear face of cartridge (looking at the back):

    ┌──────────────────────────────────────────────────────────────┐
    │                                                              │
    │         ◇ taper socket              taper socket ◇           │
    │                                                              │
    │    ○ pin    O (P1-IN)     O (P2-IN)     pin ○               │
    │                                                              │
    │    ○ pin    O (P1-OUT)    O (P2-OUT)    pin ○               │
    │                                                              │
    │         ◇ taper socket              taper socket ◇           │
    │                                                              │
    └──────────────────────────────────────────────────────────────┘

    O  = tube stub through release plate stepped bore (15mm C-C)
    ○  = release plate guide pin (3mm steel dowel, fixed in cartridge body)
    ◇  = tapered pin socket (receives dock's alignment pin)
```

### Tube Port Arrangement Analysis

| Dimension | Value |
|---|---|
| Horizontal span (C-C) | 15.0mm |
| Vertical span (C-C) | 15.0mm |
| Min port zone width | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Min port zone height | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Release plate footprint | ~39.5 x 39.5mm (with guide pin margin) |
| Maximum moment arm (center to farthest bore) | 10.6mm |

The natural pairing groups each pump's inlet and outlet in one column:

```
    Pump 1 IN    Pump 2 IN
        O            O
        ↕ 15mm
    Pump 1 OUT   Pump 2 OUT
        O            O
        ←── 15mm ──→
```

### Release Plate Mechanics (Unchanged from Prior Research)

The release plate sits on the rear face between the cartridge body wall and the tube stub tips. It slides along 4 steel dowel pins (3mm diameter, press-fit into the cartridge body) and has 4 stepped bores matching the collet release tool geometry.

**Insertion (lever closed on front face, plate retracted):**

The plate sits flush against the cartridge body wall. Tube stubs protrude ~24mm past the wall — enough for 15mm fitting insertion + 6mm plate thickness + 3mm travel gap.

**Removal (lever opened on front face, plate extends toward dock):**

Opening the front-face lever drives the push rod, which pushes the plate forward along the tube stubs. The stepped bores engage the collet rings on the dock fittings, releasing all 4 tubes simultaneously. The cartridge slides out freely.

For full details on the release plate's stepped bore geometry (8.0/10.5/12.5mm concentric diameters), compliance strategy (overtravel), and guide features, see release-plate.md.

### Tapered Pin Sockets

Two or four tapered pin sockets on the cartridge rear face receive the dock's fixed alignment pins. Placed symmetrically about the tube port pattern, outside the release plate envelope, and as far apart as practical to maximize angular correction.

- **Pin base diameter:** 8-10mm with 15-20 degree per-side taper
- **Socket placement:** corners of a ~55-60mm square (10mm outside the plate on each side)
- **Correction authority:** a 10mm base pin corrects ~4mm of misalignment over the last 15mm of insertion — more than sufficient for the ~0.5mm residual error after rail guidance

### Mating Face Dimensions

| Feature | Width | Height |
|---|---|---|
| Tube port zone (2x2 at 15mm) | 33.5mm | 33.5mm |
| Release plate + guide pins | 39.5mm | 39.5mm |
| Taper pin sockets (outside plate) | +20mm | +20mm |
| Margin/walls | +6mm | +6mm |
| **Total rear face working area** | **~66mm** | **~66mm** |

This fits comfortably within the 140mm x 90mm cartridge cross-section, leaving ample room for the cartridge shell walls, rail features, and internal structure.

---

## 4. Front Face (User Face) — Lever as Handle

### Design Principle

The lever on the front face serves two purposes: cam-driven release mechanism AND extraction handle. When the user wants to remove the cartridge, they do one thing from one place: flip the lever, grip it, pull.

This follows the blade server ejector pattern (Southco inject/eject mechanisms) where the lever on the front panel of each blade both seats/unseats the blade and provides a grip surface.

### Lever Orientation: Horizontal Swing

For a front-loading cartridge in a vertical enclosure slot, the lever swings horizontally in the plane of the front face:

```
    LOCKED (lever folded flat, cartridge docked):

    ┌──────────────────────────────────────┐
    │                                      │
    │      (cartridge front face)          │
    │      (flush with enclosure)          │
    │                                      │
    │  ═══════════════════╗ ← lever        │   90mm
    │                     ●  folded flat   │
    │                                      │
    └──────────────────────────────────────┘
                   140mm

    UNLOCKED (lever swung 180°, ready to pull):

    ┌──────────────────────────────────────┐
    │                                      │
    │                                      │
    │                                      │
    ╔═════════════════════●                │
    ║  lever extends      pivot            │
    ║  ~80mm to the left                   │
    ║                                      │
    └──────────────────────────────────────┘
```

When locked, the lever lies flat against the bottom portion of the cartridge front face, within the slot opening. The lever surface is flush with the cartridge face (recessed ~1mm). When unlocked, the lever swings out to the left (or right), protruding from the slot. The user grips the protruding lever and pulls the entire cartridge out.

### Cam-to-Plate Force Transmission

The cam pivot is at the front face. The release plate is ~100mm away at the rear face. A rigid push rod connects them through the cartridge body.

```
    Side cross-section (lever closed / locked):

    FRONT FACE              INTERIOR                    REAR FACE

    ┌──────┐                                           ┌────────┐
    │      │                                           │  plate │
    │  ●   │── push rod (rigid, ~100mm) ──────────────│──┤    │
    │ cam  │     runs alongside pumps                  │  plate │
    │(flat)│                                           │        │
    └──────┘                                           └────────┘

    Side cross-section (lever open / unlocked):

    ┌──────┐                                           ┌────────┐
    │      │                                           │ plate  │
    │  ●   │── push rod extended 3mm ─────────────────├──┤    │→→
    │ cam  │     cam rotated, pushes rod forward       │ plate  │→→
    │(open)│                                           │ pushed │
    └──────┘                                           └────────┘
```

**Push rod design:**
- Length: ~95-100mm (cartridge depth minus wall thicknesses)
- Cross-section: 4mm diameter PETG rod or 3mm x 15mm flat yoke
- Deflection under 20N: negligible (0.003mm for a 4mm round rod, even less for a flat yoke)
- The rod runs through the cartridge interior alongside the pumps, in a printed channel or clip

**Force at the lever handle:**

```
    Input force = (actuation force x cam radius) / lever length
                = (20N x 5mm) / 80mm
                = 1.25N (~0.3 lbf)
```

This is extremely light. The lever's primary purpose is tactile feedback and over-center locking, not force multiplication. The over-center cam position locks the lever flat, preventing accidental release.

### Lever Detailing for Flush Appearance

When locked, the lever must not protrude from the cartridge front face:

- **Lever thickness:** 4-5mm, recessed into a shallow pocket in the cartridge front face
- **Lever surface:** Flat, color-matched to the cartridge face (dark navy)
- **Lever edge detail:** A thin reveal line (~0.5mm gap) around the lever indicates its presence without disrupting the flush surface
- **Finger pull:** A small recess or chamfer at the lever's free end lets the user get a fingertip underneath to start the swing

When the cartridge is docked and locked, the front face reads as a nearly solid panel with a subtle lever outline — similar to a laptop's flush-fit port cover or a car's concealed door handle.

---

## 5. Electrical Contact Placement

### The Front-Loading Constraint

In the original design, electrical pads were on the cartridge's top face and pogo pins were on the dock ceiling. This worked well for a top-accessible lever with a clear top face. With the lever now on the front face, the top face is free for electrical contacts — but the geometry of a front-loading slot inside an enclosure changes the picture.

The dock bay inside the enclosure has a ceiling, floor, two side walls, and a back wall (with fittings). All are potential pogo pin locations.

### Option 1: Top Face of Cartridge (Recommended)

Pogo pins in the dock ceiling, contact pads on the cartridge top face. This remains the strongest option, and the move of the lever to the front face actually makes it better — the top face is now entirely free.

```
    Cross-section looking from the front (cartridge in dock):

    ┌─────────────────────────────────────────────────────┐
    │                DOCK CEILING                          │
    │         ↓ pogo  ↓ pogo  ↓ pogo                      │
    │      ┌──────────────────────────────────┐           │
    │      │  [=]     [=]     [=]  ← pads    │           │
    │      │         cartridge top face       │           │
    │      │                                  │           │
    │      │       (interior: pumps)          │           │
    │      │                                  │           │
    │      └──────────────────────────────────┘           │
    │                DOCK FLOOR                            │
    └─────────────────────────────────────────────────────┘
```

3 flat brass or nickel-plated pads on the cartridge top face, elongated in the insertion direction (10mm x 5mm) to provide wipe action during slide-in.

**Advantages:**
- Complete physical separation from water fittings (different face entirely)
- Water drips down, away from contacts above
- The dock ceiling is dry — no fittings, no O-rings, no potential leaks
- Top face is now fully available (lever moved to front face)
- Pogo pin alignment tolerance is generous (8mm pad for 2mm pin tolerates 2-3mm misalignment)

**Pogo pin orientation:** With the cartridge sliding in horizontally, the pogo pins press downward from the dock ceiling onto the cartridge top face. The spring force presses the pads against the cartridge, and the cartridge's own weight plus the fitting retention help maintain contact pressure.

### Option 2: Rear Mating Face, Above Tube Stubs

Place electrical pads on the rear face above the tube port zone, with pogo pins on the dock back wall above the fittings. A raised dam separates electrical and fluid zones.

**Advantages:**
- All connections on one face — single dock wall handles everything
- Simpler dock structure (no ceiling-mounted pins)

**Disadvantages:**
- Moisture risk from fitting connections (splash, condensation, drip during insertion/removal)
- The dam adds height to an already dense zone
- If a fitting leaks, electrical contacts are directly above

### Recommendation

**Option 1 (top face) is strongly preferred.** The complete physical separation eliminates an entire failure domain. The guide rails position the cartridge vertically within 0.5mm — well within the pogo pin's 2-3mm tolerance window.

---

## 6. Guide Feature Integration

### Two-Stage Alignment

Following the universal prior art pattern (server blades, battery packs, printer cartridges), the cartridge uses two-stage alignment:

1. **Coarse guidance (rails):** Guide rails on the cartridge sides engage channels in the dock side walls, constraining the cartridge to slide along a single axis. Clearance: 0.3-0.5mm per side.

2. **Fine alignment (tapered pins):** Two or four tapered pins on the dock back wall enter conical sockets on the cartridge rear face during the last 15-20mm of insertion. Pins correct ~4mm of residual misalignment down to <0.5mm.

```
    Cross-section looking from the front (cartridge in dock):

    DOCK SIDE WALL     CARTRIDGE              DOCK SIDE WALL
    ┌─────┐    ┌──────────────────────┐    ┌─────┐
    │     │    │                      │    │     │
    │  ┌──┤    ├──┐              ┌──┤    ├──┐  │
    │  │  │    │  │   (interior) │  │    │  │  │
    │  │  │    │  │              │  │    │  │  │
    │  └──┤    ├──┘              └──┤    ├──┘  │
    │     │    │                      │    │     │
    └─────┘    └──────────────────────┘    └─────┘
          rail channels          rail features on cartridge
```

### Rail Design for Front-Loading

The rails run the full ~100mm depth of the cartridge (the insertion travel). For a front-loading cartridge in an enclosure, the dock guide channels are printed as part of the enclosure's internal structure — integral ribs on the dock bay's side walls.

**Rail profile:** Rectangular or dovetail cross-section. A dovetail provides anti-lift constraint (preventing the cartridge from lifting out of the dock bay) in addition to lateral guidance. For FDM printing, a 55-degree dovetail with 0.3-0.5mm per-side clearance provides smooth sliding with anti-lift.

**Funnel entrance:** The dock bay opening (the enclosure's front face slot) has a 5mm chamfer on all four edges. This creates a ~10mm capture zone that funnels the cartridge from a sloppy initial aim into the guide rails. The chamfer also serves as a visual frame for the cartridge slot (see front-face-interaction-design.md).

### Poka-Yoke (Mistake-Proofing)

The cartridge should only fit one way. An asymmetric rail profile or a keying tab on one side prevents upside-down or left-right reversed insertion. With the lever on the front face at a specific location (e.g., bottom-right), the asymmetry is naturally established — the lever won't clear the slot if the cartridge is rotated.

---

## 7. Dock-Side Design — Passive Simplicity

### The Dock Back Wall

With the release plate, cam, lever, and push rod all on the cartridge, the dock wall is a flat panel with only fixed components:

```
    Dock back wall (rear of dock bay, looking from inside):

    ┌──────────────────────────────────────────────────────┐
    │                                                      │
    │  ▲ taper pin                        taper pin ▲      │
    │                                                      │
    │            ● fitting    ● fitting                     │
    │                                                      │
    │            ● fitting    ● fitting                     │
    │                                                      │
    │  ▲ taper pin                        taper pin ▲      │
    │                                                      │
    └──────────────────────────────────────────────────────┘

    ▲ = tapered alignment pin (fixed, dock side)
    ● = John Guest fitting (bulkhead mount, dock side)
```

### Dock Components (Complete List)

| Component | Location | Function |
|---|---|---|
| 4x John Guest fittings | Back wall (2x2 grid, 15mm C-C) | Fluid connection + passive tube retention |
| 2-4x tapered alignment pins | Back wall, outside fitting pattern | Fine alignment at end of insertion travel |
| 3x pogo pins | Ceiling of dock bay | Electrical contact to cartridge top pads |
| Guide rail channels | Side walls (full ~100mm depth) | Coarse guidance during insertion |
| Chamfered entrance | Front opening of dock bay | Captures cartridge from sloppy aim |
| Back wall/stop | Rear of dock bay | Limits insertion depth |

### What the Dock Does NOT Have

- No release plate
- No cam mechanism
- No lever
- No moving parts of any kind (except pogo pin springs)

The dock is entirely passive. All wear items (release plate, cam, guide pin sockets) are on the replaceable cartridge. The only components that contact the cartridge are spring-loaded pogo pins (100,000+ cycle life), static tapered pins, static rail channels, and John Guest fittings (designed for repeated use).

### Fitting Mounting in the Dock Wall

**Bulkhead/panel-mount fittings (preferred):** Male-threaded body passes through a panel hole, locknut on back. Thread size for 1/4" tube: 1/4" NPTF or 3/8" UNF. Provides rigid, repeatable positioning.

**Fitting spacing in the dock wall (2x2 at 15mm C-C):**

```
    ●────15mm────●
    │            │
   15mm         15mm
    │            │
    ●────15mm────●

    Fitting body OD: 12.7mm
    Gap between bodies: 15.0 - 12.7 = 2.3mm
    Status: verified workable with parts in hand
```

**Dock wall thickness:** 5-6mm for PETG with bulkhead fittings. The locknut engages threads on the back side, clamping the fitting against the wall face.

### Dock Integration with Enclosure

The dock back wall is either:
1. A separate printed panel bolted inside the enclosure, or
2. An integral wall of the enclosure's internal structure

Option 1 is preferred for prototyping — the dock wall can be iterated independently of the enclosure shell. The fittings mount through the panel, with tubes routing out the back to the solenoid valves and bags below.

The dock bay's side walls, ceiling, and floor are structural ribs of the enclosure. The ceiling carries the pogo pins. The side walls carry the guide rail channels. The floor supports the cartridge's weight (~940g).

---

## 8. Complete Cartridge Cross-Section

### Side View (Cartridge Sliding In)

```
    ← FRONT (user)                                    REAR (dock wall) →

    ┌─ front face ─┐                            ┌── rear face ──┐
    │              │                            │              │
    │  ●cam       │    push rod (~95mm)        │  release     │
    │  lever      │── ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ──│  plate       │
    │  (flush)    │                            │              │
    │              │    PUMP 1     PUMP 2       │  ════════► tube stubs
    │              │    ┌─────┐   ┌─────┐      │  ════════► (into fittings)
    │              │    │motor│   │motor│      │  ════════►
    │              │    └─────┘   └─────┘      │  ════════►
    │              │                            │              │
    │   (pads on  │    wiring, tubing          │  ◇ taper     │
    │    top face)│                            │    sockets   │
    └──────────────┘                            └──────────────┘

    ├── ~5mm ──┤  ├── ~90mm interior ──────────┤  ├── ~5mm ──┤
                               ~100mm total depth
```

### Tube Stub Length

The tube stubs must span from inside the cartridge body through the rear wall, through the release plate, across the travel gap, and into the fittings:

| Segment | Length |
|---|---|
| Cartridge body wall | ~3mm |
| Plate thickness (when retracted, plate flush with wall) | 6mm |
| Plate travel gap | 3mm |
| Fitting insertion depth | 15mm |
| Safety margin | 2-3mm |
| **Total stub length from inside wall** | **~29-30mm** |

When the plate is retracted, stubs protrude ~24mm past the cartridge body wall. When the plate extends 3mm forward, stubs still protrude ~21mm past the plate face — fully seated in the fittings.

---

## 9. Cartridge Front Face in the Enclosure

### Flush Integration

When the cartridge is fully inserted and locked, the front face fills the cartridge slot opening. The enclosure's front panel has a ~146 x 96mm slot (cartridge + rail clearance) with a 5mm chamfer. The cartridge front face sits within this opening, recessed by ~1mm from the enclosure surface.

```
    Enclosure front face detail (cartridge docked):

    ┌──────────────────────────────────────────────┐
    │                                              │
    │        ○ RP2040 display                      │
    │                                              │
    │        ○ S3 display                          │
    │                                              │
    │  ┌──╱──────────────────────────────╲──┐      │
    │  │ ╱   5mm chamfer                  ╲ │      │
    │  │┌────────────────────────────────┐ │      │
    │  ││                                │ │      │
    │  ││    CARTRIDGE FRONT FACE        │ │      │  90mm
    │  ││    (dark navy, flush)          │ │      │
    │  ││                                │ │      │
    │  ││  ═══════════════╗  lever       │ │      │
    │  ││                 ●  (recessed)  │ │      │
    │  │└────────────────────────────────┘ │      │
    │  │ ╲                                ╱ │      │
    │  └──╲──────────────────────────────╱──┘      │
    │                                              │
    │        [STATUS LED]                          │
    │                                              │
    └──────────────────────────────────────────────┘
```

### Aesthetic Details

- **Color:** Dark navy (#1a1a2e) matching the enclosure, S3 display theme, and iOS app
- **Surface finish:** Smooth printed surface, sanded or spray-painted for consistency
- **Chamfer accent:** The 5mm chamfer around the slot could be a contrasting matte silver, or a thin bright accent line (matching the app's accent color)
- **Lever reveal:** A 0.5mm gap around the lever edge indicates its presence. A small finger recess at the free end allows the user to start the swing
- **Status LED:** Above the cartridge slot, indicating cartridge state (green = present and locked, amber = present but unlocked, off = absent)

---

## 10. Tube Routing Behind the Dock Wall

With the dock wall inside the enclosure, the tubes emerging from the back of the John Guest fittings route to other components:

```
    Side view inside enclosure (behind dock wall):

    DOCK WALL                      ENCLOSURE INTERIOR
    (fittings)

    ● P1-IN ──── silicone tube ──── solenoid valve 1 ──── bag 1 outlet
    ● P1-OUT ─── silicone tube ──── tee/solenoid ──── to dispensing line
    ● P2-IN ──── silicone tube ──── solenoid valve 2 ──── bag 2 outlet
    ● P2-OUT ─── silicone tube ──── tee/solenoid ──── to dispensing line
```

The tubes route vertically downward from the dock wall to the solenoid valves in the electronics/valves zone below, and vertically upward to the bag outlets in the bag zone above. All internal tubing can be silicone except at the push-connect fitting interfaces, which require hard 1/4" OD tubing stubs.

The dock wall is a vertical partition inside the enclosure. Its back side faces downward (toward the lower electronics zone) and upward (toward the bag zone), making gravity-assisted tube routing natural.

---

## 11. Interdependencies Summary

| Dependency | Relationship |
|---|---|
| Release plate bore pattern ↔ dock fitting positions ↔ tube stub positions | All locked to 15mm C-C, 2x2 grid |
| Front-face cam ↔ rear-face release plate | Connected by ~100mm push rod through cartridge body |
| Electrical pads (cartridge top) ↔ pogo pins (dock ceiling) | Dock ceiling height = cartridge height + pogo pin compression (~1-2mm) |
| Guide rails (cartridge sides) ↔ dock channels | 0.3-0.5mm per side clearance, full 100mm depth |
| Tapered pins (dock wall) ↔ tapered sockets (cartridge rear) | Pins outside release plate envelope, ~55-60mm square spacing |
| Cartridge front face ↔ enclosure slot opening | 146 x 96mm slot, 5mm chamfer, flush fit |
| Lever swing ↔ enclosure slot width | Lever folds flat within the 140mm cartridge width |
| Push rod channel ↔ pump placement | Rod routes alongside or between the two pumps |

---

## 12. Recommendation Summary

### Rear Face (Mating Face)

| Decision | Choice | Rationale |
|---|---|---|
| Tube arrangement | 2x2 grid, 15mm C-C | Verified fit with parts. Most compact, best tilt resistance, symmetric cam loading. |
| Release plate | On cartridge, 4 stepped bores, 6mm thick | Follows all prior art. One-handed operation. Wear parts on replaceable cartridge. |
| Guide pins for plate | 4x 3mm steel dowels in cartridge body | Constrains plate to pure axial travel. |
| Tapered sockets | 4 conical sockets at ~55-60mm square | Receives dock alignment pins for <0.5mm final positioning. |
| Fitting mounting (dock) | Bulkhead panel-mount, 15mm C-C | Rigid, repeatable. Locknut on back of dock wall. |

### Front Face (User Face)

| Decision | Choice | Rationale |
|---|---|---|
| Lever placement | Front face, horizontal swing | Doubles as extraction handle. One location, one action. Blade server pattern. |
| Lever style | Eccentric cam, 1.5mm eccentricity, ~80mm handle | Over-center locking. ~0.3 lbf input force. Folds flat when locked. |
| Force transmission | Rigid push rod, ~100mm | Connects front cam to rear release plate. Negligible deflection. |
| Flush appearance | Recessed lever, 0.5mm reveal, color-matched | Cartridge face becomes part of enclosure aesthetic. |

### Electrical Contacts

| Decision | Choice | Rationale |
|---|---|---|
| Pad location | Cartridge top face | Complete moisture separation from water fittings. |
| Pin location | Dock ceiling | Presses downward, gravity-aided contact maintenance. |
| Pad size | 8mm x 5mm, elongated in insertion direction | Provides wipe action. Tolerates 2-3mm misalignment. |

### Overall Dimensions

| Measurement | Value |
|---|---|
| Cartridge cross-section | 140 x 90mm (W x H) |
| Cartridge depth | ~100mm |
| Rear mating face working area | ~66 x 66mm |
| Enclosure slot opening | ~146 x 96mm (with rail clearance) |
| Enclosure slot with chamfer | ~156 x 106mm (visual frame) |

---

## 13. Open Questions

1. **Push rod routing through cartridge interior:** The pumps occupy most of the ~140 x 90mm cross-section. The push rod needs a channel between or alongside the pumps. The side-by-side pump arrangement (Arrangement A from cartridge-envelope.md) leaves a gap between the two pump bodies — the push rod should route through this gap. Verify clearance with pump dimensions in hand.

2. **Lever pivot hardware:** A 3mm steel pin through the cartridge body wall serves as the cam pivot. The lever and cam are one piece (or two pieces pinned together). For 3D printing, a separate printed lever with a press-fit steel pivot pin is most robust. The cam face should be smooth (sanded or printed on a flat surface) for consistent over-center feel.

3. **Push rod-to-plate interface:** The rod pushes on the release plate center. A flat-on-flat contact is simplest. A small printed socket on the plate and a matching nub on the rod end prevents lateral slipping. Alternatively, the rod terminates in a wide foot (15mm disc) that distributes force across the plate center.

4. **Cartridge front face construction:** The front face must be smooth and color-matched. For FDM printing, the front face should be the build surface (printed face-down on the bed) for the smoothest finish. Alternatively, sand and spray-paint. The lever pocket must be printed accurately for flush fit.

5. **Dock ceiling pogo pin mounting:** The 3 pogo pins must be positioned precisely in the dock ceiling. A small PCB (or 3D printed pocket) holds the pins. The PCB approach is cleaner: a simple board with 3 through-hole pogo pins, mounted to the dock ceiling with standoffs. Wiring runs from the PCB to the enclosure electronics zone above.

---

## Sources

- collet-release.md (this project) — bore dimensions, forces, failure modes
- release-plate.md (this project) — stepped bore geometry, spacing, guide features, compliance
- cam-lever.md (this project) — eccentric cam mechanics, over-center behavior, prior art
- electrical-mating.md (this project) — pogo pin recommendations, moisture separation
- guide-alignment.md (this project) — rail clearances, tapered pin geometry, FDM tolerances
- cartridge-envelope.md (this project) — pump dimensions, arrangements, bounding volumes
- pump-mounting.md (this project) — mounting features, tube exit points, wire routing
- under-cabinet-ergonomics.md (this project) — reach distances, cabinet anatomy
- release-mechanism-alternatives.md (this project) — full solution space exploration
- dock-mounting-strategies.md (this project) — mounting location analysis
- cartridge-change-workflow.md (this project) — end-to-end user experience
- layout-spatial-planning.md (this project) — enclosure tower layout, component inventory
- hopper-and-bag-management.md (this project) — bag dimensions, pump-assisted filling
- front-face-interaction-design.md (this project) — display holders, cartridge slot aesthetics, lever on front face
- back-panel-and-routing.md (this project) — external connections, internal routing
- [Southco Inject/Eject Mechanisms](https://southco.com/en_us_int/fasteners/inject-eject-mechanisms) — blade server lever prior art
- [Bicycle Quick-Release Mechanisms (Sheldon Brown)](https://sheldonbrown.com/skewers.html) — eccentric cam prior art
- [John Guest OD Tube Fittings Technical Specifications](https://www.johnguest.com/sites/default/files/files/tech-spec-od-fittings-v2.pdf) — fitting dimensions
