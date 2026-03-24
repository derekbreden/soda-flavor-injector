# Incline Bag Mounting — Two-Point Stretched Mount for Platypus Bags

This document analyzes a novel bag mounting approach: stretching each Platypus bag at an incline between two fixed points inside the enclosure, with the sealed top end elevated and the connector/outlet end at the low point. This replaces the vertical hanging approach analyzed in prior research.

---

## 1. The Insight

Prior research (hopper-and-bag-management.md, bag-zone-geometry.md) focused on two orientations: vertical hanging (connector at bottom, requiring 250mm+ of vertical space) and flat on tilted cradles (requiring only ~100mm but with uncertain drainage). Both approaches have significant tradeoffs in the 400mm-tall enclosure.

The incline mount is a third option: stretch the bag diagonally between a high mount point (sealed end) and a low mount point (connector end). The bag is held taut under mild tension, filling the available space diagonally rather than consuming height vertically.

**Key advantages over vertical hanging:**
- Uses diagonal space, dramatically reducing the vertical height requirement
- Bag is held between two fixed points, constraining collapse to thinning-in-place rather than random folding
- The connector end is at the lowest point, so gravity pulls all liquid toward the outlet

**Key advantages over flat cradles:**
- Gravity still assists drainage (the incline creates a definite low point)
- The dip tube extends from the low connector UP along the incline, reaching deeper into the bag interior
- No uncertainty about drainage behavior -- the geometry is clear and predictable

---

## 2. Geometry and Space Analysis

### 2a. 1L Platypus Bag Dimensions

The active design uses 1L Platypus bags (per bag-zone-geometry.md decision to lock enclosure height at 400mm).

| Parameter | Value |
|---|---|
| Height (when full) | ~250mm |
| Width | ~140mm |
| Thickness (when full) | ~40mm |
| Connector | 28mm threaded cap at one narrow end |
| Sealed end | Heat-sealed seam at opposite narrow end |

### 2b. Available Bag Zone

From the active design in bag-zone-geometry.md:

| Parameter | Value |
|---|---|
| Enclosure exterior | 280W x 250D x 400H mm |
| Interior | 272W x 242D x 392H mm |
| Bag zone vertical range | 15mm (drip tray top) to 180mm (dock shelf bottom) |
| **Bag zone height** | **165mm** |
| **Bag zone depth** | **242mm** (full interior depth) |
| **Bag zone width** | **272mm** (full interior width) |

### 2c. Incline Geometry at Various Angles

When the bag (250mm long) is mounted at an incline, trigonometry determines the horizontal run and vertical rise:

```
    Sealed end (high)
        *
       /|
      / |  vertical rise = L * sin(theta)
     /  |
    /   |
   / θ  |
  *─────┘
  Connector    horizontal run = L * cos(theta)
  (low)
```

| Angle (from horizontal) | Vertical Rise (mm) | Horizontal Run (mm) | Fits 165mm height? | Fits 242mm depth? |
|---|---|---|---|---|
| 20 deg | 85 | 235 | Yes (85 < 165) | Yes (235 < 242) |
| 25 deg | 106 | 227 | Yes (106 < 165) | Yes (227 < 242) |
| 30 deg | 125 | 217 | Yes (125 < 165) | Yes (217 < 242) |
| 35 deg | 143 | 205 | Yes (143 < 165) | Yes (205 < 242) |
| 40 deg | 161 | 191 | Barely (161 < 165) | Yes (191 < 242) |
| 45 deg | 177 | 177 | No (177 > 165) | Yes (177 < 242) |

### 2d. Sweet Spot Analysis

The bag zone is 165mm tall and 242mm deep. The bag is 250mm long. Let us find the angle where the bag uses the space most efficiently.

**Constraint 1: Vertical rise must be less than 165mm.**
- Max angle: arcsin(165/250) = 41.3 degrees

**Constraint 2: Horizontal run must be less than 242mm.**
- Max angle for this to be binding: arccos(242/250) = 14.4 degrees (only constrains at very shallow angles -- not an issue)

**Constraint 3: Bag thickness adds to vertical space consumption.**
The bag is ~40mm thick when full. At an incline, the bag's cross-section projects onto the vertical axis as approximately `thickness * cos(theta)`:

| Angle | Bag body vertical projection (mm) | Total vertical (rise + thickness) |
|---|---|---|
| 20 deg | 38 | 85 + 38 = 123 |
| 25 deg | 36 | 106 + 36 = 142 |
| 30 deg | 35 | 125 + 35 = 160 |
| 35 deg | 33 | 143 + 33 = 176 -- exceeds 165mm |

When bag thickness is included, the maximum practical angle drops to approximately **30 degrees**.

**Constraint 4: Clearance for bag installation and tubing.**
The connector end needs ~15-25mm below for the cap, drink tube adapter, and tubing routing. The sealed end needs ~10mm above for the mounting clip.

Working clearances into the budget:

```
    165mm total bag zone height
    - 15mm  bottom clearance (connector + tubing at drip tray level)
    - 10mm  top clearance (clip/mount hardware at shelf underside)
    = 140mm usable vertical span for the bag body
```

With 140mm usable: max angle = arcsin(140/250) = 34.1 degrees. After subtracting thickness projection (~34mm at 30 deg): effective max ~30 degrees.

**Recommended angle: 25-30 degrees from horizontal.**

