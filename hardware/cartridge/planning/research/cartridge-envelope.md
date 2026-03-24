# Cartridge Envelope — Bounding Volume Design

Research and design exploration for the overall dimensions of the replaceable pump cartridge. The cartridge holds 2 Kamoer KPHM400 peristaltic pumps, routes tubing from pump inlet/outlet ports to 4 tube stubs on the mating face, and must fit comfortably under a kitchen sink.

This document is driven by pump dimensions, tubing routing constraints, and the mating face layout established in prior research (collet-release.md, electrical-mating.md, guide-alignment.md, cam-lever.md).

---

## 1. Pump Dimensions — Kamoer KPHM400

### Source Data

The pump in use is the **Kamoer KPHM400-SW3B25**: 12V DC brushed motor, 3 rollers, 400ml/min flow rate, BPT tubing (4.8mm ID x 8.0mm OD). Dimensional data is from the official Kamoer KPHM400 datasheet (page 29 of the product manual, available as an Amazon-hosted PDF).

Model code breakdown: KPHM400-**SW**3**B25** — SW = 12V brushed motor, 3 = 3 rollers, B25 = BPT tube 4.8x8mm.

### Overall Dimensions

| Dimension | Value | Notes |
|-----------|-------|-------|
| Length (motor axis direction) | 68.6 mm | Motor shaft to pump head front face |
| Width | 62.5 mm | Across pump head, perpendicular to motor axis |
| Height | 62.7 mm | Top to bottom of pump body |
| Weight | ~380 g | Per pump, from Amazon listing |

The pump is roughly a 63mm cube with the motor extending one side to ~69mm total along the motor axis.

### Dimensional Drawing Detail

From the datasheet drawing (dimensions in mm):

```
                         TOP VIEW
    ┌──────────────────────────────────────┐
    │                                      │
    │         Motor body                   │
    │         (cylindrical, dia 36)        │
    │                                      │
    │     ┌────────────────────┐           │
    │     │    Pump head       │           │
    │     │    (62.5 wide)     │           │
    │     └────────────────────┘           │
    │                                      │
    └──────────────────────────────────────┘
    ├────────── 68.6 ─────────────────────►│

                        SIDE VIEW
    ┌──────────────────────────────────────┐
    │  ○ ○  inlet/outlet barbs (top)       │
    │                                      │
    │     ┌────────────────────┐           │
    │     │    Pump head       │  62.7     │
    │     │                    │  height   │
    │     └────────────────────┘           │
    │         Motor body (dia 36)          │
    └──────────────────────────────────────┘
```

### Motor Dimensions

The DC brushed motor is cylindrical:

| Parameter | Value |
|-----------|-------|
| Motor body diameter | 36 mm |
| Motor body length | ~40 mm (from pump head rear face) |
| Motor + gearbox total length | ~52.7 mm (inferred: 68.6 - ~16 pump head depth) |

### Mounting Hole Pattern (DC Motor Variant)

The datasheet shows a suggested mounting hole pattern for the DC motor variant:

| Parameter | Value |
|-----------|-------|
| Mounting plate size | 50 x 50 mm |
| Hole pattern | 4 holes at corners |
| Hole spacing | ~56 mm diagonal (4x holes at 4mm diameter) |
| Hole diameter | 4 mm (for M4 bolts) |
| Tolerance | +0.1 / -0 mm |
| Center bore (motor clearance) | 36 mm diameter |

The mounting plate is oriented perpendicular to the motor axis — the pump mounts from the back (motor side) onto a vertical plate.

### Inlet/Outlet Port Positions

The inlet and outlet barb fittings protrude from the **top of the pump head**. From the datasheet drawing:

| Parameter | Value |
|-----------|-------|
| Port orientation | Vertical (upward from pump head top face) |
| Port spacing (center-to-center) | ~20-25 mm (estimated from drawing proportions) |
| Barb OD | ~8 mm (matches 4.8mm ID x 8mm OD BPT tubing) |
| Barb protrusion above pump body | ~15-20 mm |
| Position along length axis | Both ports near the front face of the pump head |

