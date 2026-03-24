# Decision Point 2: Bag Type, Size, and Mounting Strategy

**Status:** Open -- awaiting decision
**Depends on:** DP1 (Enclosure Height)
**Blocks:** Dock shelf position, cartridge slot height, hopper design, drainage behavior, ergonomics

## Why This Matters

Bag configuration is the second most depended-upon decision in the system. It determines:

- **Bag zone height**, which sets the dock shelf position, which sets the cartridge slot height, which determines ergonomics.
- **Refill frequency**, which affects how practical the system is day-to-day.
- **Hopper design**, because hopper capacity must match bag capacity.
- **Drainage behavior**, because mounting angle and bag geometry control how completely the bag empties.

## Current State

The research documents are inconsistent on this topic:

- `README.md` still references 2L Platypus bags.
- `bag-zone-geometry.md` switched to 1L bags, driven by the 400mm enclosure height constraint.
- `incline-bag-mounting.md` established 18-20 degree incline as the mounting strategy.
- `hopper-and-bag-management.md` analyzed hopper refill cycles assuming 1L bags.
- `dip-tube-analysis.md` confirmed the sealed dip tube path works regardless of bag size.
- `drip-tray-shelf-analysis.md` recommended removing the drip tray, freeing floor space for bags.

The switch from 2L to 1L was driven by a single constraint (400mm enclosure height) that is itself undecided -- see DP1. This means the 1L assumption may not hold.

---

## Options

### Option A: 1L Platypus Bags, Incline-Mounted at 18-20 Degrees

This is what the current research documents assume. Two bags stacked vertically within a 176mm bag zone, each stretched between two fixed points (sealed end high, connector end low).

**Estimated dimensions:** ~250mm long, 140mm wide, 40mm thick when full.
**Bag zone:** 4-180mm from enclosure floor.
**Mounting hardware:** Binder clips, J-hooks, or U-clips (~$0.25/bag).

| Pros | Cons |
|------|------|
| Fits within 400mm enclosure | Only 1L per flavor (~50 drinks at 20ml/drink) |
| Incline provides controlled drainage toward dip tube | Physical bag dimensions are estimated, not measured |
| Controlled collapse (thinning, not random folding) | 18-20 degree sweet spot is geometry-derived, not physically tested |
| Cheap mounting hardware | Stacking two bags with ~5mm gap is tight |
| Two-point stretch prevents liquid trapping above folds | If actual bags are thicker than 40mm when full, stacking fails |

### Option B: 2L Platypus Bags, Vertical Hanging

The traditional hydration-system approach. Bags hang from the sealed end with the connector at the bottom. Requires a taller enclosure (~450mm+).

| Pros | Cons |
|------|------|
| 2x capacity = half the refill frequency (~100 drinks) | Requires taller enclosure (see DP1) |
| Simpler mounting (hang from clip at top) | Uncontrolled bag collapse creates random folds that trap liquid |
| Natural gravity drainage | May need elastic wrap or rigid cradle to manage collapse |
| More common approach in hydration systems | Bag swings or sways if enclosure is bumped |

### Option C: 1L Bags, Flat/Horizontal Cradle

Bags lay flat in a shaped cradle with the connector exiting from one end.

| Pros | Cons |
|------|------|
| Minimal vertical space required | Poor drainage (liquid pools in center, away from dip tube) |
| Bags cannot fall or shift | Bag collapse creates pockets that trap liquid |
| Simple mounting | Would need a tilted cradle anyway, which converges toward Option A |
| | No gravity assist for emptying |

### Option D: 2L Bags, Incline-Mounted

The same incline strategy as Option A but with larger bags. 2L Platypus bags are approximately 350mm long when full. **Crucially, incline mounting trades horizontal length for vertical rise** — a 350mm bag at 10° incline uses only ~60mm of vertical height, at 15° it's ~90mm. This means 2L bags may be viable even in a 400mm enclosure, especially if the drip tray is removed (freeing ~15mm of floor space). This possibility has not been explored by any existing research document.

