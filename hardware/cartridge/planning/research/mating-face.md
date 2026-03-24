# Mating Face Layout — Research & Design

The mating face is the interface between the cartridge and the dock. Every connection crosses this boundary: 4 tube ports, electrical contacts, guide/alignment features, and the release mechanism. This document explores how to arrange all of these in a way that is physically compatible, printable, and easy to assemble.

This is the keystone layout decision. Once the mating face geometry is committed, it constrains the cartridge body dimensions, the dock cavity, the release plate shape, and the cam lever placement. Everything else flows from here.

**Critical design principle: the release plate, cam, and lever are all part of the cartridge (the removable part).** The dock is a simple receptacle — fittings in a wall, alignment pins, pogo pins, and guide rails. This follows the universal prior art pattern: bicycle QR levers are on the wheel, server blade ejectors are on the blade, power tool battery latches are on the battery. The user operates the lever and removes the cartridge in one motion, from one location, with one hand.

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
| Electrical contacts | 3 pogo pins, dock side | electrical-mating.md |
| Contact pad size | ~8mm x 5mm each | electrical-mating.md |
| Electrical-to-water separation | 10-20mm minimum | electrical-mating.md |
| FDM sliding clearance | 0.3-0.5mm per side | guide-alignment.md |
| Alignment pins | 15-20 deg taper, 8-10mm base | guide-alignment.md |
| Cam eccentricity | 1-1.5mm for 2-3mm stroke | cam-lever.md |
| Pump dimensions (each) | 115.6 x 68.6 x 62.7mm | cartridge-envelope.md |
| Preferred pump arrangement | Side-by-side, motors same direction | cartridge-envelope.md |
| Target cartridge envelope | ~140 x 90 x 100mm (W x H x D) | cartridge-envelope.md |

---

## 1. How the Release Plate Works on the Cartridge

### The Key Insight

The release plate is part of the cartridge, not the dock. The tube stubs are rigidly fixed to the cartridge body. The plate slides over the tube stubs on the cartridge's front face, between the cartridge body wall and the tube stub tips.

### Mechanical Sequence

**Insertion (lever open, plate retracted):**

```
    CARTRIDGE (moving right →)                    DOCK WALL

    ┌──────────┐                                  ┌──────┐
    │ cartridge│   plate     tube stubs            │fitting│
    │   body   │  ┌──┐   =================>       │collet │
    │          │  │  │   =================>       │      │
    │          │  └──┘                             │      │
    └──────────┘                                  └──────┘
                  ↑ retracted
                  plate sits against
                  cartridge body wall
```

When the plate is retracted (pulled back toward the cartridge body), the tube stubs protrude past the plate's face. The stubs are free to enter the John Guest fittings. The fittings' collets grip the tubes automatically during insertion — no mechanism needed.

**Locked (lever closed, plate still retracted):**

The lever's over-center cam locks the cartridge in the docked position. The plate stays retracted. The fittings hold the tubes; the lever adds rigidity and a "docked" feel.

**Removal (lever opened, plate extends forward):**

```
    CARTRIDGE                                     DOCK WALL

    ┌──────────┐                                  ┌──────┐
    │ cartridge│        plate  tube stubs          │fitting│
    │   body   │       ┌──┐=================>     │collet │
    │          │       │  │=================>     │ ←pushed│
    │          │       └──┘                       │      │
    └──────────┘         ↑ extended               └──────┘
                         plate has pushed forward,
                         stepped bores engage collets,
                         collets release tubes
```

When the lever is opened, the cam drives the release plate forward (toward the dock). The plate's stepped bores slide over the collet rings and push them inward, releasing the gripper teeth. With all 4 collets released, the user slides the cartridge out. The plate stays extended (held by the cam position) throughout the withdrawal motion.

### What This Means for the Mating Face

The cartridge's mating face (the front wall facing the dock) must accommodate:
- 4 tube stub pass-throughs (holes in the cartridge body wall)
- The release plate sliding in front of those pass-throughs
- Guide slots/rails for the plate's linear travel
- The cam pivot and lever mechanism
- The cam-to-plate interface

The dock's mating face is much simpler:
- 4 John Guest fittings mounted in a wall
- 2 tapered alignment pins
- 3 pogo pins (or pogo pins on a different face)
- Guide rail channels for the cartridge body

---

## 2. Tube Port Arrangements

### Constraints on Port Spacing

From collet-release.md and release-plate.md, the minimum center-to-center spacing between adjacent fittings is driven by the release plate's outer bore (cradle) diameter:

| Parameter | Value |
|---|---|
| Outer bore diameter | 12.5mm |
| Minimum wall between bores | 1.5mm (recommended) |
| Minimum C-C spacing | 14.0mm |
| Recommended C-C spacing | 15.0mm |
| Comfortable C-C spacing | 18-22mm |

The fitting body OD (~12.7mm) also sets a floor: at 15.0mm C-C, fitting bodies have only 2.3mm clearance between them. This is tight but physically workable.

### Pump Internal Layout Influence

