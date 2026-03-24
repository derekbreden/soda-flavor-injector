# V1 Master Spatial Layout: Diagonal Interleave

**Synthesized:** 2026-03-24
**Sources:** v1-diagonal-bag-placement.md, v1-cartridge-dock-placement.md, v1-hopper-integration.md, diagonal-interleave.md (Vision 1), access-architecture.md, fitting-alternatives.md, cascade-matrix.md

This is the master reference for the Vision 1 diagonal interleave layout. It defines exact component positions, clearances, tube routing, and structural members for two product variants. Where research is conclusive, positions are stated as coordinates. Where physical testing is needed, ranges are given with the uncertainty flagged.

---

## Coordinate System

**Origin:** Interior front-bottom-left corner.

- **X axis (width):** Left wall = 0, right wall = interior width. Positive rightward.
- **Y axis (depth):** Front wall = 0, back wall = interior depth. Positive rearward.
- **Z axis (height):** Floor = 0, ceiling = interior height. Positive upward.

All dimensions are interior (exterior minus 8mm per axis: 4mm walls on all sides).

---

## 1. Layout V1-A: Compact (1L Bags)

### Enclosure

| Parameter | Value |
|-----------|-------|
| Exterior | 280W x 300D x 400H mm |
| Interior | 272W x 292D x 392H mm |
| Interior volume | 31.1 liters |
| Wall thickness | 4mm all sides |

### Bag Configuration

| Parameter | Value |
|-----------|-------|
| Bag model | Platypus 1.0L |
| Bag dimensions | 280mm long x 152mm wide |
| Stack thickness | 50mm (two bags, 25mm each) |
| Mounting angle | 35 degrees from horizontal |
| Bounding box (depth x height) | 258D x 202H mm |

**Bag slab position derivation:**

The sealed ends anchor at the top-front. The connector ends anchor at the bottom-back. The top surface of the upper bag at the front wall is at Z = 392mm (enclosure ceiling). The lower surface of the bottom bag intersects the front wall at:

    Z_front = H_int - T_stack / cos(theta)
            = 392 - 50 / cos(35)
            = 392 - 50 / 0.819
            = 392 - 61.0
            = 331mm

The lower bag surface intersects the floor (Z=0) at:

    Y_floor = (H_int * cos(theta) - T_stack) / sin(theta)
            = (392 * 0.819 - 50) / 0.574
            = (321.1 - 50) / 0.574
            = 271.1 / 0.574
            = 472mm (beyond the 292mm back wall)

Since the bag lower surface never reaches the floor within the enclosure, the front-bottom void is a trapezoid spanning the full floor depth (292mm). At the back wall (Y=292), the lower bag surface height is:

    Z_at_back = 331 - 292 * tan(35)
              = 331 - 292 * 0.700
              = 331 - 204.4
              = 127mm

The bag slab's front face (top of stack at front wall) is at approximately Y = 0 (the sealed ends anchor near the front wall). The bags slope rearward and downward at 35 degrees, with the connector ends at approximately Y = 258mm (bag depth consumed) at a height of approximately 29mm (T_stack * sin(35) = 50 * 0.574 = 29mm). The back of the bag bounding box is at Y = 258mm, leaving a front gap of 292 - 258 = 34mm. However, because the bag slab origin is at the top-front and extends rearward, the front gap is 292 - 258 = 34mm measured from the front wall to where the rear of the bounding box would start -- actually, the bounding box starts at the front wall and extends 258mm rearward. The front gap is the distance from the front wall to the near face of the bag slab at low heights.

At cartridge height (Z = 80mm), the lower bag surface is at:

    Y_at_z80 = (331 - 80) / tan(35)
             = 251 / 0.700
             = 359mm (past the back wall)

The bags are nowhere near the cartridge at Z = 0-80mm. The full floor area is available.

### 1a. Side-View Cross-Section (Height Z vs. Depth Y)

Viewing from the right side. Width is into the page.

```
Z(mm)
392 ┌─────────────────────────────────────────────────┐
    │[HOPPER]                                         │
    │ funnel   ╲╲╲╲╲ sealed ends of bags              │
322 │ (70mm)    ╲╲╲╲╲╲╲╲╲  (top of stack)            │
    │○○ displ    ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲                     │
    │            ╲╲╲ bag slab ╲╲╲╲╲╲╲╲╲╲         [E] │
    │             ╲╲ (50mm thick) ╲╲╲╲╲╲╲╲╲╲╲    [L] │
    │              ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲   [E] │
    │               ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲  [C] │
    │                ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲      │
    │[VALVES]         ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲     │
127 │                  ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
    │                   ╲╲  connectors  ╲╲╲╲╲╲╲╲╲╲    │
 84 │ ┌──────────┐       (bottom-back)                │
    │ │CARTRIDGE │                                    │
    │ │150x130x80│══════════════════════════════●●    │
    │ │          │   tubes along floor     bag conn   │
  0 └─┴──────────┴────────────────────────────────────┘
    0  (front)                                   292 (back)
                        Y (depth, mm)
```

### 1b. Front-View Cross-Section (Height Z vs. Width X)

Viewing from the front. Depth is into the page.

```
Z(mm)
392 ┌─────────────────────────────────────────────┐
    │          ┌──────────────────┐                │
    │          │  HOPPER FUNNEL   │                │
322 │          │  (100mm dia)     │                │
    │          └────────┬─────────┘                │
    │     ┌──┐          │tubing           ┌──┐    │
    │     │D1│ displays │                 │D2│    │
280 │     └──┘          │                 └──┘    │
    │                   │                          │
    │      bag slab extends into page              │
    │      (152mm wide, centered)                  │
    │                                              │
    │                                              │
    │                                              │
 84 │ ┌───────────────────────────────────────┐    │
    │ │         CARTRIDGE SLOT                │    │
    │ │     (150mm wide, centered)            │    │
    │ │         front panel opening           │    │
  0 └─┴───────────────────────────────────────┴────┘
    0                  136                       272
                      X (width, mm)
```

### 1c. Top-View Cross-Section (Width X vs. Depth Y)

Viewing from above. Height is into the page.

```
Y(mm)
292 ┌─────────────────────────────────────────────┐
    │         bag connectors (bottom-back)        │
    │              ●● at ~Y=258                   │
    │                                      [ELEC] │
    │      bag slab (152mm wide, centered)  pocket│
    │      extends diagonally into page    60x80mm│
    │      from Y~0 (top-front, high Z)          │
    │      to Y~258 (bottom-back, low Z)          │
    │                                              │
    │════════ tubes along floor ════════           │
    │                                              │
165 │ ┌──────────────────────────────────┐         │
    │ │    DOCK BACK WALL (fittings)     │         │
    │ └──────────────────────────────────┘         │
130 │                                              │
    │ ┌──────────────────────────────────┐         │
    │ │         CARTRIDGE                │         │
    │ │      (150W x 130D x 80H)        │         │
    │ │  pumps, motor axis along depth   │         │
    │ └──────────────────────────────────┘         │
  0 └─────────────────────────────────────────────┘
    0          61              211              272
                      X (width, mm)
```

