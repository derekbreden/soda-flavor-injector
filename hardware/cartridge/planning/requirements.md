# Pump Cartridge — Functional Requirements

## What the cartridge is

A replaceable slide-in module containing 2 Kamoer peristaltic pumps (one per flavor line) and nothing else. No valves, no motor drivers, no electronics beyond passive contact pads. The user slides it into a dock at the front-bottom of the enclosure, it makes fluid and electrical connections, and it's done. To replace: disconnect, slide it out, slide a new one in. This is the only user-replaceable component in the entire system.

## What the cartridge is NOT

- It does not contain valves. All 8 valves (4 per pump) are mounted in the main enclosure body.
- It does not contain motor drivers. The L298N drivers live in the main body.
- It does not contain any active electronics. Only passive contact pads for receiving power.
- It is the simplest possible replaceable module: a box with two pumps, four fluid ports, and a few electrical pads.

## Environment

- Installed under a kitchen sink — limited visibility, awkward reach, possibly wet hands
- The dock is at the **front-bottom** of the enclosure, in the triangular/trapezoidal void below the diagonal bag slab
- The cartridge slot sits at floor level on the front panel — the user slides the cartridge along the cabinet floor into the enclosure
- Front insertion, user facing the front of the unit, crouching or kneeling
- One-handed operation is ideal
- Dark cabinet — the user may not have direct line-of-sight to the slot
- The cartridge rests on the enclosure floor during operation; any fluid drips from fittings during a swap pool on the enclosure floor beneath the bags

## Functional Requirements

1. The cartridge slides into the dock on floor rails, guided so that 4 fluid connections and electrical contacts align reliably, even with sloppy initial aim in a dark cabinet.
2. Four fluid connections mate during insertion — 2 pump inlets and 2 pump outlets. These connect the cartridge pumps to the enclosure's valve-routed fluid paths. The valve assemblies are in the main body; the cartridge carries only the pump side of the connections.
3. Three electrical contacts (ground, motor A 12V, motor B 12V) mate during insertion via pogo pins (dock side) pressing onto flat pads (cartridge top face). L298N motor drivers stay in the main body — only power crosses the interface.
4. The user should feel a clear "connected" and "disconnected" state — no ambiguity about whether it's seated.
5. To remove: release the fluid connections (method depends on fitting choice — see below), slide the cartridge out.
6. The cartridge is replaced every 18-36 months when pump tubes wear out. The user who does this swap may not remember the procedure — the mechanism must be self-evident or obvious from minimal labeling.

## Fluid Connection Options

The fitting choice is the single biggest design decision for the cartridge. It determines whether the cartridge needs a release mechanism, how the user disconnects it, and whether fluid drips during a swap. Two viable options:

### Option A: John Guest Push-to-Connect ($8 for 4 fittings)

Four JG 1/4" push-to-connect fittings mount in the dock back wall. The cartridge carries 4 tube stubs (1/4" OD hard nylon, ~30mm protrusion) that insert into the fittings as the cartridge slides in. Collets grip automatically on insertion.

**Release:** A cam lever on the cartridge front face drives a push rod through the cartridge body to a release plate on the rear face. The release plate has four stepped bores that engage all 4 JG collet rings simultaneously. Flipping the lever pushes the plate rearward ~3mm, depressing all collets. The user then pulls the cartridge out by the lever handle.

| Aspect | Detail |
|---|---|
| Cost | ~$8 (fittings) + ~$5-10 (cam lever, push rod, release plate hardware) |
| Auto-shutoff | No — open bore when disconnected. Drips are likely during swap. |
| Disconnect UX | Flip lever, pull out. The lever mechanism is not self-evident to a first-time user. |
| Cartridge complexity | Higher — requires cam lever, push rod, release plate, dowel pins |
| Retention | JG collets provide ~20N of grip (4 fittings). Lever adds rigidity. |

**Trade-off:** Cheaper but more complex cartridge, worse disconnect UX, and drips during swap pool on the enclosure floor (the worst possible location for drips).

### Option B: CPC Quick-Disconnect ($70 for 4 connections)

Four CPC PLC NSF valved couplings mount in the dock back wall (female bodies). The cartridge carries 4 male inserts. When the cartridge slides in, inserts engage bodies and thumb latches click. Auto-shutoff valves on both halves close automatically when separated — zero dripping during swap.

