# Cascade / Dependency Matrix

**Generated:** 2026-03-24
**Purpose:** Map how each major decision cascades to every other, identify the resolution order, and define coherent decision pathways.

---

## 1. Major Decision Points

New research (bag-dimensions-survey, under-sink-constraints, diagonal-stacking-geometry, fitting-alternatives, access-architecture, diagonal-risks-and-failure-modes) revealed that several foundational assumptions were wrong. This expands the original 4 decision points to 7.

---

### DP1 — Enclosure Outer Dimensions

**The decision:** What are the W x D x H dimensions of the enclosure?

| Option | Dimensions | Notes |
|---|---|---|
| A | 280W x 250D x 400H mm | Current assumption. Broken: 1L bags at 18deg need 282mm depth, only 242mm available. |
| B | 280W x 300D x 400H mm | Depth increase is free (under-sink research). Fixes the broken 18deg layout. |
| C | 280W x 300D x 450H mm | Enables 2L bags in diagonal or incline layouts with comfortable margins. |
| D | 320W x 300D x 450H mm | Maximum capacity option for 2L bags with cartridge beside bag stack. |

**Dependency rank: 7** (constrains DP2, DP3, DP5, DP6, DP7; indirectly constrains DP4)

---

### DP2 — Bag Size and Mounting Angle

**The decision:** Which Platypus bag model, and at what angle?

| Option | Bag | Angle | Requires |
|---|---|---|---|
| A | Platy 1.0L (152x280mm) | 35deg | 300mm+ depth |
| B | Platy 1.0L (152x280mm) | 42-45deg | 250mm depth (exact fit) |
| C | Platy 2.0L (190x350mm) | 55-65deg | 300mm+ depth, 450mm+ height, 320mm+ width OR cartridge rotation |
| D | Platy 1.0L (152x280mm) | 18deg | 300mm+ depth (282mm run fits in 292mm interior) |

**Dependency rank: 6** (constrains DP3, DP5, DP6, DP7; feeds back to DP1)

---

### DP3 — Fitting Type (Cartridge-Dock Fluid Connection)

**The decision:** What connector joins the cartridge to the dock?

| Option | Type | Cost (4x) | Eliminates release mechanism? |
|---|---|---|---|
| A | John Guest push-to-connect + cam lever release plate | $15-20 total | No (requires it) |
| B | CPC valved quick-disconnect (PLC NSF series) | $70 | Yes |
| C | Press-fit with O-ring + detent latch | $2 | Yes |

**Dependency rank: 4** (constrains DP5 cartridge envelope, mating face design, front face design, document validity)

---

### DP4 — Fluid Path Topology

**The decision:** Which fluid operations are automated?

| Option | Solenoids | Automated refill? | Automated clean? |
|---|---|---|---|
| A | 6 | Yes (pump reversal) | Yes |
| B | 4 | Yes (gravity) | Yes |
| C | 4 | No (manual bag swap) | Yes |
| D | 2 | No | No |

**Dependency rank: 3** (constrains BOM, GPIO, hopper design, firmware complexity)

---

### DP5 — Internal Layout Architecture (NEW)

**The decision:** How are components spatially arranged inside the enclosure?

| Option | Description |
|---|---|
| A | Horizontal zones (layer cake: bags bottom, dock middle, electronics top) |
| B | Diagonal interleave (bags on diagonal, components share vertical ranges at different depths) |
| C | Hybrid (bags diagonal, but cartridge and electronics in horizontal sub-zones above/beside) |

**Dependency rank: 5** (constrains dock position, cartridge slot height, electronics placement, internal structure, all zone-based calculations)

This decision was invisible in the original DP set because every document assumed Option A without discussion. The new research revealed that Option A is broken at current dimensions (bag length was wrong), and Options B/C are necessary for 2L bags in any reasonably sized enclosure.

---

### DP6 — Access Architecture (NEW)

**The decision:** How does the user physically access the internals?

