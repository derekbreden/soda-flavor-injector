# Valve Architecture: Flow Routing for Fill, Dispense, and Prime Modes

This document defines the valve subsystem that routes fluid between the hopper, bags, pumps, tap water inlet, dip tubes, and dispensing lines. Each of the 2 peristaltic pumps requires a set of valves to switch between operating modes: filling a bag from the hopper, dispensing flavor from a bag to the faucet, filling with tap water, and evacuating air via dip tubes. All valves are mounted in the main enclosure body. The removable pump cartridge contains only the 2 pumps.

**Chosen architecture: 10 two-way normally-closed (NC) solenoid valves, 5 per pump line.** This was selected over 3-way alternatives after research confirmed that 3-way valves with a center-off (all ports closed) position are not available as standard products in the required form factor, and standard 3-way valves lack the critical "all closed" idle state this system requires.

---

## 1. Complete Fluid Schematic

### 1a. Node Notation

```
b1, b2 = bag bottom outlets (main fluid port P1)
t1, t2 = bag dip tubes (air bleed port P2, tube runs to sealed end at top of bag)
i1, i2 = pump inlets
o1, o2 = pump outlets
p1, p2 = pumps (Kamoer peristaltic, in removable cartridge)
h = hopper (shared, single funnel)
h1, h2 = hopper branches (one per pump line)
d1, d2 = dispenser lines (silicone tubes to faucet)
w = tap water inlet
w1, w2 = tap water branches
```

### 1b. System Overview

Each flavor line has fluid endpoints that connect through a single pump:

- **Hopper funnel** (shared between both lines, branches to h1/h2)
- **Bag bottom outlet** (b1/b2 -- one per line, direct connection to pump inlet)
- **Bag dip tube** (t1/t2 -- air bleed port at top of bag, for air evacuation)
- **Pump** (p1/p2 -- one per line, in removable cartridge)
- **Dispensing line** (d1/d2 -- silicone tube to faucet)
- **Tap water inlet** (w -- shared, branches to w1/w2)

**CRITICAL: b1 connects directly to i1 (and b2 to i2) with NO valve.** This permanent connection is what keeps the valve count at 10 instead of 12. During dispense, v5/v7/v9/v10 are all closed, so each pump can only draw from its bag.

### 1c. The 10 Valves

All 10 valves are 2-way normally-closed (NC) solenoid, 12V DC.

| Valve | Position | Function |
|-------|----------|----------|
| v1 | o1 -> d1 | Dispense line 1, keeps flavor primed to faucet |
| v2 | o2 -> d2 | Dispense line 2 |
| v3 | w1 -> b1 | Tap water fill into bag 1 (pressure-fed, no pump needed) |
| v4 | w2 -> b2 | Tap water fill into bag 2 |
| v5 | h1 -> i1 | Hopper to pump 1 inlet (flavor fill AND air fill modes) |
| v6 | o1 -> b1 | Pump 1 outlet to bag 1 (reverse: pushing into bag) |
| v7 | h2 -> i2 | Hopper to pump 2 inlet |
| v8 | o2 -> b2 | Pump 2 outlet to bag 2 |
| v9 | t1 -> i1 | Dip tube 1 to pump 1 inlet (air evacuation) |
| v10 | t2 -> i2 | Dip tube 2 to pump 2 inlet (air evacuation) |

### 1d. Full Fluid Topology -- ASCII Schematic

```
                TAP WATER (w)           HOPPER (h)
                     |                      |
                +---------+            +---------+
                |         |            |         |
               w1        w2          h1         h2
                |         |            |         |
               V3        V4          V5         V7
          (w1->b1)  (w2->b2)   (h1->i1)   (h2->i2)
                |         |            |         |
               b1        b2           |         |
    DIP TUBE   |          |   DIP TUBE |         |
    t1----V9---+--->i1    +--->i2<---V10----t2
               |     |         |     |
               |   [P1]      [P2]   |
               |     |         |     |
               |    o1        o2     |
               |     |         |     |
               +<-V6-+---V1   V2---+-V8->+
                      |         |
                     d1        d2
                  (faucet)  (faucet)
```

Note: b1->i1 and b2->i2 are DIRECT connections (no valve). The `+--->` notation shows these permanent paths.

### 1e. Operating Modes

| Mode | Open Valves | Path |
|------|-------------|------|
| Dispense flavor 1 | v1 | b1->i1->p1->o1->v1->d1 |
| Dispense flavor 2 | v2 | b2->i2->p2->o2->v2->d2 |
| Fill bag 1 from hopper | v5, v6 | h->h1->v5->i1->p1->o1->v6->b1 |
| Fill bag 2 from hopper | v7, v8 | h->h2->v7->i2->p2->o2->v8->b2 |
| Fill bag 1 with tap water | v3 | w->w1->v3->b1 (gravity/pressure) |
| Fill bag 2 with tap water | v4 | w->w2->v4->b2 (gravity/pressure) |
| Pump out bag 1 | v1 | b1->i1->p1->o1->v1->d1 (same as dispense) |
| Fill bag 1 with air (dry cycle) | v5, v6 | h->h1->v5->i1->p1->o1->v6->b1 (hopper open to atmosphere) |
| Evacuate air via dip tube 1 | v9, v1 | t1->v9->i1->p1->o1->v1->d1 |
| Evacuate air via dip tube 2 | v10, v2 | t2->v10->i2->p2->o2->v2->d2 |
| **Idle** | **none** | **All NC valves closed, everything sealed** |

