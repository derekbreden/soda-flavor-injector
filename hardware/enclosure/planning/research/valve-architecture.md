# Valve Architecture: Flow Routing for Fill and Dispense Modes

This document defines the valve subsystem that routes fluid between the hopper, bags, pumps, and dispensing lines. Each of the 2 peristaltic pumps requires a set of valves to switch between two operating modes: filling a bag from the hopper, and dispensing flavor from a bag to the faucet. All valves are mounted in the main enclosure body. The removable pump cartridge contains only the 2 pumps.

**Chosen architecture: 8 two-way normally-closed (NC) solenoid valves, 4 per pump line.** This was selected over 3-way alternatives after research confirmed that 3-way valves with a center-off (all ports closed) position are not available as standard products in the required form factor, and standard 3-way valves lack the critical "all closed" idle state this system requires.

---

## 1. Complete Fluid Schematic

### 1a. System Overview

Each flavor line has four fluid endpoints that connect through a single pump:

- **Hopper funnel** (shared between both lines, selected by valve)
- **Bag** (one per line, permanent, sealed with dip tube)
- **Pump** (one per line, in removable cartridge)
- **Dispensing line** (one per line, silicone tube to faucet)

The pump is bidirectional (L298N H-bridge allows forward and reverse). Four 2-way NC solenoid valves per pump provide clean switching between fill and dispense modes. In idle state, all valves are closed, providing complete isolation of all fluid paths with zero power draw.

### 1b. Per-Pump Valve Arrangement

Each pump has an inlet and an outlet. Each side needs to connect to one of two sources/destinations. This is a 2x2 switching problem:

**Pump inlet connects to:**
- Hopper (during fill mode -- pump pulls from hopper) via inlet valve A
- Bag (during dispense mode -- pump pulls from bag) via inlet valve B

**Pump outlet connects to:**
- Bag (during fill mode -- pump pushes into bag) via outlet valve A
- Dispensing line (during dispense mode -- pump pushes to faucet) via outlet valve B

With 2-way NC valves, each switching point needs 2 valves (one per path), giving 4 valves per pump, 8 total. The NC property means all paths are sealed when de-energized, providing a true "all off" idle state that 3-way valves cannot achieve.

### 1c. Full Fluid Topology -- ASCII Schematic

```
                    HOPPER FUNNEL
                    (single, shared)
                         │
                    ┌────┴────┐
                    │         │
                   V1        V5
              (hopper→P1)  (hopper→P2)
                    │         │
    ┌───── V2 ─────┤         ├───── V6 ─────┐
    │  (bag1→P1)   │         │  (bag2→P2)   │
    │              │         │              │
   BAG 1      P1 INLET   P2 INLET      BAG 2
    │                                       │
    │          P1 OUTLET  P2 OUTLET         │
    │              │         │              │
    │   ┌── V3 ───┤         ├─── V7 ──┐    │
    │   │(P1→disp)│         │(P2→disp)│    │
    │   │         │         │         │    │
    │   D1       V4        V8        D2   │
    │  (disp   (P1→bag1) (P2→bag2)  (disp │
    │  line 1)    │         │     line 2)  │
    │             │         │              │
    └─────────────┘         └──────────────┘
```

### 1d. Detailed Per-Line Schematic (Flavor 1)

```
                HOPPER
                  │
                 V1 (NC: normally closed) ── inlet valve A
                  │
                  ├─────── V2 (NC: normally closed) ── inlet valve B ── BAG 1 (via dip tube)
                  │                                                        │
              P1 INLET                                                     │
                  │                                                        │
              [ PUMP 1 ]  (in cartridge)                                   │
                  │                                                        │
              P1 OUTLET                                                    │
                  │                                                        │
                  ├─────── V4 (NC: normally closed) ── outlet valve A ─────┘
                  │
                 V3 (NC: normally closed) ── outlet valve B
                  │
            DISPENSING LINE 1
              (to faucet)
```

**Fill mode (hopper → pump → bag):**
- V1 OPEN (hopper to pump inlet)
- V2 CLOSED (bag isolated from pump inlet)
- V3 CLOSED (dispensing line isolated from pump outlet)
- V4 OPEN (pump outlet to bag)
- Pump runs forward (pulls from inlet, pushes to outlet)

