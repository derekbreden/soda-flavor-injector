# Bag Zone Geometry — Resolving Bag Fit Inside the Enclosure

> **DECISION (2026-03-24):** Enclosure height is locked at **400mm**. The device must be shorter than its companion soda water machines (Brio 460mm, Lillium 440mm) since our device is subordinate to theirs. This means **1L Platypus bags** (not 2L). The 2L analysis below is retained for reference but the 400mm fallback is the active design.

This document analyzes how Platypus bags physically fit inside the enclosure, given that the original layout-spatial-planning.md allocated an impossibly small zone for them. It reconciles conflicting dimensions across the research documents and produces a viable space budget.

---

## 1. The Conflict

Two documents define the enclosure differently:

| Parameter | layout-spatial-planning.md | dock-mounting-strategies.md | cartridge-envelope.md |
|---|---|---|---|
| Enclosure dimensions | 280W x 250D x 400H mm | 250W x 200D x 450H mm | Lists both as options |
| Bag zone height | ~100mm (bags on tilted cradles) | ~250-300mm (bags hanging) | N/A |
| Dock+valves zone | ~210mm (100-310mm) | ~230mm (170-450 minus electronics) | N/A |
| Electronics zone | ~90mm (310-400mm) | ~100mm (top) | N/A |

The layout-spatial-planning.md document is labeled as the "definitive" master document and uses 280W x 250D x 400H mm. It allocates only **100mm** to the bag zone. But the bags it specifies are:

**Platypus 2L bag dimensions (from hopper-and-bag-management.md):**
- Width: 190mm
- Height: 350mm (when full)
- Thickness: 60-80mm (when full)
- Connector adds ~20-30mm to effective length
- Total hanging height with connector at bottom: ~370-380mm

A 350mm tall bag cannot fit in a 100mm zone. The layout-spatial-planning.md description says bags "sit on tilted cradles" in this zone, which implies laying flat. But a bag laid flat is still 190mm tall (now on its side), 350mm wide, and 60-80mm thick. Two bags laid flat stack to 120-160mm thick -- already exceeding the 100mm allocation.

**The 100mm bag zone in layout-spatial-planning.md is physically impossible for Platypus 2L bags in any orientation.**

---

## 2. Resolving the Enclosure Height

The dock-mounting-strategies.md document gets the bag zone right (250-300mm for hanging bags) but uses the older 250x200x450mm enclosure. The layout-spatial-planning.md document gets the overall layout and width right (280mm wide, front-loading) but has an impossible bag zone.

The solution is to combine the correct elements from both: use the 280W x 250D footprint from layout-spatial-planning.md, but increase the height to accommodate hanging bags. The question is: how tall?

### 2a. Vertical Space Budget (Bottom-Up)

Working from the floor upward, allocating every millimeter:

| Zone | Bottom (mm) | Top (mm) | Height (mm) | Contents |
|---|---|---|---|---|
| Drip tray | 0 | 15 | 15 | Integral drip tray, catches leaks |
| Bag zone | 15 | 315 | 300 | Two Platypus 2L bags hanging vertically, connector down |
| Dock shelf floor | 315 | 321 | 6 | 6mm PETG structural shelf spanning enclosure width |
| Valve zone (under shelf) | N/A | N/A | 0 | Valves mount beside/behind dock, not below (see Section 4) |
| Cartridge cavity | 321 | 401 | 80 | Cartridge envelope height (80mm) |
| Lever clearance | 401 | 441 | 40 | Cam lever swing above cartridge top |
| Drip shelf | 441 | 445 | 4 | Solid barrier between plumbing and electronics |
| Electronics zone | 445 | 535 | 90 | ESP32, L298N x3, RTC, MCP23017, fuse block, DIN rail |
| Hopper funnels | 535 | 545 | 10 | Funnel bases protrude above enclosure top |
| **Total enclosure height** | **0** | **535** | **535** | |

This produces a 535mm enclosure -- too tall. The cabinet usable height is 500-600mm, and we need clearance above for pouring. Let me compress.

### 2b. Vertical Space Budget (Compressed)

