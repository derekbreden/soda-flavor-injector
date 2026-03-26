# Pump Cartridge Parts Catalog

Structured parts list for the replaceable pump cartridge module. Every dimension is either a verified measurement, a best-available estimate (marked with ~, pending caliper verification), or TBD.

**Coordinate system:** Origin at the exterior front-bottom-left corner of the outer shell. X = width (positive right), Y = depth (positive toward rear/fittings), Z = height (positive up). Front face (Y=0) is the cam lever / pull handle side. Rear face (Y=130) carries the JG fittings. Top face (Z=80) carries the pogo pads.

See `architecture.md` for design rationale and `research/` for detailed trade-off analyses.

---

## 3D Printed Parts

### Part: Outer Shell
- **Type:** 3D printed
- **Material:** PETG
- **Exterior envelope:** 148W x 130D x 80H mm
- **Wall thickness:** 4mm solid (all sides, no ribs)
- **Interior volume:** 140W x 122D x 72H mm
- **Features:**
  - Rectangular box, open on top (lid closes it)
  - Exterior slide rails on bottom face: 2x parallel rails, 2mm tall x 3mm wide, full 130mm depth, spaced to match dock floor rails
  - Exterior side guide contact surfaces: 1.5mm wide rail features on left/right walls, 0.3-0.5mm clearance per side to dock guides
  - Rear wall (Y=126 to Y=130, 4mm thick): 4x fitting pockets for JG PP0408W union bodies in 2x2 grid, 40mm horizontal x 28mm vertical center-to-center. Pocket bore 13.0mm diameter (clears 12.7mm fitting body OD). Through-holes for tube passage. Fitting pocket centers at X=54, X=94, Z=26, Z=54 (centered on rear wall)
  - Rear wall: release plate travel cavity behind fitting pockets, 6mm deep x 59mm wide x 47mm tall (clears 55x43mm plate with 2mm margin per side)
  - Rear wall: 2x guide pin holes for release plate dowel pins (3.0mm diameter press-fit, 10mm deep), positioned at X=42, Z=40 and X=108.5, Z=40 (symmetric, outside the bore pattern, matching plate guide pin slot positions)
  - Front wall (Y=0 to Y=4, 4mm thick): cam lever pivot hole (5mm diameter, at X=74, Z=40) for pivot pin; push rod through-hole (5mm diameter, at X=74, Z=40 — coaxial with pivot, push rod passes below pivot pin)
  - Top face: recessed pocket for pogo target PCB, 15W x 30L x 1.5D mm
  - Top face: wire entry slot from interior to pogo PCB pocket
  - Interior ledges for pump tray: 2x shelves on left/right interior walls, 2mm wide, at Z=4 (supports tray 4mm above shell floor)
  - Interior: tray locating tabs or slots on walls for lateral tray alignment
  - Chamfered front edges: 5mm x 45-degree chamfer on all four leading edges (aids dock entry)
- **Interfaces:**
  - Tray screws to shell ledges via 4x M3 heat-set inserts
  - Lid attaches to top via screws or snap clips
  - JG fittings press into rear wall pockets (cartridge carries the fittings; dock has bare tube stubs)
  - Release plate rides on 2x steel dowel pins mounted in rear wall
  - Cam lever pivots on pin through front wall
  - Pogo target PCB sits in top-face recess
  - Exterior rails and guides mate with dock floor rails and side guides
- **Quantity:** 1
- **Open:** Interior ledge Z-height depends on final pump tray thickness. Print orientation strategy (upside-down for clean top-face recess vs right-side-up for rail quality) TBD.

---