**Dispense mode (bag → pump → dispensing line):**
- V1 CLOSED (hopper isolated)
- V2 OPEN (bag to pump inlet)
- V3 OPEN (pump outlet to dispensing line)
- V4 CLOSED (bag isolated from pump outlet)
- Pump runs forward

**Idle mode (all closed, zero power):**
- V1-V4 all CLOSED (NC default)
- Pump OFF
- All fluid paths sealed -- no backflow, no drip, no cross-contamination
- Flavor syrup stays primed in tubing up to the faucet without draining back

### 1e. Valve Truth Table

All valves are normally closed (NC). OPEN = energized. Pump: FWD = forward, OFF = stopped.

| Mode | V1 | V2 | V3 | V4 | V5 | V6 | V7 | V8 | P1 | P2 | Notes |
|------|----|----|----|----|----|----|----|----|----|----|-------|
| **Idle** | C | C | C | C | C | C | C | C | OFF | OFF | All closed, no flow, zero power draw |
| **Fill bag 1** | O | C | C | O | C | C | C | C | FWD | OFF | Hopper→P1→Bag1 |
| **Fill bag 2** | C | C | C | C | O | C | C | O | OFF | FWD | Hopper→P2→Bag2 |
| **Dispense flavor 1** | C | O | O | C | C | C | C | C | FWD | OFF | Bag1→P1→D1 |
| **Dispense flavor 2** | C | C | C | C | C | O | O | C | OFF | FWD | Bag2→P2→D2 |
| **Dispense both** | C | O | O | C | C | O | O | C | FWD | FWD | Both lines active |
| **Flush line 1** | C | O | O | C | C | C | C | C | FWD | OFF | Same as dispense; clean water pre-loaded in bag |
| **Flush line 2** | C | C | C | C | C | O | O | C | OFF | FWD | Same as dispense; clean water pre-loaded in bag |

**Key observations:**
- Idle draws zero power (all valves NC, all pumps off). This is the safe default state.
- Fill and dispense modes never overlap: you never fill and dispense the same line simultaneously.
- During any active mode, exactly 2 valves are energized per active pump.
- Maximum simultaneous valve load: 4 valves (dispense both).
- The "all closed" idle state is the fundamental advantage of 2-way NC valves -- it prevents backflow, keeps fluid primed at the faucet, and handles power loss, firmware crash, or error states safely.

---

## 2. Why 8 Two-Way NC Valves (Not Fewer)

### 2a. The "All Closed" Requirement

The system needs three distinct valve states:
1. **Fill mode:** hopper→pump→bag path open, dispense path closed
2. **Dispense mode:** bag→pump→faucet path open, fill path closed
3. **Idle/off:** ALL paths closed -- no flow in any direction

State 3 is critical. It prevents:
- Gravity-driven backflow from bags through idle pumps
- Syrup draining away from the dispensing line (losing the primed state)
- Cross-contamination between hopper and bag lines during idle
- Uncontrolled flow during firmware crashes, power loss, or error conditions

A 3-way valve toggles between port A and port B but has no "off" position. De-energized, it connects to port A. Energized, it connects to port B. There is always an open path. This means a 3-way valve architecture has no true idle state -- one fluid path is always connected, even when the pump is off.

### 2b. 3-Way Valve Analysis (Considered and Rejected)

A 3-way solenoid valve has one common port and two selectable ports. De-energized, the common port connects to port A. Energized, the common port connects to port B. Each pump junction needs to select between two paths, so a 3-way valve at each junction could theoretically replace a pair of 2-way valves:

| Junction | 3-Way Valve | De-energized (Port A) | Energized (Port B) |
|----------|-------------|----------------------|---------------------|
| P1 inlet | 3W-1 | Bag 1 (dispense mode) | Hopper (fill mode) |
| P1 outlet | 3W-2 | Dispensing line 1 (dispense mode) | Bag 1 (fill mode) |
| P2 inlet | 3W-3 | Bag 2 (dispense mode) | Hopper (fill mode) |
| P2 outlet | 3W-4 | Dispensing line 2 (dispense mode) | Bag 2 (fill mode) |

This would reduce valve count from 8 to 4, with the de-energized state being "dispense ready" (bag→pump→faucet always connected when power is off).

**Why this was rejected:**

