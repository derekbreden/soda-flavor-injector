# Vision 1: Diagonal Bag Placement and Enclosure Dimensions

This document analyzes the diagonal interleave layout where bags stretch from the top-front corner to the bottom-back corner of the enclosure, and other components occupy the triangular voids created by that diagonal slab. The cartridge sits in the front-bottom triangular void. Electronics occupy the top-back void.

**Critical design facts held throughout this analysis:**
- Bags are permanent fixtures, installed at manufacturing. The user never touches them. Refilling is via hopper/funnel at top.
- The cartridge goes in the front-bottom triangle, not above the bags.
- Depth is unconstrained. Under-sink cabinets have 480-510mm of usable depth. 300mm or 350mm costs nothing in compatibility.
- Corrected bag dimensions: Platy 1L = 280mm long x 152mm wide, 25mm thick per bag, 50mm stacked. Platy 2L = 350mm long x 190mm wide, 40mm thick per bag, 80mm stacked.
- Cartridge envelope: 150W x 130D x 80H mm (standard orientation). Rotated: 80W x 150D x 130H mm.

---

## 1. Geometry of Diagonal Bag Placement

### 1a. Coordinate System and Orientation

```
    FRONT WALL                          BACK WALL
    (depth=0)                           (depth=D)

    height=H ┌──────────────────────────────┐
              │  *  sealed end              │
              │   \  (top-front)            │
              │    \                        │
              │     \  BAG SLAB             │
              │      \  (angle theta)       │
              │       \                     │
              │        \                    │
              │         \                   │
              │          \                  │
              │           *  connector end  │
    height=0  └──────────────────────────────┘
              depth=0                  depth=D
```

Bags stretch from the **top-front** corner (sealed end, high) to the **bottom-back** corner (connector end, low). The angle theta is measured from horizontal. The connector is at the lowest point, so gravity pulls liquid toward the outlet.

### 1b. Bounding Box Formulas

For two stacked bags at angle theta from horizontal:

- **Depth consumed** = L x cos(theta) + T_stack x sin(theta)
- **Height consumed** = L x sin(theta) + T_stack x cos(theta)

Where L = bag length, T_stack = combined thickness of two bags.

### 1c. Front-Bottom Triangle Geometry

The front-bottom triangular void is bounded by the front wall, the floor, and the lower surface of the bottom bag. The bag slab is positioned so its connector end (lower-back corner) sits at the floor near the back of the enclosure.

The lower surface of the bag slab forms a line from:
- **Back-bottom**: (depth = interior_D, height = 0) -- connector end on the floor at the back
- **Front-upper**: (depth = interior_D - L x cos(theta), height = L x sin(theta)) -- sealed end, elevated at the front

At any depth d from the front wall, the height of the lower bag surface is:

    h(d) = L x sin(theta) x (interior_D - d) / (L x cos(theta))
         = tan(theta) x (interior_D - d)

This means the available height in the front-bottom void at depth d is:

    h_available(d) = tan(theta) x (interior_D - d)    for d >= front_gap
    h_available(d) = interior_H                         for d < front_gap

Where front_gap = interior_D - bag_bounding_box_depth = the clear space between the front wall and the front face of the bag slab. In this gap, the full enclosure height is available because the bag hasn't started yet.

**This is critical.** When the enclosure is deeper than the bag's bounding box, there is a gap between the front wall and the bag. The cartridge can sit entirely within this gap, requiring zero intrusion into the triangle -- it just sits on the floor in front of the bag slab with full height clearance.

### 1d. Angle Sweep: Platy 1L (L=280mm, T_stack=50mm)

| Angle | Depth Consumed | Height Consumed | Triangle Max Depth | Triangle Max Height |
|-------|---------------|-----------------|--------------------|--------------------|
| 15    | 283           | 121             | 270                | 72                 |
| 20    | 280           | 143             | 263                | 96                 |
| 25    | 275           | 164             | 254                | 118                |
| 30    | 267           | 183             | 242                | 140                |
| 35    | 258           | 202             | 229                | 161                |
| 40    | 247           | 218             | 214                | 180                |
| 45    | 233           | 233             | 198                | 198                |
| 50    | 218           | 247             | 180                | 214                |
| 55    | 202           | 258             | 161                | 229                |
| 60    | 183           | 267             | 140                | 242                |
| 65    | 164           | 275             | 118                | 254                |
| 70    | 143           | 280             | 96                 | 263                |
| 75    | 121           | 283             | 72                 | 270                |

All values in mm. "Triangle Max Depth" = L x cos(theta), the depth of the triangle at floor level. "Triangle Max Height" = L x sin(theta), the height of the triangle at the front wall.

