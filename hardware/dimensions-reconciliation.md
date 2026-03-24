# Dimensions Reconciliation — Cross-Document Conflict Resolution

This document identifies and resolves every dimensional contradiction found across the hardware research documents. It produces a single authoritative dimension table for enclosure layout and cartridge design.

Documents reviewed:
- `enclosure/research/layout-spatial-planning.md` (most recent rewrite, commit c274a44)
- `cartridge/planning/research/cartridge-envelope.md` (rewrite, commit cb75274)
- `cartridge/planning/research/dock-mounting-strategies.md` (rewrite, commit fcf3a63)
- `cartridge/planning/research/mating-face.md` (rewrite, commit a190850)
- `cartridge/planning/research/pump-mounting.md`
- `enclosure/research/hopper-and-bag-management.md`
- `cartridge/planning/requirements.md`

---

## 1. Contradictions Found and Resolved

### 1a. Enclosure Outer Dimensions

| Document | Stated Dimensions (W x D x H mm) |
|---|---|
| **layout-spatial-planning.md** (latest) | **280 x 250 x 400** ("front-loading tower") |
| dock-mounting-strategies.md | 250 x 200 x 450 ("tall tower") |
| cartridge-envelope.md (Section 3a) | Lists three layouts: Tall Tower 250x200x450, Cube 300x300x300, Front-Loading Tower 280x250x400 |

**What happened:** The design evolved through multiple layout candidates. The earliest documents explored a "Tall Tower" (250x200x450). The most recent rewrite of layout-spatial-planning.md (commit c274a44, the latest commit in the repo) explicitly selects the Front-Loading Tower at 280x250x400 as the definitive target. dock-mounting-strategies.md was rewritten earlier (commit fcf3a63) and still references the Tall Tower. cartridge-envelope.md lists all three candidates but uses the Tall Tower as the constraining case for fit checks.

**Resolution: 280W x 250D x 400H mm.**

layout-spatial-planning.md is the most recent document and explicitly declares this as the target. It was written after the other documents and integrates their findings. The rationale is sound: 280mm width accommodates dual hopper funnels and a wider cartridge slot, 250mm depth provides better tube routing behind the dock, and 400mm height is sufficient for the vertical stack while being shorter than the 450mm Tall Tower. dock-mounting-strategies.md and cartridge-envelope.md need updating to reflect this.

### 1b. Cartridge Envelope

| Document | Stated Dimensions (W x H x D mm) |
|---|---|
| **cartridge-envelope.md** (Section 6b) | **150 x 80 x 130** |
| layout-spatial-planning.md (Section 3c) | 140 x 90 x 100 |
| dock-mounting-strategies.md (Section 1) | 140 x 90 x 100 (citing "cartridge-envelope.md") |
| mating-face.md (established parameters) | 140 x 90 x 100 (citing "cartridge-envelope.md") |

**What happened:** cartridge-envelope.md was rewritten (commit cb75274) with a rigorous bottom-up dimension buildup from verified Kamoer KPHM400 pump dimensions (115.6 x 68.6 x 62.7 mm). The calculated envelope came to 148 x 79 x 127 mm, rounded to 150 x 80 x 130 mm. The older 140x90x100 values predate the detailed pump arrangement analysis and do not account for side-by-side pump width (137.2mm + 5mm gap + 6mm walls = 148mm) or the full pump depth (115.6mm + cam housing + walls = 127mm).

dock-mounting-strategies.md and mating-face.md cite "cartridge-envelope.md" but carry the pre-rewrite values. They were not updated after the envelope document was rewritten.

**Resolution: 150W x 80H x 130D mm.**

The cartridge-envelope.md values are physically grounded in verified pump dimensions with explicit buildup math. The 140x90x100 values are estimates that predate the pump arrangement analysis. Specific findings:

