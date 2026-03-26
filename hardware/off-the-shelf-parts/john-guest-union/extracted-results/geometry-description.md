# John Guest PP0408W 1/4" Union — Caliper-Verified Geometry Description

## Purpose of This Document

This document describes the physical geometry of the John Guest PP0408W push-to-connect union fitting in enough detail that an agent generating engineering drawings or 3D-printable mounts can precisely model every surface, shoulder, and interface zone — without holding the part.

## Overall Form

The fitting is a **symmetric inline union**: a single white acetal copolymer body with identical tube-accepting ports on each end. It connects two pieces of 1/4" OD (6.35mm) tubing end-to-end through a straight-through internal passage. The fitting is rotationally symmetric about its long axis (axis of tube flow).

When viewed from the side, the profile is **not a simple cylinder** — it has distinct diameter transitions along its length, creating a barbell-like silhouette with wider collet ends and a narrower waist.

## Axis and Orientation Convention

- **Long axis (L):** The axis of tube flow, running through the center of both ports. All "length" dimensions are along this axis.
- **Radial (R):** Perpendicular to L, in any direction. The fitting is axially symmetric, so all cross-sections perpendicular to L are circles.
- **"Collet end":** Either end of the fitting where a tube inserts. Both ends are identical.
- **"Body center":** The waist region between the two collet zones.

## Dimensional Profile Along the Long Axis

The fitting has **5 distinct zones** progressing from one tube port to the other. Because the fitting is symmetric, the profile mirrors at the center. Described from left port inward:

### Zone 1: Collet Ring (each end)
- **OD: 14.96–15.10mm** (caliper-verified from photos 01–03)
- This is the outermost visible ring at the tube entry. It houses the internal collet mechanism (spring steel gripper teeth + release sleeve).
- The collet ring has a **slight lip/shoulder** where it transitions to the body.
- **Blue variant:** Some PP0408W units include a blue plastic collet ring (visible in photo 08). This is a cosmetic/branding difference — same mechanism.
- The collet ring can be **pressed inward** (toward body center) to release the gripper teeth, allowing tube removal. This is the release action that the release plate exploits.

### Zone 2: Body Shoulder / Transition
- Short transition zone where the OD steps down from the collet ring diameter (~15mm) to the central body diameter.
- This shoulder is the surface the release plate's stepped bore engages against.

### Zone 3: Central Body (waist)
- The narrowest section, located at the midpoint of the fitting.
- This is the zone that press-fits into the cartridge rear wall pocket.
- **OD at this zone needs clarification** — the 15.10mm reading (photo 01) may be measuring across the collet ring, not the true central body. The parts.md estimated 12.7mm body OD (1/2" nominal). The caliper photos show the jaws clamping at different axial positions, and some readings differ.
- **User verification needed:** Is the 15.10mm reading (photo 01) measuring the collet ring OD, or is there a narrower central body region? If the central body is narrower, what is its OD?

### Zone 4 & 5: Mirror of Zones 2 & 1
Identical collet ring and shoulder on the opposite end.

## Caliper Measurements Summary

| Photo | Reading | What's Being Measured | Confidence |
|-------|---------|----------------------|------------|
| 01 | 15.10mm | Body or collet ring OD, side view, calipers across widest point near one end | HIGH — display clear |
| 02 | 14.96mm | End-on view looking into tube port, calipers across the collet ring OD | HIGH — display clear |
| 03 | 14.96mm | Same as 02, different camera angle | HIGH — confirms 02 |
| 04 | 9.57mm | Tube stub section OD — the smooth cylindrical zone where 1/4" tube would grip | HIGH — display clear |
| 05 | 9.57mm | Same measurement as 04, different angle | HIGH — confirms 04 |
| 06 | 9.31mm | Narrower feature — possibly the internal bore/passage ID, or a recessed inner surface | HIGH — display clear |
| 07 | 39.13mm | Overall length, port-to-port, white body only (no blue collet visible) | HIGH — display clear |
| 08 | 41.80mm | Overall length with blue collet ring visible on one end | HIGH — display clear |

## Key Corrections to Previous Estimates

The cartridge `parts.md` listed:
- Body OD: 12.7mm → **Actual: 14.96–15.10mm** (significantly larger — the 12.7mm was likely a nominal spec, not the actual OD)
- Overall length: 38.1mm → **Actual: 39.13mm** (white body only) or **41.80mm** (with blue collet ring extended)
- Collet ring OD: ~12.7mm → **Actual: 14.96–15.10mm** (same as body OD at the collet zone)

**Critical impact on cartridge design:**
- The rear wall pocket bore was spec'd at 13.0mm to clear a 12.7mm body. If the actual body/collet is ~15mm, the pocket bore must be **at least 15.5mm** for clearance.
- The release plate stepped bores (currently 10.5mm inner lip / 12.5mm outer bore) need to be reconsidered against the actual collet ring geometry.

## 9.57mm Feature — What Is It?

Photos 04 and 05 show the calipers measuring a ~9.57mm feature. Looking at the photos, this appears to be:
- The **tube stub** or **smooth cylindrical zone** at the very end of the fitting where the tube inserts. This is the portion just inside the collet ring opening, before the gripper teeth engage.
- This dimension is relevant for understanding the collet opening diameter — a 6.35mm tube must pass through this zone.

## 9.31mm Feature — What Is It?

Photo 06 shows a 9.31mm measurement. This could be:
- The **internal passage diameter** at the tube stop (the bore through the center of the fitting)
- Or a slightly different axial position on the same tube-entry zone as the 9.57mm reading

## Length Discrepancy: 39.13mm vs 41.80mm

The 2.67mm difference between photos 07 and 08 likely represents:
- **Collet protrusion:** The blue collet ring in photo 08 is in its extended (default) position, protruding ~2.67mm beyond the white body face. When pressed in for tube release, it retracts flush.
- This 2.67mm may be the **collet travel distance** — the axial distance the release plate must push the collet to release the tube grip.
- Alternatively, these may be two slightly different fitting variants. User should verify whether both measurements are from the same physical fitting.

## Geometry for 3D Modeling Agents

When modeling a pocket, bore, or clamp to hold this fitting:

1. **Press-fit pocket:** Must accommodate ~15mm OD. Use 15.5mm bore for light press or 16mm for sliding fit.
2. **Axial retention:** The shoulder between collet ring and body waist (if there is a diameter step-down) provides a natural shoulder to seat against. If there is no step-down (i.e., the fitting is a uniform ~15mm cylinder), retention must come from pocket depth or clips.
3. **Tube clearance:** Both ends need clear access for 6.35mm OD tubing insertion.
4. **Release mechanism access:** The collet ring must be accessible for inward push (~2.67mm travel) to release tubes. The release plate engages here.
5. **Overall length budget:** Plan for 42mm total length with collet extended, 39mm body-only.

## Questions for User Verification

1. **Is there a diameter step-down between the collet ring and the central body?** Photos 01–03 all measure ~15mm, but they may all be at the collet zone. If the body waist is narrower (e.g., 12–13mm), that changes the pocket bore design significantly.
2. **Are photos 07 and 08 the same physical fitting?** The blue collet suggests photo 08 might be a different fitting or the same fitting with the collet extended.
3. **What is the 9.31mm feature in photo 06?** Is it the internal bore, the collet opening, or something else?
