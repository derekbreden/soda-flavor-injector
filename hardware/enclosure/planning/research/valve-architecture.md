# Valve Architecture: Flow Routing for Fill and Dispense Modes

This document defines the valve subsystem that routes fluid between the hopper, bags, pumps, and dispensing lines. Each of the 2 peristaltic pumps requires a set of valves to switch between two operating modes: filling a bag from the hopper, and dispensing flavor from a bag to the faucet. All valves are mounted in the main enclosure body. The removable pump cartridge contains only the 2 pumps.

---

## 1. Complete Fluid Schematic

### 1a. System Overview

Each flavor line has four fluid endpoints that connect through a single pump:

- **Hopper funnel** (shared between both lines, selected by valve)
- **Bag** (one per line, permanent, sealed with dip tube)
- **Pump** (one per line, in removable cartridge)
- **Dispensing line** (one per line, silicone tube to faucet)

The pump is bidirectional (L298N H-bridge allows forward and reverse). In the previous pump-assisted-filling research, TEE junctions and solenoid valves handled mode switching. This document replaces that topology with a dedicated valve architecture where 4 valves per pump provide clean switching between fill and dispense modes without TEE junctions.

### 1b. Per-Pump Valve Arrangement

Each pump has an inlet and an outlet. Each side needs to connect to one of two sources/destinations. This is fundamentally a 2x2 switching problem:

**Pump inlet connects to:**
- Bag (during dispense mode -- pump pulls from bag)
- Hopper (during fill mode -- pump pulls from hopper)

**Pump outlet connects to:**
- Dispensing line (during dispense mode -- pump pushes to faucet)
- Bag (during fill mode -- pump pushes into bag)

With 2-way (on/off) valves, each switching point needs 2 valves (one per path), giving 4 valves per pump, 8 total.

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
                 V1 (NC: normally closed)
                  │
                  ├─────── V2 (NC: normally closed) ──── BAG 1 (via dip tube)
                  │                                         │
              P1 INLET                                      │
                  │                                         │
              [ PUMP 1 ]  (in cartridge)                    │
                  │                                         │
              P1 OUTLET                                     │
                  │                                         │
                  ├─────── V4 (NC: normally closed) ────────┘
                  │
                 V3 (NC: normally closed)
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

---

## 2. Can It Be Done with Fewer Valves?

### 2a. The 8-Valve Baseline

The current design uses 8 two-way normally-closed solenoid valves. Each pump has 4: two on the inlet side (selecting hopper vs. bag) and two on the outlet side (selecting bag vs. dispensing line). This gives full independent control of both lines in all modes.

### 2b. Reduction to 4 Valves Using 3-Way Valves

A 3-way solenoid valve has one common port and two selectable ports. De-energized, the common port connects to port A. Energized, the common port connects to port B. This is exactly the switching needed at each junction.

Each pump has two junctions (inlet and outlet), each needing to select between two paths. A 3-way valve at each junction replaces a pair of 2-way valves:

| Junction | 3-Way Valve | De-energized (Port A) | Energized (Port B) |
|----------|-------------|----------------------|---------------------|
| P1 inlet | 3W-1 | Bag 1 (dispense mode) | Hopper (fill mode) |
| P1 outlet | 3W-2 | Dispensing line 1 (dispense mode) | Bag 1 (fill mode) |
| P2 inlet | 3W-3 | Bag 2 (dispense mode) | Hopper (fill mode) |
| P2 outlet | 3W-4 | Dispensing line 2 (dispense mode) | Bag 2 (fill mode) |

**Result: 4 three-way valves replace 8 two-way valves.**

Truth table with 3-way valves:

| Mode | 3W-1 | 3W-2 | 3W-3 | 3W-4 | P1 | P2 |
|------|------|------|------|------|----|----|
| **Idle** | A (bag) | A (disp) | A (bag) | A (disp) | OFF | OFF |
| **Fill bag 1** | B (hopper) | B (bag) | A | A | FWD | OFF |
| **Fill bag 2** | A | A | B (hopper) | B (bag) | OFF | FWD |
| **Dispense flavor 1** | A (bag) | A (disp) | A | A | FWD | OFF |
| **Dispense flavor 2** | A | A | A (bag) | A (disp) | OFF | FWD |
| **Dispense both** | A (bag) | A (disp) | A (bag) | A (disp) | FWD | FWD |

This is the breakthrough: **dispense mode is the de-energized default state.** No valves are powered during the most common operating mode (dispensing). Valves only energize during fill mode, which happens once a week for a few minutes. This dramatically reduces cumulative power draw and valve wear.

### 2c. Reduction Below 4 Valves?

**3 valves:** Not possible without losing a degree of freedom. Each pump has two independent switching junctions. You need at least one switch per junction, and there are 4 junctions total (2 pumps x 2 junctions). With 3 valves, one junction is permanently wired, meaning one pump can only fill or only dispense -- not both.