### 1f. Valve Truth Table

All valves are normally closed (NC). O = OPEN (energized), C = CLOSED (NC default). Pump: FWD = forward, OFF = stopped.

| Mode | v1 | v2 | v3 | v4 | v5 | v6 | v7 | v8 | v9 | v10 | P1 | P2 | Notes |
|------|----|----|----|----|----|----|----|----|----|----|----|----|-------|
| **Idle** | C | C | C | C | C | C | C | C | C | C | OFF | OFF | All closed, zero power |
| **Dispense flavor 1** | O | C | C | C | C | C | C | C | C | C | FWD | OFF | b1->p1->d1 |
| **Dispense flavor 2** | C | O | C | C | C | C | C | C | C | C | OFF | FWD | b2->p2->d2 |
| **Dispense both** | O | O | C | C | C | C | C | C | C | C | FWD | FWD | Both lines active |
| **Fill bag 1 (hopper)** | C | C | C | C | O | O | C | C | C | C | FWD | OFF | h->p1->b1 |
| **Fill bag 2 (hopper)** | C | C | C | C | C | C | O | O | C | C | OFF | FWD | h->p2->b2 |
| **Fill bag 1 (tap water)** | C | C | O | C | C | C | C | C | C | C | OFF | OFF | Pressure-fed, no pump |
| **Fill bag 2 (tap water)** | C | C | C | O | C | C | C | C | C | C | OFF | OFF | Pressure-fed, no pump |
| **Evacuate air tube 1** | O | C | C | C | C | C | C | C | O | C | FWD | OFF | t1->p1->d1 |
| **Evacuate air tube 2** | C | O | C | C | C | C | C | C | C | O | OFF | FWD | t2->p2->d2 |

**Key observations:**
- Idle draws zero power (all valves NC, all pumps off). This is the safe default state.
- During dispense, only 1 valve is energized per active pump (v1 or v2). All other valves stay closed, so the pump can only draw from the direct b->i connection.
- During hopper fill, exactly 2 valves are energized per active pump (v5+v6 or v7+v8).
- Tap water fill requires only 1 valve and no pump (pressure-fed from house water line).
- Air evacuation via dip tube requires 2 valves per line (v9+v1 or v10+v2).
- Maximum simultaneous valve load: 2 valves (dispense both uses v1+v2; other modes use 1-2 valves).
- The "all closed" idle state is the fundamental advantage of 2-way NC valves -- it prevents backflow, keeps fluid primed at the faucet, and handles power loss, firmware crash, or error states safely.

### 1g. Dip Tube Air Evacuation (v9/v10)

Each bag has two ports: the bottom outlet (b1/b2) for liquid, and a dip tube (t1/t2) that runs from a port on the bag fitting up to the sealed top end of the bag. After filling, trapped air collects at the top of the bag above the liquid level. The dip tube reaches this air pocket.

v9 and v10 connect each dip tube to its corresponding pump inlet. During air evacuation:
1. Open v9 (or v10) and v1 (or v2)
2. Run the pump forward
3. The pump draws air from the top of the bag through the dip tube and pushes it out through the dispense line
4. Once liquid reaches the dip tube (detected by FDC1004 capacitive sensor or flow change), close v9/v10 and stop

This primes the entire system: liquid fills the bag, the dispense line is primed with flavor, and trapped air is removed. Without v9/v10, air would remain trapped and cause sputtering during initial dispenses.

### 1h. Reverse-Fill Short Circuit Note

During hopper fill (v5+v6 open for line 1), the path is h->v5->i1->p1->o1->v6->b1, pushing liquid into the bag. But b1 connects directly to i1 (no valve), creating a potential recirculation path: liquid pushed into b1 could flow back through the direct b1->i1 connection to the pump inlet.

In practice, the peristaltic pump is positive-displacement -- each roller sweep moves a fixed volume -- and the bag expands to accept it, so net flow goes into the bag. This may reduce fill efficiency slightly but does not prevent filling. Not worth adding 2 more valves to eliminate.

---

## 2. Why 10 Two-Way NC Valves (Not Fewer, Not 3-Way)

### 2a. The "All Closed" Requirement

The system needs a true idle state where ALL paths are closed -- no flow in any direction. This prevents:
- Gravity-driven backflow from bags through idle pumps
- Syrup draining away from the dispensing line (losing the primed state)
- Cross-contamination between hopper and bag lines during idle
- Uncontrolled flow during firmware crashes, power loss, or error conditions

