# Mating Face Layout — Research & Design

The mating face is the interface between the cartridge back wall and the dock front wall. Every connection crosses this boundary: 4 tube ports, a release plate pocket, electrical contacts (or routing to them), and guide/alignment features. This document explores how to arrange all of these in a way that is physically compatible, printable, and easy to assemble.

This is the keystone layout decision. Once the mating face geometry is committed, it constrains the cartridge body dimensions, the dock cavity, the release plate shape, and the cam lever placement. Everything else flows from here.

**Established parameters from Round 1 research (not re-derived here):**

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
| Electrical contacts | 3 pogo pins, dock side | electrical-mating.md |
| Contact pad size | ~8mm x 5mm each | electrical-mating.md |
| Electrical-to-water separation | 10-20mm minimum | electrical-mating.md |
| FDM sliding clearance | 0.3-0.5mm per side | guide-alignment.md |
| Alignment pins | 15-20 deg taper, 8-10mm base | guide-alignment.md |
| Cam eccentricity | 1-1.5mm for 2-3mm stroke | cam-lever.md |

---

## 1. Fitting Mounting in the Dock

Before exploring tube port arrangements, the fitting mounting method constrains minimum port spacing.

### How John Guest Fittings Are Mounted

John Guest push-to-connect fittings come in several mounting configurations:

**Bulkhead / panel-mount fittings (e.g., PMI series):**
- A male-threaded body passes through a panel hole, secured with a locknut on the back side.
- Thread size for 1/4" tube: 1/4" NPTF or 3/8" UNF (depending on model).
- Panel hole: ~10-12mm for 1/4" tube fittings.
- The collet face sits flush with (or protrudes slightly from) the panel front.
- This is the cleanest mounting option for the dock wall.

**Inline fittings (union connectors, elbows, tees):**
- No integral mounting feature. Must be held by the tubing and/or clamped/bonded into a housing.
- The fitting body is cylindrical, ~12.7mm OD for 1/4" tube size.
- Can be press-fit into a 3D-printed pocket with 0.1-0.15mm interference, or epoxied.
- A 3D-printed cradle with a snap feature or zip-tie groove is a more reliable approach than press-fit alone.

**Stem fittings:**
- A barbed or push-fit stem on one end, push-to-connect on the other.
- Useful when the dock-side plumbing is different from the cartridge-side.

### Mounting Method for This Application

The dock wall needs 4 fittings mounted with their collet faces accessible from the cartridge side. Two practical approaches:

**Option A: Panel-mount bulkhead fittings.** Drill (or print) 4 holes in the dock wall, pass the threaded fitting bodies through, secure with locknuts from behind. The collet faces protrude into the cartridge cavity. This is the most secure mounting and allows easy fitting replacement.

**Option B: Inline fittings in printed pockets.** The dock wall has 4 cylindrical bores (~12.9-13.0mm ID, sized for a friction fit around the 12.7mm fitting body). Each fitting is pressed in from behind and retained by a printed lip or snap ring at the front. Cheaper (inline fittings cost less than bulkhead fittings) but less secure.

**Recommendation:** Option A (panel-mount bulkhead) for the final design. Option B is acceptable for early prototyping since inline fittings are already in hand.

### Minimum Fitting Spacing

The fitting body OD is ~12.7mm. The release plate outer bore (cradle) is 12.5mm per hole. The minimum center-to-center spacing is constrained by:

1. **Fitting body clearance:** 12.7mm body means centers must be at least 12.7mm + wall thickness between bores. With 2mm minimum wall between adjacent bores in FDM: **14.7mm minimum center-to-center.**

2. **Release plate cradle clearance:** 12.5mm outer bore means the plate needs at least 12.5mm + wall between adjacent cradles. With 2mm wall: **14.5mm minimum center-to-center.**

3. **Practical minimum for FDM:** Add 0.5mm margin for print tolerance: **~15mm minimum center-to-center.** This is tight. 16-18mm provides more comfortable margins for FDM and allows space for the fitting locknut (if using bulkhead fittings).

4. **Panel-mount locknut clearance:** If using bulkhead fittings, the locknut hex is typically 16-19mm across flats. This sets a practical minimum of **~20mm center-to-center** for bulkhead fittings, unless locknuts are installed before printing the dock wall around them (possible but fiddly).

**Working minimum center-to-center spacing: 18mm** (comfortable for inline fittings, tight for bulkhead). **20mm** if using bulkhead fittings with locknuts.

---

## 2. Tube Port Layout Options

The 4 ports serve 2 pumps, each with an inlet and an outlet. The arrangement affects:
- Mating face width and height
- Tubing routing inside the cartridge (how much tubing has to cross or loop)
- Release plate shape (rectangular, square, or unusual)
- Cam lever force distribution (centered vs. off-center load)

