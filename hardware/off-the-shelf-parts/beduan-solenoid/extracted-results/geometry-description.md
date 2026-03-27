# Beduan Solenoid Valve (2-Way NC, 1/4" QC) — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the Beduan 12V DC normally-closed solenoid valve (Amazon B07NWCQJK9) in enough detail that an agent generating engineering drawings or designing a valve rack can model every surface, mounting zone, and interface — without holding the part.

**Reference photographs:** Caliper photos of the physical valve are in `../raw-images/`. Photo numbers referenced throughout this document (e.g., "photo 03") correspond to numbered files in that directory.

## Overall Form

The valve is a **T-shaped assembly** composed of two distinct sub-bodies:

1. **Valve body (white plastic, fluid section):** Contains the diaphragm/poppet mechanism. Has two tube ports (inlet and outlet) with built-in push-to-connect (quick-connect) fittings for 1/4" OD tubing. This is the horizontal bar of the T.
2. **Solenoid coil (metal-cased, electrical section):** Electromagnetic coil mounted perpendicular to the valve body, rising vertically from the **center** of the white body. This is the vertical stroke of the T. Has two blade-style spade connector terminals protruding from the top, running **parallel** to the tube flow axis.

The T-shape means the valve is **not a simple rectangular block**. The white valve body is horizontal (carrying the tube ports), and the metal solenoid coil housing rises vertically from the center of it. The spade connectors extend along the same axis as the tube ports (Y axis), reaching toward the edge of the white body's depth envelope. From the front (X-Z plane), the overall profile resembles a **lowercase h** — the T-shaped body plus two spade prongs hanging from the top on one side. A Chinese product label on the valve body reads "DC12V 0.02-0.8MPa".

## Axis and Orientation Convention

Oriented for typical rack mounting (solenoid coil on top, tube ports horizontal):

- **X (width):** Across the valve body, perpendicular to tube flow direction
- **Y (depth):** Along the tube flow direction (port-to-port axis)
- **Z (height):** Vertical — from bottom of valve body up through solenoid coil to spade connector tips

## Dimensional Profile

```
FRONT VIEW (X-Z plane, looking along tube flow axis Y):

              ╥ ╥  ← spade connectors (parallel to Y, into page)
        ┌─────╨─╨─────┐
        │    metal     │
        │    coil      │ 36.63mm from bottom of
        │   housing    │   metal body to connector tips
        │  (31.41mm)   │
   ┌────┴──────────────┴────┐
   │    white valve body     │ ← 32.71mm wide (X)
   │     (centered below)    │
   └────────────────────────┘

   ▲ 56.00mm total height (Z)

SIDE VIEW (Y-Z plane, looking along width axis X):

         ┌───────┐
         │ metal │  spade connectors
         │ coil  │  extend this way →  ╤═╤
         │       │                     │ │ (parallel to Y)
    ┌────┴───────┴────┐
    │  white body      │
    ○──────────────────○ ← tube ports (Y axis)

    ◄── 50.84mm depth (Y) ──►
```

### White Valve Body (Fluid Section)
- **Maximum width (X): 32.71mm** (caliper-verified, photo 03)
- **Depth (Y, port-to-port): ~50.84mm** (caliper-verified, photo 02)
- The top face of the white body has a **raised circular diaphragm boss** visible from above (photo 04) — this is where the diaphragm chamber sits internally
- Two cylindrical QC port stubs extend from opposite ends along the Y axis
- **No built-in mounting features** — no tabs, flanges, ears, or screw holes on the white body

### Metal Solenoid Coil Housing (Electrical Section)
- Rectangular metal case containing the electromagnetic coil
- Mounted on top of the valve body at its **center** (creating the T-shape)
- **Width of metal body: ~31.41mm** — narrower than the white body (32.71mm), centered on it
- **Height span (metal body bottom to spade connector tips): 36.63mm** (caliper-verified, photos 04/05)
- This 36.63mm is a **sub-section of the total 56.00mm height** — the remaining ~19.37mm is the white valve body below the metal housing
- **Spade connector terminals:** Two blade-type electrical contacts protruding from the top of the metal housing. They extend **parallel to the tube flow axis (Y)**, reaching toward the edge of the white body's depth envelope. These are the highest point of the assembly.

### Overall Assembly Envelope
- **Total height (Z): 56.00mm** exactly (caliper-verified, photo 01) — from bottom of white valve body to tips of spade connectors
- **Total depth (Y): ~50.84mm** (caliper-verified, photo 02) — port-to-port
- **Maximum width (X): 32.71mm** — controlled by the white body (the widest part). The metal body (31.41mm) is narrower and centered.
- **Spade connectors** extend along Y (parallel to tubes), not X, so they don't increase the width envelope.

