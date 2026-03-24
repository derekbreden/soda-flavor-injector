# Dimensions Reconciliation — Authoritative Cross-Document Reference

This document is the single authoritative dimension table for enclosure layout and cartridge design. All other hardware documents defer to the values listed here.

Documents reviewed:
- `enclosure/research/layout-spatial-planning.md`
- `enclosure/research/incline-bag-mounting.md`
- `enclosure/research/drip-tray-shelf-analysis.md`
- `enclosure/research/dip-tube-analysis.md`
- `enclosure/research/pump-assisted-filling.md`
- `cartridge/planning/research/cartridge-envelope.md`
- `cartridge/planning/research/dock-mounting-strategies.md`
- `cartridge/planning/research/mating-face.md`
- `cartridge/planning/research/pump-mounting.md`
- `enclosure/research/hopper-and-bag-management.md`
- `cartridge/planning/requirements.md`

---

## 1. Resolved Design Decisions

### 1a. Enclosure Outer Dimensions

**280W x 250D x 400H mm** (front-loading tower layout).

280mm width accommodates dual hopper funnels and a wider cartridge slot. 250mm depth provides tube routing behind the dock. 400mm height is sufficient for the vertical stack. Older documents referencing a "Tall Tower" at 250x200x450 are superseded.

### 1b. Cartridge Envelope

**150W x 80H x 130D mm.**

Derived from verified Kamoer KPHM400 pump dimensions (115.6 x 68.6 x 62.7 mm) with a bottom-up buildup in cartridge-envelope.md:
- **Width 150:** Two pumps side-by-side = 137.2mm + 5mm center gap + 6mm walls = 148mm, rounded to 150mm.
- **Height 80:** Two pumps = 62.7mm + 10mm tube routing + 6mm walls = 78.7mm, rounded to 80mm.
- **Depth 130:** Pump = 115.6mm + 5mm cam housing + 6mm walls = 126.6mm, rounded to 130mm.

Older documents referencing 140x90x100 predate the pump arrangement analysis. The 100mm depth is physically impossible (the pump alone is 115.6mm).

### 1c. Cartridge Weight

**~820g** (~1.8 lbs).

Component-by-component buildup from cartridge-envelope.md:
- 2x pumps at 306g = 612g
- Tubing, fittings, housing, hardware = ~208g

The 940g figure in older documents is a pre-rewrite estimate. Treat 940g as a conservative upper bound for structural calculations.

### 1d. Bag Zone and Mounting Approach

**Bags mount at 18-20 degrees incline (two-point stretch), not vertical hanging or flat cradles.**

The conflict between "100mm flat cradle zone" (layout-spatial-planning.md) and "250-300mm vertical hanging zone" (dock-mounting-strategies.md, hopper-and-bag-management.md) is resolved by the incline mounting approach (incline-bag-mounting.md):

- Each 1L Platypus bag (~250mm long) is stretched between a low mount point (connector at front) and a high mount point (sealed end at rear)
- At 18 degrees: horizontal run = 238mm (fits 242mm interior depth), vertical rise = 77mm per bag
- Two bags stack vertically in the same depth footprint with a thin divider between them
- The dip tube extends from the low connector end upward along the incline, remaining submerged until the bag is nearly empty (last 5-10%)
- Two-point tension constrains collapse to thinning-in-place rather than random folding

Bag zone height requirement for two stacked 1L bags at 18 degrees:
- Bag 1 connector center at ~26mm, top of sealed end at ~122mm
- Bag 2 connector center at ~69mm, top of sealed end at ~165mm
- Total bag zone: ~165-175mm depending on exact angle and clearances

### 1e. Drip Tray Removed

The 15mm drip tray is removed (drip-tray-shelf-analysis.md). The floor is a flat 4mm panel. Rationale:
- No bag swapping (bags are permanent, refilled via hopper)
- No condensation (no refrigeration inside enclosure)
- No open liquid surfaces during operation (fully sealed fluid path)
- The only realistic slow-leak scenario (loose zip tie at bag connector) is better solved with hose clamps
- 15mm recovered for the bag zone

### 1f. Drip Shelf Redesigned as Electronics Shelf

The "drip shelf" at ~306mm is retained as a structural electronics shelf but is no longer a sealed liquid barrier (drip-tray-shelf-analysis.md). It is an open shelf with cutouts for wiring and airflow, providing:
- DIN rail mounting surface
- Structural stiffness (ties side walls together)
- Visual/physical zone separation for assembly

Liquid cannot travel upward from the plumbing zone to the electronics zone under gravity. The "drip" designation is dropped.

### 1g. Back Panel Drip Dam Removed

The drip dam ridge on the back panel is unnecessary. Water fittings are at the bottom, electrical connections at the top. Gravity keeps water away from electronics. The vertical arrangement is sufficient protection.

### 1h. Height Constraint: Cartridge in Dock Slot

The 400mm layout allocates a ~126mm dock+valves zone (180-306mm), providing the 80mm cartridge with ~46mm of clearance for valves, lever swing, and tolerances. This is comfortable for FDM printing. The earlier concern about 5mm margin in the Tall Tower layout no longer applies.

### 1i. Hopper Connection Topology