1. **No "all closed" idle state.** With 3-way valves de-energized, the bag-to-faucet path is always open. If the pump is off, there is no flow, but the path is not sealed. Gravity or pressure differentials could cause slow seepage. The fluid column between the bag and faucet is not locked in place -- it can drain or shift.

2. **Backflow vulnerability.** If the soda water line has any back-pressure (e.g., carbonation pressure, flow transient when main soda valve opens), that pressure can push back through the always-open dispense path into the bag. With 2-way NC valves, the closed outlet valve blocks this entirely.

3. **Priming loss.** The dispensing line must stay primed with flavor syrup right up to the injection point at the faucet. An always-open path allows the syrup column to slowly drain back toward the bag under gravity (the bag is below the faucet in the under-sink installation). NC valves on both sides of the pump lock the fluid column in place.

4. **Error state safety.** During error conditions (sensor fault, firmware watchdog, unexpected state), the safest position is "everything closed, nothing flows." With 2-way NC valves, killing power achieves this instantly. With 3-way valves, killing power leaves the dispense path open.

5. **Single-port bag problem.** Each bag has one fluid connection (the dip tube). With 3-way valves, the bag connects to both the inlet valve (port A, for dispense pull) and the outlet valve (port B, for fill push). This requires a TEE at the bag connector, adding dead volume (~9ml per line) and a potential stale-fluid zone that does not exist with the 2-way architecture where each valve has its own independent connection.

### 2c. 3-Way Valves with Center-Off: Do They Exist?

A 3-way valve with a center-off position (all three ports closed when de-energized) would solve the idle-state problem. This would give: position A (fill path), position B (dispense path), and center position (all closed).

**Research findings (March 2026):**

3-way 3-position solenoid valves with a true center-off (all ports blocked) are not a standard product category in the small/cheap valve market. Web searches across Amazon, industrial suppliers, and valve manufacturers consistently show:

- **3-way 2-position** is the standard configuration. These toggle between A and B with a single solenoid. No center position exists. This is what Burkert, Parker, Beduan, U.S. Solid, and all major small-valve manufacturers sell.

- **4-way 3-position center-closed** valves exist (e.g., MettleAir 4V330C series) but these are pneumatic directional control valves designed for cylinder actuation. They have 5 ports (2 work ports, 1 pressure, 2 exhaust), require dual solenoids (one per direction), are large (~140mm long), expensive (~$30-55 each), use NPT threaded ports (not barb/quick-connect), are made of aluminum (not food-safe for syrup contact), and are designed for air, not liquid. Using 4 of these would cost $120-220, require 8 GPIOs (dual solenoid), and need NPT-to-barb adapters on every port.

- **Proportional or servo-type 3-position valves** exist in industrial process control but start at $100+ per valve and are far too large and complex.

**Conclusion:** A cheap, compact, food-safe, 1/4" barb, 12V DC, 3-way valve with center-off position that is available overnight on Amazon Prime does not exist. The 2-way NC valve is the only architecture that provides the required "all closed" idle state using readily available, inexpensive, food-compatible components.

### 2d. Architecture Comparison Summary

| Configuration | Valve Count | Idle State | Dispense Power | Fill Power | Cost (total) | Availability |
|---------------|-------------|------------|----------------|------------|-------------|--------------|
| **8x 2-way NC (chosen)** | **8** | **All closed (zero power)** | **2 valves (~10W)** | **2 valves (~10W)** | **$48-80** | **Amazon Prime, overnight** |
| 4x 3-way 2-position | 4 | Dispense path open (no off) | 0 valves (0W) | 2 valves (~8W) | $32-60 | Available but wrong behavior |
| 4x 3-way 3-position center-off | 4 | All closed (requires dual solenoid) | Complex | Complex | Not available in required form | Does not exist as standard product |
| 4x 4-way 3-pos center-closed (pneumatic) | 4 | All closed | Dual solenoid per valve | Dual solenoid | $120-220 | Available but wrong form factor |

---

## 3. Valve Selection: Specific Components

### 3a. Requirements

All 8 valves must meet:
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
- This variant may be better suited for the pump-inlet valves (V2, V6) where the pump creates suction rather than the water supply providing pressure

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

