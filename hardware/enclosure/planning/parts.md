# Enclosure Parts Catalog

Structured parts list for the under-sink flavor injection enclosure. Every dimension and interface is stated explicitly for downstream CAD geometry generation.

**Coordinate system:** Origin at exterior front-bottom-left corner. X = width (positive rightward, 0-220mm). Y = depth (positive rearward, 0-300mm). Z = height (positive upward, 0-400mm). Exterior dimensions: 220W x 300D x 400H mm. Wall thickness: 4mm solid (no ribs). Interior dimensions: 212W x 292D x 392H mm.

All dimensions are initial design values, subject to iteration during prototyping unless marked "verified."

---

## 1. Enclosure Shell

### Part: Enclosure Main Body

- **Type:** 3D printed (multi-piece assembly) or injection molded
- **Material:** ABS or ASA
- **Envelope:** 220W x 300D x 400H mm (exterior)
- **Features:**
  - Wall thickness: 4mm solid, no ribs
  - Interior: 212W x 292D x 392H mm
  - Interior volume: ~24.3 liters
  - Front panel opening for cartridge slot: 148W x 84H mm, centered in width (X=36-184), bottom edge at Z=0
  - Front panel display dock recesses: two 50mm diameter circular pockets, ~5mm deep, at X=55 Z=275 and X=157 Z=275
  - Front panel cable exit holes: two ~8mm holes centered in each display dock recess for flat cat6 cable
  - Back panel cutouts: see Back Panel section
- **Interfaces:**
  - Back panel: integral or removable (fastened with M3 screws into heat-set inserts)
  - Top panel: removable or hinged for hopper access
  - Bottom: flat base with printed floor rails for cartridge dock
  - Internal mounting bosses for electronics shelf, cradle brackets, valve rack, reel housings
- **Quantity:** 1 (may be printed as 2-4 sub-pieces and bonded)
- **Open:**
  - Print orientation and split line locations

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
  - Two display dock recesses: 50mm diameter x 5mm deep, centered at X=55 Z=275 and X=157 Z=275
  - Cable exit holes in dock recesses: ~8mm diameter for flat cat6
  - Status LED window: small, near cartridge slot (position TBD)
- **Interfaces:**
  - Attaches to main body front edge
  - Display dock recesses accept magnetic display pucks (magnets in recess surround)
  - Cartridge slot edges align with internal floor rails
- **Quantity:** 1
- **Open:**
  - Whether front panel is integral to main body or separate/removable

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
- **Envelope:** ~100W x ~20D x ~30H mm
- **Features:**
  - Holds the flat sealed end of each bag pinned against the back wall at the highest point
  - Accommodates two bags stacked (total ~2mm thick at sealed end)
  - Spring-loaded clamp with M3 screw adjustment for clamping pressure. Initial design value, subject to iteration.
- **Interfaces:**
  - Mounts to interior back wall surface near Z=380-392, Y=288-292
  - Bag sealed end sits between clamp and wall
- **Quantity:** 1 (shared for both bags)
- **Open:**
  - Exact mounting position depends on bag placement during prototyping

---

## 3. Cartridge Dock

The dock is the enclosure-side structure that receives the removable pump cartridge. Interface dimensions must match the cartridge (see cartridge planning -- envelope 148W x 130D x 80H mm).

### Part: Dock Floor Rails

- **Type:** 3D printed (integral to enclosure base or separate)
- **Material:** PETG (low friction)
- **Envelope:** 2x rails, each ~3W x 130D x 2H mm
- **Features:**
  - Two parallel rails on the enclosure floor
  - Rail width: 3mm
  - Rail height: 2mm
  - Rail length: full cartridge travel depth, ~130mm
  - Spacing: matched to cartridge base groove pattern -- inside edge to inside edge ~142mm center-to-center
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
- **Envelope:** ~148W x ~10D x ~84H mm
- **Features:**
  - Structural wall at Y=130-140 (behind fully-inserted cartridge)
  - Four 7mm through-holes for 1/4" OD (6.35mm) tube stub pass-throughs with 0.65mm clearance
  - Tube stub spacing: ~40mm horizontal x ~28mm vertical center-to-center (matching cartridge JG fitting positions)
  - Hole pattern: 2 inlet tubes (lower) + 2 outlet tubes (upper), arranged in a rectangle
  - Each hole sealed with a rubber grommet (7mm ID, ~11mm OD, compression fit into countersunk pocket). Alternative: O-ring groove or adhesive seal. Initial design: grommet.
  - Wall thickness at tube holes: 6-8mm (structural, accommodates grommet counterbore)
  - Pogo pin mount area on ceiling face (top surface at Z=80-84): see Pogo Pin Mount
  - Drainage channel molded into ceiling surface sloping away from pogo pin pockets