- **Width 150 vs 140:** Two pumps side-by-side are 137.2mm wide. Add 5mm center gap and 6mm walls = 148mm. 140mm does not fit.
- **Height 80 vs 90:** Two pumps are 62.7mm tall. Add 10mm tube routing above and 6mm walls = 78.7mm, rounded to 80mm. The 90mm value appears to be an early rough estimate with excess margin.
- **Depth 130 vs 100:** Each pump is 115.6mm deep. Add 5mm cam housing at front and 6mm walls = 126.6mm. 100mm is physically impossible -- the pump alone is 115.6mm.

### 1c. Cartridge Weight

| Document | Stated Weight |
|---|---|
| **cartridge-envelope.md** (Section 12) | **~820g** (detailed component-by-component buildup) |
| dock-mounting-strategies.md (Section 1) | ~940g |
| layout-spatial-planning.md (Section 3c) | ~940g (citing "cartridge-envelope.md") |

**What happened:** cartridge-envelope.md provides a line-item weight buildup totaling ~820g (Section 12), and the target envelope summary (Section 6b) rounds to ~810g. The 940g figure in other documents is from a pre-rewrite estimate. The rewritten envelope doc's buildup is:
- 2x pumps at 306g = 612g
- Tubing, fittings, housing, hardware = ~208g
- Total ~820g

The dock-mounting-strategies.md quotes 940g and uses it for structural analysis (cantilever calculations). The 120g discrepancy likely came from an earlier estimate that assumed heavier housing or additional components.

**Resolution: ~820g.**

The component-by-component buildup in the rewritten cartridge-envelope.md is the most physically grounded value. The 940g figure should be treated as a conservative upper bound for structural calculations but is not the expected weight.

### 1d. Bag Zone Height vs Platypus Bag Dimensions

This is the most significant contradiction. Multiple documents give conflicting information:

