# Decision Point 4: Fluid Path Topology and Filling Method

**Status:** Open -- awaiting decision
**Depends on:** DP1 (Enclosure Dimensions), DP2 (Bag Strategy)
**Blocks:** Solenoid count and placement, BOM cost, hopper design, clean cycle implementation, firmware control logic, GPIO pin allocation, back panel routing

## Why This Matters

The fluid path topology is the plumbing architecture of the entire system. It determines:

- **How many solenoid valves are needed**, which is the single largest variable cost item in the BOM (~$10 each, currently 6 listed).
- **Whether bags can be refilled in-place** or must be physically removed, which affects day-to-day usability.
- **How cleaning works**, which affects hygiene, flavor contamination between switches, and maintenance burden.
- **Firmware complexity**, because each valve and pump direction combination is a distinct state that must be sequenced correctly.
- **Hopper design**, because some options require a hopper and others eliminate it entirely.

Every solenoid added means another GPIO pin, another driver circuit, another potential leak point, and another thing to debug.

## Current State

The research documents describe three distinct fluid operations -- dispensing, cleaning, and refilling -- but they were explored independently and sometimes contradict each other.

- `plumbing.md` describes the basic dispensing path: bag -> dip tube -> pump (forward) -> solenoid -> output nozzle. The clean cycle is described as "planned" but is not implemented in firmware.
- `pump-assisted-filling.md` argues that pump reversal through the sealed dip tube is viable for refilling bags, directly contradicting earlier conclusions in `hopper-and-bag-management.md` that dismissed pump-assisted fill.
- `hopper-and-bag-management.md` originally explored gravity fill and pump-assisted fill as separate concepts. Parts of this document were invalidated by `pump-assisted-filling.md`.
- `dip-tube-analysis.md` confirmed the Platypus Drink Tube Kit creates a sealed path, proven by existing priming operations. This is the foundation for any in-place refill approach.
- `bill-of-materials.md` lists 6 solenoid valves (2 dispensing, 2 clean, 2 hopper) with the hopper solenoids marked as "NEEDED" despite the hopper system being unbuilt and untested.
- `README.md` describes only the basic dispensing path. Clean and refill paths are not documented there.

### Conflicts Between Documents

| Conflict | Documents involved |
|----------|-------------------|
| Whether pump-assisted fill is viable | `hopper-and-bag-management.md` says no; `pump-assisted-filling.md` says yes |
| Total solenoid count and placement | `bill-of-materials.md` says 6; exact topology is scattered across multiple docs |
| Check valve vs. solenoid valve at specific points | Differs between `plumbing.md` and `pump-assisted-filling.md` |
| Fill percentage via pump reversal | `pump-assisted-filling.md` estimates 85-95%, but this is theoretical and untested |
| Clean cycle implementation | `plumbing.md` describes it as planned; no firmware exists for it |

---

## Options

### Option A: Full Topology -- Pump Reversal Fill + Solenoid Clean

This is the direction the most recent research documents point toward. All three fluid operations are automated.

**Dispensing:** bag -> dip tube -> pump (forward) -> dispensing solenoid -> output nozzle
**Cleaning:** water supply -> clean solenoid -> pump (forward) -> bag path (flushes lines) -> drain
**Refilling:** hopper -> hopper solenoid -> pump (reversed) -> dip tube -> bag

**Hardware:**
- 6 solenoid valves (2 dispensing, 2 clean, 2 hopper)
- Check valve at dispensing output to prevent air ingress during refill
- Sealed hopper with one-way air inlet valve, one per flavor
- Tee fittings to branch the fluid path at pump inlet/outlet

