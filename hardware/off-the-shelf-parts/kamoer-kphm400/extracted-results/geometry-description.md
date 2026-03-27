# Kamoer KPHM400-SW3B25 Peristaltic Pump — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the Kamoer KPHM400-SW3B25 peristaltic pump in enough detail that an agent generating engineering drawings or designing a 3D-printed pump tray/cartridge can model every mounting surface, clearance zone, and interface — without holding the part.

**Reference photographs:** Caliper photos of the physical pump are in `../raw-images/`. Photo numbers referenced throughout this document (e.g., "photo 01") correspond to numbered files in that directory.

## Overall Form

The pump is a **two-body assembly**: a black plastic **pump head** housing (roughly square cross-section when viewed from front) permanently attached to a silver cylindrical **DC motor** via a white plastic **adapter plate**. The motor shaft enters the pump head from the rear, driving an internal 3-roller peristaltic mechanism.

When viewed from the front (tube connector face), the pump head appears nearly square with rounded corners. The motor protrudes behind it, creating an elongated overall profile. A **black mounting bracket** (stamped metal plate) is sandwiched between the pump head and motor, extending outward on two sides to provide mounting ears with screw holes.

## Axis and Orientation Convention

- **X (width):** Horizontal when pump is in its normal operating orientation. The mounting bracket ears extend along this axis.
- **Y (depth):** From front face (tube connectors) toward rear (motor terminals). This is the longest dimension.
- **Z (height):** Vertical. The pump head is roughly symmetric about the XY midplane.
- **Front face:** The face with the Kamoer branding label, yellow priming cap, 4 corner screws, and tube connector exits.
- **Rear:** Motor terminal end.

## Major Sections Along the Y (Depth) Axis

### Section 1: Pump Head Front Face (Y = 0)
- Nearly square when viewed head-on: **~62.6mm wide x ~62.6mm tall** (caliper-verified from multiple photos)
- 4x Phillips head screws at corners (hold the front cover plate on)
- Yellow priming cap at center (small, ~10mm diameter, press-fit)
- "Kamoer KPHM" branding label (black with yellow accent)
- **Two tube connector exits** protrude from this face:
  - White plastic barbed connectors for BPT tubing (4.8mm ID / 8.0mm OD)
  - Positioned offset from center — one above center, one below (or left/right depending on orientation)
  - Tube stubs protrude ~30–50mm from the face (flexible BPT tubing attached)

### Section 2: Pump Head Body (Y = 0 to ~48mm)
- Black plastic housing containing the peristaltic roller mechanism
- Cross-section is roughly square with rounded corners, ~62.6mm x ~62.6mm
- Depth of the pump head body: **~47.88–48.88mm** (caliper readings from photos 05/14 area, though see mounting hole notes below)

### Section 3: Mounting Bracket (Y ≈ 48mm)
- Black stamped metal bracket plate at the junction between pump head and motor
- **Extends beyond the pump head body on two sides** (mounting ears)
- Bracket width including ears: **~68.6mm** (matches datasheet — wider than 62.6mm pump head by ~3mm per side)
- **Mounting holes:**
  - **2x M3 through-holes** (hole diameter: **3.13mm**, caliper-verified from photo 06)
  - **Center-to-center spacing: 49.45mm** (derived: 47.88mm edge-to-edge + 3.13mm/2 per user measurement)
  - Measurement method: calipers squeezed into the sides of the holes measured 47.88mm edge-to-edge (photo 05), hole diameter measured directly at 3.13mm (photo 06)
  - Holes are positioned on the mounting ears, symmetric about the pump center axis
  - **Note:** This measures ONE axis of spacing. If the bracket has a 1x2 hole pattern (one hole per ear), this is the full pattern. If 2x2, the perpendicular spacing is TBD.
- Bracket thickness: ~1.5–2mm (estimated from photos)

### Section 4: Motor Adapter Plate (Y ≈ 48–52mm)
- White plastic disc/plate between bracket and motor
- Transitions from the square pump head bolt pattern to the round motor housing

### Section 5: DC Motor Body (Y ≈ 52mm to end)
- Silver cylindrical motor housing (standard 3xx-series DC motor form factor)
- **Motor diameter: ~35mm** (photos 15, 16 — display sideways, readings uncertain at ~34.54 / ~35.13mm)
- Motor has a flat on one side (standard anti-rotation feature)
- QR code, RoHS label, and Kamoer product label on motor body
- **Motor shaft nub:** A small protrusion from the center of the motor end cap (the non-drive end). This nub accounts for the 5mm difference between the two total-length measurements.
- **Motor terminal end:** Two solder tabs at the very rear

## Total Length Measurements