**Candidate 4: Beduan 12V 1/4" Inlet Water Solenoid Valve**
- ASIN: B07NWCQJK9
- Type: 2-way NC, 12V DC
- Fittings: 1/4" quick-connect
- Power: 5.5W rated
- Working pressure: 0.02-0.8 MPa
- Working temperature: up to 60C (140F)
- Package dimensions: ~215 x 60 x 34mm (valve itself smaller)
- Weight: ~100g
- Materials: Metal and plastic, designed for RO systems
- Rating: 4.2/5 stars (143 ratings)
- Price: ~$6-9 (check current listing)
- Prime eligible: Yes
- Notes: Beduan is a well-known valve brand on Amazon with a wide product line.

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

These solenoid valves draw 4.8-5.5W continuously while energized. The coils will get warm (up to 60C per manufacturer specs). During dispense, 2 valves are energized for the duration of the pour (typically 5-15 seconds). During fill, 2 valves are energized for 3-5 minutes. Neither duty cycle is a thermal concern. The valves spend the vast majority of their time de-energized (idle/NC state) at zero power draw.

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

**Verdict:** Pinch valves would work with the 8-valve architecture and have excellent food safety (no fluid contact). However, they are less common in 12V DC form, harder to source on Amazon Prime, and the tubing at the pinch point can fatigue over many cycles. They remain a viable alternative if the RO-style solenoid valves prove problematic with sugar syrup.

### 4b. 3-Way Solenoid Diaphragm Valves

Available from Burkert, Parker, Beduan, and generic Amazon/AliExpress vendors. Typically $8-20 each ($15-35 for food-grade models). Would reduce valve count to 4 but lack the center-off idle state (see Section 2b-2c for detailed rejection rationale).

### 4c. Solenoid Ball Valves (2-way and 3-way)

$15-40 per valve, larger (~30x30x60mm), slower response (50-200ms). Overkill for this application's low flow rates (~5-400 ml/min) and pressures (~2-8 PSI).

### 4d. Motorized Ball Valves

$10-25 per valve, zero holding power (motor drives to position then holds mechanically), but slow switching (1-5 seconds). A 5-second delay between pressing "dispense" and getting soda is a poor user experience. Not recommended.

### 4e. Latching Solenoid Valves

$20-50 per valve. Zero holding power (permanent magnet holds position, brief pulse to switch). Ideal on paper but rare in food-grade configurations at this price point. A latching 2-way NC valve would be the best of all worlds if sourced affordably. Not currently available in the required form factor on Amazon.

---

## 5. Physical Dimensions and Placement

### 5a. Enclosure Space Budget

Interior: 272W x 292D x 392H mm (V1-A compact layout).

The 8-valve design requires more physical space than 4 valves but the valves are individually small. Typical RO solenoid valve dimensions are approximately 50-55mm long x 30-35mm diameter (cylindrical body with quick-connect ports on each end).

### 5b. Fitting 8 Two-Way Valves

**Option A: Four per side of cartridge (4 left for P1, 4 right for P2)**

- Left group (P1 valves V1-V4): X=0-60, Y=100-165, Z=0-60. Four valves in a 2x2 grid at ~30mm pitch each = 60x60mm footprint, ~55mm tall.
- Right group (P2 valves V5-V8): X=212-272, Y=100-165, Z=0-60. Same arrangement.
- These zones are adjacent to the cartridge dock and accessible when the cartridge is removed.

**Option B: All eight in two rows along the dock back wall**

- Two rows of 4, spanning Y=130-165, Z=0-60, X=30-230 (4 valves x 50mm pitch = 200mm per row).
- Places all valves in one serviceable zone behind the cartridge.

**Option C: Two groups of 4, stacked vertically**

- Each group has 4 valves stacked in a 2x2 arrangement, one group per side.
- Minimizes floor footprint at the cost of height.

**Recommendation for prototyping: Option A (four per side).** Groups each pump's valves together for clear tube routing. Individual valves are easy to replace during development.

### 5c. Valve Mounting

RO solenoid valves typically have bracket clips or can be zip-tied/cable-clamped to a mounting surface. For prototyping, use 3D-printed valve cradles screwed to the enclosure floor. Orient valves with quick-connect ports facing toward their respective tube connections (bags rearward, hopper upward, dispensing line forward).

---

## 6. Tube Routing

### 6a. Tube Segment Map (Per Flavor Line)