### Part: Pump Tray
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** 138W x 120D x 6H mm (fits inside 140mm interior with 1mm clearance per side)
- **Features:**
  - Flat plate, prints horizontally for maximum screw boss strength
  - 4x M3 heat-set insert bosses for pump bracket mounting (boss OD: 8mm, boss height: 6mm, pilot hole: 4.0mm) — 2 per pump
  - Mounting hole pattern per pump: 49.45mm center-to-center, one axis (caliper-verified), 3.13mm hole diameter (accepts M3 screws with ~0.13mm clearance). Bracket has 2x M3 through-holes (one per ear). If a 2x2 pattern exists, the perpendicular axis spacing is TBD.
  - 4x printed C-clips for BPT tubing strain relief (clip ID: 8.3mm for 8.0mm OD BPT tube, clip opening: 6.5mm for snap-in)
  - 4x printed C-clips for 1/4" hard tubing strain relief (clip ID: 6.65mm for 6.35mm OD tube, clip opening: 4.85mm for snap-in)
  - Wire routing channel along one edge: 5mm wide x 3mm deep U-channel
  - 4x M3 through-holes at corners for mounting tray to shell (clearance holes: 3.4mm)
  - Tray edge locating tabs that key into shell wall slots
- **Interfaces:**
  - Pump brackets bolt to tray via M3 screws into heat-set inserts
  - Tray bolts to shell ledges via 4x M3 screws
  - Locating tabs engage shell wall slots for lateral alignment
- **Quantity:** 1
- **Open:** Perpendicular axis mounting hole spacing (if bracket has 2x2 pattern — currently only one axis measured). Whether rubber grommet isolators are needed (try rigid mount first).

---

### Part: Lid
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** 148W x 130D x 3H mm
- **Features:**
  - Flat plate closing the open top of the outer shell
  - 4-6x M3 screw clearance holes or snap-clip features around perimeter
  - Cutout or recess to clear pogo target PCB pocket on shell top face
- **Interfaces:**
  - Screws or snaps onto outer shell top edges
  - Must not interfere with pogo pin contact on top face
- **Quantity:** 1
- **Open:** Screw vs snap-clip attachment. Exact number of fastener points.

---

### Part: Release Plate
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** 55W x 43H x 6D mm
- **Features:**
  - 4x stepped bores in 2x2 grid, 40mm horizontal x 28mm vertical center-to-center (matches JG fitting spacing):
    - Bore centers at: (7.5, 7.5), (47.5, 7.5), (7.5, 35.5), (47.5, 35.5) relative to plate bottom-left corner
    - Tube clearance hole: 8.0mm diameter, through full thickness (+0.5/-0 mm tolerance)
    - Inner lip (collet pusher): 10.5mm diameter, 2.0mm depth (+/-0.25mm tolerance)
    - Outer bore (collet cradle): 12.5mm diameter, 2.0mm depth (+/-0.5mm tolerance)
    - 0.2mm chamfer at tube hole entry edge
    - 0.3mm x 45-degree lead-in chamfer at outer bore entry
  - Axial depth stack: 2.0mm outer bore + 2.0mm inner lip + 2.0mm structural back = 6.0mm total
  - Inner lip annular width: (10.5 - 8.0) / 2 = 1.25mm (critical — minimum viable for FDM)
  - Edge-to-bore clearance: 7.5mm from plate edge to nearest bore center (1.25mm minimum wall around outer bore)
  - 2x guide pin slots: 3.3mm wide x 7.3mm long (oriented along travel axis), positioned at X=(-5.5, 21.5) and X=(60.5, 21.5) relative to plate bottom-left — symmetric outside the bore pattern
  - Push rod contact point: centered boss on back face at (27.5, 21.5) relative to plate bottom-left, 8mm diameter x 1mm proud
- **Interfaces:**
  - Slides on 2x 3mm steel dowel pins mounted in outer shell rear wall
  - Stroke: 3.0mm (min 2.5mm)
  - Receives axial push from cam lever push rod on back face
  - Stepped bores engage JG collet rings on fitting face side
  - Return spring: 2x small compression springs (one per guide pin, ~5mm OD x 10mm free length, ~0.5 N/mm rate) ride on the dowel pins between the plate and the shell rear wall, returning the plate to the retracted (locked) position when the cam lever is released. Initial design values, subject to iteration after printing.
  - Must maintain <0.3mm parallelism deviation across 55mm plate width during full stroke