The ports exit the pump head **perpendicular to the motor axis** (pointing up when the pump is mounted motor-horizontal). This is important for tubing routing — the tubes exit vertically from the pump and must be routed to the mating face.

### Key Takeaway for Envelope Design

Each pump occupies approximately a **70 x 63 x 63 mm** box (L x W x H), plus ~20mm above for barb fittings. The motor axis is the longest dimension. Two pumps side by side will be approximately 126mm wide; two pumps stacked will be approximately 126mm tall (before adding tube routing space).

---

## 2. Pump Arrangements

With 2 pumps to fit in the cartridge, there are several possible arrangements. Each has different implications for cartridge width, height, depth, and tubing routing.

### Coordinate System

For all arrangements below:
- **Width (W)**: left-right as seen from the mating face
- **Height (H)**: top-bottom (gravity direction)
- **Depth (D)**: front-back (insertion axis, mating face at front)

### Arrangement A: Side by Side, Same Orientation

Both pumps mounted with motor axes parallel, side by side horizontally.

```
    FRONT VIEW (looking at mating face)

    ┌─────────────┬─────────────┐
    │  ↑barbs ↑   │  ↑barbs ↑   │
    │  Pump 1     │  Pump 2     │
    │  [head]     │  [head]     │
    │  [motor]    │  [motor]    │
    └─────────────┴─────────────┘

    TOP VIEW (looking down)

    ┌─────────────┬─────────────┐
    │  motor 1    │  motor 2    │
    │  ──────►    │  ──────►    │
    │  [head]     │  [head]     │
    └─────────────┴─────────────┘
    ▲ mating face
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~126 mm | 2 x 63mm pump width |
| Height | ~83 mm | 63mm pump + 20mm barb clearance above |
| Depth | ~69 mm | Single pump length (motor axis) |

**Port positions**: All 4 barbs exit from the top of the cartridge. Clean routing: tubes bend forward (toward mating face) and then down to the tube stubs.

**Pros**: Shortest depth. Simplest wiring (motors side by side). Natural layout — wide and shallow, fits under sink horizontally.

**Cons**: Widest arrangement. All barbs on top requires vertical space for tube routing.

### Arrangement B: Stacked Vertically

Both pumps stacked one above the other, motor axes horizontal.

```
    FRONT VIEW (looking at mating face)

    ┌─────────────┐
    │  ↑barbs ↑   │
    │  Pump 2     │
    │  [head]     │
    │  [motor]    │
    ├─────────────┤
    │  ↑barbs ↑   │
    │  Pump 1     │
    │  [head]     │
    │  [motor]    │
    └─────────────┘
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~63 mm | Single pump width |
| Height | ~146 mm | 2 x 63mm + 20mm barb clearance for top pump |
| Depth | ~69 mm | Single pump length |

**Port positions**: Each pump's barbs exit upward. Bottom pump's barbs must route around or through the top pump's space — problematic.

**Pros**: Narrowest arrangement. Good if horizontal space is constrained.

