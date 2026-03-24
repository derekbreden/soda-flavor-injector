# Can 2L Platypus Bags Fit in a 300mm-Deep Enclosure?

A rigorous investigation of every avenue that might allow 2L Platy bags (350mm long x 190mm wide, ~80mm stacked compressed) to work in the 300mm exterior / 292mm interior depth enclosure at moderate diagonal angles (35-45 degrees).

**Bottom line up front:** Rigid-body math says no at 35-40 degrees, yes at 50 degrees. But the bags are not rigid bodies. When bag flexibility, partial fill, mount point geometry, wall thickness trade-offs, and spout integration are combined, 2L bags can plausibly fit at 292mm interior depth at angles of 43-47 degrees -- and possibly lower. Physical prototyping is required to confirm, but the case is stronger than the previous research suggested.

---

## 1. The Rigid-Body Problem Restated

The standard bounding box formulas for two stacked bags at angle theta from horizontal:

- **Depth consumed** = L x cos(theta) + T_stack x sin(theta)
- **Height consumed** = L x sin(theta) + T_stack x cos(theta)

For the Platy 2L (L=350mm, T_stack=80mm) in 292mm interior depth:

| Angle | Depth Consumed | Deficit vs 292mm | Height Consumed |
|-------|---------------|-----------------|-----------------|
| 35    | 333mm         | -41mm (short)   | 266mm           |
| 38    | 325mm         | -33mm           | 278mm           |
| 40    | 319mm         | -27mm           | 286mm           |
| 43    | 311mm         | -19mm           | 293mm           |
| 45    | 304mm         | -12mm           | 304mm           |
| 47    | 297mm         | -5mm            | 315mm           |
| 48    | 294mm         | -2mm            | 320mm           |
| 49    | 290mm         | +2mm (fits)     | 325mm           |
| 50    | 286mm         | +6mm (fits)     | 320mm           |

By rigid-body math alone, 49 degrees is the break-even angle. The previous research set the threshold at 50 degrees. The question is whether real-world factors can recover 5-41mm of depth to bring the viable range down into the 40s or even high 30s.

---

## 2. Bag Flexibility and Deformation

### 2a. The bag is not a rigid board

The 350mm length is the flat dimension of an empty, laid-out bag. A Platypus Platy pouch is a two-panel welded flexible container made from BPA-free polyethylene film. When filled with liquid and placed on a diagonal:

- **The bag becomes a pillow.** The center inflates to 38-50mm thickness (single bag, full), while the ends taper toward the heat-sealed seam (top) and the spout collar (bottom). The effective footprint in the length dimension is determined by where the pillow profile meets the support surface, not by the full flat dimension.

- **Mid-span sag reduces the projected footprint.** When supported at two ends on a diagonal rail or cradle, gravity pulls the center of the bag downward (perpendicular to the incline). This bowing means the bag's centerline follows a catenary-like curve rather than a straight line between endpoints. The horizontal projection of a curved path is shorter than the projection of a straight path at the same angle. For a 350mm bag with 15-20mm of mid-span sag (estimated from the v1-diagonal-bag-placement analysis for 2L bags under gravity), the effective projected length shrinks by approximately 3-8mm depending on angle.

- **The sealed end is not load-bearing structure.** The heat seal at the top of the bag is a 10-15mm wide weld zone where two film panels join. This zone is flat and empty -- no liquid reaches it. It can fold over a mount bracket, curl upward, or compress against a wall without affecting bag capacity. The functional bag starts 10-15mm inward from the sealed end.

- **The spout collar concentrates the endpoint.** At the bottom, the spout protrudes through a reinforced collar, typically 25-30mm in diameter. The bag tapers into this collar over the final 15-20mm of length. The geometric endpoint of the bag for bounding-box purposes is the spout centerline, but the bag body itself ends 15-20mm before that.

### 2b. Quantifying the flexibility benefit

Conservative estimates for each flexibility effect:

| Effect | Depth Savings | Confidence |
|--------|--------------|------------|
| Mid-span sag (catenary shortening) | 3-8mm | Medium -- depends on sag magnitude and angle |
| Sealed-end fold-over (top mount) | 5-10mm | High -- the weld zone is always empty |
| Spout protrusion past lower mount | 5-10mm | High -- spout is rigid, bag body ends before it |
| Pillow taper at ends | 2-5mm | Medium -- filled bags taper at sealed end |