### 1e. Angle Sweep: Platy 2L (L=350mm, T_stack=80mm)

| Angle | Depth Consumed | Height Consumed | Triangle Max Depth | Triangle Max Height |
|-------|---------------|-----------------|--------------------|--------------------|
| 15    | 359           | 168             | 338                | 91                 |
| 20    | 356           | 195             | 329                | 120                |
| 25    | 351           | 220             | 317                | 148                |
| 30    | 343           | 244             | 303                | 175                |
| 35    | 333           | 266             | 287                | 201                |
| 40    | 320           | 286             | 268                | 225                |
| 45    | 304           | 304             | 247                | 247                |
| 50    | 286           | 320             | 225                | 268                |
| 55    | 266           | 333             | 201                | 287                |
| 60    | 244           | 343             | 175                | 303                |
| 65    | 220           | 351             | 148                | 317                |
| 70    | 195           | 356             | 120                | 329                |
| 75    | 168           | 359             | 91                 | 338                |

### 1f. Key Geometric Observations

1. **Depth and height are symmetric at 45 degrees.** Below 45 degrees, bags consume more depth than height. Above 45 degrees, vice versa. Since depth is cheap and height is expensive, angles below 45 degrees are generally preferred.

2. **The front-bottom triangle grows taller as the angle increases.** At 35 degrees, the 1L triangle is 161mm tall at the front wall -- enough for the cartridge (80mm) plus its lever (40mm). At 35 degrees for 2L, the triangle is 201mm tall -- even more generous.

3. **The T_stack penalty.** Stacking two bags adds T_stack x sin(theta) to depth and T_stack x cos(theta) to height. At shallow angles (15-25 degrees), the height penalty is nearly the full T_stack (cos is near 1). At steep angles, the depth penalty approaches T_stack. This means stacking hurts less in the scarce dimension (height) at steeper angles.

---

## 2. Enclosure Dimension Analysis

Interior dimensions = exterior - 8mm per axis (4mm walls on each side).

### 2a. Enclosure A: 280W x 300D x 400H (Corrected Standard)

**Interior: 272W x 292D x 392H**

#### 1L Bags (L=280, T_stack=50)

All 13 angles (15-75 degrees) fit in both depth and height. The 1L bags never exceed 283mm depth or 283mm height, both well within the 292D x 392H interior.

| Angle | Bag Depth | Bag Height | Front Gap | Bag Lower at Front (mm) | Cartridge Fits (130D x 80H) | Top-Back Void Height |
|-------|-----------|-----------|-----------|--------------------------|------------------------------|---------------------|
| 25    | 275       | 164       | 17        | 118                      | No (76mm clearance at d=130) | 228                 |
| 30    | 267       | 183       | 25        | 140                      | Yes (94mm clearance)         | 209                 |
| 35    | 258       | 202       | 34        | 161                      | Yes (113mm clearance)        | 190                 |
| 40    | 247       | 218       | 45        | 180                      | Yes (136mm clearance)        | 174                 |
| 45    | 233       | 233       | 59        | 198                      | Yes (162mm clearance)        | 159                 |
| 50    | 218       | 247       | 74        | 214                      | Yes (193mm clearance)        | 145                 |

"Front Gap" = distance from front wall to front face of the bag slab. The cartridge (130mm deep) extends past this gap into the triangle at shallow angles. At 55 degrees and above, the front gap exceeds 90mm and the cartridge barely intrudes under the bags at all.

**Sweet spot: 30-40 degrees.** The cartridge fits comfortably in the front-bottom zone. The top-back void has 174-209mm of height for electronics (only ~40mm needed). Every angle from 30 to 75 degrees works, but 30-40 degrees maximize the front-bottom triangle depth (useful for plumbing behind the cartridge) while keeping height consumption moderate.

At 25 degrees, the cartridge (80mm tall) has only 76mm of clearance where it extends under the bag -- it doesn't fit. At 30 degrees, clearance jumps to 94mm (14mm margin above the cartridge). This is the threshold angle for the cartridge in this enclosure.

#### 2L Bags (L=350, T_stack=80)

2L bags consume 333-359mm of depth at angles 15-35 degrees, exceeding the 292mm interior. They only fit at 50 degrees and above.

| Angle | Bag Depth | Bag Height | Front Gap | Cartridge Fits | Top-Back Void Height |
|-------|-----------|-----------|-----------|---------------|---------------------|
| 50    | 286       | 320       | 6         | Yes (193mm)   | 72                  |
| 55    | 266       | 333       | 26        | Yes (231mm)   | 59                  |

