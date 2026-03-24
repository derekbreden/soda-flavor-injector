# Decision Point 3: Cartridge-to-Dock Interface Mechanism

**Status:** Open -- awaiting decision
**Depends on:** DP1 (Enclosure Dimensions), DP2 (Bag Strategy)
**Blocks:** Mating face design, dock construction, lever placement, alignment strategy, user workflow, cartridge envelope depth budget, cartridge swap experience

## Why This Matters

The cartridge-to-dock interface mechanism is the third most depended-upon decision in the project. It determines:

- **Mating face layout**, because the rear face of the cartridge must be designed around whatever connection/disconnection method is chosen.
- **Dock construction**, because the dock shelf, walls, and ceiling must accommodate the mating geometry.
- **Cartridge depth budget**, because different mechanisms consume different amounts of space behind the pumps.
- **User workflow**, because the mechanism defines the physical steps a user performs during every cartridge swap.
- **Alignment strategy**, because fine alignment tolerances change depending on whether fittings must be concentric (John Guest) or simply axially aligned (CPC) or magnetically guided.
- **Lever and front-face interaction design**, because Option A requires a lever on the front face while other options do not.
- **Electrical contact strategy**, because the seating motion (linear push vs. twist vs. pull-apart) determines how and when pogo pins engage.

At least 8 documents directly build on this choice, and several treat the eccentric cam lever design as already decided.

## Current State

The research documents converged on a specific mechanism (eccentric cam lever + release plate) through a series of investigations:

- `requirements.md` defines three sub-problems: guide/align, seat fluid connections, secure/release.
- `cam-lever.md` recommends an eccentric cam lever (bicycle quick-release style).
- `collet-release.md` establishes that John Guest collets need even, concentric pressure via a stepped bore.
- `release-plate.md` designs the release plate: 6mm thick, 4 stepped bores in a 2x2 grid at 15mm center-to-center spacing.
- `mating-face.md` integrates everything: tube stubs on the rear face, electrical contacts on top, lever on front.
- `release-mechanism-alternatives.md` evaluates and rejects alternatives (hand disconnect, CPC couplings, dock-side sleeves, barb fittings).
- `cartridge-change-workflow.md` models the user experience at 19-25 seconds for the baseline design.
- `electrical-mating.md` recommends pogo pins (dock ceiling) pressing against brass pads (cartridge top).
- `guide-alignment.md` recommends two-stage alignment: coarse rectangular rails + fine tapered pins.
- `dock-mounting-strategies.md` designs the dock as a structural shelf.

Multiple documents treat the eccentric cam + release plate as decided. It was a research recommendation, not a decision.

---

## Options

### Option A: Eccentric Cam Lever + Release Plate (Current Research Recommendation)

User flips a lever on the front of the cartridge. The lever drives a push rod through the cartridge interior, pushing a release plate that simultaneously depresses all 4 John Guest collets, allowing tube withdrawal.

**Key design parameters:**
- Lever: eccentric cam with 1-1.5mm eccentricity, over-center self-locking
- Release plate: 6mm thick, 4 stepped bores in 2x2 grid at 15mm center-to-center
- Tube stubs: ~29-30mm long, protruding from cartridge rear face through release plate into dock fittings
- Alignment: rectangular rails (coarse) + tapered pins (fine)
- Electrical: pogo pins on dock ceiling press against brass pads on cartridge top

**Pros:**
- One-motion release: flip lever and all 4 fittings disconnect simultaneously.
- Self-locking over-center position prevents accidental release.
- 10:1+ mechanical advantage makes lever force easy.
- Fast swap: 19-25 seconds total per the workflow analysis.
- Proven precedent in bicycle quick-releases and inkjet cartridges.
- John Guest fittings auto-seal when tubes withdraw, so no dock-side drip.

**Cons:**
- Complex internal mechanism. Push rod must route between the two pumps, and the release plate must travel on guided dowel pins.
- Tight tolerances on the release plate stepped bores. Each bore must match John Guest collet geometry exactly.
- Adds depth to the cartridge. Release plate + travel + tube stubs consume approximately 35mm behind the pumps.
- Stepped bore dimensions are inferred from patents, not measured on physical fittings. Must be validated with real John Guest hardware.
- If any one bore is slightly off, that fitting will not release cleanly.
- 3D printing may not produce adequate circularity on the stepped bores.