| Option | Cost | Enclosure mods |
|---|---|---|
| A | Fixed tower (reach into cabinet) | $0 | None |
| B | Slide-out tray (enclosure on pull-out shelf) | $15-35 | None to enclosure |
| C | Clamshell top (hinged lid) | $3-10 | Split top 90mm into lid |
| D | B+C combined (tray + clamshell) | $20-40 | Moderate |
| E | Full drawer (internals on slides) | $10-20 | Major shell redesign |

**Dependency rank: 2** (constrained by DP1 width for drawer slides; affects enclosure design)

---

### DP7 — Drip Tray / Leak Containment (NEW)

**The decision:** Is there internal leak containment?

| Option | Description |
|---|---|
| A | No drip tray (current recommendation from drip-tray-shelf-analysis.md) |
| B | Sealed enclosure floor with raised edges |
| C | Drip pan under bag cradle only |

**Dependency rank: 1** (constrained by DP2 angle and DP4 refill method; minimal outbound cascades)

This was elevated to a formal decision because the new research shows leak pressure at 60deg is 2.8x higher than at 18deg, and manual bag swap (DP4-C) means liquid handling during disconnection.

---

## 2. Cascade Map

For each decision point, the constraints it imposes on others when a specific option is chosen.

---

### DP1 (Enclosure Dimensions) Cascades

**If DP1-A (280x250x400):**
- DP2 constrained to: **{B only}** (1L at 42-45deg; 18deg does not fit, 2L does not fit)
- DP5 constrained to: **{A or C}** (diagonal possible for 1L at 42deg, but no room for 2L diagonal)
- DP6-E (full drawer) reduces interior width to 246mm; bags at 152mm + cartridge at 150mm = 302mm; **cartridge must go above bags, not beside**
- All existing zone calculations in layout-spatial-planning.md are INVALID (based on 250mm bag length; actual is 280mm)

**If DP1-B (280x300x400):**
- DP2 constrained to: **{A, B, D}** (1L at 18deg now fits in 292mm interior; 1L at 35deg comfortable; 1L at 42-45deg has large depth margin; 2L still does not fit in 400mm height)
- DP5 constrained to: **{A, B, C}** (all layouts viable for 1L bags)
- Sweet spot angles shift to 30-40deg (more height preserved)

**If DP1-C (280x300x450):**
- DP2 constrained to: **{A, B, C, D}** (all options open; 2L at 55deg needs 266mm depth and 333mm height -- fits)
- DP5: all layout options viable
- 2L bags become possible but cartridge cannot sit beside 190mm-wide bags in 272mm interior; **cartridge must go above or in front**

**If DP1-D (320x300x450):**
- DP2: **all options open**, including 2L with cartridge beside bag stack (190mm bag + 80mm rotated cartridge = 270mm, fits in 312mm interior)
- DP5: all layouts viable with comfortable margins
- DP6-E (full drawer): 320mm - 26mm slides = 294mm interior; 190mm bag + 80mm cartridge = 270mm; fits with 24mm margin

---

### DP2 (Bag Size and Angle) Cascades

**If DP2-A (1L at 35deg):**
- DP1 constrained to: **{B, C, D}** (needs 300mm+ depth)
- DP5: horizontal zones viable (bags consume 212mm height, leaving 180mm for dock + electronics)
- DP7: leak pressure at 35deg is moderate (1.8x vs 18deg baseline); drip tray optional

**If DP2-B (1L at 42-45deg):**
- DP1 constrained to: **{A, B, C, D}** (fits at 250mm depth exactly; comfortable at 300mm)
- DP5: horizontal zones work but tight (bags consume 225-233mm height, leaving 159-167mm)
- DP7: leak pressure at 42deg is ~2.1x baseline; containment recommended

**If DP2-C (2L at 55-65deg):**
- DP1 constrained to: **{C, D}** (needs 450mm+ height, 300mm+ depth)
- DP5 constrained to: **{B, C}** (diagonal/hybrid mandatory; horizontal zones cannot fit 2L at any angle in 400mm)
- DP7: leak pressure at 60deg is 2.8x baseline; **containment strongly recommended**
- Bag collapse at steep angles is untested; physical prototype required before committing
- Mounting system must handle 1.73 kg axial force per bag (vs 0.31 kg at 18deg)