**2L bags do not work in this enclosure at the target angles (35-45 degrees).** They only fit at 50+ degrees, which consumes 320+ mm of height, leaving only 59-72mm for electronics in the top-back void. This is tight but possibly workable (ESP32 + driver board can fit in ~50mm height if laid flat). However, the 6mm front gap at 50 degrees means the cartridge is almost directly under the bag -- no room for error.

**Verdict: This enclosure is 1L-only territory.**

### 2b. Enclosure B: 280W x 350D x 400H (2L Optimized)

**Interior: 272W x 342D x 392H**

#### 1L Bags

All angles fit easily. The 50mm extra depth over Enclosure A creates enormous front gaps.

| Angle | Bag Depth | Front Gap | Cartridge Fits | Top-Back Void Height |
|-------|-----------|-----------|---------------|---------------------|
| 25    | 275       | 67        | Yes (99mm)    | 228                 |
| 30    | 267       | 75        | Yes (122mm)   | 209                 |
| 35    | 258       | 84        | Yes (148mm)   | 190                 |
| 40    | 247       | 95        | Yes (178mm)   | 174                 |
| 45    | 233       | 109       | Yes (full H)  | 159                 |

At 25 degrees, the front gap is 67mm. The cartridge at 130mm depth extends 63mm past the front gap into the triangle, where it has 99mm of height clearance (19mm above the 80mm cartridge). This works.

At 45 degrees, the front gap is 109mm -- the cartridge barely extends past the front of the bag slab (only 21mm of the 130mm cartridge depth is under the bag). The cartridge essentially sits in open space in front of the bags.

**Sweet spot: 25-35 degrees.** Shallower angles are viable here because the deeper enclosure provides more room. This preserves the most height for the top-back electronics zone (190-228mm).

#### 2L Bags

This is the enclosure designed for 2L bags. The 342mm interior depth accommodates 2L bags starting at 35 degrees.

| Angle | Bag Depth | Bag Height | Front Gap | Cartridge Clearance | Top-Back Height |
|-------|-----------|-----------|-----------|--------------------|--------------------|
| 35    | 333       | 266       | 9         | 148mm              | 126                |
| 40    | 320       | 286       | 22        | 178mm              | 106                |
| 45    | 304       | 304       | 38        | 212mm              | 88                 |
| 50    | 286       | 320       | 56        | 253mm              | 72                 |
| 55    | 266       | 333       | 76        | full H             | 59                 |

**At 35 degrees: depth = 333mm, fits with 9mm margin.** This is tight -- 9mm of margin means manufacturing tolerances and bag deformation could eat it up. The bag height is 266mm, leaving 126mm in the top-back void for electronics. The cartridge has 148mm of clearance in the front-bottom zone -- very generous.

**At 40 degrees: depth = 320mm, fits with 22mm margin.** More comfortable. Bag height = 286mm, top-back void = 106mm. The cartridge has 178mm clearance. This is probably the practical minimum margin for 2L bags in this enclosure.

**At 45 degrees: depth = 304mm, fits with 38mm margin.** Good margin. But bag height = 304mm, leaving only 88mm above for electronics. Still workable.

**Sweet spot: 40-45 degrees for 2L bags.** Balances depth margin against height consumption. At 35 degrees, the 9mm depth margin is a concern, but it's worth prototyping.

### 2c. Enclosure C: 280W x 300D x 380H (Shorter for Deep-Sink Compatibility)

**Interior: 272W x 292D x 372H**

#### 1L Bags

Same depth fit as Enclosure A (292mm interior). The reduced height (372mm vs 392mm) cuts 20mm from the top-back void.

| Angle | Bag Height | Cartridge Fits | Top-Back Void Height |
|-------|-----------|---------------|---------------------|
| 30    | 183       | Yes (94mm)    | 189                 |
| 35    | 202       | Yes (113mm)   | 170                 |
| 40    | 218       | Yes (136mm)   | 154                 |
| 45    | 233       | Yes (162mm)   | 139                 |

Still very comfortable for 1L bags. The top-back void is 139-189mm -- far more than the ~40-50mm needed for electronics.

**Verdict: This enclosure works well for 1L bags, giving up 20mm of headroom for better compatibility with deep sinks.** Angles 30-45 degrees all work.

#### 2L Bags

Same as Enclosure A: 2L bags only fit at 50+ degrees. At 50 degrees, the top-back void is only 52mm. At 55 degrees, it's 39mm.

**Verdict: Marginal for 2L bags. The 39-52mm top-back void is barely enough for electronics, and the front gaps are tiny (6-26mm).**

### 2d. Enclosure D: 300W x 350D x 400H (Wider for 2L Side Clearance)

**Interior: 292W x 342D x 392H**