| Document | Bag Zone Height Allocation | Platypus Bag Height |
|---|---|---|
| layout-spatial-planning.md (Section 3d) | ~100mm (0-100mm from floor) | 250-300mm (Section 2c) |
| dock-mounting-strategies.md | ~250-300mm (Zone C) | 250-300mm (citing 10-12") |
| hopper-and-bag-management.md (Section 5b) | Not specified as a zone | 350mm (13.8") when full |

**The core problem:** layout-spatial-planning.md allocates only 100mm of vertical space for the bag zone (floor to ~100mm), but bags are 250-350mm tall. 100mm cannot contain a bag that is 250-350mm tall in any orientation.

**What is actually happening:** layout-spatial-planning.md is **not claiming the bags are only 100mm tall**. It allocates the bag zone from 0 to ~100mm height, with the dock zone from ~100 to ~310mm. But the bags are laid flat on tilted cradles, not hanging vertically. The document says: "Two Platypus bags sit on tilted cradles (5-10 degree incline toward the connector end) so liquid drains completely to the outlet." In this configuration, the "height" consumed is the bag's thickness when full (~60-80mm), not its 250-350mm length. The bag's length extends along the depth axis (250mm available).

However, this contradicts dock-mounting-strategies.md, which describes bags "hanging vertically" in a 250-300mm tall Zone C at the bottom of the enclosure (0-170mm from floor). The hopper-and-bag-management.md document also analyzes bags hanging vertically with connectors at the bottom and recommends this as the best orientation.

**Resolution: The bag zone allocation of ~100mm in the layout doc is for bags laying flat on cradles, not hanging vertically.**

This is the only way the 280x250x400mm enclosure works with the stated zone heights (0-100 bags, 100-310 dock+valves, 310-400 electronics). The bags must lay flat (or nearly flat) with their 250-300mm length running along the depth axis (250mm available) and their 190mm width fitting within the 280mm enclosure width.

However, this conflicts with the recommendation in hopper-and-bag-management.md to hang bags vertically with connectors at the bottom. Vertical hanging is strongly preferred for reliable drainage and collapse behavior. A vertically hung 250-300mm bag does NOT fit in a 100mm tall zone.

**There are two possible interpretations:**

**Interpretation A (flat bags, matches layout-spatial-planning.md):** Bags lay flat on tilted cradles in a 100mm zone. The 250mm depth is sufficient for the bag length. Drainage relies on the tilt angle and connector position at the lowest point. This is a departure from the vertical hanging recommended by hopper-and-bag-management.md and all the prior art analysis (IV bags, bag-in-box). Controlled collapse is harder with flat bags.

**Interpretation B (vertical bags, matches dock-mounting-strategies.md and hopper doc):** Bags hang vertically, requiring ~250-300mm of vertical space. The dock zone must be raised, and the overall zone heights need revision. This would require either a taller enclosure or thinner zones above.

**Revised vertical layout for Interpretation B (vertical bags) in a 400mm enclosure:**

| Zone | Height Range | Allocation | Contents |
|---|---|---|---|
| Drip tray | 0-15mm | 15mm | Integral to floor |
| Bag zone | 15-275mm | ~260mm | Two bags hanging vertically (250mm realistic bag height, 10mm hook hardware) |
| Dock shelf | 275-280mm | ~5mm shelf floor | Structural horizontal member |
| Dock + valves | 280-370mm | ~90mm | Cartridge (80mm H), solenoid valves, tube routing |
| Drip shelf | 370-375mm | ~5mm | Barrier between wet and dry zones |
| Electronics | 375-400mm | ~25mm | ESP32, L298N drivers, RTC, wiring |

This is extremely tight. The electronics zone shrinks to ~25mm, which is insufficient for L298N boards (27mm tall each). The dock+valves zone at 90mm barely fits the cartridge (80mm) with no room for valves below.

**Revised vertical layout for Interpretation B in a 450mm enclosure (Tall Tower):**

| Zone | Height Range | Allocation | Contents |
|---|---|---|---|
| Drip tray | 0-15mm | 15mm | Integral to floor |
| Bag zone | 15-275mm | ~260mm | Two bags hanging vertically |
| Dock shelf | 275-280mm | 5mm | Structural shelf |
| Valves below dock | 280-340mm | ~60mm | Solenoid valves, tees, flow meter |
| Cartridge slot | 340-420mm | ~80mm | Cartridge at 80mm H |
| Lever clearance | 420-440mm | ~20mm | Lever swing above cartridge |
| Electronics | 350-450mm | ~100mm | ESP32, L298N x3, RTC (overlaps with dock/valve zone in depth, not height) |

This works better but reverts to the 450mm Tall Tower that layout-spatial-planning.md explicitly moved away from.

**Recommended resolution:** This is the single biggest open question in the design. The choice is between:
1. **Flat bags in the 280x250x400 enclosure** -- simpler enclosure, worse bag drainage behavior
2. **Vertical bags in a 250x200x450 enclosure** -- better bag behavior, more complex enclosure, smaller footprint

This requires a physical test: fill a Platypus 2L bag, lay it flat on a tilted surface (5-10 degrees), connect tubing at the low point, and run the pump. If it drains reliably without sputtering, flat bags work and the 400mm enclosure is viable. If it sputters in the last 20-30%, vertical bags are required and the enclosure height must increase.

**For this document, both options are recorded. layout-spatial-planning.md's 100mm bag zone is only valid for flat/cradle-mounted bags.**

### 1e. Height Constraint: 80mm Cartridge in 85mm Slot

cartridge-envelope.md identifies that the Tall Tower layout provides ~85mm of available height in the cartridge slot, yielding only 5mm of margin for an 80mm cartridge. This margin must accommodate:

- Guide rail clearance: 0.3-0.5mm per side = 0.6-1.0mm total
- FDM print tolerance on cartridge: +/- 0.3mm
- FDM print tolerance on dock slot: +/- 0.3mm
- Remaining clearance: 5.0 - 1.0 - 0.6 = ~3.4mm minimum

**Resolution: 5mm margin is adequate but tight.**

The 3.4mm of remaining clearance after tolerances is sufficient for insertion/removal but leaves no room for design error. The 280x250x400 layout document allocates a 210mm tall dock+valves zone (100-310mm), which gives considerably more height flexibility -- the cartridge slot height is no longer constrained to 85mm. In the 400mm layout, the cartridge slot height within the dock zone can be 90-100mm, providing 10-20mm of margin. This is comfortable for FDM.

---

## 2. Authoritative Dimension Table

### 2a. Enclosure

| Parameter | Value | Notes |
|---|---|---|
| **Outer dimensions (W x D x H)** | **280 x 250 x 400 mm** | Front-loading tower layout |
| Wall thickness | 4mm | All sides |
| Internal dimensions (W x D x H) | 272 x 242 x 392 mm | Outer minus 2x wall thickness |
| Footprint | 700 cm^2 | Smaller than a cereal box laid flat |
| Estimated weight (loaded, 2 bags full) | ~8-9 kg | |

### 2b. Zone Heights (floor = 0mm, FLAT BAG configuration)

| Zone | Floor (mm) | Ceiling (mm) | Height (mm) | Contents |
|---|---|---|---|---|
| Drip tray | 0 | 15 | 15 | Integral to enclosure floor |
| Bag zone | 15 | ~100 | ~85 | Two bags on tilted cradles (5-10 deg), bag thickness ~60-80mm when full |
| Dock + valves | ~100 | ~310 | ~210 | Cartridge dock, 4x solenoid valves, tees, flow meter, needle valve, tube routing |
| Drip shelf | ~310 | ~315 | ~5 | Solid horizontal barrier (water/electronics separation) |
| Electronics | ~315 | ~396 | ~81 | ESP32, 3x L298N, RTC, MCP23017, fuse block, DIN rail |
| Hopper | ~396 | 400+ | Extends above | Two funnels at front-top corner, caps when not in use |

### 2c. Cartridge

| Parameter | Value | Source |
|---|---|---|
| **Envelope (W x H x D)** | **150 x 80 x 130 mm** | cartridge-envelope.md buildup from pump dims |
| Volume | ~1.56 L | |
| Weight | ~820g (~1.8 lbs) | Component-by-component buildup |
| Pump (Kamoer KPHM400) | 115.6 x 68.6 x 62.7 mm, 306g each | Verified from Kamoer specs |
| Pump arrangement | Side-by-side, motors same direction | Only arrangement that fits height |
| Housing construction | Tray + shell + lid (3-piece PETG) | pump-mounting.md recommendation |
| Housing wall thickness | 3mm per side | |

### 2d. Cartridge Slot and Dock

| Parameter | Value | Source |
|---|---|---|
| Slot opening (with chamfer) | 155W x 105H mm | layout-spatial-planning.md |
| Slot chamfer | 5mm at 45 deg, all edges | Funnel for sloppy insertion |
| Cartridge-in-slot clearance (height) | 10-20mm above 80mm cartridge | 400mm layout provides more room than Tall Tower's 85mm |
| Guide rail clearance | 0.3-0.5mm per side (PETG) | guide-alignment.md |
| Dock shelf width | ~264mm (272mm interior minus ~4mm clearance/side) | |
| Dock shelf depth | ~234mm (242mm interior minus clearance) | |
| Dock shelf thickness (floor) | 6mm | Structural minimum for PETG span |
| Dock fitting wall height | ~100mm | Houses JG fittings + pogo pins + guide rail attachment |
| Dock fitting wall thickness | 6mm | Bulkhead fitting nut clamping surface |

### 2e. Mating Face and Connections

| Parameter | Value | Source |
|---|---|---|
| Tube port layout | 2x2 grid, 15mm center-to-center | mating-face.md |
| Port zone footprint | 33.5 x 33.5 mm | |
| Tube stub OD | 6.35mm (1/4") hard nylon/PE | |
| Tube stub total length | ~29-30mm from inside wall | mating-face.md |
| John Guest fitting body OD | ~12.7mm | collet-release.md |
| Release plate | 6mm thick, 3mm travel | |
| Alignment pins (dock side) | 2-4 tapered, 15-20 deg, 8-10mm base | guide-alignment.md |
| Electrical pads (cartridge top) | 3x flat pads, 10 x 5mm, 10mm C-C | electrical-mating.md |
| Pogo pins (dock ceiling) | 3x spring-loaded | |

### 2f. Depth Budget (within dock zone)

| Zone | Depth (mm) | Contents |
|---|---|---|
| Cartridge body | 130 | Pumps, tubing, release plate, cam housing |
| Dock back wall + fittings | ~35 | 5mm wall, ~25mm JG fitting depth, ~5mm alignment pin protrusion |
| Tube routing behind dock | ~22-27 | Silicone tubing from JG fittings to vertical runs |
| **Total** | **~187-192** | Fits within 242mm interior depth with ~50mm to spare |

### 2g. Bag Dimensions (Platypus 2L)

| Parameter | Value | Source |
|---|---|---|
| Height (when full) | 350mm (13.8") | hopper-and-bag-management.md |
| Width | 190mm (7.5") | hopper-and-bag-management.md |
| Thickness (when full) | 60-80mm (2.5-3") | hopper-and-bag-management.md |
| Realistic bag height (corrected) | 250-300mm (10-12") | layout-spatial-planning.md Section 2c |
| Weight empty | 37g | |
| Weight full (water) | ~2037g | |
| Connector | 28mm standard bottle thread, single opening | |

Note: layout-spatial-planning.md explicitly corrects the bag height to 250-300mm ("Platypus bags are realistically 10-12 inches tall, not 14+"). The 350mm figure from hopper-and-bag-management.md may include the connector/cap hardware or represent a fully stretched bag. This discrepancy needs physical measurement.

---

## 3. Documents Needing Updates

The following documents carry stale dimensions and should be updated to match this reconciliation:

| Document | Stale Values | Correct Values |
|---|---|---|
| dock-mounting-strategies.md | Enclosure 250x200x450, cartridge 140x90x100, weight 940g | 280x250x400, 150x80x130, ~820g |
| mating-face.md | Cartridge envelope 140x90x100 | 150x80x130 |
| layout-spatial-planning.md | Cartridge 140x90x100, weight 940g | 150x80x130, ~820g |

---

## 4. Remaining Questions Requiring Physical Measurement

1. **Platypus bag actual dimensions**: Measure a filled 2L Platypus bag with calipers. Is it 250mm, 300mm, or 350mm tall? Width and thickness when full? This determines whether flat-cradle bags work in the 100mm zone.

2. **Flat bag drainage test**: Lay a filled Platypus bag on a 5-10 degree tilted surface with the connector at the low end. Pump it dry. Does the pump sputter in the last 20-30%? This determines whether the 400mm enclosure with flat bags is viable, or whether vertical hanging (requiring a taller enclosure) is needed.

3. **Kamoer KPHM400 mounting hole pattern**: Measure center-to-center distances on the actual bracket. The cartridge tray design depends on this.

4. **Kamoer KPHM400 full envelope with barbs**: Measure the pump with barbs attached. The 62.7mm height does not include the barb protrusion (~15-20mm above the pump body). These barbs extend into the tube routing zone, which is budgeted at 10mm. If barbs protrude more than 10mm above the pump body top surface, the cartridge height increases.

5. **John Guest fitting body clearance at 15mm C-C**: Test whether the specific fittings in hand can mount at 15mm center-to-center. If hex flats or molding flash prevent this, the spacing must increase to 18mm C-C.

6. **Cartridge slot height in 280x250x400 layout**: layout-spatial-planning.md allocates a 210mm dock+valves zone but does not specify the exact cartridge slot height within that zone. The slot should be 90-100mm tall (80mm cartridge + 10-20mm clearance). This needs to be explicitly dimensioned in the layout document.

7. **Enclosure height decision**: If flat bags fail the drainage test, the enclosure must grow taller to accommodate vertical bags (~260mm bag zone). A revised height of ~450-480mm may be needed. This is the single most impactful open question.
