# Pump Cartridge — Requirements & Design Decisions

## What the cartridge is

A replaceable slide-in module containing 2 Kamoer peristaltic pumps (one per flavor line). The user slides it into a dock, it makes fluid and electrical connections automatically, and a lever gives it a solid docked feel. To replace: flip the lever, slide it out, slide a new one in.

## Functional Requirements

These were agreed upon in conversation 1 (2026-03-23):

1. The cartridge slides into the dock along rails, guided so that 4 tube stubs align with 4 John Guest 1/4" fittings within ~1mm tolerance.
2. The final push of insertion seats the tubes into the fittings. The fittings latch automatically and hold the cartridge in place — no mechanism needed for connection or retention.
3. Three electrical contacts (ground, motor A 12V, motor B 12V) mate during the last portion of insertion via spring-loaded pins or similar. L298N motor drivers stay in the main body — only power crosses the interface.
4. A lever locks the cartridge in the fully seated position, giving a clear "docked" feel. The fittings already provide retention — the lever adds rigidity and confidence.
5. To remove: actuating that same lever pushes 4 collets inward ~2-3mm simultaneously, releasing the tube grip, and the user slides the cartridge out.
6. The user should feel a clear "locked" and "released" state — no ambiguity about whether it's seated.
7. One-handed operation is ideal given the under-sink context.
8. Front insertion into the dock, user facing the front of the unit.

## Key Insight: The Cam is a Release Mechanism

The cam/lever does NOT make the fluid connections — John Guest push-to-connect fittings latch automatically when you push tubing in. The fittings also provide all the retention force needed to hold the cartridge in the dock. The cam only actuates on removal, pressing the collets to release the tubes. When closed, the cam adds rigidity and a tactile "locked" feel, but the cartridge is already held in place by the fittings alone.

## Three Sub-Problems

The cartridge mechanism is a sequence of three things that happen during insertion (and reverse on removal):

1. **Guide and align** — Rails guide the cartridge into position. Tapered lead-in (wider at entrance, narrowing to final position) allows sloppy initial alignment and self-centering on the last inch. Think desk drawer or server blade chassis.

2. **Seat the fluid connections** — 4 tube stubs on the cartridge push into 4 John Guest fittings mounted on the back wall of the dock. Happens as part of the slide-in motion. Fittings latch automatically.

3. **Secure feel and release** — The lever in its closed position adds rigidity and a solid docked feel. When opened for removal, it pushes 4 collets inward ~2-3mm simultaneously via a release plate with precisely sized holes.

## Physical Specifications

### Fluid connections
- **Fittings:** John Guest-style 1/4" push-to-connect (standard ice maker / RO type)
- **Collet travel:** ~2-3mm inward push to release
- **Collet force:** Light, ~1-2 lbs per fitting
- **Blue locking clips:** Not present (intentionally omitted)
- **Count:** 4 total (2 pumps × inlet + outlet each)
- **Tubing:** 1/4" OD (nominally 6.35mm), 1/8" ID silicone

### Electrical connections
- **Contacts:** 3 (ground, motor A 12V, motor B 12V)
- **Approach:** Pogo pins (dock side) against exposed copper pads (cartridge side), or blade connectors
- **Timing:** Mate during last few millimeters of insertion
- **Placement:** Far enough from fluid connections that moisture never reaches them

### Pumps
- 2× Kamoer peristaltic pumps
- Orientation flexible — can be arranged however works best for the cartridge footprint
- Driven by L298N motor drivers at 12V via PWM (drivers in main body, not in cartridge)

## Open Design Questions

These constrain everything else and should be resolved in order:

### 1. Mating face layout
Where do the 4 tubes and 3 electrical contacts sit relative to each other on the face where the cartridge meets the dock? This is the single most constraining decision.
- If 4 tubes are in a line → cam can be a simple bar sweeping across all collets
- If 4 tubes are in a 2×2 grid → need a plate-style actuator
- Electrical contacts must be separated from fluid connections (moisture)

### 2. Cam geometry
- Eccentric cam lobe (bicycle QR skewer style) converts rotation into ~2-3mm linear displacement
- Single lever rotates a bar or plate that presses all 4 collets simultaneously
- Lever position options: top (most accessible under sink), side, or front face

### 3. Rail profile
- Determines how forgiving insertion is
- Tapered lead-in recommended for self-centering
- Needs to constrain to ~1mm tolerance at final position

## Release Plate Design

The release mechanism is a flat plate with 4 precisely sized holes:
- Each hole clears the 1/4" OD tube (~7-8mm diameter) but is smaller than the collet outer diameter
- When the plate slides toward the fittings (~2-3mm), the hole rims catch the collet rings and push them inward concentrically
- This is the same principle as John Guest release tools

### Critical measurements (need calipers):
- Tube OD (nominally 1/4" / 6.35mm)
- Collet ring outer diameter (on the specific fittings in hand)
- Inner diameter where collet meets the fitting body
- **The plate hole must be between tube OD and collet OD**

### First print
The release plate is the ideal first 3D print:
- Small, fast to print
- Tests dimensional accuracy (hole diameters matter to ~0.5mm)
- Directly informs the rest of the cartridge design
- Expect 3-4 iterations to dial in the fit
- Simple CAD: rectangle + 4 holes (sketch, extrude, linear pattern)

## Design Approach (4 Phases)

1. **Catalog functional requirements** — Done (above)
2. **For each requirement, explore mechanism families** — Ask AI for standard mechanical approaches (tapered pins, V-grooves, kinematic couplings, rail systems, etc.)
3. **Pick a mechanism family for each requirement, then sketch** — Use OnShape to prove chosen mechanisms can physically coexist in the available space
4. **AI generates the parametric model** — Once describable in terms of mechanism choices, use CadQuery via AI to generate the actual model

## CAD Tooling Notes

- **Printer:** Bambu H2C
- **CAD:** OnShape (180-day trial) for visual design + learning; CadQuery (Python) for AI-generated parts
- **Workflow:** Learn operations in OnShape → describe to AI in shared vocabulary → AI generates CadQuery Python → export STEP/STL → print
- **Source of truth:** TBD — likely CadQuery for simple mechanical parts, OnShape for complex shapes requiring visual iteration
- **Key operations** for this project: sketch, extrude, hole, linear pattern, fillet, chamfer, shell, boolean subtract