- **Interfaces:**
  - Tube stubs protrude from the dock rear face into the valve rack zone, and from the dock front face into the cartridge cavity
  - Cartridge JG fittings slide onto the front-facing tube stubs during dock-in (JG fittings are in the cartridge, not the dock)
  - Pogo pins mount to ceiling face, press down onto cartridge top pads
- **Quantity:** 1
- **Open:**
  - Grommet vs O-ring vs adhesive seal -- prototype all three

### Part: Pogo Pin Mount

- **Type:** 3D printed bracket or small PCB mount
- **Material:** PETG bracket + FR4 PCB (if using PCB-mounted pogo pins)
- **Envelope:** ~60W x ~30D x ~10H mm
- **Features:**
  - Mounts to dock ceiling (underside of dock back wall top face or separate bracket at Z=80-84)
  - Holds 3-6 spring-loaded pogo pins (P75 or P100 series)
  - Pin diameter: 2-3mm
  - Pin stroke: 1-2mm
  - Pin center-to-center spacing: 10mm
  - Minimum 3 pins: GND, Motor A 12V, Motor B 12V
  - Optional 3 more: cartridge ID, temp sensor, spare
  - Conformal coating on PCB traces; contact surfaces bare metal
  - X-position: centered at X=106 (centered on cartridge width within 212mm interior)
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
- **Envelope:** ~181W x ~55D x ~116H mm
- **Features:**
  - Holds 10 solenoid valves in a 5-wide x 2-high grid
  - Valve orientation: long axis (port-to-port, 50.84mm) along Y (depth), coil housing at top (caliper-verified)
  - Valve is T-shaped: white body (32.71W x 50.84D x ~19.4H mm) with metal solenoid coil (31.41W mm) rising vertically from the center, total height 56.00mm (caliper-verified)
  - Valve pitch (center-to-center in X): 37mm (32.71mm body + 4.29mm gap between bodies)
  - 5 valves at 37mm pitch: span = 4 x 37 + 32.71 = 180.71mm ≈ 181mm, centered in 212mm interior (15.5mm margin per side)
  - Row 1 (bottom): Z=0 to Z=56, 5 valves across width
  - Row 2 (top): Z=60 to Z=116, 5 valves across width (4mm gap between rows)
  - Note: rack height is 116mm, taller than cartridge slot (84mm) -- verify no interference with dock back wall / pogo mount
  - Each valve cradle: T-shaped profile -- centered saddle supports the horizontal white valve body from below and on the sides, with clearance above for the centered vertical solenoid coil. Snap-over retention clips on white body (2 per valve, spaced ~30mm apart along valve length). Must not interfere with tube ports (Y axis) or spade connectors (top, extending parallel to Y). Initial design value, subject to iteration.
  - Depth consumed: Y=140 to Y=195 (55mm, port-to-port valve length plus clearance) (caliper-verified)
  - Cable routing channels for solenoid wires (12V + GND per valve, 20 wires total) along rack sides
- **Interfaces:**
  - Position: directly behind dock back wall (Y=140-195, X=16-197, Z=0-116)
  - Valve QC fittings face forward (toward dock, short tube runs to dock back wall tube stubs) and rearward (toward bags/back panel)
  - Mounts to enclosure floor and/or walls via M3 screws into heat-set inserts
  - Clearance to bags above: generous (bag cradle underside at this Y is well above Z=116)
- **Quantity:** 1
- **Open:**
  - Rack height (116mm) means valve rack extends above the cartridge slot (84mm) -- verify no interference with dock back wall / pogo mount

---

## 5. Hopper

### Part: Hopper Funnel Body