## Caliper Measurements Summary

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 56.00mm | Total height (Z): bottom of white body to top of spade connectors | HIGH (exact) |
| 02 | 50.84mm | Total depth (Y): port-to-port or port-to-coil-rear | MEDIUM |
| 03 | 32.71mm | White valve body maximum width (X) | HIGH (user-verified) |
| 04 | 36.63mm | Metal body bottom to spade connector tips (Z sub-span) | HIGH (user-verified) |
| 05 | 36.63mm | Same measurement as 04, alternate camera angle | HIGH (user-verified) |

## Additional User-Provided Dimensions

These were provided by the user from direct caliper observation, not extracted from photos:

| Dimension | Value | Description |
|-----------|-------|-------------|
| Metal body grid width | 31.41mm | Width of just the metal solenoid housing (narrower than white body) |
| White body max width | 32.71mm | Maximum width of the white valve body |
| Metal body + spade connectors | 36.63mm | From bottom of metal housing to top of spade connector terminals |
| Offset between bodies | slight | Metal and white bodies are not perfectly centered on each other in X |

## Comparison with Previous Estimates

| Dimension | Previous Estimate | Caliper-Verified | Notes |
|-----------|-------------------|------------------|-------|
| Width (X) | 34.0mm | **32.71mm** (white body) | Slightly narrower than estimated |
| Depth (Y, port-to-port) | 63.5mm | **~50.84mm** | **Significantly shorter** — 12.7mm less than estimated |
| Height (Z) | 58.4mm | **56.00mm** | Close to estimate, slightly shorter |

**Critical finding:** The depth (Y) is ~13mm shorter than estimated. This means:
- The valve rack can be significantly shallower
- Rack depth can shrink from 64mm to ~55mm
- More room behind the rack for tube routing

## Mounting Considerations for Valve Rack Design

### No Built-In Mounting Features
The valve must be held entirely by a designed cradle/clamp. There are no screw holes, tabs, or mounting ears.

### T-Shape Simplifies Cradle Design
The metal solenoid coil is centered on the white body, creating a symmetric T-profile from the front. A cradle must:
1. Support the white valve body from below and on the sides
2. Leave clearance above for the centered solenoid coil to rise vertically
3. Allow snap-over retention clips on the valve body sides
4. Not interfere with tube ports (Y axis access) or spade connectors (top, extending along Y)

### Controlling Dimensions for Rack Slot Sizing
- **Slot width (X):** Must clear 32.71mm white body. Use ~34mm for light clearance.
- **Slot depth (Y):** Must accommodate ~51mm port-to-port span.
- **Slot height (Z):** The white body alone is ~19mm tall (56.00 - 36.63 ≈ 19.37mm). The solenoid coil adds 36.63mm above that.
- **Total valve height for rack row pitch:** 56mm per valve. Two rows with 4mm gap = 116mm.

### Port and Electrical Access
- **Tube ports:** Both ends of the Y axis must be accessible for push-to-connect tubing insertion/removal.
- **Spade connectors:** Top of assembly, must be accessible for spade terminal crimps. Terminal spacing needs measurement for harness design.

## Geometry for 3D Modeling Agents

When modeling a valve rack cradle:

1. **The valve is T-shaped, not rectangular.** The bounding box is ~33mm x ~51mm x 56mm, but the actual solid only occupies a T-shaped portion of that volume (lowercase h from the front if you include spade connectors).
2. **White body is the primary clamping surface.** It's the wider, lower portion — design saddles and clips around its 32.71mm width and ~19mm height.
3. **Solenoid coil protrudes upward from the center** of the white body. The metal body (31.41mm) is narrower than and centered on the white body (32.71mm).
4. **Port stubs protrude along Y.** The QC fittings may add length beyond the 50.84mm measurement.
5. **Spade connectors are the highest point** at 56mm, and extend **parallel to the tube flow axis (Y)**, not perpendicular. Wire routing runs along the same axis as the tubes.
6. **The T-profile is symmetric in X.** Unlike an L-shape, the valve can be cradled with a centered saddle — no need for asymmetric clearance cuts.

## Remaining Unknowns

1. **Tube port stub OD and protrusion length** — not measured. Needed for tube routing clearance.
2. **Port center axis height** — distance from bottom of white body to the center of the tube ports. Needed for aligning tubes.
3. **Solenoid coil housing exact width and depth** — only the combined "grid width" of 31.41mm is known.
4. **Spade connector spacing** — needed for wiring harness design.
5. **Whether the ports are truly inline (coaxial)** — or slightly offset from each other. Ambiguous from photos alone.
6. **White valve body height alone** — derived as ~19.4mm (56.00 - 36.63) but not directly measured.
7. **Diaphragm boss diameter** — visible from top in photo 04 but not measured.
