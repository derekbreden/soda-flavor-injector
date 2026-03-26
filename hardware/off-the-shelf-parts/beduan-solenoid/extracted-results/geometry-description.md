# Beduan Solenoid Valve (2-Way NC, 1/4" QC) — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the Beduan 12V DC normally-closed solenoid valve (Amazon B07NWCQJK9) in enough detail that an agent generating engineering drawings or designing a valve rack can model every surface, mounting zone, and interface — without holding the part.

## Overall Form

The valve is an **L-shaped assembly** composed of two distinct sub-bodies joined at a right angle:

1. **Valve body (fluid section):** White plastic housing containing the diaphragm/poppet mechanism. Has two tube ports (inlet and outlet) with built-in push-to-connect (quick-connect) fittings for 1/4" OD tubing.
2. **Solenoid coil (electrical section):** Metal-cased electromagnetic coil mounted perpendicular to the valve body. Has two blade-style electrical terminals protruding from the top.

The L-shape means the valve has a **compact footprint when viewed from above** but extends in two perpendicular directions. The tube ports exit from the valve body (horizontal when mounted), while the solenoid coil rises vertically above.

**Important:** This is NOT a cylindrical valve — the previous parts.md estimated it as a simple rectangular body. The actual form is more complex.

## Axis and Orientation Convention

Oriented for typical rack mounting (solenoid coil on top, tube ports horizontal):

- **X (width):** Across the valve body, perpendicular to tube flow direction
- **Y (depth):** Along the tube flow direction (port-to-port axis)
- **Z (height):** Vertical — from bottom of valve body up through solenoid coil to electrical terminals

## Physical Description by Region

### Region 1: Valve Body (White Plastic)
- **Form:** Roughly rectangular block with two cylindrical port stubs
- **Body width (X):** ~32–37mm (caliper readings vary: 32.14mm in photo 03, 37.11mm in photo 04 — likely measuring at different sections or including/excluding port stubs)
- The valve body has a **circular diaphragm chamber** visible from the top (photo 04 shows this clearly — a raised circular boss on the top face of the white body where the diaphragm sits)
- **Port stubs:** Two white cylindrical QC fittings, one on each end along the Y axis, accepting 1/4" OD (6.35mm) tubing
- Chinese product label on one face: "DC12V 0.02-0.8MPa" and manufacturing info

### Region 2: Solenoid Coil (Metal Case)
- **Form:** Small rectangular metal housing containing the electromagnetic coil
- Mounted on top of the valve body, perpendicular to it (creating the L-shape)
- **Coil housing length (along Y):** ~35.63mm (caliper-verified, photo 05)
- The coil housing is smaller than the valve body — it doesn't extend the full width
- **Two blade-type electrical terminals** protrude from the top of the coil housing
- Terminal spacing and orientation: need to be measured for wiring harness design

### Region 3: Overall Assembly Envelope
- **Full height (Z):** ~56.00mm (caliper-verified, photo 01) — from bottom of valve body to top of solenoid coil/terminals
- **Full depth (Y):** ~50.84mm (caliper-verified, photo 02) — from one port stub end to the other, or from port to rear of solenoid
- The L-shape means the overall bounding box is NOT fully occupied — there's significant empty space in the "corner" of the L

## Caliper Measurements Summary

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 56.00mm | Full height (bottom of valve body to top of solenoid coil) | HIGH |
| 02 | 50.84mm | Full depth (port-to-port or port-to-coil-rear) | MEDIUM — display slightly angled |
| 03 | 32.14mm | Valve body width at narrowest section | MEDIUM — display upside-down |
| 04 | 37.11mm | Valve body width at wider section (possibly including port stubs) | MEDIUM — display somewhat angled |
| 05 | 35.63mm | Solenoid coil housing length | HIGH — display clear (upside-down but legible) |

## Comparison with Previous Estimates

The enclosure `parts.md` listed:
- **34.0W x 63.5D (port-to-port) x 58.4H mm**

| Dimension | Estimated | Caliper | Notes |
|-----------|-----------|---------|-------|
| Width (X) | 34.0mm | 32–37mm | Range depends on where measured; 32mm at body, 37mm at wider section |
| Depth (Y, port-to-port) | 63.5mm | ~50.84mm | **Significantly shorter** — the 63.5mm estimate may have included QC fitting protrusion or was simply wrong |
| Height (Z) | 58.4mm | 56.00mm | Close — slight overestimate in original |

**Critical finding:** The depth (port-to-port) dimension is substantially smaller than estimated. If confirmed, this means:
- The valve rack can be shallower (less Y-axis depth consumed)
- Internal tube routing has more room behind the rack
- The rack frame design in `parts.md` (spec'd at 64mm depth) can potentially shrink to ~52–55mm

## Mounting Considerations for Valve Rack Design

The valve has **no built-in mounting features** — no tabs, flanges, ears, or screw holes. It must be held by a designed cradle/clamp in the valve rack.

### Cradle Design Guidance

1. **L-shape complicates simple saddles:** The valve isn't a simple rectangular block. The cradle must support the valve body (horizontal) while accommodating the solenoid coil (vertical protrusion).
2. **Suggested approach:** A pocket/channel that the valve body drops into from above, with the solenoid coil sticking up through an opening or slot. Snap-over retention clips on the valve body sides.
3. **Valve body is the primary mounting surface:** The flat faces of the white plastic body are the best surfaces for clamp contact. The solenoid coil should not bear mounting loads.
4. **Port access:** Both tube ports must be accessible from the Y direction for push-to-connect tubing insertion/removal.
5. **Electrical access:** The blade terminals on top of the solenoid must be accessible for spade connectors or wire soldering.

### Rack Packing

- **Width pitch:** With body width of ~32–37mm, a 38mm center-to-center pitch (current spec) may be tight if the 37mm dimension is the controlling width. Verify whether 37mm includes port stubs.
- **Height pitch:** At 56mm per valve, two rows with a 4mm gap = 116mm total rack height (vs. the current 120mm spec — close agreement).
- **Depth:** At ~51mm port-to-port, the rack depth can be ~55mm (with margin) instead of the currently spec'd 64mm.

## Questions for User Verification

1. **Photo 03 vs 04 (32mm vs 37mm):** These measure the valve body width but at different sections. Can you confirm which is the maximum width (controlling dimension for rack slot width)?
2. **Port-to-port depth:** Photo 02 shows ~50.84mm. Is this measuring from tube stub tip to tube stub tip? Or from the valve body face (excluding stub protrusion)? If stubs protrude beyond the body, the body-only depth is even smaller.
3. **Are the tube ports coaxial (inline)?** Or are they offset — inlet on one face, outlet on an adjacent face? The L-shape and photo angles make this hard to determine.
4. **Only 5 photos for this part** — would additional measurements be helpful? Specifically:
   - Port stub OD and length
   - Solenoid coil housing width and height
   - Terminal spacing
   - Exact diaphragm boss diameter on top face
   - Distance from bottom of valve body to port center axis