Each Kamoer KPHM400 pump has inlet and outlet barbs on the top of the pump head, spaced ~20-25mm apart. In the side-by-side arrangement (Arrangement A from cartridge-envelope.md), both pumps have their heads facing the mating face, with barbs pointing upward.

The tube routing from pump barbs to mating face stubs involves a 90-degree bend from vertical to horizontal. The internal routing is simplest when the mating face stub positions correspond naturally to where the tubes arrive — meaning the stub pattern should roughly match the pump outlet geometry.

With two pumps side by side, each with two ports roughly 20-25mm apart, the natural tube arrival pattern is:

```
    Pump 1 ports      Pump 2 ports
       ↓   ↓            ↓   ↓
    (routed down and forward to mating face)
```

This naturally suggests either a horizontal line or a 2x2 grid arrangement.

### Option A: 4 in a Horizontal Line

```
    ┌──────────────────────────────────────────────────────────────┐
    │                                                              │
    │    (P1-IN)     (P1-OUT)     (P2-IN)     (P2-OUT)            │
    │      O           O            O           O                 │
    │                                                              │
    └──────────────────────────────────────────────────────────────┘
         ←── 15 ──→  ←── 15 ──→  ←── 15 ──→
                     45mm total span
```

| Dimension | Value |
|---|---|
| Span (outer hole C-C) | 45.0mm |
| Min face width for ports | 45.0 + 12.5 + 2x3.0 = 63.5mm |
| Min face height for ports | 12.5 + 2x3.0 = 18.5mm |

**Pros:**
- Simple tube routing — all tubes arrive from the same direction
- Release plate is a narrow bar, easy to guide linearly
- Natural fit for side-by-side pumps: each pump's pair is adjacent

**Cons:**
- Wide (63.5mm minimum) — pushes the mating face toward the full cartridge width
- Long plate has a 22.5mm maximum moment arm from center to outer bores; tilt risk is highest along the long axis
- Release plate guide features must control tilt across a 45mm span

### Option B: 4 in a Vertical Line

```
    ┌──────────────────┐
    │                  │
    │     (P1-IN)      │
    │       O          │
    │     (P1-OUT)     │
    │       O          │
    │     (P2-IN)      │
    │       O          │
    │     (P2-OUT)     │
    │       O          │
    │                  │
    └──────────────────┘
```

| Dimension | Value |
|---|---|
| Span (outer hole C-C) | 45.0mm |
| Min face width for ports | 12.5 + 2x3.0 = 18.5mm |
| Min face height for ports | 45.0 + 12.5 + 2x3.0 = 63.5mm |

**Pros:**
- Very narrow face width — leaves room for lever mechanism on the side
- Tall plate could be guided by long vertical rails

**Cons:**
- Tube routing from side-by-side pumps to a single vertical column requires crossing or funneling tubes from a wide source to a narrow target — adds routing complexity
- Same tilt risk as the horizontal line (45mm span, just rotated 90 degrees)
- The 63.5mm height may exceed the comfortable cartridge height (target ~90mm)

### Option C: 2x2 Grid

```
    ┌───────────────────────────────────────┐
    │                                       │
    │      (P1-IN)          (P2-IN)         │
    │        O                O             │
    │                                       │
    │      (P1-OUT)         (P2-OUT)        │
    │        O                O             │
    │                                       │
    └───────────────────────────────────────┘
         ←── 15 ──→
              ↕
            15mm
```

| Dimension | Value |
|---|---|
| Horizontal span | 15.0mm |
| Vertical span | 15.0mm |
| Min face width for ports | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Min face height for ports | 15.0 + 12.5 + 2x3.0 = 33.5mm |
| Release plate footprint | ~39.5 x 39.5mm (with guide pin margin) |

**Pros:**
- Most compact: 33.5mm square port zone leaves the most room on the mating face for the lever, electrical contacts, and alignment features
- Best tilt resistance: maximum moment arm is only 10.6mm (vs 22.5mm for the linear arrangements)
- Symmetric — cam force applied at center is equidistant from all 4 bores
- Natural pairing: each pump's inlet/outlet in one column

**Cons:**
- Tight fitting clearance at 15mm C-C (only 2.3mm between fitting bodies). Workable but requires precise dock fabrication
- Tube routing from pump heads must converge to a compact 15x15mm grid

### Option D: Diamond (Rotated 2x2)

```
    ┌───────────────────────────────────────────┐
    │                                           │
    │              (P1-IN)                      │
    │                O                          │
    │                                           │
    │   (P1-OUT)              (P2-IN)           │
    │     O                     O               │
    │                                           │
    │              (P2-OUT)                     │
    │                O                          │
    │                                           │
    └───────────────────────────────────────────┘
```

| Dimension | Value |
|---|---|
| Bounding width | 21.2 + 12.5 + 2x3.0 = 39.7mm |
| Bounding height | 21.2 + 12.5 + 2x3.0 = 39.7mm |

**Pros:**
- All 4 holes equidistant from center — maximum symmetry
- Slightly more fitting clearance between adjacent ports than the 2x2 grid