**2 valves:** Only works if the pump direction itself handles some routing. If the pump reverses direction in fill mode, the inlet becomes the outlet and vice versa. Then you only need one valve per pump to select the source/destination. But this creates a problem: in reverse mode, the pump would pull from the dispensing line (not the hopper) unless there is a valve blocking it. You still need a second valve or check valve per pump to prevent incorrect flow paths. Two valves plus two check valves might work, but the check valves add restriction and failure modes.

**1 valve:** Not possible. Cannot independently control two flavor lines with a single valve.

### 2d. Minimum Valve Count Summary

| Configuration | Valve Count | Active During Dispense | Active During Fill | Complexity | Recommendation |
|---------------|-------------|----------------------|-------------------|------------|----------------|
| 8x 2-way NC | 8 | 2-4 energized | 2 energized | High: 8 valves, 8 drivers, 8 GPIOs | Baseline, not recommended |
| 4x 3-way | 4 | **0 energized** | 2 energized | Moderate: 4 valves, 4 drivers, 4 GPIOs | **Strongly recommended** |
| 2x 3-way + 2x check | 4 total (2 active) | 0 energized | 2 energized | Lower: 2 valves + 2 passive check valves | Worth exploring but adds check valve restriction |

**Recommendation: 4 three-way solenoid valves.** This halves the valve count, halves the GPIO and driver requirements, and achieves zero power draw during the most common mode (dispensing). The 8-valve approach using 2-way valves is unnecessarily complex.

### 2e. Rotary Selector Valves

A 4-port rotary valve could replace a pair of 3-way valves for each pump -- one rotary valve selects the full inlet+outlet configuration. However:

- Rotary valves are typically motorized (slow: 1-5 seconds to switch) or manual
- Food-grade rotary valves in this size are expensive ($30-80 each)
- A single rotary valve failure takes out an entire pump's routing
- 3-way solenoids switch in 10-50ms, cost $8-15 each
- Not recommended for this application

---

## 3. Valve Type Selection

### 3a. Candidate Valve Types

The system needs 4 three-way solenoid valves (or 8 two-way, if the 3-way approach is rejected). All valves must be food-safe for contact with sugar syrups at room temperature.

### 3b. Solenoid Pinch Valves (2-way only)

**How they work:** An electromagnetic solenoid squeezes a section of silicone tubing shut. The fluid never contacts the valve body -- it stays inside the tubing. De-energized, a spring holds the tube open (normally open, NO) or closed (normally closed, NC).

| Parameter | Value |
|-----------|-------|
| Cost | $5-15 per valve |
| Size | ~20x20x40mm typical |
| Power (holding) | 2-5W per valve (continuous while energized) |
| Response time | 10-30ms |
| Food safety | Excellent -- no fluid contact with valve internals |
| Reliability | Good for low-cycle applications; tubing fatigue at pinch point over millions of cycles |
| Noise | Audible click on actuation |
| 3-way available? | **No.** Pinch valves are inherently 2-way (tube open or closed) |

**Verdict:** Pinch valves are attractive for food safety but cannot provide 3-way switching. If using pinch valves, the 8-valve (2-way) architecture is required. This means 4 valves energized during dispense (if NC type), which is 8-20W continuous draw during every glass of soda. Or use NO type and accept that idle state leaves all paths open (unsafe).

### 3c. 3-Way Solenoid Valves (Diaphragm or Poppet Type)

**How they work:** A solenoid shifts an internal poppet or diaphragm to connect a common port to one of two output ports. De-energized = port A connected. Energized = port B connected.

| Parameter | Value |
|-----------|-------|
| Cost | $8-20 per valve (food-grade models: $15-35) |
| Size | ~25x25x50mm typical for 1/4" port size |
| Power (holding) | 2-6W per valve (continuous while energized) |
| Response time | 15-50ms |
| Food safety | Depends on materials -- must specify EPDM/silicone diaphragm, PP/PTFE/316SS body. FDA-compliant models available |
| Reliability | Excellent; diaphragm valves rated for millions of cycles |
| Noise | Audible click, slightly louder than pinch valves |
| 3-way available? | **Yes -- this is the primary 3-way option** |

**Specific candidates:**

- **Burkert Type 0124 / 0330:** Industrial 3-way, available in food-grade materials. ~$25-50. Overkill quality but proven.
- **Parker 3-way miniature:** ~$15-30. PP body, EPDM seals.
- **Generic 1/4" 3-way 12V solenoid (Amazon/AliExpress):** $8-15. Brass or PP body. Food-grade seals may need verification. Widely available.
- **Beduan 3-way 12V solenoid:** $9-14. Same vendor as existing NC solenoids in the project. PP body available.

**Verdict:** Best option for the 4-valve architecture. The fluid does contact internal surfaces (diaphragm, valve body), so material selection matters. PP body with EPDM or silicone diaphragm is food-safe for sugar syrups.

### 3d. Solenoid Ball Valves (2-way and 3-way)

