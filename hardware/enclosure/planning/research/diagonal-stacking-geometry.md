# Diagonal Stacking Geometry: Comprehensive Analysis

Full angle/dimension sweep using CORRECTED bag dimensions and CORRECTED bag shape modeling. Previous versions used rigid-body rectangle geometry (`L cos θ + T sin θ`), which treats bags as constant-thickness rectangles. Real Platypus bags are lens-shaped — thin at the sealed end, bulging at center, intermediate at the cap end — so the rigid model significantly overstates depth at shallow angles.

Enclosure: 280W × 300D × 400H exterior, 272W × 292D × 392H interior (4mm walls).

---

## Bag Shape: Why the Rigid Model Is Wrong

The rigid-body formula `Depth = L cos θ + T sin θ` assumes the bag is a rectangle of uniform thickness T. Real Platypus bags are lens-shaped in cross-section:

| Region | 2L Per Bag | 2L Stacked (2 bags) | 1L Per Bag | 1L Stacked (2 bags) |
|---|---|---|---|---|
| Sealed end (flat film) | ~1mm | ~2mm | ~1mm | ~2mm |
| Center (max bulge) | ~40mm | ~80mm | ~25mm | ~50mm |
| Cap/connector end | ~15mm | ~30mm | ~12mm | ~25mm |

The rigid model uses the center thickness (80mm for 2L stacked, 50mm for 1L stacked) uniformly across the entire length. This overstates depth because:
- The sealed end contributes almost zero forward depth (it is flat film, not a 80mm slab).
- The cap end contributes less forward depth than the center.
- Only the center bulge reaches full thickness.

For 2L bags at 35°, the rigid model overstates depth by ~37mm. For 1L bags the correction is smaller because the bags are thinner (the rigid model was less wrong for thinner bags).

See `2l-bags-at-300mm-depth.md` for detailed 2L depth analysis and `2l-rigid-body-geometry.svg` for the visual.

---

## Corrected Bag Dimensions Used Throughout

| Model | Width | Length | Center Thickness | Stack (2 bags, center) |
|---|---|---|---|---|
| Platy 1.0L | 152mm | 280mm | ~25mm | ~50mm |
| Platy 2.0L | 190mm | 350mm | ~40mm | ~80mm |
| SoftBottle 1.0L | 152mm | 330mm | ~20mm | ~40mm |
| Hoser 2.0L | 152mm | 406mm | ~30mm est. | ~60mm est. |

---

## Angle Sweep Tables

### Rigid-Body Model (Upper Bound)

Formulas for two stacked bags at angle θ from horizontal:
- **Depth consumed** = L × cos(θ) + T_stack × sin(θ)
- **Height consumed** = L × sin(θ) + T_stack × cos(θ)

These are upper-bound estimates. Real bags consume less depth than shown here because the thickness is not uniform. See "Actual Bag Shape" tables below.

#### Platy 1.0L Rigid Body (L=280mm, T_stack=50mm)

| Angle | Depth (rigid) | Height (rigid) |
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

#### Platy 2.0L Rigid Body (L=350mm, T_stack=80mm)

| Angle | Depth (rigid) | Height (rigid) |
|---|---|---|
| 25° | 351mm | 220mm |
| 30° | 343mm | 244mm |
| 35° | 333mm | 266mm |
| 40° | 320mm | 286mm |
| 45° | 304mm | 304mm |
| 50° | 286mm | 320mm |
| 55° | 266mm | 333mm |
| 60° | 244mm | 343mm |

### Actual Bag Shape Depth Estimates

Because bags are lens-shaped, the effective depth is less than the rigid model predicts. The sealed end (~2mm stacked) contributes negligible forward depth compared to the rigid model's assumption of 80mm (2L) or 50mm (1L) thickness at that point.

#### Platy 1.0L Actual Shape (L=280mm, T_stack=50mm center, ~2mm sealed end, ~25mm cap end)