**Cons:**
- Larger bounding box than 2x2 (39.7mm vs 33.5mm) for the same center spacing
- Diamond doesn't align with the rectangular cartridge body geometry
- Complex tube routing to non-rectangular positions
- No practical advantage over the 2x2 grid

### Option E: 2 Pairs with Spacing Between

```
    ┌──────────────────────────────────────────────────────┐
    │                                                      │
    │   (P1-IN)   (P1-OUT)          (P2-IN)   (P2-OUT)    │
    │     O         O                  O         O         │
    │                                                      │
    └──────────────────────────────────────────────────────┘
       ←── 15 ──→   ←──── 25 ────→   ←── 15 ──→
                     55mm total span
```

Each pump's inlet/outlet is paired at 15mm C-C, with a 25mm gap between pairs.

| Dimension | Value |
|---|---|
| Total span | 55.0mm |
| Min face width | 55.0 + 12.5 + 2x3.0 = 74.0mm |
| Min face height | 12.5 + 2x3.0 = 18.5mm |

**Pros:**
- Matches the physical separation of the two pumps in the side-by-side arrangement
- Room between pairs for structural features or the cam mechanism
- Each pump's tubes route to a closely-spaced pair, which is natural

**Cons:**
- Widest arrangement (74mm)
- The release plate spans 55mm with a 27.5mm moment arm — worst tilt risk of all options
- The gap between pairs doesn't provide structural benefit (the release plate must span the full width anyway)

### Arrangement Comparison

| Arrangement | Face Width | Face Height | Max Moment Arm | Tilt Risk | Tube Routing | Dock Complexity |
|---|---|---|---|---|---|---|
| A: Horizontal line | 63.5mm | 18.5mm | 22.5mm | High | Simple | Simple |
| B: Vertical line | 18.5mm | 63.5mm | 22.5mm | High | Complex | Simple |
| **C: 2x2 grid** | **33.5mm** | **33.5mm** | **10.6mm** | **Low** | **Moderate** | **Moderate** |
| D: Diamond | 39.7mm | 39.7mm | 10.6mm | Low | Complex | Complex |
| E: Paired | 74.0mm | 18.5mm | 27.5mm | Highest | Simple | Simple |

### Recommendation

**2x2 grid (Option C) is the strongest choice.** The compact symmetric footprint halves the maximum moment arm compared to any linear arrangement, directly addressing the plate tilt failure mode. It uses the least mating face area, leaving room for the lever, electrical contacts, and alignment features. The tight fitting clearance (2.3mm) is the primary risk, but it's physically workable.

**Horizontal line (Option A) is the fallback** if the 2x2 fitting clearance proves too tight with the specific fittings in hand. It trades a wider mating face for simpler tube routing and more fitting clearance.

The natural pairing for the 2x2 grid groups each pump's inlet and outlet in one column:

```
    Pump 1 IN    Pump 2 IN
        O            O

    Pump 1 OUT   Pump 2 OUT
        O            O
```

---

## 3. Release Plate on the Cartridge — Mechanical Integration

### How the Plate Sits Relative to the Tube Stubs

The tube stubs are hard 1/4" OD tubes fixed rigidly to the cartridge body. They pass through holes in the cartridge's front wall and extend outward. The release plate sits in front of the cartridge body wall, between the wall and the tube stub tips.

```
    Side cross-section (one tube stub + plate):

    CARTRIDGE BODY                    DOCK FITTING
    WALL
    ┌────────┐                        ┌─────────────┐
    │        │  plate     tube stub   │   collet    │
    │  tube  ├──┤    ├═══════════════►│             │
    │  route │  │    │                │   O-ring    │
    │        ├──┤    ├═══════════════►│             │
    │        │  plate                 │   tube stop │
    └────────┘                        └─────────────┘
              ↑                   ↑
              plate slides        stub tip inserts
              on this axis        into fitting
```

The plate has 4 stepped bores (8.0/10.5/12.5mm from release-plate.md). The tube stubs (6.35mm OD) pass through the 8.0mm center holes with 0.825mm clearance per side. The plate slides freely along the stubs.

### Plate Travel Direction

**Retracted (toward cartridge body):** The plate sits flush against or near the cartridge body wall. Tube stubs protrude past the plate face by enough length to fully insert into the fittings (~15mm insertion depth + 3mm plate travel = 18mm protrusion past the plate when retracted).

**Extended (toward dock/fittings):** The plate slides forward along the tube stubs. The stepped bores engage the collet rings — the outer bore (12.5mm) surrounds the collet ring (11.4mm) for lateral constraint, and the inner lip (10.5mm) pushes the collet face inward to release the gripper teeth.

### Tube Stub Length Calculation

The tube stubs must be long enough to:
1. Pass through the cartridge body wall: ~3mm
2. Pass through the plate thickness: 6mm
3. Span the plate travel gap when plate is retracted: 3mm
4. Insert into the fitting to the tube stop: 15mm
5. Safety margin: 2-3mm

**Total stub length from inside cartridge wall: ~29-30mm**