**If DP2-D (1L at 18deg):**
- DP1 constrained to: **{B, C, D}** (needs 300mm+ depth; 282mm depth consumption)
- DP5: horizontal zones work well (bags consume only 134mm height, leaving 258mm)
- DP7: lowest leak pressure; drip tray removal is safe

---

### DP3 (Fitting Type) Cascades

**If DP3-A (John Guest + cam lever):**
- cam-lever.md, collet-release.md, release-plate.md, release-mechanism-alternatives.md: **all remain relevant**
- Cartridge depth budget: +35mm behind pumps for release plate + tube stubs
- Front face: must include lever pocket and clearance arc
- Push rod routes between pumps, constraining pump placement
- guide-alignment.md: tight alignment tolerance required (concentric collet pressure)

**If DP3-B (CPC valved):**
- cam-lever.md, collet-release.md, release-plate.md: **OBSOLETE**
- mating-face.md: **REWRITE** (CPC panel-mount bodies at 22mm OD vs JG at 12mm; different port spacing)
- Cartridge depth budget: **saves ~10-15mm** (no release plate, no tube stubs through plate)
- Front face: no lever pocket needed; pull handle or simple grip
- guide-alignment.md: tolerance requirements relax significantly (CPC self-aligns better)
- BOM: +$55-62 fittings, -$5-10 release mechanism parts, -engineering hours for release plate iteration
- **Only option with auto-shutoff** (zero drip during swap under sink)

**If DP3-C (Press-fit + O-ring):**
- Same document obsolescence as CPC (cam-lever.md, collet-release.md, release-plate.md: **OBSOLETE**)
- mating-face.md: **REWRITE** (smooth stubs + O-rings, simpler than either JG or CPC)
- **Requires new design work:** detent/latch mechanism for positive retention
- No auto-shutoff; lines must be drained before disconnect
- Cheapest option but reliability is unproven; needs physical testing

---

### DP4 (Fluid Path Topology) Cascades

**If DP4-A (full topology, 6 solenoids):**
- Hopper required (DP1/DP5 must allocate hopper space)
- gpio-planning.md: 6 solenoid pins + FDC1004 for level sensing
- bill-of-materials.md: ~$60 in solenoids
- Pump reversal is unproven; blocks firmware until tested

**If DP4-B (gravity fill, 4 solenoids):**
- Hopper required but simpler (no sealed pressure, just funnel + valve)
- Gravity flow rate through dip tube is unknown
- Fill percentage uncertain (70-80% without active air management)

**If DP4-C (manual bag swap, 4 solenoids):**
- No hopper; hopper-and-bag-management.md: **OBSOLETE**
- pump-assisted-filling.md: **OBSOLETE**
- DP7 becomes more important (user handles wet bags; drip containment matters)
- Bag removal/reinstall ergonomics become the primary frequent interaction
- DP6 (access architecture) becomes more important (easy bag access needed)

**If DP4-D (dispensing only, 2 solenoids):**
- hopper-and-bag-management.md, pump-assisted-filling.md: **OBSOLETE**
- Clean cycle plumbing: **DEFERRED**
- gpio-planning.md: MCP23017 expander possibly unnecessary
- Fastest path to working prototype

---

### DP5 (Layout Architecture) Cascades

**If DP5-A (horizontal zones):**
- All existing zone-based documents remain structurally valid (after correcting bag dimensions)
- dock-mounting-strategies.md: dock shelf at a calculated height above bag zone top
- layout-spatial-planning.md: update zone heights with corrected bag geometry
- 2L bags are **impossible** in horizontal zones at any height 450mm or below

**If DP5-B (diagonal interleave):**
- layout-spatial-planning.md: **REWRITE** (zone model replaced with 3D component placement)
- bag-zone-geometry.md: **REWRITE** (zone concept replaced with diagonal envelope)
- dock-mounting-strategies.md: dock position depends on where bags leave free space, not on a zone boundary
- Internal structure changes from horizontal shelves to angled cradles and corner brackets
- 3D printing complexity increases (compound angles, mounting tabs)