**Downstream impacts if chosen:**
- Existing research documents remain valid as-is.
- Cartridge depth budget must accommodate ~35mm behind the pumps.
- Front face must include lever pocket and clearance arc.
- Push rod routing constrains pump placement within the cartridge.

---

### Option B: Individual Hand Disconnect (No Release Plate)

User manually presses each John Guest collet release ring individually to disconnect each fitting, then pulls the cartridge out. No lever, no release plate, no push rod.

**Key design parameters:**
- User presses the collet ring on each fitting (like using the PI-TOOL or a thumbnail)
- 4 sequential disconnect operations
- No internal mechanism whatsoever

**Pros:**
- Dramatically simpler mechanical design. No internal mechanism, no custom tolerance-critical parts.
- No release plate means no stepped bore tolerances to worry about.
- No push rod routing through the cartridge interior.
- Each fitting disconnect is independent. One bad fitting does not block the others.
- Fewer custom parts overall.
- Much easier to prototype and iterate on.

**Cons:**
- Slower swap: 75-135 seconds per the workflow analysis.
- Requires two-handed operation (one hand to press collet, one hand to pull tube).
- User must reach behind the cartridge to access collet rings, which is an ergonomic challenge in an under-sink cabinet.
- Four individual operations instead of one.
- No locked/released state feedback for the user.
- Less polished user experience.

**Downstream impacts if chosen:**
- `cam-lever.md`, `collet-release.md`, `release-plate.md` become irrelevant.
- `mating-face.md` simplifies significantly (no lever, no push rod bore).
- `cartridge-change-workflow.md` must be rewritten for the slower workflow.
- Cartridge depth budget shrinks (no release plate, no push rod), freeing space.
- Front face design simplifies (no lever pocket).
- `dock-mounting-strategies.md` may simplify (no lever clearance needed).

---

### Option C: CPC Quick-Disconnect Couplings (Replace John Guest Fittings)

Replace John Guest push-to-connect fittings entirely with CPC (Colder Products) dry-break quick-disconnect couplings. Each coupling has a body (dock-side) and an insert (cartridge-side). Pulling the cartridge out disconnects all couplings. Internal valves seal both sides automatically.

**Key design parameters:**
- 4 CPC coupling pairs (body + insert each)
- Pull-to-disconnect, push-to-connect
- Both halves auto-seal on disconnection

**Pros:**
- No release mechanism needed at all. Pulling the cartridge out disconnects the fluid lines.
- Both sides auto-seal on disconnect, so no drip from either the cartridge or the dock.
- Industrial reliability, rated for thousands of cycles.
- No custom parts for the disconnect mechanism itself.
- Clean, professional connection.

**Cons:**
- Expensive: $10-15 per coupling x 4 = $40-60, compared to roughly $5 total for 4 John Guest fittings.
- Each CPC coupling is physically larger than a John Guest fitting, which affects the cartridge depth budget.
- Reconnection requires moderate insertion force (spring-loaded internal valves).
- If any coupling binds during insertion, the user may not know which one is misaligned.
- Incompatible with the rest of the plumbing system, which uses John Guest fittings throughout. Adapters or transitions would be needed.
- Less community knowledge and fewer off-the-shelf accessories compared to John Guest.

**Downstream impacts if chosen:**
- `cam-lever.md`, `collet-release.md`, `release-plate.md` all become irrelevant.
- `mating-face.md` must be redesigned around CPC coupling geometry.
- `release-mechanism-alternatives.md` conclusions change (CPC was rejected on cost, but the simplification benefit was underweighted).
- `cartridge-change-workflow.md` must be rewritten (faster than Option B, possibly comparable to Option A).
- Dock fittings change from John Guest to CPC bodies, affecting `dock-mounting-strategies.md`.
- Plumbing transition fittings needed where CPC meets John Guest elsewhere in the system.

---

### Option D: Magnetic Alignment + O-Ring Compression Seals (Custom Design)

Cartridge mates to dock via magnetic alignment. Tube stubs on the cartridge rear face seal into bored receivers on the dock wall using O-ring compression. No collets, no push-to-connect fittings.

**Key design parameters:**
- Neodymium magnets for alignment and retention
- O-rings on tube stubs seal against bored receivers
- No commercial fitting standard involved

**Pros:**
- Self-aligning via magnets.
- Simple seal mechanism (O-ring compression).
- Easy insertion and removal with a satisfying magnetic snap.
- Premium feel.