- **Quantity:** 1
- **Open:** Inner lip bore diameter (10.5mm) must be validated against actual JG fittings in hand. Outer bore (12.5mm) must clear actual collet ring OD (~12.7mm — may need to increase to 13.0mm after physical verification). Print orientation: bore axis perpendicular to build plate (Z-axis) for best circularity.

---

### Part: Cam Lever
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** 90L x 18W x 14H mm (handle length x width x cam body diameter)
- **Features:**
  - Handle: 76mm long from pivot center, ergonomic grip cross-section (18W x 10H mm, rounded edges)
  - Eccentric cam lobe at pivot end: 1.5mm eccentricity (produces 3mm stroke over 180-degree rotation)
  - Cam OD: 14mm
  - Pivot bore: 5mm diameter for pivot pin
  - Over-center geometry: cam profile allows lever to pass 2 degrees past maximum displacement for self-locking
  - Small detent bump (0.3mm) on cam track for tactile click at locked position
  - Push rod contact surface on cam face: slightly convex (1mm crown over 10mm), hardened by contact area
- **Interfaces:**
  - Pivots on 5mm steel pin through outer shell front wall
  - Cam face pushes against push rod, which transmits force to release plate
  - Handle serves as pull grip for cartridge removal
  - Over-center position locks lever against shell front face
- **Quantity:** 1
- **Note:** Initial design values, subject to iteration after printing.
- **Open:** Lever handle shape (straight vs curved). Whether an external or internal cam is more practical for 3D printing — start with external.

---

### Part: Push Rod
- **Type:** Steel rod
- **Material:** 5mm steel rod (smooth ground)
- **Envelope:** 5mm diameter x 118mm long (spans front wall inner face to release plate back face: 122mm interior depth minus 4mm clearance)
- **Features:**
  - Straight rod connecting cam lever to release plate
  - Front end: flat, receives contact from cam lobe
  - Rear end: flat, contacts release plate push rod boss
  - Slides freely through front wall hole with 0.25mm diametral clearance (5.0mm rod in 5.5mm hole)
- **Interfaces:**
  - Receives force from cam lever cam lobe at front end
  - Transmits axial force to release plate at rear end
  - Passes through outer shell front wall (5.5mm through-hole)
- **Quantity:** 1
- **Note:** Initial design values, subject to iteration after printing. A single centered rod is the starting design; add a second rod only if parallelism testing shows the plate cocking under load.

---

## Purchased Parts — Mechanical

### Part: Kamoer KPHM400-SW3B25 Peristaltic Pump
- **Type:** Purchased
- **Material:** Various (motor housing, pump head, BPT tubing)
- **Envelope:** Pump head 62.6mm square (caliper-verified), bracket ears bring width to 68.6mm. Total length 116.48mm with motor shaft nub (caliper-verified) or 111.43mm without (caliper-verified). Height 62.6mm (caliper-verified). Datasheet states 68.6W x 115.6D x 62.7H mm.
- **Features:**
  - 12V DC brushed motor, 10W, ~0.83A typical draw
  - 3-roller pump head
  - BPT 25# pump tube: 4.8mm ID x 8.0mm OD
  - Tube exits from pump head face (front of unit), inlet and outlet on same face (caliper-verified)
  - Tube stub protrusion from pump head: ~30-50mm (estimated)
  - Motor leads exit from rear of motor housing
  - Motor shaft nub protrudes 5.05mm from motor end cap (caliper-verified)
  - Mounting bracket (straight plate): metal, 2x M3 through-holes (one per ear)
  - Bracket mounting hole diameter: 3.13mm (caliper-verified), accepts M3 screws with ~0.13mm clearance
  - Bracket mounting hole center-to-center: 49.45mm, single axis (caliper-verified). If bracket has a 2x2 pattern, perpendicular axis spacing is TBD.
  - Weight: 306g per pump (confirmed)
  - Noise: <=65 dB (confirmed)
  - Flow rate: 400 ml/min (confirmed)