The bag zone is the biggest variable. Key insight: **bags do not need the full 350mm of height** because:

1. The bag is 350mm tall when fully inflated. When hanging vertically, gravity pulls liquid down and the bag narrows and elongates slightly, but the sealed top and bottom edges constrain the height to roughly the manufactured dimension.
2. The connector/cap adds 20-30mm below the bag body, but the connector can sit at or below the enclosure floor level if we route tubing through the drip tray zone.
3. The bag does not need rigid headroom above it -- the top of the bag can press against the underside of the dock shelf. The bag is flexible.
4. A partially full bag is shorter (the empty top portion collapses). Only a brand-new, completely full bag is the full 350mm.

**Practical bag zone height: 280mm.** This accommodates a full bag (350mm body) by allowing the top 70mm of the bag to fold/press against the dock shelf underside. A flexible bag tolerates this -- the folded portion is the first part that collapses as the bag drains.

Alternatively, we can use **smaller bags** as layout-spatial-planning.md suggests: "2L capacity is not a hard requirement -- smaller bags that fit the enclosure are acceptable." Platypus makes a 1L bag (roughly 250mm tall), and CNOC Vecto bags come in various sizes. But let us design for the 2L worst case.

**Revised vertical budget:**

```
    ┌──────────────────────────────────────────────────┐
    │  Hopper funnels (caps extend above top)          │ ← 500mm (top of enclosure)
    │                                                  │
    │  ESP32, L298N x3, RTC, DIN rail, MCP23017       │
    │  Fuse block, power distribution                  │
    │                                                  │ ← 410mm (drip shelf)
    │  ════════════ DRIP SHELF ════════════════════    │
    │                                                  │
    │  ┌────── lever clearance ──────┐                 │ ← 370mm
    │  │                             │                 │
    │  │  ┌── CARTRIDGE SLOT ──┐     │                 │
    │  │  │  80mm tall         │     │                 │ ← 290mm (cartridge bottom)
    │  │  └────────────────────┘     │                 │
    │  └─────────────────────────────┘                 │
    │  Solenoids, tees, needle valve (beside dock)     │
    │  ═══════ DOCK SHELF FLOOR (6mm) ════════════    │ ← 280mm
    │                                                  │
    │  ┌──────────────────────────────────────────┐    │
    │  │                                          │    │
    │  │  PLATYPUS BAG 1      PLATYPUS BAG 2      │    │
    │  │  (hanging vertically, connector down)     │    │
    │  │  350mm bag body, top 70mm may fold        │    │
    │  │  against shelf underside                  │    │
    │  │                                          │    │
    │  │  Bag connector + tubing exits at ~15mm   │    │
    │  │                                          │    │
    │  └──────────────────────────────────────────┘    │
    │  ════════════ DRIP TRAY (15mm) ═════════════    │ ← 0mm (floor)
    └──────────────────────────────────────────────────┘
     FRONT                                       BACK
```

| Zone | Bottom (mm) | Top (mm) | Height (mm) | Contents |
|---|---|---|---|---|
| Drip tray | 0 | 15 | 15 | Integral drip tray |
| Bag zone | 15 | 280 | 265 | Bags hang here; top ~70mm of full bag folds against shelf |
| Dock shelf floor | 280 | 286 | 6 | PETG structural shelf |
| Cartridge cavity | 286 | 366 | 80 | Cartridge envelope |
| Lever clearance | 366 | 406 | 40 | Reduced from 100mm; lever swings 180 deg but handle is 80mm, needs ~40mm vertical sweep in compact layout |
| Drip shelf | 406 | 410 | 4 | Solid barrier |
| Electronics zone | 410 | 500 | 90 | All electronics + wiring |
| **Total enclosure height** | **0** | **500** | **500** | |

### 2c. Enclosure Height Recommendation

**Increase enclosure height from 400mm to 500mm.**

This is a 100mm (4 inch) increase. The new enclosure dimensions are:

