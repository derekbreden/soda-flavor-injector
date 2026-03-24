# Assumption Index

**Purpose:** Categorize all existing research documents by the assumptions they bake in. This lets any future agent or reader instantly see which documents are relevant to a given design pathway and which are operating under assumptions that may not hold.

This is NOT a rewrite plan. The documents are fine as research — they just need to be read with awareness of what they assume.

---

## Assumption Categories

### A1: Horizontal Zones (Layer Cake Architecture)
Components are stacked in horizontal slices: bags at bottom, dock in middle, electronics on top. Each component confined to its own height band.

**Incompatible with:** User Vision 1 (diagonal interleave), any layout where components share vertical ranges at different depths.

### A2: 250mm Enclosure Depth
Interior depth = 242mm. All zone budgets, horizontal runs, and depth fits use this number.

**Known broken:** 1L bags at 18° need 282mm depth. Nothing fits at 250mm depth at any angle ≤30°. Corrected minimum: 300mm (for shallow angles) or 350mm (for 2L bags at 35°).

### A3: 1L Bags at 250mm × 140mm
Bag length 250mm, width 140mm. These are wrong. Actual Platy 1L: 280mm × 152mm. Actual Platy 2L: 350mm × 190mm.

**Impact:** Every geometry calculation using these numbers is wrong. Clearances, zone heights, stacking analysis — all invalid.

### A4: 18-20° Incline Angle
The "sweet spot" derived from the wrong bag dimensions. At corrected dimensions, 18° needs 282mm depth (broken at 250mm). At 300mm depth, viable. At 250mm depth, minimum viable angle is ~42°.

### A5: John Guest Fittings + Cam Lever Release
Assumes push-to-connect fittings requiring simultaneous collet depression via a cam lever and release plate mechanism.

**Eliminated by:** CPC quick-disconnects, press-fit with O-ring, Luer lock, magnetic coupling, or custom bayonet — any of which removes the need for cam lever, collet release, and release plate entirely.

### A6: 400mm Enclosure Height
All zone budgets derived from 400mm. Under-sink research shows 380-420mm typical clearance under sink bowl, but side zones can be taller (up to ~648mm).

### A7: 280mm Enclosure Width
Side zones in 33"+ US cabinets are 280-330mm. This number is reasonable but not a hard constraint.

### A8: Cartridge Must Stack Above Bags
Every layout places the cartridge in a horizontal zone above the bag zone. In a diagonal layout, the cartridge sits in the front-bottom triangular void — a completely different spatial relationship.

**This assumption persists even in the new diagonal geometry research** — the agents couldn't fully escape zone-thinking when evaluating diagonal layouts.

---

## Document Classification

### Documents assuming A1 + A2 + A3 + A4 + A5 (Full Zone Stack)
*These documents are deeply embedded in the original horizontal-zone, 250mm-depth, 1L-at-18°, John-Guest design. They have NO consideration for diagonal layouts, deeper enclosures, 2L bags, or alternative fittings.*

| Document | Additional Assumptions | Still Useful For |
|---|---|---|
| `enclosure/research/layout-spatial-planning.md` | A6, A7, A8 | Understanding the zone-based approach if chosen |
| `hardware/bag-zone-geometry.md` | A6, A8 | Historical reference; geometry method is sound, numbers are wrong |
| `hardware/dimensions-reconciliation.md` | A6, A7 | Template for a future reconciliation with corrected numbers |
| `enclosure/research/incline-bag-mounting.md` | A6 | The incline concept is valid; specific angles/numbers need recalculation |

### Documents assuming A5 only (John Guest + Release Mechanism)
*These documents are fitting-specific. If DP3 goes to CPC or press-fit, they become historical reference. If John Guest is kept, they remain relevant regardless of other decisions.*

| Document | Status if JG kept | Status if CPC/press-fit chosen |
|---|---|---|
| `cartridge/planning/research/cam-lever.md` | ✅ Valid | ❌ Obsolete |
| `cartridge/planning/research/collet-release.md` | ✅ Valid | ❌ Obsolete |
| `cartridge/planning/research/release-plate.md` | ✅ Valid | ❌ Obsolete |
| `cartridge/planning/research/release-mechanism-alternatives.md` | ⚠️ Incomplete (missing CPC/press-fit detail) | Superseded by `fitting-alternatives.md` |

### Documents assuming A1 + A2 + A3 but NOT A5 (Zone layout, independent of fittings)
*These care about enclosure dimensions and bag geometry but not the fitting choice.*