At 30 degrees:
- Vertical rise: 125mm (fits in 140mm usable with 15mm margin)
- Horizontal run: 217mm (fits in 242mm depth with 25mm margin for tubing at the rear)
- Total vertical with thickness: ~160mm (fits in 165mm with 5mm margin)
- Gravity component along the bag axis: sin(30) = 0.5 -- half of gravitational acceleration drives liquid toward the connector

---

## 3. Two-Bag Arrangement

### 3a. Side by Side (Recommended)

Two 1L bags at 30 degrees, placed side by side within the 272mm interior width:

```
    TOP VIEW (looking down into bag zone)

    ┌───────────────────── 272mm interior ─────────────────────┐
    │                                                          │
    │  ┌──────────────┐      ┌──────────────┐                 │
    │  │              │      │              │                  │
    │  │    BAG 1     │      │    BAG 2     │    remaining     │
    │  │   140mm W    │      │   140mm W    │    depth for     │
    │  │   ~40mm D    │      │   ~40mm D    │    tubing        │
    │  │              │      │              │    (~25mm)       │
    │  └──────────────┘      └──────────────┘                  │
    │                                                          │
    │  ◄── 140mm ──►  gap  ◄── 140mm ──►                      │
    │             ← ~8mm clearance per side →                  │
    │                                                          │
    │  FRONT                                          BACK     │
    │  ◄──────────────── 242mm depth ────────────────────►     │
    └──────────────────────────────────────────────────────────┘
```

Wait -- width of two bags (140 + 140 = 280mm) exceeds the 272mm interior. Side by side does NOT fit.

### 3b. Front-to-Back (Two Bags in Depth)

Two bags stacked in the depth direction, both at 30 degrees:

```
    SIDE VIEW (cross-section, looking from right side)

    ════════════ DOCK SHELF (180mm) ══════════════════
       ↑clip A2                           ↑clip B2
       │                                  │
       │  *───────── BAG 2 ──────────*    │
       │ /    30 deg                       │
       │/                                  │
       *clip A1                           *clip B1
                                    ↗ tubing
    ════════════ DRIP TRAY (15mm) ══════════════════
    FRONT                                     BACK
    ◄──────────── 242mm depth ──────────────────►

    Where:
    - BAG 1 connector (clip A1) at front-low
    - BAG 1 sealed end (clip A2) at front-high
    - BAG 2 connector (clip B1) behind BAG 1 connector
    - BAG 2 sealed end (clip B2) behind BAG 1 sealed end
```

Actually, let me reconsider the arrangement. Each bag spans 217mm of depth at 30 degrees. The enclosure interior is 242mm deep. Two bags both running front-to-back would overlap in depth -- they cannot both span the full 217mm.

**Stacked vertically at different heights:**

Two bags, both inclined at 30 degrees, one above the other. Each bag needs ~80mm of vertical space (40mm body + clearance). Two bags stacked: 160mm. The bag zone is 165mm. This is extremely tight.

**Staggered: one high-front/low-back, one low-front/high-back:**

```
    SIDE VIEW (cross-section)

    ════════════ DOCK SHELF (180mm) ══════════════════
                                   clip 1 (sealed)
                                  /
                BAG 1            /
              ──────────────── *     ← 30 deg, sealed end at rear-top
             /
    clip 1  *
    (conn.)     clip 2 (sealed)
               /
    BAG 2     /
    ──────── *                       ← 30 deg, sealed end at front-top
            /
    clip 2 *
    (conn.)
    ════════════ DRIP TRAY (15mm) ══════════════════
    FRONT                                     BACK
```

This is interesting but creates a complex mounting arrangement and the bags would physically interfere with each other.

**Recommended: Parallel, same direction, offset vertically**

The simplest arrangement: both bags incline in the same direction (connector at front-low, sealed end at rear-high), with one bag directly above the other:

```
    SIDE VIEW (cross-section)

    ════════════ DOCK SHELF UNDERSIDE (180mm) ════════
                                         * clip (sealed end, bag 2)
                                        /
                            BAG 2      /   ← upper bag
                                      /
                                     /
                          * clip    /
                         / (conn) *
                        /
            BAG 1      /               ← lower bag
                      /
                     /
          * clip    /
    (conn)         * clip (sealed end, bag 1)

    ════════════ DRIP TRAY (15mm) ══════════════════
    FRONT                                     BACK
    ◄──────────── 242mm depth ──────────────────►
```

This doesn't quite work either -- the stacking math needs to be precise. Let me work the vertical budget properly.

### 3c. Revised Two-Bag Vertical Budget

Each bag at 30 degrees occupies:
- Vertical rise of bag centerline: 125mm
- Bag thickness perpendicular to its surface: 40mm
- Vertical projection of thickness: 40 * cos(30) = 35mm

So each bag occupies a vertical band of approximately 35mm at any given depth point. The top of bag 1's body at the sealed end reaches 125 + 17 = 142mm (midline rise + half-thickness projection). The bottom of bag 1 at the connector end is at 0mm (the midline) minus 17mm = -17mm (below the connector mount point).

For two bags stacked with the lower bag connector at 15mm (drip tray top) + 15mm clearance = 30mm:

