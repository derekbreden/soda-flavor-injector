# Enclosure Parts Catalog

Structured parts list for the under-sink flavor injection enclosure. Every dimension and interface is stated explicitly for downstream CAD geometry generation. Coordinate system: origin at interior front-bottom-left corner, X = width (positive rightward), Y = depth (positive rearward), Z = height (positive upward). Exterior dimensions: 220W x 300D x 400H mm.

---

## 1. Enclosure Shell

### Part: Enclosure Main Body

- **Type:** 3D printed (multi-piece assembly) or injection molded
- **Material:** ABS or ASA
- **Envelope:** 220W x 300D x 400H mm (exterior)
- **Features:**
  - Wall thickness: 3-4mm (TBD -- 4mm used for worst-case interior calculations, 3mm with ribbing is likely final)
  - Interior volume at 4mm walls: 212W x 292D x 392H mm
  - Interior volume at 3mm walls: 214W x 294D x 394H mm
  - Front panel opening for cartridge slot: 148W x 84H mm, centered in width (X=32-180 at 4mm walls), bottom edge at Z=0
  - Front panel opening for hopper access: 100mm diameter circle, centered at X=106, Z=361 (estimated)
  - Front panel display dock recesses: two 50mm diameter circular pockets, ~5mm deep, at X=55 Z=275 and X=157 Z=275 (estimated, from display-and-front-panel.md zones)
  - Front panel cable exit holes: two ~8mm holes centered in each display dock recess for flat cat6 cable
  - Back panel cutouts: see Back Panel section
  - Internal ribs: TBD pattern if 3mm walls are chosen (typical 2mm wide x 6mm tall ribs at 40-50mm spacing)
- **Interfaces:**
  - Back panel: integral or removable (fastened with M3 screws into heat-set inserts)
  - Top panel: removable or hinged for hopper access
  - Bottom: flat base, may include printed floor rails for cartridge dock
  - Internal mounting bosses for electronics shelf, cradle brackets, valve rack, reel housings
- **Quantity:** 1 (may be printed as 2-4 sub-pieces and bonded)
- **Open:**
  - Wall thickness: 3mm vs 4mm (affects all interior dims by 2mm per axis)
  - Print orientation and split line locations
  - Rib pattern for 3mm walls

### Part: Top Panel

- **Type:** 3D printed
- **Material:** ABS or ASA (match main body)
- **Envelope:** 220W x 300D x ~10H mm
- **Features:**
  - Central hole: 100mm diameter for hopper funnel access, centered at X=110, Y=~40 (front-biased to clear bags)
  - Flip-up lid or removable cap over the hopper hole
  - Hinge at rear edge if flip-up (piano hinge or living hinge)
  - Seal: silicone gasket or lip around hopper opening
- **Interfaces:**
  - Mates to main body top edge via snap-fits, magnets, or M3 screws
  - Hopper funnel body seats into the opening from below
- **Quantity:** 1
- **Open:**
  - Hinge mechanism type
  - Seal design around hopper opening

### Part: Front Panel

- **Type:** 3D printed
- **Material:** ABS or ASA (match main body)
- **Envelope:** 220W x 4D x 400H mm
- **Features:**
  - Cartridge slot opening: 148W x 84H mm, centered in width, bottom at Z=0
  - Chamfered slot entrance: 5mm chamfer on all edges of cartridge opening for blind insertion
  - Two display dock recesses: 50mm diameter x 5mm deep, centered at approximately X=55 Z=275 and X=157 Z=275
  - Cable exit holes in dock recesses: ~8mm diameter for flat cat6
  - Hopper access opening at top: 100mm diameter, centered at X=110, Z=~361
  - Status LED window: TBD (small, near cartridge slot)
- **Interfaces:**
  - Attaches to main body front edge
  - Display dock recesses accept magnetic display pucks (magnets in recess surround)
  - Cartridge slot edges align with internal floor rails
- **Quantity:** 1
- **Open:**
  - Whether front panel is integral to main body or separate/removable
  - Exact display dock positions (estimated from 280mm-width research, need recalculation for 220mm)

---

## 2. Bag Cradle System

### Part: Diagonal Bag Cradle

- **Type:** 3D printed
- **Material:** PETG (food-safe, contacts bag exterior only)
- **Envelope:** ~200W x ~350L (along diagonal) x ~50H mm (profiled depth varies)
- **Features:**
  - Profiled channel matching lens-shaped cross-section of two stacked 2L Platypus bags
  - Channel depth at center (deepest point): ~40mm
  - Channel depth at ends: tapering to ~5mm
  - Channel width: 190mm (bag width) + 5mm margin per side = 200mm
  - 2-3mm lip on each side to prevent lateral bag sliding
  - Mounting tabs/brackets at ends and midpoint for attachment to enclosure walls
  - Diagonal angle: 35 degrees from horizontal
  - Bag length supported: 350mm
- **Interfaces:**
  - Mounts to enclosure interior walls via brackets or snap-fit tabs
  - 6mm clearance per side between cradle edge and enclosure wall (at 212mm interior)
  - Cradle lower end (cap/connector end) positioned at approximately Y=25, Z=125
  - Cradle upper end (sealed end) positioned at approximately Y=292, Z=392
- **Quantity:** 1 (may print as 2-3 segments joined along the diagonal)
- **Open:**
  - Exact channel profile -- needs physical measurement of filled 2L bag cross-section
  - Mounting bracket design
  - Print segmentation for build plate fit (350mm diagonal exceeds most print beds)

### Part: Back-Wall Bag Pin / Clamp

- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** ~100W x ~20D x ~30H mm (estimated)
- **Features:**
  - Holds the flat sealed end of each bag pinned against the back wall at the highest point
  - Accommodates two bags stacked (total ~2mm thick at sealed end)
  - Clamp, channel, adhesive pad, or bracket (mechanism TBD)
- **Interfaces:**
  - Mounts to interior back wall surface near Z=380-392, Y=288-292
  - Bag sealed end sits between clamp and wall
- **Quantity:** 1-2 (one per bag or one shared)
- **Open:**
  - Pin/clamp mechanism not yet designed
  - Exact mounting position depends on bag placement during prototyping

---

## 3. Cartridge Dock

The dock is the enclosure-side structure that receives the removable pump cartridge. Interface dimensions must match the cartridge (see cartridge architecture.md -- envelope 148W x 130D x 80H mm).

### Part: Dock Floor Rails

- **Type:** 3D printed (integral to enclosure base or separate)
- **Material:** PETG (low friction)
- **Envelope:** 2x rails, each ~3W x 130D x 2H mm
- **Features:**
  - Two parallel rails on the enclosure floor
  - Rail width: 3mm
  - Rail height: 2mm
  - Rail length: full cartridge travel depth, ~130mm
  - Spacing: matched to cartridge base groove pattern -- inside edge to inside edge = cartridge width minus shell wall margins (~142mm center-to-center, TBD)
  - Surface: smooth printed top face for low-friction sliding
- **Interfaces:**
  - Cartridge base has mating grooves (3mm wide x 2.5mm deep) that ride on these rails
  - Rail clearance: 0.3-0.5mm per side (FDM tolerance)
  - Rail start: chamfered or ramped for guided entry
- **Quantity:** 2 (one pair)
- **Open:**
  - Exact rail spacing -- depends on cartridge base groove positions

### Part: Dock Side Guides

- **Type:** 3D printed (integral to enclosure walls or separate)
- **Material:** PETG
- **Envelope:** 2x guides, each ~1.5W x 130D x 84H mm
- **Features:**
  - 1.5mm wide rails on enclosure side walls within the cartridge slot zone
  - Run full depth of cartridge travel (130mm)
  - Prevent lateral wobble during insertion
  - Clearance: 0.3-0.5mm per side to cartridge shell
- **Interfaces:**
  - Inner faces contact cartridge shell sides
  - Lower edge at Z=0, upper edge at Z=84
- **Quantity:** 2
- **Open:**
  - Exact lateral positions depend on cartridge envelope + clearance validation

### Part: Dock Back Wall (Fitting Mount Wall)

