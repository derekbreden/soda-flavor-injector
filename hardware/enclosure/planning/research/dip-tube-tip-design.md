# Dip Tube Tip Piece — 3D Printed Air Collection Bar

Design document for the 3D-printed tip piece that sits at the top of each bag's dip tube. This is the only fluid-contact 3D-printed part in the entire system. It spans the full width of the Platypus 2L bag interior, collects migrating air, and channels it to a central socket where the 1/4" hard dip tube inserts.

---

## Table of Contents

1. [Function and Context](#1-function-and-context)
2. [Ship-in-a-Bottle Assembly](#2-ship-in-a-bottle-assembly)
3. [Cross-Section Constraint](#3-cross-section-constraint)
4. [Overall Dimensions](#4-overall-dimensions)
5. [Central Tube Socket Design](#5-central-tube-socket-design)
6. [Air Channel Features](#6-air-channel-features)
7. [Structural Analysis](#7-structural-analysis)
8. [Material Selection](#8-material-selection)
9. [Manufacturing Options and Cost](#9-manufacturing-options-and-cost)
10. [Recommended Design](#10-recommended-design)

---

## 1. Function and Context

### 1a. Where It Lives

The tip piece sits inside a sealed Platypus 2L bag at the highest point -- the sealed end of the bag, which is pinned against the back wall at the top of the 35-degree diagonal mount. The bag's connector end (28mm threaded cap with two John Guest bulkhead fittings) is at the bottom-front. A 1/4" (6.35mm OD) hard tube runs from one of the bulkhead fittings up through the bag interior to this tip piece.

```
    SIDE VIEW: Bag on 35-degree diagonal, tip piece at top

    back wall
    │
    │   ┌── tip piece (spans bag width)
    │   │
    │  [═══════════════════════]  ← sealed end, pinned to wall
    │  ╱                       ╲
    │ ╱   bag interior          ╲
    │╱    (flavor concentrate)   ╲
    │     │                       ╲
    │     │ 1/4" hard dip tube     ╲
    │     │ (air bleed line)        ╲
    │     │                          ╲
    │     │                           ╲
         ╚═══╤═══╤════════════════════╝
              │cap│  ← two JG bulkhead fittings
              └─┬─┘     (bottom port + dip tube port)
                │
           bottom-front
```

### 1b. What It Does

1. **Air collection**: Air rises to the highest point in the bag. The tip piece sits at that apex. Its surface features (ribs, grooves) prevent the bag film from sealing flat against the bar, creating channels for air to migrate laterally from anywhere across the bag width toward the central tube socket.

2. **Structural span**: At ~185mm, it wedges between the bag's side seams, preventing the part from sliding sideways or rotating once installed. The bag's heat-sealed side seams act as natural stops.

3. **Tube retention**: The central socket grips the 6.35mm OD hard tube, holding the dip tube assembly in position without adhesive or external fasteners.

### 1c. Operating Environment

| Parameter | Value |
|---|---|
| Fluid | Flavor concentrate syrup (sugar-based, acidic) |
| pH | ~3-4 (citric/phosphoric acid) |
| Temperature | 15-25C (ambient under-sink) |
| Pressure | Near-atmospheric; slight vacuum during prime cycle |
| Lifespan target | Permanent installation, multi-year |
| Mechanical loads | Bag film contact pressure only (very light) |
| Cleaning | Occasional water flush through the dip tube line |

---

## 2. Ship-in-a-Bottle Assembly

The tip piece must pass through the 28mm threaded cap opening to get inside the bag, then rotate 90 degrees to span the full bag width. This is the fundamental assembly constraint.

### 2a. Assembly Sequence

```
Step 1: Insert tip piece lengthwise through 28mm opening

         28mm opening
         ┌────┐
    ═════╡    ╞═══════  ← tip piece (185mm long)
         └────┘          slides through lengthwise
                         (cross-section fits within 28mm circle)


Step 2: Rotate 90 degrees inside the bag

    Before rotation:          After rotation:
    ┌────────────────┐        ┌────────────────┐
    │                │        │                │
    │    ═══(long)   │        │  ─── (short)   │
    │                │        │  │           │  │
    │                │        │  ═══════════ │  │
    │                │        │  │           │  │
    │                │        │  ─── (short)   │
    └────────────────┘        └────────────────┘
     tip piece aligned         tip piece spans
     with bag length           bag width


Step 3: Feed dip tube through bulkhead, up to tip piece

    The 1/4" hard tube feeds through the dip tube port
    bulkhead in the cap, runs up the bag interior, and
    inserts into the central socket on the tip piece.
    Socket geometry grips the tube.


Step 4: Screw cap onto bag

    The cap with both bulkhead fittings screws onto the
    28mm bag thread. The dip tube is now tensioned between
    the bulkhead (at cap) and the tip piece (at sealed end).
    Everything is locked in place.
```

### 2b. Handling During Assembly

The user reaches into the bag through the 28mm opening (tight but possible with two fingers) or uses a long tool (chopstick, rod) to push and rotate the tip piece. This is a one-time operation per bag. It does not need to be fast or easy -- just possible.

---

## 3. Cross-Section Constraint

The tip piece cross-section must fit through a 28mm circle when the part is oriented along its length axis. The governing constraint is the diagonal of the cross-section rectangle.

### 3a. Diagonal Formula

For a rectangular cross-section W x H:

    diagonal = sqrt(W^2 + H^2)

This diagonal must be <= 28mm (with tolerance, target <= 27.5mm for clearance).

### 3b. Candidate Cross-Sections

```
Option A: 20mm x 15mm (diagonal = 25.0mm)  <-- conservative
    ┌────────────────────┐
    │                    │  15mm
    └────────────────────┘
           20mm

    Clearance: 3mm to 28mm circle
    Pro: Easy insertion, generous clearance
    Con: Less surface area for air channels


Option B: 24mm x 12mm (diagonal = 26.8mm)  <-- wider, flatter
    ┌────────────────────────────┐
    │                            │  12mm
    └────────────────────────────┘
              24mm

    Clearance: 1.2mm to 28mm circle
    Pro: Wide profile = more air channel area
    Con: Tighter insertion, less structural depth


Option C: 22mm x 14mm (diagonal = 26.1mm)  <-- balanced
    ┌──────────────────────────┐
    │                          │  14mm
    └──────────────────────────┘
             22mm

    Clearance: 1.9mm to 28mm circle
    Pro: Good balance of width and depth
    Con: Moderate on both dimensions


Option D: 25mm x 12mm (diagonal = 27.7mm)  <-- maximum width
    ┌─────────────────────────────┐
    │                             │  12mm
    └─────────────────────────────┘
               25mm

    Clearance: 0.3mm to 28mm circle
    Pro: Maximum air channel surface area
    Con: Very tight insertion; FDM tolerance may not allow this
```

### 3c. Cross-Section Within the 28mm Circle

```
           28mm circle
          ╱          ╲
        ╱              ╲
       │  ┌──────────┐  │
       │  │  Option C │  │   22 x 14mm cross-section
       │  │  22 x 14  │  │   inside 28mm circle
       │  └──────────┘  │
        ╲              ╱
          ╲          ╱

    The rectangle's corners must all fall within
    or on the 28mm diameter circle.
```

### 3d. Recommended Cross-Section

**Option C: 22mm x 14mm** is the recommended starting point.

- 1.9mm clearance to the 28mm circle accommodates FDM and SLS tolerances
- 14mm depth provides adequate structural section for the 185mm span
- 22mm width gives sufficient top/bottom surface area for air channel features
- The tube socket (6.35mm bore) fits comfortably within the 14mm depth

---

## 4. Overall Dimensions

### 4a. Length

The Platypus 2L bag is ~190mm wide when flat (measured seam-to-seam). The tip piece should be slightly shorter than the full internal width so it can be rotated into position and then wedged between the seams.

| Dimension | Value | Rationale |
|---|---|---|
| Bag external width | ~190mm | Platypus 2L spec (7.5 inches) |
| Side seam width (each) | ~3-4mm | Heat-sealed PE/nylon laminate seam |
| Bag internal clear width | ~182-184mm | External minus two seam widths |
| Tip piece length | **185mm** | Slightly longer than clear width; the flexible bag stretches to accommodate, and the wedge fit prevents sliding |

The 185mm length means the tip piece presses lightly into the side seams. The bag film is flexible enough to accommodate this, and the resulting friction holds the tip piece in its lateral position.

### 4b. Summary Dimensions

```
    TOP VIEW (looking down at tip piece):

    ├──────────── 185mm ────────────┤
    ┌───────────────────────────────┐
    │              ◉                │  ← central tube socket
    └───────────────────────────────┘
    ├── 22mm ──┤


    SIDE VIEW (cross-section, exaggerated):

    ┌──────────────────────────────┐
    │  ╱╲  ╱╲  ╱╲  ◉  ╱╲  ╱╲  ╱╲│  ← air channel ribs (top)
    │  ╲╱  ╲╱  ╲╱     ╲╱  ╲╱  ╲╱│
    └──────────────────────────────┘
       14mm depth

    END VIEW (cross-section):

         22mm
    ┌────────────┐
    │ ╱╲  ◉  ╱╲ │  14mm
    │ ╲╱     ╲╱  │
    └────────────┘
```

---

## 5. Central Tube Socket Design

The socket must grip a 6.35mm OD hard polyethylene or polyurethane tube firmly enough that the tube stays inserted during normal handling, but allow intentional removal if the user pulls hard.

### 5a. Grip Options

#### Option 1: Interference / Friction Fit

```
    CROSS-SECTION: Friction fit socket

    ┌─────────────────┐
    │                 │
    │   ┌───────┐    │
    │   │ 6.30  │    │  ← bore slightly under 6.35mm
    │   │  mm   │    │     (0.05mm interference)
    │   │ bore  │    │
    │   └───────┘    │
    │                 │
    └─────────────────┘
```

- Bore diameter: 6.25-6.30mm (0.05-0.10mm interference on 6.35mm tube)
- Depth: 15-20mm for adequate grip area
- Pro: Simplest to manufacture; works with any process
- Con: Grip force depends heavily on dimensional accuracy; may loosen over time as plastic creeps; temperature changes affect fit
- Best for: SLA or SLS where tolerances are tight (+/- 0.05mm)

#### Option 2: Internal Barb Ridge

```
    CROSS-SECTION: Barb ridge socket (longitudinal section)

                   tube insertion direction
                          ↓
    ┌─────────┬───────────────────────┐
    │         │                       │
    │    ╲    │  6.50mm              │
    │     ╲   │  (relief)    6.35mm  │  ← tube snaps past ridge
    │  barb╲──│─────────     bore    │
    │     ╱   │                      │
    │    ╱    │                      │
    │         │                       │
    └─────────┴───────────────────────┘
              ↑
         ridge: 6.15mm ID
         (0.20mm interference)
```

- A circumferential ridge or series of bumps at the socket entrance
- Ridge ID: ~6.15mm (the tube compresses the ridge as it passes, then the tube's OD springs back to 6.35mm behind the ridge)
- Pro: Positive click-past retention; resists pullout even if bore is slightly oversized
- Con: Requires the hard tube to have enough rigidity to push past the ridge; harder to print reliably at FDM resolution
- Best for: SLS or SLA where fine features hold up

#### Option 3: Collet-Style Split Grip

```
    END VIEW: Split socket with flex fingers

         ┌──╱──┐
        ╱  │    ╲
       │   │     │    Four flex fingers
       │  6.2mm  │    grip the tube
       │   │     │    concentrically
        ╲  │    ╱
         └──╲──┘

    The socket has 4 longitudinal slits creating
    flex fingers. The fingers deflect outward as
    the tube inserts, then spring back to grip.
```

- Socket has 3-4 longitudinal slits (1mm wide, 12mm deep) creating flexible fingers
- Relaxed bore: ~6.20mm; fingers flex to ~6.50mm during insertion
- Pro: Self-centering; accommodates tube OD variation; visible grip confirmation
- Con: Complex geometry; fingers may be fragile in FDM; requires adequate wall thickness around the slits
- Best for: SLA or SLS; marginal for FDM due to slit width limitations

#### Option 4: Tapered Socket with Detent

```
    LONGITUDINAL SECTION: Tapered socket

    ┌──────────────────────────────────┐
    │                                  │
    │   ╲  7.0mm   6.35mm    6.35mm   │
    │    ╲  entry   ╲_detent   bore   │
    │    ╱          ╱                  │
    │   ╱                              │
    │                                  │
    └──────────────────────────────────┘
         ↑           ↑
      lead-in     snap groove
      funnel      (6.15mm)
```

- Tapered entry (7mm to 6.35mm) guides the tube in
- A narrow detent ring (6.15mm) at the transition provides snap retention
- Beyond the detent, the bore opens to 6.35mm for zero-stress fit
- Pro: Easy insertion with positive retention; self-aligning
- Con: Detent ring must be precisely sized; best with tight-tolerance processes
- Best for: SLA or SLS

### 5b. Socket Recommendation

**Option 2 (internal barb ridge) for SLS/SLA, Option 1 (friction fit) for FDM.**

For SLS or SLA manufacturing where feature resolution is reliable at 0.1-0.2mm, the internal barb ridge provides positive retention without relying on sustained interference. The ridge is a single circumferential bump -- simpler than a collet and more reliable than pure friction.

For FDM (Bambu Lab H2D), use a friction fit with a slightly undersized bore (6.25mm). FDM cannot reliably produce a thin barb ridge at this scale. The friction fit is adequate because the dip tube is also held in tension by the bulkhead fitting at the cap end -- the socket only needs to prevent the tip piece from sliding down the tube, not resist a strong pullout force.

### 5c. Socket Dimensions

| Parameter | FDM (friction) | SLS/SLA (barb) |
|---|---|---|
| Bore diameter | 6.25mm | 6.35mm (nominal) |
| Barb ridge ID | N/A | 6.15mm |
| Barb ridge width | N/A | 1.0mm |
| Socket depth | 18mm | 15mm |
| Entry chamfer | 1mm x 45 deg | 1mm x 45 deg |
| Socket position | Centered on 185mm length | Centered on 185mm length |

### 5d. Air Path Through the Socket

The tube socket must connect to the air channels on the bar surface. The socket bore is where collected air enters the dip tube. Two approaches:

```
    Option A: Open-ended socket (tube does not bottom out)

    ┌──────────────────────────────┐
    │  air channels → ┌────┐ ← tube inserts here
    │  on surface     │bore│    but stops at shoulder
    │  feed into →    │    │    leaving a gap
    │  gap above      └────┘
    │  tube end
    └──────────────────────────────┘

    Air flows: surface channels → gap above tube → down into tube bore


    Option B: Side ports connecting surface to socket bore

    ┌──────────────────────────────┐
    │  air channels    ┌────┐
    │  on surface  ──→ │bore│ ← cross-drilled ports
    │              ──→ │    │    connect surface
    │                  └────┘    channels to bore
    └──────────────────────────────┘
```

**Recommended: Option A (open-ended socket).** The tube inserts into the socket but does not fill its full depth. The remaining void above the tube end connects to the surface air channels through a slot or opening at the top of the socket. This is simpler to manufacture -- no cross-drilling needed.

---

## 6. Air Channel Features

The top and bottom faces of the bar need surface features that prevent the bag film from sealing flat against the bar and create lateral pathways for air to migrate toward the central socket.

### 6a. Design Concept

```
    TOP VIEW: Air channel pattern on one face

    ├──────────── 185mm ────────────┤

    ╱╲  ╱╲  ╱╲  ╱╲  [◉]  ╱╲  ╱╲  ╱╲  ╱╲
    ══════════════════════════════════════    ← longitudinal groove
    ╱╲  ╱╲  ╱╲  ╱╲       ╱╲  ╱╲  ╱╲  ╱╲
    ══════════════════════════════════════    ← longitudinal groove
    ╱╲  ╱╲  ╱╲  ╱╲       ╱╲  ╱╲  ╱╲  ╱╲

    [◉] = central tube socket opening
    ╱╲  = standoff bumps (keep film off surface)
    ═══ = longitudinal grooves (air migration paths)
```

### 6b. Feature Options

#### Longitudinal Ribs (Recommended)

```
    CROSS-SECTION (looking at end of bar):

         22mm
    ┌────────────────────┐
    │╱╲  ╱╲  ◉  ╱╲  ╱╲ │  ← top face: 4 ribs
    │                    │  14mm body
    │╲╱  ╲╱     ╲╱  ╲╱  │  ← bottom face: 4 ribs
    └────────────────────┘

    Rib detail:
         ┌─┐
         │ │  1.5mm tall
    ─────┘ └─────
      3mm wide, 5mm spacing
```

- Ribs run the full 185mm length, parallel to the bar axis
- 3-4 ribs per face, spaced ~5mm apart
- Rib height: 1.5mm (enough to hold film off the surface)
- Rib width: 2-3mm at base
- The valleys between ribs are the air channels
- Air migrates along the valleys toward the center

**Important: ribs must not increase the cross-section beyond the 28mm constraint.**

With the 22mm x 14mm base cross-section:
- Adding 1.5mm ribs to top and bottom: total height becomes 14 + 1.5 + 1.5 = 17mm
- Diagonal of 22mm x 17mm = 27.8mm -- still within 28mm constraint

#### Herringbone / Chevron Pattern

```
    TOP VIEW: Chevron ribs directing air to center

    ╲   ╲   ╲   ╲   [◉]   ╱   ╱   ╱   ╱
     ╲   ╲   ╲   ╲       ╱   ╱   ╱   ╱
      ╲   ╲   ╲   ╲     ╱   ╱   ╱   ╱
```

- V-shaped ribs angled toward the center socket
- Actively directs air toward the socket rather than relying on passive migration
- Pro: More effective air channeling
- Con: More complex print geometry; ribs that cross the bar diagonally may reduce structural stiffness; FDM layer orientation may create weak points along diagonal features

#### Bump Grid / Standoff Dots

```
    TOP VIEW: Grid of standoff bumps

    o  o  o  o  o  [◉]  o  o  o  o  o
    o  o  o  o  o        o  o  o  o  o
    o  o  o  o  o        o  o  o  o  o

    Each 'o' is a hemispherical bump, ~2mm diameter, ~1.5mm tall
    Spacing: 8-10mm grid
```

- Bumps hold the bag film off the surface; air flows freely between bumps in any direction
- Pro: Omnidirectional airflow; simple geometry; prints well in any orientation
- Con: Less directed channeling than ribs; more contact points with film (more potential for film to drape between bumps and seal)

### 6c. Recommended Air Channel Design

**Longitudinal ribs** for primary air channeling, with the following specification:

| Parameter | Value |
|---|---|
| Rib count per face | 4 (2 each side of center socket) |
| Rib height | 1.5mm |
| Rib width (base) | 2.5mm |
| Rib spacing (center-to-center) | 5.5mm |
| Rib length | Full bar length minus socket zone (~80mm per side) |
| Rib profile | Rounded top (semicircular) for easy bag film contact |
| Valley depth | 1.5mm (the rib height) |
| Valley width | 3.0mm (between ribs) |
| Connection to socket | Ribs terminate 3mm from socket opening; valleys feed directly into the socket void |

The rounded rib tops reduce the chance of the bag film catching or tearing on sharp edges, and minimize bacterial harboring compared to sharp-cornered rectangular ribs.

### 6d. Cross-Section With Ribs

```
    END VIEW (actual proportions):

              22mm
    ┌─────────────────────────┐
    │  ╭╮  ╭╮  ╭╮      ╭╮  ╭╮│   ← 1.5mm ribs (top)
    │                         │
    │        [socket]         │   14mm body
    │         6.35mm          │
    │                         │
    │  ╰╯  ╰╯  ╰╯      ╰╯  ╰╯│   ← 1.5mm ribs (bottom)
    └─────────────────────────┘

    Total envelope: 22mm x 17mm
    Diagonal: sqrt(22^2 + 17^2) = 27.8mm  <  28mm  ✓
```

---

## 7. Structural Analysis

### 7a. Load Case

The tip piece is a 185mm beam supported at both ends (wedged between bag side seams). The loads are:

- **Bag film pressure**: The collapsed bag film drapes over the bar. With the bag well-supported by a cradle underneath and the sealed end pinned to the back wall, the film pressure is near zero -- just the weight of the film itself.
- **Hydrostatic head**: After priming, a thin film of syrup may sit on the bar surface. This is negligible load.
- **Assembly forces**: The tube insertion pushes axially on the socket. This is a one-time load, not sustained.

Conservatively, model the bar as a simply supported beam with a 0.1 N/mm distributed load (this is 10x what the bag film actually exerts).

### 7b. Deflection Estimate

For a rectangular beam 22mm x 14mm, 185mm span:

- Second moment of area (I) = (22 x 14^3) / 12 = 5025 mm^4
- For PETG: E = ~2100 MPa
- For PA12 Nylon: E = ~1700 MPa
- Distributed load w = 0.1 N/mm

Maximum deflection (simply supported, uniform load):

    delta = (5 * w * L^4) / (384 * E * I)

For PETG:
    delta = (5 * 0.1 * 185^4) / (384 * 2100 * 5025)
    delta = (5 * 0.1 * 1.17e9) / (384 * 2100 * 5025)
    delta = 5.85e8 / 4.05e9
    delta = 0.14mm

For PA12:
    delta = 5.85e8 / (384 * 1700 * 5025)
    delta = 5.85e8 / 3.28e9
    delta = 0.18mm

Both values are well under 1mm. **The bar will not bow perceptibly under operating loads.** Even at 10x the expected load, deflection is negligible.

### 7c. Stiffness Feel

The concern is not load-bearing deflection but "feel" -- the part should not flex when handled during assembly. A 22mm x 14mm PETG bar at 185mm will feel rigid in hand. A 22mm x 12mm bar would be noticeably less stiff (I drops to 3168 mm^4, deflection increases by 60%). The 14mm depth is preferred for perceived quality.

---

## 8. Material Selection

This is the only fluid-contact 3D-printed part in the system. It is permanently submerged in sugar-based, acidic (pH 3-4) flavor concentrate. Material selection is critical.

### 8a. Comparison Table

| Property | PETG (FDM) | PA12 Nylon (SLS) | BioMed Clear (SLA) | Standard SLA Resin |
|---|---|---|---|---|
| **Food safety** | FDA-compliant raw resin; FDM layer lines harbor bacteria | EU 10/2011 compliant (Shapeways); naturally porous, needs sealing | FDA-cleared (Formlabs); smooth surface | NOT food safe; uncured monomers leach |
| **Chemical resistance (pH 3-4)** | Excellent; PETG resists weak acids | Good; PA12 resists most food acids | Good | Poor long-term |
| **Surface finish** | Layer lines visible (0.1-0.2mm); porous between layers | Slightly grainy (powder sintered); porous | Smooth; minimal porosity after cure | Smooth |
| **Dimensional accuracy** | +/- 0.2-0.3mm (Bambu H2D) | +/- 0.1-0.15mm (Shapeways SLS) | +/- 0.05-0.1mm (Formlabs) | +/- 0.05-0.1mm |
| **Tube socket fit** | Friction fit works; barb ridge unreliable | Barb ridge feasible; friction fit works | Barb ridge feasible; best dimensional control | N/A (not food safe) |
| **Rigidity (E modulus)** | ~2100 MPa | ~1700 MPa | ~2800 MPa | Varies |
| **Water absorption** | Very low (<0.2%) | Moderate (0.5-1.8%); may swell in humid conditions | Low | Low |
| **Cost (single part)** | ~$0.50 material + own printer | ~$8-15 (Shapeways) | ~$10-20 (Formlabs service) | N/A |
| **Lead time** | Same day (own printer) | 5-10 business days (Shapeways/JLC3DP) | 5-10 business days (service) or same day (own printer) | N/A |
| **Post-processing needed** | Food-safe epoxy coating recommended | Food-safe coating recommended (porous) | UV post-cure required | N/A |
| **Layer adhesion risk** | Delamination possible under sustained moisture | N/A (isotropic powder bed) | N/A (isotropic UV cure) | N/A |

### 8b. The Porosity Problem

All 3D-printed parts have some degree of porosity that can harbor bacteria. For a part permanently submerged in sugar syrup:

- **FDM PETG**: Layer lines create grooves ~0.1mm deep. Bacteria can colonize these grooves. The sugar-acid environment inhibits most bacterial growth (low pH, high sugar concentration acts as preservative), but biofilm is possible over months.
- **SLS PA12**: Powder sintering leaves micro-pores on the surface. Similar bacterial colonization risk.
- **SLA**: Smoothest surface, least porosity. But standard resins are not food safe.

### 8c. Mitigation: Food-Safe Coating

For any manufacturing process, applying a food-safe coating eliminates the porosity concern:

- **FDA-compliant epoxy**: Two-part epoxy (e.g., ArtResin, or Max CLR) applied by brush or dip. Fills all micro-pores. Adds ~0.1-0.3mm thickness (account for this in socket bore sizing). Cures in 24-72 hours.
- **Food-safe polyurethane**: Spray or brush application. Multiple thin coats. Good chemical resistance.
- **Silicone conformal coat**: Spray application. Flexible. Excellent food safety. May not adhere well to all substrates.

### 8d. Material Recommendation

**Primary: PETG on Bambu Lab H2D, with food-safe epoxy coating on all surfaces.**

Rationale:
1. Same-day manufacturing allows rapid iteration during prototyping
2. PETG has excellent chemical resistance to the acidic syrup environment
3. The epoxy coating eliminates the FDM porosity concern
4. Cost is negligible (~$0.50 material per part)
5. The Bambu H2D is already in hand -- no new equipment or vendor relationship needed
6. Dimensional accuracy of +/- 0.2mm is adequate for the friction-fit tube socket (the socket bore can be test-fit and reprinted in minutes if needed)

**Secondary (production): PA12 Nylon SLS from Shapeways or JLC3DP, with food-safe coating.**

For production units where consistency matters more than iteration speed, SLS PA12 provides tighter tolerances and isotropic strength. The barb-ridge tube socket becomes feasible. Cost per unit at ~$8-15 is acceptable for a permanent part.

---

## 9. Manufacturing Options and Cost

### 9a. Cost Comparison

| Method | Material | Per-Part Cost | Lead Time | Min Order | Socket Accuracy | Notes |
|---|---|---|---|---|---|---|
| FDM — Bambu H2D | PETG | ~$0.50 | Same day | 1 | +/- 0.2mm | Own printer; iterate fast |
| FDM — Bambu H2D | PETG + epoxy coat | ~$2.00 | Same day + 48h cure | 1 | +/- 0.3mm (coat adds thickness) | Food-safe surface |
| SLS — Shapeways | PA12 Nylon | ~$8-15 | 5-10 days | 1 | +/- 0.1mm | Food-contact compliant material |
| SLS — JLC3DP | PA12 Nylon | ~$5-10 | 7-14 days | 1 | +/- 0.1mm | Lower cost, longer shipping |
| MJF — Shapeways | PA12 Nylon | ~$8-15 | 5-10 days | 1 | +/- 0.1mm | Similar to SLS; denser surface |
| SLA — Formlabs (service) | BioMed Clear | ~$15-25 | 5-10 days | 1 | +/- 0.05mm | FDA-cleared resin; smooth |
| Injection Molding | PP or HDPE | ~$0.50-1.00 | 4-8 weeks (tooling) | 500+ | +/- 0.05mm | Only for high-volume production |

### 9b. Prototyping Strategy

1. **Phase 1 (now)**: Print on Bambu H2D in PETG. Test fit the tube socket. Iterate bore diameter until friction grip is right. Test air channel effectiveness by filling a bag with water and observing air evacuation during a mock prime cycle. No coating needed for testing.

2. **Phase 2 (validation)**: Apply food-safe epoxy coating to the proven design. Install in a real bag with real syrup. Run for 2-4 weeks. Inspect for degradation, discoloration, or odor.

3. **Phase 3 (production)**: If the PETG+epoxy design holds up, continue using it for production units. If tighter tolerances or better surface finish are needed, switch to SLS PA12 from Shapeways.

---

## 10. Recommended Design

### 10a. Final Specification Summary

| Parameter | Value |
|---|---|
| **Overall length** | 185mm |
| **Cross-section (body)** | 22mm wide x 14mm tall |
| **Cross-section (with ribs)** | 22mm wide x 17mm tall |
| **Envelope diagonal** | 27.8mm (fits through 28mm opening) |
| **Tube socket bore** | 6.25mm (FDM friction fit) or 6.35mm + 6.15mm barb (SLS/SLA) |
| **Tube socket depth** | 18mm (FDM) or 15mm (SLS/SLA) |
| **Socket position** | Centered at 92.5mm from each end |
| **Air channel ribs** | 4 per face, 1.5mm tall, 2.5mm wide, rounded tops |
| **Rib spacing** | 5.5mm center-to-center |
| **Material (prototype)** | PETG (Bambu Lab H2D) |
| **Material (production)** | PETG + food-safe epoxy, or SLS PA12 + coating |
| **Weight (estimated)** | ~25-30g (PETG) |

### 10b. Assembly Drawing (All Views)

```
    TOP VIEW:
    ├──────────────────── 185mm ────────────────────┤
    ┌───────────────────────┬─┬────────────────────┐
    │ rib  rib  rib  rib    │◉│  rib  rib  rib  rib│
    │                       │ │                     │
    │ rib  rib  rib  rib    │ │  rib  rib  rib  rib│
    └───────────────────────┴─┴────────────────────┘
                            ↑
                     tube socket (6.35mm)


    FRONT VIEW (end):
              22mm
    ┌─────────────────────┐
    ╭╮  ╭╮  ╭╮  ╭╮  ╭╮  ╭╮   ← ribs (top face)
    │                     │
    │       [◉ 6.35]      │   14mm body
    │                     │
    ╰╯  ╰╯  ╰╯  ╰╯  ╰╯  ╰╯   ← ribs (bottom face)
    └─────────────────────┘
              17mm total


    SIDE VIEW (longitudinal section through socket):

    ribs ─╮  ╭─ ribs
          │  │
    ══════╡  ╞═══════════════════════  ← body (14mm)
          │  │
    ribs ─╯  ╰─ ribs
          │  │
          │  │ ← tube socket bore
          │  │   (18mm deep)
          └──┘
           ↓
        tube inserts from below
```

### 10c. Print Orientation (FDM)

Print the bar **on its side** (22mm face down on the bed, 185mm along X-axis, 14mm along Z-axis):

- The 185mm length runs along the bed -- no support needed for the main body
- Ribs on the top and bottom faces print as horizontal features on the vertical walls
- The tube socket bore runs vertically (Z-axis) -- this gives the best circularity for the bore
- Layer lines run perpendicular to the bar length, which is the strongest orientation for bending loads

Alternatively, print **flat** (22mm x 185mm footprint, 17mm tall including ribs):

- Ribs on top face print cleanly as top features
- Ribs on bottom face need supports (or design as flat-bottom ribs that print without support)
- Tube socket bore runs horizontally -- may be slightly oval due to FDM layer stacking
- Faster print, but socket bore accuracy is worse

**Recommended: Side orientation** for socket bore accuracy, even though print time is longer (~2-3 hours vs ~1 hour flat).

### 10d. Open Questions

1. **Exact bag internal width**: Needs physical measurement of a Platypus 2L bag. The 185mm length is estimated from the 190mm external width minus seam allowance. Measure and adjust.

2. **Rib height vs. film stiffness**: 1.5mm ribs may be excessive or insufficient depending on how the bag film behaves when collapsed against the bar at the top of the incline. Test with a real bag.

3. **Socket bore tolerance**: The ideal bore diameter for the FDM friction fit must be determined experimentally. Print test sockets at 6.20, 6.25, 6.30, and 6.35mm and test insertion/retention with the actual 1/4" hard tube stock.

4. **Epoxy coating in the socket bore**: If the socket is coated with food-safe epoxy, the bore will shrink by ~0.1-0.3mm. Either leave the socket uncoated (it is a tight bore that is difficult to coat evenly) or oversize the bore to compensate.

5. **Rotation clearance during assembly**: Confirm that the 185mm bar can rotate 90 degrees inside the bag when inserted through the 28mm opening. The bag interior at the sealed end may be narrower than 185mm if the bag is not fully inflated during assembly. The user may need to partially inflate the bag (blow into it) before performing the rotation.