**Lower bag (Bag 1):**
- Connector center at height: 30mm
- Sealed end center at height: 30 + 125 = 155mm
- Body bottom edge at connector: 30 - 17 = 13mm (barely above drip tray)
- Body top edge at sealed end: 155 + 17 = 172mm (exceeds 180mm dock shelf by... wait, 172 < 180. OK.)

**Upper bag (Bag 2):**
- Needs to clear Bag 1's upper surface. Bag 1's top edge rises from ~47mm at the connector end to ~172mm at the sealed end.
- Bag 2's connector must start above Bag 1 at the same depth. At the front where Bag 1's top edge is ~47mm, Bag 2's bottom edge must be above 47mm. So Bag 2's connector center starts at ~47 + 17 + 5mm gap = 69mm.
- Bag 2 sealed end center: 69 + 125 = 194mm. Body top: 194 + 17 = 211mm. This is **well above the dock shelf at 180mm**. Does not fit.

**Two bags at 30 degrees stacked vertically do not fit in 165mm of vertical space.**

### 3d. Reducing the Angle for Two-Bag Fit

At a shallower angle, the vertical rise is less:

At 20 degrees:
- Vertical rise: 85mm
- Thickness projection: 40 * cos(20) = 38mm (half = 19mm)
- One bag vertical band: 85 + 38 = 123mm

For two bags at 20 degrees with offset:
- Bag 1 connector at 30mm, top edge at sealed end: 30 + 85 + 19 = 134mm
- Bag 2 connector at ~30 + 38 + 5 = 73mm, sealed end top: 73 + 85 + 19 = 177mm -- exceeds 165mm usable (180mm shelf - 15mm drip tray). Very tight at 177mm vs 180mm dock shelf.

This barely works but leaves essentially zero margin. The bags would press against each other.

### 3e. Same-Plane Side-by-Side at Reduced Width

Revisiting side-by-side: two bags need 280mm (140 x 2) but we have 272mm. The shortfall is only 8mm. Each bag could compress slightly when mounted (they are soft-sided), or a narrower bag variant may exist.

However, a more practical approach: **the bags don't need to be perfectly side by side. They can overlap slightly in depth while side by side:**

```
    TOP VIEW

    ┌────────────────── 272mm interior ──────────────────┐
    │                                                     │
    │  ┌────────────┐                                     │
    │  │   BAG 1    │   ← centered on left half          │
    │  │   140mm W  │                                     │
    │  │            │                                     │
    │  └────────────┘                                     │
    │        ┌────────────┐                               │
    │        │   BAG 2    │   ← offset ~8mm right         │
    │        │   140mm W  │      and ~20mm deeper          │
    │        │            │                               │
    │        └────────────┘                               │
    │                                                     │
    │  FRONT                                     BACK     │
    └─────────────────────────────────────────────────────┘
```

Actually, let me reconsider the entire problem. The bags are soft-sided and 40mm thick when full. At the connector end (low point), where both bags' tubing needs to route, the bags will be thinner (the liquid is distributed along the incline). The 140mm width is the bag's natural width -- but the bag is flexible and can be compressed slightly during mounting.

### 3f. Practical Arrangement: Front-to-Back, Single Incline Plane

The most practical arrangement is two bags both inclined in the same direction, one in front of the other, at the SAME height (not stacked):

```
    SIDE VIEW

    ════════════ DOCK SHELF (180mm) ══════════════════
                                  * sealed end
                                 /
                 BAG 1 or 2     /    ← both bags at 25 deg
                               /       in same incline plane
                              /
                    *────────*
               connector
    ════════════ DRIP TRAY (15mm) ══════════════════
    FRONT                                     BACK
```

```
    TOP VIEW (both bags at same height, front-to-back)

    ┌────────────────── 272mm interior ──────────────────┐
    │                                                     │
    │  ┌──────────────────────────────────────────┐      │
    │  │  BAG 1 (front)      BAG 2 (behind)       │      │
    │  │  140mm wide         140mm wide            │      │
    │  │  ~40mm deep         ~40mm deep            │      │
    │  │                                           │      │
    │  └──────────────────────────────────────────┘      │
    │                                                     │
    │  ◄── ~40mm ──►◄── ~40mm ──►◄── remaining ────►     │
    │     bag 1       bag 2        ~162mm for tubing      │
    │     depth       depth        and structure          │
    │                                                     │
    │  FRONT                                     BACK     │
    └─────────────────────────────────────────────────────┘
```

Wait -- the bags extend 217-227mm in the depth direction (at 25-30 degrees). Each bag is 40mm thick. Two bags front-to-back need 80mm of depth. But the bags' 217mm horizontal run also consumes depth. The bags are thin (40mm) and long (217mm) -- they lay along the depth axis, not across it.

Let me re-clarify: the bag's 250mm LENGTH runs along the incline (front-low to rear-high). The bag's 140mm WIDTH runs across the enclosure (left-right). The bag's 40mm THICKNESS is the third dimension (up-down, roughly perpendicular to the bag surface).

So in the TOP VIEW, each bag occupies approximately 140mm (left-right) x 217mm (front-to-back at 30 degrees). The 40mm thickness is mostly in the vertical direction.

