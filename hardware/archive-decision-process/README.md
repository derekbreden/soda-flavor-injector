# Decision Points

This folder exists because multiple research agents independently explored design options, and their conclusions got treated as "design decisions" by downstream documents — when they were really recommendations from individual research threads.

**Nothing in the research folders should be treated as decided until it's been formally chosen here.**

## The 4 Key Decision Points

These are ordered by dependency — each one constrains the ones below it.

| # | Decision | Current Assumption | Status |
|---|----------|--------------------|--------|
| [DP1](dp1-enclosure-dimensions.md) | Enclosure outer dimensions | 280 x 250 x 400mm | **UNDECIDED** — treated as locked by dimensions-reconciliation.md but never formally chosen |
| [DP2](dp2-bag-strategy.md) | Bag type, size, and mounting | 1L Platypus, 18-20° incline | **UNDECIDED** — driven entirely by DP1's assumed 400mm height |
| [DP3](dp3-cartridge-dock-interface.md) | Cartridge-to-dock interface mechanism | Eccentric cam lever + release plate | **UNDECIDED** — research recommendation, not a decision |
| [DP4](dp4-fluid-path-topology.md) | Fluid path topology and filling method | Pump reversal fill + 6 solenoids | **UNDECIDED** — partially contradictory across docs |

## Dependency Chain

```
DP1 (Enclosure Dimensions)
 └─► DP2 (Bag Strategy)
      └─► Zone heights, dock position, cartridge slot height
           └─► DP3 (Cartridge Interface) ◄── mostly independent of DP1/DP2
                └─► Mating face, lever, alignment, workflow
 └─► DP4 (Fluid Path) ◄── somewhat independent, but hopper placement depends on DP1
      └─► Valve count, plumbing complexity, firmware, BOM
```

DP3 and DP4 are mostly independent of each other, but both depend on DP1/DP2 for spatial constraints.

## Visions

The [`visions/`](visions/) folder contains conceptual layouts — not decisions, not even formal options, but "what if the whole approach were different?" explorations that challenge the research's foundational assumptions.

- **[Diagonal Interleave](visions/diagonal-interleave.md)** — Bags stretch diagonally corner-to-corner through the enclosure instead of occupying a horizontal zone. Components share vertical ranges at different depths. Challenges the "horizontal zone" architecture that every research document assumes without questioning.

## The Deepest Unquestioned Assumption

Before reading the conflicts list below, understand this: **every research document assumes a "horizontal zone" architecture** — components stacked in horizontal layers. This is never questioned, never justified, and it artificially constrains every spatial calculation. The bags don't need to be "below" the dock. The electronics don't need to be "above" the cartridge. Components can interleave diagonally, share vertical space at different depths, and nest into each other's geometry. See the [assumption audit](assumption-audit.md) and [diagonal interleave vision](visions/diagonal-interleave.md) for details.

## Known Conflicts Across Research Documents

1. **README.md says 2L bags** — all recent research assumes 1L. Nobody formally decided.
2. **Drip tray removal** — drip-tray-shelf-analysis.md recommends it; other docs treat it as done. If bags leak, there's no containment.
3. **pump-assisted-filling.md explicitly corrects hopper-and-bag-management.md** — but both present their findings as conclusions.
4. **Hopper location** is described differently across documents (top of enclosure vs. front-top corner).
5. **Cam lever + release plate** is built upon by ~8 documents as if decided, but it's a research recommendation.
6. **Dimensions-reconciliation.md says "locked"** for values that were never formally approved.
7. **1L bags treated as forced by 400mm height** — but incline/diagonal mounting could fit 2L bags in the same or shorter enclosure. The constraint is the zone assumption, not the physics.

## How to Use These Documents

1. Start with the [assumption audit](assumption-audit.md) to understand what's actually open
2. Browse [visions/](visions/) for alternative architectural approaches
3. Read each DP document (they're self-contained)
4. Note that DP1 and DP2 may be more independent than they appear — diagonal/incline bag mounting can decouple bag size from enclosure height
5. DP3 and DP4 can be decided in either order
6. Each DP doc lists which research documents need updating after the decision