| Pros | Cons |
|------|------|
| Fastest refill (pump-assisted, estimated 5-12 min per 1L) | Most complex plumbing (6 solenoids, check valves, tees) |
| Bags never need removal for refill | Most complex firmware (pump direction + valve state machine for 3 modes) |
| Self-contained operation (pour concentrate, press button) | Pump reversal sealing integrity is unproven with peristaltic pumps |
| Sealed hopper minimizes air exposure to concentrate | 85-95% fill means 5-15% residual air in bag (needs testing) |
| Automated clean cycle | Air management during fill is theoretical (multi-cycle fill-and-vent) |
| | Highest BOM cost (~$60 in solenoids alone) |
| | Most leak points and failure modes |

**Key unknowns:** Peristaltic pump reversal has not been tested. The 85-95% fill estimate and multi-cycle fill-and-vent strategy are theoretical.

---

### Option B: Gravity Fill via Hopper + Solenoid Clean

Same dispensing and cleaning as Option A, but refilling uses gravity instead of pump reversal.

**Dispensing:** bag -> dip tube -> pump (forward) -> dispensing solenoid -> output nozzle
**Cleaning:** water supply -> clean solenoid -> pump (forward) -> bag path -> drain
**Refilling:** hopper -> gravity -> dip tube -> bag (pump not involved)

**Hardware:**
- 4 solenoid valves (2 dispensing, 2 clean)
- Hopper with manual valve or simple funnel-to-tube connection
- Hopper valves optional (could use manual pinch clamps)

| Pros | Cons |
|------|------|
| No pump reversal logic needed | Slow fill (gravity through dip tube, estimated 15-30+ min per 1L) |
| Fewer solenoids than Option A | Hopper must be above bag (constrains placement in enclosure) |
| Gravity is reliable and well-understood | Air escape during fill is harder without pump assistance |
| Automated clean cycle still included | May only fill to 70-80% due to trapped air |
| Simpler firmware than Option A | Fill rate depends on height differential (limited by enclosure) |

**Key unknowns:** Gravity flow rate through the dip tube has not been measured. Bag fill percentage without active air management is uncertain.

---

### Option C: Manual Bag Swap (No In-Place Refill)

Dispensing and cleaning are automated. Refilling is a manual operation performed outside the enclosure.

**Dispensing:** bag -> dip tube -> pump (forward) -> dispensing solenoid -> output nozzle
**Cleaning:** water supply -> clean solenoid -> pump (forward) -> bag path -> drain
**Refilling:** user removes bag, fills externally (open cap, pour, close), reinstalls

**Hardware:**
- 4 solenoid valves (2 dispensing, 2 clean)
- No hopper, no refill plumbing

| Pros | Cons |
|------|------|
| Simplest plumbing of any option with automated cleaning | Manual bag handling (disconnect, fill, reconnect) |
| No hopper design needed | User handles concentrate directly (sticky, staining) |
| No refill valves or pump reversal | Must design bag removal/installation to be easy and repeatable |
| Bags can be filled to 100% (open cap, pour, close) | Less "appliance-like" feel |
| Eliminates air management concerns during fill | Bag reconnection introduces potential for leaks or air in line |
| Lower BOM cost than A or B | |
| Automated clean cycle handles hygiene | |

**Key unknowns:** How easy is bag removal and reinstallation with the current incline mount clip system? Does disconnecting the dip tube path introduce air that requires re-priming?

---

### Option D: Dispensing Only -- Defer Clean and Refill

Minimum viable product. Only dispensing is automated. Cleaning is manual. Refilling is manual bag swap.

**Dispensing:** bag -> dip tube -> pump (forward) -> dispensing solenoid -> output nozzle
**Cleaning:** user manually flushes lines with water
**Refilling:** user removes bag, fills externally, reinstalls

**Hardware:**
- 2 solenoid valves (dispensing only, already owned per BOM)
- No clean solenoids, no hopper

| Pros | Cons |
|------|------|
| Validates core dispensing concept first | No automated cleaning (residue buildup, flavor contamination) |
| Only 2 solenoids (already in hand) | Manual cleaning is tedious and may not be thorough |
| Simplest possible firmware | Defers important problems that do not go away |
| Can add clean cycle and hopper later | Less hygienic over time without automated flush |
| Lowest cost and fastest path to working prototype | Two separate manual processes (clean + refill) |