When the plate is retracted, the stubs protrude ~24mm past the cartridge body wall (30mm total - 3mm wall - 3mm plate retracted position). When the plate extends 3mm, the stubs protrude ~21mm past the plate face — still fully inserted in the fittings.

### Plate Guide Features on the Cartridge

The plate must translate purely axially (no tilt). Since the plate is part of the cartridge, its guide features are also part of the cartridge.

**Recommended approach (from release-plate.md): steel dowel pin guides.**

Two or four 3mm steel dowel pins are press-fit into the cartridge body wall (or into bosses on the cartridge body). The release plate has matching slots (3.3mm wide, 7.3mm long) that slide along the pins. The pins constrain the plate to axial motion only.

```
    Front view of cartridge mating face area:

    ┌──────────────────────────────────────────────┐
    │                                              │
    │  ○ guide pin          guide pin ○            │
    │                                              │
    │         O (P1-IN)     O (P2-IN)              │
    │                                              │
    │         O (P1-OUT)    O (P2-OUT)             │
    │                                              │
    │  ○ guide pin          guide pin ○            │
    │                                              │
    └──────────────────────────────────────────────┘

    ○ = 3mm steel dowel pins fixed in cartridge body
    O = tube stubs passing through plate's stepped bores
```

The pins extend forward from the cartridge body wall through the plate slots and may protrude slightly past the plate face. The plate slides on the pins through its full 3mm travel.

---

## 4. Lever and Cam Placement on the Cartridge

### Design Principle

The lever is on the cartridge. The user grips the lever from the front (the user-facing side when reaching into the under-sink cabinet). Flipping the lever drives the eccentric cam, which pushes the release plate forward to release the collets. Closing the lever retracts the cam, and the collet springs push the plate back.

The lever must also provide the "locked" feel when the cartridge is docked. The over-center cam position locks the lever in the closed position, preventing the cartridge from working loose.

### Lever Position Options

#### Option 1: Top of Cartridge Front Face

```
    Front view:

    ┌──────────────────────────────────────┐
    │  ╔══════════════════════════╗        │
    │  ║   LEVER (pivots here →)●║        │ ← lever across the top
    │  ╚══════════════════════════╝        │
    │                                      │
    │         O        O                   │ ← tube stubs
    │         O        O                   │
    │                                      │
    └──────────────────────────────────────┘
```

The lever spans the top of the cartridge front face, pivoting on one side. The cam is at the pivot point. A linkage or direct cam contact translates the cam's rotational output to the release plate below.

**Pros:**
- Natural hand position: reach up and flip the lever down/up
- Lever is visible and accessible from the front
- The lever handle can extend above the cartridge for easy grip

**Cons:**
- The cam is at the top, but the release plate center is lower (at the tube stub grid center). The force must be transmitted downward, which could tilt the plate unless a push rod or yoke distributes the force centrally.
- If the lever extends above the cartridge, it increases the effective height

#### Option 2: Side of Cartridge

```
    Front view:

    ┌──────────────────────────────────────┐
    │                                      │
    │         O        O                   │
    │                                      ║
    │         O        O                   ║ LEVER
    │                                      ║ (pivots ●)
    │                                      │
    └──────────────────────────────────────┘
```

The lever is on one side of the cartridge face, pivoting vertically. The cam acts directly on the release plate from the side.

**Pros:**
- Cam can act directly on the plate edge (no linkage needed)
- Natural for a bicycle-QR-style flip motion

**Cons:**
- Force applied to the plate edge, not the center — creates a tilt moment
- Requires the plate guide pins to resist the resulting moment
- Side-mounted lever may be harder to reach in tight under-sink space
- Violates the "one location" UX principle — the lever is on the side, but the cartridge pulls out from the front

#### Option 3: Center of Cartridge Front Face

```
    Front view:

    ┌──────────────────────────────────────┐
    │                                      │
    │       O              O               │
    │           ●══════╗                   │
    │           LEVER  ║                   │
    │       O          ║   O               │
    │                  ║                   │
    └──────────────────────────────────────┘
```

The lever pivot is centered between the 4 tube stubs, with the lever handle extending downward or to one side.

**Pros:**
- Cam acts directly at the plate center — zero tilt moment
- Best force distribution to all 4 collets

**Cons:**
- The cam pivot occupies the center of the mating face, which is also where tube stubs are closest together
- Mechanical interference between the cam body and the tube stubs / release plate bores
- The lever may obstruct tube insertion/removal

#### Option 4: Top of Cartridge, Central Cam with Push Rod (Recommended)

```
    Side cross-section:

                    lever handle
                   ╔═══════════╗
                   ║           ║
    ┌──────────────●───────────║──────────────┐
    │              │cam        ║              │  cartridge body
    │              │           ║              │
    │         push rod                        │
    │              │                           │
    │         [release plate]                  │
    │         O    │    O                      │  tube stubs
    │         O    │    O                      │
    └──────────────────────────────────────────┘
```

The lever pivots at the top of the cartridge front face. An eccentric cam at the pivot drives a push rod (or flat yoke) downward to the center of the release plate. The push rod transmits force to the plate's center, minimizing tilt.

