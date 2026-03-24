# Pump Cartridge — Functional Requirements

## What the cartridge is

A replaceable slide-in module containing 2 Kamoer peristaltic pumps (one per flavor line). The user slides it into a dock, it makes fluid and electrical connections automatically, and a lever gives it a solid docked feel. To replace: flip the lever, slide it out, slide a new one in.

## Environment

- Installed under a kitchen sink — limited visibility, awkward reach, possibly wet hands
- Front insertion into the dock, user facing the front of the unit
- One-handed operation is ideal

## Functional Requirements

1. The cartridge slides into the dock, guided so that 4 tube stubs align with 4 John Guest 1/4" fittings within ~1mm tolerance.
2. The final push of insertion seats the tubes into the fittings. The fittings latch automatically and hold the cartridge in place — no mechanism needed for connection or retention.
3. Three electrical contacts (ground, motor A 12V, motor B 12V) mate during insertion. L298N motor drivers stay in the main body — only power crosses the interface.
4. A lever locks the cartridge in the fully seated position, giving a clear "docked" feel. The fittings already provide retention — the lever adds rigidity and confidence.
5. To remove: actuating that same lever releases all 4 John Guest collets simultaneously, and the user slides the cartridge out.
6. The user should feel a clear "locked" and "released" state — no ambiguity about whether it's seated.

## Key Constraints

- **Fittings provide retention.** The 4 John Guest push-to-connect fittings hold the cartridge in the dock on their own. The lever does not need to provide structural retention — it provides feel and enables release.
- **Collet release requires even, concentric pressure.** Pressing a collet from one side cocks it sideways instead of releasing the tube. The release mechanism must apply pressure evenly around the collet circumference and prevent lateral collet movement during release. Commercial John Guest release tools achieve this with a shaped inset that cradles the collet.
- **Electrical contacts must be separated from fluid connections.** Moisture from the water lines must not reach the electrical interface.

## Three Sub-Problems

The cartridge mechanism is a sequence of three things that happen during insertion (and reverse on removal):

1. **Guide and align** — The cartridge must find its position in the dock reliably, even with sloppy initial aim in a dark cabinet. The last portion of travel must constrain to ~1mm tolerance so tube stubs align with fittings.

2. **Seat the fluid connections** — 4 tube stubs on the cartridge push into 4 John Guest fittings mounted in the dock. The fittings latch automatically as part of the slide-in motion.

3. **Secure feel and release** — The lever provides a solid docked feel when closed. When opened for removal, it releases all 4 collets simultaneously so the cartridge can slide out.

## Physical Facts (parts in hand)

### Fluid connections
- **Fittings:** John Guest-style 1/4" push-to-connect (standard ice maker / RO type)
- **Collet travel:** ~2-3mm inward push to release
- **Collet force:** Light, ~1-2 lbs per fitting
- **Blue locking clips:** Not present (intentionally omitted)
- **Count:** 4 total (2 pumps x inlet + outlet each)
- **Tubing:** 1/4" OD (nominally 6.35mm), 1/8" ID silicone

### Electrical connections
- **Contacts:** 3 (ground, motor A 12V, motor B 12V)

### Pumps
- 2x Kamoer peristaltic pumps
- Orientation flexible — can be arranged however works best for the cartridge footprint
- Driven by L298N motor drivers at 12V via PWM (drivers in main body, not in cartridge)

### Critical measurements (need calipers)
- Tube OD (nominally 1/4" / 6.35mm)
- Collet ring outer diameter (on the specific fittings in hand)
- Inner diameter where collet meets the fitting body

## Research

### Round 1: Mechanism Families
Each sub-problem has a corresponding research document exploring mechanism families and tradeoffs:

- [Collet Release Mechanics](../research/collet-release.md)
- [Guide & Alignment Mechanisms](../research/guide-alignment.md)
- [Electrical Mating Approaches](../research/electrical-mating.md)
- [Cam & Lever Mechanisms + Prior Art](../research/cam-lever.md)

### Round 2: Component-Level Design
Grounded in specific dimensions from parts in hand and Round 1 research:

- [Release Plate](../research/release-plate.md) — Stepped bore geometry, hole arrangements, first-print spec
- [Mating Face](../research/mating-face.md) — Tube port layout, fitting mounting, electrical contact placement, guide features
- [Cartridge Envelope](../research/cartridge-envelope.md) — Pump dimensions, arrangements, tubing routing, bounding volumes
- [Pump Mounting](../research/pump-mounting.md) — Mounting features, vibration isolation, strain relief, wire routing

## Design Approach (4 Phases)

1. **Catalog functional requirements** — Done (above)
2. **For each requirement, explore mechanism families** — Research documents (above)
3. **Pick a mechanism family for each requirement, then sketch** — Use OnShape to prove chosen mechanisms can physically coexist in the available space
4. **AI generates the parametric model** — Once describable in terms of mechanism choices, use CadQuery via AI to generate the actual model

## CAD Tooling Notes

- **Printer:** Bambu H2C
- **CAD:** OnShape (180-day trial) for visual design + learning; CadQuery (Python) for AI-generated parts
- **Workflow:** Learn operations in OnShape → describe to AI in shared vocabulary → AI generates CadQuery Python → export STEP/STL → print
- **Source of truth:** TBD — likely CadQuery for simple mechanical parts, OnShape for complex shapes requiring visual iteration
- **Key operations** for this project: sketch, extrude, hole, linear pattern, fillet, chamfer, shell, boolean subtract