### 1d. Component Position Table (V1-A)

Origin at interior front-bottom-left corner. All values in mm.

| Component | X range (width) | Y range (depth) | Z range (height) | Dimensions (WxDxH) | Volume |
|-----------|----------------|-----------------|-------------------|---------------------|--------|
| Bag slab (2x 1L) | 60-212 | 0-258 | ~127-392 (varies diag.) | 152W x 258D x 202H bounding | ~1.59L (actual bag vol.) |
| Cartridge dock | 61-211 | 0-130 | 0-80 | 150W x 130D x 80H | 1.56L |
| Dock back wall | 61-211 | 130-165 | 0-84 | 150W x 35D x 84H | 0.44L |
| Lever / handle zone | 61-211 | 0-20 | 80-124 | 150W x 20D x 44H | 0.13L |
| Hopper funnel | 86-186 | 0-100 | 322-392 | 100dia x 70H | ~0.25L |
| Hopper lid zone | 86-186 | 0-100 | 392 (top surface) | 100dia x 10H | -- |
| Electronics pocket | 192-272 | 222-292 | 342-392 | 80W x 70D x 50H | 0.28L |
| Display 1 (left) | 40-80 | 0-5 | 280-320 | 40dia x 5D | -- |
| Display 2 (right) | 192-232 | 0-5 | 280-320 | 40dia x 5D | -- |
| Solenoid valves (4x) | 0-60 | 100-160 | 0-35 | ~30W x 60D x 35H each | 0.25L total |
| Hopper solenoids (2x) | 215-272 | 100-140 | 0-35 | ~25W x 40D x 35H each | 0.07L total |
| Drip tray | 0-272 | 0-50 | 0-4 | 272W x 50D x 4H | 0.05L |
| Tube channels (floor) | 60-212 | 130-258 | 0-10 | 152W x 128D x 10H | -- |

**Notes on the bag slab coordinates:** The bag slab is a diagonal parallelogram, not an axis-aligned box. The X range (60-212) is the centered width of 152mm bags in a 272mm interior. The Y and Z ranges reflect the bounding box of the diagonal: depth 0 to 258mm, height from ~29mm at the back to ~392mm at the front. At any given depth, the bags occupy a 50mm thick band at a specific height determined by the 35-degree angle.

### 1e. Clearance Analysis (V1-A)

| Component Pair | Clearance | Assessment |
|----------------|-----------|------------|
| Cartridge top (Z=80) to lower bag surface at Y=130 | 331 - 130*0.700 - 80 = 331 - 91 - 80 = **160mm** | Very generous |
| Cartridge rear (Y=130) to bag connectors (Y=258) | **128mm** along floor | Generous; tube routing space |
| Bag slab top at front (Z=392) to enclosure ceiling | **0mm** (bag sealed ends anchor at ceiling) | By design |
| Hopper bottom (Z=322) to bag slab top at Y=0 | 392 - 322 = **70mm** -- but bag top at Y=0 is at Z=392; at Y=100 (hopper rear edge) bag top is at 392 - 100*0.700 = **322mm**. Clearance = 322 - 322 = **0mm** | Tight. Hopper rear edge meets bag surface. Hopper must not extend deeper than ~90mm from front to maintain 5mm clearance. |
| Hopper funnel (Z=322-392) to displays (Z=280-320) | Hopper starts at X=86, displays at X=40-80 and 192-232. Display 2 overlaps with hopper X range. **Vertical gap: 322 - 320 = 2mm.** | Very tight. Displays should be at Z=260-300 or hopper shifted to Z=325-392. |
| Electronics (Y=222-292, Z=342-392) to bag slab | At Y=222, bag top surface is at 392 - 222*0.700 + 50*cos(35) = 392 - 155.4 + 41.0 = **277.6mm**. Electronics start at Z=342. Gap = **64mm**. | Comfortable |
| Electronics pocket to hopper funnel | Electronics at X=192-272, hopper at X=86-186. No X overlap. | No conflict |
| Solenoid valves (Y=100-160, Z=0-35) to cartridge (Y=0-130, Z=0-80) | Valves start at Y=100, cartridge ends at Y=130. Overlap Y=100-130 but valves are at X=0-60, cartridge at X=61-211. **30mm lateral gap.** | Adequate |
| Drip tray (Z=0-4) to cartridge (Z=0-80) | Cartridge sits ON the drip tray or on rails above it. | By design -- drip tray is beneath/around the cartridge slot |

**Identified tight clearances:**
1. Hopper rear edge to bag surface at Y=90-100: near zero. The hopper funnel must stay within Y=0-85 to maintain 5mm clearance above the bag surface.
2. Displays to hopper: 2mm vertical gap if displays are at Z=280-320 and hopper at Z=322. Recommend lowering displays to Z=250-290 or raising hopper to Z=330.

**No interferences detected** with the adjusted constraints above.

---

## 2. Layout V1-B: Capacity (2L Bags)

### Enclosure

| Parameter | Value |
|-----------|-------|
| Exterior | 280W x 350D x 400H mm |
| Interior | 272W x 342D x 392H mm |
| Interior volume | 36.5 liters |
| Wall thickness | 4mm all sides |

### Bag Configuration

| Parameter | Value |
|-----------|-------|
| Bag model | Platypus 2.0L |
| Bag dimensions | 350mm long x 190mm wide |
| Stack thickness | 80mm (two bags, 40mm each) |
| Mounting angle | 40 degrees from horizontal |
| Bounding box (depth x height) | 320D x 286H mm |

**Bag slab position derivation:**

Lower bag surface at front wall:

    Z_front = H_int - T_stack / cos(theta)
            = 392 - 80 / cos(40)
            = 392 - 80 / 0.766
            = 392 - 104.4
            = 288mm

Lower bag surface at back wall (Y=342):

    Z_at_back = 288 - 342 * tan(40)
              = 288 - 342 * 0.839
              = 288 - 286.9
              = 1mm

The bag slab nearly reaches the floor at the back wall. The front-bottom void is a large trapezoid: 288mm tall at the front, 1mm at the back. The bag bounding box depth is 320mm, leaving a front gap of 342 - 320 = 22mm. At cartridge height (Z=80), the lower bag surface is at:

    Y_at_z80 = (288 - 80) / tan(40)
             = 208 / 0.839
             = 248mm