**If DP5-C (hybrid):**
- bag-zone-geometry.md: **REWRITE** for diagonal bags
- dock-mounting-strategies.md: may stay roughly valid if cartridge is in a horizontal sub-zone above bags
- Moderate rewrite scope: fewer documents affected than full diagonal

---

### DP6 (Access Architecture) Cascades

**If DP6-A (fixed tower):**
- No cascade. All existing documents valid. Cheapest.

**If DP6-B (slide-out tray):**
- No cascade on internal design. Enclosure unchanged.
- External plumbing needs 250-300mm service loops
- Under-sink installation adds 25-40mm height below enclosure

**If DP6-C (clamshell top):**
- Enclosure must split at electronics shelf line (~310mm from floor)
- Hinge + magnetic hold-open; adds 2 hinges + magnets to BOM
- Hopper funnels must mount below hinge line

**If DP6-E (full drawer):**
- DP1 width must accommodate 25mm for drawer slides (enclosure needs 305mm+ width for 280mm interior, or accept 246mm interior at 280mm width)
- Fluid lines cross moving boundary; need service loops
- Cantilevered weight: 6-9kg; requires heat-set inserts, not screws into plastic

---

### DP7 (Drip Tray) Cascades

**If DP7-A (no drip tray):**
- Frees ~15mm of floor space for bags
- Acceptable only if: angle is shallow (18-35deg) AND bags are never removed (DP4-A or DP4-B)
- If DP2-C (steep angle) or DP4-C (manual bag swap): **containment risk is unacceptable**

**If DP7-B (sealed floor with raised edges):**
- No height penalty (raised edges are at perimeter, not under bags)
- Adds minor complexity to enclosure floor molding/printing

**If DP7-C (drip pan under cradle):**
- Consumes ~5-10mm of height under bags
- Best for diagonal layouts where a full-floor tray would conflict with the angled cradle

---

## 3. Decisions Ranked by Dependency Count

| Rank | Decision | Outbound Constraints | Resolve First? |
|---|---|---|---|
| 1 | **DP1 — Enclosure Dimensions** | 7 (constrains every other DP) | Yes -- first |
| 2 | **DP2 — Bag Size and Angle** | 6 (constrains layout, dock, drip tray, hopper) | Yes -- second (co-dependent with DP1) |
| 3 | **DP5 — Layout Architecture** | 5 (constrains every spatial document) | Yes -- third (determined by DP1+DP2 outcome) |
| 4 | **DP3 — Fitting Type** | 4 (constrains cartridge design, mating face, workflow) | Can be decided independently |
| 5 | **DP4 — Fluid Path Topology** | 3 (constrains BOM, firmware, hopper) | Can be decided independently |
| 6 | **DP6 — Access Architecture** | 2 (minor enclosure impact) | Decide after DP1 |
| 7 | **DP7 — Drip Tray** | 1 (minimal outbound impact) | Decide last (falls out of DP2+DP4) |

**Resolution order:** DP1 + DP2 (co-decide) -> DP5 (falls out of DP1+DP2) -> DP3 and DP4 (parallel, independent) -> DP6 -> DP7.

---

## 4. Decision Pathways

Three internally consistent combinations. For each, the chosen options, resulting enclosure, and document impact.

---

### Pathway 1: "Compact" (Minimum Change from Current Design)

| DP | Choice | Rationale |
|---|---|---|
| DP1 | B: 280x300x400mm | 50mm deeper fixes broken bag geometry; depth is free |
| DP2 | A: 1L Platy at 35deg | Comfortable fit at 300mm depth with 34mm depth margin and 180mm remaining height |
| DP5 | A: Horizontal zones | Least disruption to existing documents |
| DP3 | A: John Guest + cam lever | Cheapest fittings; existing research stays valid |
| DP4 | C: Manual bag swap, 4 solenoids | Simplifies plumbing; eliminates hopper |
| DP6 | B: Slide-out tray | Best hopper-free ergonomics for bag swap access |
| DP7 | C: Drip pan under cradle | Manual bag swap means liquid handling; containment needed |