### What the Pump Layout Suggests

Each Kamoer pump has an inlet stub and an outlet stub. The stubs emerge from the same side of the pump head, spaced roughly 25-30mm apart (center-to-center, based on typical peristaltic pump head geometry). The pump bodies are roughly 60mm x 50mm x 40mm.

Two pumps can be arranged in the cartridge as:
- **Side by side** (pump heads facing the back wall): Both inlet/outlet stub pairs are at the same depth, all 4 stubs naturally form a row or 2x2 grid depending on pump orientation.
- **Stacked** (one above the other): Tube stubs at two different heights, suggesting a vertically spaced arrangement.
- **Rotated** (one pump rotated 180 degrees relative to the other): Can be used to bring all stubs to the same side while varying the pattern.

The simplest tubing routing occurs when the tube stubs on the mating face match the natural spacing of the pump head stubs, minimizing bends and tubing length inside the cartridge.

### Layout A: Horizontal Line (4 in a Row)

```
     ┌──────────────────────────────────────────┐
     │                                          │
     │     O1       O2       O3       O4        │
     │                                          │
     └──────────────────────────────────────────┘

     Dimensions at 18mm spacing:
     Total width:  3 x 18 = 54mm between outermost centers
     Face width:   54 + 13 (half fitting each side) = ~67mm minimum
     Face height:  ~20mm (one fitting diameter + wall)
```

**Release plate:** A narrow horizontal bar, ~67mm x ~18mm, with 4 holes in a line. The plate slides axially (into the page) to press all 4 collets.

**Pros:**
- Widest but shortest face -- good if vertical space is limited.
- Release plate is a simple bar shape, easy to guide with two rail slots.
- Cam lever can push the center of the bar for balanced force.

**Cons:**
- Wide face means a wider cartridge body and dock cavity.
- Tubing inside the cartridge must route from 2 pump heads (each with 2 stubs ~25-30mm apart) to 4 ports spaced 18mm apart in a line. The outer two ports will need longer tubing runs.
- 54mm span means the release plate must be very rigid to avoid bowing when the cam pushes the center. A 67mm x 18mm x 3mm plate in PETG will flex measurably.

**Plate rigidity concern:** For a 67mm-span plate pushed at the center, deflection under 20N load with a 3mm thick PETG plate is roughly:

- PETG flexural modulus: ~2000 MPa
- Simplified beam: delta = F*L^3 / (48*E*I)
- I = b*h^3/12 = 18 * 3^3 / 12 = 40.5 mm^4
- delta = 20 * 67^3 / (48 * 2000 * 40.5) = ~1.5mm

That is significant -- the center deflects 1.5mm while the ends stay put. The center fittings would release before the outer ones. This is the "uneven pressure" failure mode from collet-release.md. **A horizontal line at 18mm spacing requires either a thicker plate (5mm+), a metal plate, or multiple push points.**

### Layout B: Vertical Line (4 in a Column)

```
     ┌─────────────┐
     │             │
     │     O1      │
     │             │
     │     O2      │
     │             │
     │     O3      │
     │             │
     │     O4      │
     │             │
     └─────────────┘

     Dimensions at 18mm spacing:
     Total height: 3 x 18 = 54mm between outermost centers
     Face height:  54 + 13 = ~67mm minimum
     Face width:   ~20mm (one fitting diameter + wall)
```

**Release plate:** A narrow vertical bar, ~18mm x ~67mm.

**Pros:**
- Narrowest face -- cartridge can be very compact in width.
- Same mechanical analysis as Layout A but rotated.

**Cons:**
- Tall face means the cartridge body is tall or the dock has a tall mating face.
- Same rigidity problem as Layout A (67mm span).
- Tubing routing from 2 side-by-side pumps to a vertical column requires significant routing through the cartridge body.
- Under-sink vertical space is often more constrained than horizontal space.

**Verdict:** Inferior to Layout A for this application. The under-sink environment has more horizontal space than vertical.

### Layout C: 2x2 Grid

```
     ┌──────────────────────┐
     │                      │
     │     O1       O2      │
     │                      │
     │     O3       O4      │
     │                      │
     └──────────────────────┘

     Dimensions at 18mm spacing:
     Total width:  18mm between column centers
     Total height: 18mm between row centers
     Face width:   18 + 13 = ~31mm minimum
     Face height:  18 + 13 = ~31mm minimum
```

**Release plate:** A compact square, ~31mm x ~31mm, with 4 holes in a grid.

**Port assignment options:**
- Row-paired: top row = pump 1 (inlet, outlet), bottom row = pump 2 (inlet, outlet)
- Column-paired: left column = inlets, right column = outlets
- Diagonal-paired: pump 1 at O1/O4, pump 2 at O2/O3