The extra 20mm of width (292mm vs 272mm) helps with 2L bag clearance and potentially allows the cartridge beside the bags.

#### 1L Bags

Identical depth/height fit to Enclosure B (same D and H). The extra width provides 292 - 152 = 140mm beside the bags. The cartridge in standard orientation (150mm wide) still doesn't fit beside the bags (152 + 150 = 302 > 292), but the rotated cartridge (80mm wide) fits easily (152 + 80 = 232 < 292, 60mm spare).

#### 2L Bags

Same depth/height performance as Enclosure B. The width picture changes:

- 2L bag (190mm) + cartridge standard (150mm) = 340mm -- doesn't fit beside (340 > 292)
- 2L bag (190mm) + cartridge rotated (80mm) = 270mm -- fits beside (270 < 292, 22mm spare)

With the cartridge in the front-bottom zone (different depth from the bags), width is not shared, so the 292mm width easily accommodates either component alone.

**The extra width provides 102mm beside the 2L bags (292 - 190 = 102mm).** This is useful for tubing routing along the side walls. In the 272mm enclosure, there's only 82mm beside 2L bags -- tighter but still workable.

**Verdict: The extra 20mm of width is a comfort margin, not a necessity. It helps with tubing routing and assembly access but doesn't unlock any configurations that Enclosure B can't handle.**

### 2e. Summary Matrix

Viable angles where bags fit AND the cartridge (130D x 80H) fits in the front-bottom zone:

| Enclosure | 1L Viable Angles | 1L Best Angle | 2L Viable Angles | 2L Best Angle |
|-----------|-----------------|---------------|------------------|---------------|
| 280x300x400 | 30-75          | 30-40         | 50-55 (tight)    | Not recommended |
| 280x350x400 | 25-75          | 25-35         | 35-55            | 40-45         |
| 280x300x380 | 30-75          | 30-40         | 50-55 (marginal) | Not recommended |
| 300x350x400 | 25-75          | 25-35         | 35-55            | 40-45         |

---

## 3. Bag Mounting in Diagonal Orientation

### 3a. Mounting Philosophy

Because bags are permanent fixtures installed during manufacturing, mounting can be robust and non-user-serviceable. There are no constraints around user replacement, tool-free access, or bag swapping. The mounting system can use screws, adhesive, welded brackets, permanent clips -- whatever provides the best support.

### 3b. Mount Points

**Top mount (sealed end, front-upper position):**
- The sealed end of the bag is a heat-sealed seam, typically 10-15mm wide.
- Mount to the front wall or to a bracket attached to the front wall, near the top of the enclosure.
- The mount must hold the bag at the correct height and prevent the sealed end from sagging forward.

**Bottom mount (connector end, back-lower position):**
- The connector end has a 28mm threaded cap where the dip tube attaches.
- Mount to the back wall or floor, near the back-bottom corner.
- The connector must be secured firmly because the fluid line connects here. Any movement risks disconnecting tubing.
- This is the low point -- gravity pulls all liquid here, so the full weight of a loaded bag hangs from this mount.

### 3c. Weight Analysis

A full 2L bag weighs approximately 2.0-2.2 kg (concentrate is slightly denser than water). A 1L bag weighs about 1.0-1.1 kg. Two stacked full bags = 2.0-2.2 kg for 1L, 4.0-4.4 kg for 2L.

At a 35-degree angle, the force components on the lower mount point are:
- Along the bag (tension): F_parallel = m x g x sin(35) = 2.2 x 9.8 x 0.574 = 12.4 N (for one 2L bag)
- Perpendicular to bag (sag force): F_perp = m x g x cos(35) = 2.2 x 9.8 x 0.819 = 17.7 N

The sag force (17.7 N, about 1.8 kg) acts to push the bag downward between the mount points. This must be resisted by the mounting system or by intermediate supports.

### 3d. Mounting Options

**Option 1: Angled Rails (Recommended for Prototyping)**

Two parallel rails run diagonally from the front-upper area to the back-lower area, spaced to match the bag width. Each rail is a 3D-printed PETG channel (U-profile, ~15mm wide x 10mm deep) screwed to the side walls of the enclosure. The bag sits in the channel, with the edges of the bag resting in the U-profile. Mounting clips at each end secure the bag.

Advantages: Simple to print, easy to position bags during assembly, provides continuous support along the bag length (prevents mid-span sag).

Disadvantages: The rails consume ~30mm of width (15mm per side), reducing the available width for bags from 272mm to 242mm. This is fine for 1L bags (152mm wide, 90mm spare) but tight for 2L bags (190mm wide, 52mm spare in 272mm enclosure).

**Option 2: Cradle Shelf**