| Parameter | Value |
|-----------|-------|
| Cost | $15-40 per valve |
| Size | ~30x30x60mm -- larger than diaphragm valves |
| Power (holding) | 3-8W per valve |
| Response time | 50-200ms (slower due to ball mass) |
| Food safety | Good -- 316SS ball, PTFE seats. Proven in food/beverage |
| Reliability | Excellent; long life, handles particulates well |
| Noise | Louder click than diaphragm valves |
| 3-way available? | Yes (L-port and T-port configurations) |

**Verdict:** Overkill for this application. The flow rates (~5-400 ml/min) and pressures (~2-8 PSI) are far below ball valve capabilities. The larger size and higher cost are disadvantages.

### 3e. Motorized Ball Valves

| Parameter | Value |
|-----------|-------|
| Cost | $10-25 per valve |
| Size | ~40x40x60mm -- large |
| Power (holding) | **0W** -- motor drives to position, then holds mechanically |
| Power (switching) | 2-5W for 1-5 seconds during actuation |
| Response time | 1-5 seconds (slow) |
| Food safety | Same as solenoid ball valves |
| Reliability | Good; gear wear over thousands of cycles |
| Noise | Motor whine during switching (1-5 seconds) |
| 3-way available? | Yes |

**Verdict:** The zero holding power is attractive, but the slow switching speed (1-5 seconds) and large size are significant disadvantages. A 5-second delay between pressing "dispense" and getting soda is a poor user experience. Could work for fill mode (where switching speed is irrelevant) but not for dispense.

### 3f. Latching Solenoid Valves

| Parameter | Value |
|-----------|-------|
| Cost | $20-50 per valve (niche, limited availability) |
| Size | Similar to standard solenoid valves |
| Power (holding) | **0W** -- permanent magnet holds position |
| Power (switching) | Brief pulse (~50-200ms) at 3-8W to switch |
| Response time | 15-50ms |
| Food safety | Same as standard solenoid diaphragm valves |
| Reliability | Good; permanent magnet does not degrade |
| Noise | Single click per switch |
| 3-way available? | Rare, but some models exist |

**Verdict:** Ideal on paper -- fast switching, zero holding power, food-safe options. The main drawback is limited availability in 3-way food-grade configurations at this price point. A latching 3-way valve would be the best of all worlds but may require sourcing from industrial suppliers at higher cost ($30-60 per valve).

### 3g. Pneumatic Pinch Valves

Not viable. Requires a compressed air supply, which adds an air pump, reservoir, and regulator. The complexity is far out of proportion to the benefit.

### 3h. Comparison Summary

| Valve Type | Count Needed | Cost (total) | Power (dispense) | Power (fill) | Size Impact | Food Safe | Speed | Recommendation |
|------------|-------------|-------------|------------------|-------------|-------------|-----------|-------|----------------|
| 2-way NC pinch | 8 | $40-120 | 8-20W (4 valves) | 4-10W (2 valves) | 8x ~20x20x40mm | Excellent | Fast | Not recommended (power, count) |
| **3-way diaphragm** | **4** | **$32-60** | **0W** | **4-12W (2 valves)** | **4x ~25x25x50mm** | **Good (material dependent)** | **Fast** | **Recommended** |
| 3-way ball (solenoid) | 4 | $60-160 | 0W | 6-16W | 4x ~30x30x60mm | Good | Moderate | Overkill |
| 3-way motorized ball | 4 | $40-100 | 0W | 0W (pulse only) | 4x ~40x40x60mm | Good | Slow (1-5s) | Too slow for dispense |
| 3-way latching solenoid | 4 | $80-200 | 0W | 0W (pulse only) | 4x ~25x25x50mm | Good | Fast | Best but expensive/hard to source |

**Primary recommendation: 4x 3-way solenoid diaphragm valves**, 12V, 1/4" ports, PP body with EPDM/silicone seals. Total cost: ~$36-60. Zero power during dispensing. Moderate power during fill (a few minutes per week).

**Stretch goal:** If food-grade 3-way latching solenoids can be sourced under $25 each, they eliminate all holding power concerns entirely.

---

## 4. Physical Dimensions and Placement

### 4a. Enclosure Space Budget

Interior: 272W x 292D x 392H mm (V1-A compact layout).

Available zones for valve placement:
- **Front-bottom, left of cartridge (X=0-60, Y=100-160, Z=0-35):** The master spatial layout already allocates this zone for "solenoid valves (4x)" with dimensions ~30W x 60D x 35H each. Total zone: 60W x 60D x 35H.
- **Front-bottom, right of cartridge (X=212-272, Y=100-160, Z=0-35):** Allocated for hopper solenoids (2x) in the master layout.
- **Along dock back wall (Y=130-165, Z=0-84):** Behind the cartridge, accessible when cartridge is removed.

### 4b. Fitting 4 Three-Way Valves