| Document | Key Assumptions | Notes |
|---|---|---|
| `enclosure/research/hopper-and-bag-management.md` | A1, A2, A3, A4, A6 | Core hopper/funnel concepts valid; position and geometry numbers wrong |
| `enclosure/research/back-panel-and-routing.md` | A2, A6, A7 | Routing concepts valid; specific lengths change with depth |
| `enclosure/research/front-face-interaction-design.md` | A5 (lever), A6, A7 | Display design valid; cartridge slot depends on DP3 |
| `hardware/bill-of-materials.md` | A3, A5 | Component list useful as baseline; quantities change with DP3/DP4 |

### Documents assuming A1 + A8 (Cartridge above bags)
*These place the cartridge dock in a zone above the bag zone. In a diagonal layout, the dock moves to the front-bottom triangle.*

| Document | Notes |
|---|---|
| `cartridge/planning/research/dock-mounting-strategies.md` | Dock-as-shelf concept assumes horizontal zone boundary. Dock position changes completely in diagonal layout. |
| `cartridge/planning/research/cartridge-envelope.md` | 150×80×130mm envelope derived from zone height budget. If lever is eliminated, 40mm freed. If diagonal layout, envelope constraints change. |
| `cartridge/planning/research/under-cabinet-ergonomics.md` | Cartridge slot height at ~226mm assumes zone layout. Changes in diagonal. |
| `cartridge/planning/research/cartridge-change-workflow.md` | Workflow assumes cam lever + front-loading at zone height. Depends on DP3 and DP5. |

### Documents with NO broken assumptions (valid regardless of pathway)
*These are about physics, materials, or component specs that don't depend on layout, dimensions, or fittings.*

| Document | Why It's Safe |
|---|---|
| `enclosure/research/dip-tube-analysis.md` | Fluid dynamics of the dip tube. Independent of layout. Steeper angles may actually improve drainage. |
| `enclosure/research/drip-tray-shelf-analysis.md` | Scenario analysis of leak modes. Conclusions hold, though steeper angles increase leak pressure (noted). |
| `enclosure/research/pump-assisted-filling.md` | Fluid topology for hopper→bag filling. Independent of geometry. |
| `cartridge/planning/research/electrical-mating.md` | Pogo pin specs and contact design. Independent of everything. |
| `cartridge/planning/research/guide-alignment.md` | Alignment concepts (tapered pins, V-grooves). Valid for any layout; precision requirements may relax with CPC. |
| `cartridge/planning/research/pump-mounting.md` | Kamoer pump dimensions, vibration isolation. Independent of enclosure layout. |
| `hardware/gpio-planning.md` | ESP32 pin mapping, I2C devices. Solenoid count depends on DP4 but pin assignments are valid. |

### New Research (no legacy assumptions)
*These documents were produced with corrected data and awareness of the full possibility space.*

| Document | What It Covers |
|---|---|
| `decision-points/research/bag-dimensions-survey.md` | Corrected Platypus dimensions from manufacturer specs |
| `decision-points/research/under-sink-constraints.md` | Real cabinet dimensions across markets |
| `decision-points/research/diagonal-stacking-geometry.md` | Angle/dimension sweep with corrected bags (⚠️ still has zone-bias in component placement — see A8 correction) |
| `decision-points/research/diagonal-risks-and-failure-modes.md` | Devil's advocate on diagonal approach |
| `decision-points/research/fitting-alternatives.md` | 8 fitting types compared |
| `decision-points/research/access-architecture.md` | How user accesses internals |
| `decision-points/cascade-matrix.md` | Decision dependencies and pathways (⚠️ has A8 correction appended) |
| `decision-points/rewrite-assessment.md` | Document-by-document impact of new findings |
| `decision-points/visions/diagonal-interleave.md` | Owner's vision — the reference for non-zone thinking |

---

## How to Use This Index

**If you are an agent working on this project:**

1. Check which assumptions your task involves
2. Look up which documents share those assumptions — they are your relevant reading
3. Look up which documents assume DIFFERENT things — they may contain viable alternatives you should consider
4. **Do not treat any document's conclusions as decisions.** They are research under specific assumptions.

**If you are exploring User Vision 1 (diagonal interleave):**
- Documents in the "Full Zone Stack" and "Cartridge above bags" categories have little relevance — they assume a layout that Vision 1 rejects
- Documents in the "No broken assumptions" category are all relevant
- Documents in the "A5 only" category are relevant only if John Guest fittings are chosen
- The new research documents are all relevant, with the caveat that the diagonal geometry research still has residual zone-bias in its component placement analysis

**If you are exploring a corrected horizontal-zone layout (e.g., 300mm depth, 35° angle):**
- Documents in the "Full Zone Stack" category are relevant but need their numbers recalculated
- The zone architecture itself (bags below, dock above, electronics on top) still works — it just needs corrected dimensions
- Fitting choice (A5) is independent and all fitting documents are relevant