**Pros:**
- Most compact face overall (~31mm x ~31mm). Smallest cartridge body footprint.
- Release plate is nearly square -- maximum rigidity for a given area. Deflection is minimal because the maximum span is only ~31mm. At the same 20N load: delta = 20 * 31^3 / (48 * 2000 * I) where I is much larger relative to the span. Deflection is well under 0.1mm. **This is the stiffest plate geometry.**
- A single central push point from the cam distributes force evenly to all 4 corners.
- Row-paired assignment matches two side-by-side pumps with minimal tubing routing: each pump's inlet/outlet stubs map directly to one row.

**Cons:**
- 18mm center-to-center is tight for bulkhead fitting locknuts (need 20mm). At 20mm spacing: face becomes ~33mm x ~33mm -- still very compact.
- All 4 fittings are close together. A water leak from one fitting is more likely to reach an adjacent fitting than in a wider layout.
- Less room for guide features on the mating face itself (they would need to go outside the fitting cluster).

**Verdict:** Strong candidate. Best plate rigidity, most compact face, natural match for 2 side-by-side pumps.

### Layout D: Diamond (Rotated 2x2)

```
     ┌──────────────────────────┐
     │                          │
     │           O1             │
     │                          │
     │     O2          O3       │
     │                          │
     │           O4             │
     │                          │
     └──────────────────────────┘

     Dimensions at 18mm spacing (center-to-center along diagonals):
     Horizontal span: 18 * sqrt(2) = ~25.5mm
     Vertical span:   18 * sqrt(2) = ~25.5mm
     Face width:  25.5 + 13 = ~38.5mm
     Face height: 25.5 + 13 = ~38.5mm
```

**Release plate:** A diamond or circular shape, ~38mm diameter.

**Pros:**
- Looks clean and symmetric.
- Equal spacing from center to all 4 ports -- very even force distribution.
- A circular release plate with 4 holes at 90-degree intervals is easy to machine or print.

**Cons:**
- Wastes more face area than the 2x2 grid (38mm vs 31mm per side) for the same fitting count.
- Tubing routing is awkward -- the top and bottom fittings don't correspond to natural pump stub positions.
- The diamond shape complicates the rectangular cartridge body and dock cavity.
- No clear advantage over the 2x2 grid.

**Verdict:** Aesthetically interesting but less practical than the 2x2 grid. The 2x2 is more compact and routes tubing more naturally.

### Layout E: Two Pairs with Gap (Inlet Pair / Outlet Pair)

```
     ┌──────────────────────────────────────────────┐
     │                                              │
     │     O1       O2       ║       O3       O4    │
     │    (inlet1) (inlet2)  ║  (outlet1) (outlet2) │
     │                                              │
     └──────────────────────────────────────────────┘

     Dimensions: 18mm within each pair, 10-15mm gap between pairs
     Total width:  18 + gap + 18 = 46-51mm between outermost centers
     Face width:   46-51 + 13 = ~59-64mm
     Face height:  ~20mm
```

**Release plate:** Two separate release plates (one per pair), or one plate with a central stiffener/bridge spanning the gap.

**Pros:**
- The gap between pairs provides a natural zone for electrical contacts (if placed on the same face) or for a structural bridge in the dock wall.
- Functional grouping (all inlets on one side, all outlets on the other) makes plumbing logical and easy to trace for maintenance.
- Two smaller release plates (each only ~31mm span) would each be very rigid.

**Cons:**
- Nearly as wide as Layout A but with less flexibility in arrangement.
- Two release plates require two cam push points or a bridged mechanism.
- The gap is "wasted" space unless used for electrical or structural purposes.
- Tubing routing depends on pump orientation -- if pumps are side by side with heads facing back, inlet/outlet stubs are already paired per pump, not per function (all inlets vs. all outlets).

**Verdict:** Interesting if the gap serves a structural or electrical purpose. Otherwise, the 2x2 grid is more compact.

### Layout F: 2x2 Grid, Row-Paired, Wider Spacing

A variant of Layout C with more generous spacing to accommodate bulkhead locknuts and guide features:

```
     ┌──────────────────────────────┐
     │                              │
     │      O1            O2        │
     │   (pump1-in)  (pump1-out)    │
     │                              │
     │      O3            O4        │
     │   (pump2-in)  (pump2-out)    │
     │                              │
     └──────────────────────────────┘

     Dimensions at 22mm spacing:
     Face width:  22 + 13 = ~35mm
     Face height: 22 + 13 = ~35mm

     Dimensions at 25mm spacing:
     Face width:  25 + 13 = ~38mm
     Face height: 25 + 13 = ~38mm
```

**Release plate:** ~35-38mm square. Still very rigid. Allows 2mm+ wall between cradle bores. Ample room for bulkhead locknut hex flats.