The bag reaches Z=80 at Y=248mm. Since the cartridge only extends to Y=130, the clearance is 248 - 130 = 118mm of depth between the cartridge rear face and where the bag descends to cartridge height. At Y=130 (cartridge rear):

    Z_bag_at_130 = 288 - 130 * tan(40)
                 = 288 - 109.1
                 = 179mm

Clearance above cartridge at dock wall: 179 - 80 = **99mm**.

Top-back void for electronics: At the back wall (Y=342), the bag top surface is at approximately 286 - (342-22)*0.839... More directly: bag height consumed is 286mm. Top-back void height = 392 - 286 = **106mm**.

### 2a. Side-View Cross-Section (Height Z vs. Depth Y)

```
Z(mm)
392 ┌───────────────────────────────────────────────────────────┐
    │[HOPPER]                                                   │
    │ funnel  ╲╲╲╲╲╲╲ sealed ends of bags (top of stack)       │
    │(70mm)    ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲                               │
306 │           ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲                         │
    │○○ disp    ╲╲╲╲╲ bag slab ╲╲╲╲╲╲╲╲╲╲╲╲╲╲              [E]│
    │            ╲╲╲ (80mm thick) ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲        [L]│
    │             ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲     [E]│
    │              ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲  [C]│
    │               ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
    │[VALVES]        ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲   │
    │                 ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲  │
    │                  ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲ │
 84 │ ┌──────────┐      ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲│
    │ │CARTRIDGE │       connectors near floor at back      ≈1mm│
    │ │150x130x80│══════════════════════════════════════════●●   │
    │ │          │        tubes along floor            bag conn  │
  0 └─┴──────────┴──────────────────────────────────────────────┘
    0  (front)                                          342 (back)
                            Y (depth, mm)
```

### 2b. Front-View Cross-Section (Height Z vs. Width X)

```
Z(mm)
392 ┌─────────────────────────────────────────────┐
    │          ┌──────────────────┐                │
    │          │  HOPPER FUNNEL   │                │
322 │          │  (100mm dia)     │                │
    │          └────────┬─────────┘                │
    │    ┌──┐           │tubing           ┌──┐    │
    │    │D1│  displays │                 │D2│    │
260 │    └──┘           │                 └──┘    │
    │                   │                          │
    │      bag slab extends into page              │
    │      (190mm wide, centered)                  │
    │                                              │
    │                                              │
    │                                              │
 84 │ ┌───────────────────────────────────────┐    │
    │ │         CARTRIDGE SLOT                │    │
    │ │     (150mm wide, centered)            │    │
    │ │         front panel opening           │    │
  0 └─┴───────────────────────────────────────┴────┘
    0                  136                       272
                      X (width, mm)
```

### 2c. Top-View Cross-Section (Width X vs. Depth Y)

```
Y(mm)
342 ┌─────────────────────────────────────────────┐
    │       bag connectors (bottom-back)          │
    │            ●● at ~Y=320                     │
    │                                      [ELEC] │
    │      bag slab (190mm wide, centered)  pocket│
    │      extends diagonally into page    80x80mm│
    │      from Y~0 (top-front, high Z)          │
    │      to Y~320 (bottom-back, low Z)          │
    │                                              │
    │════════ tubes along floor ════════           │
    │                                              │
165 │ ┌──────────────────────────────────┐         │
    │ │    DOCK BACK WALL (fittings)     │         │
    │ └──────────────────────────────────┘         │
130 │                                              │
    │ ┌──────────────────────────────────┐         │
    │ │         CARTRIDGE                │         │
    │ │      (150W x 130D x 80H)        │         │
    │ │  pumps, motor axis along depth   │         │
    │ └──────────────────────────────────┘         │
  0 └─────────────────────────────────────────────┘
    0          61              211              272
                      X (width, mm)
```

### 2d. Component Position Table (V1-B)

| Component | X range (width) | Y range (depth) | Z range (height) | Dimensions (WxDxH) | Volume |
|-----------|----------------|-----------------|-------------------|---------------------|--------|
| Bag slab (2x 2L) | 41-231 | 0-320 | ~1-392 (varies diag.) | 190W x 320D x 286H bounding | ~3.18L (actual bag vol.) |
| Cartridge dock | 61-211 | 0-130 | 0-80 | 150W x 130D x 80H | 1.56L |
| Dock back wall | 61-211 | 130-165 | 0-84 | 150W x 35D x 84H | 0.44L |
| Lever / handle zone | 61-211 | 0-20 | 80-124 | 150W x 20D x 44H | 0.13L |
| Hopper funnel | 86-186 | 0-85 | 322-392 | 100dia x 70H | ~0.25L |
| Hopper lid zone | 86-186 | 0-85 | 392 (top surface) | 100dia x 10H | -- |
| Electronics pocket | 192-272 | 262-342 | 342-392 | 80W x 80D x 50H | 0.32L |
| Display 1 (left) | 20-60 | 0-5 | 260-300 | 40dia x 5D | -- |
| Display 2 (right) | 212-252 | 0-5 | 260-300 | 40dia x 5D | -- |
| Solenoid valves (4x) | 0-60 | 100-160 | 0-35 | ~30W x 60D x 35H each | 0.25L total |
| Hopper solenoids (2x) | 215-272 | 100-140 | 0-35 | ~25W x 40D x 35H each | 0.07L total |
| Drip tray | 0-272 | 0-50 | 0-4 | 272W x 50D x 4H | 0.05L |
| Tube channels (floor) | 41-231 | 130-320 | 0-10 | 190W x 190D x 10H | -- |

### 2e. Clearance Analysis (V1-B)

| Component Pair | Clearance | Assessment |
|----------------|-----------|------------|
| Cartridge top (Z=80) to lower bag surface at Y=130 | 288 - 130*0.839 - 80 = 288 - 109.1 - 80 = **99mm** | Generous |
| Cartridge rear (Y=165 incl. dock wall) to bag connectors (Y=320) | **155mm** along floor | Generous |
| Bag depth consumed (320mm) vs interior depth (342mm) | **22mm** margin | Workable but merits prototyping. Bag deformation under load could consume 5-10mm. |
| Hopper rear edge (Y=85) to bag top surface at Y=85 | Bag top at Y=85: 392 - 85*0.839 + 80*cos(40)/... Simplified: top of stack at Y=85 is approximately 392 - 85*tan(40) = 392 - 71.3 = **321mm**. Hopper bottom at Z=322. Gap = **1mm**. | Very tight. Hopper must stay within Y=0-80 to maintain 5mm clearance. |
| Electronics (Y=262-342, Z=342-392) to bag top surface at Y=262 | Top of bag at Y=262 is approximately 392 - 262*tan(40) + 80/cos(40) -- this requires care. The bag top surface at Y=262 is at Z = 392 - (262 - 0)*tan(40) = 392 - 219.8 = 172mm. The electronics start at Z=342. Gap = **170mm**. | Very generous |
| Solenoid valves to cartridge | Same arrangement as V1-A: adjacent in X, separated by ~1mm wall. | Adequate |
| Displays (Z=260-300) to hopper (Z=322) | **22mm** vertical gap | Comfortable |
| 2L bag width (190mm) to enclosure walls | (272 - 190) / 2 = **41mm** per side | Adequate for tubing and rails but tighter than 1L (60mm per side) |