**Cons**: Tallest arrangement (146mm = ~5.7"). Bottom pump's tube routing is blocked by top pump. Under-sink height may be tight. Heavy center of gravity when tall — less stable during insertion.

### Arrangement C: Head-to-Tail (Motors Opposing)

One pump flipped 180 degrees so the motors point in opposite directions. Pumps side by side.

```
    TOP VIEW (looking down)

    ┌─────────────┬─────────────┐
    │  motor 1    │    2 motor  │
    │  ──────►    │  ◄──────    │
    │  [head]     │  [head]     │
    └─────────────┴─────────────┘
    ▲ mating face
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~126 mm | 2 x 63mm pump width |
| Height | ~83 mm | 63mm + 20mm barb clearance |
| Depth | ~69 mm | Same as side-by-side (motors cancel out) |

**Port positions**: Same as Arrangement A — barbs still exit from top.

**Pros**: Motor weight distributed symmetrically front/back. Identical envelope to Arrangement A.

**Cons**: Same width as A. Wiring slightly more complex (motor leads exit opposite ends). No real advantage over Arrangement A.

### Arrangement D: Perpendicular / L-Shaped

One pump rotated 90 degrees relative to the other. Motor axes perpendicular.

```
    TOP VIEW (looking down)

    ┌─────────────┐
    │  motor 1    │
    │  ──────►    │
    │  [head]     │
    ├─────────┐   │
    │ motor 2 │   │
    │   │     │   │
    │   ▼     │   │
    │ [head]  │   │
    └─────────┘   │
                  │
    ▲ mating face
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~132 mm | 69mm (pump 1 depth) + 63mm (pump 2 width) |
| Height | ~83 mm | Still limited by barb clearance |
| Depth | ~132 mm | 69mm (pump 2 depth) + 63mm (pump 1 width) |

**Port positions**: Barbs exit in different directions (up for pump 1, up-but-rotated for pump 2). Complex routing.

**Pros**: Interesting for corner installations. Could reduce one dimension at the expense of another.

**Cons**: Largest footprint. Complex tube routing. Asymmetric — harder to handle. No clear advantage for an under-sink installation.

### Arrangement E: Inline (Motors End-to-End)

Both pumps in a line along the depth axis, one behind the other.

```
    TOP VIEW (looking down)

    ┌─────────────────────────────────┐
    │  motor 1 ──► [head1] motor 2 ──► [head2]  │
    └─────────────────────────────────┘
    ▲ mating face
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~63 mm | Single pump width |
| Height | ~83 mm | 63mm + 20mm barb clearance |
| Depth | ~138 mm | 2 x 69mm pump length |

**Port positions**: Both sets of barbs on top, but the rear pump's barbs are ~140mm from the mating face.

**Pros**: Narrowest footprint (63mm wide). Slim profile.

**Cons**: Very deep — nearly 6 inches. Rear pump's tubes must route a long distance to the mating face. Requires deep under-sink clearance.

---

## 3. Tubing Routing Constraints

### Tubing Specifications

The cartridge uses two types of tubing:

| Tubing | ID | OD | Material | Use |
|--------|----|----|----------|-----|
| Pump internal tubing | 4.8 mm | 8.0 mm | BPT (PharMed) | Inside pump head, pre-installed |
| External routing + mating stubs | 1/8" (3.2 mm) | 1/4" (6.35 mm) | Hard tubing (nylon/polyethylene) | Push-connect compatibility |

**Critical note from project context**: Soft silicone tubing does NOT hold in push-connect fittings — it deforms and slips out. The tube stubs on the mating face MUST be hard 1/4" OD tubing (nylon or polyethylene).

### Transition Between Pump Barbs and Mating Stubs

The pump barbs are 8mm OD, fitting the 4.8mm ID BPT pump tubing. The mating face stubs are 1/4" (6.35mm) OD hard tubing. A transition is needed:

**Option 1 — Barb-to-push-connect adapter**: A small fitting that accepts the pump's BPT tubing on one end and has a 1/4" OD hard tube stub on the other. This could be a short piece of tubing with a reducer barb, or a custom 3D printed manifold.

**Option 2 — Direct barb-to-barb with hard tube**: Replace the BPT tubing with 1/4" OD hard tubing that barbs directly onto the pump ports (if the pump barb accepts it). Unlikely — the pump barb is sized for 8mm OD tubing.

**Option 3 (recommended) — Short BPT tube to a barb reducer, then hard tube to mating face**: Pump barb → ~50mm BPT tubing → reducer barb fitting → 1/4" OD hard tubing → mating face stub. The BPT section provides flexibility; the hard tube provides push-connect compatibility.

### Minimum Bend Radius

For routing tubes from pump barbs (on top of the pump) to the mating face (on the front), the tubing must make at least one 90-degree bend.

**Soft silicone/BPT tubing (4.8mm ID x 8mm OD):**

| Source | Minimum Bend Radius | Notes |
|--------|---------------------|-------|
| General rule (2-2.5x OD) | 16-20 mm | Conservative for soft tubing |
| General rule (3-4x OD) | 24-32 mm | Very conservative, prevents any kinking |
| Practical (thick-wall soft silicone) | ~10-15 mm | Can achieve tight bends when tubing wall is >1.5mm |

**Hard tubing (1/4" OD nylon/polyethylene):**

| Source | Minimum Bend Radius | Notes |
|--------|---------------------|-------|
| General rule (4-6x OD) | 25-38 mm (~1-1.5") | Standard for semi-rigid plastic tubing |
| With heat forming | 15-20 mm | Possible with nylon, heat-bent to shape |
| Without forming (cold bend) | ~30-40 mm | Safe cold bend for 1/4" nylon |

**Design implication**: If using hard tubing for the routing (not just the mating stubs), each 90-degree bend needs ~30-40mm of clearance. If using BPT/silicone for routing with only the last stub being hard tubing, the bend radius drops to ~15-20mm.

### Recommended Routing Strategy

Use **soft BPT tubing** for all internal routing (from pump barbs through bends), transitioning to **hard 1/4" OD tube stubs** only for the last ~20-25mm that protrude through the mating face. This minimizes bend radius constraints and keeps the cartridge compact.

With BPT tubing at 15-20mm bend radius, a 90-degree turn from vertical (pump barb) to horizontal (toward mating face) needs approximately:

```
    Pump barb (pointing up)
         │
         │  ~15-20mm
         │  bend radius
         └──────────── toward mating face

    Space needed above pump for bend: ~15-20mm
    Space needed in front of pump for bend: ~15-20mm
```

This adds approximately **20mm** to the height (above pump) and **20mm** to the depth (in front of pump head) for tubing routing.

### Tube Stub Layout on Mating Face

From collet-release.md, each John Guest fitting has:
- Fitting body OD: ~12.7mm
- Collet ring OD: ~11.4mm
- Outer bore (cradle) on release plate: 12.5mm diameter

Minimum fitting-to-fitting spacing (center-to-center) must clear the outer bore diameter plus wall material:

| Parameter | Value | Notes |
|-----------|-------|-------|
| Minimum C-C spacing | ~18-20 mm | 12.5mm bore + 2x 2.5mm wall minimum |
| Comfortable C-C spacing | 22-25 mm | Allows thicker walls, easier printing |

Four fittings can be arranged:

**2x2 grid:**
```
    ○  ○       ~20-25mm spacing
    ○  ○
```
- Grid width: 20-25mm
- Grid height: 20-25mm
- Compact, but collet release plate must depress all 4 simultaneously

**1x4 line (horizontal):**
```
    ○  ○  ○  ○
```
- Line width: 60-75mm
- Line height: 0 (single row)
- Wide, but simpler release plate (linear motion)

**2x2 grid is strongly preferred**: keeps the mating face compact and makes the release plate a simple square.

With a 2x2 grid at 22mm spacing, the tube stub zone occupies approximately **35 x 35 mm** on the mating face (including fitting body clearance).

---

## 4. Mating Face Layout

The mating face must accommodate:
1. **4 tube stubs** (2x2 grid, ~35 x 35mm zone)
2. **3 electrical pads** (pogo pin targets, ~30 x 10mm zone)
3. **2 alignment features** (tapered pins or rail engagement, ~10mm diameter each)
4. **Release plate clearance** (must overlap tube stub zone, ~3mm travel)
5. **Cam/lever mechanism** clearance

From electrical-mating.md: electrical contacts should be on a **different face** than water fittings, or if on the same face, electrical above water with a dam between them.

### Recommended Mating Face Layout (Same Face)

If all features share the front mating face:

```
    ┌───────────────────────────────────────────┐
    │                                           │
    │   ○ align pin                align pin ○  │
    │                                           │
    │          [=] [=] [=]                      │
    │          electrical pads                  │
    │     ─────── dam/ridge ───────             │
    │                                           │
    │           ○    ○                          │
    │           tube stubs                      │
    │           ○    ○                          │
    │                                           │
    └───────────────────────────────────────────┘

    Minimum face dimensions:
      Width:  ~60mm (tube grid + alignment pins + margin)
      Height: ~80mm (alignment + electrical + dam + tubes + margin)
```

### Recommended Mating Face Layout (Split Face)

Electrical pads on the **top face** of the cartridge; water fittings on the **front face**:

```
    TOP FACE:
    ┌─────────────────────┐
    │  [=] [=] [=]        │   Electrical pads (pogo pin targets)
    └─────────────────────┘

    FRONT FACE (mating face):
    ┌───────────────────────────────────────────┐
    │                                           │
    │   ○ align pin                align pin ○  │
    │                                           │
    │           ○    ○                          │
    │           tube stubs                      │
    │           ○    ○                          │
    │                                           │
    └───────────────────────────────────────────┘

    Front face minimum dimensions:
      Width:  ~60mm (tube grid + alignment pins + margin)
      Height: ~50mm (alignment + tubes + margin)
```

The split-face layout is preferred for moisture isolation and results in a smaller front mating face.

---

## 5. Minimum Envelope Estimates

For each pump arrangement, we add:
- **Tube routing space**: +20mm above pump for bends, +20mm in front of pump head
- **Housing walls**: 3mm per side (PETG, structural minimum for FDM)
- **Mating face hardware**: +5mm depth for tube stub protrusion (inside the dock, outside the cartridge envelope)
- **Electrical pads**: negligible added volume if on top face (flush pads)
- **Alignment features**: included in mating face zone

### Arrangement A: Side by Side (Same Orientation)

```
    Width  = 2 × 63 (pumps) + 2 × 3 (walls) + 5 (center gap) = 137 mm
    Height = 63 (pump) + 20 (tube routing) + 2 × 3 (walls) = 89 mm
    Depth  = 69 (pump) + 20 (tube routing to face) + 2 × 3 (walls) = 95 mm
```

**Envelope: ~137 x 89 x 95 mm (W x H x D)** — approximately 5.4" x 3.5" x 3.7"

### Arrangement B: Stacked Vertically

```
    Width  = 63 (pump) + 2 × 3 (walls) = 69 mm
    Height = 2 × 63 (pumps) + 20 (tube routing top) + 5 (gap) + 2 × 3 (walls) = 157 mm
    Depth  = 69 (pump) + 20 (tube routing to face) + 2 × 3 (walls) = 95 mm
```

**Envelope: ~69 x 157 x 95 mm (W x H x D)** — approximately 2.7" x 6.2" x 3.7"

### Arrangement C: Head-to-Tail (Motors Opposing)

Same footprint as Arrangement A (motors flip direction but occupy the same volume):

**Envelope: ~137 x 89 x 95 mm (W x H x D)** — same as A

### Arrangement E: Inline (End-to-End)

```
    Width  = 63 (pump) + 2 × 3 (walls) = 69 mm
    Height = 63 (pump) + 20 (tube routing) + 2 × 3 (walls) = 89 mm
    Depth  = 2 × 69 (pumps) + 20 (tube routing to face) + 5 (gap) + 2 × 3 (walls) = 169 mm
```

**Envelope: ~69 x 89 x 169 mm (W x H x D)** — approximately 2.7" x 3.5" x 6.7"

### Arrangement D: Perpendicular / L-Shaped

```
    Width  = 69 (pump 1 depth) + 63 (pump 2 width) + 2 × 3 (walls) = 138 mm
    Height = 63 (tallest pump) + 20 (tube routing) + 2 × 3 (walls) = 89 mm
    Depth  = 69 (pump 2 depth) + 63 (pump 1 width) + 2 × 3 (walls) = 138 mm
```

**Envelope: ~138 x 89 x 138 mm (W x H x D)** — approximately 5.4" x 3.5" x 5.4"

### Comparison Table

| Arrangement | W (mm) | H (mm) | D (mm) | Volume (L) | Shape Factor | Tube Routing Complexity |
|-------------|--------|--------|--------|------------|--------------|------------------------|
| **A: Side by side** | 137 | 89 | 95 | **1.16** | Wide, shallow | Simple — all barbs on top |
| B: Stacked | 69 | 157 | 95 | **1.03** | Tall, narrow | Hard — bottom pump blocked |
| C: Head-to-tail | 137 | 89 | 95 | **1.16** | Wide, shallow | Same as A |
| D: Perpendicular | 138 | 89 | 138 | **1.70** | Square, bulky | Complex — ports in different planes |
| **E: Inline** | 69 | 89 | 169 | **1.04** | Narrow, deep | Moderate — rear pump far from face |

---

## 6. Under-Sink Space Context

### Typical Under-Sink Cabinet Dimensions

Standard US kitchen sink base cabinets (from cabinetry dimension guides):

| Dimension | Typical Range | Notes |
|-----------|--------------|-------|
| Cabinet width (exterior) | 30" - 42" (760 - 1070 mm) | 36" most common |
| Cabinet interior width | 28.5" - 40.5" (720 - 1030 mm) | 1.5" less than exterior |
| Cabinet depth (front-to-back) | 24" (610 mm) | Standard countertop depth |
| Cabinet interior depth | ~22" (560 mm) | After face frame |
| Cabinet height (floor to countertop bottom) | ~28" (710 mm) | 34.5" cabinet - 6" toe kick |

### Available Space (After Plumbing)

Under a kitchen sink, the actual available space is significantly reduced by:
- **P-trap and drain pipes**: occupy center area, typically 6-8" wide
- **Water supply lines** (hot/cold shutoff valves): 2-4" from back wall
- **Garbage disposal** (if present): hangs from sink, 6-8" diameter x 12-15" tall
- **Water filter** (if present): typically on the back wall or side

**Realistic available volume for accessories**: Two zones, one on each side of the P-trap:
- Each zone: approximately **10-14" wide x 16-20" deep x 20-24" tall** (250-350mm x 400-500mm x 500-600mm)

### Cartridge Size Comfort Assessment

| Cartridge Size | Fit Under Sink | One-Handed Handling | Notes |
|---------------|----------------|---------------------|-------|
| 137 x 89 x 95 mm (Arr. A) | Very comfortable | Easy | Fits in one hand, like a thick paperback |
| 69 x 157 x 95 mm (Arr. B) | Comfortable | Awkward | Tall + narrow = tippy, hard to grip |
| 69 x 89 x 169 mm (Arr. E) | Comfortable | Moderate | Long but narrow, like a TV remote |
| 138 x 89 x 138 mm (Arr. D) | Comfortable | Bulky | Square footprint, harder to orient |

All proposed envelopes are well within the available under-sink space. The limiting factor is not space — it is **handling ergonomics** and **dock depth** (how far the cartridge protrudes from the back/side wall where the dock is mounted).

### Dock Mounting Considerations

The dock is permanently mounted (screwed to the inside of the cabinet). The cartridge slides into the dock. The total protrusion from the mounting surface includes:
- Dock body: ~20-30mm (rails, fitting holders, back plate)
- Cartridge depth: 95-169mm depending on arrangement
- Lever/cam mechanism: ~10-20mm in front of cartridge

**Total protrusion from mounting surface**: approximately 125-220mm (5-9 inches). For a cabinet with 560mm (22") interior depth, even the deepest arrangement leaves ~340mm (13.5") of clearance in front of the dock.

---

## 7. Weight Estimate

| Component | Weight | Qty | Subtotal |
|-----------|--------|-----|----------|
| Kamoer KPHM400 pump | ~380 g | 2 | 760 g |
| BPT tubing (internal routing, ~400mm total) | ~15 g | 1 | 15 g |
| Hard tube stubs (4x, ~30mm each) | ~2 g | 4 | 8 g |
| 3D printed PETG housing (3mm walls, ~137x89x95 shell) | ~100-150 g | 1 | ~125 g |
| Barb reducer fittings | ~5 g | 4 | 20 g |
| Brass electrical pads (3x) | ~5 g | 3 | 15 g |
| **Total estimated** | | | **~940 g (~2.1 lbs)** |

### Handling Assessment

- 940g / 2.1 lbs is well within comfortable one-handed handling
- For reference: a standard can of soda is 368g; this is about 2.5 cans
- The center of gravity will be dominated by the two pumps (81% of total weight)
- Side-by-side arrangement (A) distributes weight symmetrically left-right, making it the most natural to grip

---

## 8. Interdependencies with Other Components

### Release Plate (from collet-release.md)

The release plate sits between the dock and the cartridge mating face. It must:
- Travel 3mm axially (min 2.5mm)
- Accommodate 4 stepped bores at the tube stub positions
- Total actuation force: 12-20N

**Impact on envelope**: The release plate adds ~5mm to the effective depth of the dock (not the cartridge). The tube stub positions on the cartridge mating face must match the release plate hole pattern exactly. The 2x2 grid spacing chosen here (22mm C-C) must be carried through to the release plate design.

### Cam/Lever Mechanism (from cam-lever.md)

The lever must be accessible from outside the dock. Eccentric cam with 1-1.5mm eccentricity, lever length 50-100mm.

**Impact on envelope**: The lever pivots on the dock (not the cartridge). The cam pushes the release plate. The cartridge envelope itself is not affected, but the dock's external dimensions grow by the lever length on one side. For the side-by-side arrangement (137mm wide), the lever could extend from either side of the dock, adding ~50-100mm to the total dock width.

### Guide Rails (from guide-alignment.md)

FDM tolerance: 0.3-0.5mm clearance per side for sliding fits in PETG. The guide rails run the full depth of the dock.

**Impact on envelope**: Rail channels add ~3-5mm to the cartridge width and height (grooves in the cartridge sides that mate with rails in the dock). This is already accounted for in the 3mm wall allowance. The cartridge's exterior dimensions are the rail-to-rail dimensions.

### Electrical Contacts (from electrical-mating.md)

3 pogo pins in dock, 3 flat pads on cartridge. Recommended: different face than water fittings.

**Impact on envelope**: If pads are on the top face, the cartridge height must include a flat zone for the pads (~10mm wide strip). Already within the 89mm height. If pads are on the mating face, the mating face grows taller by ~15mm (pads + dam + clearance).

### Pump Mounting

Pumps mount via their 50x50mm bolt pattern (4x M4 holes). Inside the cartridge, pumps can be bolted to internal ribs or shelves printed into the housing. The mounting orientation (which face of the pump contacts the mount) affects:
- Vibration isolation: mounting from the motor side is standard
- Tube routing: pump head must face toward the mating face for shortest tube runs

**Recommended mounting**: Motor backs against an internal vertical rib, pump heads facing forward (toward mating face). M4 bolts through the rib into the pump mounting plate.

---

## 9. Scenario Ranking and Recommendation

### Scoring (1-5, 5 = best)

| Criteria | Weight | A: Side-by-Side | B: Stacked | C: Head-to-Tail | D: Perpendicular | E: Inline |
|----------|--------|-----------------|------------|-----------------|-------------------|-----------|
| Compact volume | 3 | 4 | 4 | 4 | 2 | 4 |
| Tube routing simplicity | 4 | 5 | 2 | 5 | 2 | 3 |
| One-hand ergonomics | 3 | 5 | 2 | 5 | 3 | 4 |
| Mating face compactness | 3 | 4 | 4 | 4 | 3 | 4 |
| Symmetric weight | 2 | 5 | 4 | 5 | 2 | 3 |
| Dock simplicity | 2 | 4 | 3 | 4 | 2 | 4 |
| Wiring simplicity | 1 | 5 | 4 | 3 | 3 | 4 |
| **Weighted Score** | | **82** | **54** | **79** | **43** | **65** |

### Ranking

1. **Arrangement A: Side by Side** — Score 82. Best overall. Simple tube routing, symmetric handling, compact depth. The 137mm width is easily accommodated under a sink. This is the natural arrangement and the one to prototype first.

2. **Arrangement C: Head-to-Tail** — Score 79. Nearly identical to A in every way. Only slightly worse due to opposing motor lead routing. No practical reason to choose this over A unless a specific wiring constraint emerges.

3. **Arrangement E: Inline** — Score 65. Good narrow profile, but the 169mm depth means the rear pump's tubes must travel a long distance. Viable if width is constrained (e.g., mounting between drain pipes).

4. **Arrangement B: Stacked** — Score 54. The bottom pump's tube routing is the fatal flaw — tubes must route around or through the top pump. Tall profile makes handling awkward.

5. **Arrangement D: Perpendicular** — Score 43. Largest volume, asymmetric, complex routing. No advantages for this application.

### Recommended Envelope

**Side-by-side (Arrangement A), target dimensions:**

```
    ┌───────────────────────────────────────────────┐
    │                                               │
    │              137 mm (5.4")                     │
    │   ┌────────────────────────────────────────┐  │
    │   │                                        │  │ 89 mm
    │   │     Pump 1          Pump 2             │  │ (3.5")
    │   │     [====]          [====]             │  │
    │   │                                        │  │
    │   └────────────────────────────────────────┘  │
    │              95 mm (3.7") depth                │
    └───────────────────────────────────────────────┘

    Weight: ~940g (2.1 lbs)
    Volume: ~1.16 L
```

With margins for iteration, a **target envelope of 140 x 90 x 100 mm** provides comfortable room for the side-by-side arrangement with internal tube routing.

---

## Sources

- [Kamoer KPHM400 Datasheet / Product Manual (Amazon PDF)](https://m.media-amazon.com/images/I/A1at7U9PyNL.pdf) — Dimensional drawing, mounting hole pattern, tube specifications
- [Kamoer KPHM400 Amazon Listing (B09MS6C91D)](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D) — Product weight, specifications
- [Kamoer KPHM400 Official Product Page](https://www.kamoer.com/us/product/detail.html?id=10014) — Model information
- [KPHM400 Data Sheet — DirectIndustry](https://pdf.directindustry.com/pdf/kamoer-fluid-tech-shanghai-co-ltd/kphm400-peristaltic-pump-data-sheet/242598-1017430.html) — Technical specifications
- [Silicone Tubing Bend Radius — Zeus Inc.](https://www.zeusinc.com/resources/summary-material-properties/bend-radius/) — Bend radius engineering data
- [Understanding Minimum Bend Radius for Hoses — Jay20hose.com](https://jay20hose.com/blogs/news/understanding-minimum-bend-radius-for-hoses) — General hose bend radius guidelines
- [Silicone Hose Bending Techniques — CNTOPA](https://cntopa.com/silicone-hose-bending-techniques-a-complete-guide.html) — Silicone tubing bending practices
- [Sink Base Cabinet Dimensions Guide — Casta Cabinetry](https://castacabinetry.com/post/sink-base-cabinet-dimensions/) — Under-sink space reference
- [Standard Kitchen Cabinet Sizes — Kitchen Cabinet Kings](https://kitchencabinetkings.com/guides/kitchen-cabinet-sizes) — Cabinet dimension standards
- [Kitchen Cabinet Dimensions — Builders Surplus](https://www.builderssurplus.net/kitchen-cabinet-dimensions-and-measurements-guide/) — Cabinet interior clearance data