A single 3D-printed shelf at the bag angle, spanning the full width. The shelf has a shallow lip (5mm) along the edges to prevent bags from sliding off. Bags rest on the shelf surface. The shelf is screwed to the side walls at each end.

Advantages: Full-width support, bags cannot sag. Manufacturing is one piece per shelf.

Disadvantages: The shelf blocks airflow around the bags. For two stacked bags, you need two shelves, consuming ~20mm of vertical space (10mm per shelf including structure). The shelf must handle the bending load of full bags (2.2 kg for 1L pair, 4.4 kg for 2L pair) across a 150-190mm span, which requires 4-6mm PETG thickness.

**Option 3: End Clips Only (Two-Point Stretch)**

Clips at each end of the bag only, no intermediate support. The sealed end clips to a bracket on the front wall. The connector end clips to a bracket on the back wall/floor. The bag hangs freely between the two points.

Advantages: Simplest, cheapest, no width consumed. Allows full airflow around bags.

Disadvantages: The bag sags between mount points. For a 1L bag at 35 degrees, the sag at mid-span is approximately:

    sag = (w x L^2) / (8 x T)

Where w = weight per unit length = 1.1 kg / 0.28m = 3.93 kg/m, L = span = 0.28m, T = tension in the bag.

At 35 degrees with mild pre-tension (~2 N), the sag is roughly 15-20mm. This pushes the bag downward into the front-bottom triangle, potentially interfering with the cartridge. For 2L bags (heavier, longer), sag could reach 30-40mm.

**Verdict: Two-point stretch alone is risky for 2L bags.** The sag is significant enough to intrude into the cartridge zone. Angled rails or a cradle shelf are safer choices. For 1L bags, two-point stretch with moderate pre-tension may work, but rails are still preferred for reliability.

**Option 4: Angled Rails with End Clips (Recommended)**

Combine options 1 and 3. Rails provide continuous support along the bag length. Clips at each end prevent longitudinal sliding. The rails are the primary structural element; the clips just hold position.

This is the recommended approach for both 1L and 2L bags. The rails eliminate sag, the clips prevent sliding, and the combination is robust enough for permanent installation.

### 3e. Bag Flexibility and Deformation

Platypus bags are flexible pouches. When full, they're relatively rigid due to internal liquid pressure. As they drain, the bag collapses from the sealed end (high) toward the connector end (low). On a diagonal mount:

- **Full bag:** Essentially rigid, conforms to the rail/shelf shape. No deformation concern.
- **Half-full bag:** The upper half collapses flat against the lower half. The lower half remains pressurized by the remaining liquid. The bag shortens along the incline but the weight concentrates at the lower mount.
- **Nearly empty bag:** The bag is mostly collapsed flat. Only a small puddle remains at the connector end. Minimal weight, minimal sag.

The worst-case sag occurs at full capacity. As the bag drains, sag decreases. The mounting system only needs to handle full-bag loading.

### 3f. Mounting for Two Stacked Bags

Two bags stack perpendicular to the incline surface. The lower bag rests on the rails/shelf. The upper bag rests on top of the lower bag. A thin separator (1-2mm PETG sheet) between the bags prevents them from sticking or interfering with each other's collapse.

The upper bag's weight adds to the lower bag's load. For 2L bags, the lower bag supports its own weight (2.2 kg) plus the upper bag (2.2 kg) = 4.4 kg total. The rails/shelf must handle this combined load.

At 40 degrees, the perpendicular component of 4.4 kg on the lower shelf/rail:
- F_perp = 4.4 x 9.8 x cos(40) = 33.0 N (about 3.4 kg)

This is easily handled by 4mm PETG rails screwed to the enclosure walls at 100mm intervals.

---

## 4. Drainage and Dip Tube Considerations

### 4a. Gravity Drainage by Angle

The connector end is at the bottom-back corner (low point). Gravity pulls liquid toward the connector along the incline with force F = m x g x sin(theta).

| Angle | sin(theta) | Gravity Component (fraction of g) | Qualitative Drainage |
|-------|-----------|-----------------------------------|---------------------|
| 15    | 0.26      | 26%                               | Poor -- liquid pools in bag creases |
| 20    | 0.34      | 34%                               | Adequate with pump suction |
| 25    | 0.42      | 42%                               | Good |
| 30    | 0.50      | 50%                               | Good -- half of gravity drives drainage |
| 35    | 0.57      | 57%                               | Very good |
| 40    | 0.64      | 64%                               | Very good |
| 45    | 0.71      | 71%                               | Excellent |
| 50+   | 0.77+     | 77%+                              | Excellent, approaching full gravity |