**Identified tight clearances:**
1. Depth margin: 22mm between bag bounding box and back wall. Adequate but not generous.
2. Hopper to bag surface: near zero at Y=85. Hopper must be constrained to Y=0-80.
3. Top-back void: 106mm of height for electronics. Sufficient (ESP32 + driver boards fit in ~50mm laid flat) but leaves only 56mm margin.
4. Side clearance for 2L bags: 41mm per side. Rails and tubing must fit in this space.

**No interferences detected** with the constraints above.

---

## 3. Tube Routing Map

### 3a. Fluid Line Topology

Each flavor line has the following components in series:

```
BAG --> dip tube --> TEE1 --> [clean solenoid branch] --> pump (in cartridge) --> TEE2 --> [hopper solenoid branch] --> dispensing solenoid --> dispensing nozzle (exits enclosure)
```

The cartridge contains both pumps. All TEE junctions, solenoids, and check valves are in the dock or on the enclosure floor/walls (permanent, not in the cartridge).

### 3b. Tube Routing Paths (V1-A: 1L at 35deg in 300D)

**Segment 1: Bag connector to TEE1 (bag-side tee)**

- Bag connectors at approximately (136, 258, 29) -- centered in X, at Y=258, Z=29mm
- TEE1 mounted on enclosure floor at approximately (136, 200, 10)
- Path: short diagonal run along floor from Y=258 to Y=200, roughly following the bag slope
- Length: ~65mm
- Dead volume: 65mm of 6.35mm ID tubing = 2.1ml per line

**Segment 2: TEE1 to cartridge inlet (dock back wall)**

- TEE1 at (136, 200, 10)
- Dock fittings at approximately (100, 135, 40) and (172, 135, 40) for the two lines
- Path: along floor from Y=200 forward to Y=135, slight rise from Z=10 to Z=40
- Length: ~80mm per line
- Dead volume: 80mm of 6.35mm ID = 2.5ml per line

**Segment 3: Cartridge outlet (dock back wall) to TEE2 (pump-side tee)**

- Dock outlet fittings at approximately (100, 135, 55) and (172, 135, 55)
- TEE2 mounted near dock wall at approximately (100, 145, 55) and (172, 145, 55)
- Path: 10mm straight back from dock wall
- Length: ~10mm per line
- Dead volume: 0.3ml per line

**Segment 4: TEE2 to dispensing solenoid**

- TEE2 at (100, 145, 55) and (172, 145, 55)
- Dispensing solenoids at approximately (30, 130, 15) and (242, 130, 15)
- Path: lateral run along dock wall, then down to solenoid
- Length: ~120mm per line
- Dead volume: 3.8ml per line

**Segment 5: Dispensing solenoid to nozzle (exits enclosure)**

- Solenoid at (30, 130, 15) and (242, 130, 15)
- Nozzle exits through enclosure floor or back wall
- Length: ~50mm per line
- Dead volume: 1.6ml per line

**Segment 6: Hopper funnel to hopper solenoid to TEE2**

- Hopper funnel outlet at approximately (136, 50, 322)
- Hopper solenoids at approximately (243, 120, 15) and (243, 140, 15)
- Path: vertical drop from funnel (Z=322) down front wall to floor, then lateral to solenoid, then to TEE2
- Length: ~350mm per line (long vertical run)
- Dead volume: 350mm of 4.5mm ID = 5.6ml per line

### 3c. Summary: Dead Volume per Flavor Line (V1-A)

| Segment | Length (mm) | ID (mm) | Dead Volume (ml) |
|---------|-------------|---------|-------------------|
| Bag to TEE1 | 65 | 6.35 | 2.1 |
| TEE1 to cartridge inlet | 80 | 6.35 | 2.5 |
| Cartridge outlet to TEE2 | 10 | 6.35 | 0.3 |
| TEE2 to dispensing solenoid | 120 | 6.35 | 3.8 |
| Dispensing solenoid to nozzle | 50 | 6.35 | 1.6 |
| **Total dispensing path** | **325** | | **10.3** |
| Hopper to hopper solenoid to TEE2 | 350 | 4.5 | 5.6 |
| **Total hopper fill path** | **350** | | **5.6** |

Total tubing length per flavor line: ~675mm (325mm dispensing + 350mm hopper).
Total dead volume per flavor line (dispensing): ~10.3ml.
Total dead volume (both lines, dispensing): ~20.6ml.

### 3d. Tube Routing Paths (V1-B: 2L at 40deg in 350D)

The 2L layout has a deeper enclosure (342mm interior vs 292mm) and bag connectors further back (Y~320 vs Y~258).

| Segment | Length (mm) | ID (mm) | Dead Volume (ml) |
|---------|-------------|---------|-------------------|
| Bag to TEE1 | 85 | 6.35 | 2.7 |
| TEE1 to cartridge inlet | 110 | 6.35 | 3.5 |
| Cartridge outlet to TEE2 | 10 | 6.35 | 0.3 |
| TEE2 to dispensing solenoid | 120 | 6.35 | 3.8 |
| Dispensing solenoid to nozzle | 50 | 6.35 | 1.6 |
| **Total dispensing path** | **375** | | **11.9** |
| Hopper to hopper solenoid to TEE2 | 380 | 4.5 | 6.0 |
| **Total hopper fill path** | **380** | | **6.0** |

Total tubing length per flavor line: ~755mm.
Total dead volume per flavor line (dispensing): ~11.9ml.
Total dead volume (both lines, dispensing): ~23.8ml.

### 3e. Tube Routing Design Notes

- All floor-level tubes run in printed channels (U-profile, 10mm wide x 8mm deep) to prevent kinking and keep the floor organized.
- Tubes cross under the bag slab in the large front-bottom void. They never need to route over or around the bags.
- The hopper fill tubes are the longest runs in the system (~350-380mm vertical drop from ceiling to floor). These tubes should be silicone (flexible, food-grade) routed along the front wall interior, clipped at 100mm intervals.
- CPC fittings at the dock wall add ~35mm of depth to each connection versus JG fittings (~20mm), but this is absorbed by the generous void.