**280W x 250D x 500H mm (11" x 10" x 20")**

This fits within the 500-600mm usable height of standard under-sink cabinets. With the enclosure at 500mm, there is 0-100mm of clearance above for hopper access. The hopper funnels at the front-top corner remain accessible when the cabinet door is open.

If 500mm proves too tight for cabinet clearance, the bag zone can be shortened to 230mm (using 1.5L or smaller bags, or accepting that the top of a full 2L bag folds more aggressively). This brings the enclosure down to 465mm.

---

## 3. Where the Dock Shelf Sits -- Correcting the Zone Layout

The layout-spatial-planning.md document places the dock zone at 100-310mm and the bag zone at 0-100mm. With the corrected height:

**Old (layout-spatial-planning.md, 400mm enclosure):**
```
400mm ┬ Electronics (310-400, 90mm)
      │ Dock + valves (100-310, 210mm)
100mm ┤ Bags (0-100, 100mm)     ← IMPOSSIBLE
  0mm ┘
```

**New (corrected, 500mm enclosure):**
```
500mm ┬ Electronics (410-500, 90mm)
      │ Drip shelf (406-410, 4mm)
      │ Lever clearance (366-406, 40mm)
      │ Cartridge (286-366, 80mm)
      │ Dock shelf (280-286, 6mm)
280mm ┤ Bags (15-280, 265mm usable)
 15mm ┤ Drip tray (0-15, 15mm)
  0mm ┘
```

The dock shelf at 280mm means the cartridge slot center is at ~326mm from the floor (~13 inches). This is higher than the original ~265mm estimate in dock-mounting-strategies.md, but still accessible when kneeling or crouching at a cabinet. The user reaches in at roughly mid-height of the enclosure.

### 3a. What Happened to the 210mm Dock+Valves Zone?

Layout-spatial-planning.md allocated 210mm (100-310mm) to the dock+valves zone. This included:
- Cartridge: 80mm height
- Lever clearance: ~100mm above cartridge
- Solenoid valves: 4x at ~80x35x45mm each
- Flow meter, needle valve, tees

In the revised layout, the 80mm cartridge + 40mm lever clearance + 6mm shelf = 126mm. The remaining components (solenoids, tees, flow meter, needle valve) must fit **beside** or **behind** the dock, not in a vertical stack below it. This is addressed in Section 4 (horizontal layout).

The lever clearance was reduced from 100mm to 40mm. The original 100mm assumed the lever swings through a full vertical arc above the cartridge. With an 80mm lever handle that pivots at the top of the cartridge front face and swings 180 degrees, the swept envelope is ~80mm radius. But the lever only needs clearance for the arc -- it does not need the full radius as headroom because the lever swings through the space momentarily. A 40mm clearance above the cartridge top allows the lever to swing if it swings primarily forward (outward from the front face) rather than straight up. The cam lever design from cam-lever.md has the pivot at the top-front of the cartridge with the handle sweeping upward and then forward -- 40mm of vertical clearance above the cartridge top is tight but workable if the lever rotates primarily in the horizontal plane or the user pulls the cartridge slightly forward before flipping the lever.

**If 40mm proves insufficient, increase to 60mm, adding 20mm to total enclosure height (520mm). This is still within cabinet limits.**

---

## 4. Horizontal Cross-Section — Bag Placement

### 4a. The Width Problem

Two Platypus 2L bags side by side: 190mm x 2 = 380mm. The enclosure interior width is ~272mm (280mm minus 4mm walls on each side). **Side by side does not fit.**

### 4b. The Depth Problem

Two bags front-to-back: 60-80mm x 2 = 120-160mm. The enclosure interior depth is ~242mm (250mm minus 4mm walls). This fits, but consumes 50-66% of the available depth for bags alone.

### 4c. Viable Arrangement: Front-to-Back (Stacked in Depth)

```
    TOP VIEW (looking down, at bag zone height)

    ┌──────────────────── 280mm ────────────────────┐
    │                                                │
    │  FRONT FACE                         BACK PANEL │
    │  │                                         │   │
    │  │   ┌────────────────────┐                │   │
    │  │   │                    │                │   │
    │  │   │   BAG 1 (front)   │                │   │
    │  │   │   190mm wide      │   remaining    │   │
    │  │   │   60-80mm deep    │   space for    │   │
    │  │   │                    │   tubing and   │   │
    │  │   ├────────────────────┤   structure    │   │
    │  │   │                    │   (~80-120mm)  │   │
    │  │   │   BAG 2 (behind)  │                │   │
    │  │   │   190mm wide      │                │   │
    │  │   │   60-80mm deep    │                │   │
    │  │   │                    │                │   │
    │  │   └────────────────────┘                │   │
    │  │                                         │   │
    │  │◄──── ~160mm bags ────►◄── ~80mm ──────►│   │
    │                                                │
    └────────────────────────────────────────────────┘
```

Two bags front-to-back use ~120-160mm of the 242mm interior depth, leaving 80-120mm for:
- Tubing runs from bag connectors to tees and solenoids
- Hook/mounting hardware
- Air gap for bag inflation/deflation

The bags are centered or offset to one side of the 272mm interior width. Each bag is 190mm wide, so there is 82mm of side clearance (41mm per side, or all on one side). This is adequate for tubing routing along the walls.

### 4d. Alternative: Offset/Staggered Arrangement

If the bags are offset laterally, they can overlap in depth while using more width:

```
    TOP VIEW (staggered arrangement)

    ┌──────────────────── 280mm ────────────────────┐
    │                                                │
    │  ┌──────────────────┐                          │
    │  │    BAG 1          │                          │
    │  │    190mm x 70mm   │                          │
    │  │                    │                          │
    │  └──────────────────┘                          │
    │            ┌──────────────────┐                 │
    │            │    BAG 2          │                 │
    │            │    190mm x 70mm   │                 │
    │            │                    │                 │
    │            └──────────────────┘                 │
    │                                                │
    └────────────────────────────────────────────────┘
```

This staggers the bags ~80mm laterally and ~30mm in depth. Total footprint: ~270mm wide x ~100mm deep. This fits the width (272mm interior) but is very tight. It has no real advantage over front-to-back stacking unless the reduced depth matters for tubing access.

### 4e. Recommended Horizontal Arrangement

**Front-to-back stacking is the simplest viable arrangement.**

- Bag 1 hangs at the front of the bag zone
- Bag 2 hangs directly behind Bag 1
- Both bags are centered on the enclosure width (190mm bag in 272mm space = 41mm clearance per side)
- Total depth consumed: ~120-160mm (varies with fill level)
- Remaining depth behind bags: ~80-120mm for tubing routing

When bags are partially full, they thin out (a half-full hanging bag is roughly 30-40mm thick instead of 60-80mm). This means the front-to-back arrangement gets easier as bags drain.

### 4f. Horizontal Cross-Section With Dock

At the dock shelf height (280mm), the dock and valves occupy the space. Below is the plan view at that level:

```
    TOP VIEW (at dock shelf height, 280mm)

    ┌──────────────────── 280mm ────────────────────┐
    │                                                │
    │  FRONT FACE                         BACK PANEL │
    │  │                                         │   │
    │  │  ┌── CARTRIDGE SLOT ─────┐              │   │
    │  │  │  150W x 130D          │   ┌────────┐ │   │
    │  │  │  (incl fitting wall)  │   │SOLENOID│ │   │
    │  │  │                        │   │CLUSTER │ │   │
    │  │  │                        │   │SV-D1   │ │   │
    │  │  │                        │   │SV-D2   │ │   │
    │  │  └────────────────────────┘   │SV-C1   │ │   │
    │  │                               │SV-C2   │ │   │
    │  │                               │NV, FM  │ │   │
    │  │                               └────────┘ │   │
    │  │                                         │   │
    │  │◄── 150mm dock ──►◄── ~120mm valves ──►│   │
    │                                                │
    └────────────────────────────────────────────────┘
```

The dock (150mm wide including cartridge cavity + fitting wall) sits on the left/center. The solenoid valves, needle valve, flow meter, and tees mount to the right of the dock on the shelf or on brackets attached to the right enclosure wall. The four solenoids (each ~80x35x45mm) can be arranged in a 2x2 grid taking roughly 80mm wide x 80mm deep, or in a vertical column taking ~35mm wide x 180mm tall. A vertical column beside the dock fits well.

---

## 5. Bag Mounting Details

### 5a. Hook Positions

Per hopper-and-bag-management.md Phase 1 recommendation: simple hooks at the top of the bag zone (underside of dock shelf) from which bags hang.

```
    FRONT VIEW (bag zone detail, 15-280mm height)

    ══════════ DOCK SHELF (280mm) ══════════════
    ↓hook 1                    ↓hook 2
    │                          │
    ╔══════════╗              ╔══════════╗
    ║          ║              ║          ║
    ║  BAG 1   ║              ║  BAG 2   ║
    ║  190mm W ║              ║  190mm W ║
    ║  350mm H ║              ║  350mm H ║
    ║          ║              ║          ║
    ║          ║              ║          ║
    ╚════╤═════╝              ╚════╤═════╝
         │                        │
       tubing                   tubing
    ═══════════ DRIP TRAY (0-15mm) ═════════════
```

Wait -- this front view shows bags side by side, which does not fit (380mm in 272mm). In the front-to-back arrangement, the front view shows only one bag (the front one), with the second hidden behind it:

```
    FRONT VIEW (bags stacked front-to-back; only BAG 1 visible)

    ══════════ DOCK SHELF UNDERSIDE (280mm) ══════════
       ↓hook 1 (front)     ↓hook 2 (behind, not visible)
       │
       ╔══════════════════╗
       ║                  ║
       ║    BAG 1         ║  ← 190mm wide, centered
       ║    (BAG 2        ║
       ║     behind)      ║
       ║                  ║
       ║                  ║
       ╚════════╤═════════╝
                │
              tubing
    ═══════════ DRIP TRAY ═══════════════════════
```

```
    SIDE VIEW (bags stacked front-to-back)

    ══════════ DOCK SHELF (280mm) ═══════════════
    ↓hook 1           ↓hook 2
    │                 │
    ┌────────┐       ┌────────┐
    │        │       │        │
    │ BAG 1  │       │ BAG 2  │
    │ 60-80mm│       │ 60-80mm│    ← remaining
    │ thick  │       │ thick  │       ~80-120mm
    │        │       │        │       depth for
    │        │       │        │       tubing
    └───┬────┘       └───┬────┘
        │                │
      tubing           tubing
    ═══════════ DRIP TRAY ═══════════════════════
    FRONT                              BACK
    ◄──────────── 250mm depth ───────────────►
```

### 5b. Hook Design

Each hook is a simple J-hook or U-clip printed into the underside of the dock shelf (or screwed into it). The bag's top (sealed) edge has a heat-sealed seam that forms a natural ridge. A binder clip or carabiner clip attaches the bag's top seam to the hook.

| Parameter | Value |
|---|---|
| Hook attachment point | Underside of dock shelf, or printed ledge on enclosure side wall at ~270mm height |
| Hook spacing (front to back) | ~80-90mm center-to-center (one hook at front position, one at rear position) |
| Hook spacing (left to right) | Centered at ~136mm from left wall (centering the 190mm bag in 272mm interior) |
| Bag clip type | Binder clip, carabiner, or printed snap-hook |
| Bag top to hook distance | 0-10mm (bag hangs directly from hook) |

### 5c. Bag Connector Clearance

The Platypus bag connector (28mm thread cap + drink tube adapter) extends ~25mm below the bag body. With the bag hanging from 270mm (hook height), the bag body extends from ~270mm down to ~270-350 = below the floor. This confirms the bag is taller than the zone.

**Resolution:** The bag body is 350mm, but the available zone is 265mm (15mm drip tray to 280mm shelf). The bag must either:

1. **Fold at the top**: The top ~85mm of the bag folds against the shelf underside. The bag hangs in a J-shape rather than straight. This works because the top of the bag empties first and becomes floppy early in the bag's life. A full, freshly installed bag will have the top pressed against the shelf, which is acceptable -- the bag is flexible.

2. **Fold at the bottom**: The bag connector sits in the drip tray zone, and the bottom 85mm of the bag curls on the drip tray floor. This is worse because it creates a sump where liquid pools outside the connector path.

3. **Accept reduced fill**: Fill bags to only ~1.5L instead of 2L. A 1.5L bag is thinner and the liquid column is shorter, reducing the effective hanging height. But the bag body is still 350mm (the empty top collapses).

**Option 1 (fold at top) is the best approach.** The top of the bag is the part that collapses first during drainage anyway. Having it pre-folded against the shelf simply means the collapse starts from a slightly compressed state. The fold also acts as a gentle constraint, similar to the bag-in-box approach, which may actually improve collapse behavior.

### 5d. Clearances Summary

| Clearance | Value | Notes |
|---|---|---|
| Bag zone height (drip tray to shelf) | 265mm | Accommodates ~265mm of straight-hanging bag; remaining ~85mm folds at top |
| Bag width to side wall | 41mm per side | Tubing routing space |
| Bag-to-bag spacing (front to back) | 10-20mm air gap | Allows bags to inflate/deflate independently |
| Connector to drip tray floor | ~0-15mm | Connector may sit at or near drip tray level; tubing exits through tray |
| Top of bag to shelf underside | 0mm (contact) | Bag top presses against shelf -- this is intentional and acceptable |

---

## 6. Assessment: Is the Current Enclosure Height Sufficient?

**No. The current 400mm height is not sufficient.** The 100mm bag zone cannot hold Platypus 2L bags in any orientation.

### Required Changes

| Parameter | Old Value | New Value | Change |
|---|---|---|---|
| Enclosure height | 400mm | 500mm | +100mm |
| Bag zone | 0-100mm (100mm) | 0-280mm (280mm, with 15mm drip tray) | +180mm |
| Dock shelf position | ~100mm from floor | ~280mm from floor | +180mm |
| Cartridge slot center height | ~205mm from floor | ~326mm from floor | +121mm |
| Electronics zone | 310-400mm | 410-500mm | Shifted up |

### Impact on Other Systems

1. **Cartridge ergonomics**: The cartridge slot moves from ~205mm (~8") to ~326mm (~13") above the enclosure floor. If the enclosure sits on the cabinet floor at 0mm, the slot is at 326mm (12.8") from the cabinet floor. This is actually more ergonomic -- the user does not have to reach as low. Under a standard 24" high cabinet opening, 326mm from the floor is roughly knee height when kneeling, which is a natural reach position.

2. **Hopper access**: The hopper moves from 400mm to 500mm above the cabinet floor (~20"). This is still below the typical 500-600mm cabinet interior height. Pouring into the funnel requires reaching slightly higher, but the front-top-corner funnel position remains accessible.

3. **Enclosure stability**: The taller enclosure has a higher center of gravity. However, the bags (heaviest components when full, ~4kg total) are at the bottom, keeping the CG low. The 280x250mm footprint provides adequate base area. Rubber feet and an optional L-bracket to the cabinet wall prevent tipping.

4. **Cabinet fit**: A 500mm tall enclosure fits in cabinets with 500mm+ interior height. Standard US sink base cabinets have 500-600mm of usable interior height. The enclosure fits, but with less headroom above (0-100mm vs the original 100-200mm). Cabinets at the low end of the range (500mm) have no headroom, which makes hopper pouring difficult. For those cabinets, the enclosure could use 1L or 1.5L bags (shorter) to reduce height to ~450-465mm.

### Alternative: Keep 400mm Height with Smaller Bags

If the 500mm height is unacceptable:

- Use **1L Platypus bags** (~250mm tall, ~140mm wide, ~40mm thick when full)
- Bag zone needs ~200mm instead of 265mm
- Enclosure height: 400mm with bag zone of 0-200mm, dock shelf at 200mm
- Cartridge slot center at ~250mm -- close to the original design
- Tradeoff: half the concentrate capacity, twice as frequent refills

This is a valid Phase 1 option. Start with 1L bags in a 400mm enclosure, move to 500mm with 2L bags later if needed.

---

## 7. Revised Dimension Summary

### Primary Recommendation (2L bags)

| Dimension | Value |
|---|---|
| **Enclosure exterior** | **280W x 250D x 500H mm** |
| Wall thickness | 4mm |
| Interior | 272W x 242D x 492H mm |
| Bag zone (interior) | 265mm tall (15-280mm) |
| Dock shelf position | 280mm from floor |
| Cartridge slot center | ~326mm from floor |
| Electronics zone | 410-500mm (90mm) |
| Weight (loaded) | ~9-10 kg (20-22 lbs) |

### ✅ ACTIVE DESIGN: 1L bags, 400mm height

Locked at 400mm to stay shorter than companion soda water machines (Brio 460mm, Lillium 440mm).

| Dimension | Value |
|---|---|
| **Enclosure exterior** | **280W x 250D x 400H mm** |
| Wall thickness | 4mm |
| Interior | 272W x 242D x 392H mm |
| Drip tray | 0-15mm (15mm) |
| Bag zone (interior) | 165mm tall (15-180mm) |
| Dock shelf position | 180mm from floor |
| Dock shelf thickness | 6mm (180-186mm) |
| Cartridge cavity | 186-266mm (80mm) |
| Lever clearance | 266-306mm (40mm) |
| Drip shelf | 306-310mm (4mm) |
| Electronics zone | 310-400mm (90mm) |
| Cartridge slot center | ~226mm from floor (~9") |
| Weight (loaded) | ~7-8 kg (15-18 lbs) |

**1L Platypus bag dimensions (estimated):**
- Width: ~140mm
- Height: ~250mm (when full)
- Thickness: ~40mm (when full)

**Two 1L bags front-to-back:** ~80mm depth total, leaving ~162mm of the 242mm interior depth for tubing and structure. Comfortable fit.

**Two 1L bags side-by-side:** ~280mm width — just barely fits the 272mm interior. Front-to-back remains the recommended arrangement.

**Bag zone clearance:** 165mm available vs ~250mm bag height. The bag's top ~85mm folds against the shelf underside (same folding principle as the 2L analysis, but a smaller fold). Alternatively, 1L bags may be short enough to hang nearly straight — needs physical measurement of the specific Platypus 1L model.

**Companion machine context:**
| Machine | Height | Width |
|---|---|---|
| Brio cold carbonated | 460mm | 220mm |
| Lillium cold carbonated | 440mm | 170mm |
| **Soda Flavor Injector** | **400mm** | **280mm** |

Our device is wider but shorter — it sits beside the companion machine as a clear accessory, not competing for visual dominance.

---

## 8. Summary of Document Conflicts Resolved

| Conflict | Resolution |
|---|---|
| Enclosure height 400mm vs 450mm | Increase to **500mm** for 2L bags; keep 400mm only if using 1L bags |
| Enclosure width 250mm vs 280mm | Use **280mm** (layout-spatial-planning.md is correct; 250mm is from the deprecated tall tower option) |
| Enclosure depth 200mm vs 250mm | Use **250mm** (layout-spatial-planning.md is correct) |
| Bag zone 100mm vs 250-300mm | Use **265mm usable** (dock-mounting-strategies.md was closer to correct; layout-spatial-planning.md bag zone was impossible) |
| Dock+valves zone 210mm | Reduce to **126mm** vertical (80mm cartridge + 40mm lever + 6mm shelf); valves mount beside dock horizontally, not below it |
| Bags on tilted cradles vs hanging | **Hanging vertically, connector down** (hopper-and-bag-management.md is correct; tilted cradles in 100mm zone is not viable for 2L bags) |
| Bags side by side vs front-to-back | **Front-to-back** (side by side needs 380mm width, enclosure is only 272mm interior) |

---

## Sources

- layout-spatial-planning.md -- Enclosure master layout (definitive for width/depth, incorrect for bag zone height)
- dock-mounting-strategies.md -- Dock integration (correct bag zone analysis, older enclosure dimensions)
- cartridge-envelope.md -- Cartridge dimensions (150W x 80H x 130D mm)
- hopper-and-bag-management.md -- Bag dimensions (190W x 350H x 60-80D mm), mounting recommendation (hanging, connector down)