A 3-way valve toggles between port A and port B but has no "off" position. De-energized, it connects to port A. Energized, it connects to port B. There is always an open path. This means a 3-way valve architecture has no true idle state.

### 2b. Why 2-Way NC Not 3-Way

3-way valves toggle between ports A and B but have NO "off" position. The all-closed idle state is essential for preventing backflow, keeping dispense lines primed to the faucet, and handling error/fault states. Research confirmed that 3-way valves with center-off position don't exist in the required form factor (12V DC, 1/4", food-safe, Amazon Prime).

### 2c. 3-Way Valve Analysis (Considered and Rejected)

A 3-way solenoid valve has one common port and two selectable ports. De-energized, the common port connects to port A. Energized, the common port connects to port B. Each pump junction needs to select between two paths, so a 3-way valve at each junction could theoretically replace a pair of 2-way valves:

| Junction | 3-Way Valve | De-energized (Port A) | Energized (Port B) |
|----------|-------------|----------------------|---------------------|
| P1 inlet | 3W-1 | Bag 1 (dispense mode) | Hopper (fill mode) |
| P1 outlet | 3W-2 | Dispensing line 1 (dispense mode) | Bag 1 (fill mode) |
| P2 inlet | 3W-3 | Bag 2 (dispense mode) | Hopper (fill mode) |
| P2 outlet | 3W-4 | Dispensing line 2 (dispense mode) | Bag 2 (fill mode) |

This would reduce valve count but loses the critical "all closed" idle state (bag-to-faucet path always connected when power is off).

**Why this was rejected:**

1. **No "all closed" idle state.** With 3-way valves de-energized, the bag-to-faucet path is always open. If the pump is off, there is no flow, but the path is not sealed. Gravity or pressure differentials could cause slow seepage. The fluid column between the bag and faucet is not locked in place -- it can drain or shift.

2. **Backflow vulnerability.** If the soda water line has any back-pressure (e.g., carbonation pressure, flow transient when main soda valve opens), that pressure can push back through the always-open dispense path into the bag. With 2-way NC valves, the closed outlet valve blocks this entirely.

3. **Priming loss.** The dispensing line must stay primed with flavor syrup right up to the injection point at the faucet. An always-open path allows the syrup column to slowly drain back toward the bag under gravity (the bag is below the faucet in the under-sink installation). NC valves on both sides of the pump lock the fluid column in place.

4. **Error state safety.** During error conditions (sensor fault, firmware watchdog, unexpected state), the safest position is "everything closed, nothing flows." With 2-way NC valves, killing power achieves this instantly. With 3-way valves, killing power leaves the dispense path open.

### 2d. 3-Way Valves with Center-Off: Do They Exist?

A 3-way valve with a center-off position (all three ports closed when de-energized) would solve the idle-state problem. This would give: position A (fill path), position B (dispense path), and center position (all closed).

**Research findings (March 2026):**

3-way 3-position solenoid valves with a true center-off (all ports blocked) are not a standard product category in the small/cheap valve market. Web searches across Amazon, industrial suppliers, and valve manufacturers consistently show:

- **3-way 2-position** is the standard configuration. These toggle between A and B with a single solenoid. No center position exists. This is what Burkert, Parker, Beduan, U.S. Solid, and all major small-valve manufacturers sell.

- **4-way 3-position center-closed** valves exist (e.g., MettleAir 4V330C series) but these are pneumatic directional control valves designed for cylinder actuation. They have 5 ports (2 work ports, 1 pressure, 2 exhaust), require dual solenoids (one per direction), are large (~140mm long), expensive (~$30-55 each), use NPT threaded ports (not barb/quick-connect), are made of aluminum (not food-safe for syrup contact), and are designed for air, not liquid. Using 4 of these would cost $120-220, require 8 GPIOs (dual solenoid), and need NPT-to-barb adapters on every port.

- **Proportional or servo-type 3-position valves** exist in industrial process control but start at $100+ per valve and are far too large and complex.

**Conclusion:** A cheap, compact, food-safe, 1/4" barb, 12V DC, 3-way valve with center-off position that is available overnight on Amazon Prime does not exist. The 2-way NC valve is the only architecture that provides the required "all closed" idle state using readily available, inexpensive, food-compatible components.

### 2e. Architecture Comparison Summary

| Configuration | Valve Count | Idle State | Dispense Power | Fill Power | Cost (total) | Availability |
|---------------|-------------|------------|----------------|------------|-------------|--------------|
| **10x 2-way NC (chosen)** | **10** | **All closed (zero power)** | **1 valve (~5W)** | **2 valves (~10W)** | **~$90** | **Amazon Prime, overnight** |
| 4x 3-way 2-position | 4 | Dispense path open (no off) | 0 valves (0W) | 2 valves (~8W) | $32-60 | Available but wrong behavior |
| 4x 3-way 3-position center-off | 4 | All closed (requires dual solenoid) | Complex | Complex | Not available in required form | Does not exist as standard product |
| 4x 4-way 3-pos center-closed (pneumatic) | 4 | All closed | Dual solenoid per valve | Dual solenoid | $120-220 | Available but wrong form factor |