---

## 4. Access and Serviceability

### 4a. Hopper Refill (Weekly)

**Sequence:**
1. Open cabinet door
2. Pull slide-out tray forward ~200-300mm (enclosure comes to cabinet door opening under room light)
3. Flip hinged top lid (or remove top panel with magnets/snap-fits). Lid is ~40mm tall, hinges at rear edge. At 90-degree open, total height = 400 + 35mm (tray) + 40mm (lid) = 475mm. Fits within standard 500-600mm cabinet clearance.
4. Select flavor on front-panel display (tap left or right display, or single button press)
5. Pour concentrate from bottle into silicone funnel. Funnel holds 200-300ml buffer. Pump runs in reverse, pulling syrup from funnel through hopper solenoid into the bag. Pour rate matches pump rate (~200-400 ml/min). For 1L refill: 2.5-5 minutes of pouring.
6. Display shows progress. When bag-side capacitive sensor detects full, pump stops and display shows "Complete."
7. Optionally pour 50ml of clean water to flush the hopper line.
8. Remove silicone funnel, rinse in sink (30 seconds). Replace.
9. Close lid, push tray back, close cabinet door.

**Tool-free.** No tools required for any step.

### 4b. Cartridge Swap (Every 18-36 Months)

**Sequence:**
1. Open cabinet door
2. Pull slide-out tray forward
3. Cartridge slot is at the bottom of the front panel (Z=0 to Z=84mm, or Z=0 to Z=124mm with lever)

**With CPC fittings (recommended):**
4. Grip cartridge pull handle at front face
5. Pull firmly forward (~15-25N force). All 4 CPC connections break simultaneously. Auto-shutoff valves close on both halves -- zero dripping.
6. Slide old cartridge out along floor rails. Set on cabinet floor.
7. Slide new cartridge in along floor rails until CPC connections click (audible confirmation).
8. Pogo pins on dock ceiling make electrical contact with cartridge top pads automatically.
9. Push tray back, close door.

**With JG fittings (fallback):**
4. Flip cam lever on cartridge front face to release position
5. Push lever inward ~3mm to depress all 4 JG collet rings via release plate
6. Pull cartridge forward while holding lever in release position
7. Expect ~1-3ml of dripping from open tube stubs. Drip tray on enclosure floor catches this.
8. Slide new cartridge in; JG collets grip automatically on tube stub insertion
9. Flip cam lever to locked position
10. Push tray back, close door.

**Tool-free.** No tools for either approach.

### 4c. Front Face Layout (Bottom to Top)

```
Z(mm)  FRONT PANEL
392 ┬──────────────────────────────────────┐
    │        [ LID / TOP ACCESS ]          │ ← hinged or removable
    │     (hopper funnel beneath)          │
322 ├──────────────────────────────────────┤
    │                                      │
    │    [D1]     product logo     [D2]    │ ← round displays
    │   display                  display   │
260 │                                      │
    │         (blank panel area)           │
    │                                      │
124 ├──────────────────────────────────────┤
    │     (lever clearance zone if JG)     │ ← cam lever swing
 84 ├──────────────────────────────────────┤
    │  ┌──────────────────────────────┐    │
    │  │     CARTRIDGE SLOT           │    │ ← 150mm wide opening
    │  │   (pull handle or cam lever) │    │
    │  └──────────────────────────────┘    │
  0 └──────────────────────────────────────┘
```

The front face has three visual zones:
1. **Bottom (0-124mm):** Cartridge maintenance zone. Infrequently accessed. Slot can be a contrasting color or have a subtle pull handle.
2. **Middle (260-320mm):** Status zone. Two round displays showing flavor levels, status, settings.
3. **Top (322-392mm):** Hopper access zone. Lid or panel that opens for refilling.

### 4d. Components Requiring Tools

**None for user-facing operations.** All user tasks (hopper refill, cartridge swap) are tool-free.

Internal service (bag replacement, electronics repair) requires disassembly of the enclosure shell, which uses screws. These are manufacturing/repair operations, not user operations. Bags are permanent and never replaced by the user.

---

## 5. Structural Members

### 5a. Angled Bag Rails

**Purpose:** Support the diagonal bag slab, prevent sag, maintain consistent drainage angle.

**Design:** Two parallel PETG rails running diagonally from front-upper mounts to back-lower mounts, spaced to match bag width. Each rail is a U-channel (15mm wide x 10mm deep) that cradles the bag edges.

| Parameter | V1-A (1L) | V1-B (2L) |
|-----------|-----------|-----------|
| Rail length | ~280mm (bag length) | ~350mm (bag length) |
| Rail spacing | 152mm (1L bag width) | 190mm (2L bag width) |
| Rail cross-section | 15W x 10H mm U-channel | 15W x 10H mm U-channel |
| Material | PETG (3D printed for prototype) | PETG (3D printed for prototype) |
| Mount points | Front wall (Z~350, Y~10) and back wall (Z~30, Y~260) | Front wall (Z~340, Y~10) and back wall (Z~10, Y~320) |
| Width consumed | 30mm total (15mm per side) | 30mm total (15mm per side) |
| Load capacity needed | 2.2kg (two full 1L bags) | 4.4kg (two full 2L bags) |

A thin PETG separator sheet (1-2mm) between the two stacked bags prevents them from sticking.

**Attachment:** Rails screw into heat-set inserts in the enclosure side walls. Two screws per rail (one at each end) plus one intermediate screw for the 2L rails (to prevent flex along the 350mm span).

### 5b. Cartridge Guide Rails

**Purpose:** Guide the cartridge during insertion/removal, carry cartridge weight, prevent lateral wobble.

**Design:** Two parallel floor rails (2mm tall x 3mm wide, full slot depth of 130mm) on the enclosure floor. Two side wall guides (1.5mm wide rails, 0.3-0.5mm clearance per side) on the cartridge slot side walls.

| Parameter | Value |
|-----------|-------|
| Floor rail length | 130mm (cartridge depth) |
| Floor rail height | 2mm |
| Side guide length | 130mm |
| Chamfer at entrance | 5mm at 30-degree taper for blind insertion |
| Material | Integral to enclosure shell (injection molded PP/ABS) |

### 5c. Electronics Shelf

**Purpose:** Mount the ESP32, driver board, fuse block, and wiring in the top-back corner.

**Design:** A horizontal shelf or L-bracket in the top-back corner. The shelf is screwed to the back wall and one side wall.

