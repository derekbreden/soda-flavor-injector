# Decision Point 1: Enclosure Outer Dimensions

**Status:** PENDING DECISION
**Date opened:** 2026-03-24
**Decision maker:** Human
**Blocking:** This is the single most depended-upon decision in the project. At least 15 downstream documents, every zone height calculation, component fit check, bag choice, cartridge slot position, and under-cabinet clearance number cascades from this.

---

## Why This Decision Exists

Multiple research documents have converged on **280W x 250D x 400H mm** as the enclosure size, but this was never formally decided. It emerged from one research thread's constraint that the injector should be "subordinate" in height to companion soda machines. The dimensions then propagated through subsequent documents as if locked.

Before building anything further on this foundation, the dimensions need to be a deliberate choice rather than an inherited assumption.

## Current State of the Codebase

- `dimensions-reconciliation.md` states 280W x 250D x 400H mm as "locked."
- `layout-spatial-planning.md` uses 280x250x400 throughout all zone calculations.
- `README.md` still references older or inconsistent dimensions in some areas.
- The 400mm height constraint originated from aesthetic subordination to soda machines, not from a formal engineering decision.

---

## Options

### Option A: 280W x 250D x 400H mm (Current Research Default)

Keep what the research documents already assume.

**Pros:**
- All recent research is built around this. No documents need updating.
- Fits comfortably under standard US 36-inch sink cabinets (usable height roughly 711-762mm, so 400mm is well within range).
- Supports 1L Platypus bags at 18-20 degree incline in a stacked vertical arrangement.
- Compact footprint is reasonable for under-sink placement alongside plumbing.

**Cons:**
- Forces 1L bags. 2L bags will not fit, meaning less concentrate capacity and more frequent refills.
- Vertical budget is tight: bag zone gets approximately 176mm, dock and valves get approximately 126mm, electronics get approximately 90mm.
- Lever clearance zone is only about 40mm, which may be tight for comfortable lever operation.
- No headroom for design iteration. If any component runs slightly larger than spec, there is no margin to absorb it.

**Downstream impacts:**
- 1L bags are locked in.
- Zone heights are fixed at current values with minimal tolerance.
- Cartridge slot position lands at approximately 226mm from the enclosure floor.

---

### Option B: 280W x 250D x 450H mm (+50mm taller)

Same footprint, 50mm more vertical space.

**Pros:**
- Could potentially accommodate 2L Platypus bags, eliminating the frequent-refill problem.
- Every zone gets more vertical headroom and looser tolerances.
- More lever clearance for ergonomic operation.
- More room in the electronics zone for wiring, connectors, or additional boards.
- Still fits under any standard US sink cabinet with substantial margin (450mm is roughly 18 inches against 28-30 inches of usable height).

**Cons:**
- All current zone calculations in research documents would need to be reworked.
- May be unnecessary if 1L bags prove sufficient for the use case.
- Larger enclosure means more material, more weight, and potentially harder positioning in a crowded under-sink cabinet.

**Downstream impacts:**
- Possibly enables 2L bags (needs verification against bag dimensions at incline).
- All zone budgets become more relaxed, reducing risk of fit problems.
- Cartridge slot height shifts upward; dock position calculations must be redone.

---

### Option C: 280W x 250D x 350H mm (-50mm shorter)

Same footprint, 50mm less vertical space. Prioritizes compactness.

**Pros:**
- Maximally compact. Easiest to place under a cabinet and leaves the most room for other under-sink items (garbage disposal, P-trap, cleaning supplies).
- Smallest material cost and weight.

**Cons:**
- Bag zone shrinks to roughly 126mm, which may not physically fit 1L bags at incline.
- All zones are compressed significantly. Electronics zone becomes marginal.
- Probably forces a fundamentally different bag mounting strategy (flat or horizontal rather than inclined).
- Very little room for any component tolerances or iteration.

**Downstream impacts:**
- Likely requires a complete redesign of the bag mounting approach.
- May force a different bag product entirely (not Platypus).
- Could cascade into different cartridge and dock geometry.

---

### Option D: Larger Footprint (e.g., 300W x 280D x 400H mm)

Keep the height at 400mm but expand width and/or depth.

**Pros:**
- More internal width could allow a larger cartridge envelope or a different pump arrangement.
- More depth provides tube routing space behind the dock.
- Could enable a stacked (vertical) pump arrangement instead of forced side-by-side placement.

**Cons:**
- Larger footprint under the sink. Under-sink space is constrained horizontally by plumbing, disposal units, and cabinet walls.
- All current cartridge envelope and dock research assumes 280x250. Width and depth changes ripple into those calculations.
- May conflict with available space next to P-traps and supply lines in real-world installations.

**Downstream impacts:**
- Potentially different cartridge envelope dimensions.
- Different pump arrangement options (stacked vs. side-by-side).
- Dock and bag geometry calculations need rework for the new internal volume.

---

## Key Questions Before Deciding

1. **Have you physically measured the available space in your specific under-sink cabinet?** The generic "standard US 36-inch cabinet" numbers give a range, but your actual installation site may have constraints (disposal unit, water filter, supply lines) that narrow the available envelope.

2. **How important is 2L bag capacity vs. compact size?** If the injector runs through 1L of a flavor in a week and swapping bags takes 30 seconds, frequent refills may be a non-issue. If it runs through 1L in two days, the refill burden matters more.

3. **Is the "subordinate to soda machine" height constraint actually important to you?** The 400mm height originated from an aesthetic goal. If the enclosure lives under a sink and is never seen next to the soda machine, this constraint may not matter.

4. **Would you rather have tight-but-works or comfortable-margins?** Option A works on paper with little room for error. Option B gives breathing room at the cost of reworking current documents. This is a risk-tolerance question.

---

## Documents That Must Be Updated After This Decision

Once dimensions are formally chosen, the following documents need to be reviewed and updated for consistency:

- `dimensions-reconciliation.md`
- `layout-spatial-planning.md`
- `bag-zone-geometry.md`
- `incline-bag-mounting.md`
- `dock-mounting-strategies.md`
- `cartridge-envelope.md` (indirectly, via dock position changes)
- `under-cabinet-ergonomics.md`
- `front-face-interaction-design.md`
- `back-panel-and-routing.md`
- `bill-of-materials.md` (material quantities change with enclosure size)
- `hopper-and-bag-management.md`

If Option A is chosen, most documents need no changes beyond adding a note that the dimensions are formally decided. Any other option triggers meaningful rework across the list above.