---

## 3. Valve Selection: Specific Components

### 3a. Requirements

All 10 valves must meet:
- 12V DC operation (system runs on 12V rail)
- Normally closed (NC) -- closed when de-energized
- 2-way (single inlet, single outlet)
- Food-safe or food-compatible wetted materials (for sugar syrup contact at room temperature)
- 1/4" (6.35mm) quick-connect or barb fittings (matches system tubing)
- Compact (under 60mm in any dimension, ideally under 50mm)
- Under $10 each
- Available on Amazon Prime for overnight/2-day delivery

### 3b. Amazon-Available 2-Way NC Valves (Researched March 2026)

The RO (reverse osmosis) water purification market produces exactly the type of valve needed: small, 12V DC, NC, 1/4" quick-connect, food-grade plastic, designed for potable water contact. These are mass-produced, cheap, and available from multiple vendors on Amazon Prime.

**Candidate 1: DIGITEN DC 12V 1/4" Inlet Feed Water Solenoid Valve**
- ASIN: B016MP1HX0
- Type: 2-way NC, 12V DC
- Fittings: 1/4" quick-connect (push-fit, matches standard RO tubing)
- Power: 4.8W rated
- Working pressure: 0.02-0.8 MPa
- Working temperature: 0-70C (32-158F)
- Weight: ~100g
- Materials: Food quality, designed for RO water systems
- Price: ~$7-8 (check current listing)
- Prime eligible: Yes
- Notes: One of the most popular RO solenoid valves on Amazon. Thousands of reviews. Also available in a zero-pressure variant (B076KFCPGM) that works without inlet pressure -- important since our peristaltic pump provides suction, not pressure, on the inlet side.

**Candidate 2: DIGITEN DC 12V 1/4" Zero-Pressure Variant**
- ASIN: B076KFCPGM
- Same specs as above but rated for 0-0.05 MPa working pressure
- Specifically designed for applications with no inlet water pressure
- This variant may be better suited for pump-inlet valves where the pump creates suction rather than the water supply providing pressure

**Candidate 3: SENSTREE (formerly ZAOJIAO) DC 12V 1/4" Solenoid Valve**
- ASIN: B0743CSRFF
- Type: 2-way NC, 12V DC
- Fittings: 1/4" quick-connect
- Power: ~4.8W rated
- Working pressure: 0.02-0.8 MPa
- Materials: Food quality for RO water systems
- Price: ~$6-8 (check current listing)
- Prime eligible: Yes
- Notes: Similar spec to DIGITEN. Sold by SENSTREE, fulfilled by Amazon.

**Candidate 4: Beduan 12V 1/4" Inlet Water Solenoid Valve** ★ Selected, caliper-verified
- ASIN: B07NWCQJK9
- Type: 2-way NC, 12V DC
- Fittings: 1/4" quick-connect (push-to-connect)
- Power: 5.5W rated
- Working pressure: 0.02-0.8 MPa
- Working temperature: up to 60C (140F)
- **Valve dimensions (caliper-verified):** L-shaped body, 32.71mm wide (X) x 50.84mm deep (Y, port-to-port) x 56.00mm tall (Z, bottom of white body to top of spade connectors)
- Package dimensions: ~215 x 60 x 34mm (Amazon packaging, not the valve)
- Weight: ~100g
- Materials: White plastic valve body + metal solenoid coil housing, designed for RO systems
- Rating: 4.2/5 stars (143 ratings)
- Price: ~$8.99 (check current listing)
- Prime eligible: Yes
- Notes: Beduan is a well-known valve brand on Amazon with a wide product line. L-shaped profile: horizontal white valve body with vertical metal solenoid coil rising from one end. No built-in mounting features -- requires designed cradle/clamp.

**Candidate 5: Akent 1/4" DC12V Inlet Feed Water Solenoid Valve**
- ASIN: B071RYT994
- Type: 2-way NC, 12V DC
- Fittings: 1/4" quick-connect
- Power: 4.8W rated
- Working pressure: 0-0.8 MPa
- Materials: Designed for RO reverse osmosis systems
- Price: ~$6-8 (check current listing)
- Prime eligible: Yes

### 3c. Valve Material Compatibility with Sugar Syrups

All candidates above are designed for potable water contact in RO systems. Sugar syrup introduces additional concerns:

- **Viscosity:** Flavor syrups (typically 1:5 concentrate) are slightly more viscous than water but well within these valves' flow capability
- **Sugar residue:** Sugar can crystallize on valve seats if the valve dries out. Mitigation: keep valves in the fluid path (always wet), periodic flush cycle
- **Acidity:** Fruit-flavored syrups may be mildly acidic (pH 3-4). The plastic/elastomer materials in RO valves handle this range
- **Temperature:** Syrups are stored and dispensed at room temperature (15-25C), well within all valve ratings

