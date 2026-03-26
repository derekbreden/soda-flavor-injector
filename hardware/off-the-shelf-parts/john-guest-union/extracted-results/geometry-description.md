# John Guest PP0408W 1/4" Union — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the John Guest PP0408W push-to-connect union fitting in enough detail that an agent generating engineering drawings or 3D-printable mounts can precisely model every surface, shoulder, and interface zone — without holding the part.

## Overall Form

The fitting is a **symmetric inline union**: a single white acetal copolymer body with identical tube-accepting ports on each end. It connects two pieces of 1/4" OD (6.35mm) tubing end-to-end through a straight-through internal passage. The fitting is rotationally symmetric about its long axis (axis of tube flow).

When viewed from the side, the profile is a **barbell**: two wider collet-ring sections (15.10mm OD) at each end, connected by a significantly narrower central body (9.31mm OD). This diameter step-down is critical for understanding how the fitting sits in pockets and how the release plate engages it.

## Axis Convention

- **Long axis (L):** The axis of tube flow, running through the center of both ports. All "length" dimensions are along this axis.
- **Radial (R):** Perpendicular to L. The fitting is axially symmetric — all cross-sections perpendicular to L are circles.

## Dimensional Profile — 5 Zones Along the Long Axis

The fitting has 5 distinct zones. Because the fitting is symmetric, only 3 unique zones exist. Described from left port inward:

```
  ┌──────┐              ┌──────┐
  │COLLET│  ┌────────┐  │COLLET│
  │15.10 │──│  9.31  │──│15.10 │
  │      │  │ center │  │      │
  │12.08 │  │ 12.16  │  │12.08 │
  └──────┘  └────────┘  └──────┘
  ◄─────── 36.32mm body ────────►
  ◄──── 39.13mm collets compressed ────►
  ◄───── 41.80mm collets extended ──────►
```

### Zone 1: Body End Section (left end)
- **OD: 15.10mm** (caliper-verified, photo 01) — this is the fixed housing that contains the collet mechanism
- **Length: 12.08mm** (caliper-verified)
- This section houses the internal collet mechanism: spring steel gripper teeth that grab the inserted tube, and the collet (release sleeve).
- The outer surface is smooth cylindrical white acetal.
- **End face:** Open circular port accepting 1/4" OD (6.35mm nominal / 6.30mm caliper-measured) tubing.
- End-on view of body end OD: **14.96mm** (photos 02, 03) — slight measurement variation from photo 01, likely parallax. Use 15.10mm as the controlling dimension.
- **Collet (release sleeve):** The visible moving ring at the end face that the user pushes inward to release the tube. This is the part the release plate must engage.
  - **Collet OD: 9.57mm** (caliper-verified, photos 04, 05)
  - **Collet wall thickness: 1.44mm** (caliper-verified)
  - **Collet ID: 6.69mm** (derived: 9.57 - 2×1.44 = 6.69mm) — the tube (6.30mm) passes through this opening with 0.39mm clearance
  - The collet protrudes from the body end face and translates axially inward when pushed
- **Blue variant:** Some PP0408W units include a blue plastic collet (visible in photo 08). Same mechanism and dimensions.

### Zone 2: Shoulder / Step-Down
- Sharp transition from 15.10mm body end OD to 9.31mm central body.
- This shoulder is a **radial face** — a flat annular ring perpendicular to the long axis.
- **Shoulder annular width:** (15.10 - 9.31) / 2 = **2.90mm** per side.
- These shoulders provide axial location when the center body sits in a pocket bore.

### Zone 3: Central Body
- **OD: 9.31mm** (caliper-verified, photo 06)
- **Length: 12.16mm** (caliper-verified)
- Smooth cylindrical section — the narrowest part of the fitting.
- This is the section that press-fits into the cartridge rear wall pocket. A **9.8mm bore** would give a sliding fit; a **9.5mm bore** would give a light press-fit.

### Zone 4 & 5: Mirror of Zones 2 & 1
Identical shoulder and collet ring on the opposite end.

## Overall Length Measurements

| Measurement | Value | What It Represents |
|-------------|-------|--------------------|
| Body without collets | 36.32mm | Sum of 12.08 + 12.16 + 12.08 (just the 3 solid body zones) |
| Collets compressed | 39.13mm | Full length with collet sleeves pushed fully inward (photo 07) |
| Collets extended | 41.80mm | Full length with collet sleeves in default/extended position (photo 08, blue ring visible) |
| Collet protrusion (total, both ends) | 39.13 - 36.32 = **2.81mm** | ~1.4mm per side when compressed |
| Collet travel (total, both ends) | 41.80 - 39.13 = **2.67mm** | ~1.3mm per side from extended to compressed |