**Pros:**
- All the advantages of Layout C with more breathing room.
- At 25mm center-to-center, each pump's inlet-outlet stub spacing (~25-30mm on the Kamoer head) maps almost directly to one row of the grid -- tube stubs run nearly straight back from the mating face to the pump head. Minimal internal tubing routing.
- Release plate at 38mm span is essentially zero-deflection under 20N load.

**Cons:**
- Slightly larger face than the tight 2x2. ~38mm vs ~31mm per side.
- Still requires guide features to be placed outside the fitting cluster.

**Verdict:** The best overall layout. Matches pump geometry, gives room for bulkhead fittings, and keeps the plate stiff.

---

## 3. Layout Comparison Summary

| Layout | Face Width | Face Height | Plate Span | Plate Rigidity | Tubing Routing | Fitting Spacing |
|---|---|---|---|---|---|---|
| A: Horizontal line | ~67mm | ~20mm | 67mm | Poor (flexes ~1.5mm) | Moderate | Comfortable |
| B: Vertical line | ~20mm | ~67mm | 67mm | Poor | Poor | Comfortable |
| C: 2x2 tight (18mm) | ~31mm | ~31mm | 31mm | Excellent | Good | Tight |
| D: Diamond | ~38mm | ~38mm | 38mm | Good | Poor | Comfortable |
| E: Two pairs + gap | ~60mm | ~20mm | 31mm each | Excellent (if split) | Moderate | Comfortable |
| F: 2x2 wide (22-25mm) | ~35-38mm | ~35-38mm | 35-38mm | Excellent | Excellent | Comfortable |

---

## 4. Release Plate Pocket

The release plate sits between the cartridge's tube stubs and the dock's John Guest fittings. When the cartridge is docked, the plate is pushed axially into the fittings to hold the collets in their engaged position (normal operation -- plate is NOT pressing collets). When the lever is actuated for removal, the plate pushes the collets inward to release the tubes.

Wait -- let's clarify the operational sequence:

### Operational Sequence

1. **Cartridge slides in.** Tube stubs pass through the release plate holes and push into the John Guest fittings. The fittings grip the tubes automatically.
2. **Lever is closed (locked position).** The cam pushes the release plate into its "stowed" position -- the plate is NOT pressing the collets. The over-center cam holds the lever closed.
3. **Normal operation.** The release plate sits idle. The fittings hold the tubes. Water flows.
4. **Lever is opened (release position).** The cam releases. A return spring (or the collet springs themselves) push the release plate back to a neutral position -- still not pressing collets.
5. **Actually -- re-thinking.** The release plate needs to PUSH the collets inward to release. So the plate must move TOWARD the fittings (deeper into the dock) to release, not away.

### Corrected Sequence

The release plate's purpose is to release collets on removal. The lever must push the plate INTO the fittings:

1. **Cartridge slides in.** Tube stubs pass through release plate holes. Stubs seat in fittings. Fittings grip.
2. **Lever is closed (locked).** The cam is in its "locked" position. The release plate is NOT pressing collets. The over-center cam prevents the lever from opening accidentally. The cartridge is held by the 4 John Guest fittings.
3. **Normal operation.** Everything is passive.
4. **User opens lever (release).** Rotating the lever drives the cam, which pushes the release plate axially TOWARD the fittings (into the dock wall). The plate's inner lips press the 4 collets inward simultaneously. The collets release the tubes.
5. **User slides cartridge out.** The tube stubs withdraw from the fittings. The release plate (which is part of the dock, not the cartridge) stays in the dock.

**Key insight: the release plate belongs to the dock, not the cartridge.** The cartridge is just tube stubs and pump bodies. The dock has the fittings, the release plate, the cam lever, and the guide rails.

### Plate Pocket Design

The release plate lives in the dock, between the dock wall (where fittings are mounted) and the cartridge cavity:

```
    Side view (cross-section through one fitting):

    ← cartridge side          dock side →

                     fitting
    tube stub →  ====|XXXXXX|==== plumbing
                     |      |
          ┌─────────┐|      |
          │  plate  │|collet|
          │  pocket │|      |
          └─────────┘|      |
                     └──────┘
                       dock wall

    ←── plate travel (3mm) ──→
```

The pocket is a recess in the dock face, sized to hold the release plate and allow it to slide 3mm axially (toward and away from the fittings).

**Pocket dimensions (for Layout F, 2x2 at 25mm spacing):**

| Feature | Dimension | Notes |
|---|---|---|
| Pocket width | ~42mm | Plate width (~38mm) + 2mm clearance per side |
| Pocket height | ~42mm | Same |
| Pocket depth (axial) | ~6-8mm | Plate thickness (3mm) + travel (3mm) + 1-2mm clearance |
| Plate thickness | 3mm | Sufficient for stepped bore + structural rigidity |
| Guide features | 2 slots or pins | Keep plate parallel during travel |

