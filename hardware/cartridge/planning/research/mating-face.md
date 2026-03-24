# Mating Face Layout — Cartridge-to-Enclosure Interface Design

The mating face is the boundary between the replaceable pump cartridge and the enclosure's internal dock. Every connection crosses this interface: 4 fluid ports, 3 electrical contacts, guide/alignment features, and the release mechanism. This document defines what connects, where, how, and why.

The enclosure is a self-contained, desktop-PC-tower-sized unit that sits on the floor of a kitchen sink cabinet. The cartridge loads from the front, like a CD drive in a PC tower or a blade server ejector. A lever on the cartridge's front face doubles as the release mechanism and extraction handle. The dock inside the enclosure is entirely passive -- no moving parts.

---

## Established Parameters

These values are derived from prior research and are not re-derived here:

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
| Pump dimensions (each) | 115.6 x 68.6 x 62.7mm | pump-mounting.md |
| Preferred pump arrangement | Side-by-side, motors same direction | cartridge-envelope.md |
| Target cartridge envelope | ~140 x 90 x 100mm (W x H x D) | cartridge-envelope.md |

---

## 1. Context: Front-Loading in an Enclosure

### Why Front-Loading

The enclosure sits on the cabinet floor, pushed against the back or side wall. The user opens the cabinet door, crouches, and reaches in. Under-cabinet ergonomics research established that comfortable forward reach is ~14" (356mm) and visibility drops sharply beyond 12" from the cabinet opening. A front-loading cartridge keeps the interaction at minimum reach depth.

The front face of the enclosure is the primary interaction surface. It hosts the two round displays (RP2040 flavor, S3 config), the cartridge slot, a status LED, and (optionally) the hopper access. The cartridge slot is the largest feature: approximately 146 x 96mm (cartridge cross-section plus rail clearance), with a 5mm chamfered entrance. This is where the user's attention goes during a cartridge swap.

### The Blade Server Ejector Analogy

The strongest mechanical analogy is a blade server sliding into a chassis:

1. The user grips the front-mounted lever
2. The lever swings to release the locking cam
3. The cartridge slides out on guide rails
4. Insertion is the reverse: slide in until the tubes engage the fittings, swing the lever closed

The lever on the cartridge's front face doubles as the extraction handle. This is a single-location, single-hand operation. The user never reaches around, over, or behind the cartridge.

### What the Dock Provides

The dock is the passive receptacle inside the enclosure. It consists of:

| Component | Location | Function |
|---|---|---|
| 4x John Guest fittings | Dock back wall | Fluid connection; collets grip tubes automatically on insertion |
| 2-4x tapered alignment pins | Dock back wall, outside fitting pattern | Fine alignment at end of insertion travel |
| 3x pogo pins | Dock ceiling (top inner surface) | Electrical contact to cartridge top pads |
| Guide rail channels | Dock side walls, full depth | Coarse lateral/vertical guidance during insertion |
| Chamfered entrance | Front opening | Captures cartridge from sloppy initial aim |

The dock has no moving parts. All wear items -- release plate, cam, lever, guide pin sockets -- are on the replaceable cartridge. The dock's only consumable-lifetime components are the pogo pin springs (rated 100,000+ cycles, far beyond the cartridge swap frequency).

---

## 2. What Crosses the Mating Face

### 2a. Fluid Connections (4 Ports)

Four 1/4" OD hard tube stubs protrude from the cartridge's rear face and insert into four John Guest push-to-connect fittings mounted in the dock wall. Each stub carries one fluid path:

| Port | Function | Direction (during dispensing) |
|---|---|---|
| Pump 1 inlet | Flavor 1 from bag to pump | Into cartridge |
| Pump 1 outlet | Flavor 1 from pump to dispensing solenoid | Out of cartridge |
| Pump 2 inlet | Flavor 2 from bag to pump | Into cartridge |
| Pump 2 outlet | Flavor 2 from pump to dispensing solenoid | Out of cartridge |

Inside the cartridge, soft BPT tubing (4.8mm ID x 8.0mm OD) connects the Kamoer KPHM400 pump barbs to brass barb reducers, which transition to 1/4" OD hard tubing (nylon or polyethylene) for the final stub that enters the John Guest fitting. Soft silicone does not hold in push-connect fittings; the stubs must be hard tubing.

The tube stubs are permanently installed during cartridge assembly. The user never touches them.

### 2b. Electrical Connections (3 Contacts)