**Recommendation:** Order 2-3 units each of the DIGITEN (B016MP1HX0) and Beduan (B07NWCQJK9) for bench testing with actual flavor syrup. Run a 30-day soak test to verify seal integrity and check for sugar crystallization on the valve seat.

### 3d. Valve Heating Consideration

These solenoid valves draw 4.8-5.5W continuously while energized. The coils will get warm (up to 60C per manufacturer specs). During dispense, only 1 valve is energized for the duration of the pour (typically 5-15 seconds). During fill, 2 valves are energized for 3-5 minutes. During air evacuation, 2 valves are energized briefly. Neither duty cycle is a thermal concern. The valves spend the vast majority of their time de-energized (idle/NC state) at zero power draw.

### 3e. Quick-Connect Fitting Compatibility

All candidates use standard 1/4" (6.35mm OD) RO-style push-fit connectors. These accept standard 1/4" OD polyethylene or silicone tubing by pushing the tube into the fitting until it clicks. To release, press the collet ring and pull the tube out. This is the same fitting standard used throughout the RO water purification industry.

The system uses 1/4" ID silicone tubing (approximately 6.35mm OD with thin-wall tubing). Verify OD compatibility with specific tubing before ordering -- some silicone tubing has thicker walls giving a larger OD that won't fit push-connect fittings. If needed, short adapters (1/4" barb to 1/4" push-connect) are available for under $1 each.

---

## 4. Other Valve Types Considered

### 4a. Solenoid Pinch Valves (2-way only)

**How they work:** An electromagnetic solenoid squeezes a section of silicone tubing shut. The fluid never contacts the valve body -- it stays inside the tubing.

| Parameter | Value |
|-----------|-------|
| Cost | $5-15 per valve |
| Size | ~20x20x40mm typical |
| Power (holding) | 2-5W per valve (continuous while energized) |
| Food safety | Excellent -- no fluid contact with valve internals |
| 3-way available? | No. Pinch valves are inherently 2-way |

**Verdict:** Pinch valves would work with the 10-valve architecture and have excellent food safety (no fluid contact). However, they are less common in 12V DC form, harder to source on Amazon Prime, and the tubing at the pinch point can fatigue over many cycles. They remain a viable alternative if the RO-style solenoid valves prove problematic with sugar syrup.

### 4b. 3-Way Solenoid Diaphragm Valves

Available from Burkert, Parker, Beduan, and generic Amazon/AliExpress vendors. Typically $8-20 each ($15-35 for food-grade models). Would reduce valve count but lack the center-off idle state (see Section 2b-2d for detailed rejection rationale).

### 4c. Solenoid Ball Valves (2-way and 3-way)

$15-40 per valve, larger (~30x30x60mm), slower response (50-200ms). Overkill for this application's low flow rates (~5-400 ml/min) and pressures (~2-8 PSI).

### 4d. Motorized Ball Valves

$10-25 per valve, zero holding power (motor drives to position then holds mechanically), but slow switching (1-5 seconds). A 5-second delay between pressing "dispense" and getting soda is a poor user experience. Not recommended.

### 4e. Latching Solenoid Valves

$20-50 per valve. Zero holding power (permanent magnet holds position, brief pulse to switch). Ideal on paper but rare in food-grade configurations at this price point. A latching 2-way NC valve would be the best of all worlds if sourced affordably. Not currently available in the required form factor on Amazon.

---

## 5. Physical Dimensions and Placement

### 5a. Enclosure Space Budget

At the 220mm width target, valves cannot be arranged in side banks flanking the cartridge -- there is not enough lateral space. Instead, the 10 valves are placed behind and/or above the cartridge dock area.

The Beduan solenoid valve (caliper-verified) is L-shaped, not cylindrical: 32.71mm wide x 50.84mm deep (port-to-port) x 56.00mm tall (white body base to spade connector tips). The white valve body is ~19.4mm tall; the metal solenoid coil housing rises ~36.6mm above that.

### 5b. Fitting 10 Two-Way Valves

**Option A: Behind cartridge, two rows of 5**

- Two rows of 5 valves along the dock back wall, spanning the full enclosure width.
- 5 valves x ~35mm pitch (32.71mm body + ~2mm gap) = ~175mm per row, fits within the 220mm width.
- Two rows at ~55mm depth (50.84mm port-to-port) = ~110mm total depth behind the cartridge.
- All valves in one serviceable zone, accessible when cartridge is removed.
- Height per row: 56mm (to spade connector tips). Two rows stacked would need ~116mm vertical.

**Option B: Above cartridge, single layer**

- 10 valves arranged in a 5x2 grid above the cartridge dock area.
- Requires vertical space but preserves depth for bags.
- Valves can be mounted upside-down with ports facing down for clean tube routing.

**Option C: Mixed behind and above**

