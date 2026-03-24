# Diagonal Stacking Geometry: Comprehensive Analysis

Full angle/dimension sweep using CORRECTED bag dimensions. Previous research used 250mm length and 140mm width for 1L bags — the actual Platy 1L is 280mm × 152mm. This changes everything.

---

## CRITICAL FINDING: The Existing 18° Design Is Broken

The current `bag-zone-geometry.md` uses 250mm bag length. The actual Platy 1.0L is **280mm long**. At 18° incline:

- Depth consumed = 280 × cos(18°) + 50 × sin(18°) = 266 + 15 = **282mm**
- Interior depth available (250mm enclosure) = **242mm**
- **The bags don't fit. The current design is 40mm too deep.**

This is not a marginal miss. The entire existing layout — bag-zone-geometry, layout-spatial-planning, dimensions-reconciliation — is based on dimensions that are wrong by 30mm on bag length.

### Fixes for the Existing Design

| Fix | Enclosure Change | Angle | Depth Used | Height Used | Remaining Height |
|---|---|---|---|---|---|
| Deepen to 300mm | 280×300×400 | 18° | 282mm → fits in 292mm interior | 134mm | **258mm** — ample |
| Steepen to 30° | 280×250×400 | 30° | 268mm → still doesn't fit 242mm | 190mm | 202mm |
| Steepen to 42° | 280×250×400 | 42° | 242mm → exactly fits | 225mm | **167mm** — tight but viable |
| Both (deepen + steepen) | 280×300×400 | 35° | 258mm → fits in 292mm with 34mm margin | 212mm | **180mm** — good balance |

---

## Corrected Bag Dimensions Used Throughout

| Model | Width | Length | Compressed Thickness | Stack (2 bags) |
|---|---|---|---|---|
| Platy 1.0L | 152mm | 280mm | ~25mm | 50mm |
| SoftBottle 1.0L | 152mm | 330mm | ~20mm | 40mm |
| Platy 2.0L | 190mm | 350mm | ~40mm | 80mm |
| Hoser 2.0L | 152mm | 406mm | ~30mm est. | 60mm est. |

---

## Angle Sweep Tables

Formulas for two stacked bags at angle θ from horizontal:
- **Depth consumed** = L × cos(θ) + T_stack × sin(θ)
- **Height consumed** = L × sin(θ) + T_stack × cos(θ)

### Platy 1.0L (L=280mm, T_stack=50mm)

| Angle | Depth | Height |
|---|---|---|
| 18° | 282mm | 134mm |
| 25° | 275mm | 166mm |
| 30° | 268mm | 190mm |
| 35° | 258mm | 212mm |
| 40° | 247mm | 233mm |
| 42° | 242mm | 225mm |
| 45° | 233mm | 233mm |
| 50° | 218mm | 247mm |
| 55° | 202mm | 258mm |
| 60° | 183mm | 268mm |
| 65° | 164mm | 275mm |
| 70° | 143mm | 280mm |
| 75° | 121mm | 283mm |

### Platy 2.0L (L=350mm, T_stack=80mm)

| Angle | Depth | Height |
|---|---|---|
| 50° | 286mm | 320mm |
| 55° | 266mm | 333mm |
| 60° | 244mm | 343mm |
| 65° | 220mm | 351mm |
| 70° | 195mm | 356mm |
| 75° | 168mm | 359mm |

### Hoser 2.0L (L=406mm, T_stack=60mm est.)

| Angle | Depth | Height |
|---|---|---|
| 55° | 282mm | 367mm |
| 60° | 255mm | 382mm |
| 65° | 226mm | 393mm |
| 70° | 195mm | 402mm |

---

## Enclosure Dimension Sweep

### Platy 1.0L in Various Enclosures

Interior dimensions = exterior - 8mm per axis (4mm walls).

**In 280W × 250D × 400H (current assumption, 272×242×392 interior):**

| Angle | Depth | Fits? | Height | Remaining H | Cartridge+lever above? (need 120mm) | Electronics too? (+40mm) |
|---|---|---|---|---|---|---|
| 42° | 242mm | Exactly | 225mm | 167mm | ✅ (47mm spare) | ✅ tight (37mm) |
| 45° | 233mm | ✅ (9mm) | 233mm | 159mm | ✅ (39mm spare) | ✅ barely (29mm) |
| 50° | 218mm | ✅ (24mm) | 247mm | 145mm | ✅ (25mm spare) | Marginal (15mm) |
| 55° | 202mm | ✅ (40mm) | 258mm | 134mm | ✅ (14mm spare) | ❌ (4mm) |
| 60°+ | <183mm | ✅ | 268mm+ | <124mm | ❌ | ❌ |

**Sweet spot at 250mm depth: 42-45°.** Steeper than 50° crowds out the cartridge zone.

**In 280W × 300D × 400H (deeper enclosure, 272×292×392 interior):**