**Cons:**
- Entirely custom design with no commercial precedent for this combination in fluid systems.
- O-ring seals under low pressure may weep, especially with the intermittent flow patterns of a flavor injector.
- Magnetic retention force must be strong enough to hold against tube back-pressure but weak enough to allow hand removal.
- Magnetic fields near electronics and motor drivers could cause interference.
- No positive locking mechanism. The cartridge could be bumped loose.
- Completely unproven. High risk for a prototype.

**Downstream impacts if chosen:**
- Nearly all existing interface documents become irrelevant.
- Entirely new seal design, retention analysis, and magnetic field analysis required.
- Dock wall must be redesigned with bored receivers and magnet pockets.
- No off-the-shelf replacement parts for the sealing interface.

---

### Option E: Bayonet/Twist-Lock Mount

Cartridge inserts straight into the dock, then rotates approximately 30 degrees to lock. The rotation drives ramps that seat the fluid connections and engage electrical contacts.

**Key design parameters:**
- Linear insertion followed by rotational lock
- Spiral ramps on dock walls engage pins on cartridge
- Rotation compresses fluid seals and electrical contacts simultaneously

**Pros:**
- Single compound motion (insert + twist).
- Positive lock that cannot be pulled out without twisting back.
- Proven precedent in camera lenses, medical connectors, and industrial couplings.

**Cons:**
- Rotational motion under a sink cabinet is ergonomically awkward. The user must grip and twist in a confined space.
- Tube stubs would need to accommodate rotation during the locking motion, risking cross-threading or O-ring shear.
- Complex dock geometry with spiral ramps.
- Must clear surrounding plumbing and enclosure walls for the rotation arc.
- Connecting rotational motion to simultaneous fluid sealing and electrical contact is mechanically complex.

**Downstream impacts if chosen:**
- All existing interface documents must be rewritten for rotational mating.
- Dock geometry becomes significantly more complex.
- Alignment strategy changes completely (rotational registration instead of linear).
- Electrical contact design changes (sliding contacts instead of pogo pins).

---

## Key Questions for the Decision Maker

1. **How often will you swap cartridges?** If it is a monthly maintenance task, swap speed matters less and Option B's simplicity becomes more attractive. If it is weekly or more frequent, the one-motion release of Option A or the pull-to-disconnect of Option C is more valuable.

2. **How comfortable are you with tight-tolerance 3D printed parts?** Option A requires stepped bores that must match John Guest collet geometry precisely. If you prefer simpler mechanisms that are forgiving of print tolerances, Options B or C avoid this risk entirely.

3. **How much does "premium feel" matter vs. "works and is simple"?** Option A delivers a polished, one-lever experience. Option B is utilitarian but dramatically simpler to build and test.

4. **Is $40-60 for CPC couplings acceptable?** Option C eliminates the entire release mechanism at the cost of more expensive fittings and plumbing incompatibility with the rest of the system.

5. **Would you accept a slower hand-disconnect workflow in exchange for a dramatically simpler first prototype?** You could prototype with Option B and upgrade to Option A later if the experience demands it.

---

## Conflicts Identified

- `release-mechanism-alternatives.md` rejects CPC on cost but does not adequately weigh the massive mechanical simplification of eliminating the release plate, push rod, and lever entirely.
- `cartridge-change-workflow.md` compares swap times, but those times assume a specific dock geometry that has not been physically built or tested.
- Multiple documents treat the cam lever design as decided. Switching to Option B or C would invalidate significant research work across at least 6 documents.
- `collet-release.md` stepped bore dimensions are inferred from patents, not measured on physical John Guest fittings. This is a risk for Option A regardless of whether it is chosen.

---

## Documents That Must Be Updated When This Decision Is Made

**Core interface documents (affected by any choice):**
- `requirements.md`
- `mating-face.md`
- `cartridge-change-workflow.md`
- `release-mechanism-alternatives.md`
- `guide-alignment.md`
- `electrical-mating.md`
- `dock-mounting-strategies.md`

**Documents that become irrelevant if Option A is not chosen:**
- `cam-lever.md`
- `collet-release.md`
- `release-plate.md`

**Documents with indirect impacts:**
- `cartridge-envelope.md` (depth budget changes with mechanism choice)
- `pump-mounting.md` (push rod routing is only relevant for Option A)
- `front-face-interaction-design.md` (lever presence/absence)
- `under-cabinet-ergonomics.md` (swap workflow and motion requirements change)