## Caliper Measurements Summary

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 15.10mm | Collet ring OD, side view | HIGH |
| 02 | 14.96mm | Collet ring OD, end-on view | HIGH |
| 03 | 14.96mm | Collet ring OD, side angle (confirms 02) | HIGH |
| 04 | 9.57mm | Collet (release sleeve) OD, end-on | HIGH |
| 05 | 9.57mm | Same as 04, different angle | HIGH |
| 06 | 9.31mm | Central body OD (the narrow waist) | HIGH |
| 07 | 39.13mm | Overall length, collets compressed | HIGH |
| 08 | 41.80mm | Overall length, collets extended (blue ring visible) | HIGH |

## Corrections to Previous parts.md Values

| Parameter | Old Value | Corrected Value | Impact |
|-----------|-----------|-----------------|--------|
| Body OD | 12.7mm | **9.31mm** (center body), **15.10mm** (body ends) | Rear wall pocket bore must match 9.31mm center body, NOT 12.7mm. Current 13.0mm bore is too large for press-fit of 9.31mm body. |
| Overall length | 38.1mm | **39.13mm** (compressed) / **41.80mm** (extended) | Slightly longer than spec |
| Body end OD | ~12.7mm | **15.10mm** | Release plate outer bore must clear 15.10mm, not 12.7mm |
| Collet travel | TBD | **~1.3mm per side** (2.67mm total) | Release plate stroke of 3mm is more than sufficient |
| Collet OD | not measured | **9.57mm** | The moving release sleeve; release plate inner lip must hug this |
| Collet wall | not measured | **1.44mm** | Collet ID = 6.69mm; tube (6.30mm) passes through |

## Critical Design Implications

### Rear Wall Pocket Bore
The fitting's barbell profile means the pocket bore design is fundamentally different than assumed:
- **Option A (pocket grips center body):** Bore = ~9.5–9.8mm. The collet rings (15.10mm) protrude on both sides of the wall. The wall pocket is narrow but the grip is on the skinny section.
- **Option B (pocket clears collet ring):** Bore = ~15.5mm. The fitting slides through freely and must be retained by other means (snap ring, adhesive, interference on collet shoulder).
- **The current design assumed a uniform ~12.7mm cylinder — this must be redesigned** for the actual barbell profile.

### Release Plate Stepped Bores
Updated bore design based on caliper-verified collet dimensions (collet OD 9.57mm, wall 1.44mm, ID 6.69mm):
- **Tube clearance hole:** Must be between 6.30mm (tube OD) and 6.69mm (collet ID) — only 0.39mm design window. The plate face around this bore contacts the collet's annular end face (from collet ID 6.69mm to collet OD 9.57mm = full 1.44mm wall contact).
- **Inner lip (collet hugger):** Just over 9.57mm collet OD, as tight as manufacturing allows. This bore closely surrounds the collet for lateral constraint, preventing wobble during release — the same principle as the PI-TOOL's outer cradle but at the collet scale.
- **Outer bore (body end cradle):** Just over 15.10mm body end OD, as tight as manufacturing allows.

### Release Mechanism Stroke
The collet travel is ~1.3mm per side. The cam lever produces 3mm of plate travel. This means the release mechanism has ~1.7mm of margin — comfortable, but the plate must contact the collet ring before the stroke begins (i.e., the fitting must be seated so the collet ring is within reach of the plate's starting position).

## Geometry for 3D Modeling Agents

When modeling parts that interface with this fitting:

1. **The fitting is a barbell, not a cylinder.** Any pocket, bore, or clamp must account for the 15.10mm → 9.31mm → 15.10mm profile.
2. **Press-fit on the 9.31mm center body** is the natural mounting strategy. The shoulders provide axial location.
3. **Body end sections protrude at 15.10mm OD** — ensure clearance or intentional engagement (for release plate).
4. **The collet is the moving part (9.57mm OD, 1.44mm wall, 6.69mm ID).** The release plate pushes on the collet's annular end face. The plate's through-hole must be between 6.30mm (tube) and 6.69mm (collet ID) to contact the face. The inner lip must be just over 9.57mm to hug the collet for lateral constraint.
5. **Tube insertion depth:** ~16mm per side (industry standard, not directly measured here). Plan for 6.35mm (nominal) / 6.30mm (measured) tube protruding ~16mm into each end.
6. **Total axial footprint:** 42mm with collets extended, 39mm compressed, plus tube insertion lengths on each side.