- v1-v8 behind the cartridge (these connect to pump inlet/outlet), v9-v10 above or beside (dip tube connections route differently).
- Keeps the most-used valves in the tightest integration zone near the pump fittings.

**Recommendation for prototyping: Option A (behind cartridge).** Groups all valves in one zone for clear tube routing and easy access. The 220mm width constraint rules out side-bank layouts used in wider enclosure designs.

### 5c. Valve Mounting

The Beduan valve has no built-in mounting features (no tabs, flanges, ears, or screw holes). It must be held entirely by a designed cradle/clamp. For prototyping, use 3D-printed valve cradles that grip the white valve body (32.71mm wide, ~19.4mm tall) with snap-over retention clips, and leave a slot for the vertical solenoid coil. Orient valves with quick-connect ports facing toward their respective tube connections (bags rearward, hopper upward, dispensing line forward).

---

## 6. Tube Routing

### 6a. Tube Segment Map

With the 10-valve topology and direct b->i connections, tube routing is simpler than the previous 8-valve design -- no TEE junctions are needed at the pump inlet. The bag bottom outlet connects directly to the pump inlet with a single tube.

**Key tube segments per line (Line 1 shown):**

| Seg | From | To | Notes |
|-----|------|----|-------|
| b1->i1 | Bag 1 bottom outlet | Pump 1 inlet | DIRECT, no valve, always connected |
| v5 path | Hopper branch h1 | Pump 1 inlet (via v5) | Joins at i1 with TEE |
| v9 path | Dip tube t1 | Pump 1 inlet (via v9) | Joins at i1 with TEE |
| v1 path | Pump 1 outlet o1 | Dispense line d1 (via v1) | To faucet |
| v6 path | Pump 1 outlet o1 | Bag 1 bottom b1 (via v6) | Reverse fill into bag |
| v3 path | Tap water w1 | Bag 1 bottom b1 (via v3) | Pressure-fed water fill |

**Note on pump-inlet TEE:** The pump inlet (i1) has three possible sources: b1 (direct), v5 (hopper), and v9 (dip tube). These join at a TEE or manifold at i1. Because only one source path is ever open at a time (or none, during dispense where only the direct b1 path is active with all other valves closed), there is no flow conflict.

**Note on pump-outlet split:** The pump outlet (o1) connects to v1 (dispense) and v6 (reverse fill to bag). These split at a TEE at o1. Only one is ever open at a time.

**Flavor Line 2: Mirror image using v2, v4, v7, v8, v10, b2, i2, o2, t2, d2.**

---

## 7. Cartridge Interface

### 7a. Cartridge Fluid Connections

The cartridge contains 2 pumps, each with an inlet and outlet tube stub. The valve architecture is entirely in the enclosure.

| Connection | Cartridge Side | Enclosure Side |
|------------|---------------|----------------|
| P1 inlet | Tube stub from pump 1 inlet | TEE joining b1 (direct), v5 output, v9 output |
| P1 outlet | Tube stub from pump 1 outlet | TEE splitting to v1 input and v6 input |
| P2 inlet | Tube stub from pump 2 inlet | TEE joining b2 (direct), v7 output, v10 output |
| P2 outlet | Tube stub from pump 2 outlet | TEE splitting to v2 input and v8 input |

**4 fluid connections total.** John Guest 1/4" push-to-connect fittings at the dock back wall provide tool-free insertion/removal. A cam lever release plate on the cartridge disconnects all 4 fittings in a single motion.

### 7b. Cartridge Insertion Sequence

1. User slides cartridge into dock
2. JG fittings grip tube stubs automatically -- fluid paths sealed
3. Pogo pins make electrical contact (pump motor connections)
4. Firmware detects cartridge (MCP23017 input) and runs a self-test
5. Self-test: briefly open each valve pair and run each pump to verify flow

---

## 8. Failure Modes

### 8a. Valve Failure Analysis

With 2-way NC valves, a failure means either the valve is stuck closed (fails to open when energized) or stuck open (fails to close when de-energized).

| Failure | Impact During Dispense | Impact During Fill | Impact During Idle | Severity |
|---------|----------------------|-------------------|-------------------|----------|
| v1 stuck closed | Dispense line 1 fails | None (v1 not used in fill) | None (NC is idle state) | High |
| v1 stuck open | Dispense works but faucet path always open | Pump output leaks to faucet during fill | Faucet path not sealed; drip risk | High |
| v5 stuck closed | None (v5 not used in dispense) | Hopper fill fails for line 1 | None | Medium |
| v5 stuck open | Hopper connected to pump inlet; air ingestion if hopper empty | Fill works normally | Hopper path not sealed | High |
| v6 stuck closed | None (v6 not used in dispense) | Reverse fill into bag 1 fails | None | Medium |
| v6 stuck open | Pump output leaks to bag during dispense | Fill works normally | Bag path not sealed | Medium |
| v9 stuck closed | None (v9 not used in dispense) | None | Air evacuation fails for line 1 | Medium |
| v9 stuck open | Dip tube connected to pump inlet; air ingestion | None | Dip tube path not sealed | High |