**Pros:**
- Cam force applied at the plate center via push rod — best force distribution
- Lever is on top, accessible from the front — natural reach
- Lever handle can fold flat against the cartridge top when closed (bicycle QR style)
- Clear separation between lever (top) and tube stubs (center/bottom)

**Cons:**
- Adds a push rod component between cam and plate
- Slightly more complex assembly
- The push rod must be rigid enough to not flex under 12-20N load (trivially achievable in PETG)

### Lever Length

From cam-lever.md: a lever of 50-100mm provides comfortable force multiplication. With 1.5mm cam eccentricity delivering 3mm plate stroke, and 12-20N total actuation force, the required input force at a 75mm lever handle is approximately:

```
    Input force = (actuation force × cam radius) / lever length
                = (20N × 5mm) / 75mm
                = 1.3N (~0.3 lbf)
```

This is extremely light — the lever's primary purpose is tactile feedback and over-center locking, not force multiplication.

### Recommendation

**Option 4 (top lever with central push rod)** provides the best combination of:
- One-handed operation from the front
- Central force application to the plate (minimizing tilt)
- Clear separation between the lever zone and the tube/fitting zone
- The lever folds flat when closed, maintaining a compact profile

---

## 5. Electrical Contact Placement

### The Recommendation from electrical-mating.md

Electrical contacts should ideally be on a **different face** than water fittings for moisture isolation. If they must share a face: electrical above water, with a dam/channel between them.

### Option 1: Top Face of Cartridge (Recommended)

```
    Cartridge top view:
    ┌─────────────────────────────────────────┐
    │                                         │
    │      [=]    [=]    [=]                  │   3 pogo target pads
    │                                         │
    └─────────────────────────────────────────┘
         ← insertion direction →
```

3 flat brass or nickel-plated pads on the cartridge top face, contacted by 3 pogo pins mounted in the dock ceiling. The pads are elongated in the insertion direction (e.g., 10mm long x 5mm wide) to provide wipe action during the slide-in motion.

**Pros:**
- Complete physical separation from water fittings (different face)
- Water drips down (gravity), away from the electrical contacts above
- The dock ceiling is dry — no fittings, no O-rings, no potential leaks
- Pogo pins in the dock ceiling press downward; the cartridge's weight and the fitting retention help maintain contact pressure

**Cons:**
- The dock must have a ceiling surface with precisely positioned pogo pins
- Wire routing inside the cartridge must go from the pump motors to the top face
- The pogo pins must be aligned with the cartridge top within the tolerance provided by the guide rails (~0.5mm)

**Pogo pin alignment tolerance:** From electrical-mating.md, pogo pins on oversized pads (8mm pad for 2mm pin) tolerate 2-3mm of misalignment. The guide rails provide ~0.5mm lateral positioning at the mating face. This is well within the pogo pin tolerance — oversized pads make this robust.

### Option 2: Front Mating Face, Above Tube Stubs

```
    Front view of cartridge mating face:

    ┌──────────────────────────────────────────┐
    │                                          │
    │     ══ lever ══                          │
    │                                          │
    │     [=]   [=]   [=]  electrical pads     │
    │     ════════════════  dam/ridge          │
    │                                          │
    │         O        O    tube stubs          │
    │         O        O                        │
    │                                          │
    └──────────────────────────────────────────┘
```

3 pogo target pads on the front face, above the tube stubs, with a raised dam between the electrical and fluid zones.

**Pros:**
- All connections on one face — single mating action
- Simpler dock (everything on one wall)

**Cons:**
- Moisture risk: water from fitting connections could splash or drip upward during insertion/removal
- The dam adds height to the mating face
- The lever at the top now shares space with both electrical pads and the lever mechanism

### Recommendation

**Option 1 (top face) is strongly preferred.** The complete physical separation between water and electrical connections eliminates an entire failure domain. The pogo pin alignment tolerance is generous enough that the guide rails easily provide adequate positioning.

---

## 6. Guide Feature Integration

### Tapered Alignment Pins (Dock Side)

From guide-alignment.md: two tapered pins (15-20 degree per-side taper, 8-10mm base diameter) at the mating face provide final ~1mm alignment. These pins are on the dock — they are fixed, permanent features.

The matching conical sockets are on the cartridge mating face. As the cartridge slides in, the tapered pins enter the sockets and correct the remaining lateral error.

### Pin Placement Relative to Tube Ports

The alignment pins should be placed symmetrically about the tube port pattern, outside the release plate envelope, and as far apart as practical to maximize their angular correction authority.

```
    Front view of DOCK mating wall:

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

    ▲ = tapered alignment pin (dock side)
    ● = John Guest fitting (dock side)
```

**Pin spacing:** With the 2x2 tube grid occupying a ~33.5mm square zone and the release plate at ~39.5mm square, the pins should be outside the plate envelope. Placing pins at the corners of a ~55-60mm square puts them ~10mm outside the plate on each side. This provides good angular correction authority.

**Pin base diameter:** 8-10mm base with 15-20 degree per-side taper. A 10mm base pin with 15-degree taper corrects ~4mm of misalignment over the last 15mm of insertion travel — ample for the ~0.5mm residual error after rail guidance.