**Enclosure:** 280 x 300 x 400mm (33.6L)
**Refill frequency:** Every 1-2 weeks (moderate user), every 3-5 days (family)
**BOM impact:** 4 solenoids (~$40), JG fittings ($8), release mechanism (~$10), slide-out tray ($15-35)
**Prototype risk:** Low. Release plate stepped bores are the main unknown.

**Documents that remain valid (with bag dimension corrections):**
- cam-lever.md, collet-release.md, release-plate.md, mating-face.md (all stay)
- guide-alignment.md, electrical-mating.md (stay)
- layout-spatial-planning.md (rewrite zone heights for 300mm depth and 35deg angle)
- bag-zone-geometry.md (rewrite with corrected dimensions)
- dock-mounting-strategies.md (update position for new zone heights)
- dimensions-reconciliation.md (update to 280x300x400)

**Documents that become obsolete:**
- hopper-and-bag-management.md (no hopper)
- pump-assisted-filling.md (no pump reversal fill)

---

### Pathway 2: "Capacity" (2L Bags, Maximum Product Viability)

| DP | Choice | Rationale |
|---|---|---|
| DP1 | C: 280x300x450mm | Height and depth for 2L bags; still fits most cabinets |
| DP2 | C: 2L Platy at 55deg | Two stacked bags: 266mm depth, 333mm height; fits in 292x442mm interior |
| DP5 | B: Diagonal interleave | Required; horizontal zones cannot fit 2L bags |
| DP3 | B: CPC valved | Auto-shutoff prevents under-sink drip damage; eliminates release mechanism |
| DP4 | C: Manual bag swap, 4 solenoids | Avoids hopper complexity; 2L bags refill monthly anyway |
| DP6 | D: Slide-out tray + clamshell top | Best access for diagonal bag installation |
| DP7 | B: Sealed floor with raised edges | Steep angle = 2.8x leak pressure; mandatory containment |

**Enclosure:** 280 x 300 x 450mm (37.8L)
**Refill frequency:** Monthly (moderate user), every 10 days (family)
**BOM impact:** 4 solenoids (~$40), CPC fittings ($70), tray + clamshell ($20-40)
**Prototype risk:** HIGH. Bag collapse at 55deg is untested. Structural cradle design is non-trivial. Must build and test cradle prototype before committing.

**Width constraint:** 2L bags at 190mm wide leave only 82mm beside them in 272mm interior. Cartridge at 150mm does not fit beside bags. Cartridge must go above bag stack in ~109mm of remaining height (442 - 333 = 109mm), which fits the 80mm-tall cartridge with 29mm for lever/clearance. Tight but viable.

**Documents that remain valid:**
- guide-alignment.md (tolerance relaxes with CPC)
- electrical-mating.md (pogo pins unchanged)
- plumbing.md (topology stays, fittings change at dock interface)
- dip-tube-analysis.md (unchanged)

**Documents that require REWRITE:**
- layout-spatial-planning.md (horizontal zones replaced with diagonal placement)
- bag-zone-geometry.md (zone model replaced with diagonal envelope)
- dock-mounting-strategies.md (dock position changes)
- mating-face.md (CPC bodies replace JG fittings)
- cartridge-change-workflow.md (CPC squeeze-pull replaces lever flip)
- cartridge-envelope.md (depth budget changes, position changes)
- dimensions-reconciliation.md (new dimensions)
- front-face-interaction-design.md (no lever)
- under-cabinet-ergonomics.md (new slot height)

**Documents that become OBSOLETE:**
- cam-lever.md
- collet-release.md
- release-plate.md
- release-mechanism-alternatives.md (superseded by fitting-alternatives.md)
- hopper-and-bag-management.md
- pump-assisted-filling.md

---

### Pathway 3: "Budget / Fast Prototype" (Simplest Path to a Working Unit)

| DP | Choice | Rationale |
|---|---|---|
| DP1 | B: 280x300x400mm | Fixes broken geometry; conservative |
| DP2 | D: 1L Platy at 18deg | Shallowest viable angle at 300mm depth; most existing research survives |
| DP5 | A: Horizontal zones | Maximum document reuse |
| DP3 | C: Press-fit + O-ring + latch | $2 fittings; eliminates release mechanism; fastest to prototype |
| DP4 | D: Dispensing only, 2 solenoids | MVP; defer clean and refill |
| DP6 | A: Fixed tower | $0; simplest |
| DP7 | A: No drip tray | Shallow angle + no bag removal = low leak risk |

