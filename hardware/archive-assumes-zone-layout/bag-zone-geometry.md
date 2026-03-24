# Bag Zone Geometry — 1L Incline-Mounted Bags in the 400mm Enclosure

> **DECISION (2026-03-24):** Enclosure height is locked at **400mm**. The device must be shorter than its companion soda water machines (Brio 460mm, Lillium 440mm) since our device is subordinate to theirs. This means **1L Platypus bags** (not 2L). Bags mount at an **18-20 degree incline** with two-point stretch (sealed end high, connector low). Two bags stack vertically within the bag zone.

This document defines the resolved bag zone geometry for the 400mm enclosure, incorporating incline bag mounting, dip tube drainage, drip tray removal, and pump-assisted filling.

---

## 1. 1L Platypus Bag Dimensions

| Parameter | Value |
|---|---|
| Height (when full) | ~250mm |
| Width | ~140mm |
| Thickness (when full) | ~40mm |
| Connector | 28mm threaded cap at one narrow end |
| Sealed end | Heat-sealed seam at opposite narrow end |

The Platypus Drink Tube Kit screws onto the 28mm connector. A dip tube extends from the cap through the bag interior, creating a sealed path. The bag is effectively a sealed vessel with a single port (the dip tube bore, 1/4" ID / 6.35mm).

---

## 2. Vertical Space Budget

The drip tray (previously 15mm) is removed. Analysis of every liquid scenario in the system found no justification for an internal drip tray — the fluid path is fully sealed during operation, there is no condensation source, and the single scenario it would help with (slow connector weep from loose zip ties) is better addressed by using hose clamps and proper assembly. The enclosure floor is a flat 4mm panel.

The "drip shelf" at ~310mm is retained as a structural/organizational shelf (electronics shelf), not a liquid barrier. It provides DIN rail mounting, ties the side walls together, and separates zones for assembly.

| Zone | Bottom (mm) | Top (mm) | Height (mm) | Contents |
|------|-------------|----------|-------------|---------|
| Floor panel | 0 | 4 | 4 | Flat PETG panel |
| Bag zone | 4 | 180 | 176 | Two 1L Platypus bags, incline-mounted at 18-20 deg |
| Dock shelf | 180 | 186 | 6 | 6mm PETG structural shelf |
| Cartridge cavity | 186 | 266 | 80 | Cartridge envelope (150W x 130D x 80H mm) |
| Lever clearance | 266 | 306 | 40 | Cam lever swing above cartridge top |
| Electronics shelf | 306 | 310 | 4 | Open structural shelf with cutouts for wiring and airflow |
| Electronics zone | 310 | 400 | 90 | ESP32, L298N x3, RTC, MCP23017, fuse block, DIN rail |
| **Total** | **0** | **400** | **400** | |

```
    ┌──────────────────────────────────────────────────────┐
    │  ESP32, L298N x3, RTC, DIN rail, fuse block         │ 400mm
    │                                                      │
    │  ═══════════ ELECTRONICS SHELF (310mm) ═══════════   │
    │                                                      │
    │  ┌── CARTRIDGE ──┐  solenoids, tees, needle valve    │
    │  │   dock        │  flow meter                       │
    │  └───────────────┘                                   │
    │  ═══════════ DOCK SHELF (180mm) ═══════════════════  │
    │                       11mm gap                       │
    │                             ──────────────*  BAG 2   │
    │                            /       sealed (169mm)    │
    │                 BAG 2     /   18 deg                  │
    │                          /                           │
    │                    *────*  connector (73mm)           │
    │                   ─ ─ ─ ─  5mm gap                   │
    │                         ──────────────*  BAG 1        │
    │                        /       sealed (126mm)         │
    │             BAG 1     /   18 deg                      │
    │                      /                               │
    │                *────*  connector (30mm)               │
    │  ═══════════ FLOOR PANEL (4mm) ═══════════════════   │ 0mm
    └──────────────────────────────────────────────────────┘
    FRONT                                             BACK
    ◄─────────────────── 250mm depth ──────────────────►
```

---

## 3. Incline Bag Mounting — Geometry

Each bag mounts at an 18-20 degree incline from horizontal, stretched between two fixed points: the connector (low, at the front of the enclosure) and the sealed end (high, at the rear). This replaces vertical hanging, which required 250mm+ of vertical space that is unavailable in the 400mm enclosure.

### 3a. Incline Geometry (18 degrees)

| Parameter | Value |
|---|---|
| Bag length | 250mm |
| Vertical rise (250 x sin 18) | 77mm |
| Horizontal run (250 x cos 18) | 238mm |
| Bag thickness (perpendicular) | ~40mm |
| Vertical projection of thickness (40 x cos 18) | ~38mm |
| Total vertical per bag (rise + thickness) | ~115mm |

The 238mm horizontal run fits within the 242mm interior depth with 4mm margin. The vertical dimensions accommodate two stacked bags within the 176mm bag zone.

### 3b. Why 18-20 Degrees

The angle is bounded by two constraints:

**Upper bound (~30 degrees):** At steeper angles, two stacked bags exceed the 176mm bag zone height. A single bag at 30 degrees fits (rise 125mm + thickness 35mm = 160mm), but two bags do not (the upper bag's sealed end reaches ~210mm, well above the 180mm dock shelf).

**Lower bound (~15 degrees):** At shallower angles, the horizontal run exceeds the 242mm interior depth, and gravity assistance becomes minimal (sin 15 = 0.26).

At 18-20 degrees, two stacked bags fit with 11-17mm of clearance to the dock shelf, and the gravity component along the bag axis is sin(18-20) = 0.31-0.34 — roughly one-third of gravitational force driving liquid toward the connector.

### 3c. Advantages Over Vertical Hanging

| Factor | Vertical Hanging | Incline Mount (18-20 deg) |
|---|---|---|
| Vertical space required | 250mm+ (bag + connector + hook) | ~140mm (rise + thickness + clearance) |
| Fits in 176mm bag zone | No — requires top folding or 500mm enclosure | Yes — designed for the space |
| Drainage reliability | Good (full gravity) | Good (partial gravity + pump suction) |
| Last 10-20% behavior | Bag becomes floppy, random collapse, potential kinking | Liquid pools at connector end, collapse is top-down along incline |
| Collapse control | Uncontrolled random folding | Two-point tension constrains collapse to thinning-in-place |
| Dip tube interaction | Tube hangs straight down, parallel to gravity | Tube extends UP along incline, reaches into liquid pool, last to see air |

---

## 4. Two-Bag Arrangement — Vertical Stacking

Two 1L bags are stacked vertically, both inclined in the same direction (connector at front-low, sealed end at rear-high). Each bag consumes the full interior depth (238mm of 242mm) and the full interior width (140mm of 272mm, centered with 66mm per side for tubing).

### 4a. Vertical Position of Each Bag (at 18 degrees)

| Parameter | Bag 1 (Lower) | Bag 2 (Upper) |
|---|---|---|
| Connector center height | 30mm | 73mm |
| Connector top edge | 49mm | 92mm |
| Sealed end center height | 107mm | 150mm |
| Sealed end top edge | 126mm | 169mm |

Gap between Bag 1 top and Bag 2 bottom at the connector end: 73 - 49 = 24mm (comfortable). At the sealed end, Bag 2 top reaches 169mm, leaving 11mm clearance to the 180mm dock shelf.

### 4b. Side Cross-Section

```
    ═══════════════ DOCK SHELF (180mm) ═══════════════
                                          ·  ← 11mm clearance
                                        * BAG 2 sealed (169mm top)
                                       /
                            BAG 2     /    18 deg
                                     /
                            * conn. /
                           (92mm)  * ← BAG 2 connector (73mm center)
                   ─ ─ ─ ─ ─ ─ ─ ─ ─  5mm gap
                          * BAG 1 sealed (126mm top)
                         /
              BAG 1     /    18 deg
                       /
              * conn. /
             (49mm)  * ← BAG 1 connector (30mm center)

    ═══════════════ FLOOR PANEL (4mm) ═══════════════
    FRONT                                        BACK
    ◄────────────── 238mm run ──────────────────►
```

### 4c. Top View

```
    ┌────────────────── 272mm interior ──────────────────┐
    │                                                     │
    │      ┌──────────────────────────────────────┐      │
    │      │                                      │      │
    │      │  BAG 1 and BAG 2 (same footprint)    │      │
    │      │  140mm wide, 238mm deep              │      │
    │      │  stacked vertically, not in depth    │      │
    │      │                                      │      │
    │      └──────────────────────────────────────┘      │
    │                                                     │
    │  ◄──── 66mm ────►◄── 140mm bag ──►◄── 66mm ────►   │
    │   tubing space          width        tubing space   │
    │                                                     │
    │  FRONT                                     BACK     │
    │  ◄──────────────── 242mm depth ──────────────────►  │
    └─────────────────────────────────────────────────────┘
```

Both bags share the same depth footprint (238mm). The 66mm clearance on each side of the bag provides space for tubing routing along the enclosure walls.

---

## 5. Drainage and the Dip Tube

The Platypus Drink Tube Kit creates a sealed path into the bag. The dip tube extends from the connector (low point) upward along the incline toward the sealed end, reaching into the liquid pool inside the bag.

### 5a. Dip Tube Orientation on Incline

```
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
    ═══ FLOOR PANEL ═══
```

The tube opening sits partway up the incline, in the middle of the liquid volume. As the bag drains, liquid slides downhill to the connector, but the tube opening remains submerged until the bag is nearly empty (last 5-10%).

### 5b. Drainage Efficiency

The inclined dip tube configuration achieves near-complete bag evacuation:

1. Gravity pulls liquid toward the connector (low point) at sin(18-20) = 0.31-0.34 of full gravitational force
2. The bag collapses from the sealed end (high) downward — the empty portion has no liquid behind it
3. The dip tube opening is the last point to be exposed to air, providing a natural buffer
4. At ~20% remaining (~200ml), liquid concentrates in a wedge at the connector end, roughly 80-100mm long x 140mm wide x 15-20mm thick

The dip tube adds ~11ml of dead volume (7.9ml inside bag + 3.2ml external), which is insignificant relative to 1L capacity (1-2%). This dead volume must be flushed during clean cycles.

### 5c. Kinking Analysis

On a stretched incline mount, kinking is largely a non-issue:

- **Kinks above the liquid line are irrelevant** — the collapsed upper bag is empty, nothing is trapped
- **Kinks below the liquid line are self-healing** — liquid weight on the uphill side pushes through folds as the pump draws from below
- **The stretched mount prevents severe kinking** — mild tension along the bag's length axis resists lateral folding; the bag thins in place rather than folding randomly

This effectively resolves the collapse/kinking concern from prior research without requiring elastic frames, rollers, or squeeze mechanisms.

### 5d. Flow Restriction

The dip tube (1/4" ID, 6.35mm) is NOT the flow bottleneck. The black silicone tubing (1/8" ID, 3.175mm) has 4x less cross-sectional area. The dip tube contributes less than 4% of total flow resistance (Poiseuille analysis). The Kamoer pump's suction easily overcomes the hydrostatic head in the dip tube (~0.37 PSI for a 250mm column).

---

## 6. Filling — Pump-Assisted via Reversed Pump

Bags are filled using the existing Kamoer peristaltic pump running in reverse. The hopper connects downstream of the pump (at a TEE between pump outlet and dispensing point). During refill, the pump reverses direction: pulling concentrate from the hopper and pushing it through the dispensing solenoid, through the bag-side tee, through the dip tube, and into the bag.

This approach was validated by the pump-assisted filling research, which corrected a prior misconception that the bag was an "open pouch" — the dip tube creates a sealed path, and bidirectional flow through it is already proven by existing priming and clean cycle operations.

### 6a. Refill Flow Path

```
Hopper → [hopper valve OPEN] → TEE2 → [pump REVERSED] → [disp solenoid OPEN] → TEE1 → dip tube → Bag
```

A check valve at the dispensing point prevents air from being pulled in during refill. The hopper should be sealable (silicone cap with duckbill valve for air inlet) to minimize air exchange with the bag contents.

### 6b. Hardware Additions

| Item | Qty | Est. Price |
|------|-----|-----------|
| 1/4" inline check valve (push-connect) | 2 | $6-10 |
| TEE fitting (pump outlet side) | 2 | $0 (already in hand) |
| Hopper solenoid valve (Beduan 12V NC) | 2 | $18 |

No additional pumps, drivers, or GPIO pins needed. Pump reversal is a 3-line firmware change (swap H-bridge inputs via L298N).

### 6c. Air Management During Filling

The bag is sealed — displaced air must exit back through the dip tube, creating counter-flow. At 2-5 PSI pump pressure, air compresses per Boyle's law, allowing ~85-90% fill on a single pump cycle. Multi-cycle fill-and-vent operations can achieve higher fill levels. For 1L bags, estimated fill time is 3-6 minutes with pump assist (vs 8-15 minutes gravity-only).

---

## 7. Mounting Hardware

### 7a. Connector End (Low Mount)

A 3D-printed snap-fit U-clip on the interior front wall grips the 28mm threaded cap. The user pushes the connector into the clip; friction and gravity hold it. Two clips: one at 30mm height (Bag 1), one at 73mm height (Bag 2).

### 7b. Sealed End (High Mount)

A standard 50mm binder clip (~$0.25) grips the heat-sealed seam. The clip's handles hook onto a 3D-printed J-hook on the rear wall or dock shelf underside. Two J-hooks positioned at the appropriate heights for each bag.

### 7c. Installation Sequence

1. Attach binder clip to bag's sealed seam
2. Hang binder clip on J-hook at rear of bag zone
3. Route connector down toward front
4. Push connector/cap into U-clip on front wall
5. Connect tubing (Platypus drink tube adapter already attached)

Estimated installation time: 15-30 seconds per bag.

### 7d. Complete Mount (Side View)

```
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
    ┌─────┤
    │U-clip│ (printed on front wall interior)
    │holds │ cap/connector
    └──────┘
    ═══════════ FLOOR PANEL ══════════════════════
    FRONT                                    BACK
```

---

## 8. Horizontal Cross-Section at Dock Height

At the dock shelf height (180mm), the dock and valve components occupy the space:

```
    TOP VIEW (at dock shelf height, 180mm)

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

The dock (150mm wide) sits on the left/center. Solenoid valves, needle valve, flow meter, and tees mount to the right of the dock on the shelf or on brackets attached to the right enclosure wall. The four solenoids (each ~80x35x45mm) arrange in a vertical column (~35mm wide x ~180mm tall) beside the dock.

---

## 9. Dimension Summary

### Active Design: 1L Bags, 400mm Height, Incline Mounting

| Dimension | Value |
|---|---|
| **Enclosure exterior** | **280W x 250D x 400H mm** |
| Wall thickness | 4mm |
| Interior | 272W x 242D x 392H mm |
| Floor panel | 0-4mm (4mm flat PETG) |
| Bag zone | 4-180mm (176mm) |
| Bag mounting angle | 18-20 degrees from horizontal |
| Dock shelf position | 180mm from floor |
| Dock shelf thickness | 6mm (180-186mm) |
| Cartridge cavity | 186-266mm (80mm) |
| Lever clearance | 266-306mm (40mm) |
| Electronics shelf | 306-310mm (4mm, open with cutouts) |
| Electronics zone | 310-400mm (90mm) |
| Cartridge slot center | ~226mm from floor (~9") |
| Weight (loaded) | ~7-8 kg (15-18 lbs) |

### Two-Bag Configuration

| Parameter | Bag 1 (Lower) | Bag 2 (Upper) |
|---|---|---|
| Connector center height | 30mm | 73mm |
| Sealed end center height | 107mm | 150mm |
| Sealed end top edge | 126mm | 169mm |
| Horizontal run | 238mm | 238mm |
| Width | 140mm (centered) | 140mm (centered) |
| Gap to next element | 5mm to Bag 2 | 11mm to dock shelf |

### Companion Machine Context

| Machine | Height | Width |
|---|---|---|
| Brio cold carbonated | 460mm | 220mm |
| Lillium cold carbonated | 440mm | 170mm |
| **Soda Flavor Injector** | **400mm** | **280mm** |

Our device is wider but shorter — it sits beside the companion machine as a clear accessory, not competing for visual dominance.

---

## 10. Historical Note

Prior analysis explored vertical hanging of 2L Platypus bags, which required a 500mm enclosure (280W x 250D x 500H mm) with a 265mm bag zone. That design was superseded by the 400mm height lock and the incline mounting approach, which solves the fundamental space problem without increasing enclosure height. The 2L vertical hanging analysis is preserved in earlier git history for reference.

---

## Sources

- [incline-bag-mounting.md](enclosure/research/incline-bag-mounting.md) — Incline geometry, two-bag stacking, mounting hardware, drainage analysis
- [dip-tube-analysis.md](enclosure/research/dip-tube-analysis.md) — Dip tube sealed path, orientation effects, flow restriction, dead volume
- [drip-tray-shelf-analysis.md](enclosure/research/drip-tray-shelf-analysis.md) — Drip tray removal justification, electronics shelf redesignation
- [pump-assisted-filling.md](enclosure/research/pump-assisted-filling.md) — Pump reversal fill method, topology correction, check valve design
- layout-spatial-planning.md — Enclosure master layout (280x250x400mm)
- cartridge-envelope.md — Cartridge dimensions (150W x 80H x 130D mm)
- hopper-and-bag-management.md — Bag dimensions, collapse analysis (historical)