| Angle | Depth | Margin | Height | Remaining H | Cartridge+electronics above? |
|---|---|---|---|---|---|
| 18° | 282mm | 10mm | 134mm | **258mm** | ✅ abundant |
| 25° | 275mm | 17mm | 166mm | **226mm** | ✅ abundant |
| 30° | 268mm | 24mm | 190mm | **202mm** | ✅ good |
| 35° | 258mm | 34mm | 212mm | **180mm** | ✅ good (50mm for electronics) |
| 40° | 247mm | 45mm | 233mm | **159mm** | ✅ tight (29mm electronics) |
| 45° | 233mm | 59mm | 233mm | **159mm** | ✅ tight |
| 50° | 218mm | 74mm | 247mm | **145mm** | Marginal |

**Sweet spot at 300mm depth: 30-40°.** The extra depth allows shallower angles that preserve height.

**In 280W × 280D × 450H (taller enclosure, 272×272×442 interior):**

| Angle | Depth | Margin | Height | Remaining H | Notes |
|---|---|---|---|---|---|
| 30° | 268mm | 4mm | 190mm | **252mm** | Tight depth, generous height |
| 35° | 258mm | 14mm | 212mm | **230mm** | Good balance |
| 42° | 242mm | 30mm | 225mm | **217mm** | Comfortable everywhere |
| 50° | 218mm | 54mm | 247mm | **195mm** | Very comfortable |
| 60° | 183mm | 89mm | 268mm | **174mm** | Still works |

At 450mm height, everything fits with generous margins. **The 450mm enclosure eliminates all dimensional tension.**

---

## The Width Problem

### The Core Conflict

| Bags + Cartridge Side-by-Side | Width Needed | 272mm Interior | Fit? |
|---|---|---|---|
| 2L bag (190mm) + cartridge (150mm) | 340mm+ | 272mm | ❌ 68mm short |
| 1L bag (152mm) + cartridge (150mm) | 302mm+ | 272mm | ❌ 30mm short |
| 1L bag (152mm) + rotated cartridge (80mm) | 232mm+ | 272mm | ✅ 40mm spare |
| 2L bag (190mm) + rotated cartridge (80mm) | 270mm+ | 272mm | ✅ barely (2mm) |

**Nothing fits side-by-side in 280mm width with the cartridge in its current 150mm-wide orientation.** The cartridge must be rotated to its 80mm-wide orientation (80W × 150D × 130H) or placed above/below the bags rather than beside them.

### Solutions Explored

**A. Widen enclosure:** 320mm for 1L, 358mm for 2L. 320mm is viable; 358mm is too wide for most installations.

**B. Cartridge above bags (vertical stacking):** Works at 42-50° for 1L bags in 400H. Remaining height after bags (~145-167mm) accommodates cartridge (80mm) + lever (40mm) = 120mm.

**C. Cartridge beside bags at different depth (Z-stagger):** The diagonal creates a wedge of free space at the front-upper area. But the cartridge at 130mm depth doesn't fit in this wedge until ~250mm above the bag bottom — too high, crowds electronics.

**D. Rotated cartridge (80W × 150D × 130H):** Works for 1L bags with 40mm spare width. Requires redesigning the cartridge for a narrow, deep loading slot. Marginal for 2L bags (2mm clearance).