**Key unknowns:** How quickly does concentrate residue become a hygiene or flavor contamination problem without automated cleaning?

---

### Option E: Bag-in-Cartridge (Replaceable Cartridge Includes Bags)

Bags are integrated into or attached to the cartridge. Swapping the cartridge swaps the bags.

**Dispensing:** cartridge bag -> dip tube -> pump (forward) -> dispensing solenoid -> output nozzle
**Cleaning:** water supply -> clean solenoid -> pump (forward) -> old cartridge path -> drain (done during cartridge swap)
**Refilling:** swap entire cartridge (new flavor = new bags)

**Hardware:**
- 2-4 solenoid valves (dispensing + optional clean)
- Cartridge redesign to include bag mounting

| Pros | Cons |
|------|------|
| One-motion flavor change (new cartridge = new everything) | Cartridge becomes much larger and heavier (~2.8kg vs ~0.8kg with full bags) |
| Eliminates separate bag mounting system | Requires fundamentally different cartridge design |
| No in-place refill mechanism needed | Waste: discarding bags with residual concentrate when switching flavors |
| Clean break between flavors | 1L of liquid = 1kg, so handling weight is significant |
| | Bag dimensions must fit within or attach to cartridge envelope |
| | Conflicts with current cartridge research (pump + valve cartridge, not fluid storage) |

**Key unknowns:** Can the cartridge physically accommodate bags while remaining insertable? Does the added weight make cartridge insertion awkward or require a different latching mechanism?

---

## Comparison Summary

| Factor | Option A | Option B | Option C | Option D | Option E |
|--------|----------|----------|----------|----------|----------|
| Solenoid count | 6 | 4 | 4 | 2 | 2-4 |
| Automated refill | Yes (pump) | Yes (gravity) | No | No | No (swap) |
| Automated clean | Yes | Yes | Yes | No | Optional |
| Firmware complexity | High | Medium | Medium | Low | Low-Medium |
| Plumbing complexity | High | Medium | Medium | Low | Low |
| BOM cost (solenoids) | ~$60 | ~$40 | ~$40 | ~$20 | ~$20-40 |
| Unproven assumptions | Pump reversal, air mgmt | Gravity flow rate | Bag reinstall ease | Residue buildup rate | Cartridge redesign |
| Hopper required | Yes | Yes | No | No | No |

---

## Key Questions for the Decision Maker

1. **Is in-place refill a must-have for v1?** Options A and B provide it. Options C and D defer it. Option E replaces it with cartridge swapping.
2. **How much plumbing complexity are you comfortable prototyping?** Option A is the most complex. Option D is the simplest.
3. **Would you prefer a working MVP with manual processes, then layer in automation?** Option D gets dispensing working fastest. Option C adds automated cleaning. Options A/B add automated refill.
4. **How often do you plan to switch flavors?** Frequent switching makes automated cleaning more important (favors Options A, B, or C over D).
5. **Are you willing to test pump reversal before committing to Option A?** A bench test of pump reversal through the dip tube would resolve the biggest unknown. If it fails, Option A is off the table.
6. **Is the current cartridge design flexible enough to absorb bags?** If not, Option E requires a ground-up cartridge redesign.

---

## Documents That Must Be Updated After This Decision

- `plumbing.md` -- valve topology, clean cycle path, refill path
- `hopper-and-bag-management.md` -- hopper design or removal
- `pump-assisted-filling.md` -- confirm or retire pump reversal approach
- `dip-tube-analysis.md` -- refill path through dip tube (or not)
- `bill-of-materials.md` -- solenoid count, hopper components, check valves
- `gpio-planning.md` -- solenoid control pin allocation
- `back-panel-and-routing.md` -- water supply routing, drain routing
- `layout-spatial-planning.md` -- hopper placement, valve placement
- `bag-zone-geometry.md` -- updated if bags become part of cartridge (Option E)
- `README.md` -- system architecture description