With 2-way NC valves, each valve has 2 ports (in and out). Each fluid path gets its own dedicated valve and tube run. No TEE junctions are needed at the bag connector -- the bag's single dip tube connects to a short manifold or splits to two separate valve ports via individual tubes.

**Flavor Line 1 (P1):**

| Seg | From | To | Length (est.) | Tube ID |
|-----|------|----|---------------|---------|
| T1 | Hopper funnel outlet | V1 inlet (hopper→pump valve) | ~350mm | 4.5mm |
| T2 | V1 outlet | P1 inlet (dock fitting) | ~75mm | 6.35mm |
| T3 | Bag 1 dip tube connector | V2 inlet (bag→pump valve) | ~150mm | 6.35mm |
| T4 | V2 outlet | P1 inlet (dock fitting, via TEE with T2) | ~80mm | 6.35mm |
| T5 | P1 outlet (dock fitting) | V3 inlet (pump→dispense valve) | ~75mm | 6.35mm |
| T6 | V3 outlet | Dispensing line exit (to faucet) | ~160mm | 6.35mm |
| T7 | P1 outlet (dock fitting, via TEE with T5) | V4 inlet (pump→bag valve) | ~80mm | 6.35mm |
| T8 | V4 outlet | Bag 1 dip tube connector (via TEE with T3) | ~150mm | 6.35mm |

**Note on pump-side TEEs:** The pump has a single inlet tube and single outlet tube. Two valves connect to each side. This requires a TEE or Y-connector at the pump inlet (joining V1 output and V2 output) and another at the pump outlet (splitting to V3 input and V4 input). Because only one valve on each side is ever open at a time, there is no flow conflict. Similarly, the bag has a single dip tube connection, so bag-side valves V2 (inlet from bag) and V4 (outlet to bag) share a TEE at the bag connector.

**Flavor Line 2 (P2): Mirror image on the right side of the enclosure, using V5-V8.**

### 6b. Dead Volume Estimate

| Per Line | Approximate Dead Volume |
|----------|------------------------|
| Dispensing path (bag→V2→TEE→pump→TEE→V3→faucet) | ~15-18ml |
| Fill path (hopper→V1→TEE→pump→TEE→V4→bag) | ~16-20ml |

Dead volume is comparable to the 3-way valve topology since both architectures require TEEs at the pump connections. The 2-way architecture adds valve internal volume (small) but eliminates the bag-side TEE that the 3-way design requires.

---

## 7. Cartridge Interface

### 7a. Cartridge Fluid Connections

The cartridge contains 2 pumps, each with an inlet and outlet tube stub. The valve architecture is entirely in the enclosure.

| Connection | Cartridge Side | Enclosure Side |
|------------|---------------|----------------|
| P1 inlet | Tube stub from pump 1 inlet | TEE joining V1 and V2 outputs |
| P1 outlet | Tube stub from pump 1 outlet | TEE splitting to V3 and V4 inputs |
| P2 inlet | Tube stub from pump 2 inlet | TEE joining V5 and V6 outputs |
| P2 outlet | Tube stub from pump 2 outlet | TEE splitting to V7 and V8 inputs |

**4 fluid connections total.** CPC quick-disconnect fittings at the dock back wall provide tool-free insertion/removal with auto-shutoff when disconnected.

### 7b. Cartridge Insertion Sequence

1. User slides cartridge into dock
2. CPC fittings click and engage -- fluid paths sealed
3. Pogo pins make electrical contact (pump motor connections)
4. Firmware detects cartridge (MCP23017 input) and runs a self-test
5. Self-test: briefly open each valve pair and run each pump to verify flow

---

## 8. Failure Modes

### 8a. Valve Failure Analysis

With 2-way NC valves, a failure means either the valve is stuck closed (fails to open when energized) or stuck open (fails to close when de-energized).