| Parameter | V1-A (1L) | V1-B (2L) |
|-----------|-----------|-----------|
| Shelf position | Z=342, Y=222-292, X=192-272 | Z=342, Y=262-342, X=192-272 |
| Shelf dimensions | 80W x 70D mm | 80W x 80D mm |
| Available height above shelf | 50mm (to ceiling) | 50mm (to ceiling) |
| Material | 3mm PETG sheet (prototype), injection molded PP/ABS (production) |
| Mounting | 2 screws into heat-set inserts on back wall + 1 screw into side wall |

Components mounted on the shelf: ESP32 dev board (~50x25mm), motor driver (~40x25mm), fuse holder (~30x15mm), terminal blocks for solenoid wiring. All components are within the 80x70mm (V1-A) or 80x80mm (V1-B) footprint at <50mm total stack height.

### 5d. Hopper Funnel Mount

**Purpose:** Hold the removable silicone funnel in a fixed position at the top of the enclosure.

**Design:** A rigid PP or ABS mounting ring permanently attached to the enclosure structure just below the lid line. The silicone funnel press-fits into the ring. The ring has a 15mm outlet hole connecting to hopper tubing.

| Parameter | Value |
|-----------|-------|
| Ring outer diameter | 110mm |
| Ring inner diameter | 100mm (matches funnel) |
| Ring height | 15mm |
| Position | Centered at X=136, Y=50, Z=315-330 |
| Attachment | 2 screws into front wall or integral to enclosure molding |

### 5e. Dock Back Wall

**Purpose:** Hold fluid fittings (CPC or JG), alignment features, and pogo pin housing.

**Design:** A vertical wall spanning the cartridge slot width, at the rear of the slot. Contains 4 fluid fitting bores (2x2 grid) and a pogo pin housing on its upper surface.

| Parameter | Value |
|-----------|-------|
| Wall position | Y=130-165 |
| Wall dimensions | 150W x 35D x 84H mm |
| Material | PP or ABS (injection molded) |
| Fitting bores | 4x 22.4mm (CPC) or 4x 12.7mm (JG), in 2x2 grid with 30mm center spacing |
| Pogo pin housing | 3-pin block on top surface, Z=80-84mm |

### 5f. Material Summary

| Component | Prototype Material | Production Material | Notes |
|-----------|-------------------|---------------------|-------|
| Enclosure shell | 3D printed PETG | Injection molded PP or ABS | 4mm walls |
| Bag rails | 3D printed PETG | Injection molded PP or stainless rod | Must handle 4.4kg at 40deg |
| Cartridge guide rails | Integral to shell | Integral to shell | PP/ABS |
| Electronics shelf | 3mm PETG sheet | Molded as part of back wall | PP/ABS |
| Hopper funnel | Food-grade silicone (Shore 40-60A) | Same | Removable, dishwasher safe |
| Funnel mount ring | 3D printed PETG | Injection molded PP | Rigid |
| Dock back wall | 3D printed PETG | Injection molded PP or ABS | Holds fittings under load |
| Drip tray | 3D printed PETG | Injection molded PP | Removable, food-grade |
| Bag separator | 1-2mm PETG sheet | Thin PP sheet | Between stacked bags |

### 5g. Enclosure Wall Attachment

All internal structural members attach to the enclosure walls via one of:
- **Heat-set brass inserts** (M3, 5mm long): For prototype PETG enclosures. Press in with soldering iron. Reusable.
- **Boss-and-screw** (integral molded bosses): For production injection molded enclosures. Bosses on inner wall surfaces receive self-tapping screws.
- **Snap-fit tabs**: For the drip tray and hopper funnel mount (removable by design).

---

## 6. Thermal and Ventilation

### 6a. Heat Sources

| Component | Power Dissipation | Location |
|-----------|-------------------|----------|
| ESP32 | ~0.5W continuous, ~1W peak (WiFi) | Electronics pocket, top-back |
| Motor drivers (2x) | ~0.5W each during operation (intermittent) | Electronics pocket |
| Solenoid valves (6x) | ~2-4W each when energized (intermittent, <30s at a time) | Front-bottom, near cartridge |
| Kamoer pumps (2x, in cartridge) | ~10W each at peak, ~5W sustained | Cartridge, front-bottom |

**Total sustained heat during dispensing:** ~12-15W (both pumps + 2 solenoids + ESP32).
**Idle heat:** ~0.5-1W (ESP32 only).

### 6b. Thermal Path

Heat from electronics (top-back corner) rises naturally upward and exits through the enclosure top. The under-sink cabinet provides natural convection -- warm air rises out the top of the cabinet, cooler air enters from below.

Heat from the cartridge pumps (front-bottom) is intermittent (30-60 seconds per dispensing event) and dissipates through the cartridge shell and surrounding air. The large front-bottom void provides ample air volume for convective cooling.

Solenoid valve heat is intermittent and low total energy. Not a concern.

### 6c. Ventilation

**Ventilation slots (recommended):**
- **Top panel:** 4-6 slots (3mm x 30mm each) along the rear edge of the top panel, above the electronics pocket. Warm air exits here.
- **Bottom panel:** 4-6 matching slots along the rear of the enclosure floor, behind the cartridge slot. Cool air enters here. These also provide drainage for any condensation or minor spills.
- **Slot orientation:** Slots run left-right (parallel to the width axis) to minimize dust ingress while allowing vertical airflow.

**No fan required.** The peak sustained power (15W) in a 31-36L enclosure produces a negligible temperature rise. ESP32 operates reliably up to 85C; ambient under-sink temperature is 20-30C. Even with zero ventilation, the internal temperature rise from 15W intermittent load would be <5C.

### 6d. Proximity of Electronics to Fluid

| Electronics Component | Nearest Fluid Component | Distance | Risk |
|----------------------|------------------------|----------|------|
| ESP32 (top-back) | Bag slab (diagonal, closest at back wall) | 64mm (V1-A) to 170mm (V1-B) vertical separation | Very low. Bags are sealed, permanently mounted. No splash risk. |
| ESP32 (top-back) | Hopper tubing (runs along front wall) | ~200mm horizontal separation | None. Different regions of enclosure. |
| Motor drivers (top-back) | Same as ESP32 | Same | Same |
| Solenoid valves (front-bottom) | Cartridge fittings (front-bottom) | 30-60mm | Moderate. Solenoids are IP65 rated bodies, water resistant. A drip during cartridge swap could reach them but does no damage. |
| Pogo pins (dock ceiling) | CPC/JG fittings (dock back wall) | 30-40mm | Managed by physical separation. Pogo pins face down from ceiling; fittings face rearward from wall. Gravity pulls drips away from pins. Drainage channel on dock ceiling provides secondary protection. |