### 8b. Safe Default State

All 10 valves de-energize to closed (NC). If power fails or firmware crashes:
- All valves close (spring return to NC)
- Pumps stop (no power = no flow)
- All fluid paths sealed (except the direct b->i connections, which are harmless with pump stopped)
- No unintended flow possible

This is the safest possible default state. The only way to get unintended flow is if a valve physically fails open AND a pump somehow continues running -- requiring both a valve hardware fault AND a driver fault simultaneously.

### 8c. Detection Methods

| Method | Detects | Hardware Needed | Already Planned? |
|--------|---------|-----------------|-----------------|
| FDC1004 capacitive sensing | Air in lines, empty bag, hopper empty | FDC1004 (4 channels, I2C) | Yes |
| Flow meter on soda line | Dispense duration mismatch | Flow meter | Yes |
| Pump current monitoring | Dry-run detection | Current sense resistor + ADC | No |

### 8d. Recommended Safety Interlocks

1. **Fill mode timeout:** If fill runs longer than expected (e.g., 10 minutes for a 1L bag), abort and alert.
2. **Dispense flow correlation:** If pump runs for >2 seconds with no flow meter activity on the soda line, pause and alert.
3. **Post-fill verification:** After fill completes, close all valves and verify FDC1004 detects fluid (not air) in the dispense tube segment.

---

## 9. Power Budget

### 9a. Valve Power Draw

Peak theoretical: 10 valves x 4.8W = 48W. But all 10 are never open simultaneously. Maximum concurrent valves is 2-3, so real peak valve power is ~15W.

| Mode | Valves Energized | Valve Power | Pump Power | Total System |
|------|-----------------|-------------|------------|-------------|
| Idle | 0 | 0W | 0W | ~1W (ESP32 + displays) |
| Dispense 1 line | 1 (v1 or v2) | ~5W | ~5W | ~11W |
| Dispense both | 2 (v1+v2) | ~10W | ~10W | ~21W |
| Fill 1 line (hopper) | 2 (v5+v6 or v7+v8) | ~10W | ~5W | ~16W |
| Fill 1 line (tap water) | 1 (v3 or v4) | ~5W | 0W | ~6W |
| Air evacuation | 2 (v9+v1 or v10+v2) | ~10W | ~5W | ~16W |

### 9b. PSU Sizing

Peak draw is during dual dispense: ~21W, or during fill + pump: ~16W. Real peak with pumps is approximately 25W. A 12V/3A (36W) power supply handles all modes with generous headroom. A 12V/2A (24W) supply also works for all normal modes.

### 9c. Annual Energy Cost

Assuming 10 dispenses/day at 10 seconds each, plus 1 fill/week at 5 minutes:
- Dispense valve energy: 5W x 10s x 10/day x 365 = 0.05 kWh/year
- Fill valve energy: 10W x 300s x 52/week = 0.04 kWh/year
- Total valve energy: ~0.09 kWh/year (negligible)

---

## 10. GPIO and Driver Requirements

### 10a. MCP23017 I2C Expander Control

Each 2-way NC solenoid valve needs 1 GPIO output (energize/de-energize) and 1 MOSFET driver circuit. The 10 valves are controlled via MCP23017 I2C I/O expander connected to the ESP32.

| Resource | Count | Source |
|----------|-------|--------|
| GPIO outputs | 10 | MCP23017 (GPB0-GPB7 = v1-v8, GPA0-GPA1 = v9-v10) |
| MOSFET drivers | 10 | IRLZ44N + 10k gate resistor + 100k pulldown + 1N4007 flyback diode, per valve |
| 12V switched power | 10 channels | From 12V rail through MOSFETs |

The MCP23017 has 16 GPIO pins in two 8-bit ports (GPA0-7, GPB0-7). GPB0-GPB7 control v1-v8. GPA0-GPA1 control v9-v10. Remaining GPA pins handle other functions.

### 10b. Updated MCP23017 Pin Plan

| MCP Pin | Direction | Function | Priority |
|---------|-----------|----------|----------|
| GPA0 | Output | V9: dip tube 1 air evacuation (t1->i1) | High |
| GPA1 | Output | V10: dip tube 2 air evacuation (t2->i2) | High |
| GPA2 | Input | Cartridge detection switch | High |
| GPA3 | Input | Lever position sensor | Medium |
| GPA4 | Output | Status LED green | Medium |
| GPA5 | Output | Status LED amber | Medium |
| GPA6 | -- | Unassigned | -- |
| GPA7 | -- | Unassigned | -- |
| GPB0 | Output | V1: dispense line 1 (o1->d1) | High |
| GPB1 | Output | V2: dispense line 2 (o2->d2) | High |
| GPB2 | Output | V3: tap water fill bag 1 (w1->b1) | High |
| GPB3 | Output | V4: tap water fill bag 2 (w2->b2) | High |
| GPB4 | Output | V5: hopper to pump 1 inlet (h1->i1) | High |
| GPB5 | Output | V6: pump 1 outlet to bag 1 (o1->b1) | High |
| GPB6 | Output | V7: hopper to pump 2 inlet (h2->i2) | High |
| GPB7 | Output | V8: pump 2 outlet to bag 2 (o2->b2) | High |