### Plate Guidance

The plate must translate purely axially -- no tilt. Two approaches:

**Guide pins (preferred):** Two smooth pins (steel dowels or printed pegs) fixed to the dock wall, passing through clearance holes in the release plate. The plate slides on the pins. A compression spring on each pin (or a single central spring) provides return force.

```
    Front view of dock face with release plate:

    ┌───────────────────────────────────┐
    │                                   │
    │   [pin]    O1      O2    [pin]    │
    │                                   │
    │            O3      O4             │
    │                                   │
    │   [pin]                  [pin]    │
    │                                   │
    └───────────────────────────────────┘

    4 guide pins at corners, plate slides on all 4
    (2 pins minimum, 4 pins for zero tilt)
```

With 4 guide pins, the plate is constrained against tilting in any direction. Each pin has a compression spring that pushes the plate to its "home" (non-pressing) position.

**Edge slots:** The plate rides in grooves machined or printed into the dock pocket walls. Simpler but more friction and more prone to binding in FDM prints.

**Recommendation:** 4 guide pins (3mm steel dowels from a hardware store). The pins also serve as the return spring mounting. Total mechanism: plate + 4 pins + 4 small compression springs.

---

## 5. Electrical Contact Placement

The electrical-mating.md research strongly recommends placing electrical contacts on a different face than water fittings. Let's evaluate the options.

### Option 1: Top of Cartridge (Recommended)

Electrical pads on the top face of the cartridge, pogo pins mounted on the roof of the dock cavity.

```
    Cross-section (looking at cartridge from the front):

    ┌─── dock ceiling ────────────────────────┐
    │  [pogo] [pogo] [pogo]                   │   ← 3 pogo pins pointing down
    │                                         │
    │  ┌── cartridge top ─────────────────┐   │
    │  │  [pad]  [pad]  [pad]             │   │   ← 3 brass pads facing up
    │  │                                  │   │
    │  │         (pump bodies)            │   │
    │  │                                  │   │
    │  └──────────────────────────────────┘   │
    │                                         │
    └─────────────────────────────────────────┘
```

**How contact is made:** As the cartridge slides in, the top surface passes under the pogo pins. The pins drag along the top surface until they reach the pads, providing natural wipe action. When fully seated, the pins press down on the pads under spring force.

**Pros:**
- Complete physical separation from water fittings (which are on the back face).
- Water drips downward (gravity), away from contacts above.
- The dock ceiling is a large, stable surface for mounting pogo pin holders.
- Pogo pin wipe occurs along the slide direction -- the full pad length provides cleaning.
- No moisture mitigation features needed beyond basic separation.

**Cons:**
- The dock ceiling must be parallel to the cartridge top within ~1mm for consistent pin compression. Rail guidance ensures this.
- The dock cavity is now constrained in height: cartridge height + pin stroke + clearance.
- Pogo pins pointing downward can accumulate dust and debris on the plunger tip.

**Contact pad location on cartridge top:**
- Pads near the back of the cartridge (toward the mating face) so they engage last, after the rails have aligned everything.
- Pads spaced 10mm center-to-center in a row across the cartridge width.
- Pad size: 8mm long (insertion direction) x 5mm wide. The 8mm length provides generous wipe area.

### Option 2: Side of Cartridge

Electrical pads on one side face, pogo pins on the corresponding dock wall.

**Pros:**
- Separated from water (back face) and from any drips (top).
- Dock wall mounting is straightforward.

**Cons:**
- Asymmetric force from pogo pin springs pushes the cartridge sideways. With 3 pins at ~0.5N each, that is ~1.5N laterally -- enough to bias the cartridge against the opposite rail, causing uneven wear.
- Reduces effective dock cavity width (must leave room for pins + stroke + clearance on one side).
- If the cartridge is keyed (asymmetric cross-section), which side gets contacts? Must be the side with the most rail clearance.

**Verdict:** Workable but the lateral force bias is undesirable. Top is better.

### Option 3: Same Face as Water Fittings (Back of Cartridge)

Electrical pads on the mating face, above or below the fitting cluster, with a moisture barrier between.

```
    Mating face (cartridge back wall, viewed from behind):

    ┌───────────────────────────────┐
    │                               │
    │   [pad] [pad] [pad]          │  ← electrical contacts
    │                               │
    │   ═══════════════════        │  ← raised dam (3mm high)
    │                               │
    │      O1      O2              │  ← tube ports
    │                               │
    │      O3      O4              │
    │                               │
    └───────────────────────────────┘
```