**Total potential savings: 15-33mm.** Even taking the conservative end (15mm), this shifts the break-even from 49 degrees to approximately 45 degrees. At the optimistic end (33mm), 40-degree territory becomes feasible.

### 2c. Compression of the stack

The 80mm stack thickness assumes two bags at 40mm compressed each. This is the owner's reported measurement from physical handling. The question is whether tighter compression is possible:

- **At 35mm per bag (70mm stack):** Each bag loses approximately 5mm of cross-section. For a bag with a center thickness of 38-50mm (full), compressing to 35mm means 3-15mm of squeeze. At the low end this is trivial -- the bag barely resists. At the high end it requires moderate spring force from the cradle.

- **At 30mm per bag (60mm stack):** This is a 25% reduction from the 40mm baseline. The bag is significantly deformed. Internal pressure rises, stressing the seals. Not recommended for long-term permanent installation without testing.

- **Trade-off:** Reducing T_stack from 80mm to 70mm saves sin(theta) x 10mm of depth. At 45 degrees, that is 7mm. At 40 degrees, 6.4mm. Helpful but not transformative on its own.

The effect on the depth formula at 45 degrees with T_stack = 70mm:

    Depth = 350 x cos(45) + 70 x sin(45) = 247.5 + 49.5 = 297mm

Still 5mm over the 292mm interior. But combined with the flexibility savings above (15mm conservative), effective depth drops to approximately 282mm -- well within 292mm.

---

## 3. Partial Fill Scenarios

### 3a. Why partial fill matters

A "2L bag" does not have to hold 2L. The bag is a flexible container whose shape depends entirely on fill volume. At partial fill:

- The unfilled portion of the bag collapses flat (two film layers, ~0.5mm total).
- The filled portion forms a pillow that is shorter than the full-bag pillow.
- The effective length of the pressurized zone decreases.
- The effective thickness also decreases (less liquid, thinner pillow).

### 3b. Estimated dimensions at partial fill

Using the elliptical pillow approximation (V = pi/4 x W x L_eff x T_center) and assuming the liquid concentrates at the low end (connector) when mounted on a diagonal:

| Fill Level | Volume | Estimated L_eff | Estimated T_center | Notes |
|-----------|--------|----------------|-------------------|-------|
| 100% (2.0L) | 2000ml | 350mm | 38-50mm | Full bag, maximum dimensions |
| 90% (1.8L) | 1800ml | ~330mm | 36-48mm | Upper 20mm collapses flat |
| 75% (1.5L) | 1500ml | ~290-300mm | 34-44mm | Upper 50-60mm collapsed |
| 50% (1.0L) | 1000ml | ~230-250mm | 28-36mm | Upper half mostly collapsed |

These are estimates. On a diagonal mount, gravity concentrates the liquid at the low end, so the collapse progresses from the sealed (high) end downward. The effective length shrinks as the upper portion goes flat.

### 3c. The 1.5-1.8L sweet spot

At 1.5L fill in a 2L bag:
- Effective length drops to approximately 290-300mm.
- At 40 degrees: depth = 300 x cos(40) + 70 x sin(40) = 230 + 45 = 275mm. Fits with 17mm margin.
- At 35 degrees: depth = 300 x cos(35) + 70 x sin(35) = 246 + 40 = 286mm. Fits with 6mm margin.

At 1.8L fill:
- Effective length approximately 330mm.
- At 45 degrees: depth = 330 x cos(45) + 70 x sin(45) = 233 + 49 = 282mm. Fits with 10mm margin.
- At 40 degrees: depth = 330 x cos(40) + 70 x sin(40) = 253 + 45 = 298mm. 6mm over.

**Conclusion:** If the operating fill is 1.5L in a 2L bag, the depth problem largely disappears at 35-40 degrees. At 1.8L fill, the range is 43-45 degrees. This is meaningful: 1.5L is still 50% more capacity than 1L, and the refill interval extends proportionally. A 2L bag filled to 1.5L is a valid product strategy -- the extra bag volume provides headroom for overfill and ensures the bag never strains at the seals.

**However**, this introduces a firmware/UX constraint: the system must stop filling at 1.5L or 1.8L rather than maxing out the bag. Since the pump controls fill volume precisely, this is trivially enforceable. The user never interacts with the bags directly, so they would not know or care that the bag has unused capacity.

---

## 4. Mount Point Geometry

### 4a. The 350mm is between extremes -- mounts do not have to be at extremes

The rigid-body bounding box assumes the bag stretches its full 350mm along the diagonal. But the mount points determine the effective diagonal span, and they do not have to be at the absolute ends of the bag.

