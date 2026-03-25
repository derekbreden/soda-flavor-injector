# Pump Cartridge Parts Catalog

Structured parts list for the replaceable pump cartridge module. Every dimension is either a verified measurement, an estimate (marked with ~), or TBD. Coordinate convention: front = pull handle / cam lever face (low-Y), rear = fluid fittings face (high-Y), top = pogo pad face (high-Z).

See `architecture.md` for design rationale and `research/` for detailed trade-off analyses.

---

## 3D Printed Parts

### Part: Outer Shell
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** 148W x 130D x 80H mm (exterior)
- **Features:**
  - Rectangular box, open on top (lid closes it)
  - Wall thickness: ~3-4mm (all sides)
  - Exterior slide rails on bottom face: 2x parallel rails, 2mm tall x 3mm wide, full 130mm depth, spaced to match dock floor rails
  - Exterior side guide contact surfaces: 1.5mm wide rail features on left/right walls, 0.3-0.5mm clearance per side to dock guides
  - Rear wall: 4x fitting pockets for JG union bodies in 2x2 grid, 15mm center-to-center, pocket bore ~13mm diameter (clears ~12.7mm fitting body OD), through-holes for tube stubs
  - Rear wall: release plate travel cavity behind fitting pockets, 6mm deep x ~40mm wide x ~40mm tall
  - Rear wall: 2x guide pin holes for release plate dowel pins (3.2mm diameter, ~10mm deep)
  - Front wall: cam lever pivot hole (~5mm diameter for pivot pin), push rod through-hole (~4-5mm diameter, centered on release plate)
  - Top face: recessed pocket for pogo target PCB, ~15W x 30L x 1.5D mm
  - Top face: wire entry slot from interior to pogo PCB pocket
  - Interior ledges for pump tray: 2x shelves on left/right interior walls, ~2mm wide, positioned to support tray at correct Z height
  - Interior: tray locating tabs or slots on walls for lateral tray alignment
  - Chamfered front edges: 5mm x 45-degree chamfer on all four leading edges (aids dock entry)
- **Interfaces:**
  - Tray screws to shell floor or ledges via 4x M3 heat-set inserts
  - Lid attaches to top via screws or snap clips
  - JG fittings press into rear wall pockets
  - Release plate rides on 2x steel dowel pins mounted in rear wall
  - Cam lever pivots on pin through front wall
  - Pogo target PCB sits in top-face recess
  - Exterior rails and guides mate with dock floor rails and side guides
- **Quantity:** 1
- **Open:** Exact wall thickness (3mm vs 4mm) needs prototyping. Interior ledge Z-height depends on final pump tray thickness. Print orientation strategy (upside-down to get clean top-face recess vs right-side-up for rail quality) TBD.

---

### Part: Pump Tray
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** ~140W x 120D x 6H mm
- **Features:**
  - Flat plate, prints horizontally for maximum screw boss strength
  - 4-8x M3 heat-set insert bosses for pump bracket mounting (boss OD: 8mm, boss height: 6mm, pilot hole: 4.0mm)
  - Exact mounting hole pattern: TBD -- must be measured from physical KPHM400 bracket with calipers. Expected: 2x or 4x M3 per pump, spacing estimated ~55-65mm x ~40-50mm per pump (scaled from KK series)
  - 4x printed C-clips for BPT tubing strain relief (clip ID: 8.3mm for 8.0mm OD BPT tube, clip opening: 6.5mm for snap-in)
  - 4x printed C-clips for 1/4" hard tubing strain relief (clip ID: 6.65mm for 6.35mm OD tube, clip opening: 4.85mm for snap-in)
  - Wire routing channel along one edge: ~5mm wide x 3mm deep U-channel
  - 4x M3 through-holes at corners for mounting tray to shell (clearance holes: 3.4mm)
  - Tray edge locating tabs that key into shell wall slots
- **Interfaces:**
  - Pump brackets bolt to tray via M3 screws into heat-set inserts
  - Tray bolts to shell floor/ledges via 4x M3 screws
  - Locating tabs engage shell wall slots for lateral alignment
- **Quantity:** 1
- **Open:** Exact pump mounting hole pattern (critical -- must measure physical pumps). Number of mounting holes per pump (2 or 4). Whether rubber grommet isolators are needed (try rigid mount first).

---

### Part: Lid
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** ~148W x 130D x ~2-3H mm
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
- **Envelope:** ~40W x 40H x 6D mm (2x2 grid arrangement)
- **Features:**
  - 4x stepped bores in 2x2 grid, 15mm center-to-center:
    - Tube clearance hole: 8.0mm diameter, through full thickness (+0.5/-0 mm tolerance)
    - Inner lip (collet pusher): 10.5mm diameter, 2.0mm depth (+/-0.25mm tolerance)
    - Outer bore (collet cradle): 12.5mm diameter, 2.0mm depth (+/-0.5mm tolerance)
    - 0.2mm chamfer at tube hole entry edge
    - 0.3mm x 45-degree lead-in chamfer at outer bore entry
  - Axial depth stack: 2.0mm outer bore + 2.0mm inner lip + 2.0mm structural back = 6.0mm total
  - Inner lip annular width: (10.5 - 8.0) / 2 = 1.25mm (critical -- minimum viable for FDM)
  - 2x guide pin slots: 3.3mm wide x 7.3mm long (oriented along travel axis), positioned symmetrically outside the bore pattern
  - Push rod contact point: centered boss or flat on the back face for cam push rod engagement
  - Bore arrangement (pump pairing):
    ```
    Pump 1 IN    Pump 2 IN
        O            O
    Pump 1 OUT   Pump 2 OUT
        O            O
    ```