**Assigned: 14 of 16 pins. Free: 2 pins (GPA6-7).**

### 10c. Firmware Valve Control

Valve control via MCP23017 uses two registers -- GPIOA for v9/v10 and GPIOB for v1-v8:

- I2C write to GPB register sets v1-v8 states simultaneously
- I2C write to GPA register sets v9-v10 states (along with other GPA functions)
- Mode changes are atomic per port
- MCP23017 I2C address: 0x20 (A0=A1=A2=GND)
- I2C bus: shared with FDC1004 capacitive sensor and any other I2C peripherals

GPB valve state byte examples:
  - Idle: `0b00000000` (all closed)
  - Dispense line 1: `0b00000001` (v1 open)
  - Dispense line 2: `0b00000010` (v2 open)
  - Dispense both: `0b00000011` (v1+v2 open)
  - Fill bag 1 (hopper): `0b00110000` (v5+v6 open)
  - Fill bag 2 (hopper): `0b11000000` (v7+v8 open)
  - Fill bag 1 (tap water): `0b00000100` (v3 open)
  - Fill bag 2 (tap water): `0b00001000` (v4 open)

GPA valve bits (low 2 bits):
  - Air evacuate line 1: GPA0=1 (v9 open), combined with GPB v1 open
  - Air evacuate line 2: GPA1=1 (v10 open), combined with GPB v2 open

---

## 11. BOM Summary (Valves Only)

| Item | Qty | Unit Price | Total | Source |
|------|-----|-----------|-------|--------|
| Beduan 12V 1/4" NC Solenoid Valve (B07NWCQJK9) | 10 | ~$8.99 | ~$90 | Amazon Prime |
| IRLZ44N MOSFET (driver) | 10 | ~$0.50 | ~$5 | Amazon/Mouser |
| 1N4007 flyback diode | 10 | ~$0.10 | ~$1 | Amazon/Mouser |
| 10k + 100k resistors (per driver) | 20 | ~$0.05 | ~$1 | Amazon/Mouser |
| **Valve subsystem total** | | | **~$97** | |

---

## 12. Decision Summary

### 12a. Resolved

| Decision | Resolution | Rationale |
|----------|-----------|-----------|
| Valve architecture | 10 two-way NC solenoid valves (5 per pump) | Only architecture providing "all closed" idle state with cheap, available components; direct b->i connection saves 2 valves |
| 3-way valves | Rejected | No center-off position; always leaves one path open; 3-position variants don't exist in required form |
| Valve type | 2-way NC solenoid, RO water style, 12V DC, 1/4" quick-connect | Food-grade, ~$9 each, Amazon Prime overnight, proven in water systems |
| Primary candidate | Beduan B07NWCQJK9 | ~$8.99, food quality, 12V DC, 1/4" QC, widely available |
| GPIO control | MCP23017 I2C expander, GPB0-GPB7 = v1-v8, GPA0-GPA1 = v9-v10 | All 10 valves on one expander, preserves ESP32 native GPIOs |
| Safe default | All valves NC (closed), all pumps off | Power loss = everything sealed, no flow possible |
| Physical placement | Behind/above cartridge (not side banks) | 220mm width target too narrow for side-mounted valve banks |

### 12b. Open Questions

| Question | Impact | Next Step |
|----------|--------|-----------|
| Sugar syrup compatibility | Valve seal longevity with sticky residue | Order Beduan + DIGITEN, run 30-day soak test with actual syrup |
| Zero-pressure variant needed? | Pump-inlet valves see suction, not pressure | Test both standard (B016MP1HX0) and zero-pressure (B076KFCPGM) DIGITEN variants |
| Quick-connect vs. barb fitting | Tubing OD compatibility | Verify 1/4" silicone tubing OD fits push-connect; order barb adapters as backup |
| PSU sizing | ~25W peak is well within 36W supply | Size PSU at 36W (12V/3A) for comfortable headroom |
| Valve mounting in enclosure | Physical layout of 10 valves behind cartridge at 220mm width | Update master spatial layout for behind-cartridge valve zone |

---

## References

- [requirements.md](../requirements.md) -- Enclosure functional requirements including valve description
- [pump-assisted-filling.md](pump-assisted-filling.md) -- Previous fill topology analysis (TEE-based, with Option 1 recommended)
- [v1-master-spatial-layout.md](v1-master-spatial-layout.md) -- Component positions including allocated valve zones
- [v1-hopper-integration.md](v1-hopper-integration.md) -- Hopper funnel design, fill routing, two-flavor management
- [gpio-planning.md](../../../gpio-planning.md) -- MCP23017 pin assignments, MOSFET driver circuits