| Aspect | Detail |
|---|---|
| Cost | ~$70 (4 CPC pairs). Dock bodies are permanent; replacement cartridges need 4 male inserts (~$6-8 each). |
| Auto-shutoff | Yes — both sides valve closed on disconnect. No drips. |
| Disconnect UX | Squeeze-and-pull each CPC, or a pull handle that actuates all 4. Audible click confirms connection. |
| Cartridge complexity | Lower — no cam lever, no push rod, no release plate. Just a box with pumps and CPC inserts. |
| Retention | CPC coupling latches provide ~15-25N of positive retention. No additional mechanism needed. |
| Food safety | NSF 169 (specifically for food equipment — strongest cert available) |
| Body OD | ~22mm (vs ~12mm for JG) — requires more space on mating face, but space is unconstrained in this layout |

**Trade-off:** More expensive up front ($70 vs $15-20), but eliminates the cam lever, release plate, and push rod entirely. The cartridge becomes dramatically simpler. Auto-shutoff prevents drips at the worst-case location (enclosure floor). CPC bodies in the dock are a one-time cost; only inserts are replaced with each cartridge.

### Current Status

The dock placement research recommends CPC for the front-bottom position because drips pool on the enclosure floor and auto-shutoff has high product value in this geometry. The owner prefers John Guest for cost reasons. Both remain viable — the cartridge envelope (150W x 130D x 80H) works with either fitting type. The fitting choice should be resolved before Phase 3 (sketching).

## Key Constraints

- **Valves are external.** The cartridge connects only to pump inlets and outlets. All valve routing happens in the main body. This means the cartridge's 4 fluid connections carry: hopper-or-bag-to-pump (2 inlets) and pump-to-bag-or-dispensing-line (2 outlets). The enclosure's valve assemblies switch between fill and dispense modes.
- **Electrical contacts must be separated from fluid connections.** Fluid connections are on the rear face; electrical pads are on the top face. Moisture drips downward, away from the ceiling-mounted pogo pins.
- **If JG fittings are used:** Collet release requires even, concentric pressure. The release plate must apply pressure evenly around each collet circumference. Commercial JG release tools achieve this with a shaped inset that cradles the collet.
- **If CPC fittings are used:** No release mechanism needed. Each coupling is independently thumb-released, or a single pull handle can actuate all 4 via mechanical linkage.

## Three Sub-Problems

The cartridge mechanism is a sequence of three things that happen during insertion (and reverse on removal):

1. **Guide and align** — The cartridge must find its position in the dock reliably, even with sloppy initial aim in a dark cabinet. Floor rails carry the cartridge weight and provide primary depth guidance. Side wall guides prevent lateral wobble. A 5mm chamfer on the slot entrance accepts blind insertion. The last portion of travel constrains to ~1mm tolerance so fluid connections align.

2. **Seat the fluid connections** — 4 connections mate as the cartridge reaches full insertion depth. With JG: tube stubs push into fittings, collets latch automatically. With CPC: male inserts engage female bodies, thumb latches click, auto-shutoff valves open.

3. **Confirm docked state and enable release** — The user needs clear tactile or audible feedback that the cartridge is fully seated. With JG: the cam lever flips to a locked position, providing a solid docked feel; flipping it back releases all 4 collets for removal. With CPC: the audible click of 4 CPC latches confirms connection; to remove, the user squeezes the CPC bodies (or uses a pull handle) and slides the cartridge out.

## Physical Facts (parts in hand)

### Fluid connections
- **John Guest fittings:** 1/4" push-to-connect (standard ice maker / RO type)
  - Collet travel: ~2-3mm inward push to release
  - Collet force: Light, ~1-2 lbs per fitting
  - Blue locking clips: Not present (intentionally omitted)
  - Body OD: ~12-14mm
  - Body length (union): ~38-42mm
- **CPC fittings:** PLC NSF series, valved
  - Body OD: ~22.4mm
  - Mated pair length: ~65-75mm
  - NSF 169 certified (food equipment)
  - Auto-shutoff on both halves
- **Count:** 4 total (2 pumps x inlet + outlet each)
- **Tubing:** 1/4" OD (nominally 6.35mm), 1/8" ID silicone