**E. Bag compression / overlap:** Feasible for 1L bags (lose ~10-15% capacity). Not feasible for 2L bags (would need 68mm of compression — more than the bag's full thickness).

---

## Curved Cradles: Complexity Without Benefit

### Concave (bag middles sag lower)
- Bounding box is **unchanged** — determined by endpoints, not middle
- Drainage is **worse** — liquid pools in the belly, away from the connector
- **Not recommended**

### Convex (bag middles pushed up)
- Bounding box **increases** — middle is now the highest point
- Drainage is better (liquid runs to connector) but also runs to sealed end (no outlet)
- **Not recommended**

### S-Curve / Compound
- Bounding box unchanged
- Creates fold lines / kink risk in the flat pouch
- **Not recommended**

### Minimum radius of curvature
- Full bag conforms to R > ~80mm (2× bag thickness)
- Gentle curves (R = 500-1000mm) are feasible but save no space
- **Curvature is geometrically feasible but pointless — it does not reduce the bounding box**

---

## Component Placement: Three Viable Layouts

### Layout A: 1L at 42° in 280W × 250D × 400H (current enclosure size)

The minimum-change option. Keeps current enclosure, just steepens the angle.

- Bags: 42°, consuming 242mm depth (exactly fills interior) × 225mm height
- Cartridge: above bags, 231-311mm height, front-loading
- Lever: 311-351mm
- Electronics: 355-392mm (37mm zone — extremely tight)
- Hopper funnels: integrated into top, accessible from above

**Pro:** Keeps 250mm depth. **Con:** Zero depth margin, electronics squeezed into 37mm.

### Layout B: 1L at 35° in 280W × 300D × 400H (deeper enclosure)

The balanced option. Trades 50mm of depth for a much more comfortable layout.

- Bags: 35°, consuming 258mm depth × 212mm height
- Cartridge: above bags, 218-298mm height
- Lever: 298-338mm
- Electronics: 342-392mm (50mm zone — adequate)
- Front zone: 34mm clear in front of bags for plumbing runs

**Pro:** Comfortable clearances, simple geometry. **Con:** 50mm deeper than current assumption (trivial for under-sink).

### Layout C: 2L at 55° in 320W × 300D × 450H (larger enclosure)

The maximum capacity option.

- Bags: 55°, consuming 266mm depth × 333mm height, 190mm wide
- Cartridge: beside bags in 80mm-wide slot (rotated to 80W × 150D × 130H)
- Electronics: above bag stack, 337-442mm (105mm zone)
- Hopper: top of enclosure

**Pro:** 2L capacity, monthly refills. **Con:** 54% larger enclosure volume (43.2L vs 28.0L).

---

## Tube Routing and Dead Volume

Tube ID = 6.35mm, cross-section = 31.7mm².

| Layout | Tube Run | Dead Volume (2 lines) | vs. Current Design |
|---|---|---|---|
| Current (18°, broken) | ~200mm | 12.7ml | baseline |
| A (42°, 250D×400H) | ~250mm | 15.9ml | +25% |
| B (35°, 300D×400H) | ~220mm | 13.9ml | +10% |
| C (55°, 2L beside) | ~150mm | 9.5ml | -25% |

---

## Key Conclusions

### 1. The existing design is broken
The corrected 1L bag length (280mm, not 250mm) means the 18° incline consumes 282mm of depth — 40mm more than the 242mm interior. Every document that builds on `bag-zone-geometry.md` inherits this error.

### 2. Steep diagonal (60-75°) is NOT the answer
It trades the abundant depth dimension for the scarce height dimension. At steep angles, bags consume 268-283mm of height, leaving insufficient room for cartridge + electronics in a 400mm enclosure.

### 3. The practical sweet spots are moderate angles
- **At 250mm depth: 42-45°** — the minimum angle to fit corrected bags
- **At 300mm depth: 30-40°** — preserves the most height for other components
- These are "inclined" rather than "diagonal" — still recognizably angled, not nearly vertical

### 4. Depth increase to 300mm is the simplest fix
It allows angles as shallow as 18° (if slightly over 292mm interior is okay) or comfortably 25-35°. The under-sink research confirms 300mm depth costs nothing in compatibility. This fixes the broken design with minimal disruption.

### 5. 2L bags at shallow diagonal angles — A GAP IN THIS RESEARCH

**CORRECTION (post-review):** This research only swept 2L bags at 50°+, and the devil's advocate only checked 35° for horizontal zones (separate layers, not diagonal stacking). Nobody computed 2L diagonal stacking at 35-45°.

Additionally, ALL component placement layouts in this document place the cartridge ABOVE the bags, which is horizontal-zone thinking applied to a diagonal layout. In a true diagonal layout, the bags occupy a diagonal slab and the cartridge sits in the triangular void at the front-bottom — a completely different depth/height zone. The "remaining height above bags" metric is the wrong question for diagonal layouts.

**The missing analysis: 2L diagonal stacking at 35-45° in a 350mm-deep enclosure:**

| Angle | Depth (L=350, T=80) | Fits 342mm int.? | Height | Bags alone in 392mm int. | Front-bottom triangle |
|---|---|---|---|---|---|
| 35° | 333mm | ✅ (9mm margin) | 266mm | 126mm above bags | ~130D × 200H available |
| 40° | 320mm | ✅ (22mm margin) | 286mm | 106mm above bags | ~130D × 160H available |
| 45° | 304mm | ✅ (38mm margin) | 304mm | 88mm above bags | ~130D × 130H available |

The cartridge (150W × 130D × 80H) fits in the front-bottom triangle at ALL of these angles. It does not need to go above the bags. Electronics can go in the top zone above bags, or in the upper-front triangle, or both — there is ample space when components are not forced into horizontal stacking.

**This means 2L bags at 35° in a 280W × 350D × 400H enclosure (39.2L) may be viable.** This option was never explored. It requires 350mm depth (still well within under-sink capacity of 480-510mm) but keeps the 400mm height.

Previous conclusion 5 stated "2L requires 450mm+ height." That conclusion was based on (a) horizontal zone analysis where the cartridge must stack above bags, and (b) a 2L sweep that only started at 50°. Both had the same zone-thinking bias. The corrected conclusion is: **2L bags require either 450mm+ height (if cartridge goes above bags) OR 350mm+ depth at 35-40° (if cartridge goes in the front-bottom triangle).**

### 6. Curved cradles don't help
The bounding box is determined by endpoints. Curvature redistributes material within the box but doesn't shrink it. Straight cradles are simpler and have better drainage.

### 7. The "diagonal vision" maps to 35-45°, not 60-65°
The owner's vision of bags stretching diagonally through the enclosure is geometrically sound, but the optimal angle is much shallower than originally computed. At 35-45°, bags are visibly angled but not nearly vertical. This still breaks the horizontal zone paradigm — the bags span most of the depth-height plane rather than lying in a thin bottom zone.