- **Interfaces:**
  - Bracket bolts to pump tray via M3 screws
  - BPT tube stubs connect to brass barb fittings for transition to 1/4" hard tubing
  - Motor leads route through wire channel to pogo target PCB
- **Quantity:** 2
- **Status:** Envelope, mounting hole pattern (one axis), and motor nub protrusion caliper-verified. Tube exit X/Z positions, motor lead length, and motor body diameter (~35mm, low confidence) still TBD.
- **Open:** Tube exit positions relative to mounting face. Motor lead length. Motor body diameter (caliper readings ~34.5-35.1mm, low confidence). Perpendicular axis mounting hole spacing (if 2x2 pattern exists).

---

### Part: John Guest PP0408W 1/4" Push-to-Connect Union
- **Type:** Purchased
- **Material:** Acetal copolymer body, stainless steel gripper teeth, nitrile/EPDM O-ring
- **Envelope:** 12.7mm body OD x 38.1mm overall length (1/2" x 1-1/2")
- **Features:**
  - Accepts 1/4" OD (6.35mm) tubing from each end
  - Tube insertion depth to tube stop: ~16mm per side (industry convention, unverified)
  - Collet ring OD: ~12.7mm (approximately same as body OD)
  - Collet ring protrusion from body face: ~2-3mm
  - Collet travel (inward, for release): TBD (needs physical measurement)
  - Release force per fitting: ~3-5N
  - Grip force per fitting: ~20N (4 fittings total ~80N retention)
  - Max working pressure: 150 PSI at 70F (confirmed)
  - NSF 61 certified for potable water
  - Weight: ~45g per fitting
- **Interfaces:**
  - Press into rear wall pockets of outer shell (13mm bore accommodates 12.7mm body)
  - Cartridge carries all 4 fittings in its rear wall (union style — accepts tubes from both ends)
  - Dock side: bare 1/4" OD tube stubs protrude from dock back wall (~30mm). Cartridge slides onto these stubs during insertion.
  - Cartridge interior side: accepts hard tube stubs from pump tubing transition
  - Collet rings engage release plate stepped bores during disconnect
  - 2x2 grid arrangement, 40mm horizontal x 28mm vertical center-to-center, centered on rear wall
- **Quantity:** 4
- **Status:** Best available dimensions, pending caliper verification of body OD, collet ring OD, and collet travel.

---

### Part: 1/4" OD Hard Nylon Tubing (Tube Stubs)
- **Type:** Purchased
- **Material:** Nylon or polyethylene, 1/4" OD (6.35mm)
- **Envelope:** 6.35mm OD, cut to ~46mm length per stub (~16mm insertion depth + ~30mm protrusion)
- **Features:**
  - Rigid enough to push into JG fittings without buckling
  - Food-grade compatible
  - 4 stubs on dock side (protrude from dock back wall; cartridge slides onto these during insertion)
  - 4 stubs inside cartridge (connect BPT-to-hard-tube transition to interior side of JG fittings)
- **Interfaces:**
  - Inserts into JG push-to-connect fittings (gripped by collet)
  - Interior stubs connect to brass barb fittings via the barb-to-tube transition
- **Quantity:** 8 (4 dock-side stubs + 4 cartridge-interior stubs)
- **Open:** Whether cartridge-interior stubs are separate pieces or if the hard tube runs continuously from barb fitting to JG fitting interior.

---

### Part: Brass Barb Fitting (BPT-to-Hard-Tube Transition)
- **Type:** Purchased
- **Material:** Brass (food-grade)
- **Envelope:** ~25-30mm long x ~10mm max OD (estimated, depends on specific fitting)
- **Features:**
  - One end: barb sized for BPT pump tube (4.8mm ID, barb OD ~5-6mm)
  - Other end: barb or press-fit for 1/4" OD hard tubing (tube ID ~4mm, barb OD ~4mm)
  - Provides rigid transition between soft pump tubing and hard tube stubs