- **Type:** 3D printed
- **Material:** PETG (structural, does not contact food -- silicone insert does)
- **Envelope:** ~100mm top opening diameter x ~70H mm
- **Features:**
  - Top opening: ~100mm diameter
  - Bottom outlet: ~10mm diameter (connects to 1/4" barb fitting)
  - Funnel capacity: ~200-300ml
  - Curved/asymmetric funnel profile: shallow at front (near Y=0), deeper toward back, pushed as far forward as possible. The funnel profile follows the void above the lens-shaped bag profile, which is thinnest at the sealed end (top-front). Not a straight cone.
  - Interior surface: smooth for silicone insert seating
  - Mounting flange at top rim for attachment to enclosure top panel
  - Drain fitting boss at bottom center: 1/4" barb or push-connect
- **Interfaces:**
  - Seats into 100mm hole in top panel from below
  - Flange rests on panel underside, secured with M3 screws into heat-set inserts (3 points, 120 degrees apart). Initial design value, subject to iteration.
  - Position: top-front of enclosure, centered at approximately X=106, Y=~40, Z=322-392
  - Bottom outlet connects to hopper feed tube, routed to valve v5/v7
  - Silicone insert drops in from above
- **Quantity:** 1
- **Open:**
  - Exact funnel curvature -- depends on physical bag profile at 35 degrees

### Part: Hopper Silicone Insert

- **Type:** Purchased or custom-molded
- **Material:** Platinum-cured food-grade silicone (FDA compliant)
- **Envelope:** ~98mm top diameter x ~65H mm (fits inside funnel body)
- **Features:**
  - Flexible funnel shape matching the PETG funnel body interior (curved/asymmetric)
  - Removable for dishwasher cleaning
  - Bottom opening: ~8mm diameter, mates to funnel body outlet
  - Lip at top rim to prevent insert from falling through
- **Interfaces:**
  - Friction-fit into PETG funnel body
  - User removes for cleaning
- **Quantity:** 1 (spare recommended)
- **Open:**
  - Source: custom mold vs off-the-shelf silicone funnel trimmed to fit
  - Wall thickness: ~2mm

---

## 6. Two-Port Cap and Dip Tube

One set per bag, two sets total.

### Part: Custom Two-Port Cap

- **Type:** 3D printed or machined
- **Material:** PETG, nylon, or machined Delrin (food-safe, threaded)
- **Envelope:** 28mm thread diameter x ~25H mm
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

Low priority -- may be scrapped if air management works adequately without it.

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
- **Envelope:** ~70mm diameter x ~22mm deep (per reel)
- **Features:**
  - Spool hub: 24mm diameter (12mm radius)
  - Winding width (axial): 10mm (cable width 7mm + 3mm margin)
  - Full spool outer diameter: 70mm (calculated: 1m of flat cat6, 3mm effective thickness per layer, hub radius 12mm. Each layer wraps at increasing radius: 12, 15, 18, 21, 24, 27, 30mm. Cumulative circumference through 7 full layers = 924mm. Remaining ~76mm wraps partway through layer 8 at r=33mm. OD = 2 x 34.2mm, rounded up to 70mm with housing wall.)
  - Spring housing: integrated, adds ~8mm to spool depth (total ~22mm)
  - Constant-force (clock) spring or spiral torsion spring, 0.3-0.5N retraction force
  - Pull-to-lock, pull-to-release mechanism (adds ~5mm to depth)
  - Cable exit with ball-and-socket strain relief at panel side
  - Cable exit hole: ~8mm (for flat cat6, ~7mm wide x 3mm thick)
- **Interfaces:**
  - Mounts behind front panel, cable exits through panel hole
  - Two reels side-by-side: 140mm total width (70mm each), fits within 212mm interior with 36mm per side
  - Reel depth: Y=0-22mm (behind front panel)
  - Approximate positions: reel 1 centered at X=71, reel 2 centered at X=141 (at 212mm interior)
  - Cable connects to RJ45 jack internally and to display module externally
- **Quantity:** 2
- **Open:**
  - Spring type and specification (constant-force vs torsion)
  - Lock mechanism detailed design

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
    - Position: X=165, Z=370 (upper-right, above electronics)
    - 25mm interior clearance for terminal tabs
  - **Lower zone (Z=30-70):**
    - Tap water inlet: 15.9mm (5/8") hole for JG PP1208W bulkhead, at X=30, Z=50
    - Soda water inlet: 15.9mm hole for JG PP1208W bulkhead, at X=80, Z=50
    - Soda water outlet: 15.9mm hole for JG PP1208W bulkhead, at X=130, Z=50
    - All three with exterior 90-degree elbows (JG PP0308W)
    - 30mm interior clearance per fitting
  - **Mid zone (Z=180-220):**
    - Flavor line 1 exit: 12.5mm hole for PG7 cable gland, at X=50, Z=200
    - Flavor line 2 exit: 12.5mm hole for PG7 cable gland, at X=170, Z=200
    - 15mm interior clearance per gland
  - Color-coding rings and embossed labels at each fitting position
- **Interfaces:**
  - IEC C14 interior tabs connect to PSU (shortest high-voltage run)
  - JG water fittings connect to internal tubing
  - PG7 glands clamp around continuous 1/4" OD silicone flavor tubes
  - Flow meter mounts on interior behind soda water fittings
- **Quantity:** 1
- **Open:**
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
- **Open:** None

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
- **Source:** Beduan B07NWCQJK9 (~$9 each, ~$90 for 10)
- **Material:** Food-grade plastic and elastomer (RO-rated)
- **Envelope:** T-shaped assembly, not a simple rectangular block (caliper-verified):
  - White plastic valve body (fluid section): 32.71W x 50.84D (port-to-port) x ~19.4H mm (caliper-verified)
  - Metal solenoid coil housing: 31.41W mm, centered on top of valve body (caliper-verified)
  - Spade connectors protrude from top of metal housing parallel to tube flow axis (Y); metal body to spade tips: 36.63mm (caliper-verified)
  - Total bounding box: 32.71W x 50.84D x 56.00H mm (caliper-verified)
  - With 1/4" QC fittings on both ends: approximately 32.71W x 68D x 56.00H mm
- **Weight:** 113g each
- **Features:**
  - 2-way normally-closed, 12V DC solenoid
  - 1/4" quick-connect fittings (push-fit, standard RO tubing compatible)
  - Power draw: 4.8-5.5W per valve (energized only during active modes)
  - Working pressure: 0-0.8 MPa (label reads "DC12V 0.02-0.8MPa")
  - Working temperature: 0-70C
  - No built-in mounting features (no tabs, flanges, ears, or screw holes) -- needs designed cradle/clamp in valve rack
  - Metal coil housing is slightly offset from white body in X (not perfectly centered)
- **Interfaces:**
  - Mount in valve rack frame (contoured saddle supporting white body + slot for solenoid coil protrusion + snap-over retention clips)
  - QC fittings accept 1/4" OD (6.35mm) PE or silicone tubing
  - Solenoid wire leads (spade connectors at top) connect to MCP23017-gated MOSFET driver circuit
- **Quantity:** 10
- **Open:**
  - Zero-pressure variant (DIGITEN B076KFCPGM) may be needed for pump-inlet valves (suction side)
  - Tube port stub OD and protrusion length not yet measured (needed for tube routing clearance)
  - Spade connector spacing not yet measured (needed for wiring harness design)

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
  - Back panel: 3x (tap water inlet, soda inlet, soda outlet)
  - Custom two-port cap: 1x per cap (P1 main fluid port)
- **Quantity:** 5 total (3 back panel + 2 caps)
- **Open:** None -- well-specified

### Part: John Guest PP0408W 1/4" Union (Cartridge Fluid Connection)

- **Type:** Purchased
- **Material:** Acetal copolymer body, stainless steel gripper teeth, nitrile/EPDM O-ring
- **Envelope:** 12.7mm body OD x 38.1mm overall length
- **Features:**
  - Accepts 1/4" OD (6.35mm) tubing from each end
  - Insertion depth to tube stop: ~16mm per side (industry convention)
  - **Barbell profile (caliper-verified):** 15.10mm body end OD, 9.31mm center body OD, 12.08mm body end length, 12.16mm center body length
  - **Collet (release sleeve, caliper-verified):** 9.57mm OD, 1.44mm wall, 6.69mm ID
  - Collet protrusion from body face: ~1.4mm per side compressed, ~2.7mm extended (caliper-verified)
  - Collet travel (inward, for release): ~1.3mm per side (caliper-verified)
  - Release force per fitting: ~3-5N
  - Grip force per fitting: ~20N (4 fittings total ~80N retention)
  - Max working pressure: 150 PSI at 70F
  - NSF 61 certified for potable water
- **Interfaces:**
  - Mounted inside cartridge rear wall (JG fittings are cartridge-mounted, not dock-mounted)
  - Cartridge slides onto bare 1/4" OD tube stubs protruding from dock back wall
  - Cartridge interior side: accepts hard tube stubs from pump tubing transition
  - Release plate stepped bores engage collets (9.57mm) and body ends (15.10mm) during disconnect
  - 2x2 grid arrangement in cartridge, ~40mm horizontal x ~28mm vertical center-to-center
- **Quantity:** 4 (in cartridge -- listed here for dock interface reference)
- **Open:** None — all critical dimensions caliper-verified (see cartridge parts.md and off-the-shelf-parts/john-guest-union/)

### Part: 1/4" OD Hard Nylon Tubing (Dock Tube Stubs)

- **Type:** Purchased
- **Material:** Nylon or polyethylene, 1/4" OD (6.35mm)
- **Envelope:** 6.35mm OD, cut to ~60-80mm length per stub (passes through dock back wall, protrudes on both sides)
- **Features:**
  - Rigid enough to push into JG fittings without buckling
  - Food-grade compatible
  - 4 stubs pass through dock back wall 7mm grommeted holes
  - Front side protrusion: ~30mm (cartridge JG fittings slide onto these)
  - Rear side protrusion: connects to valve rack tubing
- **Interfaces:**
  - Pass through dock back wall via 7mm grommeted holes
  - Front side: cartridge JG fittings grip these stubs on dock-in
  - Rear side: connect to valve rack tubing via JG or barb fittings
  - Spacing: ~40mm horizontal x ~28mm vertical center-to-center (matches cartridge fitting positions)
- **Quantity:** 4 (dock tube stubs only)
- **Open:**
  - Exact stub length depends on dock back wall thickness + required protrusion on each side

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
  - Mounts in back panel upper zone (X=165, Z=370)
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
- **Envelope:** 115.6W x 68.6D x 62.7H mm (single pump, verified)
- **Weight:** 306g each (verified)
- **Features:**
  - 12V DC brushed motor
  - 400ml/min flow rate
  - BPT pump tube: 4.8mm ID x 8.0mm OD
  - Mounting bracket: 2-4x M3 holes (exact pattern TBD, measure from physical pump)
  - Noise: <=65 dB
- **Interfaces:**
  - Two pumps side-by-side in cartridge: ~137.2mm combined width (at 68.6mm each)
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
  - Mount in dock ceiling PCB or bracket at X=106 (centered on cartridge)
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
  - Spool compatible: 3mm thickness, ~70mm spool OD for 1m length (see reel housing calculation)
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

### Part: 1/4" OD Silicone Tubing (External Flavor Lines)

- **Type:** Purchased
- **Material:** Platinum-cured food-grade silicone (FDA 21 CFR 177.2600)
- **Envelope:** 6.35mm OD (1/4"), 3.18mm ID (1/8"), 1.59mm wall
- **Features:**
  - External cosmetic run only: back panel cable gland to faucet dispenser nozzle
  - External run: ~600-900mm per line (under-sink to countertop)
  - Dead volume: ~7ml per line (at 1/8" ID, 900mm external)
  - Flexible, transparent or translucent
  - Not used for any internal connections — all internal plumbing uses hard 1/4" OD PE/nylon tubing with JG push-to-connect fittings
- **Interfaces:**
  - Internal end: passes through PG7 cable gland in back panel; short silicone stub connects to hard tubing inside via barb or push-fit
  - External end: routes to faucet dispenser nozzle
- **Quantity:** 2 lines, ~1.0m each (total ~2.0m)
- **Open:** None

### Part: Tube Routing Clips and Saddles

- **Type:** 3D printed (integral to enclosure floor/walls) or purchased snap-in clips
- **Material:** PETG or ABS (printed); nylon (purchased)
- **Envelope:** Varies -- individual clips/saddles, ~10mm wide each
- **Features:**
  - Clips and saddle clamps secure hard 1/4" OD tubing against enclosure floor and walls
  - U-channels are not used — JG elbow fittings protrude perpendicular and won't fit in tight channels
  - Route from valve rack forward to bag connector zone and rearward to back panel
  - Approximate coverage: Y=130-292 (behind cartridge to back panel), X=varies
- **Interfaces:**
  - 1/4" OD hard tubing snaps into clips
  - Screw-down or printed-integral mounting
- **Quantity:** Estimated 15-25 clips across all tube runs
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
| Rubber grommet | 7mm ID, ~11mm OD | 4 | Dock back wall tube stub pass-throughs |