The two Kamoer KPHM400 pumps share a common ground. Three contacts are needed:

| Contact | Function | Max Current |
|---|---|---|
| GND | Common ground for both motors | ~1.7A |
| Motor A+ | Pump 1 positive, 12V | ~0.85A |
| Motor B+ | Pump 2 positive, 12V | ~0.85A |

Three spring-loaded pogo pins in the dock ceiling press downward against three flat nickel-plated brass pads on the cartridge top face. The pads are elongated in the insertion direction (10mm x 5mm) to provide self-cleaning wipe action as the cartridge slides in.

Electrical contacts are on the cartridge's **top face**, physically separated from the fluid connections on the rear face. Water drips downward (gravity), away from the contacts. The dock ceiling is a dry surface with no fittings or O-rings. This eliminates the moisture-to-electrical failure domain entirely.

Pogo pins on oversized pads tolerate 2-3mm of lateral misalignment. The guide rails position the cartridge within ~0.5mm, well inside this tolerance.

### 2c. Mechanical Engagement

The release plate, cam lever, and guide pin sockets are all on the cartridge. The dock provides only passive mating features (fittings, taper pins, rail channels).

| Feature | Cartridge Side | Dock Side |
|---|---|---|
| Fluid retention | Tube stubs (hard 1/4" OD) | John Guest fittings (collets grip automatically) |
| Collet release | Release plate with 4 stepped bores | -- (fittings are passive) |
| Release actuation | Eccentric cam + front-mounted lever | -- |
| Plate guidance | 2-4 steel dowel pins fixed in cartridge body | -- |
| Coarse alignment | Rail features on cartridge exterior | Rail channels in dock side walls |
| Fine alignment | Conical sockets on cartridge rear face | 2-4 tapered pins on dock back wall |
| Electrical | Flat pads on cartridge top face | 3 pogo pins on dock ceiling |
| Locking | Over-center cam position holds lever closed | -- |

---

## 3. Release Plate Mechanics

### Where the Plate Lives

The release plate is part of the cartridge. It sits on the cartridge's rear face, between the cartridge body wall and the tube stub tips. The plate slides axially along the tube stubs, guided by 2-4 steel dowel pins (3mm diameter) press-fit into the cartridge body.

### Mechanical Sequence

**Insertion (lever in extraction position, plate extended):**

The user slides the cartridge into the dock. The tube stubs protrude past the release plate. As the cartridge reaches full insertion depth, the stubs enter the John Guest fittings. The collets grip the tubes automatically -- no mechanism needed. The user swings the lever to the locked position. The over-center cam holds the cartridge firmly.

**Docked (lever locked, plate retracted):**

The cam holds the lever closed. The plate is retracted against the cartridge body wall. The fittings grip the tubes. Pogo pins press against pads. The system is operational.

**Removal (lever opened, plate extends):**

The user swings the lever on the front face. The eccentric cam drives a push rod that pushes the release plate rearward (toward the dock). The plate's four stepped bores engage the four collet rings:

1. The outer bore (12.5mm) surrounds the collet ring (11.4mm) -- lateral constraint
2. The inner lip (10.5mm) pushes the collet face inward -- disengages gripper teeth

With all four collets released simultaneously, the user grips the lever (which doubles as the extraction handle) and slides the cartridge out.

### Stepped Bore Profile

Each bore replicates the geometry of a John Guest PI-TOOL release tool:

```
    ← dock side (collet faces this way)        cartridge body →

    ┌──────────────────────────────────────────────────────────┐
    │   outer bore      inner lip      tube clearance hole     │
    │   12.5mm dia      10.5mm dia     8.0mm dia (through)     │
    │   2.0mm deep      2.0mm deep     remaining thickness     │
    └──────────────────────────────────────────────────────────┘
    Total plate thickness: 6.0mm
```

The inner lip is the critical feature. Its 1.25mm annular width contacts the collet face and pushes it inward. The outer bore prevents the collet from cocking or tilting during release -- the failure mode that causes tube scoring and fitting damage.

### Tube Stub Length

The stubs must span the full assembly stack:

| Segment | Length |
|---|---|
| Cartridge body wall | 3mm |
| Release plate thickness | 6mm |
| Plate travel gap (retracted position) | 3mm |
| Insertion into fitting (to tube stop) | 15mm |
| Safety margin | 2-3mm |
| **Total** | **~29-30mm** |

---

## 4. Tube Port Arrangement

### 2x2 Grid at 15mm Center-to-Center

The four fittings are arranged in a 2x2 grid with 15mm spacing. Each pump's inlet and outlet occupy one column:

```
    Pump 1 IN    Pump 2 IN
        O            O
       15mm
    Pump 1 OUT   Pump 2 OUT
        O            O
```

This arrangement was chosen over linear alternatives for three reasons:

1. **Smallest footprint**: The port zone occupies ~33.5 x 33.5mm, leaving maximum room on the mating face for the lever, electrical contacts, and alignment features.

2. **Best tilt resistance**: The maximum moment arm from plate center to any bore is 10.6mm (diagonal). For a horizontal 4-in-line arrangement, this doubles to 22.5mm. Smaller moment arms mean the plate guide pins have an easier job preventing the tilt failure mode.

3. **Symmetric cam loading**: The cam pushes the plate center. All four bores are equidistant from the center, so the force distributes evenly to all four collets.

**Fitting clearance at 15mm**: The fitting body OD is ~12.7mm. At 15mm center-to-center, adjacent fitting bodies have 2.3mm clearance. This is tight but physically workable for round fittings without hex flats. This must be verified with the specific fittings in hand -- if too tight, increasing to 18mm C-C gives 5.3mm clearance while growing the port zone to only 36.5mm square.

### Fitting Mounting in the Dock

Bulkhead/panel-mount John Guest fittings are preferred. The fitting body passes through a hole in the dock back wall, and a locknut on the inside clamps it securely. The collet face sits flush with the wall surface facing the incoming cartridge.

The dock wall must be 5-6mm thick (PETG) to provide adequate locknut engagement and structural rigidity.

---

## 5. Lever Placement: Front Face of the Cartridge

### Design Principle

The lever is on the cartridge's front face -- the face visible when the cartridge is fully inserted in the enclosure's front-loading slot. When the cartridge is docked, the lever folds flat within the slot opening. The lever swings 180 degrees to unlock, then serves as the extraction handle.

This follows universal prior art: blade server ejector handles are on the blade's front panel. Power tool battery release buttons are on the battery. Bicycle QR levers are on the wheel. The user operates the mechanism and removes the part from the same location, with one hand.

### How the Lever Drives the Plate

The lever pivots on the cartridge's front face. An eccentric cam at the pivot (1.5mm eccentricity, providing 3mm displacement) drives a push rod that runs from the front face through the cartridge interior to the release plate on the rear face. The push rod transmits force to the plate's center, minimizing tilt.

```
    Side cross-section (cartridge, lever on left/front):

    FRONT FACE                              REAR FACE (mating)
    ┌─────────────────────────────────────────────────┐
    │ ●cam ──push rod──────────────► [release plate]  │
    │ lever                          O  O  tube stubs │
    │ pivot                          O  O             │
    └─────────────────────────────────────────────────┘
          ← user side                    dock side →
```

The lever handle extends 50-80mm from the pivot, providing comfortable grip and ample force multiplication. With 1.5mm eccentricity and 12-20N total plate force, the required input force at a 75mm lever handle is approximately 1.3N (0.3 lbf) -- the lever's primary purpose is tactile feedback and over-center locking, not force multiplication.

### Over-Center Locking

The eccentric cam goes slightly past dead center when the lever is closed, creating a self-locking position (identical to a bicycle quick-release). The lever cannot vibrate open. Opening requires deliberate force past the over-center point, providing a clear "break" feel that confirms the transition from locked to released.

The lever also needs a detent or stop at the fully open position so it stays open during cartridge withdrawal. If the lever swings closed during withdrawal, the collets re-grip the tubes and the cartridge gets stuck.

---

## 6. Electrical Contact Placement: Top Face

Electrical contacts are on the cartridge's **top face**, contacted by pogo pins mounted in the dock ceiling. Three pads (GND, Motor A+, Motor B+) are arranged in a line along the insertion axis, spaced 10mm center-to-center.

```
    Cartridge top face (looking down):

    ┌──────────────────────────────────────────┐
    │                                          │
    │      [GND]    [A+]    [B+]               │
    │       ← insertion direction →            │
    └──────────────────────────────────────────┘
```

Each pad is 10mm long x 5mm wide (elongated in the insertion direction). The pogo pin tip drags across the pad during the final millimeters of insertion, providing self-cleaning wipe action that scrapes away oxide films and dust.

### Why Top Face, Not Front Face

Placing electrical contacts on a different face from the fluid connections eliminates the moisture failure domain. Water from fitting connections can weep during insertion/removal. A raised dam or splash guard on a shared face is a mitigation, not a solution. Complete physical separation (water on the rear face, electrical on the top face) means there is no geometry in which a drip from a fitting reaches an electrical contact.

### Wire Routing Inside the Cartridge

Motor wires route from the rear of each pump through a channel in the cartridge wall to a small PCB on the top face that carries the three pogo target pads. Solder joints on the PCB are potted with hot glue or silicone for strain relief and moisture protection.

---

## 7. Guide and Alignment Features

### Two-Stage Alignment

This follows the universal pattern from server blades, printer cartridges, and power tool batteries: coarse guidance gets the module roughly in place, then fine alignment features pull it into exact position.

**Stage 1 -- Guide rails (coarse, full travel):**

Rails on the cartridge's outer sides engage channels in the dock's side walls. The rails run the full insertion depth (~100mm), constraining the cartridge to a single axis of motion. FDM clearance: 0.3-0.5mm per side for smooth sliding in PETG.

The cartridge slot entrance has a 5mm chamfer on all four edges, acting as a funnel that captures the cartridge from a sloppy initial aim. The user does not need to see the rail alignment; the chamfer guides the cartridge in.

**Stage 2 -- Tapered pins (fine, last ~15mm):**

Two to four tapered pins on the dock back wall (15-20 degree per-side taper, 8-10mm base diameter) enter matching conical sockets on the cartridge's rear face. Over the last 15mm of insertion travel, the pins correct residual lateral error to <0.5mm.

The pins are placed symmetrically about the tube port pattern, outside the release plate envelope. At the corners of a ~55-60mm square, they provide good angular correction authority. A 10mm base pin with 15-degree taper corrects ~4mm of misalignment -- ample for the ~0.5mm residual error after rail guidance.

### Poka-Yoke (Mistake-Proofing)

The cartridge can only be inserted in one orientation. Asymmetric rail profiles or a keying tab on one side prevent upside-down or reversed insertion. If it fits, it is right.

---

## 8. Overall Mating Face Dimensions

### Cartridge Rear Face (Dock-Facing)

The rear face accommodates the tube stubs, release plate, plate guide pins, and taper pin sockets:

```
    Cartridge rear face (facing dock):

    ┌──────────────────────────────────────────────────────┐
    │                                                       │
    │  ◇ taper socket                    taper socket ◇     │
    │                                                       │
    │    ○ pin    O (P1-IN)     O (P2-IN)    pin ○         │
    │                                                       │
    │    ○ pin    O (P1-OUT)    O (P2-OUT)   pin ○         │
    │                                                       │
    │  ◇ taper socket                    taper socket ◇     │
    │                                                       │
    └──────────────────────────────────────────────────────┘

    O  = tube stub through release plate stepped bore
    ○  = release plate guide pin (3mm steel dowel, fixed in cartridge body)
    ◇  = tapered pin socket (receives dock's alignment pin)
```

| Feature Zone | Width | Height |
|---|---|---|
| Tube port zone (2x2 at 15mm) | 33.5mm | 33.5mm |
| Release plate + guide pins | ~39.5mm | ~39.5mm |
| Taper pin sockets (outside plate) | +20mm | +20mm |
| Margin/walls | +6mm | +6mm |
| **Total rear face feature zone** | **~66mm** | **~66mm** |

This fits well within the 140 x 90mm cartridge cross-section, leaving ~37mm on each side for the cartridge shell walls and rail features.

### Cartridge Front Face (User-Facing)

The front face accommodates the lever mechanism:

```
    Cartridge front face (visible from enclosure front):

    ┌──────────────────────────────────────┐
    │                                      │
    │    ┌────────────────────┐            │  90mm
    │    │   Cartridge Body   │            │
    │    │                    │            │
    │    └────────────────────┘            │
    │                                      │
    │  ═══════════════╗  ← lever           │
    │                 ║    (swings left     │
    │                 ║     to unlock)      │
    └──────────────────────────────────────┘
                  140mm
```

When locked, the lever folds flat against the cartridge face within the enclosure's slot opening. When unlocked, it swings ~180 degrees, protruding 50-80mm. The user grips the extended lever and slides the cartridge out.

### Cartridge Top Face

Three pogo target pads in a line, approximately centered on the top surface.

---

## 9. Dock-Side Simplicity

With everything mechanical on the cartridge, the dock is remarkably simple:

```
    Side view (cartridge sliding in from left):

                     pogo pins (ceiling)
                     ↓   ↓   ↓
    ┌─────────────────────────────────────────────────┐
    │                dock ceiling                      │
    │                                                  │
    │  chamfer   rail channel        fittings   wall  │
    │  ╱                             ●●          │    │
    │ ╱    ═══════════════════════   ●●          │    │
    │ ╲    ═══════════════════════                │    │
    │  ╲                             taper pins  │    │
    │                                ▲  ▲         │    │
    │                dock floor                        │
    └─────────────────────────────────────────────────┘
```

**The dock has no moving parts.** It is entirely passive:
- John Guest fittings grip tubes by spring-loaded collets (passive)
- Tapered pins are fixed cones (passive)
- Pogo pins are spring-loaded contacts (passive, 100K+ cycle life)
- Rail channels are static grooves (passive)

All wear surfaces -- release plate lip edges, cam bearing surfaces, guide pin sockets, lever pivot -- are on the cartridge. The replaceable part wears; the permanent part does not.

---

## 10. Interface with the Enclosure

### Cartridge Slot on the Enclosure Front Face

The dock is permanently mounted inside the enclosure. The front face of the enclosure has a rectangular slot opening (~146 x 96mm plus chamfer) through which the cartridge slides. The slot is framed with a 5mm chamfered entrance and optionally a recessed border for visual finish.

The cartridge's front face sits flush with the enclosure's front panel when fully inserted. The lever is accessible within the slot opening.

### Routing from Dock to Internal Plumbing

Behind the dock wall, four tubes route from the dock's John Guest fittings to the enclosure's internal plumbing:

- Pump inlet fittings connect to tubing that runs to the flavor bags
- Pump outlet fittings connect to tubing that runs to the dispensing solenoid valves

The dock wall serves as the boundary between the replaceable fluid path (cartridge side) and the permanent fluid path (enclosure side). The John Guest fittings provide tool-free, push-to-connect interfaces on both sides of the wall.

### Electrical Routing

The three pogo pins in the dock ceiling connect via wire to the L298N motor drivers mounted elsewhere in the enclosure. The pogo pins are soldered to a small PCB or terminated with solder cups. Wiring runs along the dock ceiling and through the enclosure's internal cable management channels.

### Physical Integration

The dock mounts inside the enclosure with screws into heat-set inserts. It sits behind the front panel's cartridge slot, aligned so the chamfered slot entrance funnels into the dock's rail channels. The dock back wall is the structural plate that holds the John Guest fittings, tapered alignment pins, and serves as the mechanical stop for full cartridge insertion.

---

## 11. Cartridge Change Workflow (Summary)

The full workflow analysis lives in cartridge-change-workflow.md. Here is the mating-face-relevant summary:

| Step | Action | Time | Mating Face Involvement |
|---|---|---|---|
| 1 | Open cabinet, locate cartridge slot | 5-10s | Cartridge front face visible in enclosure slot |
| 2 | Swing lever to release position | 2s | Cam drives plate forward, collets release |
| 3 | Pull cartridge out by lever | 2-3s | Tube stubs exit fittings, pogo pins disconnect, rails guide withdrawal |
| 4 | Set old cartridge aside (have towel ready -- stubs drip) | 5s | Open tube stubs drip residual liquid |
| 5 | Slide new cartridge in until tubes seat | 3s | Rails guide insertion, taper pins align, stubs enter fittings, pogo pins contact |
| 6 | Swing lever to locked position | 2s | Over-center cam locks; clear "click" confirms docked state |
| **Total** | | **~19-25s** | |

The entire operation is one-handed, from the front, in one location. The user never reaches behind the enclosure, never disconnects individual fittings, and never uses tools.

---

## 12. Interdependencies

### Release Plate <-> Fitting Pattern <-> Dock Wall

The release plate bore pattern, the fitting positions in the dock wall, and the tube stub positions on the cartridge body are all locked to the same center-to-center spacing. Changing the spacing in one place changes it everywhere. The 15mm (or 18mm fallback) grid pitch is the single most constrained dimension in the entire system.

### Cam/Lever <-> Release Plate

The cam provides 3mm of stroke. With the lever on the front face and the plate on the rear face, the push rod spans the full cartridge depth (~100mm). A 4mm diameter PETG push rod under 20N axial load compresses approximately 0.003mm over 100mm -- negligible.

### Pogo Pins <-> Dock Ceiling <-> Cartridge Height

The dock ceiling must have a surface with precisely positioned pogo pins. The cartridge top-to-ceiling gap when docked equals the pogo pin compression distance (~1-2mm). The rail channels must position the cartridge vertically within the pogo pin tolerance.

### Rails <-> Taper Pins

The rails provide ~0.5mm lateral positioning. The taper pins correct residual error. The pin entrance cone must capture the worst-case rail positioning error. A 10mm base pin with 15-degree taper captures a 4mm misalignment window -- more than sufficient.

### Tube Stubs <-> Pump Arrangement

The 2x2 grid on the rear face must connect internally to two pumps arranged side-by-side. Each pump's barbs exit from the top of the pump head. BPT tubing routes from the barbs (pointing up) through a 90-degree bend (15-20mm radius) and forward to the rear face. The routing bend requires ~20mm of clearance above the pump heads and ~20mm of depth between the pump heads and the rear face.

### Cartridge Envelope <-> Enclosure Slot

The mating face features (66 x 66mm zone on the rear face) are smaller than the cartridge cross-section (140 x 90mm). The remaining perimeter is available for the shell walls, rail features, and the front-face lever mechanism. The enclosure slot opening (146 x 96mm with clearance) is determined by the cartridge cross-section plus rail clearance, not by the mating face features.

---

## 13. Open Questions

1. **Fitting body clearance at 15mm C-C**: Do the specific John Guest fittings have hex flats, molding flash, or protrusions that prevent 2.3mm gap mounting? Measure with calipers. If too tight, the fallback is 18mm C-C.

2. **Bulkhead fitting availability**: Are John Guest bulkhead/panel-mount fittings available for 1/4" OD tube? Check McMaster, Amazon, or John Guest direct. If not, inline fittings press-fit into printed pockets with adhesive are the fallback.

3. **Push rod routing**: The push rod runs from the front-face cam through the cartridge interior (between or beside the two pumps) to the rear-face plate center. Verify that the pump arrangement leaves a clear path for a rigid rod along the cartridge's centerline.

4. **Lever swing clearance**: The lever swings 180 degrees on the cartridge front face, protruding 50-80mm when open. This must clear the enclosure slot edges and any adjacent front-face elements (displays, hopper door). The enclosure front-face layout must reserve clearance for the lever sweep.

5. **Plate return force**: The collet springs push the plate back when the lever is closed (cam retracts). When undocked, the collet springs are not engaged, but the plate position does not matter when undocked. The cam profile naturally retracts the plate during the closing motion.

---

## Sources

- collet-release.md -- bore dimensions, collet mechanics, failure modes
- release-plate.md -- stepped bore geometry, spacing, guide features, compliance
- cam-lever.md -- eccentric cam mechanics, over-center behavior, prior art survey
- electrical-mating.md -- pogo pin recommendations, moisture separation, contact specs
- guide-alignment.md -- rail clearances, tapered pin geometry, FDM tolerances
- pump-mounting.md -- mounting features, tube exit points, wire routing
- cartridge-envelope.md -- pump dimensions, arrangements, bounding volumes
- release-mechanism-alternatives.md -- full solution space (CPC couplings, hand disconnect, etc.)
- under-cabinet-ergonomics.md -- reach depth, visibility, user posture constraints
- dock-mounting-strategies.md -- mounting location analysis, water filter prior art
- cartridge-change-workflow.md -- step-by-step UX analysis, drip management, timing
- layout-spatial-planning.md -- enclosure dimensions, component inventory, tower layouts
- hopper-and-bag-management.md -- pump-assisted filling, bag dimensions, FDC1004 sensing
- front-face-interaction-design.md -- display holders, cartridge slot aesthetics, lever position
- back-panel-and-routing.md -- bulkhead fittings, internal plumbing diagram, tube management
- [Southco Inject/Eject Mechanisms](https://southco.com/en_us_int/fasteners/inject-eject-mechanisms) -- server blade ejector prior art
- [Bicycle Quick-Release Mechanisms (Sheldon Brown)](https://sheldonbrown.com/skewers.html) -- over-center cam prior art
- [John Guest OD Tube Fittings Technical Specifications](https://www.johnguest.com/sites/default/files/files/tech-spec-od-fittings-v2.pdf)