### Electrical connections
- **Method:** Pogo pins (dock ceiling) pressing onto flat pads (cartridge top face)
- **Minimum contacts:** 3 (ground, motor A 12V, motor B 12V)
- **Optional contacts:** Cartridge ID pin (resistor divider for type/revision identification), temperature sensor (thermistor for pump temp monitoring) — up to 6 total
- **Pad size:** 8mm x 5mm each, 10mm center-to-center
- **Self-cleaning:** Pin tip drags across elongated pad during insertion, wiping oxidation
- **Moisture separation:** Pads on cartridge top face, pogo pins on dock ceiling. Water drips downward, away from contacts. Drainage channel in dock ceiling slopes away from pin pockets.

### Pumps
- 2x Kamoer peristaltic pumps
- Each pump: ~68.6mm wide x 115.6mm deep (motor axis)
- Side-by-side layout: ~137.2mm combined width
- Orientation flexible — can be arranged however works best for the cartridge footprint
- Driven by L298N motor drivers at 12V via PWM (drivers in main body, not in cartridge)
- Max current per motor: ~0.85A

### Cartridge envelope
- **Recommended: 150W x 130D x 80H mm** (unchanged from prior research)
- Pumps dictate minimum depth (~130mm for 115.6mm motors + mounting clearance)
- 150mm width accommodates 137.2mm side-by-side pumps with margin
- 80mm height provides adequate room for pumps, tubing routing, and connector protrusions
- Smaller front panel slot is easier to seal against moisture and dust
- The triangular void has far more space than needed — the envelope is constrained by the pumps, not by the void geometry

### Critical measurements (need calipers)
- Tube OD (nominally 1/4" / 6.35mm)
- Collet ring outer diameter (on the specific JG fittings in hand)
- Inner diameter where collet meets the fitting body
- CPC insert OD and engagement depth (if CPC fittings are ordered)

## Research

### Round 1: Mechanism Families
Each sub-problem has a corresponding research document exploring mechanism families and tradeoffs:

- [Collet Release Mechanics](../research/collet-release.md) — JG collet behavior and release strategies
- [Guide & Alignment Mechanisms](../research/guide-alignment.md) — rail, funnel, and chamfer approaches
- [Electrical Mating Approaches](../research/electrical-mating.md) — pogo pins, spring contacts, edge connectors
- [Cam & Lever Mechanisms + Prior Art](../research/cam-lever.md) — cam lever design for JG release (only relevant if JG fittings are chosen)

### Round 2: Component-Level Design
Grounded in specific dimensions from parts in hand and Round 1 research:

- [Release Plate](../research/release-plate.md) — Stepped bore geometry, hole arrangements, first-print spec (only relevant if JG fittings are chosen)
- [Mating Face](../research/mating-face.md) — Tube port layout, fitting mounting, electrical contact placement, guide features
- [Cartridge Envelope](../research/cartridge-envelope.md) — Pump dimensions, arrangements, tubing routing, bounding volumes
- [Pump Mounting](../research/pump-mounting.md) — Mounting features, vibration isolation, strain relief, wire routing

### Round 3: Dock Placement and Fitting Alternatives
Research driven by the diagonal interleave enclosure layout:

- [V1 Cartridge Dock Placement](../research/v1-cartridge-dock-placement.md) — Front-bottom triangular void geometry, envelope optimization, ergonomics, fluid/electrical connection analysis for the diagonal interleave layout
- [Fitting Alternatives](../research/fitting-alternatives.md) — Comprehensive comparison of JG, CPC, barb, Luer, bayonet, magnetic, and press-fit options. Recommends CPC for auto-shutoff and simplicity; JG as fallback.

## Design Approach (4 Phases)

1. **Catalog functional requirements** — Done (above)
2. **For each requirement, explore mechanism families** — Research documents (above)
3. **Pick a mechanism family for each requirement, then sketch** — Use OnShape to prove chosen mechanisms can physically coexist in the available space
4. **AI generates the parametric model** — Once describable in terms of mechanism choices, use CadQuery via AI to generate the actual model

## CAD Tooling Notes

- **Printer:** Bambu H2C
- **CAD:** OnShape (180-day trial) for visual design + learning; CadQuery (Python) for AI-generated parts
- **Workflow:** Learn operations in OnShape -> describe to AI in shared vocabulary -> AI generates CadQuery Python -> export STEP/STL -> print
- **Source of truth:** TBD — likely CadQuery for simple mechanical parts, OnShape for complex shapes requiring visual iteration
- **Key operations** for this project: sketch, extrude, hole, linear pattern, fillet, chamfer, shell, boolean subtract