Below 20 degrees, gravity drainage becomes unreliable. Liquid can pool in bag creases and folds rather than flowing to the connector. Pump suction compensates but can't overcome all trapped pockets.

Above 30 degrees, gravity reliably moves liquid toward the connector. The bag collapses from the sealed end downward in a controlled manner. The dip tube remains submerged until the bag is nearly empty.

### 4b. Dip Tube Routing

The Platypus Drink Tube Kit creates a sealed path from the connector into the bag interior. The dip tube (1/4" ID, ~6.35mm OD) extends from the 28mm threaded cap into the bag.

In the diagonal orientation:
- The dip tube enters at the connector (back-bottom corner, lowest point).
- It extends upward along the incline toward the sealed end.
- The tube opening sits 100-150mm up the incline from the connector.

At various angles, the dip tube orientation relative to the liquid pool:

**Shallow angles (15-25 degrees):** The dip tube is nearly horizontal, extending into a thin, wide puddle of liquid. The tube opening stays submerged until the liquid level drops below the tube height (~10-15mm above the bag floor at the tube opening). Residual volume is higher because the shallow angle doesn't effectively concentrate liquid at the connector.

**Moderate angles (30-45 degrees):** The dip tube angles upward at a meaningful incline. Liquid concentrates at the connector end as a deeper, narrower puddle. The tube opening stays submerged longer. This is the optimal range for drainage efficiency.

**Steep angles (50+ degrees):** The dip tube is nearly vertical. Liquid falls rapidly to the connector. The puddle is deep and narrow. Drainage is excellent, but the near-vertical tube may trap air bubbles during refilling.

### 4c. Residual Volume Estimates

Residual volume is liquid that cannot be evacuated by gravity and pump suction combined. It includes liquid trapped in bag creases, below the dip tube opening, and in dead spots where the bag folds prevent flow.

These are rough estimates. Actual values require physical testing. The dip tube extends 100-150mm into the bag, which reaches well past the residual puddle at angles above 25 degrees. Pump suction (2-5 PSI) further reduces residual.

| Angle | 1L Estimated Residual | 1L % | 2L Estimated Residual | 2L % |
|-------|----------------------|------|----------------------|------|
| 20    | 70-80ml              | 7-8% | 140-160ml            | 7-8% |
| 25    | 55-65ml              | 5-7% | 110-130ml            | 5-7% |
| 30    | 45-55ml              | 4-6% | 90-110ml             | 4-6% |
| 35    | 40-50ml              | 4-5% | 80-100ml             | 4-5% |
| 40    | 35-40ml              | 3-4% | 70-80ml              | 3-4% |
| 45    | 30-40ml              | 3-4% | 60-80ml              | 3-4% |
| 50    | 30-35ml              | 3%   | 60-70ml              | 3%   |

Beyond 45 degrees, residual volume improvements are marginal. The dip tube and pump suction dominate over gravity at these angles. The practical floor is around 25-35ml for 1L and 50-70ml for 2L, limited by bag crease geometry rather than angle.

### 4d. Drainage Performance Summary

**Angles below 25 degrees are poor for drainage.** The gravity component is too weak to reliably move liquid through bag creases to the connector. Residual volume exceeds 5-7%.

**Angles 30-45 degrees are the drainage sweet spot.** Good gravity assistance, controlled bag collapse, reasonable residual volume (3-5%). The dip tube stays submerged until 90-95% of the bag is evacuated.

**Angles above 45 degrees offer diminishing returns.** Drainage improves marginally. The steep angle may cause the dip tube to trap air during refilling, complicating the fill cycle.

---

## 5. Width Analysis

### 5a. The Key Insight: Depth Separation

In Vision 1, the cartridge and bags occupy **different depth zones**. The cartridge sits in the front-bottom void (depth 0 to ~130mm from the front wall). The bags span from roughly (interior_D - bag_depth) to interior_D. At most angles, the bag slab's front face is 17-109mm behind the front wall (see front gap values in Section 2).

Because they're at different depths, the cartridge and bags do not compete for width. The full enclosure width is available for each component independently at its respective depth.

This resolves the width conflict identified in previous research, which assumed the cartridge would be beside the bags at the same depth.

### 5b. Width Budget at Cartridge Depth (Front)

At the cartridge depth (0-130mm from front), the only width consumers are:
- Cartridge: 150mm wide (standard orientation)
- Tubing runs: ~20-30mm per side for plumbing to/from the cartridge

Total: ~200-210mm of 272mm interior. Remaining: 62-72mm. Comfortable.

### 5c. Width Budget at Bag Depth (Back)

At the bag depth, the only width consumers are:
- 1L bags: 152mm wide. Remaining: 120mm (60mm per side for tubing, airflow, mounting hardware).
- 2L bags: 190mm wide. Remaining: 82mm (41mm per side).

Both are workable. The 2L bags in the 272mm enclosure (41mm per side) are tighter but functional.

In the 292mm enclosure (300W exterior): 2L bags have 51mm per side -- more comfortable.

### 5d. Width Overlap Zone

There IS a zone where the cartridge and bags share the same depth: where the front of the bag slab overlaps the back of the cartridge. In this overlap zone, the cartridge is below the bags (in the triangle) and the bags are above. They share the same depth range but are separated vertically by the triangle geometry.

Width is not constrained in this overlap zone because the cartridge (150mm wide) and bags (152-190mm wide) are vertically separated, not side-by-side.

### 5e. Side-by-Side Placement (Alternative to Front-Back Separation)

If for some reason the cartridge needed to be beside the bags at the same depth:

| Configuration | Width Needed | 272mm Interior | 292mm Interior |
|--------------|-------------|----------------|----------------|
| 1L bag + cartridge standard | 302mm | No (-30mm) | No (-10mm) |
| 1L bag + cartridge rotated (80W) | 232mm | Yes (+40mm) | Yes (+60mm) |
| 2L bag + cartridge standard | 340mm | No (-68mm) | No (-48mm) |
| 2L bag + cartridge rotated (80W) | 270mm | Yes (+2mm) | Yes (+22mm) |

Side-by-side only works with the rotated cartridge (80mm wide), and is extremely tight for 2L bags in the 272mm enclosure (2mm clearance). **This is unnecessary in Vision 1 because the cartridge goes in the front-bottom void, not beside the bags.**

---

## 6. Recommended Configurations

### 6a. Configuration 1: 1L Bags at 35 degrees in 280W x 300D x 400H

**The compact workhorse.**

| Parameter | Value |
|-----------|-------|
| Enclosure exterior | 280W x 300D x 400H mm |
| Enclosure interior | 272W x 292D x 392H mm |
| Bag angle | 35 degrees |
| Bag bounding box | 258D x 202H mm |
| Front gap (bag to front wall) | 34mm |
| Cartridge zone | 0-130mm depth, 0-80mm height (113mm clearance to bag) |
| Lever clearance above cartridge | 80-120mm height (40mm) |
| Top-back electronics void | 190mm height x 258mm depth |
| Enclosure volume | 33.6 liters |

**Why 35 degrees:** Balances depth usage (258mm of 292mm, 34mm margin) against height (202mm of 392mm, 190mm above for electronics). The 34mm front gap means 96mm of the cartridge's 130mm depth extends under the bag, with 113mm of clearance -- plenty of room for the cartridge plus its lever mechanism.

**Trade-offs:** Compared to 30 degrees, 35 degrees gives a taller front-bottom triangle (161mm vs 140mm at the front wall) at the cost of 13mm more height consumption. Compared to 40 degrees, 35 degrees preserves 16mm more height in the top-back void while the cartridge still fits comfortably.

**Drainage:** sin(35) = 0.57. Good gravity drainage. Estimated residual: 40-50ml (4-5%). Acceptable.

### 6b. Configuration 2: 2L Bags at 40 degrees in 280W x 350D x 400H

**The high-capacity option.**

| Parameter | Value |
|-----------|-------|
| Enclosure exterior | 280W x 350D x 400H mm |
| Enclosure interior | 272W x 342D x 392H mm |
| Bag angle | 40 degrees |
| Bag bounding box | 320D x 286H mm |
| Front gap (bag to front wall) | 22mm |
| Cartridge zone | 0-130mm depth, 0-80mm height (178mm clearance to bag) |
| Lever clearance above cartridge | 80-120mm height |
| Top-back electronics void | 106mm height x 320mm depth |
| Enclosure volume | 39.2 liters |

**Why 40 degrees:** At 35 degrees, depth margin is only 9mm (333mm bag in 342mm interior) -- too tight for manufacturing tolerances and bag deformation. At 40 degrees, margin grows to 22mm, which is workable. The top-back void at 106mm is adequate for electronics (ESP32, drivers, fuse block can fit in ~50mm height if mounted horizontally).

**Why not 45 degrees:** At 45 degrees, bag height = 304mm, leaving only 88mm in the top-back void. This is technically sufficient but leaves no margin. 40 degrees provides 18mm more breathing room.

**Trade-offs:** The enclosure is 16.7% larger in volume (39.2L vs 33.6L) than the 1L configuration. It's 50mm deeper. But it holds twice the concentrate (2L per flavor vs 1L), doubling the refill interval.

**Drainage:** sin(40) = 0.64. Very good gravity drainage. Estimated residual: 70-80ml (3.5-4%). The larger bag volume means the 4% residual is a larger absolute volume, but the percentage is acceptable.

### 6c. Configuration 3: 2L Bags at 35 degrees in 280W x 350D x 400H (Aggressive)

**The maximum-capacity option, pushing tolerances.**

| Parameter | Value |
|-----------|-------|
| Bag angle | 35 degrees |
| Bag bounding box | 333D x 266H mm |
| Front gap | 9mm |
| Depth margin | 9mm (tight) |
| Top-back electronics void | 126mm height (generous) |
| Cartridge clearance | 148mm (generous) |

**Why consider this:** At 35 degrees, the top-back void is 126mm -- 20mm more than at 40 degrees. The cartridge clearance is ample. The only problem is the 9mm depth margin. If bag deformation under full load adds 5-10mm to the effective bounding box, the bag could press against the back wall.

**Mitigation:** The back wall can incorporate a slight recess or the bag connector mount can be positioned 5mm inward from the back wall, creating an effective 14mm margin. Alternatively, slight bag compression (reducing T_stack from 80mm to 75mm) reduces depth consumption to 328mm (14mm margin). A 5mm thickness reduction across two bags is easily achievable with modest compression in the cradle.

**Verdict: Worth prototyping alongside Configuration 2.** If the 9mm margin proves workable in physical testing, this is the superior layout because it maximizes the top-back void (easier electronics packaging) and provides the shallowest practical angle for 2L bags (better height economy).

### 6d. Configuration Comparison

| Parameter | Config 1 (1L/35/300D) | Config 2 (2L/40/350D) | Config 3 (2L/35/350D) |
|-----------|----------------------|----------------------|----------------------|
| Bag capacity | 2 x 1L | 2 x 2L | 2 x 2L |
| Enclosure (ext) | 280x300x400 | 280x350x400 | 280x350x400 |
| Volume | 33.6L | 39.2L | 39.2L |
| Depth margin | 34mm | 22mm | 9mm |
| Top-back void H | 190mm | 106mm | 126mm |
| Cartridge clearance | 113mm | 178mm | 148mm |
| Drainage quality | Good (sin 35=0.57) | Very good (sin 40=0.64) | Good (sin 35=0.57) |
| Residual | ~45ml (4.5%) | ~75ml (3.8%) | ~90ml (4.5%) |
| Risk | Low | Low | Medium (tight margin) |

### 6e. Product Variant Strategy

The 1L and 2L configurations share the same width (280mm) and height (400mm). Only the depth differs (300mm vs 350mm). This suggests a two-variant product line:

**Variant A: Standard (280 x 300 x 400)** -- 1L bags, 35 degrees. Compact, fits in tighter under-sink spaces. Biweekly refill for moderate use.

**Variant B: Extended (280 x 350 x 400)** -- 2L bags, 40 degrees. 50mm deeper, holds twice the concentrate. Monthly refill for moderate use.

Both variants can share:
- Same cartridge design (150W x 130D x 80H)
- Same electronics (ESP32, drivers, fuse block)
- Same front panel and user interface
- Same hopper/funnel design
- Same plumbing topology

Only the enclosure shell, bag mounting rails, and bags themselves differ. This minimizes the engineering effort for offering two variants.

Alternatively, a single enclosure (280 x 350 x 400) could serve both markets. 1L bags at 25-35 degrees would have enormous margins in this larger enclosure, though it wastes 50mm of depth for 1L users.

### 6f. Open Questions for Physical Testing

1. **Bag deformation under load:** Do full 2L bags expand beyond the 80mm stack thickness when mounted at 35-40 degrees? If so, by how much? This determines whether Configuration 3's 9mm margin is viable.

2. **Sag between rails:** With rails at the bag edges, how much does the bag center sag? If more than 10mm, a center support rail may be needed.

3. **Dip tube behavior at 35-40 degrees:** Does the dip tube stay submerged as the bag drains? At what fill level does air first reach the tube opening?

4. **Refilling at 35-40 degrees:** Can the pump push concentrate uphill through the dip tube into a bag that's angled at 35-40 degrees? The hydrostatic head is L x sin(theta) x density x g, which at 40 degrees for a 2L bag is 350 x 0.64 x 1.05 x 9.8 / 1000 = 2.3 kPa (0.33 PSI). The Kamoer pump can handle this easily.

5. **Bag collapse pattern:** Does the bag collapse cleanly from the sealed end toward the connector, or do random folds form that trap liquid? This is critical for residual volume estimates.

6. **Front gap usability:** With only 9-34mm between the front wall and the bag slab, can plumbing and electrical connections route through this gap? Or must all connections come from the sides?