**Top mount (sealed end):** The sealed end is a 10-15mm heat-sealed seam. If the top mount is a clamp or channel that the sealed end folds over (the bag doubles back on itself for 15-20mm), the effective mount point is 15-20mm inward from the bag end. This shortens the diagonal span.

**Bottom mount (connector end):** The spout protrudes 15-20mm from the bag body. If the lower mount secures the bag body (not the spout), and the spout extends past the mount into or through the back wall, the effective diagonal span is 15-20mm shorter at this end too.

### 4b. Combined mount point savings

If both ends contribute 15mm of reclaimed length:

    Effective diagonal span = 350 - 15 - 15 = 320mm

At 45 degrees with T_stack = 80mm:

    Depth = 320 x cos(45) + 80 x sin(45) = 226 + 57 = 283mm

That fits in 292mm with 9mm margin. At 43 degrees:

    Depth = 320 x cos(43) + 80 x sin(43) = 234 + 55 = 289mm

3mm margin. Tight but physically possible.

At 40 degrees:

    Depth = 320 x cos(40) + 80 x sin(40) = 245 + 51 = 296mm

4mm over. Does not fit with mount geometry alone.

### 4c. Combined with stack compression (T_stack = 70mm)

At 43 degrees:

    Depth = 320 x cos(43) + 70 x sin(43) = 234 + 48 = 282mm

Fits with 10mm margin.

At 40 degrees:

    Depth = 320 x cos(40) + 70 x sin(40) = 245 + 45 = 290mm

Fits with 2mm margin. Extremely tight, but physically within the envelope.

---

## 5. Angle Optimization at 292mm Interior

### 5a. Minimum angle for rigid-body fit

Setting depth = 292mm and solving for theta:

    350 x cos(theta) + 80 x sin(theta) = 292

This is a transcendental equation. Numerical solution: theta approximately 49 degrees (confirmed by the sweep table: 290mm at 49 degrees).

### 5b. With combined savings (effective L=320, T_stack=70)

    320 x cos(theta) + 70 x sin(theta) = 292

Numerical solution: theta approximately 38-39 degrees.

### 5c. Height consumed at the minimum angle

At 49 degrees (rigid body):

    Height = 350 x sin(49) + 80 x cos(49) = 264 + 52 = 316mm

Remaining height in 392mm interior: 76mm.

At 43 degrees (with mount geometry, T_stack=80):

    Height = 320 x sin(43) + 80 x cos(43) = 218 + 59 = 277mm

Remaining height: 115mm.

At 39 degrees (with mount geometry + compression):

    Height = 320 x sin(39) + 70 x cos(39) = 201 + 54 = 255mm

Remaining height: 137mm.

### 5d. Does the cartridge fit?

The cartridge (150W x 130D x 80H) sits in the front-bottom triangle. The triangle's height at the front wall equals L_eff x sin(theta):

- At 49 degrees (rigid): 350 x sin(49) = 264mm. Cartridge (80mm tall) fits with 184mm clearance.
- At 43 degrees (mount adjusted): 320 x sin(43) = 218mm. Cartridge fits with 138mm clearance.
- At 39 degrees (all savings): 320 x sin(39) = 201mm. Cartridge fits with 121mm clearance.

The cartridge fits comfortably at all candidate angles. The front-bottom triangle is always generous for the cartridge in 2L configurations because the longer bag creates a taller triangle.

### 5e. Drainage quality

| Angle | sin(theta) | Drainage Quality |
|-------|-----------|-----------------|
| 39    | 0.63      | Very good       |
| 43    | 0.68      | Very good       |
| 45    | 0.71      | Excellent       |
| 49    | 0.75      | Excellent       |
| 50    | 0.77      | Excellent       |

All candidate angles are at or above 39 degrees, where gravity drainage is very good. There is no drainage concern at any of these angles.

---

## 6. Enclosure Wall Thickness Trade-Off

### 6a. The impact of wall thickness on interior depth

| Wall Thickness | Interior Depth (300mm exterior) | Gain vs 4mm |
|---------------|-------------------------------|-------------|
| 4.0mm (baseline) | 292mm | -- |
| 3.5mm | 293mm | +1mm |
| 3.0mm | 294mm | +2mm |
| 2.5mm | 295mm | +3mm |
| 2.0mm | 296mm | +4mm |

### 6b. At what wall thickness does the math work?