The hopper connects at the pump outlet side, not at the bag-side tee (pump-assisted-filling.md). During refill, the pump reverses to pull concentrate from the hopper through the pump and into the bag. This corrects the earlier topology assumption in hopper-and-bag-management.md. The change affects plumbing routing but not enclosure dimensions.

### 1j. Dip Tube Creates Sealed Path

The Platypus Drink Tube Kit creates a sealed bag-to-tubing path through the dip tube (dip-tube-analysis.md). The bag is not an "open pouch" — fluid must travel through the dip tube. This validates:
- Bidirectional flow (dispensing and hopper refill both work through the same sealed path)
- Clean cycle fill (house water pressure pushes through the dip tube into the bag)
- The incline mount's drainage reliability (dip tube extends upward into the liquid pool)

---

## 2. Authoritative Dimension Table

### 2a. Enclosure

| Parameter | Value | Notes |
|---|---|---|
| **Outer dimensions (W x D x H)** | **280 x 250 x 400 mm** | Front-loading tower layout |
| Wall thickness | 4mm | All sides |
| Internal dimensions (W x D x H) | 272 x 242 x 392 mm | Outer minus 2x wall thickness |
| Footprint | 700 cm^2 | Smaller than a cereal box laid flat |
| Estimated weight (loaded, 2 bags full) | ~6-7 kg | Two 1L bags instead of 2L |

### 2b. Zone Heights (floor = 0mm, INCLINE MOUNT configuration)

| Zone | Floor (mm) | Ceiling (mm) | Height (mm) | Contents |
|---|---|---|---|---|
| Floor panel | 0 | 4 | 4 | Flat 4mm panel (no drip tray) |
| Bag zone | 4 | ~180 | ~176 | Two 1L bags on incline mounts (18-20 deg), stacked vertically |
| Dock shelf | ~180 | ~186 | ~6 | Structural horizontal member |
| Dock + valves | ~186 | ~306 | ~120 | Cartridge dock (80mm H), solenoid valves, tees, flow meter, needle valve, tube routing |
| Electronics shelf | ~306 | ~310 | ~4 | Open structural shelf with cutouts for wiring and airflow |
| Electronics | ~310 | ~396 | ~86 | ESP32, 3x L298N, RTC, MCP23017, fuse block, DIN rail |
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
| Cartridge-in-slot clearance (height) | 10-20mm above 80mm cartridge | 400mm layout provides comfortable margin |
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

### 2g. Bag Dimensions

**Primary: Platypus 1L bags** (active design, locked by 400mm enclosure height decision)

| Parameter | Value | Notes |
|---|---|---|
| Length | ~250mm | Connector end to sealed end |
| Width | ~140mm | Across the bag face |
| Thickness (when full) | ~40mm | |
| Connector | 28mm standard bottle thread, single opening | |
| Dip tube | 1/4" ID PU tube, extends ~100-150mm into bag interior | Platypus Drink Tube Kit |

**Reference: Platypus 2L bags** (not used in current design)

| Parameter | Value | Notes |
|---|---|---|
| Length (when full) | 250-300mm (corrected from 350mm) | layout-spatial-planning.md Section 2c |
| Width | 190mm | |
| Thickness (when full) | 60-80mm | |

### 2h. Incline Mount Geometry (Two 1L Bags)

| Parameter | Value |
|---|---|
| Incline angle | 18-20 degrees from horizontal |
| Horizontal run per bag | 235-238mm (of 242mm available) |
| Bag 1 connector center height | ~26-30mm |
| Bag 1 sealed end top | ~122-134mm |
| Bag 2 connector center height | ~69-78mm |
| Bag 2 sealed end top | ~165-169mm |
| Gap between bags | ~5mm |
| Clearance to dock shelf | ~11-15mm |
| Width consumed | 140mm of 272mm (66mm per side for tubing) |
| Lower mount | Snap-fit U-clip for connector cap (printed on front wall) |
| Upper mount | Binder clip on J-hook for sealed seam (printed on rear wall / shelf underside) |

---

## 3. Remaining Questions Requiring Physical Measurement

1. **Platypus 1L bag actual dimensions**: Measure a filled 1L Platypus bag. Is it 250mm long, 140mm wide, 40mm thick? The incline geometry analysis depends on these inputs.

2. **Dip tube length**: Measure how far the Platypus Drink Tube Kit dip tube extends into the bag interior. This determines the last-liquid threshold.

3. **Incline drainage test**: Mount a filled 1L bag at 18-20 degrees (two nails in a board, binder clips). Pump it dry. At what remaining volume does air appear? Does the dip tube extend the useful range as predicted?

4. **Two-bag clearance verification**: With two bags physically mounted, verify the 5mm inter-bag gap and the 11mm clearance to the dock shelf. Full bags may deform slightly.

5. **Kamoer KPHM400 mounting hole pattern**: Measure center-to-center distances on the actual bracket. The cartridge tray design depends on this.

6. **Kamoer KPHM400 full envelope with barbs**: Measure the pump with barbs attached. The 62.7mm height does not include barb protrusion (~15-20mm above the pump body). If barbs protrude more than 10mm above the body, the cartridge height increases.

7. **John Guest fitting body clearance at 15mm C-C**: Test whether the specific fittings in hand can mount at 15mm center-to-center. If hex flats or molding flash prevent this, the spacing must increase to 18mm C-C.