- **Interfaces:**
  - BPT pump tube pushes over barb end, secured with small hose clamp
  - Hard tube pushes over or into opposite barb end
  - Positioned between pump head tube exit and JG fitting, held by strain relief clips on tray
- **Quantity:** 4 (one per tube run: 2 pumps x 2 lines each)
- **Open:** Exact barb sizing depends on BPT tube ID (4.8mm) and hard tube ID. May need 3/16" barb x 1/4" barb or similar combination. Source from Amazon or McMaster.

---

### Part: Small Hose Clamp
- **Type:** Purchased
- **Material:** Stainless steel
- **Envelope:** Fits ~8mm OD tubing (BPT tube OD)
- **Features:**
  - Secures BPT pump tube onto brass barb fitting
  - Spring-clip style preferred (easier in tight space)
- **Interfaces:**
  - Clamps around BPT tube at barb connection point
- **Quantity:** 4 (one per barb fitting)
- **Open:** Exact clamp size.

---

### Part: Compression Spring (Release Plate Return)
- **Type:** Purchased
- **Material:** Stainless steel
- **Envelope:** ~5mm OD x 10mm free length, 3.2mm ID (rides on 3mm dowel pin)
- **Features:**
  - Spring rate: ~0.5 N/mm
  - Compressed length at plate retracted (locked) position: ~7mm
  - Compressed length at plate extended (released) position: ~4mm
  - Preload at retracted position: ~1.5N per spring (3N total)
  - Max load at extended position: ~3N per spring (6N total)
  - Returns release plate to retracted position when cam lever is released
- **Interfaces:**
  - Rides on 3mm dowel pin between shell rear wall inner face and release plate back face
  - One spring per guide pin
- **Quantity:** 2
- **Note:** Initial design values, subject to iteration after printing.

---

## Purchased Parts — Electrical

### Part: Pogo Target PCB
- **Type:** Purchased (custom PCB from JLCPCB/PCBWay) or fabricated from brass strip
- **Material:** FR4 PCB with nickel-plated copper pads, or nickel-plated brass plates
- **Envelope:** ~15W x 30L x 1.6H mm
- **Features:**
  - 3x contact pads: 8mm x 5mm each, 10mm center-to-center spacing
  - Pad layout: GND | Motor A 12V | Motor B 12V
  - Pad surface: bare copper with nickel or gold plating (no solder mask on contact area)
  - Solder pads on underside for wire connections
  - Mounting holes or friction-fit tabs for recess mounting
- **Interfaces:**
  - Sits in recessed pocket on cartridge top face (shell top)
  - Pad face flush with or slightly recessed (0.5mm) from cartridge top surface
  - Wires route from PCB underside through shell wall slot to pump motor leads
  - Pogo pins on dock ceiling press against pads during insertion
  - Pads elongated in insertion direction for natural wipe action
- **Quantity:** 1
- **Open:** PCB vs brass strip fabrication. Gold plating for better corrosion resistance vs nickel for cost. Exact pad dimensions may adjust based on pogo pin tip size.

---

### Part: Steel Dowel Pin (Release Plate Guide)
- **Type:** Purchased
- **Material:** Steel
- **Envelope:** 3mm diameter x 20mm long
- **Features:**
  - Smooth cylindrical surface for plate sliding
  - Provides linear guidance for release plate travel
  - Also serves as guide shaft for return compression springs
- **Interfaces:**
  - Press-fit or epoxied into 3.0mm holes in outer shell rear wall (10mm engagement depth)
  - 10mm of pin protrudes into cavity for plate travel + spring
  - Release plate slides on pins via 3.3mm wide slots (0.15mm clearance per side)
  - Compression spring rides on each pin between wall and plate
- **Quantity:** 2

---