| Angle | Depth (rigid) | Depth (actual, est.) | Correction | Fits 292mm? |
|---|---|---|---|---|
| 18° | 282mm | ~274mm | ~-8mm | Yes (18mm margin) |
| 25° | 275mm | ~264mm | ~-11mm | Yes (28mm margin) |
| 30° | 268mm | ~254mm | ~-14mm | Yes (38mm margin) |
| 35° | 258mm | ~240mm | ~-18mm | Yes (52mm margin) |
| 40° | 247mm | ~228mm | ~-19mm | Yes (64mm margin) |
| 45° | 233mm | ~216mm | ~-17mm | Yes (76mm margin) |

The 1L correction is modest (8-19mm) because the bags are thinner overall. All 1L angles fit the 292mm interior comfortably even under the rigid model.

#### Platy 2.0L Actual Shape (L=350mm, T_stack=80mm center, ~2mm sealed end, ~30mm cap end)

| Angle | Depth (rigid) | Depth (actual, est.) | Correction | Fits 292mm? |
|---|---|---|---|---|
| 25° | 351mm | ~318mm | ~-33mm | No (26mm over) |
| 30° | 343mm | ~307mm | ~-36mm | No (15mm over) |
| 35° | 333mm | ~296mm | ~-37mm | Yes (barely, ~4mm margin) |
| 40° | 320mm | ~285mm | ~-35mm | Yes (7mm margin) |
| 45° | 304mm | ~273mm | ~-31mm | Yes (19mm margin) |
| 50° | 286mm | ~261mm | ~-25mm | Yes (31mm margin) |

**With back-wall mounting** (sealed end pinned flat against back wall, cap end pulled forward, gravity-positioned in a profiled 3D-printed cradle):

| Angle | Depth (back-wall mounted, est.) | Margin in 292mm | Notes |
|---|---|---|---|
| 30° | ~280mm | 12mm | Tight but viable |
| 35° | ~267mm | 25mm | Good working margin |
| 40° | ~256mm | 36mm | Comfortable |
| 45° | ~245mm | 47mm | Generous |

Back-wall mounting works because the sealed end is flat film (~2mm) that pins directly against the back wall, contributing zero forward depth. The effective depth starts from the point where the bag begins to bulge. A 3D-printed profiled cradle supports from underneath — shallow at the ends, deep at center — matching the lens cross-section.

---

## Enclosure Fit Analysis

### Platy 1.0L in 280W × 300D × 400H (272×292×392 interior)

Using actual bag shape estimates:

| Angle | Depth (actual) | Margin | Height | Remaining H | Cartridge+electronics above? |
|---|---|---|---|---|---|
| 18° | ~274mm | 18mm | ~134mm | **258mm** | Yes, abundant |
| 25° | ~264mm | 28mm | ~166mm | **226mm** | Yes, abundant |
| 30° | ~254mm | 38mm | ~190mm | **202mm** | Yes, good |
| 35° | ~240mm | 52mm | ~212mm | **180mm** | Yes, good (50mm for electronics) |
| 40° | ~228mm | 64mm | ~233mm | **159mm** | Yes, tight (29mm electronics) |
| 45° | ~216mm | 76mm | ~233mm | **159mm** | Yes, tight |

**Sweet spot: 25-40°.** The lens-shaped correction gives substantially more margin than the rigid model suggested. Even 18° now fits with room to spare.

### Platy 2.0L in 280W × 300D × 400H (272×292×392 interior)

Using actual bag shape estimates (free-standing):

| Angle | Depth (actual) | Margin | Height | Remaining H | Notes |
|---|---|---|---|---|---|
| 35° | ~296mm | ~-4mm | ~266mm | 126mm | Barely does not fit free-standing |
| 40° | ~285mm | 7mm | ~286mm | 106mm | Fits, tight margin |
| 45° | ~273mm | 19mm | ~304mm | 88mm | Fits, adequate margin |