A typical 3-way 12V solenoid valve (1/4" ports) measures approximately 25W x 25D x 50H mm. Four of these:

**Option A: Two per side of cartridge (2 left, 2 right)**

- Left pair (P1 valves): X=0-55, Y=100-155, Z=0-50. Two valves side by side at 25mm wide each = 50mm. Fits within the 60mm width.
- Right pair (P2 valves): X=217-272, Y=100-155, Z=0-50. Same arrangement.
- Total footprint: well within the zones already allocated in the master spatial layout.

**Option B: All four in a row along the dock back wall**

- Y=135-160, Z=0-50, spanning X=30-230 (4 valves x 50mm pitch = 200mm).
- Places all valves in one serviceable zone behind the cartridge.
- Accessible when cartridge is removed.

**Option C: Valve manifold block**

- A custom manifold (3D-printed or machined PP block) integrates all 4 valve bodies into a single block with internal channels.
- Eliminates individual tube connections between valve pairs; fluid routes through the manifold.
- Size: ~100W x 60D x 50H mm for a 4-valve manifold.
- Reduces tube count and potential leak points.
- More complex to manufacture; suited for production, not prototyping.

**Recommendation for prototyping: Option A (two per side).** Uses the zones already allocated in the master layout. Individual valves are easy to replace and re-plumb during development. Option C (manifold) is a production optimization.

### 4c. Valve Mounting

Each valve has two M3 or M4 mounting holes. Mount to the enclosure floor or interior wall using standoffs or a printed bracket. Valves should be oriented with ports facing rearward (toward the dock back wall) to minimize tube routing distance.

### 4d. Clearance to Other Components

| Valve Location | Adjacent Component | Clearance | Assessment |
|---------------|-------------------|-----------|------------|
| Left pair (X=0-55) | Cartridge (X=61-211) | 6mm lateral | Adequate |
| Left pair (Z=0-50) | Bag slab lower surface at Y=130 | ~160mm above valves (V1-A) | Very generous |
| Right pair (X=217-272) | Cartridge (X=61-211) | 6mm lateral | Adequate |
| All valves (Y=100-160) | Dock back wall (Y=130-165) | Adjacent/overlapping Y | Valves and dock wall share depth zone -- tubes route between them |
| All valves (Z=0-50) | Drip tray (Z=0-4) | Valves sit above drip tray | Mount on 5mm standoffs |

---

## 5. Tube Routing

### 5a. Tube Segment Map (Per Flavor Line)

With 3-way valves, each pump connects through 2 valves. Each valve has 3 ports (common, port A, port B). The tube segments are:

**Flavor Line 1 (P1):**

| Segment | From | To | Length (est.) | Tube ID | Dead Vol. |
|---------|------|----|---------------|---------|-----------|
| T1: Hopper drain to 3W-1 port B | Hopper funnel outlet (136, 50, 322) | 3W-1 port B (30, 130, 25) | ~350mm (vertical drop + lateral) | 4.5mm | 5.6ml |
| T2: Bag 1 connector to 3W-1 port A | Bag 1 dip tube cap (136, 258, 29) | 3W-1 port A (30, 130, 25) | ~150mm (along floor) | 6.35mm | 4.8ml |
| T3: 3W-1 common to P1 inlet | 3W-1 common (30, 140, 25) | Dock fitting (100, 135, 40) | ~75mm | 6.35mm | 2.4ml |
| T4: P1 outlet to 3W-2 common | Dock fitting (100, 135, 55) | 3W-2 common (30, 150, 25) | ~80mm | 6.35mm | 2.5ml |
| T5: 3W-2 port A to disp line 1 | 3W-2 port A (30, 155, 25) | Enclosure exit (30, 0, 25) | ~160mm | 6.35mm | 5.1ml |
| T6: 3W-2 port B to bag 1 connector | 3W-2 port B (30, 145, 25) | Bag 1 dip tube cap (136, 258, 29) | ~150mm | 6.35mm | 4.8ml |

Wait -- segments T2 and T6 both go to the bag 1 connector. The bag has a single dip tube with one external tube connection. With 3-way valves, the bag connects to the inlet valve (port A for dispense, pulling from bag) and the outlet valve (port B for fill, pushing to bag). These are two separate tube connections to the same bag.

**This is a problem.** The Platypus bag has a single threaded connector. One dip tube, one opening. You cannot connect two separate tubes to the same bag port.

### 5b. The Single-Port Bag Problem

The bag has exactly one fluid connection (the dip tube cap). Both fill mode (pump pushes into bag) and dispense mode (pump pulls from bag) use this same connection. The fluid path to/from the bag must be a single tube that branches.

**Solution: Add a TEE at the bag connector.** One tube from the bag connector splits to two valve ports:

```
                    BAG 1
                      │
                   dip tube
                      │
                    TEE-B1
                   /      \
          to 3W-1         to 3W-2
          port A          port B
        (inlet valve)   (outlet valve)
```

This TEE is not a routing decision point -- the valves control which branch is active. During dispense, 3W-1 port A is active (bag→pump inlet) and 3W-2 port B is closed. During fill, 3W-1 port A is closed (hopper→pump inlet instead) and 3W-2 port B is active (pump outlet→bag).

**Importantly, both branches of the TEE are never active simultaneously in the same direction.** The valves guarantee that flow either enters or exits the bag, never both. The TEE is passive.

### 5c. Revised Tube Segment Map (Per Flavor Line)

**Flavor Line 1 (P1):**

| Seg | From | To | Length | ID | Dead Vol. |
|-----|------|----|--------|----|-----------|
| T1 | Hopper funnel outlet | 3W-1 port B (inlet valve, fill path) | ~350mm | 4.5mm | 5.6ml |
| T2 | Bag 1 connector | TEE-B1 | ~20mm | 6.35mm | 0.6ml |
| T3 | TEE-B1 branch A | 3W-1 port A (inlet valve, dispense path) | ~140mm | 6.35mm | 4.4ml |
| T4 | TEE-B1 branch B | 3W-2 port B (outlet valve, fill path) | ~140mm | 6.35mm | 4.4ml |
| T5 | 3W-1 common | P1 inlet (dock fitting) | ~75mm | 6.35mm | 2.4ml |
| T6 | P1 outlet (dock fitting) | 3W-2 common | ~80mm | 6.35mm | 2.5ml |
| T7 | 3W-2 port A | Dispensing line exit | ~160mm | 6.35mm | 5.1ml |

**Flavor Line 2 (P2): Mirror image on right side of enclosure.**

| Seg | From | To | Length | ID | Dead Vol. |
|-----|------|----|--------|----|-----------|
| T8 | Hopper funnel outlet | 3W-3 port B | ~350mm | 4.5mm | 5.6ml |
| T9 | Bag 2 connector | TEE-B2 | ~20mm | 6.35mm | 0.6ml |
| T10 | TEE-B2 branch A | 3W-3 port A | ~140mm | 6.35mm | 4.4ml |
| T11 | TEE-B2 branch B | 3W-4 port B | ~140mm | 6.35mm | 4.4ml |
| T12 | 3W-3 common | P2 inlet (dock fitting) | ~75mm | 6.35mm | 2.4ml |
| T13 | P2 outlet (dock fitting) | 3W-4 common | ~80mm | 6.35mm | 2.5ml |
| T14 | 3W-4 port A | Dispensing line exit | ~160mm | 6.35mm | 5.1ml |

### 5d. Total Tube Length and Dead Volume

| Per Line | Length | Dead Volume |
|----------|--------|------------|
| Dispensing path (bag→TEE→T3→3W-1→T5→P1→T6→3W-2→T7→faucet) | T2+T3+T5+T6+T7 = 20+140+75+80+160 = **475mm** | 0.6+4.4+2.4+2.5+5.1 = **15.0ml** |
| Fill path (hopper→T1→3W-1→T5→P1→T6→3W-2→T4→TEE→bag) | T1+T5+T6+T4+T2 = 350+75+80+140+20 = **665mm** | 5.6+2.4+2.5+4.4+0.6 = **15.5ml** |
| Hopper-only segment (T1, not shared with dispense) | 350mm | 5.6ml |

| System Total | Value |
|-------------|-------|
| Total tube segments | 14 |
| Total tube length (both lines) | ~2 x 1025mm = **~2.1 meters** |
| Dispensing dead volume (both lines) | 2 x 15.0 = **30.0ml** |
| Hopper fill dead volume (both lines) | 2 x 15.5 = **31.0ml** |

### 5e. Dead Volume Discussion

The 15ml dispensing dead volume per line is higher than the previous TEE-based topology (~10.3ml per line in the master spatial layout). The increase comes from the bag TEE branches (T3 and T4) which add ~9ml per line that did not exist in the simpler topology.

**Mitigation strategies:**
- Use smaller ID tubing (4.5mm instead of 6.35mm) on the TEE branches. Reduces T3+T4 dead volume from 8.8ml to 5.6ml per line.
- Keep TEE-to-valve runs as short as possible: mount valves close to the bag connectors.
- Accept the dead volume: 15ml per line is ~1.5% of a 1L bag. At typical syrup-to-soda ratios (1:5), 15ml of syrup flavors 75ml of soda -- about one sip. The first dispense after idle flushes this volume.

---

## 6. Cartridge Interface Implications

### 6a. Cartridge Fluid Connections

The cartridge contains 2 pumps, each with an inlet and outlet tube stub. With the valve architecture fully in the enclosure, the cartridge interface remains simple:

| Connection | Cartridge Side | Enclosure Side |
|------------|---------------|----------------|
| P1 inlet | Tube stub from pump 1 inlet | Connects to 3W-1 common port |
| P1 outlet | Tube stub from pump 1 outlet | Connects to 3W-2 common port |
| P2 inlet | Tube stub from pump 2 inlet | Connects to 3W-3 common port |
| P2 outlet | Tube stub from pump 2 outlet | Connects to 3W-4 common port |

**4 fluid connections total.** Unchanged from previous assumptions. The valves are entirely on the enclosure side.

### 6b. Connection Type

CPC (Colder Products) quick-disconnect fittings at the dock back wall provide tool-free insertion/removal with auto-shutoff when disconnected. The auto-shutoff is particularly important with the valve architecture: when the cartridge is removed, the enclosure-side shutoff valves prevent any leaking from the valve network, and the cartridge-side shutoff valves prevent dripping from residual pump tube contents.

### 6c. Cartridge Insertion Sequence

1. User slides cartridge into dock
2. CPC fittings click and engage -- fluid paths sealed
3. Pogo pins make electrical contact (pump motor connections)
4. Firmware detects cartridge (MCP23017 input) and runs a self-test
5. Self-test: briefly energize each 3-way valve and run each pump to verify flow (bag weight or capacitive level sensing confirms)

---

## 7. Failure Modes

### 7a. Valve Failure Analysis

With 3-way valves, a failure means the valve is stuck in one position (A or B) and does not switch when energized/de-energized.

| Failure | Stuck Position | Impact During Dispense | Impact During Fill | Severity |
|---------|---------------|----------------------|-------------------|----------|
| 3W-1 stuck at A (bag) | Port A always connected | Dispense works normally (A is the dispense path) | Fill fails: pump pulls from bag instead of hopper. Bag contents recirculate. No syrup enters bag. | Medium: fill inoperative but dispense works |
| 3W-1 stuck at B (hopper) | Port B always connected | Dispense fails: pump pulls from hopper (may be empty/dry) instead of bag. Air ingestion likely. | Fill works normally (B is the fill path) | High: dispense inoperative |
| 3W-2 stuck at A (disp) | Port A always connected | Dispense works normally (A is the dispense path) | Fill fails: pump pushes to dispensing line instead of bag. Syrup goes to faucet. | High: syrup waste, user confusion |
| 3W-2 stuck at B (bag) | Port B always connected | Dispense fails: pump pushes back into bag instead of dispensing line. No output at faucet. | Fill works normally (B is the fill path) | High: dispense inoperative |

### 7b. Worst-Case Scenarios

**3W-2 stuck at A during fill attempt:** The pump pushes hopper syrup straight to the dispensing line. Undiluted syrup appears at the faucet. Detectable by: unexpected flow at dispensing line during fill mode (flow meter or drip sensor), and no weight/level change in the target bag.

**3W-1 stuck at B during dispense:** Pump pulls from hopper instead of bag. If hopper is empty (usual state), pump runs dry, ingests air. Detectable by: capacitive air-in-line sensor (FDC1004), abnormal pump current draw (no load = lower current).

### 7c. Safe Default State

All 3-way valves de-energize to position A (bag-to-pump-to-dispense path). If power fails or firmware crashes:
- Pumps stop (no power = no flow)
- Valves return to dispense-ready position
- No hopper connection (hopper path requires energized valve)
- No unintended flow (pumps are off)

This is a safe state. The only risk is if a pump somehow continues to run with valves in an unexpected state, which requires both a firmware fault AND a hardware driver fault (L298N latch-up). Adding a hardware watchdog that kills the 12V pump rail on timeout provides defense in depth.

### 7d. Detection Methods

| Method | Detects | Hardware Needed | Already Planned? |
|--------|---------|-----------------|-----------------|
| FDC1004 capacitive sensing | Air in lines, empty bag, hopper empty | FDC1004 (4 channels, I2C) | Yes -- already in GPIO plan |
| Flow meter on soda line | Dispense duration mismatch (flavor pump running but no correlated soda flow) | Flow meter | Yes -- already installed |
| Pump current monitoring | Dry-run detection (pump with no fluid load draws less current) | Current sense resistor + ADC | No -- would need ADC input |
| Bag weight/strain gauge | Bag level tracking, fill confirmation | Load cell under bag cradle | No -- possible future addition |

### 7e. Recommended Safety Interlocks

1. **Fill mode timeout:** If fill runs longer than expected (e.g., 10 minutes for a 1L bag), abort and alert. Prevents infinite loop if valve is stuck and bag is not filling.
2. **Dispense flow correlation:** If pump runs for >2 seconds with no flow meter activity on the soda line, pause and alert. Catches dry-run or misdirected flow.
3. **Post-fill verification:** After fill completes, briefly switch to dispense mode and check that the FDC1004 detects fluid (not air) in the dispense tube segment. Confirms valves switched back correctly.

---

## 8. Power Budget

### 8a. Valve Power Draw by Architecture

| Architecture | Dispense (most common) | Fill (weekly, ~5 min) | Idle |
|-------------|----------------------|---------------------|------|
| 8x 2-way NC solenoid | 4 valves x 3W = **12W** | 2 valves x 3W = **6W** | **0W** |
| 4x 3-way solenoid | **0W** (all de-energized) | 2 valves x 4W = **8W** | **0W** |
| 4x 3-way latching | **0W** | **~0W** (pulse only) | **0W** |

### 8b. System Power Budget (3-Way Solenoid Architecture)

| Component | Idle | Dispense (1 line) | Dispense (2 lines) | Fill |
|-----------|------|-------------------|--------------------|----|
| Valves | 0W | 0W | 0W | 8W (2 valves) |
| Pump(s) | 0W | 5W (1 pump) | 10W (2 pumps) | 5W (1 pump) |
| ESP32 + peripherals | 0.5W | 0.5W | 0.5W | 0.5W |
| Displays (2x) | 0.5W | 0.5W | 0.5W | 0.5W |
| **Total** | **1W** | **5.5W** | **11W** | **13.5W** |

### 8c. PSU Sizing

Peak draw is during fill mode: ~13.5W. Dispense (both lines) draws ~11W. An existing 12V/2A (24W) power supply handles all modes with comfortable headroom. The 3-way architecture avoids the 12W valve overhead that would push the 8-valve design to 23W during dispense.

### 8d. Comparison: Holding Current vs. Pulse-Only

The 3-way solenoid approach draws 8W during fill mode, but fill happens once a week for ~5 minutes. Annual energy cost of fill-mode valve holding: 8W x 5min x 52 weeks = 0.035 kWh/year. Negligible. The latching solenoid option eliminates this entirely but at 2-3x the cost per valve. The standard solenoid is the pragmatic choice.

---

## 9. GPIO and Driver Requirements

### 9a. With 4 Three-Way Valves

Each 3-way solenoid valve needs 1 GPIO output (energize/de-energize) and 1 MOSFET driver circuit (same as the hopper solenoid MOSFET circuit already designed in the GPIO planning document).

| Resource | Count | Source |
|----------|-------|--------|
| GPIO outputs | 4 | MCP23017 (GPB0-GPB3 are free) |
| MOSFET drivers | 4 | IRLZ44N + 10k gate + 100k pulldown + 1N4007 flyback, per valve |
| 12V switched power | 4 channels | From 12V rail through MOSFETs |

The MCP23017 has 10 unassigned pins (GPA6-7, GPB0-7). Using GPB0-GPB3 for the 4 valve control signals leaves 6 pins free. No additional I/O expander needed.

### 9b. Compared to 8 Two-Way Valves

| Resource | 4x 3-way | 8x 2-way |
|----------|----------|----------|
| GPIOs | 4 | 8 |
| MOSFET drivers | 4 | 8 |
| MCP23017 pins used | 4 of 10 free | 8 of 10 free (uses nearly all remaining) |
| Wiring complexity | 12 wires (4 signal + 4 power + 4 ground) | 24 wires |

The 3-way architecture is clearly better for GPIO and driver economy.

### 9c. Updated MCP23017 Pin Plan

| MCP Pin | Direction | Function | Priority |
|---------|-----------|----------|----------|
| GPA0 | Output | Hopper solenoid valve flavor 1 (SV-H1) | High |
| GPA1 | Output | Hopper solenoid valve flavor 2 (SV-H2) | High |
| GPA2 | Input | Cartridge detection switch | High |
| GPA3 | Input | Lever position sensor | Medium |
| GPA4 | Output | Status LED green | Medium |
| GPA5 | Output | Status LED amber | Medium |
| GPA6 | -- | Unassigned | -- |
| GPA7 | -- | Unassigned | -- |
| GPB0 | Output | 3-way valve 3W-1 (P1 inlet) | High |
| GPB1 | Output | 3-way valve 3W-2 (P1 outlet) | High |
| GPB2 | Output | 3-way valve 3W-3 (P2 inlet) | High |
| GPB3 | Output | 3-way valve 3W-4 (P2 outlet) | High |
| GPB4 | -- | Unassigned | -- |
| GPB5 | -- | Unassigned | -- |
| GPB6 | -- | Unassigned | -- |
| GPB7 | -- | Unassigned | -- |

**Assigned after valve addition: 12 of 16 pins. Free: 4 pins.**

**Note on hopper solenoids (GPA0-1):** With 3-way valves handling the fill/dispense routing, the hopper solenoid valves (SV-H1, SV-H2) from the previous design may be redundant. In the previous topology, hopper solenoids controlled whether the hopper connected to the pump circuit via TEE2. In the new valve architecture, 3W-1/3W-3 already control whether the pump inlet connects to the hopper or the bag. The hopper solenoids would be a second valve in series on the hopper line, providing defense-in-depth against backflow. Whether to keep them depends on risk tolerance:

- **Keep them:** Double isolation of the hopper line. If a 3-way valve leaks in position B, the NC hopper solenoid still blocks backflow. Cost: 2 extra valves, 2 GPIOs, 2 MOSFETs.
- **Remove them:** Simplify the system. The 3-way valve alone isolates the hopper. If the 3-way valve seals well, the hopper solenoid is redundant. Saves ~$18 + 2 GPIOs.

**Recommendation:** Remove the dedicated hopper solenoids for the prototype. The 3-way valve on each pump inlet provides the hopper isolation function. If testing reveals backflow concerns, add them back. This frees GPA0-1 for other uses or keeps them as reserve.

---

## 10. Integration with Existing Topology

### 10a. What This Architecture Replaces

The previous pump-assisted-filling research (Option 1) used this topology per line:

```
Previous: Bag ← TEE1 ← [clean sol] ← water
                 ↓
           [disp solenoid]
                 ↓
              [pump]
                 ↓
               TEE2
              /    \
     [hopper sol]  [check valve]
         ↓              ↓
      hopper      dispensing point
```

The new valve architecture replaces TEE1, TEE2, the dispensing solenoid, the hopper solenoid, and the check valve with 2 three-way valves plus 1 bag-side TEE:

```
New:     Bag ← TEE-B1 ← 3W-1 port A
                  ↓                ↓ (common)
            3W-2 port B        P1 inlet
                  ↓
             3W-2 common
                  ↓
             P1 outlet
                  ↓
            3W-2 port A → dispensing line

            3W-1 port B ← hopper
```

### 10b. What About the Clean Solenoids?

The previous topology had a clean solenoid at TEE1 for injecting tap water into the bag during cleaning. In the new architecture, there is no TEE1 -- the bag connects through TEE-B1 to the valve network.

**Clean water injection options:**

1. **Through the hopper:** Pour clean water into the hopper funnel, run fill mode. Water enters through the same path as syrup. Simple but requires the user to pour water.
2. **Dedicated clean water port on the bag TEE:** Add a 4th port to TEE-B1 (making it a cross fitting) with a clean solenoid and needle valve connection to tap water. This preserves automatic cleaning.
3. **Clean water through a valve port:** Connect the tap water line to one of the 3-way valve ports (e.g., 3W-1 port B could serve double duty for both hopper and clean water via an upstream TEE). The clean solenoid and hopper share the fill-mode inlet path.

**Recommendation: Option 3.** Add a TEE on the hopper/fill path between the hopper line and 3W-1 port B. The clean solenoid connects to this TEE. In fill mode, the hopper valve opens and clean solenoid stays closed. In clean mode, the clean solenoid opens and hopper valve stays closed. Same 3-way valve position (B) serves both fill and clean modes.

```
Clean water integration:

         hopper ← [hopper valve NC]
                        ↓
                    TEE-FILL
                        ↓
tap water → [clean sol NC] → TEE-FILL → 3W-1 port B
```

This preserves automatic cleaning without additional valves in the main routing path.

---

## 11. Decision Points and Open Questions

### 11a. Resolved

| Decision | Resolution | Rationale |
|----------|-----------|-----------|
| 8 vs 4 valves | 4 three-way valves | Fewer parts, zero dispense power, simpler wiring |
| Valve type | 3-way solenoid diaphragm (12V, 1/4" ports, PP/EPDM) | Best balance of cost, speed, size, food safety |
| Valve placement | Two per side of cartridge dock, in existing allocated zones | Uses space already identified in master spatial layout |
| GPIO source | MCP23017 GPB0-3 | Adequate latency for on/off control, preserves ESP32 native GPIOs |
| Safe default state | All valves de-energized = dispense-ready, pumps off = no flow | Power loss results in safe idle |
| Hopper solenoids | Remove for prototype; 3-way valves provide isolation | Simplifies system; can add back if backflow is observed |

### 11b. Open Questions

| Question | Impact | Next Step |
|----------|--------|-----------|
| Specific 3-way valve model and sourcing | Cost, lead time, port sizing | Order 2-3 candidate valves for bench testing (Beduan, generic PP 3-way, Parker miniature) |
| TEE-B1 dead volume acceptable? | Adds ~9ml per line vs previous topology | Bench test with colored water to visualize stale-zone flushing |
| Clean water integration path | Affects tube routing near 3W-1/3W-3 | Prototype the TEE-FILL junction, verify clean water flow rate through the valve path |
| Valve seal quality with sugar syrups | Sticky residue could impede diaphragm sealing over time | Run a 30-day sugar syrup endurance test on the selected valve model |
| Latching solenoid availability | Could eliminate all holding power | Search for 3-way latching solenoids in 1/4" food-grade; if found under $25/unit, switch |

---

## References

- [requirements.md](../requirements.md) -- Enclosure functional requirements including valve description
- [pump-assisted-filling.md](pump-assisted-filling.md) -- Previous fill topology analysis (TEE-based, with Option 1 recommended)
- [v1-master-spatial-layout.md](v1-master-spatial-layout.md) -- Component positions including allocated valve zones
- [v1-hopper-integration.md](v1-hopper-integration.md) -- Hopper funnel design, fill routing, two-flavor management
- [gpio-planning.md](../../../gpio-planning.md) -- MCP23017 pin assignments, MOSFET driver circuits