For rigid-body fit at 45 degrees: depth needed = 304mm. No wall thickness reduction makes this work (would need negative walls).

For mount-geometry-adjusted fit (L_eff=320, T_stack=80) at 45 degrees: depth needed = 283mm. Already fits at 4mm walls (292mm interior, 9mm margin).

For mount-geometry-adjusted fit at 43 degrees: depth needed = 289mm. Fits at 4mm walls with 3mm margin. Thinner walls help but are not necessary.

For rigid-body fit at 48 degrees: depth needed = 294mm. Fits at 3mm walls (294mm interior). Does not fit at 4mm walls (292mm interior).

### 6c. Structural implications of thinner walls

For injection-molded ABS or polycarbonate enclosures:

- **4mm walls:** Standard for consumer electronics enclosures of this size. Provides good rigidity and impact resistance. No concerns.
- **3mm walls:** Within the recommended range for ABS (1.1-3.5mm). Adequate stiffness for a stationary under-sink box. May need internal ribs at 100-150mm spacing on the larger panels (the 300x400mm side panels) to prevent flex.
- **2.5mm walls:** Still within ABS range. Requires ribs on all panels larger than 150mm span. Acceptable for a non-structural enclosure that sits on a flat surface and is not handled frequently.
- **2mm walls:** Minimum recommended for ABS enclosures of this size. Requires dense ribbing. Panels will feel flimsy without ribs. Not recommended for a consumer product that needs to feel solid.

**Verdict:** 3mm walls are viable with modest ribbing, gaining 2mm of interior depth (294mm). This is helpful but not transformative. The primary depth recovery comes from bag flexibility and mount geometry, not wall thickness. A 3mm wall is a reasonable design choice on its own merits (lighter, less material cost) but should not be relied upon to make the depth math work.

For sheet metal construction (0.8-1.2mm steel or aluminum), the interior would be 297-298mm. This makes rigid-body fit possible at 48 degrees. Sheet metal is viable for a commercial product but changes the manufacturing approach significantly.

---

## 7. The 50-Degree Option Honestly Evaluated

### 7a. Rigid-body fit at 50 degrees

At 50 degrees with the standard dimensions (L=350, T_stack=80):

    Depth = 350 x cos(50) + 80 x sin(50) = 225 + 61 = 286mm

Fits in 292mm interior with 6mm margin. This is the simplest answer.

    Height = 350 x sin(50) + 80 x cos(50) = 268 + 51 = 319mm

Remaining height: 392 - 319 = 73mm.

### 7b. Is 73mm of remaining height a problem?

Previous research flagged this as tight. But 73mm is the space above the TOP of the bag slab. The question is: what needs to go there?

**Electronics:** ESP32 module (~25mm tall mounted horizontally), L298N driver board (~20mm), fuse block (~15mm). These can be stacked or placed side-by-side. Total height needed: approximately 30-50mm when laid flat. 73mm accommodates this.

**The hopper does NOT go in the top-back zone.** The hopper sits at the top-front of the enclosure, where the bags are at their lowest point. The relevant metric for the hopper is the space above the bags at the FRONT wall, not the space above the bags at the back wall.

### 7c. Front-top triangle at 50 degrees