**Pros:**
- Single mating face -- everything connects at once during the slide-in motion.
- Simplest dock geometry (no ceiling-mounted pins).
- Pogo pins in the dock wall face the cartridge directly -- no angular concerns.

**Cons:**
- Moisture risk. Even with a 3mm dam and 15mm separation, a fitting leak during cartridge removal (when water pressure pushes a drip out of a partially-unseated fitting) can splash onto the electrical zone.
- The mating face grows larger (height increases by ~20mm for contacts + dam + clearance).
- Electrical contacts on this face would be pressed by the cartridge sliding in, requiring the pogo pins to be oriented along the insertion axis. This means the pins compress during insertion and must be fully compressed when docked -- works fine mechanically, but the pins bear the full insertion force as the cartridge seats. The pin springs must be stiff enough to survive this without damage, but soft enough for easy insertion.
- If the release plate is on this face, the electrical contacts must be outside the plate envelope, further growing the face.

**Verdict:** Acceptable for prototyping but the moisture risk is real. For the final design, top-mounted is safer.

### Option 4: Bottom of Cartridge

Same as Option 1 but inverted -- pads on the bottom, pins pointing up from the dock floor.

**Pros:**
- Separated from water fittings.
- Gravity pulls the cartridge onto the pins (adds to contact force).

**Cons:**
- Water that drips from the mating face fittings collects on the dock floor, directly where the pogo pins are.
- Debris (crumbs, dust from under a sink) collects at the bottom and fouls the contacts.
- Hardest surface to inspect visually.

**Verdict:** The worst option for moisture and debris. Eliminated.

### Electrical Placement Summary

| Location | Moisture Risk | Force Bias | Face Growth | Wipe Action | Verdict |
|---|---|---|---|---|---|
| Top of cartridge | Lowest | None (gravity helps) | None | Along slide axis | Best |
| Side of cartridge | Low | Lateral bias (~1.5N) | None | Along slide axis | Acceptable |
| Same face (above water) | Moderate | None | +20mm height | Along slide axis | Prototype OK |
| Bottom of cartridge | Highest | None | None | Along slide axis | Eliminated |

**Recommendation:** Electrical contacts on the top of the cartridge, pogo pins on the dock ceiling.

---

## 6. Guide Feature Integration

From guide-alignment.md, the recommended approach is: funnel entrance + rectangular/dovetail rails + tapered pins at mating face.

### Where Tapered Pins Go

Tapered alignment pins sit on the dock's mating face (the same wall that holds the John Guest fittings). They engage conical sockets on the cartridge's back wall as the last 15-20mm of insertion seats everything.

The pins should be placed **outside the release plate envelope** so they don't interfere with plate travel. Two pins, diagonally opposite, constrain X, Y, and rotation:

```
    Dock mating face (viewed from cartridge side):

    ┌─────────────────────────────────────────┐
    │                                         │
    │   [taper pin]                           │
    │              ┌───────────────┐          │
    │              │               │          │
    │              │  O1      O2   │          │
    │              │               │ release  │
    │              │  O3      O4   │ plate    │
    │              │               │ pocket   │
    │              └───────────────┘          │
    │                          [taper pin]    │
    │                                         │
    └─────────────────────────────────────────┘
```

**Pin dimensions (from guide-alignment.md):**
- Base diameter: 8-10mm
- Taper: 15-20 degrees per side
- Length: 15-20mm
- Socket entrance: 12-15mm diameter (provides ~5-7mm capture range per pin)

**Pin center distance from fitting cluster:** The pins should be at least 10mm from the nearest fitting cradle bore edge. For Layout F (25mm grid, ~38mm cluster): pins at approximately 30mm from the cluster center, diagonally. This puts the pin centers at roughly 50-60mm diagonal apart.

### Rail Attachment Points

The rails (rectangular or dovetail) run parallel to the insertion axis, mounted on the dock's side walls (or top/bottom walls). The rails are NOT on the mating face -- they are along the length of the dock cavity.

However, the rail cross-section at the entrance to the dock determines the mating face's overall dimensions. The mating face width must equal the cartridge width, which includes the rail features on the sides.

For rectangular rails:
- Rail width: 5-8mm per side
- Rail height: 3-5mm
- Clearance: 0.3mm per side

The mating face width = fitting cluster width + tapered pin zone + rail features on each side.

---

## 7. Overall Mating Face Dimensions

Combining all elements for Layout F (2x2 grid, 25mm spacing):

### Minimum Mating Face

```
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ rail ╔═══════════════════════════════════════╗ rail  │
    │ zone ║                                       ║ zone │
    │      ║  [pin]                                ║      │
    │      ║         ┌─────────────────┐           ║      │
    │      ║         │                 │           ║      │
    │      ║         │   O1       O2   │           ║      │
    │      ║         │                 │  plate    ║      │
    │      ║         │   O3       O4   │  pocket   ║      │
    │      ║         │                 │           ║      │
    │      ║         └─────────────────┘           ║      │
    │      ║                          [pin]        ║      │
    │      ║                                       ║      │
    │      ╚═══════════════════════════════════════╝      │
    │                                                     │
    └─────────────────────────────────────────────────────┘

    Electrical contacts are NOT on this face (they are on the top).
```