A shallower angle than the 1L configuration would be needed. Whether drainage is adequate at 10-15° (vs. 18-20° for 1L) is an open question that needs physical testing with actual 2L bags.

| Pros | Cons |
|------|------|
| Combines incline drainage with larger capacity | 2L bag thickness when full is unknown — critical for stacking |
| Controlled collapse behavior | Heavier when full (~2kg per bag vs. ~1kg) |
| Fewer refills than Option A (~100 drinks vs ~50) | Shallower incline angle may reduce drainage effectiveness |
| **May fit in 400mm enclosure** — this needs research, not assumption | Mounting hardware must handle more weight |
| Could eliminate the primary reason 1L was chosen | Longer bag means wider mounting point spacing — must fit enclosure depth |
| Drip tray removal further improves feasibility | Bag thickness is the key unknown (must measure physical 2L bag) |

### Option E: External Bag Reservoir

Bags stored outside the enclosure in a separate container or bracket. Tubing runs into the enclosure. Any bag size could be used.

| Pros | Cons |
|------|------|
| Enclosure can be much shorter | Messy under-sink installation |
| Unlimited bag size options | Longer tube runs = more dead volume = more waste during cleaning |
| Easy to inspect and refill bags | Two separate things to install instead of one self-contained unit |
| | Tubing routing adds failure points |

---

## Key Questions

1. **Refill tolerance and target users:** A moderate user (24 cans/week) needs ~480ml/week of concentrate at 1:20 ratio. 2L lasts about a month; 1L lasts about two weeks. A heavy-use family could go through 1L in a week. What refill frequency is acceptable for the target market?
2. **Measured dimensions:** Do you have physical bags (both 1L and 2L) on hand to measure actual filled thickness? This is the single most critical unknown — stacking math depends on it.
3. **2L at incline — unexplored:** No existing research has calculated whether 2L bags at a shallower incline (10-15°) fit in the current 400mm zone budget. This is a research gap, not a known impossibility. Should this be explored before deciding?
4. **Self-containment:** Is everything-in-one-box important, or is an external reservoir acceptable?
5. **Coupling with DP1:** This decision and DP1 are linked — but less tightly than assumed, because incline mounting may decouple bag size from enclosure height. If 2L bags fit at incline in 400mm, DP1 and DP2 become more independent.

---

## Conflicts in the Current Documentation

These inconsistencies exist across the research documents and need to be resolved when this decision is made:

- **README vs. research docs:** README says 2L; all recent research docs assume 1L.
- **1L treated as decided:** `bag-zone-geometry.md` treats 1L as a settled decision, but it was driven by the 400mm height constraint, which is itself a research conclusion from DP1.
- **Incline was only explored for 1L bags:** `incline-bag-mounting.md` calculated geometry only for 1L bags at 18-20°. It never explored 2L bags at shallower angles, even though the incline approach is precisely what could make 2L bags viable in a 400mm enclosure. This is the biggest research gap in the bag strategy.
- **Drip tray removal + leak risk:** `drip-tray-shelf-analysis.md` recommended removing the drip tray. If bags leak, there is now no containment. This trade-off should be revisited once bag strategy is settled.
- **Usage math not present:** No document calculates actual concentrate consumption rates for target user profiles (moderate solo user, heavy family use) to justify bag size requirements. The 1L vs 2L choice should be driven by product requirements, not just what fits.

---

## Documents to Update After This Decision

The following files reference bag size, bag zone height, or mounting strategy and will need to be brought into alignment:

- `bag-zone-geometry.md` -- zone height calculations
- `incline-bag-mounting.md` -- mounting angle and hardware
- `layout-spatial-planning.md` -- zone heights and vertical stack
- `dimensions-reconciliation.md` -- overall dimension consistency
- `hopper-and-bag-management.md` -- hopper capacity and fill timing
- `dip-tube-analysis.md` -- tube length
- `drip-tray-shelf-analysis.md` -- containment strategy
- `dock-mounting-strategies.md` -- shelf position
- `under-cabinet-ergonomics.md` -- cartridge slot height
- `bill-of-materials.md` -- bag and hardware costs
- `README.md` -- project-level bag size reference