At the front wall (depth = 0), the top of the bag slab is at:

    Height at front = L x sin(theta) = 350 x sin(50) = 268mm (above the bag's bottom-back origin)

Wait -- this needs more careful geometry. The bag slab is positioned with its connector end at the floor near the back wall. The sealed end is at the top-front. At 50 degrees:

- The sealed end (top-front corner of bag) is at height = L x sin(50) + T_stack x cos(50) = 268 + 51 = 319mm above the floor, and at depth = 0 from the front (assuming the bag's front face is flush with the front wall).

- But the bag slab does not start at the front wall. The front gap = interior_depth - depth_consumed = 292 - 286 = 6mm. The bag's upper-front corner is 6mm behind the front wall.

At the front wall itself, there is 392mm of full height available (no bag). The bag starts 6mm back. Between 0 and 6mm depth, the entire 392mm height is open. This is not useful for the hopper (only 6mm deep).

At the hopper depth (say, 50-80mm from the front wall), the available height above the bag is:

    Bag lower surface at depth d = tan(theta) x (interior_D - d) measured from the floor at the back

Actually, let me recompute using the coordinate system from v1-diagonal-bag-placement. The lower bag surface (bottom of the slab) runs from the connector end at (depth=292, height=0) to the sealed end at (depth=292-L*cos(50), height=L*sin(50)) = (depth=292-225, height=268) = (depth=67, height=268).

At depth d, the height of the lower bag surface is:

    h_lower(d) = 268 x (292 - d) / (292 - 67) = 268 x (292 - d) / 225

At d = 0 (front wall): h_lower = 268 x 292/225 = 348mm. Height above bag at front wall = 392 - 348 = 44mm.

Wait, that does not work either -- the bag slab is only 286mm deep, so at depth=0, we are 6mm in front of the bag. Let me be more precise.

The bag slab occupies depth 6mm to 292mm. At d < 6mm, there is no bag (full height available). At d = 6mm:

    h_lower(6) = tan(50) x (292 - 6) = 1.19 x 286 = 341mm

Height above the top of the bag slab at d=6: the top surface is at h_lower + T_stack*cos(theta) perpendicular translated, but for the bounding box the top is at 319mm everywhere within the slab footprint.

The key insight: **the bag slab is a rectangular slab tilted at 50 degrees, and its top surface is a straight line parallel to the bottom surface, displaced by T_stack perpendicular to the incline.** The bounding box top is 319mm. At the front face of the bag (d=6mm), the top of the bag is at 319mm. Above that: 392 - 319 = 73mm.

But the front face of the bag is only 6mm from the front wall. At d=0 (the actual front wall), the full 392mm is available. So the hopper could mount right at the front wall with its back face 6mm from the front wall, getting the full enclosure height.

More practically, the hopper funnel occupies perhaps 50-80mm of depth at the top. Within that first 6mm there is full height. From 6-80mm the bag is present but the space above the bag drops from 73mm (at d=6) to... well, the top of the bag slab is at 319mm across the entire slab. So from d=6 to d=292, the space above the bag is consistently 73mm.

**The space BELOW the bag at the front is the useful zone for the cartridge.** At d=80 (cartridge back face):

    h_lower(80) = tan(50) x (292 - 80) = 1.19 x 212 = 253mm

The cartridge (80mm tall) fits under a 253mm ceiling with 173mm of clearance. No problem.

### 7d. Honest assessment of 50 degrees

**Positives:**
- Fits cleanly with no flexibility assumptions, no mount geometry tricks, no partial fill.
- 6mm depth margin is small but real.
- Drainage is excellent (sin(50) = 0.77).
- The front-bottom triangle is enormous (225mm deep at the floor, 268mm tall at the front wall) -- the cartridge sits in open space.
- 73mm of height above the bag slab is enough for electronics.

**Negatives:**
- Height consumption is 319mm of 392mm available. Only 73mm remains above the bags.
- The hopper at the front-top must work within the 73mm zone (above the bag slab) or the narrow 6mm front gap (before the bag slab starts). This is architecturally awkward -- the hopper needs at least 100mm of height and 60-80mm of depth for a practical funnel.
- At 50 degrees, the bags are closer to vertical than horizontal. The visual impression is of bags standing up rather than lying at an angle. This is not inherently bad, but it changes the user's mental model during manufacturing assembly.

**The hopper problem is real.** The hopper needs to sit above the bags at the front, with enough volume to accept a pour of concentrate (200-300ml capacity). At 50 degrees, the space above the bags at the front is only 73mm tall. A funnel that is 73mm tall and, say, 150mm wide x 100mm deep holds roughly 73 x 150 x 100 x 0.4 (funnel factor) = 438ml. That is technically sufficient. But the funnel opening would be very wide and shallow -- more of a trough than a funnel. This works, but the pouring experience is different from a deep funnel.

**Verdict:** 50 degrees is a viable fallback. It is not a bad design. The hopper geometry is the main architectural concern, and it is solvable with a wide, shallow funnel design. Previous research was not wrong to flag 50 degrees as tight, but it overstated the problem by measuring "remaining height above bags" as a single number without considering where that height is needed.

---

## 8. Alternative Bag Models and Custom Pouches

### 8a. Other off-the-shelf options

The Platy 2L (350mm x 190mm) was selected for its wide mouth, food-safe materials, and robust connector system. Other options:

**Hoser 2.0L (152mm x 406mm):** Longer, not shorter. Makes the depth problem worse. Rejected.

**Stand-up spout pouches (generic):** Food-grade spout pouches are available from manufacturers like ePac, Impak, and Bowe Pack in custom sizes. A typical 2L spout pouch might be approximately 250mm wide x 300mm tall (stand-up orientation). Laid flat, that is 300mm long x 250mm wide. The shorter length helps with depth -- at 45 degrees, depth = 300 x cos(45) + 70 x sin(45) = 212 + 49 = 261mm. Fits easily. But these pouches have different connector systems (press-fit spouts rather than threaded caps), and the wider profile (250mm) would not fit two bags side-by-side in a 272mm enclosure. Stacked, the 250mm width is fine (single bag width < 272mm).

**Wine bag-in-box bladders:** Available in 1.5L and 3L sizes with spout fittings. Typically shorter and wider than Platypus bags. A 1.5L bladder might be approximately 250mm x 200mm flat. These are food-grade and designed for long-term liquid storage. However, they are not designed for repeated fill/drain cycles and may not withstand thousands of pump-assisted refills.

### 8b. Custom pouch option

Custom flexible pouches can be ordered from manufacturers like ePac Flexibles, Polysmarts, and VistaPrint:

- **Minimum order quantities:** As low as 100 units (VistaPrint) to 1,000 units (Polysmarts, ePac). For a commercial product, MOQs of 1,000-5,000 are typical for custom food-grade pouches.
- **Custom dimensions:** Manufacturers can produce pouches in arbitrary dimensions. A 250mm x 250mm flat pouch with a threaded spout could hold approximately 1.5-2.0L depending on pillow thickness.
- **Lead time:** 2-4 weeks for prototypes, 4-8 weeks for production runs.
- **Cost:** $0.50-2.00 per pouch at 1,000-unit quantities, depending on material and features.

A custom pouch designed specifically for this application could be:
- **300mm long x 220mm wide** (fits 272mm enclosure stacked, shorter than Platypus)
- **Threaded 28mm spout** (compatible with existing connector design)
- **Food-grade PE or PET/PE laminate**
- **Capacity: ~1.8-2.0L** depending on pillow geometry

At 300mm length, the depth math at 45 degrees: 300 x cos(45) + 70 x sin(45) = 212 + 49 = 261mm. Fits with 31mm margin. At 40 degrees: 300 x cos(40) + 70 x sin(40) = 230 + 45 = 275mm. Fits with 17mm margin.

**This is the cleanest solution if Platypus bags are not a hard requirement.** A custom 300mm x 220mm pouch eliminates the depth problem entirely at 40-45 degrees while maintaining 1.8-2.0L capacity.

### 8c. Two 1L bags per flavor (4 bags total)

The Platy 1L (280mm x 152mm) fits easily at 35 degrees in 292mm depth:

    Depth = 280 x cos(35) + 50 x sin(35) = 229 + 29 = 258mm (34mm margin)

Four bags (two per flavor) stacked in two pairs: each pair is 50mm thick, so two pairs side-by-side need 2 x 152mm = 304mm width (does not fit in 272mm). Two pairs stacked vertically: total stack thickness = 100mm. At 35 degrees:

    Depth = 280 x cos(35) + 100 x sin(35) = 229 + 57 = 286mm (6mm margin)
    Height = 280 x sin(35) + 100 x cos(35) = 161 + 82 = 243mm

This works, but 100mm stack thickness is substantial and the 6mm depth margin is tight. More importantly, plumbing becomes complex -- each flavor line needs to draw from two bags in sequence or in parallel, requiring additional valves or a manifold.

**Verdict:** Two 1L bags per flavor is technically feasible but adds significant plumbing complexity for modest capacity gain (2L total per flavor, same as one 2L bag). Not recommended over a custom 2L pouch.

---

## 9. Diagonal with Bag Ends Protruding into Corner Voids

### 9a. Back-wall integration

The connector end of the bag sits at the bottom-back corner. The back wall has push-to-connect fittings for water lines. Could the bag connector integrate directly with the back panel?

If the bag's spout passes through the back wall:
- The bag body ends 15-20mm inside the enclosure (before the spout).
- The spout occupies a hole in the back panel.
- The fluid connection is on the exterior of the back panel.
- This eliminates internal tube routing from the connector to the back wall and reclaims 15-20mm of depth (the spout no longer occupies interior space).

**Effective result:** The bag's functional length inside the enclosure is 350 - 20 = 330mm (spout protrudes through the wall). The rigid-body depth at 45 degrees:

    Depth = 330 x cos(45) + 80 x sin(45) = 233 + 57 = 290mm

Fits with 2mm margin. At 43 degrees:

    Depth = 330 x cos(43) + 80 x sin(43) = 241 + 55 = 296mm

4mm over. Does not fit alone.

### 9b. Combined with top fold-over

If the sealed end also folds over the top mount (15mm), effective internal length = 330 - 15 = 315mm. At 43 degrees:

    Depth = 315 x cos(43) + 80 x sin(43) = 230 + 55 = 285mm

Fits with 7mm margin.

At 40 degrees:

    Depth = 315 x cos(40) + 80 x sin(40) = 241 + 51 = 292mm

Exactly fits (0mm margin). Not practical -- zero margin means any variation breaks it.

At 41 degrees:

    Depth = 315 x cos(41) + 80 x sin(41) = 238 + 52 = 290mm

Fits with 2mm margin.

### 9c. Feasibility of back-wall spout integration

This is architecturally interesting but has complications:
- The spout must seal through the back wall (gasket or o-ring required).
- The bag is a permanent fixture, so this is a one-time assembly step during manufacturing -- acceptable.
- The back wall becomes structurally connected to the bag mount, complicating disassembly for service.
- Fluid connections are now on the exterior of the back wall, which simplifies internal routing but means the spout and tubing are exposed when the enclosure is pulled out for service.

**Verdict:** Back-wall spout integration is a meaningful design option. It reclaims 15-20mm of effective depth and simplifies internal tube routing. Combined with sealed-end fold-over, it enables 43-45 degree angles for 2L bags at 292mm interior depth. Worth prototyping.

---

## 10. Synthesis: What Combination Gets 2L Bags into 300mm Depth?

### 10a. Summary of depth savings by technique

| Technique | Depth Saved | Reliability |
|-----------|-------------|------------|
| Sealed-end fold-over (15mm off L) | 11-13mm at 40-50 deg | High |
| Spout protrusion past mount (15mm off L) | 11-13mm at 40-50 deg | High |
| Stack compression (70mm vs 80mm) | 5-7mm at 40-50 deg | Medium |
| Mid-span sag / pillow taper | 3-8mm | Medium-low |
| Wall thickness 3mm vs 4mm | 2mm | High |
| Back-wall spout integration (20mm off L) | 14-17mm at 40-50 deg | Medium (design change) |

### 10b. Scenario analysis

**Scenario A: Conservative (high-confidence savings only)**

- Sealed-end fold-over: L_eff = 335mm
- Standard compression: T_stack = 80mm
- 4mm walls: interior = 292mm

At 47 degrees: 335 x cos(47) + 80 x sin(47) = 228 + 59 = 287mm. Fits with 5mm margin.
At 45 degrees: 335 x cos(45) + 80 x sin(45) = 237 + 57 = 294mm. 2mm over.

**Result: 47 degrees minimum with conservative assumptions.** This is only 3 degrees better than the rigid-body 50 degrees.

**Scenario B: Moderate (fold-over + spout protrusion + mild compression)**

- Both ends adjusted: L_eff = 320mm
- Mild compression: T_stack = 70mm
- 4mm walls: interior = 292mm

At 43 degrees: 320 x cos(43) + 70 x sin(43) = 234 + 48 = 282mm. Fits with 10mm margin.
At 40 degrees: 320 x cos(40) + 70 x sin(40) = 245 + 45 = 290mm. Fits with 2mm margin.

**Result: 43 degrees comfortably, 40 degrees at the margin.** This is a meaningful improvement.

**Scenario C: Aggressive (back-wall integration + fold-over + compression + thinner walls)**

- Back-wall spout (20mm) + fold-over (15mm): L_eff = 315mm
- Compression: T_stack = 70mm
- 3mm walls: interior = 294mm

At 40 degrees: 315 x cos(40) + 70 x sin(40) = 241 + 45 = 286mm. Fits with 8mm margin.
At 38 degrees: 315 x cos(38) + 70 x sin(38) = 248 + 43 = 291mm. Fits with 3mm margin.

**Result: 38-40 degrees achievable with aggressive but physically plausible design choices.**

**Scenario D: Custom pouch (eliminates the problem)**

- Custom 300mm x 220mm pouch, 2L capacity
- T_stack = 70mm (thinner pouch material)
- 4mm walls: interior = 292mm

At 40 degrees: 300 x cos(40) + 70 x sin(40) = 230 + 45 = 275mm. Fits with 17mm margin.
At 35 degrees: 300 x cos(35) + 70 x sin(35) = 246 + 40 = 286mm. Fits with 6mm margin.

**Result: 35-40 degrees with comfortable margins.**

### 10c. Height consequences at each scenario

| Scenario | Minimum Angle | Height Consumed | Remaining Height | Top-Back Void |
|----------|--------------|-----------------|-----------------|---------------|
| Rigid body (50 deg) | 50 | 319mm | 73mm | 73mm |
| A: Conservative (47 deg) | 47 | 306mm | 86mm | 86mm |
| B: Moderate (43 deg) | 43 | 277mm | 115mm | 115mm |
| C: Aggressive (40 deg) | 40 | 257mm | 135mm | 135mm |
| D: Custom pouch (35 deg) | 35 | 212mm | 180mm | 180mm |

Scenario B (43 degrees, 115mm remaining) and Scenario C (40 degrees, 135mm remaining) both provide substantially more headroom for electronics and the hopper than the 50-degree fallback.

---

## 11. Recommendations

### 11a. The answer to "can 2L bags fit at 300mm depth?"

**Yes, but not at 35 degrees with rigid-body assumptions.** The rigid-body math is clear: 2L Platypus bags at 35 degrees need 333mm of depth. No amount of wall thinning or compression closes a 41mm gap.

However, the rigid-body model overstates the problem. The bag is a flexible pouch, not a rigid panel. Mount point geometry reclaims 20-35mm of effective length. Mild stack compression saves 5-7mm. Combined, these effects shift the break-even angle from 49-50 degrees down to 43-45 degrees (moderate assumptions) or 38-40 degrees (aggressive assumptions).

### 11b. Recommended path forward

1. **Prototype at 45 degrees first.** This is the lowest-risk angle that plausibly fits 2L Platypus bags in 292mm depth with mount-geometry savings. Build a mockup cradle, mount two filled 2L bags, and measure the actual bounding box. If the measured depth is under 292mm, the problem is solved.

2. **Test mount-point geometry.** Physically fold the sealed end over a mount bracket and measure how much length is reclaimed. Physically let the spout protrude past the lower mount. Measure the effective diagonal span between functional mount points.

3. **Measure actual stack compression.** Place two full 2L bags on a flat surface with a weighted plate on top. Measure thickness at 5N, 10N, 20N of compression force. Determine whether 70mm (35mm per bag) is achievable without risk.

4. **Evaluate back-wall spout integration.** If prototyping shows the depth is still 5-10mm too much at 45 degrees, design a back-wall spout pass-through. This is the single most impactful design change (15-20mm of depth recovery) and has side benefits (simpler internal routing).

5. **Evaluate custom pouches in parallel.** If the Platypus bag is not a hard requirement, a custom 300mm-long food-grade pouch eliminates the depth problem entirely. Prototype quantities (100-1,000 units) are affordable and available with 2-4 week lead times.

6. **Partial fill as fallback strategy.** If physical testing shows 2L bags at 45 degrees consume 296-300mm of depth (a few mm over), operating at 1.8L fill rather than 2.0L fill may provide just enough dimensional relief. The firmware can enforce this limit trivially.

### 11c. What the owner should know

The previous research was not wrong -- it was incomplete. The rigid-body math is correct for rigid bodies. The gap in the analysis was treating a flexible pouch as a rigid panel. When real-world bag behavior is modeled (fold-over, spout protrusion, sag, compression), the 2L bags become feasible at 43-47 degrees in the 300mm enclosure, depending on how many design accommodations are made.

The cleanest path to 2L capacity at 300mm depth is a custom pouch (shorter and wider than Platypus). If Platypus bags are preferred, 45 degrees is the target angle, with physical prototyping needed to confirm fit. The 50-degree fallback remains viable if prototyping shows 45 degrees is too tight.

The 300mm depth constraint is defensible. It does not require giving up on 2L capacity. It requires either (a) accepting a steeper angle than the ideal 35-40 degrees, (b) exploiting bag flexibility through clever mount design, or (c) switching to a shorter custom pouch.

---

## 12. Open Questions for Physical Testing

1. What is the actual bounding box of two full 2L Platy bags mounted at 45 degrees on a rail cradle? (The single most important measurement.)
2. How much of the sealed end can fold over a mount bracket without stressing the seal?
3. What stack compression is achievable at 5N, 10N, 20N sustained force?
4. Does mid-span sag measurably reduce the projected depth footprint, or is the effect too small to matter?
5. Can the Platypus spout reliably seal through a back-wall pass-through with a gasket?
6. At 1.8L fill, what are the actual bag dimensions when mounted at 45 degrees?
7. What is the lead time and unit cost for a custom 300mm x 220mm food-grade spout pouch at 500-unit quantities?