**Thermal effect on fluid:** The electronics pocket (0.5-1W continuous) is separated from the nearest bag surface by 64-170mm of air. The temperature gradient at the bag surface due to electronics heat is negligible (<0.5C). No effect on concentrate freshness or flavor stability.

---

## 7. Conflicts and Unresolved Questions

### 7a. Discrepancies Between Research Documents

**1. Hopper depth constraint:**
- The bag placement research defines the bag bounding box starting at Y=0 (front wall) with sealed ends at the top.
- The hopper research places a 100mm-deep funnel at Y=0-100.
- At Y=85-100, the bag top surface is at Z=322-331mm (V1-A) or Z=306-321mm (V1-B).
- A hopper extending to Y=100 at Z=322 has near-zero clearance to the bag surface.
- **Resolution:** Constrain the hopper to Y=0-80mm maximum depth. A 100mm-diameter funnel mounted at Y=0-80 has its center at Y=40, which is fine. The conical shape means the funnel is narrower at the bottom (outlet at ~15mm diameter) and fits within the Y=0-80 envelope.

**2. Bag bounding box depth at 35deg for 2L:**
- The bag placement research reports 333mm depth consumed, with only 9mm margin in the 342mm interior.
- The cartridge research assumes bags fit at 35deg and computes clearances accordingly.
- The hopper research uses 35deg for its 2L geometry examples.
- **This 9mm margin is the single tightest tolerance in the entire design.** Bag deformation under load could add 5-10mm.
- **Resolution:** The recommended configuration for 2L uses 40deg (22mm margin), not 35deg. The 35deg configuration (Config 3 in the bag placement doc) is flagged for prototyping only.

**3. Solenoid valve placement:**
- The cartridge dock research mentions solenoids beside the cartridge on the floor, behind the dock wall, or on the back wall, without committing.
- The hopper research mentions hopper solenoids near TEE2 (front-bottom) to minimize dead volume.
- **Resolution:** All 6 solenoid valves (2 dispensing, 2 clean, 2 hopper) are placed in the front-bottom void, flanking the cartridge. There is ample room: the cartridge is 150mm wide in a 272mm enclosure, leaving 122mm for valves (4 valves at ~30mm width = 120mm). Tight but feasible. Alternative: stack valves vertically (two per side in a 2-high arrangement) to halve the width requirement to 60mm.

### 7b. What Needs Physical Prototyping

1. **Bag deformation under load at 35-40 degrees.** Do full bags expand beyond the nominal stack thickness (50mm for 1L, 80mm for 2L)? This determines whether the 22mm depth margin (V1-B at 40deg) is sufficient.

2. **Bag sag between rails.** With rails only at the bag edges (152mm or 190mm apart), how much does the bag center sag when full? If >10mm, a center support rail is needed. This affects the bag-to-cartridge clearance and drainage consistency.

3. **Dip tube behavior during drainage at 35-40 degrees.** At what fill level does air first reach the dip tube opening? This determines the practical residual volume.

4. **Dip tube behavior during refilling.** Can the pump push concentrate uphill through the dip tube into an angled bag? Air must counter-flow downward past the rising concentrate. At 40deg for 2L, the hydrostatic head is ~2.3 kPa (0.33 PSI) -- within pump capability but air displacement needs testing.

5. **Hopper funnel position.** Can the user comfortably pour into a funnel at Z=322-392mm when the enclosure is on a slide-out tray at cabinet floor level (~200mm below countertop)? Total pour height is approximately 200mm (cabinet floor) + 35mm (tray) + 322mm (funnel bottom) = 557mm above floor, or roughly at waist height for a standing adult. This should be comfortable, but test with actual users.

6. **Cartridge floor-level insertion.** Is sliding a cartridge along the cabinet floor into a bottom-mounted slot ergonomically acceptable? The user is crouching. The slot is at floor level. Prototype and user-test.

7. **CPC simultaneous disconnect force.** Can a user pull the cartridge forward with enough force (15-25N) to break all 4 CPC connections at once? This is about 3-5 lbs of force -- should be trivial but verify with actual CPC samples.

### 7c. Open Design Decisions

1. **CPC vs JG fittings.** CPC is recommended but costs $70 vs $8. This is the largest cost delta in the dock design. The product owner must weigh the UX benefit (zero drip, no cam lever) against the cost.

2. **One hopper funnel vs two.** The hopper research recommends a single funnel with firmware-controlled valve selection (Option D). Two physical funnels (Option A/B) are simpler but use more space and require the user to identify the correct funnel. Decision depends on whether the firmware UI for flavor selection is ready for the first prototype.

3. **Hopper capacity: 200ml vs 300ml vs 500ml.** 200-300ml is recommended as a funnel (user pours while pump drains). 500ml allows a half-refill pour-and-walk-away for 1L bags. The space allows up to 500ml in V1-A (180mm clearance) but only ~300ml in V1-B (126mm clearance at front).

4. **Slide-out tray: included or BYO?** The access architecture research recommends a third-party slide-out tray ($15-35). Should the product include one, or specify "compatible with standard under-sink slide-out trays" and let the user provide their own?

5. **Enclosure depth for 2L: 350mm vs 370mm.** A 370mm-deep enclosure (362mm interior) would give 42mm margin for 2L bags at 40deg instead of 22mm. The under-sink research confirms 480-510mm of usable depth -- 370mm is still well within budget. The tradeoff is a marginally larger enclosure.

---

## 8. Comparison: Vision 1 Diagonal vs. Archived Zone-Based Layout

The zone-based (horizontal layer cake) layout was the prior assumption. Components stacked in horizontal slices: bags at the bottom, dock shelf in the middle, cartridge above that, electronics on top.

### 8a. Comparison Table