- **Interfaces:**
  - Slides on 2x 3mm steel dowel pins mounted in outer shell rear wall
  - Stroke: 3.0mm (min 2.5mm)
  - Receives axial push from cam lever push rod on back face
  - Stepped bores engage JG collet rings on fitting face side
  - Must maintain <0.3mm parallelism deviation across 33.5mm plate during full stroke
- **Quantity:** 1
- **Open:** Inner lip bore diameter (10.5mm) must be validated against actual JG fittings in hand. Outer bore (12.5mm) must clear actual collet ring OD (~11.4mm, needs physical verification). Print orientation: bore axis should be perpendicular to build plate (Z-axis) for best circularity.

---

### Part: Cam Lever
- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** ~100L x ~20W x ~15H mm (handle length x width x cam body)
- **Features:**
  - Handle: ~80-100mm long, ergonomic grip cross-section (~15W x 10H mm)
  - Eccentric cam lobe at pivot end: 1.5mm eccentricity (produces 3mm stroke over 180-degree rotation)
  - Cam OD: ~10-15mm (estimated)
  - Pivot bore: ~5mm diameter for pivot pin
  - Over-center geometry: cam profile allows lever to pass ~2-3 degrees past maximum displacement for self-locking
  - Optional: small detent bump on cam track for tactile click at locked position
  - Push rod contact surface on cam face (flat or slightly convex)
- **Interfaces:**
  - Pivots on 5mm steel pin through outer shell front wall
  - Cam face pushes against push rod, which transmits force to release plate
  - Handle serves as pull grip for cartridge removal
  - Over-center position locks lever against shell front face
- **Quantity:** 1
- **Open:** Exact cam lobe diameter. Whether an external or internal cam is more practical for 3D printing. Lever handle shape (straight vs curved). Detent geometry for click feel.

---

### Part: Push Rod
- **Type:** 3D printed or steel rod
- **Material:** PETG or 4-5mm steel rod
- **Envelope:** ~4-5mm diameter x ~120mm long (spans front wall to release plate)
- **Features:**
  - Straight rod connecting cam lever to release plate
  - Front end: contact surface for cam lobe (flat or cupped)
  - Rear end: contact surface for release plate back face (flat)
  - Must slide freely through front wall hole with ~0.3mm clearance
- **Interfaces:**
  - Receives force from cam lever cam lobe at front end
  - Transmits axial force to release plate at rear end
  - Passes through outer shell front wall (4-5mm through-hole)
- **Quantity:** 1
- **Open:** Whether a single centered rod is sufficient or if two rods are needed for even force distribution across the 2x2 plate. Steel vs printed (steel is stiffer but requires hardware).

---

## Purchased Parts -- Mechanical

### Part: Kamoer KPHM400-SW3B25 Peristaltic Pump
- **Type:** Purchased
- **Material:** Various (motor housing, pump head, BPT tubing)
- **Envelope:** 68.6W x 115.6D x 62.7H mm (verified, per pump)
- **Features:**
  - 12V DC brushed motor, 10W, ~0.83A typical draw
  - 3-roller pump head
  - BPT 25# pump tube: 4.8mm ID x 8.0mm OD
  - Tube exits from pump head face (front of unit), inlet and outlet on same face
  - Tube stub protrusion from pump head: ~30-50mm (estimated)
  - Motor leads exit from rear of motor housing
  - Mounting bracket (straight plate): metal, 2x or 4x M3 through-holes
  - Bracket mounting hole pattern: TBD (must measure)
  - Weight: ~306g per pump (verified)
  - Noise: <=65 dB (verified)
  - Flow rate: 400 ml/min (verified)
- **Interfaces:**
  - Bracket bolts to pump tray via M3 screws
  - BPT tube stubs connect to brass barb fittings for transition to 1/4" hard tubing
  - Motor leads route through wire channel to pogo target PCB
- **Quantity:** 2
- **Open:** Exact mounting hole pattern (center-to-center spacing). Bracket overall dimensions. Tube exit positions relative to mounting face. Motor lead length. Motor protrusion beyond bracket (determines tray-to-lid clearance).

---