### Rail Attachment Points

The guide rails run along the cartridge's outer sides, spanning the full insertion depth. The rails on the cartridge engage channels in the dock.

```
    Cross-section (looking from the front, cartridge in dock):

    DOCK                 CARTRIDGE                 DOCK
    ┌─────┐    ┌──────────────────────────┐    ┌─────┐
    │     │    │                          │    │     │
    │  ┌──┤    ├──┐                  ┌──┤    ├──┐  │
    │  │  │    │  │     (interior)   │  │    │  │  │
    │  │  │    │  │                  │  │    │  │  │
    │  └──┤    ├──┘                  └──┤    ├──┘  │
    │     │    │                          │    │     │
    └─────┘    └──────────────────────────┘    └─────┘
         rail channels              rail features
```

The rail features on the cartridge and the channels in the dock are sized per guide-alignment.md: 0.3-0.5mm clearance per side for sliding fits in PETG.

**Relationship to mating face:** The rails define the cartridge's lateral position within the dock. The tapered pins at the mating face provide the final ~1mm correction. The rails must be precisely located relative to the tube stubs so that when the cartridge is fully inserted, the tube stubs align with the fittings within the tapered pin correction range.

---

## 7. Fitting Mounting in the Dock

### John Guest Fitting Mounting Options

**Bulkhead / panel-mount fittings (PMI series):**
- Male-threaded body passes through a panel hole, locknut on back
- Thread size for 1/4" tube: 1/4" NPTF or 3/8" UNF
- Panel hole: ~10-12mm
- Cleanest option — fitting is rigidly fixed, collet face flush with panel

**Inline fittings (union connectors) in printed pockets:**
- Fitting body (~12.7mm OD) press-fit or bonded into a 3D-printed pocket
- Pocket diameter: 12.7 + 0.1mm (snug) to 12.7 + 0.3mm (with adhesive gap)
- Snap features or zip-tie grooves provide additional retention
- Less rigid than bulkhead mounting; the fitting can shift under repeated insertion/removal

**Recommendation:** Bulkhead / panel-mount fittings are preferred for the prototype. They provide rigid, repeatable positioning with minimal complexity. The dock wall is a flat 3D-printed panel with 4 drilled/printed holes and locknuts on the back. If bulkhead fittings for 1/4" tube are not readily available, inline fittings in printed pockets with adhesive are the fallback.

### Fitting Spacing in the Dock

For the 2x2 grid at 15.0mm C-C:

```
    Dock wall (rear view, looking from inside dock):

    ┌───────────────────────────────────────────────────┐
    │                                                   │
    │         ●────15mm────●                            │
    │         │            │                            │
    │        15mm         15mm                          │
    │         │            │                            │
    │         ●────15mm────●                            │
    │                                                   │
    │   Fitting body OD: 12.7mm                         │
    │   Gap between bodies: 15.0 - 12.7 = 2.3mm        │
    │                                                   │
    └───────────────────────────────────────────────────┘
```

The 2.3mm gap between fitting bodies is tight. If the fittings have hex flats or molded protrusions, they may interfere. **This must be verified with fittings in hand before committing to the 2x2 layout at 15mm spacing.**

If 15mm is too tight, increasing to 18mm C-C gives (18.0 - 12.7) = 5.3mm clearance — very comfortable. The plate width grows to 18.0 + 12.5 + 2x3.0 = 36.5mm (still compact). The trade-off is a slightly larger mating face and a longer maximum moment arm (12.7mm vs 10.6mm for the plate).

### Dock Wall Thickness

The dock wall must be thick enough to:
- Hold the fitting bodies securely (locknut engagement or pocket depth)
- Provide mounting for tapered alignment pins
- Resist the insertion force of 4 tubes being pushed into fittings (light — a few newtons per tube)

**Minimum wall thickness:** 5-6mm for printed PETG with bulkhead fittings. The fitting's locknut engages threads on the back side, clamping the fitting against the wall face.

---

## 8. Dock-Side Simplicity

With the release plate, cam, lever, and guide pins all on the cartridge, the dock becomes remarkably simple. Here is what the dock consists of:

### Dock Components

| Component | Location | Function |
|---|---|---|
| 4x John Guest fittings | Dock wall (mating face) | Fluid connection + passive retention |
| 2-4x tapered alignment pins | Dock wall, outside fitting pattern | Fine alignment of cartridge at end of travel |
| 3x pogo pins | Dock ceiling (top inner surface) | Electrical contact to cartridge top pads |
| Guide rail channels | Dock side walls (full depth) | Coarse guidance during insertion |
| Funnel entrance | Dock entrance (front opening) | Captures cartridge from sloppy initial aim |
| Back wall/stop | Rear of dock cavity | Limits insertion depth |

### Dock Cross-Section