| Criterion | Vision 1 (Diagonal Interleave) | Zone-Based (Horizontal Layers) |
|-----------|-------------------------------|-------------------------------|
| **Enclosure for 1L** | 280x300x400 (33.6L) | 280x250x400 (28.0L) |
| **Enclosure for 2L** | 280x350x400 (39.2L) | Not feasible in 400H. Needs 280x300x450+ (~38L+). |
| **2L bags feasible?** | Yes, at 40deg in 350D enclosure | Only at 55-65deg in 450H enclosure, with severe height budget pressure |
| **Height budget conflicts** | None. Components share height at different depths. | Severe. Every mm of bag zone steals from dock/electronics zone and vice versa. |
| **Width budget conflicts** | None at the cartridge. Bags and cartridge are at different depths. | None (same -- full width available per zone). |
| **Depth budget** | Bags consume 258-320mm. Enclosure must be deeper. | Bags lie flat, consume less depth (~152mm). Enclosure can be shallower. |
| **Cartridge ergonomics** | Floor-level insertion. User rests cartridge on cabinet floor during alignment. | Mid-height insertion. User holds cartridge in the air while aligning. |
| **Tube routing** | Front-to-back along floor. ~325-375mm per line. | Short vertical runs. ~150-200mm per line. |
| **Dead volume** | 10-12ml per line (longer tubes) | 6-8ml per line (shorter tubes) |
| **Drainage** | Gravity-assisted at 35-40deg. Good (sin=0.57-0.64). | Gravity-assisted at 18-45deg. Range depends on configuration. |
| **Internal structure** | Angled rails, no horizontal shelves. More 3D. | Horizontal shelves at zone boundaries. Simple stacking. |
| **Component access** | All from front face (cartridge at bottom, displays middle, hopper at top). | All from front face (dock at mid-height, funnels above). |
| **Bag installation** | Diagonal mount from corner to corner. Requires 3D bracket alignment. | Flat or gently inclined. Simpler spatial reasoning. |
| **Enclosure depth** | 300-350mm (larger) | 250mm (more compact) |

### 8b. What Vision 1 Gains

1. **2L bag compatibility without increasing height.** The diagonal uses the enclosure's cross-sectional diagonal (~490mm in a 350D x 400H box) instead of the height alone (400mm). A 350mm bag fits easily.
2. **Elimination of height budget competition.** The zone-based layout's central challenge -- allocating 400mm among bag zone, dock shelf, cartridge cavity, lever clearance, and electronics zone -- disappears entirely. Every component has ample space.
3. **Better cartridge ergonomics.** Floor-level insertion is easier than mid-height insertion when crouching under a sink.
4. **Structural simplicity at the dock.** No mid-height shelf cantilevering the dock and cartridge weight. The dock is at the floor, the strongest structural position.
5. **Scalability.** 1L and 2L variants share the same cartridge, electronics, front panel, and hopper. Only the shell depth and bag rails change.

### 8c. What Vision 1 Loses

1. **Compactness in depth.** The 300-350mm depth is 50-100mm deeper than the zone layout's 250mm. Under most sinks this is free, but some tight installations may prefer the shallower box.
2. **Tube routing length.** Front-to-back floor runs are 60-80% longer than the zone layout's short vertical drops. Dead volume increases proportionally (~10ml vs ~7ml per line).
3. **Structural complexity.** Angled rails are harder to manufacture and align than horizontal shelves. The 3D geometry requires more careful tolerance management.
4. **Bag installation complexity.** Diagonal mounting from corner to corner requires precise bracket alignment during manufacturing. In the zone layout, bags lay on a flat shelf.

### 8d. Worth Adopting from Zone Layout

1. **Horizontal electronics shelf.** The zone layout's top-level electronics shelf is a simple, proven design. Vision 1 already adopts this -- the electronics pocket in the top-back corner is effectively a small horizontal shelf.
2. **Dock wall fitting layout.** The 2x2 fitting grid, pogo pin placement, and alignment pin geometry from the zone-based mating face research transfers directly. No changes needed.
3. **Solenoid valve grouping.** The zone layout grouped solenoids on the dock shelf for short tube runs. Vision 1 should similarly group solenoids near the dock wall in the front-bottom void, not scatter them around the enclosure.

---

## 9. Layout Selection Guidance

**Choose V1-A (1L, 280x300x400)** when:
- Target market is moderate-use households (1-2 flavored drinks per day)
- Under-sink space is constrained in depth (some cabinets have pipes at 350mm)
- Cost targets favor smaller enclosures and smaller bags
- Biweekly refill cadence is acceptable

**Choose V1-B (2L, 280x350x400)** when:
- Target market is heavy-use households (3-5+ flavored drinks per day) or small commercial
- Monthly refill cadence is strongly preferred
- Under-sink depth is not constrained (most standard cabinets have 480-510mm usable)
- The 50mm depth increase is acceptable

**Both variants share:** Cartridge design, electronics, firmware, front panel layout, hopper design, fluid topology, displays, and all user-facing interactions. The only variant-specific components are the enclosure shell, bag rails, and bags themselves.

---

## Appendix: Derived Geometry Reference

### Bag Lower Surface Height at Any Depth

For either layout, the height of the lower bag surface at depth Y from the front wall:

    Z_lower(Y) = Z_front - Y * tan(theta)

Where Z_front = H_int - T_stack / cos(theta).

| Layout | theta | Z_front | Formula |
|--------|-------|---------|---------|
| V1-A (1L, 35deg) | 35 | 331mm | Z = 331 - 0.700*Y |
| V1-B (2L, 40deg) | 40 | 288mm | Z = 288 - 0.839*Y |

### Front-Bottom Void Height at Key Depths

| Depth Y (mm) | V1-A Z_lower (mm) | V1-B Z_lower (mm) |
|---------------|--------------------|--------------------|
| 0 (front wall) | 331 | 288 |
| 50 | 296 | 246 |
| 100 | 261 | 204 |
| 130 (cart rear) | 240 | 179 |
| 150 | 226 | 162 |
| 200 | 191 | 120 |
| 250 | 156 | 78 |
| 292 (V1-A back) | 127 | -- |
| 300 | -- | 36 |
| 320 (V1-B bag end) | -- | 20 |
| 342 (V1-B back) | -- | 1 |

These values confirm the trapezoid shape of the front-bottom void: tall at the front (288-331mm), tapering to near zero at the back wall.

### Bag Upper Surface Height at Key Depths

Upper surface = lower surface + T_stack / cos(theta) projected vertically. More precisely, the upper surface at depth Y:

    Z_upper(Y) = Z_front + T_stack / cos(theta) - Y * tan(theta)
               = H_int - Y * tan(theta)

| Depth Y (mm) | V1-A Z_upper (mm) | V1-B Z_upper (mm) |
|---------------|--------------------|--------------------|
| 0 (front wall) | 392 | 392 |
| 50 | 357 | 350 |
| 100 | 322 | 308 |
| 150 | 287 | 266 |
| 200 | 252 | 224 |
| 250 | 217 | 182 |
| 292 (V1-A back) | 188 | -- |
| 320 (V1-B bag end) | -- | 124 |
| 342 (V1-B back) | -- | 105 |

The space above Z_upper at any depth is available for the hopper zone and electronics. At the front (Y=0), this is 0mm (bags touch ceiling). At the back, this grows to 204mm (V1-A) or 287mm (V1-B at back wall: 392 - 105 = 287mm).