### Part: Pivot Pin (Cam Lever)
- **Type:** Purchased
- **Material:** Steel
- **Envelope:** 5mm diameter x 26mm long (4mm shell wall + 18mm lever width + 4mm retention)
- **Features:**
  - Smooth cylindrical pin
  - Cam lever rotates on this pin
- **Interfaces:**
  - Passes through cam lever pivot bore and outer shell front wall hole
  - Retained by E-clip on exterior end
- **Quantity:** 1

---

### Part: M3 x 8mm Socket Head Cap Screw
- **Type:** Purchased
- **Material:** Stainless steel
- **Envelope:** M3 thread, 8mm length
- **Features:**
  - Secures pump brackets to tray (through bracket into heat-set insert)
  - Secures tray to shell (through tray into shell inserts)
- **Interfaces:**
  - Threads into M3 heat-set inserts in pump tray and shell
- **Quantity:** 8 (4 for pump brackets — 2 per pump + 4 for tray-to-shell)

---

### Part: M3 x 5mm Brass Heat-Set Insert
- **Type:** Purchased
- **Material:** Brass, knurled exterior
- **Envelope:** ~4.0mm pilot hole, 5mm length, M3 internal thread
- **Features:**
  - Installed into PETG with soldering iron at 245C
  - Knurled exterior grips plastic after cooling
  - Pull-out strength: 200-400N per insert in PETG
  - Unlimited reassembly cycles
- **Interfaces:**
  - Press-set into pump tray bosses (4.0mm pilot holes)
  - Press-set into shell interior bosses for tray mounting
- **Quantity:** 8 (matching screw count)

---

### Part: E-Clip (Pivot Pin Retention)
- **Type:** Purchased
- **Material:** Spring steel
- **Envelope:** 5mm shaft size
- **Features:**
  - Snaps into groove on pivot pin to prevent axial withdrawal
- **Interfaces:**
  - Clips onto pivot pin on exterior side of shell front wall
- **Quantity:** 1

---

### Part: Rubber Grommet Isolator (Optional)
- **Type:** Purchased
- **Material:** Neoprene rubber
- **Envelope:** ~6-8mm OD, ~3mm ID (for M3 screw pass-through), ~2mm thick
- **Features:**
  - Sits between pump bracket and tray mounting surface
  - Reduces vibration transmission by 60-80% above 30 Hz
  - Adds ~2mm to mounting stack height
- **Interfaces:**
  - M3 screw passes through grommet center hole
  - Grommet sits in counterbore on tray or between bracket and tray surface
- **Quantity:** 4 (one per pump mounting screw — 2 per pump)
- **Open:** Not needed for MVP — try rigid mount first. Add if vibration/noise is objectionable.

---

## Sub-Assembly: Release Mechanism

Composed of: cam lever + pivot pin + E-clip + push rod + release plate + 2x dowel pins + 2x compression springs.

- **Function:** Single lever flip releases all 4 JG collets simultaneously. Springs return the plate when the lever is released.
- **Actuation:** 180-degree lever rotation produces 3mm axial plate travel via 1.5mm eccentric cam.
- **Force path:** Hand -> lever handle (76mm arm) -> eccentric cam (1.5mm offset) -> push rod (118mm, 5mm steel) -> release plate (55x43x6mm) -> 4x collet rings.
- **Return path:** 2x compression springs on dowel pins push plate back to retracted position, which pushes push rod back, which rotates cam lever back past over-center detent.
- **Mechanical advantage:** ~10:1 (76mm lever arm / 7mm cam effective radius).
- **Total required force at collets:** 12-20N (4 fittings x 3-5N each).
- **Spring return force (retracted position):** ~3N total (must be overcome by hand when flipping lever to release).
- **Required hand force:** ~2-3N (comfortable one-handed, including spring preload).
- **Self-locking:** Over-center cam position (2 degrees past peak) holds lever in both locked and released states.
- **Parallelism requirement:** Release plate must stay within 0.3mm deviation across its 55mm width during full 3mm stroke.