Using actual bag shape estimates (back-wall mounted):

| Angle | Depth (BWM) | Margin | Height | Remaining H | Notes |
|---|---|---|---|---|---|
| 30° | ~280mm | 12mm | ~244mm | 148mm | Tight depth, good height |
| 35° | ~267mm | 25mm | ~266mm | 126mm | Good balance |
| 40° | ~256mm | 36mm | ~286mm | 106mm | Comfortable depth |
| 45° | ~245mm | 47mm | ~304mm | 88mm | Generous depth, tight height |

**With back-wall mounting, 2L bags fit at 35° in the 300mm-deep enclosure with 25mm margin.** This is a critical finding — previous analysis concluded 2L bags required 50°+ angles or a 350mm+ deep enclosure, but that was based on the rigid-body model.

---

## The Width Problem

### The Core Conflict

| Bags + Cartridge Side-by-Side | Width Needed | 272mm Interior | Fit? |
|---|---|---|---|
| 2L bag (190mm) + cartridge (150mm) | 340mm+ | 272mm | No, 68mm short |
| 1L bag (152mm) + cartridge (150mm) | 302mm+ | 272mm | No, 30mm short |
| 1L bag (152mm) + rotated cartridge (80mm) | 232mm+ | 272mm | Yes, 40mm spare |
| 2L bag (190mm) + rotated cartridge (80mm) | 270mm+ | 272mm | Yes, barely (2mm) |

**Nothing fits side-by-side in 280mm width with the cartridge in its current 150mm-wide orientation.** The cartridge must be rotated to its 80mm-wide orientation (80W × 150D × 130H) or placed above/below the bags rather than beside them.

### Solutions Explored

**A. Widen enclosure:** 320mm for 1L, 358mm for 2L. 320mm is viable; 358mm is too wide for most installations.

**B. Cartridge above bags (vertical stacking):** Works at moderate angles for both 1L and 2L bags. In the 300D × 400H enclosure, remaining height above bags is 126-258mm depending on angle and bag size — sufficient for cartridge (80mm) + lever (40mm) = 120mm at most angles.

**C. Cartridge in front-bottom triangle (diagonal layout):** The diagonal bag slab creates a triangular void at the front-bottom of the enclosure. At 35°, this triangle is roughly 130D × 200H — the cartridge (150W × 130D × 80H) fits here without needing to stack above the bags at all.

**D. Rotated cartridge (80W × 150D × 130H):** Works for 1L bags with 40mm spare width. Requires redesigning the cartridge for a narrow, deep loading slot. Marginal for 2L bags (2mm clearance).

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

### Profiled cradle (matches lens cross-section)
- Shallow at sealed end, deep at center, intermediate at cap end
- Supports bag weight without wasted space
- Works naturally with back-wall mounting: cradle holds bag shape, sealed end pins to back wall
- **Recommended for back-wall mounting configuration**

### Minimum radius of curvature
- Full bag conforms to R > ~80mm (2x bag thickness)
- Gentle curves (R = 500-1000mm) are feasible but save no space in the length-wise bounding box
- **Curvature along the length axis does not reduce the bounding box**

---

## Component Placement: Three Viable Layouts

### Layout A: 1L at 35° in 280W × 300D × 400H (recommended for 1L)

The balanced option. Actual bag shape gives generous margins everywhere.

- Bags: 35°, consuming ~240mm depth (52mm margin) × ~212mm height
- Cartridge: above bags, ~218-298mm height, front-loading
- Lever: 298-338mm
- Electronics: 342-392mm (50mm zone — adequate)
- Front zone: 52mm clear in front of bags for plumbing runs

**Pro:** Comfortable clearances, simple geometry, no back-wall mounting needed. **Con:** None significant.

### Layout B: 2L at 35° with back-wall mounting in 280W × 300D × 400H

The maximum capacity option that fits the standard enclosure.

