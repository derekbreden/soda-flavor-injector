# Kamoer KPHM400-SW3B25 Peristaltic Pump — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the Kamoer KPHM400-SW3B25 peristaltic pump in enough detail that an agent generating engineering drawings or designing a 3D-printed pump tray/cartridge can model every mounting surface, clearance zone, and interface — without holding the part.

## Overall Form

The pump is a **two-body assembly**: a black plastic **pump head** housing (roughly square cross-section when viewed from front) permanently attached to a silver cylindrical **DC motor** via a white plastic **adapter plate**. The motor shaft enters the pump head from the rear, driving an internal 3-roller peristaltic mechanism.

When viewed from the front (tube connector face), the pump head appears nearly square with rounded corners. The motor protrudes behind it, creating an elongated overall profile. A **black mounting bracket** (stamped metal plate) is sandwiched between the pump head and motor, extending outward on two sides to provide mounting ears with screw holes.

## Axis and Orientation Convention

- **X (width):** Horizontal when pump is in its normal operating orientation. The mounting bracket ears extend along this axis.
- **Y (depth):** From front face (tube connectors) toward rear (motor terminals). This is the longest dimension.
- **Z (height):** Vertical. The pump is roughly symmetric about the XY midplane.
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
  - Positioned offset from center — one above center, one below (or left/right — orientation depends on mounting)
  - Tube stubs protrude ~30–50mm from the face (flexible BPT tubing attached)

### Section 2: Pump Head Body (Y = 0 to ~48mm)
- Black plastic housing containing the peristaltic roller mechanism
- Cross-section is roughly square with rounded corners, ~62.6mm x ~62.6mm
- Depth of the pump head body: **~47.88–48.88mm** (caliper-verified from photos 05, 14)
- The head body tapers slightly or has features that make different depth measurements possible depending on where exactly the caliper contacts

### Section 3: Mounting Bracket (Y ≈ 48mm)
- Black stamped metal bracket plate
- Sits at the junction between pump head and motor
- **Extends beyond the pump head body on two sides** (the mounting ears)
- Bracket width including ears: **~68.6mm** (matches datasheet — wider than the 62.6mm pump head body by ~3mm per side)
- Mounting holes: **2x or 4x M3 through-holes** in the bracket ears (pattern TBD — not directly measured in these photos)
- The bracket is the primary mechanical interface for mounting the pump

### Section 4: Motor Adapter Plate (Y ≈ 48–52mm)
- White plastic disc/plate between bracket and motor
- Transitions from the square pump head bolt pattern to the round motor housing
- Visible in several side-view photos

### Section 5: DC Motor Body (Y ≈ 52mm to ~115mm)
- Silver cylindrical motor housing
- **Motor diameter: ~35mm** (caliper readings from photos 15, 16 — display was upside down, reading uncertain, possibly 34.54 or 35.13mm)
- Motor length (from adapter plate to terminals): **~63mm** (derived: total length ~115mm minus pump head ~48mm minus adapter ~4mm)
- Motor has a flat on one side (standard 3xx-series DC motor flat for anti-rotation)
- QR code and RoHS label visible on motor housing
- Kamoer product label with model number on motor body
- **Motor terminal end:** Two solder tabs at the very rear of the motor

## Caliper Measurements Summary

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 62.61mm | Pump head width, top-down view | HIGH |
| 02 | 62.51mm | Pump head width, side view | HIGH |
| 03 | 62.61mm | Pump head width, front view with tube exits visible | HIGH |
| 04 | 62.61mm | Pump head width, front face with Kamoer branding | HIGH |
| 05 | 47.88mm | Pump head depth, measured from top-down | HIGH |
| 06 | 37.13mm | Tube connector area width or inter-connector spacing | MEDIUM — display somewhat faint |
| 07 | ??? | Total height, pump standing vertical, display upside-down | LOW — cannot read reliably |
| 08 | 115.48mm | Total length, full side profile, pump + motor | MEDIUM — display upside-down but legible |
| 09 | ??? | Total length from different angle, display upside-down | LOW — cannot read reliably |
| 10 | ~68.74mm | Pump head depth including front cover, side view | MEDIUM — display sideways |
| 11 | 65.15mm | Pump body height or depth measurement | HIGH |
| 12 | ~61.19mm | Pump head height from different angle | MEDIUM |
| 13 | ~51.68mm | Pump head dimension (possibly including part of bracket) | MEDIUM |
| 14 | 48.88mm | Pump head depth, side view with motor visible | HIGH |
| 15 | ~34.54mm | Motor body diameter, display upside-down | LOW — needs user verification |
| 16 | ~35.13mm | Motor body diameter, display upside-down | LOW — needs user verification |
| 17 | 82.82mm | Height with tube connectors or bracket-to-connector span | MEDIUM |