Two bags side by side: 280mm left-right (doesn't fit 272mm).
Two bags front-to-back: each bag already spans 217mm of the 242mm depth. There isn't room for a second bag behind the first -- they'd overlap.

**The only way to fit two bags is to stack them in the thickness direction (one on top of the other in the remaining vertical space).** We showed above this doesn't work at 30 degrees.

### 3g. Solution: Lower Angle (20 Degrees) With Compressed Vertical Stacking

At 20 degrees:
- Horizontal run: 235mm (fits 242mm with 7mm margin)
- Vertical rise: 85mm
- Thickness projection: ~38mm per bag

Total vertical for two bags sharing the same depth footprint, one above the other with the lower bag resting on a cradle and the upper bag on a shelf above it:

```
    SIDE VIEW (two bags at 20 degrees, stacked)

    ════════════ DOCK SHELF UNDERSIDE (180mm) ════════
                                                 *─── BAG 2 sealed end (~163mm)
                                                /
                                    BAG 2      /
                                              /
                                      *──────* ─── BAG 2 connector (~78mm)
                                     ─── thin shelf/divider ───
                                    *─── BAG 1 sealed end (~135mm)
                                   /                       ↑ overlap is OK:
                       BAG 1      /                          BAG 1 top and BAG 2 bottom
                                 /                           are at different DEPTHS
                         *──────* ─── BAG 1 connector (~30mm)
    ════════════ DRIP TRAY (15mm) ══════════════════
    FRONT                                     BACK
```

Hmm, the vertical positions overlap because both bags span the same depth range. The key insight I was missing: at the FRONT of the enclosure, Bag 1's bottom edge is at ~30 - 19 = 11mm, and Bag 2 needs to clear Bag 1 at that same depth. But Bag 2 also extends all the way to the back -- at the BACK of the enclosure, Bag 1's top edge is at ~30 + 85 + 19 = 134mm, and Bag 2's bottom edge at that depth would be at 134 + 5 = 139mm, with Bag 2's top at 139 + 38 = 177mm. That fits under the 180mm dock shelf.

But at the FRONT, Bag 2's connector bottom edge is at 78 - 19 = 59mm, while Bag 1's connector top edge is at 30 + 19 = 49mm. Gap: 59 - 49 = 10mm. That works.

At every depth point, we need to verify clearance. The critical point is at the rear where Bag 2 reaches highest:

- Bag 2 sealed end center: 78 + 85 = 163mm
- Bag 2 top surface at sealed end: 163 + 19 = 182mm -- **2mm above dock shelf at 180mm**

This is 2mm over. Either:
1. Reduce angle to 18-19 degrees (rise = 77-81mm, moves everything down ~4-8mm)
2. Accept that the soft bag compresses against the shelf underside (it's flexible)
3. Reduce bottom clearance

**At 18 degrees:**
- Rise: 250 * sin(18) = 77mm
- Run: 250 * cos(18) = 238mm (fits 242mm)
- Thickness projection: 40 * cos(18) = 38mm

Bag 1: connector at 30mm, sealed end center at 107mm, top at 126mm
Bag 2: connector at 30 + 38 + 5 = 73mm, sealed end center at 150mm, top at 169mm

169mm is well under 180mm. This works with 11mm of margin at the top.

```
    SIDE VIEW — Two 1L bags at 18 degrees, stacked

    ═══════════════ DOCK SHELF (180mm) ═══════════════
                                            ·  ← 11mm clearance
                                          * BAG 2 sealed (169mm top)
                                         /
                              BAG 2     /    18 deg
                                       /
                              * conn. /
                             (92mm)  * ← BAG 2 connector (73mm center, 92mm top)
                     ─ ─ ─ ─ ─ ─ ─ ─ ─  5mm gap
                            * BAG 1 sealed (126mm top)
                           /
                BAG 1     /    18 deg
                         /
                * conn. /
               (49mm)  * ← BAG 1 connector (30mm center, 49mm top)

    ═══════════════ DRIP TRAY (15mm) ═══════════════
    FRONT                                        BACK
    ◄────────────── 238mm run ──────────────────►
```

### 3h. Recommended Two-Bag Configuration

**Two bags at 18-20 degrees, stacked vertically, both running front-to-back (connector at front-low, sealed end at rear-high).**

| Parameter | Value |
|---|---|
| Incline angle | 18-20 degrees from horizontal |
| Bag 1 connector height | ~30mm from floor |
| Bag 1 sealed end height | ~107-115mm from floor |
| Bag 2 connector height | ~73-78mm from floor |
| Bag 2 sealed end height | ~150-163mm from floor |
| Horizontal run (each bag) | 235-238mm |
| Top clearance to dock shelf | 11-17mm |
| Depth consumed | 235-238mm of 242mm available |
| Width consumed | 140mm of 272mm (centered, 66mm per side for tubing) |

---

## 4. Drainage Behavior

### 4a. Gravity Assistance at Incline

At 18-20 degrees, the gravitational component along the bag's length axis is sin(18-20) = 0.31-0.34. This means roughly one-third of gravitational force drives liquid toward the low (connector) end.

For a full 1L bag of syrup (~1050 g/ml):
- Weight: ~1.05 kg
- Force along incline: 1.05 * 9.81 * sin(20) = 3.52 N
- This is the force pushing liquid toward the connector

This is less than the full 10.3 N of vertical hanging, but more than zero (flat). The peristaltic pump provides its own suction (operating pressure of the Kamoer KPHM400 is adequate for self-priming), so gravity assistance is a bonus, not a requirement.

### 4b. Last 10-20% of Liquid

As the bag empties, the remaining liquid collects at the low (connector) end of the incline. The bag collapses from the high (sealed) end downward, because:

1. Atmospheric pressure acts uniformly on the bag exterior
2. The liquid weight creates higher internal pressure at the low end
3. The high end loses liquid first (gravity drains it downward along the incline)
4. The bag film at the high end has no liquid behind it, so it collapses inward

At ~20% remaining (~200ml), the liquid pool is concentrated at the connector end. The bag's cross-section at this point is roughly a wedge:

```
    SIDE VIEW — bag at ~20% capacity

    ═════ DOCK SHELF ════════════════════════
                                   (collapsed, flat)
                              ────────────── sealed end
                            /
            (transitioning)/
                          /
             ~~~~~~~~~~~~/  ← liquid pool (~200ml)
         *═══════════════*
    connector           thin liquid wedge
    ═════ DRIP TRAY ═════════════════════════
    FRONT                               BACK
```

The liquid wedge at the connector end is roughly 80-100mm long, 140mm wide, and 15-20mm thick. The connector is submerged in this pool. The pump continues to draw liquid cleanly.

### 4c. The Dip Tube Advantage

The Platypus Drink Tube Kit includes a dip tube that enters through the cap and extends into the bag interior. When the bag is mounted on an incline with the connector at the low end:

```
    SIDE VIEW — dip tube orientation

                                    sealed end (high)
                                   /
                                  /
                                 /   bag interior
                                /
                     ┌─────────/─── dip tube extends UP
                     │        /     along the incline toward
                     │       /      the sealed end
                     │      /
        cap ─────────┤     /
        connector    │    /
                     └───/
                        /
    ═════ DRIP TRAY ═══════
```

The dip tube extends from the low (connector) end UPWARD into the bag interior, roughly along the incline axis. This means:

- The dip tube opening is positioned partway up the incline, in the middle of the liquid volume
- As the bag drains, the dip tube opening is one of the LAST points to be exposed to air
- The liquid must drain past the dip tube opening before air reaches it
- This provides a natural buffer: even if the upper portion of the bag has collapsed and contains air, the dip tube opening remains submerged in the liquid pool at the lower portion

**The dip tube effectively extends the useful range of the bag.** The pump draws through the dip tube, not from the cap opening at the very bottom. Air only reaches the dip tube opening when the liquid level drops below it -- which, given the dip tube extends ~100-150mm up the incline, doesn't happen until the bag is nearly empty (last 5-10%).

### 4d. Kinking Analysis

The owner's key insight about kinking: "If the bag kinks under its own weight, we have plenty of syrup below the kink to work with before that kink becomes a problem."

On a stretched incline mount:

1. **Kinks above the liquid line are irrelevant.** The collapsed/empty upper portion of the bag may fold or crease, but there's no liquid trapped above it. Gravity has already drained everything downward.

2. **Kinks below the liquid line are self-healing.** If a fold forms in the bag wall below the liquid surface, the liquid weight on the uphill side of the fold pushes against the fold. As the pump draws liquid from below the fold, the pressure differential pushes liquid past/through the fold. The fold "opens" because the liquid on the uphill side has nowhere else to go -- gravity pulls it toward the connector, and the only path is past the fold.

3. **The stretched mount prevents severe kinking.** With the bag held taut between two fixed points, the bag material is under mild tension along its length. This tension resists lateral folding. The bag can thin (collapse in the thickness direction) but it cannot easily fold perpendicular to the tension axis.

---

## 5. Mounting Hardware

### 5a. Connector End (Low Mount)

The connector end has a 28mm threaded cap. This is the strongest mounting point on the bag.

**Mount design: Threaded cap cradle**

```
    FRONT VIEW — connector mount

    ┌──────────────────┐
    │   ┌──────────┐   │
    │   │  U-clip  │   │  ← 3D printed U-shaped cradle
    │   │  grips   │   │     snaps around the cap/connector
    │   │  28mm    │   │     body below the thread
    │   │  cap     │   │
    │   └──────────┘   │
    │                  │
    │   mounted to     │
    │   enclosure      │
    │   front wall     │
    │   or shelf       │
    └──────────────────┘
```

Options:
- **Clip/cradle on the enclosure front wall**: A 3D-printed U-bracket or snap-fit clip mounted to the interior front wall at the appropriate height (30mm for Bag 1, 73mm for Bag 2). The cap sits in the clip, held by friction and gravity.
- **Threaded receiver**: A 28mm threaded socket printed into the front wall. The bag screws in. Most secure, but harder to print threads accurately in FDM.
- **Slot in a shelf/ledge**: A slot cut into a horizontal ledge. The cap drops into the slot and the bag hangs below. Simple and effective.

**Recommended: Snap-fit U-clip.** The clip is printed as part of the front wall interior. The user pushes the cap into the clip, and the clip's flex holds it. To remove, pull the cap out. Quick, one-handed, no threading.

### 5b. Sealed End (High Mount)

The sealed end is a heat-sealed seam across the full width of the bag (~140mm). The seam creates a narrow ridge of doubled material, typically 5-10mm wide.

**Mount design: Clamp bar or binder clip**

```
    REAR VIEW — sealed end mount

    ┌────────────────────────────────────┐
    │                                    │
    │   ┌──── clamp bar ──────────┐     │
    │   │  ═══════════════════════ │     │  ← 3D printed bar with
    │   │  bag seam clamps here   │     │     spring clip or
    │   │  ═══════════════════════ │     │     printed snap
    │   └─────────────────────────┘     │
    │                                    │
    │   mounted to enclosure rear wall   │
    │   or dock shelf underside          │
    └────────────────────────────────────┘
```

Options:
- **Binder clip on a hook**: Use a standard 50mm binder clip to grip the sealed seam, then hang the clip on a hook attached to the rear wall or dock shelf underside. Cheap, adjustable, replaceable.
- **Printed clamp bar**: Two parallel bars with a spring or screw mechanism that pinch the seam between them. More permanent but more complex.
- **Slot and retention tab**: A horizontal slot in the rear wall, sized for the seam thickness. Push the seam into the slot; a flexible tab prevents it from sliding out. Simple but may not grip reliably.
- **Velcro strap**: A loop of Velcro around the sealed end, attached to a hook. Very simple but may slip.

**Recommended: Binder clip on a printed hook.** A 50mm binder clip (cost: $0.25) grips the sealed seam firmly. The clip's handles fold up and hook onto a simple J-hook printed into the rear wall or dock shelf underside. The user clips the seam, hangs it on the hook, done. To remove, lift off the hook and unclip.

### 5c. Complete Mount Assembly

```
    SIDE VIEW — complete incline mount for one bag

    ═══════════ DOCK SHELF UNDERSIDE ════════════
                                          ↓ J-hook (printed)
                                          │
                                     ┌────┤ binder clip
                                     │    │ grips sealed seam
                              ──────*────┘
                             / sealed end
                            /
                           /   BAG (inclined 18-20 deg)
                          /
                         /
                        /
              ─────────*
             / connector end
    ┌───────┤
    │ U-clip│ (printed on front wall interior)
    │ holds │ cap/connector
    └───────┘
    ═══════════ DRIP TRAY ══════════════════════
    FRONT                                  BACK
```

### 5d. Installation Sequence

1. Attach binder clip to bag's sealed seam (can be done outside the enclosure)
2. Hang binder clip on the J-hook at the rear of the bag zone
3. Route the bag connector down toward the front
4. Push the connector/cap into the U-clip on the front wall
5. Connect tubing to the cap (Platypus drink tube adapter already attached)
6. Bag is installed -- the incline angle is set by the hook and clip positions

Removal is the reverse: disconnect tubing, pull cap from U-clip, lift binder clip off hook.

**Estimated installation time: 15-30 seconds per bag.**

---

## 6. Comparison to Vertical Hanging

| Factor | Vertical Hanging | Incline Mount (18-20 deg) |
|---|---|---|
| **Vertical space required** | 250mm+ (bag height + connector + hook) | ~140mm (rise + thickness + clearance) |
| **Fits in 165mm bag zone** | No -- requires folding top 85mm against shelf, or 500mm enclosure | Yes -- designed for the space |
| **Drainage reliability** | Good (full gravity, sin 90 = 1.0) | Good (partial gravity sin 20 = 0.34, plus pump suction) |
| **Last 10-20% behavior** | Bag becomes floppy, random collapse, potential kinking | Liquid pools at connector end, collapse is top-down along incline |
| **Dip tube interaction** | Dip tube hangs straight down, parallel to gravity -- no extended reach into bag interior | Dip tube extends UP along incline, reaches into liquid pool, last to see air |
| **Collapse behavior** | Uncontrolled -- bag folds randomly as it becomes floppy | Controlled -- tension between two mount points constrains collapse to thinning |
| **Mounting complexity** | One hook + one clip (top only) | Two mount points (connector clip + sealed end hook), slightly more complex |
| **Bag installation** | Hang on hook, connect tubing | Clip sealed end, snap connector in clip, connect tubing |
| **Space for two bags** | Front-to-back stacking (80mm depth) | Same footprint, stacked vertically within the incline envelope |
| **Enclosure height impact** | Requires 500mm enclosure for 2L bags; 400mm only works with aggressive folding for 1L bags | 400mm enclosure works cleanly with 1L bags at 18-20 degrees |

**The incline mount is superior for the 400mm enclosure design.** It solves the fundamental space problem without increasing enclosure height, while providing equal or better drainage behavior and significantly better collapse control.

---

## 7. Controlled Collapse and Flattening

### 7a. Why Incline Mount Improves Collapse

Prior research (hopper-and-bag-management.md Section 5) extensively analyzed bag collapse problems:

> "If a fold or crease forms BELOW the remaining liquid level, liquid gets trapped above the fold. The pump now pulls air from below the fold instead of liquid from above."

The incline mount addresses this through three mechanisms:

**Mechanism 1: Tension prevents lateral folding.**
The bag is stretched between two fixed points. The bag material is under mild tension along its length axis. A fold perpendicular to the length axis (the problematic kind that traps liquid) would require the bag to buckle against this tension. While the bag material is thin and flexible, even mild tension significantly increases the resistance to perpendicular folding.

**Mechanism 2: Gravity drains liquid along the incline, creating a clear liquid-air boundary.**
At any moment during drainage, there is a distinct boundary: liquid below (toward the connector) and collapsed bag above (toward the sealed end). This boundary moves from the sealed end toward the connector as the bag empties. Because gravity continuously pulls liquid downhill along the incline, any momentary fold or crease that traps a small pocket of liquid will drain as the pump creates suction -- the liquid has a clear downhill path to the connector.

**Mechanism 3: The bag thins in place rather than folding randomly.**
With the bag held between two fixed points, the collapse mode is primarily thinning (the two faces of the bag coming together). This is fundamentally different from unsupported hanging, where the bag can fold, twist, or balloon outward. Thinning is the ideal collapse mode because it maintains the bag's overall shape while reducing volume.

### 7b. Comparison to Prior Solutions

The prior research proposed multiple mechanical solutions for the collapse problem:

| Solution | Complexity | Required? With Incline Mount |
|---|---|---|
| Gravity alone (Phase 1) | None | The incline mount IS this, but improved -- gravity acts along the incline axis |
| Elastic frame / bag squeezer | Low-medium | Likely unnecessary -- the two-point stretch provides similar constraint |
| Roller / wiper bar | Medium-high | Unnecessary -- the incline provides natural top-down drainage |
| Rigid channel / cradle | Medium | Could complement the incline mount but adds bulk |
| Top-suspended with weight | Medium | Rejected previously, not applicable |

The incline mount effectively combines the benefits of "gravity alone" with "elastic frame" -- the bag is constrained by its mounting points, and gravity drives drainage in the right direction. The most likely outcome is that no additional mechanical assistance is needed.

---

## 8. Interaction with Dock Shelf

### 8a. Clearance Analysis

The dock shelf sits at 180mm from the enclosure floor (per the active design). The top of the upper bag (Bag 2) reaches ~169mm at the sealed end (at 18 degrees). This leaves 11mm between the bag's upper surface and the dock shelf underside.

```
    DETAIL — dock shelf interface

    ═══════════════ DOCK SHELF (180mm) ═════════════
    │                                              │
    │   11mm gap                                   │
    │                                              │
    │   ───────── BAG 2 upper surface (169mm) ──── │
    │                                              │
    │   Tubing from connectors routes FORWARD      │
    │   and DOWN to tee fittings at front of       │
    │   bag zone, then UP through shelf to dock    │
```

### 8b. Tubing Routing

The connector end of each bag is at the front-low position. Tubing connects at the connector and routes:

1. From the bag connector (front-low), tubing runs forward and slightly downward
2. Tubing passes through or around the enclosure front wall at the appropriate height
3. Tubing routes upward along the enclosure side wall to the dock zone above
4. Tubing passes through holes in the dock shelf to reach the tee fittings and solenoid valves

```
    SIDE VIEW — tubing routing from inclined bag to dock

    ═══════════ DOCK SHELF (180mm) ═══════════════
    │   ○ tee fitting                             │
    │   │                                         │
    │   │ tubing runs up                          │
    │   │ side wall                               │
    │   │              * sealed end (high)         │
    │   │             /                            │
    │   │            /   BAG                       │
    │   │           /                              │
    │   │          /                               │
    │   │    *────* connector                      │
    │   └────┘                                     │
    │   tubing exits connector,                    │
    │   routes along front wall                    │
    │   then up side wall                          │
    ═══════════ DRIP TRAY ═════════════════════════
    FRONT                                     BACK
```

The 11mm gap between the top bag and the dock shelf provides clearance for the tubing to pass through the shelf. The tubing (1/4" OD = 6.35mm) fits through a 10mm hole in the shelf with room to spare.

**No interference between the bag and the tubing routing.** The tubing exits at the front-low position and routes upward along the front or side wall, well clear of the bag body.

---

## 9. ASCII Diagrams — Complete Assembly

### 9a. Side Cross-Section (Both Bags Installed)

```
    ┌──────────────────────────────────────────────────────────┐
    │                                                          │ 400mm
    │  ESP32, L298N x3, RTC, fuse block                       │
    │  ═══════════ DRIP SHELF (310mm) ═════════════════════    │
    │                                                          │
    │  ┌── CARTRIDGE ──┐  solenoid valves, tees                │
    │  │   dock        │  flow meter, needle valve             │
    │  └───────────────┘                                       │
    │  ═══════════ DOCK SHELF (180mm) ═════════════════════    │
    │                        11mm gap                          │
    │                              ──────────────*  BAG 2      │
    │                             /        sealed end (169mm)  │
    │                  BAG 2     /   18 deg                     │
    │                           /                              │
    │                     *────*  connector (73mm)              │
    │                    ─ ─ ─ ─  5mm gap                      │
    │                          ──────────────*  BAG 1           │
    │                         /        sealed end (126mm)       │
    │              BAG 1     /   18 deg                         │
    │                       /                                  │
    │                 *────*  connector (30mm)                  │
    │  ═══════════ DRIP TRAY (15mm) ═══════════════════════    │
    └──────────────────────────────────────────────────────────┘
    FRONT                                                 BACK
    ◄──────────────────── 250mm depth ─────────────────────►
```

### 9b. Front View (Both Bags Behind Front Panel)

```
    ┌──────────────────── 280mm ──────────────────────┐
    │                                                  │ 400mm
    │  [electronics zone]                              │
    │  ════════════════════════════════════════════     │ 310mm
    │  [dock + valves zone]                            │
    │  ═══════════ DOCK SHELF ═════════════════════    │ 180mm
    │                                                  │
    │  ┌─────────────────────────────────────────┐    │
    │  │          bag zone (front view)           │    │
    │  │                                         │    │
    │  │     ┌────────────────────┐              │    │
    │  │     │  BAG 2 connector   │  ← 73mm     │    │
    │  │     │  (visible through  │              │    │
    │  │     │   front panel)     │              │    │
    │  │     └────────────────────┘              │    │
    │  │     ┌────────────────────┐              │    │
    │  │     │  BAG 1 connector   │  ← 30mm     │    │
    │  │     │  140mm wide        │              │    │
    │  │     └────────────────────┘              │    │
    │  │                                         │    │
    │  └─────────────────────────────────────────┘    │
    │  ═══════════ DRIP TRAY ══════════════════════    │ 15mm
    └──────────────────────────────────────────────────┘
```

---

## 10. Open Questions and Next Steps

### 10a. Physical Verification Needed

1. **Platypus 1L bag actual dimensions**: Measure a filled 1L Platypus bag. Is it really 250mm long, 140mm wide, 40mm thick? The geometry analysis is only as good as these inputs.

2. **Dip tube length**: Measure the Platypus Drink Tube Kit dip tube. How far does it extend into the bag interior? This determines how much liquid remains when air reaches the tube opening.

3. **Drainage test**: Mount a filled 1L bag at 18-20 degrees using binder clips and hooks (the prototype can be two nails in a board). Run the pump. Does it drain completely without sputtering? At what remaining volume does air appear?

4. **Two-bag clearance**: With two bags physically present, verify the 5mm gap between bags and the 11mm gap to the shelf. The bags may deform slightly when full, changing the clearance.

### 10b. Design Decisions

1. **Exact angle**: The analysis suggests 18-20 degrees. Physical testing will determine the best angle within this range. A slightly steeper angle (20 degrees) provides better gravity assistance but tighter vertical clearance. A shallower angle (18 degrees) provides more margin.

2. **Mount point adjustability**: Should the hook and clip positions be adjustable (slotted mounts) or fixed? Fixed is simpler; adjustable accommodates bag dimension variations.

3. **Bag access order**: With two bags stacked, the upper bag (Bag 2) must be installed first (its sealed end hook is behind Bag 1's position). Or the hooks could be designed so Bag 1 (lower) is installed first, with Bag 2's mounts accessible above it. The installation order depends on the exact mount positions.

### 10c. Impact on Other Systems

1. **Hopper routing**: The hopper tee connects at the bag connector. With connectors at the front-low position, the hopper tubing routes from the top of the enclosure down the front wall to the connector -- a straightforward vertical run.

2. **Clean cycle**: No impact. The clean cycle fills the bag through the same connector, regardless of bag orientation.

3. **Capacitive sensing**: FDC1004 sensor placement on the tubing between the bag connector and the tee is unaffected by the incline mount. The tubing exits the connector and runs along the front wall.

---

## 11. Recommendation

**Adopt the incline bag mounting approach at 18-20 degrees for the 400mm enclosure design.**

This approach:

1. **Solves the space problem** that has plagued the bag zone design. The 165mm-tall bag zone cannot fit 250mm-tall bags vertically, but it comfortably fits them at an 18-20 degree incline.

2. **Improves drainage reliability** over both vertical hanging (better collapse control due to two-point tension) and flat cradles (definite gravity-driven drainage toward the connector).

3. **Leverages the dip tube** as a natural air buffer. The tube extends from the low connector end upward into the bag, meaning air only reaches the tube opening when the bag is nearly empty.

4. **Simplifies the collapse problem.** The extensive research into elastic frames, rollers, and channels (hopper-and-bag-management.md Section 5d) becomes largely moot. The two-point stretch constrains the bag to thin in place, which is the ideal collapse mode.

5. **Uses minimal hardware.** Two mounting points per bag: a printed U-clip for the connector, and a binder clip on a printed J-hook for the sealed end. Total cost per bag: ~$0.25 (binder clip).

6. **Maintains the 400mm enclosure height.** No need to increase to 500mm. The device stays shorter than the companion soda water machines (Brio 460mm, Lillium 440mm).

The approach should be validated with a physical test: mount a 1L Platypus bag at 18-20 degrees and pump it dry. If it drains cleanly to the last 5-10%, the design is confirmed. If sputtering occurs earlier, the angle can be adjusted or a light elastic wrap can be added around the bag -- but the incline geometry itself is sound.

---

## Sources

- [bag-zone-geometry.md](../../../hardware/bag-zone-geometry.md) -- Active bag zone analysis, 400mm enclosure decision, 1L bag dimensions
- [hopper-and-bag-management.md](hopper-and-bag-management.md) -- Bag flattening problem, collapse solutions, drainage analysis, prior art
- [layout-spatial-planning.md](layout-spatial-planning.md) -- Enclosure master layout, zone definitions, 280x250x400mm dimensions
- [dimensions-reconciliation.md](../../../hardware/dimensions-reconciliation.md) -- Authoritative dimension table, zone heights
- [dock-mounting-strategies.md](../../../hardware/cartridge/planning/research/dock-mounting-strategies.md) -- Dock shelf architecture, zone boundaries
- [plumbing.md](../../../docs/plumbing.md) -- Platypus Drink Tube Kit details, tubing specifications