```
    Side view (cartridge sliding in from left):

                     pogo pins (ceiling)
                     ↓   ↓   ↓
    ┌────────────────────────────────────────────────────┐
    │                dock ceiling                        │
    │                                                    │
    │  funnel   rail channel        fittings   back wall│
    │  ╱                            ●●         │        │
    │ ╱    ═══════════════════════  ●●         │        │
    │ ╲    ═══════════════════════             │        │
    │  ╲                            taper pins │        │
    │                               ▲  ▲       │        │
    │                dock floor                          │
    └────────────────────────────────────────────────────┘
```

### What the Dock Does NOT Have

- No release plate
- No cam mechanism
- No lever
- No moving parts of any kind

The dock is entirely passive. The only components that touch the cartridge are:
- Spring-loaded pogo pins (dock ceiling) — press against flat pads
- John Guest fittings (dock wall) — tubing pushes in and collets grip
- Tapered pins (dock wall) — static cones that the cartridge sockets slide onto
- Rail channels (dock sides) — static grooves that guide the cartridge

This passivity is a major advantage: the dock never wears out mechanically (pogo pin springs are rated for 100,000+ cycles), and all wear items (release plate, cam, guide pin sockets) are on the replaceable cartridge.

---

## 9. Overall Mating Face Dimensions

### 2x2 Grid Layout — Full Mating Face

Combining all components, the cartridge's front mating face must accommodate:

```
    Cartridge front face:

    ┌──────────────────────────────────────────────────────────────┐
    │                                                              │
    │  ╔═══════════════════════════════════════════╗               │
    │  ║  LEVER (folds flat when closed)  ●pivot   ║               │
    │  ╚═══════════════════════════════════════════╝               │
    │                                                              │
    │         ◇ taper socket              taper socket ◇           │
    │                                                              │
    │    ○ pin    O (P1-IN)     O (P2-IN)     pin ○               │
    │                    push                                      │
    │    ○ pin    O (P1-OUT)    O (P2-OUT)    pin ○               │
    │                    rod                                       │
    │         ◇ taper socket              taper socket ◇           │
    │                                                              │
    └──────────────────────────────────────────────────────────────┘

    O  = tube stub through release plate stepped bore
    ○  = release plate guide pin (fixed in cartridge body)
    ◇  = tapered pin socket (receives dock's alignment pin)
    ●  = cam pivot
```

**Dimension estimates:**

| Feature | Width contribution | Height contribution |
|---|---|---|
| Tube port zone (2x2 at 15mm) | 33.5mm | 33.5mm |
| Release plate + guide pins | 39.5mm | 39.5mm |
| Taper pin sockets (outside plate) | +20mm (10mm each side) | +20mm |
| Lever zone | (spans plate width) | +15mm (above ports) |
| Margin/walls | +6mm (3mm each side) | +6mm |
| **Total mating face** | **~66mm** | **~81mm** |

This fits comfortably within the target cartridge envelope width of 140mm and height of 90mm. The mating face is centered on the front of the cartridge, leaving ~37mm on each side for the cartridge body walls and internal structure.

### Horizontal Line Layout — Full Mating Face

If the 2x2 grid's fitting clearance is too tight, the horizontal line arrangement would produce:

| Feature | Width contribution | Height contribution |
|---|---|---|
| Tube port zone (4-in-line at 15mm) | 63.5mm | 18.5mm |
| Release plate + guide pins | 69.5mm | 24.5mm |
| Taper pin sockets | +20mm | +20mm |
| Lever zone | (spans plate width) | +15mm |
| Margin/walls | +6mm | +6mm |
| **Total mating face** | **~96mm** | **~66mm** |

This is wider (96mm) and approaches the cartridge envelope width of 140mm more closely. Still workable, but leaves less room for side features.

---

## 10. Interdependencies

### Release Plate ↔ Mating Face

The release plate bore pattern (from release-plate.md) must exactly match the fitting pattern in the dock wall and the tube stub positions on the cartridge body. All three are locked to the same C-C spacing. Changing the spacing in one place changes it everywhere.

### Cam/Lever ↔ Release Plate

The cam must deliver 3mm of axial stroke to the plate center. With the cam at the top and the plate center below, a push rod spans the distance. The push rod length depends on the mating face layout — for the 2x2 grid, the cam pivot to plate center distance is approximately 25-30mm.

### Electrical Contacts ↔ Dock Ceiling

If the electrical pads are on the cartridge top face, the dock must have a ceiling surface with pogo pins. The ceiling height determines the cartridge top-to-ceiling gap when docked — this should be close to zero (pogo pin travel is 1-2mm, so the gap equals the pogo pin compression distance). The dock rail channels must position the cartridge vertically within the pogo pin tolerance.

### Guide Rails ↔ Alignment Pins

The rails provide coarse positioning (~0.5mm). The tapered pins correct residual error to <0.5mm. The pin entrance cone diameter must be large enough to capture the worst-case rail positioning error — for 0.5mm error, a 10mm base pin with 15-degree taper captures a ~4mm misalignment window, which is more than sufficient.

### Tube Stub Positions ↔ Pump Arrangement