| Measurement | Value | What It Includes |
|-------------|-------|-----------------|
| Total length (with motor nub) | **116.48mm** | Front face to end of motor shaft nub (photos 07, 08) |
| Total length (without motor nub) | **111.43mm** | Front face to motor end cap, excluding nub (photo 09) |
| Motor shaft nub protrusion | **~5.05mm** | Difference: 116.48 - 111.43 |

## Additional Verified Dimensions

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 62.61mm | Pump head width, top-down | HIGH |
| 02 | 62.51mm | Pump head width, side view | HIGH |
| 03 | 62.61mm | Pump head width, front view | HIGH |
| 04 | 62.61mm | Pump head width, front face with branding | HIGH |
| 05 | 47.88mm | Mounting hole edge-to-edge spacing | HIGH |
| 06 | 3.13mm | Mounting hole diameter | HIGH |
| 07 | 116.48mm | Total length with motor nub (sideways display) | HIGH (user-verified) |
| 08 | 115.48mm | Total length with motor nub, alt angle | MEDIUM (similar to 07) |
| 09 | 111.43mm | Total length without motor nub (sideways display) | HIGH (user-verified) |
| 10 | ~68.74mm | Pump head depth including front cover | MEDIUM |
| 11 | 65.15mm | Pump body height or depth | HIGH |
| 12 | ~61.19mm | Pump head height, alt angle | MEDIUM |
| 13 | ~51.68mm | Pump head dimension (possibly including bracket) | MEDIUM |
| 14 | 48.88mm | Pump head depth, side view | HIGH |
| 15 | ~34.54mm | Motor body diameter (sideways display) | LOW — user verification needed |
| 16 | ~35.13mm | Motor body diameter (sideways display) | LOW — user verification needed |
| 17 | 82.82mm | Height with tube connectors or partial assembly span | MEDIUM |

## Cross-Referencing with Datasheet

The Kamoer datasheet lists: **68.6W x 115.6D x 62.7H mm**

| Dimension | Datasheet | Caliper | Notes |
|-----------|-----------|---------|-------|
| Width (X) | 68.6mm | 62.61mm (head), ~68.6 (with bracket ears) | Pump head is 62.6mm; bracket ears add ~3mm per side |
| Depth (Y) | 115.6mm | 116.48mm (with nub), 111.43mm (without) | Datasheet likely measures to motor end cap |
| Height (Z) | 62.7mm | 62.51–62.61mm | Pump head is nearly square |

## Mounting Hole Pattern — Critical for Pump Tray Design

```
          ◄── 49.45mm center-to-center ──►

    ○─────────────────────────────────────○
    │            mounting bracket          │
    │     ┌────────────────────────┐      │
    │     │      pump head         │      │
    │     │      62.6 x 62.6      │      │
    │     └────────────────────────┘      │
    │                                      │
    └──────────────────────────────────────┘
    ◄────────── ~68.6mm total ────────────►
```

- **Hole diameter:** 3.13mm (accepts M3 screws with ~0.13mm clearance)
- **Center-to-center:** 49.45mm (one axis)
- **Hole positions:** On the bracket ears, outside the 62.6mm pump head body, symmetric about center
- **Ear overhang per side:** (68.6 - 62.6) / 2 ≈ 3mm — holes are roughly centered on each 3mm ear
- **Second axis spacing:** TBD — not measured in these photos. The bracket may have only 2 holes total (1 per ear), or 4 holes in a 2x2 pattern.

## Clearance Zones for 3D Modeling

When designing a pump tray or cartridge that holds this pump:

1. **Pump head envelope:** 62.6mm x 62.6mm square (rounded corners), ~48mm deep
2. **Bracket clearance:** Bracket ears extend to 68.6mm total width. Mount with M3 screws at 49.45mm center-to-center.
3. **Motor protrusion behind bracket:** ~63–68mm cylindrical body (~35mm diameter) with a 5mm nub at the very end. Total behind bracket: ~68mm.
4. **Tube exit clearance in front:** ~30–50mm of BPT tube stubs protrude forward.
5. **Wiring clearance at rear:** Motor terminals at the back need ~5mm for solder connections.
6. **Total depth budget:** 116mm from front face to motor nub tip, plus tube stubs in front.
7. **Motor nub:** 5mm protrusion at center of motor end — must not bottom out against any tray surface.

## Remaining Unknowns

1. **Motor body diameter:** ~35mm but uncertain. Low-confidence readings from photos 15, 16.
2. **Second axis mounting hole spacing:** If the bracket has a 2x2 pattern, the perpendicular spacing is not measured.
3. **Tube connector exit positions:** Exact X/Z positions of the two tube stubs on the front face.
4. **Bracket-to-pump-head attachment:** Whether the bracket can be separated from the pump head (for measurement) or is permanently fixed.