| Failure | Impact During Dispense | Impact During Fill | Impact During Idle | Severity |
|---------|----------------------|-------------------|-------------------|----------|
| V1 stuck closed | None (V1 not used in dispense) | Fill fails: no hopper→pump path | None (NC is the idle state) | Medium |
| V1 stuck open | Hopper connected to pump inlet during dispense; air ingestion if hopper empty | Fill works normally | Hopper path not sealed; potential slow drain | High |
| V2 stuck closed | Dispense fails: no bag→pump path | None (V2 not used in fill) | None | High |
| V2 stuck open | Dispense works but bag path is always open | Bag not isolated during fill; recirculation risk | Bag path not sealed; potential backflow | High |
| V3 stuck closed | Dispense fails: no pump→faucet path | None (V3 not used in fill) | None | High |
| V3 stuck open | Dispense works but faucet path always open | Pump output leaks to faucet during fill | Faucet path not sealed; drip risk | High |
| V4 stuck closed | None (V4 not used in dispense) | Fill fails: no pump→bag path | None | Medium |
| V4 stuck open | Pump output leaks to bag during dispense | Fill works normally | Bag path not sealed | Medium |

### 8b. Safe Default State

All 8 valves de-energize to closed (NC). If power fails or firmware crashes:
- All valves close (spring return to NC)
- Pumps stop (no power = no flow)
- All fluid paths sealed
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

| Mode | Valves Energized | Valve Power | Pump Power | Total System |
|------|-----------------|-------------|------------|-------------|
| Idle | 0 | 0W | 0W | ~1W (ESP32 + displays) |
| Dispense 1 line | 2 (V2+V3 or V6+V7) | ~10W | ~5W | ~16W |
| Dispense both | 4 (V2+V3+V6+V7) | ~20W | ~10W | ~31W |
| Fill 1 line | 2 (V1+V4 or V5+V8) | ~10W | ~5W | ~16W |

### 9b. PSU Sizing

Peak draw is during dual dispense: ~31W. A 12V/3A (36W) power supply handles all modes with headroom. If a 12V/2A (24W) supply is preferred, simultaneous dual-line dispense would need to be software-limited (dispense one line at a time, or stagger slightly).

**Comparison to 3-way architecture:** The 3-way design draws 0W valve power during dispense (a clear advantage), but the 2-way architecture's dispense power draw of 10-20W is manageable for the short duration of a pour (5-15 seconds). The valves are only energized during active pump operation, not continuously.

### 9c. Annual Energy Cost

Assuming 10 dispenses/day at 10 seconds each, plus 1 fill/week at 5 minutes:
- Dispense valve energy: 10W x 10s x 10/day x 365 = 0.10 kWh/year
- Fill valve energy: 10W x 300s x 52/week = 0.04 kWh/year
- Total valve energy: ~0.14 kWh/year (negligible)

---

## 10. GPIO and Driver Requirements

### 10a. MCP23017 I2C Expander Control

Each 2-way NC solenoid valve needs 1 GPIO output (energize/de-energize) and 1 MOSFET driver circuit. The 8 valves are controlled via MCP23017 I2C I/O expander connected to the ESP32.

| Resource | Count | Source |
|----------|-------|--------|
| GPIO outputs | 8 | MCP23017 (GPB0-GPB7) |
| MOSFET drivers | 8 | IRLZ44N + 10k gate resistor + 100k pulldown + 1N4007 flyback diode, per valve |
| 12V switched power | 8 channels | From 12V rail through MOSFETs |

The MCP23017 has 16 GPIO pins in two 8-bit ports (GPA0-7, GPB0-7). Using all 8 GPB pins for valve control. GPA pins handle other functions (cartridge detection, status LEDs, etc.).

### 10b. Updated MCP23017 Pin Plan

| MCP Pin | Direction | Function | Priority |
|---------|-----------|----------|----------|
| GPA0 | Output | Status LED green | Medium |
| GPA1 | Output | Status LED amber | Medium |
| GPA2 | Input | Cartridge detection switch | High |
| GPA3 | Input | Lever position sensor | Medium |
| GPA4 | -- | Unassigned | -- |
| GPA5 | -- | Unassigned | -- |
| GPA6 | -- | Unassigned | -- |
| GPA7 | -- | Unassigned | -- |
| GPB0 | Output | V1: P1 inlet valve A (hopper→pump) | High |
| GPB1 | Output | V2: P1 inlet valve B (bag→pump) | High |
| GPB2 | Output | V3: P1 outlet valve B (pump→dispense) | High |
| GPB3 | Output | V4: P1 outlet valve A (pump→bag) | High |
| GPB4 | Output | V5: P2 inlet valve A (hopper→pump) | High |
| GPB5 | Output | V6: P2 inlet valve B (bag→pump) | High |
| GPB6 | Output | V7: P2 outlet valve B (pump→dispense) | High |
| GPB7 | Output | V8: P2 outlet valve A (pump→bag) | High |