## Cross-Referencing with Datasheet

The Kamoer datasheet lists: **68.6W x 115.6D x 62.7H mm**

| Dimension | Datasheet | Caliper | Notes |
|-----------|-----------|---------|-------|
| Width | 68.6mm | 62.61mm (head), ~68.6 (with bracket ears) | 62.6mm is the pump head body; bracket ears add ~3mm per side to reach 68.6mm |
| Depth | 115.6mm | ~115.48mm | Excellent agreement — total pump+motor length |
| Height | 62.7mm | 62.51–62.61mm | Excellent agreement — pump head is nearly square |

## Tube Connector Geometry

- **Two tube exits** on the front face (inlet and outlet)
- Connectors are white plastic barbed stubs
- BPT pump tubing (4.8mm ID x 8.0mm OD) attaches to these stubs
- **Tube exit positions relative to front face center:** Not precisely measured. Photo 06 suggests the overall connector zone width is ~37mm, but exact center-to-center spacing needs verification.
- **Tube stub protrusion:** ~30–50mm from front face (estimated from photos, not directly measured with calipers)
- Connectors appear to exit at approximately the same Y position (same face) but offset vertically (one inlet, one outlet)

## Mounting Bracket Detail

- Stamped metal (black-coated steel or aluminum)
- The bracket is the **only mounting interface** — the pump head body itself has no mounting features
- Bracket sits between pump head rear face and motor adapter plate
- **Mounting hole pattern:** NOT directly measured in these photos. This remains the critical TBD.
- Bracket ears extend ~3mm beyond pump head body on each side (total bracket width ~68.6mm)
- Bracket thickness: ~1.5–2mm (estimated from photos, not directly measured)
- The bracket has a rectangular profile with the pump head passing through a central opening

## Clearance Zones for 3D Modeling

When designing a pump tray or cartridge that holds this pump:

1. **Pump head envelope:** 62.6mm x 62.6mm square (rounded corners), ~48mm deep
2. **Bracket clearance:** Bracket ears extend to 68.6mm total width — the tray boss pattern must match these mounting holes
3. **Motor protrusion behind bracket:** ~63mm cylindrical, ~35mm diameter. The tray/cartridge must have clearance behind the bracket for the motor body.
4. **Tube exit clearance in front:** ~30–50mm of tube stubs protrude forward from the pump head face. The cartridge must have space for these and their routing to JG fittings.
5. **Wiring clearance at rear:** Motor terminals at the very back need ~5mm clearance for solder connections and wire routing.
6. **Total depth budget:** ~115mm from front face to motor terminals, plus ~30–50mm for tube stubs in front = **~145–165mm total depth footprint**.

## Questions for User Verification

1. **Motor diameter:** Photos 15 and 16 had the display upside-down. Can you confirm the motor body diameter? It looks like ~35mm.
2. **Photos 07, 09:** The caliper display was upside-down and I couldn't read these reliably. What dimensions were you measuring in these shots?
3. **Photo 10 (~68.74mm):** Was this measuring the pump head depth including the front cover/face plate? Or the bracket width?
4. **Photo 06 (37.13mm):** Was this the tube connector center-to-center spacing, or the width of the connector area?
5. **Mounting hole pattern:** Were any photos intended to capture the bracket mounting holes? The hole center-to-center spacing is the single most critical missing measurement for the pump tray design.
6. **Photo 17 (82.82mm):** What was this measuring? It's larger than the pump head but smaller than the total length.