**Enclosure:** 280 x 300 x 400mm (33.6L)
**Refill frequency:** Every 1-2 weeks (moderate user)
**BOM impact:** 2 solenoids (~$20), press-fit O-rings ($2), no release mechanism, no tray, no hopper hardware
**Prototype risk:** MEDIUM. Press-fit reliability is unproven. Detent latch design needed. But overall simplest path to a dispensing prototype.

**Documents that remain valid (with corrections):**
- layout-spatial-planning.md (rewrite zone heights for 300mm depth)
- bag-zone-geometry.md (update to 280mm bag length at 18deg)
- dock-mounting-strategies.md (update shelf position)
- dip-tube-analysis.md
- guide-alignment.md (tolerance relaxes with press-fit)
- electrical-mating.md

**Documents that become OBSOLETE:**
- cam-lever.md
- collet-release.md
- release-plate.md
- hopper-and-bag-management.md
- pump-assisted-filling.md
- release-mechanism-alternatives.md (superseded)

**Documents that require REWRITE:**
- mating-face.md (press-fit stubs + O-rings)
- cartridge-change-workflow.md (pull-to-disconnect)
- cartridge-envelope.md (no release plate, shallower depth)
- front-face-interaction-design.md (no lever pocket)
- dimensions-reconciliation.md (280x300x400)
- bill-of-materials.md (reduced component count)

---

## Pathway Comparison

| Factor | Compact | Capacity | Budget |
|---|---|---|---|
| Enclosure volume | 33.6L | 37.8L | 33.6L |
| Bag capacity | 1L | 2L | 1L |
| Refill (moderate user) | 2 weeks | 1 month | 2 weeks |
| Refill (family) | 3-5 days | 10 days | 3-5 days |
| Fitting cost | $8 | $70 | $2 |
| Solenoid cost | $40 | $40 | $20 |
| Documents surviving unchanged | ~15 | ~5 | ~10 |
| Documents requiring rewrite | ~8 | ~12 | ~10 |
| Documents becoming obsolete | 2 | 6 | 5 |
| Prototype risk | Low | High | Medium |
| Key unproven element | Release plate stepped bores | Bag collapse at 55deg | Press-fit retention reliability |
| Consumer product viability | Moderate (frequent refills) | High (monthly refills) | Low (MVP only) |

---

## Critical Finding: All Pathways Require DP1 Depth Increase to 300mm

The single finding that cuts across every pathway: **the current 250mm depth is broken**. The corrected 1L bag length (280mm, not 250mm) means even the shallowest viable angle (18deg) consumes 282mm of depth. At 242mm interior depth, no bag fits at any angle.

Increasing depth to 300mm (292mm interior) costs nothing in under-sink compatibility (480-510mm of depth is available). This is not a decision -- it is a correction. Every pathway above uses 300mm depth.

## Correction: Zone-Thinking Bias in Diagonal Analysis

Post-review revealed that the diagonal geometry research evaluated all layouts by asking "how much height remains above the bags for the cartridge?" — which is horizontal-zone thinking applied to diagonal layouts. In a diagonal layout, the cartridge sits in the front-bottom triangular void, NOT above the bags.

This means:
- **The "Capacity" pathway may not need 450mm height.** 2L bags at 35° diagonal stacking consume 266mm height and 333mm depth. In a 350mm-deep enclosure (342mm interior), this fits. The cartridge goes in the front-bottom triangle (~130mm deep × ~200mm tall), not above the bags. A 280W × 350D × 400H enclosure (39.2L) may support 2L bags — a new pathway option not covered above.
- **DP1 should include a new option:** 280W × 350D × 400H. This keeps the 400mm height while using the unconstrained depth dimension more aggressively.
- The 350mm depth is still well within the 480-510mm available under sinks.