**Assigned: 12 of 16 pins. Free: 4 pins (GPA4-7).**

### 10c. Firmware Valve Control

Valve control via MCP23017 is straightforward:
- I2C write to GPB register sets all 8 valve states simultaneously
- Mode changes are atomic: write one byte to switch from idle→dispense or idle→fill
- MCP23017 I2C address: 0x20 (A0=A1=A2=GND)
- I2C bus: shared with FDC1004 capacitive sensor and any other I2C peripherals
- Valve state byte examples:
  - Idle: `0b00000000` (all closed)
  - Dispense line 1: `0b00000110` (V2+V3 open)
  - Dispense line 2: `0b01100000` (V6+V7 open)
  - Dispense both: `0b01100110` (V2+V3+V6+V7 open)
  - Fill line 1: `0b00001001` (V1+V4 open)
  - Fill line 2: `0b10010000` (V5+V8 open)

---

## 11. Clean Water Integration

### 11a. Cleaning Path

The previous topology had dedicated clean solenoids for injecting tap water into bags. With the 8-valve architecture, clean water can enter through the hopper path:

1. Pour clean water into the hopper funnel
2. Run fill mode (V1+V4 open for line 1, or V5+V8 for line 2)
3. Pump pushes clean water from hopper through pump into bag
4. Switch to dispense mode (V2+V3 or V6+V7) and run pump to flush the dispense line

Alternatively, a dedicated clean solenoid can be added on the hopper feed line (controlled by a spare GPA pin) to connect tap water directly without pouring into the hopper. This uses the same fill-mode valve configuration.

---

## 12. Decision Summary

### 12a. Resolved

| Decision | Resolution | Rationale |
|----------|-----------|-----------|
| Valve architecture | 8 two-way NC solenoid valves (4 per pump) | Only architecture providing "all closed" idle state with cheap, available components |
| 3-way valves | Rejected | No center-off position; always leaves one path open; 3-position variants don't exist in required form |
| Valve type | 2-way NC solenoid, RO water style, 12V DC, 1/4" quick-connect | Food-grade, $6-8 each, Amazon Prime overnight, proven in water systems |
| Primary candidate | DIGITEN B016MP1HX0 or Beduan B07NWCQJK9 | Both ~$7, food quality, 12V DC, 1/4" QC, widely available |
| GPIO control | MCP23017 I2C expander, GPB0-GPB7 | All 8 valves on one port, single-byte state changes, preserves ESP32 native GPIOs |
| Safe default | All valves NC (closed), all pumps off | Power loss = everything sealed, no flow possible |

### 12b. Open Questions

| Question | Impact | Next Step |
|----------|--------|-----------|
| Sugar syrup compatibility | Valve seal longevity with sticky residue | Order DIGITEN + Beduan, run 30-day soak test with actual syrup |
| Zero-pressure variant needed? | Pump-inlet valves see suction, not pressure | Test both standard (B016MP1HX0) and zero-pressure (B076KFCPGM) DIGITEN variants |
| Quick-connect vs. barb fitting | Tubing OD compatibility | Verify 1/4" silicone tubing OD fits push-connect; order barb adapters as backup |
| PSU sizing for dual dispense | 31W peak may exceed 24W supply | Size PSU at 36W (12V/3A) or limit to single-line dispense in firmware |
| Valve mounting in enclosure | Physical layout of 8 valves (vs. previous 4-valve plan) | Update master spatial layout to accommodate 4 valves per side |

---

## References

- [requirements.md](../requirements.md) -- Enclosure functional requirements including valve description
- [pump-assisted-filling.md](pump-assisted-filling.md) -- Previous fill topology analysis (TEE-based, with Option 1 recommended)
- [v1-master-spatial-layout.md](v1-master-spatial-layout.md) -- Component positions including allocated valve zones
- [v1-hopper-integration.md](v1-hopper-integration.md) -- Hopper funnel design, fill routing, two-flavor management
- [gpio-planning.md](../../../gpio-planning.md) -- MCP23017 pin assignments, MOSFET driver circuits
