# Release Plate — Detailed Design Research

The release plate is the part that makes simultaneous contact with all 4 John Guest collets to release the cartridge tubing. It translates axially under cam or lever actuation, pushing each collet inward to disengage the gripper teeth. This document explores the geometry, spacing, compliance, guide features, material choices, and print strategy in enough detail to produce a first print.

This builds on established values from prior research:

| Parameter | Value | Source |
|---|---|---|
| Tube OD | 6.35mm (1/4") | collet-release.md (verified) |
| Fitting body OD | ~12.7mm | collet-release.md (measured/inferred) |
| Collet ring OD | ~11.4mm | collet-release.md (measured/inferred) |
| Tube clearance bore | 8.0mm | collet-release.md |
| Inner lip bore (collet pusher) | 10.5mm | collet-release.md |
| Outer bore (collet cradle) | 12.5mm | collet-release.md |
| Collet travel (inward) | ~1.5-2.0mm | collet-release.md |
| Plate travel (stroke) | 3.0mm (min 2.5mm) | collet-release.md |
| Force per fitting | ~3-5N | collet-release.md |
| Total force (4 fittings) | ~12-20N | collet-release.md |
| FDM sliding clearance | 0.3-0.5mm per side | guide-alignment.md |
| Cam eccentricity | 1-1.5mm (3mm stroke) | cam-lever.md |

---

## 1. Stepped Bore Geometry in Detail

Each hole in the release plate replicates the geometry of a John Guest release tool. The bore has three concentric diameters, each with a specific function.

### 1.1 Cross-Section Profile

```
    ← fitting side (collet faces this way)     plate body →

    ┌───────────────────────────────────────────────────────┐
    │                                                       │
    │   ┌─ outer bore (cradle) ─────────────────────────┐   │
    │   │                                               │   │
    │   │   ┌─ inner lip ───────────────────────────┐   │   │
    │   │   │                                       │   │   │
    │   │   │   ┌─ tube clearance hole ─────────┐   │   │   │
    │   │   │   │                               │   │   │   │
    │   │   │   │       8.0mm dia               │   │   │   │
    │   │   │   │                               │   │   │   │
    │   │   │   └───────────────────────────────┘   │   │   │
    │   │   │         10.5mm dia                    │   │   │
    │   │   └───────────────────────────────────────┘   │   │
    │   │              12.5mm dia                       │   │
    │   └───────────────────────────────────────────────┘   │
    │                                                       │
    └───────────────────────────────────────────────────────┘
```

### 1.2 Axial Profile (Side View of One Bore)

```
    ← fitting side                    back side →

    plate travel direction: ←───────

         outer     inner lip    tube hole
         bore      (pusher)     (through)
        ┌─────┐   ┌────────┐   ┌──────────────────────────┐
        │     │   │        │   │                          │
        │     │   │        │   │    8.0mm through hole    │
        │     │   │        │   │                          │
    ────┘     └───┘        └───┘                          └────
    plate     2.0mm  1.5mm        remaining plate
    face      deep   deep         thickness
```

### 1.3 Feature-by-Feature Breakdown

**Tube clearance hole: 8.0mm diameter, through the full plate thickness**

This is the center bore that the tube passes through freely. The 8.0mm diameter provides 1.65mm of clearance around the 6.35mm tube (0.825mm per side). This clearance serves two purposes: (1) the tube passes through without binding even with slight misalignment, and (2) during release, the tube can withdraw at a slight angle without catching on the bore wall. The through-hole extends the full thickness of the plate.

For print quality, 8.0mm is large enough that FDM can produce a clean circular hole. Printing with the bore axis vertical (Z-axis) gives the best circularity. A 0.2mm chamfer at the entry helps thread the tube through during assembly.

**Inner lip: 10.5mm bore, 1.5mm depth**

The inner lip is the annular surface that contacts the collet face and pushes it inward. It is the most critical feature on the entire plate.

- **Inner diameter (10.5mm)**: Must clear the tube (8.0mm) with margin but contact the collet ring face. The collet ring OD is ~11.4mm. The lip's 10.5mm ID means the lip overhangs the collet face by (11.4 - 10.5) / 2 = 0.45mm per side. This is the contact annulus -- the ring of material that actually pushes the collet.
- **Lip annular width**: (10.5 - 8.0) / 2 = 1.25mm. This is the radial width of the lip wall. For PETG at reasonable infill, 1.25mm is approximately 3 perimeter walls at 0.4mm nozzle width -- structurally adequate for the 3-5N per fitting force, but at the lower bound of what FDM can produce with precision.
- **Depth (1.5mm)**: The lip must remain engaged with the collet face through the full collet travel (~1.5-2.0mm). A 1.5mm lip depth means the lip starts flush with the collet face at initial contact and is still engaged at the end of 1.5mm of travel. However, if the collet protrusion varies by +/-0.3mm between fittings, some collets may not engage until 0.3mm into the stroke, leaving only 1.2mm of effective engagement at the end.

  **Recommendation**: Increase lip depth to 2.0mm. This provides 1.7mm minimum engagement even with 0.3mm protrusion variation. The tradeoff is a slightly thicker plate, but 0.5mm additional material is negligible.

- **Lip face profile**: The face of the lip (the surface that contacts the collet) should be flat, not chamfered. A chamfer would reduce the contact area and could allow the collet to cam sideways. A very slight break (0.1-0.2mm 45-degree chamfer) on the inner edge is acceptable to prevent catching during insertion, but the outer contact annulus must remain a flat, perpendicular face.

**Outer bore (cradle): 12.5mm bore, 2.0mm depth**

The outer bore surrounds the collet ring sides and prevents lateral movement during release. This is the feature that prevents the cocking/tilting failure mode described in collet-release.md Section 4.

- **Bore diameter (12.5mm)**: The collet ring OD is ~11.4mm. This gives (12.5 - 11.4) / 2 = 0.55mm clearance per side between the collet ring and the cradle wall. This is tight enough to prevent meaningful lateral collet movement but loose enough for the plate to slide over the collet without binding.
- **Depth (2.0mm)**: The collet ring protrudes ~2-3mm from the fitting body face. A 2.0mm cradle depth means the cradle wall surrounds at least the outer 2.0mm of the protruding collet ring. The collet moves inward during release (further into the cradle), so the cradle engagement depth actually increases during actuation.

  **Key insight from the John Guest PI-TOOL**: The commercial tool's cradle wraps around the collet sides, not just its face. The depth of cradle engagement determines how much lateral constraint is provided. At 2.0mm depth, with a 2-3mm collet protrusion, the cradle captures at least the outer 2/3 of the collet -- sufficient to prevent tilting during a 1.5-2.0mm inward push.

- **Wall angle**: The cradle bore should be straight (cylindrical), not tapered. A tapered cradle would allow the collet to shift laterally as it moves axially. The cylindrical bore maintains constant lateral constraint through the full stroke. A small lead-in chamfer (0.3mm x 45 degrees) at the entry helps the plate slide over the collet without catching on any slight collet protrusion asymmetry.

### 1.4 Transition Between Bores

The transition from outer bore to inner lip is a flat annular step (a shoulder). The shoulder face is perpendicular to the bore axis. This shoulder doesn't contact anything during normal operation -- it exists between the lip face (which pushes the collet) and the cradle wall (which constrains the collet sides). The sharp internal corner where the shoulder meets the cradle wall will naturally have a small radius from FDM printing (~0.2mm for a 0.4mm nozzle), which is fine.

The transition from inner lip to tube hole is also a flat step. This internal shoulder serves no functional purpose during release -- it's simply the geometry left by the two different bore diameters. Again, FDM corner radius is acceptable.

### 1.5 Complete Axial Dimension Stack

From the fitting-facing surface of the plate inward:

| Zone | Depth | Running Total | Feature |
|---|---|---|---|
| Outer bore (cradle) | 2.0mm | 2.0mm | 12.5mm dia, constrains collet sides |
| Inner lip (pusher) | 2.0mm | 4.0mm | 10.5mm dia, pushes collet face |
| Structural back | 2.0mm | 6.0mm | 8.0mm through-hole only |
| **Total plate thickness** | | **6.0mm** | |

The 2.0mm structural back provides material behind the inner lip for rigidity. Without this, the lip would be a thin cantilevered ring that could flex under load. The 8.0mm through-hole continues through this zone.

**Alternative: 5.0mm total** by reducing the structural back to 1.0mm. This is the minimum before the lip becomes too flexible in PETG. 6.0mm is the conservative starting point; 5.0mm is acceptable if plate thickness is constrained by the overall assembly.

---

## 2. Hole Spacing and Arrangement

### 2.1 Minimum Center-to-Center Spacing

The outer bore is 12.5mm diameter. Two adjacent outer bores need a wall of material between them for structural integrity.

**Minimum wall thickness between bores in PETG (FDM)**:
- Absolute minimum: 1.0mm (approximately 2.5 perimeter widths at 0.4mm nozzle). The wall would be structurally sound in compression (the plate pushes collets inward) but fragile if any lateral or bending load is applied.
- Recommended minimum: 1.5mm (approximately 4 perimeter widths). Provides meaningful cross-section for both compression and bending loads.
- Comfortable: 2.0mm. Allows full infill between bores and provides enough material to resist plate flex between holes.

**Center-to-center spacing = outer bore diameter + wall thickness**:

| Wall Thickness | Center-to-Center | Notes |
|---|---|---|
| 1.0mm | 13.5mm | Absolute minimum, fragile |
| 1.5mm | 14.0mm | Recommended minimum |
| 2.0mm | 14.5mm | Comfortable, resistant to flex |
| 3.0mm | 15.5mm | Very robust, may be larger than needed |

**Recommendation: 15.0mm center-to-center**. This gives a 2.5mm wall between outer bores -- robust enough to resist any flex under the 12-20N total actuation force, and provides margin for FDM dimensional variation (if the bore prints 0.2mm oversize, the wall is still 2.3mm).

### 2.2 Arrangement Options

The 4 fittings correspond to 2 pumps x 2 connections each (inlet + outlet). The physical arrangement of these 4 holes on the plate determines the plate footprint and aspect ratio.

#### Option A: 4-in-a-Line (1x4)

```
    ┌───────────────────────────────────────────────────────────┐
    │   (1)         (2)         (3)         (4)                 │
    │    O           O           O           O                  │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
         ←─ 15 ─→   ←─ 15 ─→   ←─ 15 ─→
                   45mm total span
```

| Dimension | Value |
|---|---|
| Span (center-to-center, outer holes) | 45.0mm |
| Plate width | 45.0 + 12.5 + 2x3.0 (margin) = 63.5mm |
| Plate height | 12.5 + 2x3.0 = 18.5mm |
| Aspect ratio | ~3.4:1 |

**Pros**: Simple layout. All holes in a single plane. Easy to ensure uniform cam/lever force distribution along one axis. Tube routing behind the plate is straightforward (all tubes exit in one row).

**Cons**: Long and narrow. The 63.5mm width is significant -- this is the widest the plate (and the mating face of the dock) needs to be. Tilt control is most critical along the long axis: if the cam applies force off-center, the outer holes are 22.5mm from center, creating meaningful moment arms.

**Tilt sensitivity**: A 0.3mm parallelism deviation across 45mm span = 0.38 degrees. This is within the <0.3mm deviation spec from collet-release.md only if the guide features constrain the plate's rotation about the short axis (height axis).

#### Option B: 2x2 Grid

```
    ┌───────────────────────────────────────┐
    │   (1)         (2)                     │
    │    O           O                      │
    │                                       │
    │   (3)         (4)                     │
    │    O           O                      │
    └───────────────────────────────────────┘
         ←─ 15 ─→
              ↕
             15mm
```

| Dimension | Value |
|---|---|
| Span (horizontal) | 15.0mm |
| Span (vertical) | 15.0mm |
| Plate width | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Plate height | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Aspect ratio | 1:1 (square) |

**Pros**: Compact square footprint. Tilt resistance is equal in both axes. The cam force application point can be centered on the plate, and the maximum moment arm to any hole is only sqrt(7.5^2 + 7.5^2) = 10.6mm (vs. 22.5mm for 4-in-a-line). This makes parallelism much easier to maintain. The smaller footprint means the dock mating face can be smaller.

**Cons**: Tube routing is more complex -- tubes exit in a 2x2 grid, which may conflict with pump placement inside the cartridge. The 15mm vertical spacing puts the top and bottom fittings closer together than might be comfortable for John Guest fitting bodies (fitting body OD is ~12.7mm, so two fittings at 15mm C-C have only 2.3mm between their bodies).

**Potential issue**: The 2x2 grid means the dock must have 4 John Guest fittings arranged in a 15x15mm grid. The fitting body OD is ~12.7mm. At 15.0mm C-C, the gap between fitting bodies is 15.0 - 12.7 = 2.3mm. This is physically possible (the fittings don't touch), but tight. If the fittings have any hex or molding flash, they may interfere. The dock mounting holes/pockets would need precise placement.

#### Option C: Diamond (Rotated 2x2)

```
    ┌─────────────────────────────────────────────┐
    │              (1)                             │
    │               O                             │
    │                                             │
    │   (2)                     (3)               │
    │    O                       O                │
    │                                             │
    │              (4)                             │
    │               O                             │
    └─────────────────────────────────────────────┘
```

Same as 2x2 but rotated 45 degrees. Center-to-center distances remain 15.0mm between adjacent holes. The bounding box becomes:

| Dimension | Value |
|---|---|
| Width (horizontal span) | 2 x 15.0 x cos(45) = 21.2mm + 12.5 + 2x3.0 = 39.7mm |
| Height (vertical span) | 2 x 15.0 x sin(45) = 21.2mm + 12.5 + 2x3.0 = 39.7mm |

**Pros**: Maximizes the distance between all 4 holes (all are equidistant from the center). Slightly more space between adjacent fitting bodies along the horizontal axis. Looks elegant.

**Cons**: The bounding box is larger than the 2x2 grid (39.7mm vs 33.5mm) for the same center spacing. The diamond orientation doesn't align with natural rectangular geometry of the cartridge body. Tube routing in a diamond pattern is awkward. No practical advantage over the 2x2 grid for this application.

#### Option D: Offset Pairs (2+2)

```
    ┌─────────────────────────────────────────────────────┐
    │   (1)         (2)                                   │
    │    O           O                                    │
    │                                                     │
    │         (3)         (4)                              │
    │          O           O                              │
    └─────────────────────────────────────────────────────┘
         ←─ 15 ─→
              offset 7.5mm
```

Two rows of 2, with the bottom row offset horizontally by half the spacing (7.5mm). This is a hexagonal close-packing arrangement.

| Dimension | Value |
|---|---|
| Width | 22.5 + 12.5 + 2x3.0 = 41.0mm |
| Height | 15.0 + 12.5 + 2x3.0 = 33.5mm |

**Pros**: Allows tighter vertical spacing between rows because the holes don't stack directly above each other. The offset means adjacent-row holes are spaced at sqrt(15^2 + 7.5^2) = 16.8mm C-C, which is more than the 15mm same-row spacing.

**Cons**: Asymmetric. The cam/lever force must still be centered, but the plate's center of area doesn't align neatly with any single hole. More complex to manufacture the matching dock (fitting positions are non-rectangular). No significant advantage over the 2x2 grid.

### 2.3 Arrangement Comparison Summary

| Arrangement | Plate Width | Plate Height | Max Moment Arm | Tilt Risk | Tube Routing | Dock Complexity |
|---|---|---|---|---|---|---|
| 4-in-a-line | 63.5mm | 18.5mm | 22.5mm | **High** (long axis) | Simple (1 row) | Simple (1 row) |
| 2x2 grid | 33.5mm | 33.5mm | 10.6mm | **Low** (symmetric) | Moderate (2x2) | Moderate |
| Diamond | 39.7mm | 39.7mm | 10.6mm | Low | Complex | Complex |
| Offset pairs | 41.0mm | 33.5mm | 13.5mm | Moderate | Complex | Complex |

### 2.4 Recommendation

**2x2 grid is the strongest option.** The symmetric footprint cuts the maximum moment arm in half compared to 4-in-a-line, which directly addresses the tilt failure mode. The compact 33.5mm square is easier to guide and actuate evenly than a 63.5mm bar. The fitting body clearance (2.3mm) is tight but workable.

**4-in-a-line is the fallback** if the cartridge body geometry or tube routing makes the 2x2 impractical. The long plate requires more robust guide features and a wider cam/lever.

Pairing: In the 2x2 grid, the natural grouping is pump 1 inlet/outlet as one column, pump 2 inlet/outlet as the other column:

```
    Pump 1 IN    Pump 2 IN
        O            O

    Pump 1 OUT   Pump 2 OUT
        O            O
```

This keeps each pump's inlet and outlet in a vertical pair, which simplifies internal tube routing inside the cartridge.

---

## 3. Plate Thickness

### 3.1 Thickness Budget

The plate thickness is driven by the axial depth stack of the stepped bore features plus structural material behind them.

From Section 1.5:

| Zone | Depth |
|---|---|
| Outer bore (cradle) | 2.0mm |
| Inner lip (pusher) | 2.0mm |
| Structural back | 2.0mm |
| **Total** | **6.0mm** |

### 3.2 Can It Be Thinner?

**5.0mm minimum** (reducing structural back to 1.0mm):
- The inner lip wall is only 1.25mm radial width. With only 1.0mm of material behind it, the lip is essentially a thin-walled tube with 1.25mm wall and 1.0mm backing. Under 5N per fitting load (distributed around the annulus), this is structurally adequate in compression but has no margin for FDM defects (underextrusion, layer adhesion issues).
- Acceptable for a test coupon. Not recommended for the production plate.

**6.0mm recommended** for prototyping. Provides 2.0mm structural back, which gives confident rigidity and allows the back face to serve as a bearing surface for guide features.

**7.0mm or more** would add no functional benefit and increases the overall assembly depth unnecessarily. The plate travel is only 3.0mm, so the plate moves within a small axial envelope.

### 3.3 Assembly Depth Implications

The plate sits between the cam/lever mechanism and the fitting face. The total axial depth consumed by the release mechanism is:

```
    cam mechanism depth + plate thickness + plate travel + collet protrusion

    = (TBD, ~10-15mm for cam) + 6.0mm + 3.0mm + 2.5mm
    = ~21.5-26.5mm total
```

This is the depth behind the dock mating face dedicated to the release mechanism. For a cartridge dock that might be 150-200mm deep total, this is a manageable 10-15% of the total depth.

---

## 4. Linear Guide Features

### 4.1 Why Guides Are Critical

The plate must translate axially (parallel to the tube/fitting axis) without tilting. Collet-release.md Section 4 identifies plate tilt as a system-level failure mode: if the plate cocks during actuation, some collets release while others grip harder, replicating the single-fitting "one-sided press" failure at the multi-fitting level.

The parallelism spec (from collet-release.md): <0.3mm deviation across the plate. For a 33.5mm square plate (2x2 arrangement), this means the plate must stay parallel to within 0.3mm / 33.5mm = 0.51 degrees during the full 3.0mm stroke. For a 63.5mm plate (4-in-a-line), it's 0.3mm / 63.5mm = 0.27 degrees -- tighter.

### 4.2 Guide Options for the Plate

#### Option 1: Pin-in-Slot (Recommended for Prototype)

Two or more round pins press-fit or bolted into the dock frame. The plate has matching holes or slots that slide along the pins. The pins constrain the plate to axial translation only.

```
    Top view (plate face):

    ┌──────────────────────────────────┐
    │  ○ pin hole      ○ pin hole      │
    │                                  │
    │       O (1)        O (2)         │
    │                                  │
    │       O (3)        O (4)         │
    │                                  │
    │  ○ pin hole      ○ pin hole      │
    └──────────────────────────────────┘
```

**Pin sizing**: 3mm or 4mm diameter steel dowel pins. For 3mm pins, the plate holes would be 3.2-3.3mm (0.1-0.15mm clearance per side for smooth sliding). For 4mm pins, holes at 4.2-4.3mm.

**Pin placement**: Symmetrically placed at corners or midpoints of edges, outside the bore pattern. For the 2x2 grid plate (33.5mm square), pins at the 4 corners would work but crowd the bores. Better: 2 pins centered on opposite edges (top and bottom), spaced 30mm apart. This constrains tilt about the horizontal axis (the most critical axis for a 2x2 grid).

**Number of pins**: 2 pins fully constrain tilt in one plane. For full anti-tilt, either use 2 pins far apart (maximizing the moment arm against tilt) or 4 pins in a rectangular pattern. Two pins is sufficient if the plate is compact (2x2 grid) and the pins are at least 25mm apart.

**Slot vs. round hole**: Round holes provide the tightest constraint but require the pins to be perfectly parallel. If the dock frame has any tolerance error in pin spacing or parallelism, the plate binds. Slots (elongated holes, round-ended) provide forgiveness in one axis at the cost of looseness in that axis. For a 3D-printed dock, slots are more practical -- they tolerate the ~0.2mm dimensional variation in FDM without binding.

**Slot dimensions**: For 3mm pins: slot width 3.3mm, slot length 3.3mm + 3.0mm travel + 1.0mm margin = 7.3mm. The slot is oriented along the plate travel axis.

**Pros**: Simple, cheap (steel dowel pins are pennies each), precise (steel pin surface is much smoother than 3D printed). The pins can also serve as spring seats if return springs are needed.

**Cons**: Requires hardware (pins). Pins must be securely mounted in the dock frame. Two separate components (plate + dock frame) must be printed and assembled.

#### Option 2: Integral Rail Grooves

The plate has grooves or channels on its sides that mate with corresponding rails on the dock frame. The plate slides along the rails.

```
    Side view:

    dock frame wall    plate edge    dock frame wall
    ┌─────┐          ┌──────────┐          ┌─────┐
    │     │          │          │          │     │
    │  ▶──┤          ├──◀  ▶──┤          ├──◀  │
    │     │          │          │          │     │
    │     │          │          │          │     │
    └─────┘          └──────────┘          └─────┘
              rail/groove interface
```

**Pros**: No separate hardware. Can be printed as part of the plate and dock frame.

**Cons**: FDM rail surfaces are rough (layer lines). Binding is likely without post-processing (sanding). The clearance needed for smooth FDM sliding (0.3-0.5mm per side) is loose enough to allow 0.3-1.0mm of tilt -- right at or beyond the parallelism spec. This approach is sensitive to print quality.

#### Option 3: Bore-Guided (Collets Themselves as Guides)

The outer bores of the stepped profile slide over the collet ring/fitting body, using the fittings themselves as guide surfaces. The plate is guided by the 4 fittings it engages with.

**Pros**: Zero additional hardware. The guide and actuation surfaces are colocated, which inherently prevents the plate from tilting relative to the collets (the most important reference).

**Cons**: The plate must approach the fittings already roughly aligned -- it can't be guided by features it hasn't engaged yet. There's a "last millimeter problem": the plate has no guidance until the outer bore slides over the collet ring. Works as secondary guidance once engaged but needs primary guidance for the approach. Also, the clearance between outer bore (12.5mm) and collet ring (11.4mm) is 0.55mm per side, which is too loose for primary guidance.

#### Option 4: Hybrid (Recommended)

**Pins for primary guidance, bores for secondary alignment.**

Two steel dowel pins (3mm) through the plate provide the primary linear constraint. The outer bore cradles provide secondary centering as the plate engages the collets. The pins keep the plate from tilting during the approach; the bore cradles self-center on the collets during engagement.

This is the server blade approach: coarse rails (pins) for the travel, fine features (bores) for the final alignment.

### 4.3 Cam/Lever Interface

The plate needs a feature where the cam or lever applies force to drive it axially. Options:

**Central boss**: A raised pad or boss on the back face of the plate, centered. The cam presses against this pad. Transmits force through the plate's center, minimizing tilt.

**Cam slot**: A through-slot in the plate that the cam shaft passes through. As the cam rotates, its eccentric profile pushes against the slot walls. This constrains the cam to act at a known location on the plate.

**Edge push**: The cam or lever pushes on the plate's edge. Simpler mechanically but applies force off-center, which tends to tilt the plate. Only acceptable if the guide pins are robust enough to resist the resulting moment.

**Recommendation for first print**: Design the back face of the plate with a flat surface (the 2.0mm structural back). The cam interface can be a simple flat-on-flat contact at the plate's center. The cam mechanism is a separate design problem; the plate just needs a flat back face and guide pin holes.

### 4.4 Return Mechanism

After the cam retracts (lever opened), the plate must return to its retracted position (away from the collets). Options:

1. **Collet springs**: The collet rings themselves are spring-loaded to their resting (gripping) position. When the plate retracts, the collets push the plate back. This provides ~2-5N per fitting of return force (same as the actuation force), totaling 8-20N. This is enough to return the plate without additional springs.

2. **Compression springs on guide pins**: Small compression springs around the guide pins, between the plate and the dock frame. Provides a positive return force independent of the collets. Adds cost (springs) but ensures the plate always returns even if collets are absent (useful during assembly/testing).

3. **Cam profile**: If the cam has a positive displacement profile in both directions (not just a single lobe), it can drive the plate both inward and outward. An eccentric cam does this naturally -- rotating one way pushes the plate in, rotating the other way allows it to retract. The plate follows the cam profile.

**Recommendation**: Rely on collet spring-back (option 1) for the initial prototype. The collets naturally push the plate back. If testing reveals unreliable return, add compression springs later (option 2). The plate design should accommodate either -- meaning the guide pin holes should have enough length for a spring to sit around the pin.

---

## 5. Compliance (Handling Collet Protrusion Variation)

### 5.1 The Problem

The 4 collet rings may not all protrude the same distance from the fitting body face. Collet-release.md estimates +/-0.3mm variation between fittings of the same model. Additionally, the dock's fitting mounting may introduce +/-0.5mm variation in fitting face position. Total worst-case variation between the most-protruding and least-protruding collet: up to 0.8mm.

A rigid plate pushing all 4 collets simultaneously will contact the most-protruding collet first. The plate must continue traveling until the least-protruding collet is also fully engaged. During this continued travel, the already-engaged collets are pushed beyond their required travel.

### 5.2 Overtravel (Simplest Approach)

**Concept**: Design the plate travel (3.0mm) to exceed the collet travel (1.5-2.0mm) by enough margin to absorb the variation.

**Math**: Collet travel needed = 2.0mm (using the high end). Variation = 0.8mm worst case. Minimum plate travel = 2.0mm + 0.8mm = 2.8mm. The 3.0mm design travel provides 3.0 - 2.8 = 0.2mm margin.

**What happens to the first-engaged collets**: They are pushed 0.8mm beyond their required travel (worst case). The collet has an internal hard stop where it bottoms out against the fitting body. The force required to push a collet past its travel limit is higher than the release force (the spring compresses further and/or the collet contacts a hard stop). This additional force is absorbed by the cam mechanism.

**Risk**: If the collet has a definite hard stop (metal-on-metal inside the fitting), the overtravel force could be substantial. If the collet spring simply compresses further (no hard stop), the overtravel force is modest. This must be tested with actual fittings.

**Verdict**: Overtravel is the simplest approach and should be the first thing tested. The 3.0mm plate travel from collet-release.md already accounts for this. If testing shows the collet has a hard stop that makes overtravel problematic, add compliance.

### 5.3 Elastomeric Layer

**Concept**: A thin sheet of rubber or silicone between the rigid plate and the collets. Each collet presses into the elastomer independently, and the elastomer deforms to accommodate variation.

**Material**: 1-2mm sheet of 40-60A durometer silicone (soft). Available as silicone gasket sheet from McMaster-Carr or Amazon.

**Compliance**: A 1.5mm sheet of 50A silicone compresses approximately 20-30% under the 3-5N per fitting load. That's 0.3-0.45mm of independent travel per collet -- close to the +/-0.3mm fitting variation spec.

**Implementation**: Cut a gasket-shaped piece (same outline as the plate, with 8.0mm holes for tube clearance) and sandwich it between the plate face and the collets. The gasket sits in the outer bore cradle.

**Pros**: Simple, cheap, no moving parts. Provides distributed compliance. Can be replaced if it degrades.

**Cons**: Adds an axial layer (1-2mm) to the assembly. The elastomer may creep under sustained load (though the cam only applies force during the release action, not continuously). The elastomer fills the outer bore cradle space, which may interfere with collet lateral constraint.

**Verdict**: Good fallback if overtravel alone doesn't work. Easy to retrofit -- just cut a gasket and insert it. The plate design doesn't need to change.

### 5.4 Spring-Loaded Lips

**Concept**: Each of the 4 inner lip features is independently spring-loaded, allowing it to travel axially relative to the plate body.

**Implementation in FDM**: Extremely difficult. Each lip would need to be a separate sliding ring within the plate body, with a small compression spring behind it. The bore diameter is only 10.5mm, leaving very little room for spring seats and sliding features. The assembly complexity (4 springs, 4 sliding rings, 4 retaining features) is far beyond what's needed for 0.8mm of compliance.

**Verdict**: Over-engineered for this application. The compliance need is <1mm, and overtravel or an elastomeric layer handles it trivially. Spring-loaded lips are appropriate for systems with 5mm+ variation or where different fittings need fundamentally different travel.

### 5.5 Compliance Recommendation

**Start with overtravel alone.** The 3.0mm plate travel provides 1.0mm margin over the 2.0mm collet travel. If the worst-case 0.8mm fitting variation is absorbed by this margin without excessive force, no additional compliance is needed.

If overtravel causes excessive force (collet hard stops), **add a 1.5mm silicone gasket** between the plate and the collets. This adds <0.5mm of independent compliance per collet, bringing the total compliance budget to 1.0mm + 0.5mm = 1.5mm -- more than adequate.

---

## 6. Material and Print Orientation

### 6.1 Material: PETG

PETG is the recommended material (consistent with guide-alignment.md for all sliding surfaces):

- **Tensile strength**: ~50 MPa (adequate for the 12-20N total load on the plate)
- **Flexural modulus**: ~2.1 GPa (stiff enough that the plate won't flex meaningfully under load)
- **Layer adhesion**: significantly better than PLA. Critical because the inner lip is a thin feature (1.25mm wall) that could delaminate under repeated axial loading if layer adhesion is poor.
- **Chemical resistance**: resistant to water, mild acids/bases. Important for moisture-adjacent environment.
- **Friction coefficient (PETG on PETG)**: ~0.1-0.2. Lower than PLA-on-PLA (~0.2-0.3). Important for smooth sliding on guide pins and in the dock frame.
- **Creep resistance**: better than PLA at room temperature.

**Alternatives**:
- PLA: easier to print, more brittle. Acceptable for test coupons, not for the production plate.
- ABS: better heat resistance, worse layer adhesion than PETG, requires enclosure. No advantage for this application.
- Nylon/PA: excellent wear resistance and toughness. Harder to print (warping, moisture absorption). Overkill.

### 6.2 Print Orientation

The critical question is which axis the bore runs relative to the build plate.

#### Option A: Bore axis vertical (Z-up) -- RECOMMENDED

```
    Build plate (XY)
    ┌───────────────────┐
    │                   │
    │  O  O   ← bores  │   ← looking DOWN at the plate
    │  O  O             │      lying flat on the bed
    │                   │
    └───────────────────┘
```

The plate lies flat on the build plate. The bore axis runs vertically (Z direction). Each layer is a horizontal slice through the plate.

**Pros**:
- The circular bores are printed as circles in the XY plane -- best possible circularity from FDM.
- The inner lip is formed by the XY perimeter paths, which gives the smoothest and strongest surface.
- The lip wall (1.25mm radial) is printed as perimeter walls, which are the strongest part of an FDM print.
- The plate's overall flatness is determined by the build plate, which is the most planar reference surface available.
- No supports needed (all features are either flat bottom or circular holes).

**Cons**:
- The plate thickness (6mm) is in the Z direction. Layer adhesion must be good to resist the axial (Z-direction) forces. PETG's superior layer adhesion makes this acceptable.
- The flat outer face (fitting-facing surface) is the first layer on the build plate. First-layer quality matters for the lip face flatness. Print on a smooth PEI sheet for the best first-layer finish.

#### Option B: Bore axis horizontal

The plate stands upright on the build plate, with the bore axes running in the XY plane.

**Pros**: Layer adhesion direction aligns with the axial load direction (strongest). The lip face is formed by the perimeter paths.

**Cons**: The circular bores are formed by stacked layers (stairstepping). A circle printed in the ZX or ZY plane has visible layer steps that create a rough bore surface. The inner lip bore at 10.5mm would have ~0.2mm step artifacts at 0.2mm layer height. The plate needs supports for the overhanging bore features. Overall worse geometric accuracy.

**Verdict**: Option A (bore axis vertical) is clearly superior.

### 6.3 Print Parameters

| Parameter | Value | Rationale |
|---|---|---|
| Layer height | 0.16-0.20mm | Standard quality. 0.16mm for best bore accuracy. |
| Nozzle | 0.4mm | Standard. The 1.25mm lip wall is ~3 perimeters at 0.4mm. |
| Perimeters/walls | 4 minimum | Ensures the lip wall is solid perimeters (no infill). |
| Infill | 40-60% | Body infill between bores. Higher infill = stiffer plate. |
| Infill pattern | Grid or gyroid | Gyroid for best isotropic stiffness. |
| Top/bottom layers | 4 minimum | Solid cap on the fitting-facing surface (lip face). |
| First layer | 0.20mm on smooth PEI | Clean first layer = flat lip face. |
| Print speed | 40-60mm/s | Slower for better dimensional accuracy on small features. |

### 6.4 Minimum Wall Thicknesses

| Feature | Wall Thickness | Perimeters (0.4mm nozzle) | Assessment |
|---|---|---|---|
| Inner lip radial wall | 1.25mm | 3.1 perimeters | Adequate. Will be printed as 3 perimeters + thin infill. |
| Wall between outer bores (at 15mm C-C) | 2.5mm | 6.25 perimeters | Comfortable. Solid material between bores. |
| Outer rim (bore center to plate edge) | 3.0mm minimum | N/A (includes half the bore + margin) | Provides material for guide pin holes outside the bore pattern. |

---

## 7. First Print Specifications

### 7.1 Single-Hole Test Coupon

**Purpose**: Validate the stepped bore geometry against an actual John Guest fitting before committing to a 4-hole plate. Verify that the inner lip correctly engages and releases the collet. Measure actual collet release feel and force.

**Design**:

```
    Top view:
    ┌───────────────────┐
    │                   │
    │        O          │    Single stepped bore
    │                   │    centered on a square block
    │                   │
    └───────────────────┘

    Dimensions:
    - Block: 20mm x 20mm x 6mm thick
    - Bore: 8.0 / 10.5 / 12.5mm stepped profile per Section 1
```

| Dimension | Value |
|---|---|
| Block width | 20.0mm |
| Block height | 20.0mm |
| Block thickness | 6.0mm |
| Tube clearance bore | 8.0mm through |
| Inner lip bore | 10.5mm, 2.0mm deep |
| Outer bore (cradle) | 12.5mm, 2.0mm deep |
| Material | PETG |
| Print orientation | Bore axis vertical (Z-up) |
| Estimated print time | ~10-15 minutes |
| Estimated material | <2g |

**Test procedure**:
1. Insert 1/4" hard tubing through the test coupon's bore.
2. Insert the tubing into a John Guest fitting.
3. Slide the test coupon along the tubing toward the fitting until the cradle engages the collet ring.
4. Push the coupon axially toward the fitting (simulating plate actuation).
5. Feel for collet release. Can the tubing be withdrawn while the coupon holds the collet depressed?
6. Test centering: does the collet release cleanly, or does it cock/bind?
7. Intentionally tilt the coupon during release to find the tilt tolerance.

**Print 3 copies with variation**: One at nominal dimensions (8.0/10.5/12.5mm), one with inner lip bore at 10.0mm (tighter, more contact area), one at 11.0mm (looser, less contact area). This brackets the optimal lip diameter.

### 7.2 Four-Hole Plates

Print after validating the single-hole bore geometry. Use the bore dimensions confirmed by the test coupon.

#### Spec A: 2x2 Grid Plate

```
    Top view:
    ┌─────────────────────────────────────────────┐
    │  ○                                     ○    │
    │        O (1)              O (2)             │
    │                                             │
    │                                             │
    │        O (3)              O (4)             │
    │  ○                                     ○    │
    └─────────────────────────────────────────────┘

    ○ = guide pin holes (3.3mm dia)
    O = stepped bore (8.0/10.5/12.5mm)
```

| Dimension | Value |
|---|---|
| Plate width | 39.5mm |
| Plate height | 39.5mm |
| Plate thickness | 6.0mm |
| Bore center-to-center | 15.0mm (both axes) |
| Bore pattern center | Plate center |
| Guide pin holes | 4x 3.3mm diameter, 3.5mm from each corner |
| Guide pin hole slot length | 7.3mm (oriented along bore axis, i.e., through-plate) |
| Material | PETG |
| Estimated print time | ~30-45 minutes |
| Estimated material | ~8-10g |

Note: The 39.5mm dimension provides 3.0mm from the outermost bore edge (12.5mm/2 = 6.25mm from center) to the plate edge on each side: 15.0/2 + 6.25 + 3.0 = 16.75mm from center, x2 = 33.5mm for the bore area + 3.0mm margin on each side = 39.5mm. The extra margin accommodates guide pin holes at the corners.

#### Spec B: 4-in-a-Line Plate

```
    Top view:
    ┌──────────────────────────────────────────────────────────────────┐
    │  ○       O (1)        O (2)        O (3)        O (4)      ○   │
    └──────────────────────────────────────────────────────────────────┘

    ○ = guide pin holes (3.3mm dia)
    O = stepped bore
```

| Dimension | Value |
|---|---|
| Plate width | 69.5mm |
| Plate height | 18.5mm |
| Plate thickness | 6.0mm |
| Bore center-to-center | 15.0mm (horizontal) |
| Guide pin holes | 2x 3.3mm diameter, centered vertically, 3.5mm from each short edge |
| Guide pin slot length | 7.3mm |
| Material | PETG |
| Estimated print time | ~25-40 minutes |
| Estimated material | ~6-8g |

---

## 8. Interdependencies with Other Components

### 8.1 Mating Face (Dock)

The dock's mating face must have:
- 4 John Guest fittings mounted in the same pattern as the plate bores (15.0mm C-C for 2x2).
- Guide pin holes (or pin mounts) matching the plate's guide pin holes, with pins extending toward the incoming plate.
- Clearance behind the plate for the cam mechanism.
- The fitting body OD (~12.7mm) dictates the minimum feasible C-C spacing. At 15.0mm, fitting bodies have 2.3mm clearance. The dock mounting holes for the fittings must be precisely placed. The fitting body may have a hex wrench flat or molded feature that needs clearance -- **measure the actual fittings before finalizing dock design**.

### 8.2 Cam/Lever Mechanism

The cam acts on the back face of the plate. Key interface dimensions:
- Plate back face is flat, at a known distance from the fitting face (6.0mm behind the fitting-facing surface in the retracted position, 3.0mm in the actuated position -- because the plate moves 3.0mm).
- The cam must apply force at or near the plate center to minimize tilt. For the 2x2 grid, the center is at the geometric center of the 4 bores.
- The cam's eccentricity (1.0-1.5mm from cam-lever.md) produces 2.0-3.0mm of displacement. The plate travel spec (3.0mm) matches the upper end of this range.
- The cam pivot axis should be perpendicular to the plate's travel axis (i.e., the cam rotates in a plane parallel to the plate face).

### 8.3 Cartridge Body

The cartridge body houses the pumps and tube stubs. The tube stubs must align with the plate's tube clearance holes. For the 2x2 grid, this means 4 hard 1/4" OD tube stubs in a 15.0mm C-C grid on the cartridge face. The stubs pass through the plate's 8.0mm holes and into the John Guest fittings.

**Tube stub length**: Must be long enough to reach through the plate thickness (6.0mm) and into the fitting (15mm insertion depth per collet-release.md). Total stub length from cartridge face: 6.0mm (plate) + 3.0mm (plate travel gap when retracted) + 15.0mm (fitting insertion) = 24.0mm minimum. Add 2-3mm safety margin = **27mm stub length**.

### 8.4 Return Path

If the plate relies on collet spring-back for return (Section 4.4), the plate must have a feature that prevents it from separating from the cam. Either:
- The cam is captive in a slot in the plate (cam pushes and pulls), or
- The guide pins have retaining features (snap rings, shoulders) that limit the plate's outward travel, and the collet springs push the plate against these stops.

---

## 9. Recommendation Ranking

### Arrangement

1. **2x2 grid** -- smallest footprint (39.5mm square), best tilt resistance (10.6mm max moment arm), symmetric. Tight fitting clearance (2.3mm) is workable.
2. **4-in-a-line** -- simpler tube routing, but 63.5mm width and 22.5mm moment arm make tilt harder to control.
3. **Diamond / offset** -- no practical advantage, more complex.

### Compliance

1. **Overtravel** -- simplest, no additional parts. 3.0mm plate travel provides 1.0mm margin over 2.0mm collet travel. Test first.
2. **Elastomeric gasket** -- retrofit if overtravel causes excessive force at collet hard stops. 1.5mm silicone sheet.
3. **Spring-loaded lips** -- over-engineered. Only if variation exceeds 2mm.

### Guide Features

1. **Steel dowel pins through plate slots** (hybrid with bore self-centering) -- precise, simple hardware, tolerant of FDM variation.
2. **Integral printed rails** -- no hardware, but FDM surface finish and tolerance make smooth sliding difficult.
3. **Bore-guided only** -- works as secondary alignment but can't guide the approach.

### Material and Orientation

1. **PETG, bore axis vertical (Z-up)** -- best bore circularity, strongest lip walls, no supports needed.

---

## 10. Concrete First-Print Plan

### Print 1: Three Single-Hole Test Coupons

**Goal**: Validate bore geometry against actual fittings.

Three 20mm x 20mm x 6mm blocks, each with a single stepped bore:
- **Coupon A**: 8.0 / 10.5 / 12.5mm (nominal from collet-release.md)
- **Coupon B**: 8.0 / 10.0 / 12.5mm (tighter lip, more contact area)
- **Coupon C**: 8.0 / 11.0 / 12.5mm (looser lip, less contact area)

All three print on one bed in under 30 minutes. Test each against a John Guest fitting with tubing inserted. Record which coupon gives the cleanest release.

### Print 2: 2x2 Grid Plate (No Guide Pins)

**Goal**: Validate 4-hole spacing and simultaneous release.

39.5mm x 39.5mm x 6.0mm plate with 4 stepped bores at 15.0mm C-C in a 2x2 grid. No guide pin holes yet -- just hand-hold the plate against 4 fittings and press. Verify that all 4 collets release. Check for uneven engagement.

Use the bore dimensions validated by Print 1.

### Print 3: 2x2 Grid Plate with Guide Pin Holes

**Goal**: Test guided actuation.

Same as Print 2, but add 4 corner guide pin holes (3.3mm diameter, 7.3mm slot length). Purchase 4x 3mm steel dowel pins (40-50mm long). Build a simple test frame (3D printed) that holds the pins and 4 John Guest fittings. Test plate travel on the pins. Measure parallelism deviation with calipers at each corner.

### Hardware Needed for Testing

| Item | Qty | Source | Approximate Cost |
|---|---|---|---|
| 3mm x 50mm steel dowel pins | 4 | Amazon / McMaster-Carr | $5-8 |
| John Guest 1/4" push-to-connect unions | 4 | Already in hand | $0 |
| 1/4" OD hard tubing (short stubs) | 4x 50mm | Already in hand | $0 |
| 1.5mm silicone gasket sheet (optional) | 1 small piece | Amazon | $5-8 |
| PETG filament | ~20g total | Already in hand | ~$0.50 |

---

## Sources

- Collet-release.md (this project) -- bore dimensions, forces, failure modes, tolerances
- Guide-alignment.md (this project) -- FDM clearances, rail tolerances, sliding fit recommendations
- Cam-lever.md (this project) -- cam eccentricity, force multiplication, over-center behavior
- Electrical-mating.md (this project) -- contact placement relative to water fittings
- [John Guest 1/4" Push-to-Connect Technical Specifications](https://www.johnguest.com/sites/default/files/files/tech-spec-od-fittings-v2.pdf) -- fitting body dimensions
- [3D Printing Tolerances and Fits (Prusa Knowledge Base)](https://help.prusa3d.com/article/tolerance-and-engineering-fits_341710) -- FDM clearance guidelines for sliding fits
- [PETG Material Properties (MatWeb)](https://www.matweb.com/search/DataSheet.aspx?MatGUID=4de1c3bb37f04e7e959e286bf7474ae3) -- tensile strength, flexural modulus
- [Engineering Fit Tolerances for FDM (CNC Kitchen)](https://www.cnckitchen.com/blog/the-ultimate-engineering-fits-guide-for-3d-printing) -- clearance per side recommendations
- [Silicone Rubber Durometer and Compression Properties](https://www.stockwell.com/blog/silicone-rubber-hardness-durometer/) -- compression % vs durometer for elastomeric compliance layer