- **Type:** 3D printed
- **Material:** PETG
- **Envelope:** ~148W x ~35D x ~84H mm
- **Features:**
  - Structural wall at Y=130-165 (behind fully-inserted cartridge)
  - Four 15.9mm (5/8") through-holes for John Guest PP1208W 1/4" push-to-connect bulkhead fittings
  - Hole pattern: 2 inlet fittings (lower) + 2 outlet fittings (upper), arranged in a rectangle
  - Hole center-to-center spacing: ~40mm horizontal (estimated), ~20mm vertical (estimated)
  - Wall thickness at fitting holes: 6-8mm (to accommodate JG bulkhead nut + body)
  - Pogo pin mount area on ceiling face (top surface at Z=80-84): see Pogo Pin Mount
  - Drainage channel molded into ceiling surface sloping away from pogo pin pockets
- **Interfaces:**
  - JG fittings face rearward (valve rack side) and forward (cartridge tube stubs insert from front)
  - Cartridge tube stubs: 4x 1/4" OD hard nylon, ~30mm protrusion, insert into JG fittings on dock-in
  - Pogo pins mount to ceiling face, press down onto cartridge top pads
  - Release plate bore alignment: four 8.0/10.5/12.5mm stepped bores in release plate (separate part, cartridge side) must align with JG collet rings
- **Quantity:** 1
- **Open:**
  - Exact JG fitting hole pattern -- needs physical JG fitting measurement for center-to-center
  - Release plate stepped bore geometry needs validation on first print

### Part: Pogo Pin Mount

- **Type:** 3D printed bracket or small PCB mount
- **Material:** PETG bracket + FR4 PCB (if using PCB-mounted pogo pins)
- **Envelope:** ~60W x ~30D x ~10H mm (estimated)
- **Features:**
  - Mounts to dock ceiling (underside of dock back wall top face or separate bracket at Z=80-84)
  - Holds 3-6 spring-loaded pogo pins (P75 or P100 series)
  - Pin diameter: 2-3mm
  - Pin stroke: 1-2mm
  - Pin center-to-center spacing: 10mm
  - Minimum 3 pins: GND, Motor A 12V, Motor B 12V
  - Optional 3 more: cartridge ID, temp sensor, spare
  - Conformal coating on PCB traces; contact surfaces bare metal
  - Drainage channel in surrounding dock ceiling slopes away from pin pockets
- **Interfaces:**
  - Pins press onto cartridge top face pads (8mm x 5mm each, nickel-plated brass, 10mm c-t-c)
  - Guide rails position cartridge within ~0.5mm lateral tolerance
  - Oversized pads (8mm target for 2mm pin tip) tolerate 2-3mm misalignment
  - Wipe action: pin tip drags across elongated pad during slide-in
- **Quantity:** 1
- **Open:**
  - PCB vs printed bracket decision
  - Exact pin count (3 minimum, 6 maximum)

### Part: Chamfered Slot Entrance

- **Type:** 3D printed (integral to front panel / dock entrance)
- **Material:** ABS or ASA (match enclosure)
- **Envelope:** ~158W x ~20D x ~94H mm
- **Features:**
  - 5mm chamfer on all edges of cartridge slot opening
  - Funnel narrows from ~158W x ~94H (outer lip) to ~148W x ~84H (rail clearance) over first 15-20mm of depth
  - Accepts cartridge with 10-15mm of initial misalignment
- **Interfaces:**
  - Transitions to floor rail and side guide surfaces
- **Quantity:** 1 (integral to front panel)
- **Open:** None -- geometry is well-defined

---

## 4. Valve Rack

Ten solenoid valves mounted behind the cartridge dock, using the depth dimension. Two rows of 5 valves stacked vertically.

### Part: Valve Rack Frame

- **Type:** 3D printed
- **Material:** PETG or ABS
- **Envelope:** ~185W x ~75D x ~75H mm
- **Features:**
  - Holds 10 solenoid valves in a 5-wide x 2-high grid
  - Valve orientation: cylindrical axis along Y (depth), quick-connect ports facing front and rear
  - Row 1 (bottom): Z=0-35, 5 valves across width
  - Row 2 (top): Z=40-75, 5 valves across width
  - Each valve cradle: 35mm diameter semicircular saddle with retention clip or strap
  - Valve pitch (center-to-center in X): ~37mm (35mm valve body + 2mm gap)
  - 5 valves at 37mm pitch = 185mm total span, centered in 212mm interior (13.5mm margin per side)
  - Depth consumed: Y=165-240 (75mm, one valve body length with QC fittings)
  - Cable routing channels for solenoid wires (12V + GND per valve, 20 wires total)
- **Interfaces:**
  - Position: directly behind dock back wall (Y=165-240, X=13.5-198.5, Z=0-75)
  - Valve QC fittings face forward (toward dock, short tube runs ~35mm to dock fittings) and rearward (toward bags/back panel)
  - Mounts to enclosure floor and/or walls via M3 screws into heat-set inserts
  - Clearance to bags above: at Y=240, bag cradle underside is at Z~356 -- gap of ~281mm
- **Quantity:** 1
- **Open:**
  - Exact valve body dimensions -- need caliper measurement of purchased Beduan valves (spec: ~30-35mm dia x 50-55mm body, ~75mm with QC fittings)
  - Valve retention method (clip, strap, zip-tie channel, or friction saddle)

---

## 5. Hopper

### Part: Hopper Funnel Body

- **Type:** 3D printed
- **Material:** PETG (structural, does not contact food -- silicone insert does)
- **Envelope:** ~100mm diameter x ~70H mm (cone/funnel shape)
- **Features:**
  - Top opening: ~100mm diameter
  - Bottom outlet: ~10mm diameter (connects to 1/4" barb fitting)
  - Funnel capacity: ~200-300ml
  - Interior surface: smooth for silicone insert seating
  - Mounting flange at top rim for attachment to enclosure top panel
  - Drain fitting boss at bottom center: 1/4" barb or push-connect
- **Interfaces:**
  - Seats into 100mm hole in top panel from below
  - Flange rests on panel underside, secured with screws or snap-fit
  - Position: top-front of enclosure, centered at approximately X=106, Y=~40, Z=322-392
  - Bottom outlet connects to hopper feed tube, routed to valve v5/v7
  - Silicone insert drops in from above
- **Quantity:** 1
- **Open:**
  - Exact funnel profile (straight cone vs curved)
  - Mounting flange geometry

### Part: Hopper Silicone Insert

- **Type:** Purchased or custom-molded
- **Material:** Platinum-cured food-grade silicone (FDA compliant)
- **Envelope:** ~98mm top diameter x ~65H mm (fits inside funnel body)
- **Features:**
  - Flexible funnel shape matching the PETG funnel body interior
  - Removable for dishwasher cleaning
  - Bottom opening: ~8mm diameter, mates to funnel body outlet
  - Lip at top rim to prevent insert from falling through
- **Interfaces:**
  - Friction-fit into PETG funnel body
  - User removes for cleaning
- **Quantity:** 1 (spare recommended)
- **Open:**
  - Source: custom mold vs off-the-shelf silicone funnel trimmed to fit
  - Wall thickness: ~2mm (estimated)

---

## 6. Two-Port Cap and Dip Tube

One set per bag, two sets total.

### Part: Custom Two-Port Cap

- **Type:** 3D printed or machined
- **Material:** PETG, nylon, or machined Delrin (food-safe, threaded)
- **Envelope:** 28mm thread diameter x ~25H mm (estimated)
- **Features:**
  - 28mm thread to match Platypus bag opening (standard 28mm PCO-1881 or equivalent)
  - Two through-holes for fittings:
    - P1 (main fluid port): 15.9mm (5/8") hole for John Guest PP1208W bulkhead fitting
    - P2 (dip tube port): ~8mm hole for 1/4" barb fitting (smaller footprint than JG bulkhead)
  - Usable cap face diameter: ~24mm (inside the thread ring)
  - P1 and P2 holes arranged to fit within 24mm usable diameter
  - O-ring groove or gasket face for thread seal
- **Interfaces:**
  - Threads onto Platypus bag 28mm opening
  - P1 JG fitting: exterior connects to bag-to-pump tubing (b->i direct line)
  - P2 barb fitting: exterior connects to dip tube air bleed line (to v9/v10)
  - Interior side: P1 has short stub or flush opening; P2 has dip tube hard tube insertion
- **Quantity:** 2 (one per bag)
- **Open:**
  - Exact thread spec for Platypus bag opening (28mm -- measure pitch from physical bag)
  - JG + barb hole spacing within 24mm usable diameter -- needs CAD validation
  - Material selection for food contact + thread durability

### Part: Dip Tube (Hard Tube)

- **Type:** Purchased
- **Material:** 1/4" OD (6.35mm) hard polyethylene or polyurethane tube
- **Envelope:** 6.35mm OD x ~350mm length (runs full bag length)
- **Features:**
  - Runs from P2 barb fitting in cap up through bag interior to sealed end (highest point)
  - Tube inserts into tip piece central socket at upper end
  - Tube inserts into barb fitting at lower end (cap)
  - Slight flex to follow bag interior curvature on the 35-degree diagonal
- **Interfaces:**
  - Lower end: press-fit into P2 barb fitting in custom cap
  - Upper end: inserts into tip piece central tube socket (friction or barb grip)
  - Held in tension between cap and tip piece when assembled
- **Quantity:** 2 (one per bag)
- **Open:**
  - Exact tube length -- depends on bag internal dimension from cap to sealed end (~340-350mm)

### Part: Dip Tube Tip Piece (Air Collection Bar)

- **Type:** 3D printed
- **Material:** PETG (FDM), nylon (SLS), or resin (SLA) -- food-safe, only fluid-contact 3D-printed part
- **Envelope:** 185L x 22W x 14H mm
- **Features:**
  - Rectangular bar spanning full bag width
  - Cross-section: 22mm x 14mm (diagonal = 26.1mm, fits through 28mm cap opening with 1.9mm clearance)
  - Ship-in-a-bottle assembly: inserts lengthwise through 28mm cap, rotates 90 degrees inside bag
  - Central tube socket at midpoint (92.5mm from each end):
    - FDM: friction-fit bore, 6.25mm diameter, 18mm deep, 1mm x 45-degree entry chamfer
    - SLS/SLA: barb-ridge bore, 6.35mm nominal with 6.15mm ridge (0.20mm interference), 1mm wide ridge, 15mm deep socket
  - Air channel ribs on top and bottom faces:
    - Rib geometry: triangular or trapezoidal, ~2mm tall, ~3mm pitch
    - Longitudinal groove connecting ribs to central socket bore
    - Ribs prevent bag film from sealing flat against bar surface
  - Open-ended socket (tube does not bottom out) -- void above tube end connects to surface air channels
  - Wedges between bag heat-sealed side seams (185mm > 182-184mm bag internal clear width)
- **Interfaces:**
  - Grips 6.35mm OD hard dip tube in central socket
  - Wedge-fit between bag side seams prevents lateral movement
  - Air channels collect migrating air and route to central bore / dip tube
- **Quantity:** 2 (one per bag)
- **Open:**
  - Material: FDM PETG vs SLS nylon vs SLA resin for food safety (this is the only food-contact printed part)
  - Exact rib pattern and count

---

## 7. Display System

### Part: Display Reel Housing

- **Type:** 3D printed
- **Material:** ABS or PETG
- **Envelope:** ~55mm diameter x ~22mm deep (per reel, using flat cat6 cable)
- **Features:**
  - Spool hub: 24mm diameter (12mm radius)
  - Winding width (axial): 14mm
  - Full spool outer diameter: ~54mm (with 1m of flat cat6 wound)
  - Spring housing: integrated, adds ~8mm to spool depth (total ~22mm)
  - Constant-force (clock) spring or spiral torsion spring, 0.3-0.5N retraction force
  - Pull-to-lock, pull-to-release mechanism (adds ~5mm to depth)
  - Cable exit with ball-and-socket strain relief at panel side
  - Cable exit hole: ~8mm (for flat cat6, ~7mm wide x 3mm thick)
- **Interfaces:**
  - Mounts behind front panel, cable exits through panel hole
  - Two reels side-by-side: 110mm total width (fits within 212mm interior with ~51mm per side)
  - Reel depth: Y=0-22mm (behind front panel)
  - Approximate positions: reel 1 centered at X=55, reel 2 centered at X=157 (at 212mm interior)
  - Cable connects to RJ45 jack internally and to display module externally
- **Quantity:** 2
- **Open:**
  - Spring type and specification (constant-force vs torsion)
  - Lock mechanism detailed design
  - Whether reels are side-by-side (110mm wide, 22mm deep) or vertically stacked (55mm wide, 44mm deep)

### Part: Display Puck Shell

- **Type:** 3D printed or injection molded
- **Material:** ABS or polycarbonate
- **Envelope:** 50mm diameter x 12-15mm thick
- **Features:**
  - Front: protective lens (polycarbonate or glass), 1.5mm thick, with silicone gasket seal
  - Display cavity: 37mm diameter (for 1.28" GC9A01 round TFT module, 37mm PCB diameter x 4mm thick)
  - Controller PCB cavity: behind display, ~3mm deep (RP2040 or ESP32-S3 + passives)
  - Base: flat with rubber grip ring, fold-out kickstand (30-45 degree tilt, adds ~2mm when folded)
  - 2-3 neodymium disc magnets: 6mm diameter x 2mm thick each, embedded in base (1-2 kg total pull force)
  - RJ45 jack recess: recessed or rear-facing, with rubber flap for splash protection
  - IPX2 (drip-proof) minimum rating
  - Weight target: 25-40g
- **Interfaces:**
  - RJ45 jack connects to flat cat6 cable from reel
  - Magnets interface with steel disc or magnets in front panel dock recess
  - Kickstand deploys for countertop use
  - Magnets hold to fridge door or steel surfaces
- **Quantity:** 2 (one config display S3, one flavor display RP2040)
- **Open:**
  - Display size: 1.28" (37mm PCB) vs 1.69" (43mm PCB) -- housing should accommodate either with different bezel insert
  - Exact PCB stackup height (display + controller)

---

## 8. Back Panel

### Part: Back Panel

- **Type:** 3D printed (integral to enclosure or separate/removable)
- **Material:** ABS or ASA (match enclosure)
- **Envelope:** 220W x 4D x 400H mm (exterior face)
- **Features:**
  - Two-row fitting arrangement for 212mm width:
  - **Upper zone (Z=340-380):**
    - IEC C14 panel-mount inlet: rectangular cutout 27.4W x 19.8H mm, two M3 mounting holes at 40mm horizontal c-t-c pitch
    - Position: X=~165, Z=~370 (upper-right, above electronics)
    - 25mm interior clearance for terminal tabs
  - **Lower zone (Z=30-70):**
    - Tap water inlet: 15.9mm (5/8") hole for JG PP1208W bulkhead, at X=~30, Z=~50
    - Soda water inlet: 15.9mm hole for JG PP1208W bulkhead, at X=~80, Z=~50
    - Soda water outlet: 15.9mm hole for JG PP1208W bulkhead, at X=~130, Z=~50
    - All three with exterior 90-degree elbows (JG PP0308W)
    - 30mm interior clearance per fitting
  - **Mid zone (Z=180-220):**
    - Flavor line 1 exit: 12.5mm hole for PG7 cable gland, at X=~50, Z=~200
    - Flavor line 2 exit: 12.5mm hole for PG7 cable gland, at X=~170, Z=~200
    - 15mm interior clearance per gland
  - Color-coding rings and embossed labels at each fitting position
- **Interfaces:**
  - IEC C14 interior tabs connect to PSU (shortest high-voltage run)
  - JG water fittings connect to internal tubing
  - PG7 glands clamp around continuous 1/4" OD silicone flavor tubes
  - Flow meter mounts on interior behind soda water fittings
- **Quantity:** 1
- **Open:**
  - Exact fitting positions need recalculation from 280mm-width research to 220mm width (positions above are estimated proportional scaling)
  - Whether back panel is integral or removable

### Part: Flow Meter Mount

- **Type:** 3D printed bracket/clip
- **Material:** PETG or ABS
- **Envelope:** ~70W x ~40D x ~45H mm (bracket around flow meter)
- **Features:**
  - Holds DIGITEN 1/4" quick-connect flow sensor (63.5L x 30.5W x 38.1H mm)
  - Clip or saddle with two M3 screw holes into heat-set inserts on enclosure floor
  - Sensor oriented with 1/4" QC ports facing left and right (toward inlet and outlet bulkheads)
- **Interfaces:**
  - Position: approximately X=80-144, Y=262-292 (against back wall), Z=0-40
  - Signal cable (3-wire: VCC, GND, signal) routes up back wall interior to electronics zone
  - 1/4" hard tube: ~80mm from soda inlet bulkhead to flow meter inlet; ~80mm from flow meter outlet to soda outlet bulkhead
- **Quantity:** 1
- **Open:**
  - Exact position adjusted for 220mm width back panel

---

## 9. Electronics Zone

All electronics mount in the top-rear corner, above and behind the diagonal bags, approximately X=11-201, Y=200-292, Z=275-392.

### Part: Electronics Mounting Shelf

- **Type:** 3D printed
- **Material:** PETG or ABS
- **Envelope:** ~190W x ~92D x ~4H mm (flat plate)
- **Features:**
  - Flat plate with standoff bosses (M3 heat-set inserts) for mounting PCBs
  - Mounting positions for:
    - ESP32 dev board: ~50 x 25mm footprint
    - L298N motor driver(s): ~43 x 43mm footprint each (2 needed for 2 pumps)
    - MCP23017 breakout: ~25 x 25mm footprint
    - DS3231 RTC module: ~25 x 25mm footprint
    - PSU board: ~80 x 50mm footprint (estimated, for 36W 12V + 5V output)
  - Wire routing channels along edges
  - Ventilation holes or slots for PSU heat dissipation
- **Interfaces:**
  - Mounts to enclosure walls via brackets or tabs at Z=~275-280
  - Position: behind upper bag surface, above Y=200
  - PSU positioned adjacent to IEC C14 inlet (shortest AC wire run)
  - Available height: ~117mm (Z=275-392) but varies by depth due to diagonal bag surface above
- **Quantity:** 1
- **Open:**
  - Exact component layout on shelf
  - Whether single shelf or multiple small brackets
  - PSU dimensions depend on specific unit selected

---

## 10. Purchased Parts

### Part: Solenoid Valve (2-Way NC)

- **Type:** Purchased
- **Material:** Food-grade plastic and elastomer (RO-rated)
- **Envelope:** ~30-35mm diameter x 50-55mm body length; ~75-80mm with 1/4" QC fittings on both ends
- **Features:**
  - 2-way normally-closed, 12V DC solenoid
  - 1/4" quick-connect fittings (push-fit, standard RO tubing compatible)
  - Power draw: 4.8-5.5W per valve (energized only during active modes)
  - Working pressure: 0-0.8 MPa
  - Working temperature: 0-70C
  - Weight: ~100g each
- **Interfaces:**
  - Mount in valve rack frame (35mm diameter saddle)
  - QC fittings accept 1/4" OD (6.35mm) PE or silicone tubing
  - Solenoid wire leads connect to MCP23017-gated MOSFET driver circuit
- **Quantity:** 10
- **Open:**
  - Final vendor selection: Beduan B07NWCQJK9 (~$9 each) or DIGITEN B016MP1HX0 (~$7-8 each)
  - Exact body dimensions need caliper measurement of purchased units
  - Zero-pressure variant (DIGITEN B076KFCPGM) may be needed for pump-inlet valves (suction side)

### Part: John Guest PP1208W Bulkhead Union (1/4" Push-to-Connect)

- **Type:** Purchased
- **Material:** Acetal (POM), NSF 61 certified
- **Envelope:** ~25mm diameter body x ~40mm length
- **Features:**
  - 1/4" push-to-connect on both sides of bulkhead
  - Requires 15.9mm (5/8") mounting hole
  - Hex nut clamps to panel
  - Collet release: press ring to disconnect tube
  - ~20N grip force per fitting
- **Interfaces:**
  - Dock back wall: 4x (2 inlet + 2 outlet for cartridge fluid connections)
  - Back panel: 3x (tap water inlet, soda inlet, soda outlet)
  - Custom two-port cap: 1x per cap (P1 main fluid port)
- **Quantity:** 9 total (4 dock + 3 back panel + 2 caps)
- **Open:** None -- well-specified

### Part: John Guest PP0308W 90-Degree Elbow (1/4" Push-to-Connect)

- **Type:** Purchased
- **Material:** Acetal (POM)
- **Envelope:** ~20mm x 20mm x 15mm
- **Features:**
  - 90-degree elbow for back panel exterior fittings
  - Directs incoming tubes downward parallel to back panel, preventing kinking against cabinet wall
- **Interfaces:**
  - Connects to exterior side of back panel JG bulkhead fittings
- **Quantity:** 3 (one per back panel water fitting)
- **Open:** None

### Part: IEC C14 Panel-Mount Inlet (with Fuse)

- **Type:** Purchased
- **Material:** Thermoplastic, brass contacts
- **Envelope:** Panel cutout: 27.4W x 19.8H mm; body depth behind panel: ~25mm
- **Features:**
  - IEC 60320 C14, 10A 250VAC rated
  - Screw-mount: two M3 screws at 40mm horizontal c-t-c
  - Integrated fuse holder (3A slow-blow recommended)
  - Quick-connect or solder tabs on interior
- **Interfaces:**
  - Mounts in back panel upper zone (X=~165, Z=~370)
  - Interior tabs wire to PSU input
  - Accepts standard IEC C13 power cord
- **Quantity:** 1
- **Open:** None

### Part: PG7 Nylon Cable Gland

- **Type:** Purchased
- **Material:** Nylon, IP68 rated
- **Envelope:** Thread OD: 12.5mm; body length: ~20mm
- **Features:**
  - Panel hole: 12.5mm
  - Clamping range: 3-6.5mm (fits 6.35mm OD silicone tube)
  - Compression seal for strain relief and environmental sealing
  - Locknut on exterior
- **Interfaces:**
  - Threads into back panel mid-zone holes (flavor line exits)
  - Clamps around continuous 1/4" OD silicone flavor tube
- **Quantity:** 2 (one per flavor line)
- **Open:** None

### Part: DIGITEN 1/4" Quick-Connect Hall-Effect Flow Sensor

- **Type:** Purchased
- **Material:** Food-grade POM (polyoxymethylene) body
- **Envelope:** 63.5L x 30.5W x 38.1H mm
- **Features:**
  - 1/4" push-to-connect fittings on both ends (inlet and outlet)
  - Flow range: 0.3-10 L/min
  - Pulse output: F = 36 x Q (L/min)
  - Working voltage: DC 3-24V (compatible with ESP32 3.3V GPIO)
  - Max current draw: 1.5mA at 5V
  - 3-wire signal cable: VCC (red), GND (black), signal (yellow), ~15cm factory length
  - Accuracy: +/- 2%
- **Interfaces:**
  - Mounts inline between soda water inlet and outlet bulkheads
  - Signal cable routes to ESP32 GPIO 23
  - Factory 15cm cable is too short -- extend to ~300mm with solder splice or JST connector
- **Quantity:** 1
- **Open:** None

### Part: Kamoer KPHM400-SW3B25 Peristaltic Pump

- **Type:** Purchased (inside cartridge, not in enclosure -- listed for interface reference)
- **Material:** Various (motor, PPS pump head, BPT pump tube)
- **Envelope:** 68.6W x 115.6D x 62.7H mm (single pump)
- **Features:**
  - 12V DC brushed motor
  - 400ml/min flow rate
  - BPT pump tube: 4.8mm ID x 8.0mm OD
  - Weight: 306g each
  - Mounting bracket: 2-4x M3 holes (exact pattern TBD, measure from physical pump)
- **Interfaces:**
  - Two pumps side-by-side in cartridge: ~137.2mm combined width
  - Motor power via pogo pin interface: GND + 12V per motor
  - Pump tube transitions to 1/4" OD hard tubing via brass barb fittings inside cartridge
  - Current: ~0.85A typical, ~3A stall transient per motor
- **Quantity:** 2 (in cartridge)
- **Open:**
  - Exact mounting hole pattern (must measure with calipers)

### Part: Pogo Pin (Spring-Loaded Contact)

- **Type:** Purchased
- **Material:** Brass/gold-plated tip, stainless steel spring
- **Envelope:** 2-3mm diameter x ~16mm length (compressed + extended range)
- **Features:**
  - P75 or P100 series
  - Spring stroke: 1-2mm
  - Current rating: adequate for 3A transient
  - Solder-tail for PCB mounting
- **Interfaces:**
  - Mount in dock ceiling PCB or bracket
  - Press onto cartridge top face contact pads
  - 10mm center-to-center spacing
- **Quantity:** 3-6
- **Open:**
  - Exact series and model based on current and stroke requirements

### Part: MCP23017 I2C GPIO Expander

- **Type:** Purchased (breakout board or DIP IC)
- **Material:** Standard IC
- **Envelope:** ~25 x 25mm (breakout) or 37.6 x 6.2mm (DIP-28 package)
- **Features:**
  - 16 GPIO pins total (GPA0-7, GPB0-7)
  - I2C interface (SDA, SCL, 2 wires + power)
  - 10 valve outputs: GPB0-GPB7 (8 valves) + GPA0-GPA1 (2 valves)
  - 6 remaining GPIOs available for future use
- **Interfaces:**
  - I2C bus to ESP32 (SDA=GPIO 21, SCL=GPIO 22)
  - GPIO outputs drive MOSFET gate circuits (one per valve)
  - Mounts on electronics shelf
- **Quantity:** 1
- **Open:** None

### Part: L298N Motor Driver Board

- **Type:** Purchased
- **Material:** Standard PCB
- **Envelope:** ~43W x 43D x 27H mm (with heatsink)
- **Features:**
  - Dual H-bridge, 12V input
  - PWM speed control for peristaltic pumps
  - Bidirectional motor control (forward/reverse for fill vs dispense)
  - 2A per channel continuous, 3A peak
- **Interfaces:**
  - 12V power input from PSU
  - Control inputs from ESP32 GPIOs
  - Motor outputs to pogo pin interface (through to cartridge pumps)
  - ENB pins currently also used for valve driving
  - Mounts on electronics shelf
- **Quantity:** 2 (one per pump, or possibly combined)
- **Open:**
  - Whether to use L298N or smaller DRV8871 boards (~25 x 20mm)

### Part: Neodymium Disc Magnet (Display Puck)

- **Type:** Purchased
- **Material:** NdFeB (neodymium iron boron), nickel plated
- **Envelope:** 6mm diameter x 2mm thick
- **Features:**
  - Pull force: ~0.3-0.5 kg each
  - 2-3 per display puck = 0.6-1.5 kg total (adequate for <40g puck)
- **Interfaces:**
  - Embedded in display puck base
  - Corresponding magnets or steel discs in front panel dock recesses
- **Quantity:** 4-6 (2-3 per puck x 2 pucks)
- **Open:** None

### Part: Constant-Force Spring (Retractable Reel)

- **Type:** Purchased
- **Material:** Pre-stressed stainless steel strip
- **Envelope:** ~12mm hub diameter, ~10mm wide strip
- **Features:**
  - Retraction force: 0.3-0.5N (enough to retract cable, not enough to drag display off countertop)
  - Rated for thousands of extend/retract cycles
- **Interfaces:**
  - Mounts inside reel housing hub
  - Drives cable retraction via spool rotation
- **Quantity:** 2 (one per reel)
- **Open:**
  - Exact spring spec depends on final spool geometry and cable weight

### Part: Flat Cat6 Cable (UTP)

- **Type:** Purchased or custom-terminated
- **Material:** Copper conductors, PVC or TPE jacket
- **Envelope:** ~7mm wide x 3mm thick x 1000mm long
- **Features:**
  - 8 conductors in 4 twisted pairs
  - Pinout: pins 1-2 (UART TX/RX), pin 3 (RESET), pins 4-5 (VCC +5V doubled), pin 6 (backlight PWM), pins 7-8 (GND doubled)
  - Flat profile for compact spool winding
  - Spool compatible: 3mm thickness allows ~55mm spool OD for 1m length
  - UTP (unshielded) -- shielding not needed for UART at 115200 baud over 1m
- **Interfaces:**
  - Display end: RJ45 plug into display puck RJ45 jack
  - Enclosure end: soldered to reel hub slip ring or breakout PCB
  - Minimum bend radius: 4 x OD = 12mm (spool hub 12mm radius meets this)
- **Quantity:** 2 (one per display)
- **Open:**
  - Source for pre-terminated flat cat6 at 1m length, or terminate in-house

### Part: RJ45 Jack (Panel Mount, Vertical)

- **Type:** Purchased
- **Material:** Thermoplastic housing, gold-plated contacts
- **Envelope:** ~16W x 16D x 13H mm (standard vertical RJ45)
- **Features:**
  - 8-pin, cat6 rated
  - PCB-mount or solder-cup
- **Interfaces:**
  - Display puck: recessed in base, cable enters from rear/bottom
  - Enclosure side: may use direct solder instead of jack for permanent connection
- **Quantity:** 2 (one per display puck)
- **Open:** None

### Part: 1/4" Barb Fitting (for Two-Port Cap P2)

- **Type:** Purchased
- **Material:** Brass or food-grade nylon
- **Envelope:** ~8mm diameter x ~15mm length
- **Features:**
  - 1/4" (6.35mm) barb on one end for hard tube press-fit
  - Thread or smooth shank for mounting in custom cap P2 hole
  - Smaller footprint than JG bulkhead -- enables two fittings in 28mm cap
- **Interfaces:**
  - Mounts in custom two-port cap P2 hole (~8mm)
  - Barb grips 1/4" OD hard dip tube
  - Exterior side connects to silicone tubing routed to dip tube valve (v9/v10)
- **Quantity:** 2 (one per cap)
- **Open:**
  - Exact fitting type (barb-to-bulkhead vs barb-to-barb)

### Part: Heat-Set Threaded Insert (M3)

- **Type:** Purchased
- **Material:** Brass, knurled
- **Envelope:** ~4.5mm OD x 5mm length (standard M3 short)
- **Features:**
  - Press-in with soldering iron at 220-250C
  - M3 internal thread
  - Knurled exterior for grip in PETG/ABS
- **Interfaces:**
  - Used throughout: enclosure mounting bosses, electronics shelf, valve rack, flow meter bracket, cradle brackets
  - Boss diameter: 7-8mm minimum around insert
- **Quantity:** ~40-60 (estimated, across all assemblies)
- **Open:** None

### Part: M3 Socket Head Cap Screw

- **Type:** Purchased
- **Material:** Stainless steel or zinc-plated steel
- **Envelope:** M3 x 6-12mm length (various)
- **Features:**
  - Hex socket head for Allen key drive
  - Various lengths as needed per assembly
- **Interfaces:**
  - Thread into M3 heat-set inserts throughout
- **Quantity:** ~40-60 (matched to heat-set inserts)
- **Open:** None

### Part: PSU (Power Supply Unit)

- **Type:** Purchased
- **Material:** Standard enclosed or open-frame switching PSU
- **Envelope:** ~80W x 50D x 25H mm (estimated for 36W unit)
- **Features:**
  - Input: 120V AC (from IEC C14 inlet)
  - Output: 12V DC (pumps via L298N, valves via MOSFET) + 5V/3.3V (logic, display tethers)
  - Power budget: 2x pumps at 5W each + 2 valves max at 5W each + logic ~3W = ~23W typical, ~36W peak
  - Open-frame or enclosed with terminal block
- **Interfaces:**
  - AC input from IEC C14 (shortest wire run)
  - 12V output to L298N motor drivers and MOSFET valve drivers
  - 5V output to ESP32, ESP32-S3 (via cat6 tether), RP2040 (via cat6 tether)
  - Mounts on electronics shelf adjacent to IEC inlet
- **Quantity:** 1
- **Open:**
  - Exact PSU model and dimensions
  - Whether 12V + 5V from one unit or separate regulators

### Part: 1/4" OD Tubing (Hard, PE/Nylon)

- **Type:** Purchased
- **Material:** Polyethylene or nylon, food-grade
- **Envelope:** 6.35mm OD (1/4"), various lengths
- **Features:**
  - Standard RO system tubing
  - Compatible with all JG push-to-connect fittings and solenoid valve QC fittings
  - Semi-rigid for push-connect insertion
- **Interfaces:**
  - All internal plumbing connections between valves, fittings, and bag connectors
  - Internal tube runs: estimated 20-30 segments totaling ~3-4m
- **Quantity:** ~4m total (bulk roll)
- **Open:** None

### Part: 1/4" OD Silicone Tubing (Flavor Lines)

- **Type:** Purchased
- **Material:** Platinum-cured food-grade silicone (FDA 21 CFR 177.2600)
- **Envelope:** 6.35mm OD (1/4"), 3.18mm ID (1/8"), 1.59mm wall
- **Features:**
  - Continuous run from pump/valve area through back panel to faucet
  - External run: ~600-900mm per line (under-sink to countertop)
  - Internal run: ~200mm per line (valve to back panel)
  - Dead volume: ~7ml per line (at 1/8" ID, 900mm external)
  - Flexible, transparent or translucent
- **Interfaces:**
  - Internal end: push-fit or barb connection to dispense valve (v1/v2) outlet
  - Back panel pass-through: PG7 cable gland compression seal
  - External end: routes to faucet dispenser nozzle
- **Quantity:** 2 lines, ~1.1m each (total ~2.2m)
- **Open:** None

### Part: Tube Routing Channels (Floor)

- **Type:** 3D printed (integral to enclosure floor or snap-in)
- **Material:** PETG or ABS
- **Envelope:** Varies -- U-profile channels, 10mm wide x 8mm deep
- **Features:**
  - Printed U-channels on enclosure floor for organizing internal tube runs
  - Prevent kinking and tangling
  - Route from valve rack forward to bag connector zone and rearward to back panel
  - Approximate coverage: Y=130-292 (behind cartridge to back panel), X=varies
- **Interfaces:**
  - 1/4" OD tubing sits in channels
  - Clips or friction-fit hold tubes in place
- **Quantity:** Multiple segments (estimated 4-8 channel runs)
- **Open:**
  - Exact routing paths depend on final valve and fitting positions

---

## 11. Fasteners and Hardware Summary

| Item | Spec | Quantity | Used For |
|------|------|----------|----------|
| M3 heat-set insert | Brass, knurled, 4.5mm OD x 5mm L | ~50 | All screw bosses throughout assembly |
| M3 x 6mm SHCS | Stainless steel | ~20 | Electronics shelf, short bracket mounts |
| M3 x 8mm SHCS | Stainless steel | ~15 | Valve rack, panel mounts |
| M3 x 12mm SHCS | Stainless steel | ~10 | Deep bosses, through-panel mounts |
| Neodymium magnet | 6mm dia x 2mm thick | 6 | Display puck magnets (3 per puck) |
| Pogo pin | P75/P100, 2-3mm dia | 3-6 | Dock ceiling electrical interface |
| Constant-force spring | 0.3-0.5N, ~12mm hub | 2 | Display reel retraction |