### Part: John Guest 1/4" Push-to-Connect Union
- **Type:** Purchased (e.g., PI0408S or PP0408W)
- **Material:** Acetal copolymer body, stainless steel gripper teeth, nitrile/EPDM O-ring
- **Envelope:** ~12.7mm body OD x ~38-42mm body length (union, accepts tube from both ends)
- **Features:**
  - Accepts 1/4" OD (6.35mm) tubing from each end
  - Insertion depth to tube stop: ~15-18mm per side
  - Collet ring OD: ~11.4mm (measured/inferred)
  - Collet ring protrusion from body face: ~2-3mm
  - Collet travel (inward, for release): ~1.5-2.0mm
  - Release force per fitting: ~3-5N
  - Grip force per fitting: ~20N (4 fittings total ~80N retention)
  - Max working pressure: 150 PSI at 70F (verified)
  - NSF 61 certified for potable water
- **Interfaces:**
  - Press into rear wall pockets of outer shell (13mm bore accommodates 12.7mm body)
  - Dock side: accepts hard tube stubs from cartridge (~30mm protrusion)
  - Cartridge interior side: accepts hard tube stubs from pump tubing transition
  - Collet rings engage release plate stepped bores during disconnect
  - 2x2 grid arrangement, 15mm center-to-center
- **Quantity:** 4
- **Open:** Exact body OD (verify ~12.7mm with calipers). Exact collet ring OD (verify ~11.4mm). Whether union style (tube from both ends) or elbow/stem variant is better for this mounting approach.

---

### Part: 1/4" OD Hard Nylon Tubing (Tube Stubs)
- **Type:** Purchased
- **Material:** Nylon or polyethylene, 1/4" OD (6.35mm)
- **Envelope:** 6.35mm OD, cut to ~45-50mm length per stub (15-18mm insertion depth + ~30mm protrusion)
- **Features:**
  - Rigid enough to push into JG fittings without buckling
  - Food-grade compatible
  - 4 stubs on dock side (protrude from dock back wall into cartridge rear fittings during insertion)
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
  - Worm-drive or spring-clip style
- **Interfaces:**
  - Clamps around BPT tube at barb connection point
- **Quantity:** 4 (one per barb fitting)
- **Open:** Spring clip vs worm drive preference. Exact clamp size.

---

## Purchased Parts -- Electrical

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
- **Envelope:** 3mm diameter x ~15mm long
- **Features:**
  - Smooth cylindrical surface for plate sliding
  - Provides linear guidance for release plate travel
- **Interfaces:**
  - Press-fit or epoxied into 3.0mm holes in outer shell rear wall
  - Release plate slides on pins via 3.3mm wide slots (0.15mm clearance per side)
- **Quantity:** 2
- **Open:** Exact length depends on shell wall depth + plate travel + retention depth. 3mm vs 4mm diameter (3mm is adequate for the 12-20N load).

---

### Part: Pivot Pin (Cam Lever)
- **Type:** Purchased
- **Material:** Steel
- **Envelope:** ~5mm diameter x ~25-30mm long (spans shell front wall + lever pivot bore)
- **Features:**
  - Smooth cylindrical pin
  - Cam lever rotates on this pin
- **Interfaces:**
  - Passes through cam lever pivot bore and outer shell front wall hole
  - Retained by press-fit, E-clip, or cotter pin at ends
- **Quantity:** 1
- **Open:** Retention method. Exact length depends on shell wall thickness + lever width.

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
- **Quantity:** 8-12 (4-8 for pump brackets + 4 for tray-to-shell)
- **Open:** Exact count depends on pump bracket hole count (2 or 4 per pump).

---

### Part: M3 x 5mm Brass Heat-Set Insert
- **Type:** Purchased
- **Material:** Brass, knurled exterior
- **Envelope:** ~4.0mm pilot hole, ~5mm length, M3 internal thread
- **Features:**
  - Installed into PETG with soldering iron at 245C
  - Knurled exterior grips plastic after cooling
  - Pull-out strength: 200-400N per insert in PETG
  - Unlimited reassembly cycles
- **Interfaces:**
  - Press-set into pump tray bosses (4.0mm pilot holes)
  - Press-set into shell interior bosses for tray mounting
- **Quantity:** 8-12 (matching screw count)
- **Open:** 4mm vs 5mm insert length.

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
- **Quantity:** 4-8 (one per pump mounting screw)
- **Open:** Not needed for MVP -- try rigid mount first. Add if vibration/noise is objectionable.

---

## Sub-Assembly: Release Mechanism

Composed of: cam lever + pivot pin + push rod + release plate + 2x dowel pins.

- **Function:** Single lever flip releases all 4 JG collets simultaneously.
- **Actuation:** 180-degree lever rotation produces 3mm axial plate travel via 1.5mm eccentric cam.
- **Force path:** Hand -> lever handle (80-100mm arm) -> eccentric cam (1.5mm offset) -> push rod -> release plate -> 4x collet rings.
- **Mechanical advantage:** ~10:1 or higher (lever arm / cam radius).
- **Total required force at collets:** 12-20N (4 fittings x 3-5N each).
- **Required hand force:** ~1.2-2.0N (comfortable one-handed).
- **Self-locking:** Over-center cam position holds lever in both locked and released states.
- **Parallelism requirement:** Release plate must stay within 0.3mm deviation across its 33.5mm face during full 3mm stroke.