- Bags: 35° with back-wall mounting, consuming ~267mm depth (25mm margin) × ~266mm height
- Sealed end pinned to back wall, cap/connector end pulled forward
- 3D-printed profiled cradle supports from underneath
- Cartridge: in front-bottom triangle (~130D × 126H available) or above bags (126mm remaining height)
- Electronics: above bags if cartridge is in triangle, or in upper-front area

**Pro:** 2L capacity in the standard 300mm-deep enclosure. **Con:** Requires back-wall mounting and profiled cradle; 25mm depth margin is adequate but not generous.

### Layout C: 2L at 40° free-standing in 280W × 300D × 400H

A simpler 2L option without back-wall mounting.

- Bags: 40° free-standing, consuming ~285mm depth (7mm margin) × ~286mm height
- Cartridge: in front-bottom triangle or above bags (106mm remaining height)
- Electronics: in remaining space above

**Pro:** No special mounting needed. **Con:** Only 7mm depth margin — tight. Height consumption (286mm) limits space above for cartridge + electronics.

---

## Tube Routing and Dead Volume

Tube ID = 6.35mm, cross-section = 31.7mm².

| Layout | Tube Run | Dead Volume (2 lines) | vs. Baseline |
|---|---|---|---|
| A (1L at 35°, 300D×400H) | ~220mm | 13.9ml | baseline |
| B (2L at 35° BWM, 300D×400H) | ~200mm | 12.7ml | -9% |
| C (2L at 40°, 300D×400H) | ~210mm | 13.3ml | -4% |

---

## Key Conclusions

### 1. The rigid-body model significantly overstates depth

The formula `L cos θ + T sin θ` treats bags as constant-thickness rectangles. Real Platypus bags are lens-shaped: ~2mm at the sealed end, full bulge only at center, ~30mm at the cap end. This overstates depth by ~37mm for 2L bags at 35° and ~18mm for 1L bags at 35°. All previous depth calculations were upper bounds, not actual depths.

### 2. 1L bags fit easily at any angle from 18-45°

With the lens-shape correction, 1L bags fit the 292mm interior with generous margins at all practical angles. The sweet spot is 25-40° — preserving height for components above while maintaining comfortable depth margins.

### 3. 2L bags fit at 35° — not 50°+

Previous analysis concluded 2L bags required 50°+ angles or a 350mm+ deep enclosure. This was wrong — it was based on the rigid-body model. With actual bag shape:
- Free-standing at 40°: fits with 7mm margin (tight but viable)
- Back-wall mounted at 35°: fits with 25mm margin (good working clearance)
- Back-wall mounted at 30°: fits with 12mm margin (tight but possible)

### 4. Back-wall mounting is a significant depth saver for 2L bags

The sealed end is flat film that pins directly against the back wall, contributing zero forward depth. This drops effective depth by ~29mm at 35° compared to free-standing. A 3D-printed profiled cradle (shallow at ends, deep at center) supports the bag from underneath. This technique matters most for 2L bags where the lens-shape correction is largest.

### 5. The 300mm-deep enclosure supports both 1L and 2L bags

No need for a 350mm or deeper enclosure. The 280W × 300D × 400H enclosure (272 × 292 × 392 interior) works for:
- 1L bags at 25-40° with abundant margins
- 2L bags at 35-40° with back-wall mounting and adequate margins
- 2L bags at 40-45° free-standing with tighter margins

### 6. Curved cradles along the length axis don't help

The bounding box is determined by endpoints. Curvature redistributes material within the box but doesn't shrink it. However, a profiled cradle that matches the lens cross-section (shallow at ends, deep at center) is recommended for back-wall mounting to properly support the bag shape.

### 7. The "diagonal vision" maps to 30-40°, not 60-65°

The vision of bags stretching diagonally through the enclosure is geometrically sound at moderate angles. At 35°, bags are visibly angled and span most of the depth-height plane — breaking the horizontal zone paradigm — while leaving usable triangular voids for the cartridge and electronics.