**Width calculation:**
- Fitting cluster: 25mm (center-to-center) + 13mm (half fitting OD each side) = 38mm
- Tapered pin clearance: each pin base is 10mm, centered ~20mm from cluster edge = adds ~10mm per side beyond the cluster
- Rail zones: 8mm per side (rail + wall)
- Total mating face width: 38 + 20 + 16 = **~74mm**

**Height calculation:**
- Fitting cluster: 25mm + 13mm = 38mm
- Tapered pin clearance: ~10mm beyond cluster (only one pin per corner, diagonal): adds ~10mm to one end vertically
- Structural walls: 5mm top and bottom
- Total mating face height: 38 + 10 + 10 = **~58mm**

This could be reduced by:
- Tightening fitting spacing to 20mm: saves 10mm total (5mm in each dimension)
- Using smaller tapered pins (6mm base): saves ~4mm per pin zone
- Integrating pins inside the rail profile: saves their dedicated zone

### Dimension Summary Per Layout

| Layout | Fitting Cluster | + Pin Zones | + Rails | Total Width | Total Height |
|---|---|---|---|---|---|
| A: Horizontal line (18mm) | 67 x 20mm | +20mm W, +20mm H | +16mm W | ~103mm | ~46mm |
| C: 2x2 tight (18mm) | 31 x 31mm | +20mm each | +16mm W | ~67mm | ~56mm |
| F: 2x2 wide (25mm) | 38 x 38mm | +20mm each | +16mm W | ~74mm | ~63mm |
| F: 2x2 (20mm) | 33 x 33mm | +20mm each | +16mm W | ~69mm | ~58mm |

For context, 74mm is about 3 inches. A 74mm x 63mm mating face is quite reasonable for an under-sink device.

---

## 8. Cam Lever Integration with Mating Face

The cam lever must push the release plate. The push point should be at the geometric center of the 4 fitting positions for even force distribution. For the 2x2 grid, the center of the plate is equidistant from all 4 fittings.

### Lever Placement Options

**Option A: Lever on top of dock, cam pushes down then converts to axial push.**
The lever is on the visible/accessible top of the dock. A cam rotates and pushes a follower that redirects force from vertical to axial (into the page). This requires a right-angle force redirection (wedge ramp or bell crank), adding complexity.

**Option B: Lever on the side of the dock, cam pushes plate axially.**
The lever pivot is on the dock's side wall. The lever arm extends outward (accessible to the user). The cam at the pivot directly pushes a rod or plate that translates axially to push the release plate. The cam axis is perpendicular to the insertion direction.

**Option C: Lever on the cartridge front face, cam pushes against dock.**
Similar to server blade ejectors. The lever is mounted on the front of the cartridge (the face the user sees). Rotating the lever drives a cam that pushes against the dock frame, which in turn pushes the release plate. But wait -- the release plate is in the dock, and the cam is on the cartridge. The cam would need to push against a dock feature that then pushes the plate. This adds a force transmission element.

**Option D: Lever on the dock, beside the cartridge opening.**
A lever mounted on the dock body, adjacent to the cartridge slot opening. The lever arm is accessible. The cam is inside the dock wall and directly pushes the release plate via a pushrod through the dock wall.

**Best fit for mating face:** Option D keeps the lever in the dock (where it belongs, since the release plate is also in the dock) and connects to the plate via a simple pushrod through the dock wall. The pushrod passes through the dock wall at the center of the 4-fitting cluster and pushes the plate center. The lever pivot is on the dock exterior, near the cartridge opening.

The mating face design should include a **central hole** (or slot) in the dock wall behind the release plate for the pushrod to pass through.

---

## 9. Interdependency Map

All the decisions are coupled. Here is how each component constrains the others:

```
    Fitting spacing  ──→  Release plate size  ──→  Plate pocket dimensions
         │                      │                        │
         ↓                      ↓                        ↓
    Mating face size  ←──  Cam push point        Dock wall thickness
         │                      │
         ↓                      ↓
    Cartridge body      Lever arm length
    cross-section       and placement
         │
         ↓
    Rail dimensions  ──→  Dock cavity size
         │
         ↓
    Tapered pin        ──→  Mating face
    placement               total size
```

**Critical path:** Fitting spacing (the tightest constraint) drives everything. Once fitting spacing is chosen, the release plate, mating face, and cartridge body all follow.