The tube stubs must connect to the pump barbs inside the cartridge via BPT tubing with barb reducers. For the side-by-side pump arrangement with the 2x2 grid, each pump's two tubes route from the top of the pump head (vertical) down and forward to the mating face. The pump head center is approximately 35mm from the mating face (half the cartridge depth after wall thickness). The tube routing bend radius (~15-20mm for BPT) must fit in this space.

### Cartridge Envelope ↔ Mating Face

The mating face (66 x 81mm for 2x2 grid) is smaller than the cartridge cross-section (140 x 90mm). The mating face features are centered, with the remaining perimeter area available for the cartridge shell walls, rail features, and the lever mechanism.

---

## 11. Recommendation Ranking

### Tube Port Arrangement

| Rank | Arrangement | Rationale |
|---|---|---|
| 1 | **2x2 grid, 15mm C-C** | Smallest footprint, best tilt resistance, symmetric cam loading. Verify fitting clearance. |
| 2 | 2x2 grid, 18mm C-C | Fallback if 15mm is too tight. Still compact (36.5mm square). |
| 3 | Horizontal line, 15mm C-C | Simpler tube routing, wider mating face. Higher tilt risk. |

### Release Plate Location

| Rank | Location | Rationale |
|---|---|---|
| 1 | **On the cartridge** | Follows all prior art. One-handed operation. Wear parts are replaceable. Dock stays simple. |

### Lever Placement

| Rank | Placement | Rationale |
|---|---|---|
| 1 | **Top of cartridge face, cam + push rod to plate center** | Best force distribution, accessible, folds flat. |
| 2 | Top of cartridge face, direct cam on plate edge | Simpler (no push rod), but edge loading causes tilt moment. Viable if guide pins are robust. |
| 3 | Side of cartridge | Poor UX — different location from pull direction. |

### Electrical Contact Face

| Rank | Location | Rationale |
|---|---|---|
| 1 | **Cartridge top face** | Complete moisture isolation. Gravity pulls water away. Pogo pins on dock ceiling. |
| 2 | Front mating face, above water fittings | All connections on one face, but moisture risk requires dam/channel. |

### Dock-Side Design

| Rank | Approach | Rationale |
|---|---|---|
| 1 | **Passive dock: fittings + taper pins + pogo pins + rails** | Simplest possible. No moving parts. All wear on the replaceable cartridge. |

### Overall Mating Face Dimensions

| Configuration | Face Width | Face Height | Status |
|---|---|---|---|
| **2x2 grid, 15mm, top lever** | ~66mm | ~81mm | Primary recommendation |
| 2x2 grid, 18mm, top lever | ~72mm | ~87mm | If 15mm fitting gap too tight |
| Horizontal line, 15mm, top lever | ~96mm | ~66mm | If 2x2 tube routing too complex |

---

## 12. Open Questions

1. **Fitting body clearance at 15mm C-C**: Do the specific John Guest fittings in hand have hex flats, molding flash, or other protrusions that prevent 2.3mm gap mounting? Measure with calipers.

2. **Bulkhead fitting availability**: Are John Guest bulkhead/panel-mount fittings available for 1/4" OD tube in the needed configuration? Check McMaster, Amazon, or John Guest direct.

3. **Push rod rigidity**: A 25-30mm PETG push rod under 20N load — is deflection negligible? Quick calculation: a 4mm diameter PETG rod, 30mm long, under 20N axial load compresses approximately 0.003mm. Negligible. A flat yoke (3mm x 15mm cross-section) would be even stiffer.

4. **Cam-to-plate interface**: Does the cam push directly on the push rod, or does it push on the plate through a bearing surface? A flat cam face on a flat rod end is simplest but may introduce friction. A small ball bearing at the cam tip eliminates friction but adds a part.

5. **Plate return force**: With the cam, lever, and plate all on the cartridge, what provides the return force when the lever is opened? The collet springs push the plate back toward the cartridge body. If the collets are not engaged (cartridge is undocked), there is no return force — but this is fine, because the plate position doesn't matter when the cartridge is undocked. The cam profile can also provide positive return (an eccentric cam naturally retracts when rotated back).

---

## Sources

- collet-release.md (this project) — bore dimensions, forces, failure modes
- release-plate.md (this project) — stepped bore geometry, spacing, guide features, compliance
- cam-lever.md (this project) — eccentric cam mechanics, over-center behavior, prior art
- electrical-mating.md (this project) — pogo pin recommendations, moisture separation
- guide-alignment.md (this project) — rail clearances, tapered pin geometry, FDM tolerances
- cartridge-envelope.md (this project) — pump dimensions, arrangements, bounding volumes
- pump-mounting.md (this project) — mounting features, tube exit points, wire routing
- [Bicycle Quick-Release Mechanisms (Sheldon Brown)](https://sheldonbrown.com/skewers.html) — lever-on-removable-part prior art
- [Southco Inject/Eject Mechanisms](https://southco.com/en_us_int/fasteners/inject-eject-mechanisms) — server blade ejector (lever on blade) prior art
- [John Guest OD Tube Fittings Technical Specifications](https://www.johnguest.com/sites/default/files/files/tech-spec-od-fittings-v2.pdf) — fitting body dimensions, mounting configurations