**Decoupled decision:** Electrical contact placement (top of cartridge) is independent of the mating face layout. It adds a height constraint to the dock cavity but does not affect the mating face design.

---

## 10. Recommendation Ranking

### 1. Layout F: 2x2 Grid at 22mm Spacing (Best Overall)

**Fitting center-to-center: 22mm horizontal, 22mm vertical.**

- Mating face fitting cluster: 35mm x 35mm
- Release plate: ~39mm x 39mm (with 2mm margin per side)
- Plate deflection under 20N central load: <0.1mm -- negligible
- Tubing routing: each row maps to one pump's inlet/outlet pair. Pump stub spacing (~25-30mm) is close to the 22mm port spacing -- short, low-stress tubing runs with gentle bends
- Bulkhead fittings: 22mm center-to-center accommodates locknuts (16-19mm hex) with ~3-6mm clearance between adjacent hex flats. Tight but workable -- install one pair of locknuts, then the other pair
- Overall mating face (with pins and rails): ~70mm wide x 57mm tall
- Electrical contacts: on cartridge top, not on this face

**Why this wins:**
- Most compact while still accommodating bulkhead locknuts
- Best plate rigidity (small span, nearly square)
- Natural tubing routing (row = pump)
- Leaves room for alignment pins outside the plate pocket
- The 2mm gain over 20mm spacing (the absolute minimum for bulkhead fittings) provides meaningful tolerance margin

### 2. Layout F: 2x2 Grid at 25mm Spacing (Most Comfortable)

Same rationale as above but with more generous clearances. Mating face grows to ~74mm x 63mm. Choose this if the first prototype at 22mm reveals tight-tolerance problems with locknut installation or plate bore wall thickness.

### 3. Layout C: 2x2 Grid at 18mm Spacing (Most Compact)

Only viable with inline fittings (no bulkhead locknuts). Release plate wall between bores is only ~5.5mm (18mm center - 12.5mm bore = 5.5mm). This is printable in PETG but fragile under repeated loading. Use only if space constraints are severe.

### 4. Layout E: Two Pairs + Gap (Best if Electrical Must Share the Face)

If top-mounted electrical contacts prove impractical (dock ceiling access, wiring difficulty), the gap between the two fitting pairs provides a natural zone for 3 electrical contacts with physical separation from water. The gap width of 10-15mm exceeds the 10mm minimum recommended separation. Face width grows to ~60mm.

### 5. Layout A: Horizontal Line (Avoid Unless Forced)

The 67mm plate span causes unacceptable deflection in a 3D-printed plate. Would require a metal plate or multiple push points. Only justified if the cartridge body is severely width-constrained (unlikely given 2 pumps sit side by side).

---

## 11. Next Steps

1. **Measure Kamoer pump head stub spacing** with calipers. The assumed 25-30mm center-to-center between inlet and outlet stubs on a single pump head determines how well the 22mm row spacing matches.

2. **Measure specific John Guest fittings in hand.** The assumed 12.7mm body OD and 11.4mm collet ring OD should be verified. If the actual body OD is smaller, fitting spacing can be tightened.

3. **Decide bulkhead vs. inline fitting mounting.** This is the main decision that determines whether 18mm or 22mm spacing is viable. Bulkhead is more reliable but demands more space.

4. **Print a test plate** with 4 stepped bores at 22mm center-to-center. Test it against the actual fittings. Does the inner lip engage the collets evenly? Does the plate flex? Can all 4 collets release simultaneously with a central push?

5. **Prototype the dock wall** with 4 fittings mounted and the release plate in its pocket on guide pins. Verify the full insertion and release sequence before designing the cam lever attachment.

---

## Sources

- [John Guest Panel Mount Fittings (PMI Series)](https://www.johnguest.com/us/en/od-tube-fittings/panel-mount-fittings)
- [John Guest PP0408W Union Connector Dimensions](https://www.h2odistributors.com/product/pp0408w-john-guest-straight-union-connector/)
- [John Guest Bulkhead Fitting Dimensions (CI3212W)](https://www.johnguest.com/us/en/product/1-4-bulkhead-connector-ci3212w)
- [PETG Flexural Modulus (~2000 MPa) -- MatWeb Material Property Data](https://www.matweb.com/search/datasheet.aspx?matguid=19ca0e21f0154b3ea55ebf2ecc945dfd)
- [FDM Wall Thickness Guidelines -- Formlabs Design Guide](https://formlabs.com/blog/fdm-3d-printing-design-guide/)
- [Compression Spring Selection Guide -- Lee Spring](https://www.leespring.com/compression-springs)
- Round 1 research: [collet-release.md](collet-release.md), [electrical-mating.md](electrical-mating.md), [guide-alignment.md](guide-alignment.md), [cam-lever.md](cam-lever.md)
