# Rewrite Assessment: Impact of New Research on Existing Documents

**Date:** 2026-03-24
**Trigger:** Six new research documents in `decision-points/research/` revealed critical errors in bag dimensions, depth assumptions, angle calculations, fitting alternatives, and access architecture that invalidate or weaken significant portions of the existing documentation.

---

## Summary of New Findings

1. **Bag dimensions were wrong everywhere.** The Platy 1L is 280mm long x 152mm wide (not 250mm x 140mm). The Platy 2L is 350mm x 190mm. This breaks the existing 18-degree layout at 250mm depth (needs 282mm, only 242mm available).
2. **Depth is unconstrained.** Under-sink cabinets have 480-510mm of usable depth. Going to 300mm costs nothing. The 250mm assumption was never justified.
3. **The practical bag angle sweet spot is 30-45 degrees, not 18-20 degrees or 60-65 degrees.** At 300mm depth, 30-40 degrees provides the best balance. At 250mm depth, 42-45 degrees is the minimum viable angle.
4. **2L bags require a fundamentally larger enclosure** (~320x300x450mm). No arrangement of 2L bags fits in 400mm height.
5. **CPC and press-fit fittings eliminate the cam lever / release plate entirely.** Three major research documents (cam-lever.md, collet-release.md, release-plate.md) may be obsolete depending on the fitting choice.
6. **Curved cradles do not help.** Bounding box is endpoint-determined.
7. **A $15-35 slide-out tray solves the access problem** with zero enclosure modifications.

---

## Document-by-Document Assessment

### Enclosure Research (`hardware/enclosure/research/`)

---

#### incline-bag-mounting.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Uses 250mm bag length and 140mm bag width throughout. Actual Platy 1L is 280mm x 152mm.
- The 18-20 degree sweet spot analysis is based on wrong bag length. At 280mm, 18 degrees needs 282mm of depth -- exceeds 242mm interior by 40mm. The design physically does not fit.
- The depth constraint analysis (Section 2c) concludes 238mm horizontal run fits in 242mm with 4mm margin. With corrected dimensions, it misses by 40mm.
- Bag width of 152mm (not 140mm) reduces side clearance from 66mm per side to 60mm per side -- marginal but not breaking.

**What needs to change:**
- All geometry tables must be recalculated with L=280mm, W=152mm.
- The sweet spot analysis must be redone. At 250mm depth, minimum viable angle is ~42 degrees. At 300mm depth, the sweet spot shifts to 30-40 degrees.
- Two-bag stacking analysis must be recalculated -- at steeper angles, vertical consumption per bag increases, and the 176mm bag zone may not hold two stacked bags.
- The "advantages over vertical hanging" section remains conceptually valid but all specific numbers are wrong.

**Dependencies:** DP1 (enclosure depth), DP2 (bag size choice).

---

#### layout-spatial-planning.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- Built entirely on 280x250x400mm enclosure dimensions, which are challenged on depth (250mm is insufficient for 1L bags at 18 degrees).
- Zone heights assume 18-degree bag mounting. If bags go to 30-42 degrees, the bag zone height changes significantly, which cascades into dock shelf position, cartridge slot height, and everything above.
- Bag dimensions stated as 250mm x 140mm (wrong).
- 2L bag length stated as "250-300mm (corrected from 350mm)" -- this "correction" was itself wrong; 350mm is the actual manufacturer dimension for Platy 2L.
- References cam lever and release plate as part of the dock zone design. If DP3 goes to CPC or press-fit, the lever clearance zone (266-306mm, 40mm) is freed.

**What needs to change:**
- Must present multiple scenarios: (a) 280x300x400mm with 1L bags at 30-35 degrees, (b) 280x300x450mm with 2L bags, (c) current dimensions if somehow viable.
- All zone height calculations must be conditional on the bag angle and depth decisions.
- The lever clearance zone should be conditional on DP3 (cam lever vs. CPC/press-fit).
- Bag dimensions must be corrected throughout.
- Under-sink fit analysis should incorporate the finding that 300mm depth is easily available.

**Dependencies:** DP1, DP2, DP3.

---

#### dip-tube-analysis.md

**Status:** ✅ Still valid

**What is wrong:** Nothing fundamental. The dip tube analysis is about the Platypus Drink Tube Kit's fluid dynamics, seal integrity, and flow characteristics. These do not depend on bag dimensions or enclosure geometry.

**What needs to change:** Minor -- if the bag mounting angle changes from 18-20 degrees to 30-45 degrees, the dip tube orientation analysis (Section 3) should note the steeper angle means the tube extends more vertically into the bag, which is actually better for staying submerged until the bag is nearly empty. The core conclusions strengthen rather than weaken.

**Dependencies:** None blocking.

---

#### drip-tray-shelf-analysis.md

**Status:** ✅ Still valid

**What is wrong:** Nothing. The analysis is about liquid scenarios in the sealed fluid path and whether drip containment is justified. The bag dimensions do not affect the liquid scenario inventory or the drip tray cost-benefit analysis.

**What needs to change:** If the bag angle steepens to 30-45 degrees, hydrostatic pressure at a pinhole leak increases (sin(30-45) = 0.50-0.71 vs. sin(18) = 0.31). The diagonal-risks research notes that a 60-degree bag leaks 2.8x faster than an 18-degree bag. At 30-40 degrees this is ~1.6-2.2x. The conclusion that the drip tray is unjustified still holds -- the leak scenarios are still assembly defects or catastrophic failures -- but the increased pressure at steeper angles could be noted.

**Dependencies:** DP2 (bag angle).

---

#### pump-assisted-filling.md

**Status:** ✅ Still valid

**What is wrong:** Nothing related to the new findings. The pump-assisted filling analysis is about fluid topology, pump reversal feasibility, and air management. These do not depend on bag dimensions or enclosure geometry.

**What needs to change:** Nothing from these findings. The document's existing caveats (pump reversal is untested, 85-95% fill is theoretical) remain the main concerns.

**Dependencies:** DP4.

---

#### hopper-and-bag-management.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Bag dimensions referenced as 250mm x 140mm for 1L bags (wrong).
- Bag mounting described as 18-20 degree incline (angle will change).
- Hopper fill timing calculations may change if bag zone geometry shifts.

**What needs to change:**
- Correct bag dimensions throughout.
- Bag mounting angle references must be updated to reflect the corrected sweet spot.
- Hopper placement analysis should consider whether the access-architecture finding (slide-out tray) changes how the user interacts with the hopper.
- Core findings about pump-assisted vs. gravity fill, funnel design, and capacitive sensing remain valid.

**Dependencies:** DP1 (depth), DP2 (bag size/angle), DP4 (fill method).

---

#### front-face-interaction-design.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- Assumes cartridge slot with cam lever handle on front face. If DP3 goes to CPC or press-fit fittings, the lever is eliminated and the front face design simplifies significantly.
- Display holder design (magnetic retention, cable routing) is independent of the new findings and remains valid.
- Enclosure dimensions (280x250x400) used for panel layout may change (depth increase to 300mm does not affect front face width or height, but a height change would).

**What needs to change:**
- The cartridge slot section should present scenarios with and without a cam lever.
- If CPC fittings are chosen, the front face only needs a pull handle or recessed grip, not a lever mechanism with clearance arc.
- Display sections, material/finish, and layout hierarchy remain valid as-is.

**Dependencies:** DP3 (fitting choice determines lever presence).

---

#### back-panel-and-routing.md

**Status:** ⚠️ Needs update

**What is wrong:**
- References enclosure dimensions as "approximately 250mm wide x 350mm tall x 250mm deep" in Section 1c -- inconsistent with the 280x250x400mm used elsewhere.
- If enclosure depth increases to 300mm, back panel depth dimension changes and cable/tube routing lengths adjust.
- The drip dam ridge analysis remains valid (water connections low, electrical high).

**What needs to change:**
- Correct enclosure dimension references.
- If depth changes to 300mm, update internal routing path lengths.
- Connection inventory and arrangement remain valid regardless.

**Dependencies:** DP1 (enclosure dimensions).

---

### Cartridge Research (`hardware/cartridge/planning/research/`)

---

#### cam-lever.md

**Status:** ❌ Potentially obsolete

**What is wrong:** The entire document is predicated on John Guest push-to-connect fittings requiring a simultaneous collet release mechanism. The new fitting-alternatives research shows that CPC quick-disconnects and press-fit fittings both eliminate the need for any release mechanism whatsoever.

**What needs to change:**
- If DP3 = CPC or press-fit fittings, this document becomes historical reference only.
- If DP3 = John Guest with cam lever, the document remains valid. The cam lever analysis itself (eccentricity, force multiplication, over-center behavior) is mechanically sound and not affected by the new findings.

**Dependencies:** DP3 (fitting choice). This document lives or dies on DP3.

---

#### collet-release.md

**Status:** ❌ Potentially obsolete

**What is wrong:** Same as cam-lever.md -- entirely specific to John Guest push-to-connect fittings. If CPC or press-fit fittings are chosen, there are no collets to release.

**What needs to change:**
- If DP3 = CPC or press-fit, this document is void.
- If DP3 = John Guest, the document remains valid but its inferred dimensions (collet travel 1.5-2.0mm, release force 2-5N) still need physical verification.

**Dependencies:** DP3.

---

#### release-plate.md

**Status:** ❌ Potentially obsolete

**What is wrong:** Same dependency chain. The release plate exists solely to press John Guest collets simultaneously. CPC and press-fit fittings have no collets.

**What needs to change:**
- If DP3 = CPC or press-fit, this document is void.
- If DP3 = John Guest, the stepped bore geometry, guide features, and print strategy remain relevant but all dimensions are inferred from patents and need physical validation.

**Dependencies:** DP3.

---

#### release-mechanism-alternatives.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Rejects CPC primarily on cost ($40-60) without adequately weighing the engineering simplification of eliminating the release plate, push rod, and lever entirely.
- The new fitting-alternatives research prices CPC PLC NSF valved at ~$70 for 4 connections and provides much more detailed comparison including NSF 169 food certification, auto-shutoff, and one-hand operation.
- The document does not consider press-fit fittings at all.

**What needs to change:**
- Incorporate the detailed CPC pricing, specifications, and auto-shutoff analysis from fitting-alternatives.md.
- Add press-fit fittings as an evaluated option.
- Reweight the cost-vs-simplification tradeoff given that the CPC premium ($70 vs $8) eliminates the entire release mechanism and its associated engineering risk.
- Note that cartridge swaps are 18-36 month maintenance events where intuitiveness after a long gap matters more than speed.

**Dependencies:** DP3.

---

#### mating-face.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- Built entirely on the assumption of John Guest fittings with tube stubs protruding through a release plate, in a 2x2 grid at 15mm center-to-center.
- If CPC fittings are chosen, the mating face is fundamentally different: CPC inserts protrude from the rear face, no release plate, no push rod bore, different spacing (CPC body OD is ~22mm vs JG at ~12mm).

**What needs to change:**
- Present the mating face design as conditional on DP3.
- Scenario A: John Guest + release plate (current content, still valid).
- Scenario B: CPC couplings (new design needed -- larger spacing, no release plate, simpler alignment requirements).
- Scenario C: Press-fit (new design -- potentially simplest face geometry).
- Electrical contacts section (pogo pins) is independent of fitting choice and remains valid.
- Guide alignment section may simplify with CPC (more tolerant of misalignment).

**Dependencies:** DP3.

---

#### electrical-mating.md

**Status:** ✅ Still valid

**What is wrong:** Nothing from the new findings. Pogo pin analysis, contact pad sizing, wipe action, and sourcing are all independent of bag dimensions, enclosure depth, and fitting choice.

**What needs to change:** Minor -- if DP3 changes the mating motion (e.g., CPC pull-disconnect changes the insertion/withdrawal sequence), the contact engagement timing analysis should be reviewed. But the core pogo pin recommendation holds.

**Dependencies:** DP3 (minor -- engagement sequence only).

---

#### guide-alignment.md

**Status:** ✅ Still valid (may simplify)

**What is wrong:** Nothing from the new findings. Tapered pins, V-grooves, and kinematic coupling analysis are geometry-independent.

**What needs to change:** If DP3 = CPC, alignment requirements relax (CPC couplings are more tolerant of misalignment than John Guest fittings with a release plate). The document's content remains valid as reference, but the recommended precision level may decrease.

**Dependencies:** DP3 (precision requirements may relax).

---

#### pump-mounting.md

**Status:** ✅ Still valid

**What is wrong:** Nothing. Pump dimensions, vibration analysis, mounting hole patterns, and tray/shell construction are independent of bag dimensions, enclosure depth, and fitting choice.

**What needs to change:** If DP3 eliminates the push rod (CPC or press-fit), there is more internal space in the cartridge. The pump mounting design itself does not change, but the cartridge interior layout has more room for tube routing.

**Dependencies:** DP3 (minor -- internal space availability).

---

#### cartridge-envelope.md

**Status:** ⚠️ Needs update

**What is wrong:**
- The 150x80x130mm envelope is derived from the current 400mm enclosure height and zone budget. If DP1 changes (taller enclosure or different depth), the dock zone height budget changes and the cartridge height constraint may relax.
- If DP3 = CPC, the depth budget shrinks (no release plate = ~6mm saved, no tube stub protrusion through plate). The cartridge could be shallower.
- Side-by-side pump arrangement is called "the ONLY viable option" -- true at 80mm height, but if the enclosure grows taller, stacked arrangements become viable.

**What needs to change:**
- Present envelope dimensions as conditional on DP1 (enclosure height) and DP3 (fitting choice).
- If lever clearance zone is eliminated (DP3 = CPC/press-fit), 40mm of height is freed in the dock zone. This could increase cartridge height to 100-110mm, potentially enabling stacked pump arrangement.
- Core pump dimension data and weight calculation remain valid.

**Dependencies:** DP1, DP3.

---

#### cartridge-change-workflow.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- Modeled entirely around the eccentric cam lever + release plate baseline. The 19-25 second swap time is specific to this mechanism.
- If DP3 = CPC, the workflow simplifies to "pull out, push in" with no lever operation.
- The document does not model the CPC workflow or compare it against the cam lever at equivalent detail.
- The access-architecture finding (slide-out tray) could significantly improve the cartridge swap experience regardless of fitting choice -- the user pulls the enclosure out into the light before swapping.

**What needs to change:**
- Present workflows for each DP3 option: cam lever, CPC, hand disconnect.
- Incorporate the slide-out tray finding -- if the enclosure is on a pull-out tray, the user works in full visibility and light, which changes the ergonomic analysis significantly.
- Time estimates should be recalculated for CPC workflow.
- The "clean before remove" section remains valid regardless of mechanism choice.

**Dependencies:** DP3, access architecture choice.

---

#### dock-mounting-strategies.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Dock position at ~226mm from floor is derived from the current zone layout (1L bags at 18 degrees in 176mm bag zone). With corrected bag dimensions and a different angle, the dock shelf position shifts.
- Lever clearance zone (40mm above cartridge) is unnecessary if DP3 = CPC or press-fit.
- If enclosure depth increases to 300mm, the dock shelf depth and tube routing behind the dock both gain space.

**What needs to change:**
- Dock shelf position must be recalculated based on corrected bag zone geometry.
- Lever clearance section should be conditional on DP3.
- If depth increases to 300mm, the "tube routing behind dock" section gains ~50mm of clearance.
- The structural shelf argument (ties side walls together, DIN rail mounting, zone separation) remains valid regardless.

**Dependencies:** DP1, DP2, DP3.

---

#### under-cabinet-ergonomics.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Cartridge slot at ~226mm is derived from current zone layout (will shift).
- Does not consider the slide-out tray option from access-architecture research, which fundamentally changes the ergonomic analysis -- user pulls enclosure into the light instead of reaching into darkness.
- Cabinet depth analysis (22 inches / 559mm) aligns with the new under-sink-constraints finding (480-510mm usable depth), but the document does not leverage the finding that 300mm enclosure depth is easily accommodated.

**What needs to change:**
- Incorporate the slide-out tray option as a primary ergonomic improvement.
- Update cartridge slot height once bag zone geometry is recalculated.
- Add the finding that depth is unconstrained (480-510mm available vs. 250-300mm needed).
- Reach calculations should be revisited with the slide-out tray in consideration.

**Dependencies:** DP1, DP2 (dock position), access architecture choice.

---

#### requirements.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- Defines three sub-problems (guide/align, seat fluid connections, secure/release) that are specific to the John Guest + cam lever architecture.
- If DP3 = CPC, the three sub-problems collapse: CPC couplings handle connection, disconnection, and sealing in a single mechanism. The "secure feel and release" sub-problem evaporates.
- Still references "4 John Guest 1/4" push-to-connect fittings" as if this is the only option.

**What needs to change:**
- Present the sub-problems as conditional on fitting choice.
- Add CPC and press-fit as fitting options with their own sub-problem breakdown.
- The functional requirements (slide-in, auto-connect, clear docked/released states) remain valid at a higher level regardless of fitting choice.

**Dependencies:** DP3.

---

### Top-Level Hardware Documents

---

#### bag-zone-geometry.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- **Every number in this document is wrong.** Bag dimensions of 250mm x 140mm should be 280mm x 152mm. The 18-degree geometry table, the two-bag stacking arrangement, the 238mm horizontal run, the 242mm interior depth fit -- all are based on incorrect inputs.
- The 18-degree layout does not fit: 280mm bag at 18 degrees needs 282mm of depth, but only 242mm is available.
- The enclosure depth of 250mm (242mm interior) is identified as unnecessarily constrained by the new under-sink research.
- The lever clearance zone (266-306mm, 40mm) may not be needed if DP3 eliminates the cam lever.

**What needs to change:**
- Complete recalculation with corrected bag dimensions (L=280mm, W=152mm for 1L; L=350mm, W=190mm for 2L).
- Present scenarios: (a) 300mm depth enclosure with 1L bags at 30-35 degrees, (b) 300mm depth with 1L bags at 42 degrees if 250mm depth is kept, (c) 2L bags require ~320x300x450mm minimum.
- Zone height budget must be recalculated for each scenario.
- The lever clearance zone should be conditional on DP3.
- Mounting hardware section (U-clips, binder clips) should be revisited for steeper angles where axial forces increase.
- The diagram and dimension summary table are the most-referenced artifacts in the project -- they must be correct.

**Dependencies:** DP1, DP2, DP3.

---

#### dimensions-reconciliation.md

**Status:** 🔄 Needs conditional rewrite

**What is wrong:**
- States 280x250x400mm as "locked" -- this was never formally decided and is now challenged by the depth constraint finding.
- Bag dimensions table states 1L bags as "~250mm long, ~140mm wide" (wrong: 280mm x 152mm).
- 2L bag length stated as "250-300mm (corrected from 350mm)" -- the "correction" was wrong, 350mm is the actual dimension.
- Incline mount geometry section (2h) uses wrong bag length for all calculations.
- Uses "locked" framing that discourages reconsideration of values that are now known to be incorrect.

**What needs to change:**
- Correct all bag dimensions to manufacturer-stated values.
- Remove "locked" framing for undecided values; clearly mark what is decided vs. assumed.
- Present enclosure dimensions as conditional on DP1 outcome, not as resolved.
- Recalculate incline mount geometry with corrected dimensions.
- The cartridge envelope section (2c), mating face section (2e), and depth budget section (2f) remain valid if the cartridge design is unchanged.

**Dependencies:** DP1, DP2.

---

#### bill-of-materials.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Lists 4 John Guest push-connect fittings for the dock wall ($10). If DP3 = CPC, these become 4 CPC coupling pairs (~$70), and all release-mechanism-related hardware (dowel pins, push rod, release plate) drops out.
- Lists 6 solenoid valves -- this depends on DP4 and is not affected by the new findings.
- Enclosure material quantities assume 280x250x400mm. If depth increases to 300mm, slightly more filament is needed.
- Platypus bags listed as "2L Collapsible Bottle" (OWNED) -- this is correct and the 2L bags in hand should be measured.

**What needs to change:**
- Fitting line items should be conditional on DP3.
- If CPC chosen: add ~$70 for CPC couplings, remove release mechanism hardware (dowel pins, push rod material).
- If enclosure dimensions change, update material estimates.
- The BOM should be the LAST document updated after decisions are made.

**Dependencies:** DP1, DP3, DP4.

---

#### gpio-planning.md

**Status:** ✅ Still valid

**What is wrong:** Nothing from the new findings. GPIO assignments, I2C bus topology, MCP23017 planning, and FDC1004 channel assignments are all independent of bag dimensions, enclosure depth, and fitting choice.

**What needs to change:** If DP4 simplifies (fewer solenoids), some planned GPIO allocations become unnecessary. But this is a DP4 dependency, not a new-research dependency.

**Dependencies:** DP4 (solenoid count).

---

### Decision Point Documents (`hardware/decision-points/`)

---

#### dp1-enclosure-dimensions.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Option A (280x250x400mm) is now known to not fit 1L bags at 18 degrees. The document presents it as "all recent research is built around this, no documents need updating" -- but this is now false.
- Does not include a depth-increase option (e.g., 280x300x400mm), which the new research shows is the easiest fix for the bag fit problem at zero practical cost.
- Does not reference the under-sink-constraints finding that 480-510mm of depth is available.

**What needs to change:**
- Add an option for increased depth (280x300x400mm or 280x300x450mm).
- Note that Option A is now known to fail for 1L bags at 18 degrees. It can be rescued by increasing the angle to 42+ degrees, but this is a significant change from the current research.
- Incorporate the finding that depth is unconstrained.

**Dependencies:** New research (bag-dimensions-survey, under-sink-constraints, diagonal-stacking-geometry).

---

#### dp2-bag-strategy.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Option A dimensions ("~250mm long, 140mm wide") are wrong.
- Option D (2L bags incline-mounted) notes that 2L bag length is "approximately 350mm" and width "190mm" -- the length is correct but the document does not calculate the actual geometry to determine feasibility.
- Does not incorporate the diagonal-stacking-geometry finding that 2L bags require a minimum ~320x300x450mm enclosure.
- Does not note that 1L bags at 18 degrees do not fit the current enclosure.

**What needs to change:**
- Correct all bag dimension references.
- Add the finding that 1L bags at 18 degrees do not fit the current 250mm-depth enclosure.
- Present corrected feasibility analysis for each option using actual dimensions.
- Note that 2L bags in a 400mm enclosure are not viable in any arrangement (horizontal zones or diagonal) -- they need 450mm minimum.
- The "conflicts in the current documentation" section should note the dimension errors.

**Dependencies:** New research (bag-dimensions-survey, diagonal-stacking-geometry).

---

#### dp3-cartridge-dock-interface.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Option C (CPC) prices CPC at "$10-15 per coupling x 4 = $40-60" -- the new fitting-alternatives research prices CPC PLC NSF valved at ~$17-18 per pair, ~$70 for 4 connections, and adds critical details about NSF 169 certification and auto-shutoff.
- Does not include press-fit fittings as an option.
- Does not adequately weigh the engineering simplification of eliminating the release plate, push rod, and lever.

**What needs to change:**
- Update CPC pricing and specifications from fitting-alternatives.md.
- Add press-fit fittings as an option (Option F or merged into existing options).
- Reweight the cost-vs-simplification analysis.
- Note that CPC is the only option with auto-shutoff (zero dripping during swap), which has product value for under-sink installation.

**Dependencies:** New research (fitting-alternatives).

---

#### dp4-fluid-path-topology.md

**Status:** ✅ Still valid

**What is wrong:** Nothing from the new findings. The fluid path topology options (pump reversal, gravity fill, manual swap, MVP dispensing only) are independent of bag dimensions, enclosure depth, and fitting choice.

**What needs to change:** Nothing from these specific findings. The existing caveats about untested pump reversal and theoretical fill percentages remain the main concerns.

**Dependencies:** None from new research.

---

#### assumption-audit.md

**Status:** ✅ Still valid (partially vindicated)

**What is wrong:** Nothing -- the assumption audit correctly identified many of the issues that the new research confirmed. The bag dimension uncertainty, the unquestioned 250mm depth, the horizontal zone assumption, and the fitting choice lock-in were all flagged.

**What needs to change:** Could add a note that several of its warnings have been confirmed by the new research (bag dimensions were wrong, depth was unconstrained, diagonal/steeper incline options are viable).

**Dependencies:** None.

---

#### layout-alternatives-research.md

**Status:** ⚠️ Needs update

**What is wrong:**
- Uses corrected 2L bag dimensions but uses estimated 1L dimensions (250mm length) in some places.
- The diagonal stacking math is correct for 2L bags but should also present the corrected 1L geometry.
- The "Key Geometry Facts" table lists "2L Platypus bag width" as "~140mm" in the original -- this needs checking against the corrected 190mm.

**What needs to change:**
- Verify all bag dimensions against bag-dimensions-survey.md.
- Incorporate the diagonal-stacking-geometry finding that the existing 18-degree design is broken.
- The document's core insight (diagonal interleave uses the enclosure diagonal) remains valid and is strengthened by the finding that horizontal zones cannot fit 2L bags.

**Dependencies:** New research (bag-dimensions-survey, diagonal-stacking-geometry).

---

#### visions/diagonal-interleave.md

**Status:** ✅ Still valid

**What is wrong:** Nothing fundamental. This is a conceptual vision document, not a design specification. The diagonal concept is validated by the diagonal-stacking-geometry research.

**What needs to change:** Could note the new research confirmation that diagonal is the only way to fit 2L bags in a 400mm enclosure, per the diagonal-risks and diagonal-stacking-geometry findings. The vision is stronger than when written.

**Dependencies:** None.

---

#### README.md (decision-points)

**Status:** ✅ Still valid

**What is wrong:** Nothing. The README correctly identifies the 4 decision points, their dependency chain, and the known conflicts. The new research adds evidence but does not change the decision structure.

**What needs to change:** Could add a reference to the new research documents and note that the bag dimension error has been confirmed.

**Dependencies:** None.

---

## Priority Order for Rewrites

The following documents should be updated in this order, based on dependency chain:

### Tier 1: Foundation documents (block everything else)

1. **bag-zone-geometry.md** -- Every spatial calculation depends on this. Must be recalculated with correct dimensions and presented conditionally.
2. **dimensions-reconciliation.md** -- Authoritative dimension table must be corrected.

### Tier 2: Decision point updates (inform decisions)

3. **dp1-enclosure-dimensions.md** -- Must add depth-increase option.
4. **dp2-bag-strategy.md** -- Must correct dimensions and feasibility analysis.
5. **dp3-cartridge-dock-interface.md** -- Must update CPC analysis and add press-fit option.

### Tier 3: Research documents with wrong numbers

6. **incline-bag-mounting.md** -- Core geometry recalculation.
7. **layout-spatial-planning.md** -- Master layout conditional rewrite.
8. **hopper-and-bag-management.md** -- Dimension corrections and angle updates.
9. **layout-alternatives-research.md** -- Dimension verification.

### Tier 4: Conditionally affected documents (wait for DP3)

10. **cartridge-change-workflow.md** -- Conditional on fitting choice.
11. **mating-face.md** -- Conditional on fitting choice.
12. **requirements.md** -- Conditional on fitting choice.
13. **front-face-interaction-design.md** -- Conditional on fitting choice.
14. **release-mechanism-alternatives.md** -- Needs CPC/press-fit update.

### Tier 5: Minor updates

15. **dock-mounting-strategies.md** -- Position recalculation.
16. **cartridge-envelope.md** -- Conditional dimensions.
17. **under-cabinet-ergonomics.md** -- Add slide-out tray, depth finding.
18. **back-panel-and-routing.md** -- Dimension correction.
19. **bill-of-materials.md** -- Update last, after decisions.

### Tier 6: Potentially obsolete (wait for DP3 decision)

20. **cam-lever.md** -- Void if DP3 != John Guest + cam lever.
21. **collet-release.md** -- Void if DP3 != John Guest + cam lever.
22. **release-plate.md** -- Void if DP3 != John Guest + cam lever.

---

## Summary Table

| Document | Status | Primary Issue | Blocking Decision |
|---|---|---|---|
| bag-zone-geometry.md | 🔄 Conditional rewrite | All dimensions wrong | DP1, DP2, DP3 |
| dimensions-reconciliation.md | 🔄 Conditional rewrite | Bag dims wrong, "locked" framing | DP1, DP2 |
| incline-bag-mounting.md | ⚠️ Needs update | Bag dims wrong, angle wrong | DP1, DP2 |
| layout-spatial-planning.md | 🔄 Conditional rewrite | All zones assume wrong bag geometry | DP1, DP2, DP3 |
| dip-tube-analysis.md | ✅ Still valid | Minor angle reference | None |
| drip-tray-shelf-analysis.md | ✅ Still valid | Minor pressure note at steeper angles | None |
| pump-assisted-filling.md | ✅ Still valid | None from new findings | DP4 |
| hopper-and-bag-management.md | ⚠️ Needs update | Bag dims wrong, angle wrong | DP1, DP2, DP4 |
| front-face-interaction-design.md | 🔄 Conditional rewrite | Lever may not exist | DP3 |
| back-panel-and-routing.md | ⚠️ Needs update | Dimension inconsistency | DP1 |
| cam-lever.md | ❌ Potentially obsolete | Void if CPC/press-fit chosen | DP3 |
| collet-release.md | ❌ Potentially obsolete | Void if CPC/press-fit chosen | DP3 |
| release-plate.md | ❌ Potentially obsolete | Void if CPC/press-fit chosen | DP3 |
| release-mechanism-alternatives.md | ⚠️ Needs update | Missing CPC detail, no press-fit | DP3 |
| mating-face.md | 🔄 Conditional rewrite | Assumes JG + release plate | DP3 |
| electrical-mating.md | ✅ Still valid | Minor engagement sequence note | DP3 (minor) |
| guide-alignment.md | ✅ Still valid | May simplify with CPC | DP3 (minor) |
| pump-mounting.md | ✅ Still valid | More internal space if no push rod | DP3 (minor) |
| cartridge-envelope.md | ⚠️ Needs update | Envelope conditional on DP1, DP3 | DP1, DP3 |
| cartridge-change-workflow.md | 🔄 Conditional rewrite | Only models cam lever workflow | DP3 |
| dock-mounting-strategies.md | ⚠️ Needs update | Dock position shifts, lever clearance conditional | DP1, DP2, DP3 |
| under-cabinet-ergonomics.md | ⚠️ Needs update | Missing slide-out tray, depth finding | DP1, DP2 |
| requirements.md | 🔄 Conditional rewrite | Sub-problems specific to JG fittings | DP3 |
| dp1-enclosure-dimensions.md | ⚠️ Needs update | Missing depth-increase option | New research |
| dp2-bag-strategy.md | ⚠️ Needs update | Wrong dimensions, missing feasibility | New research |
| dp3-cartridge-dock-interface.md | ⚠️ Needs update | CPC underpriced, no press-fit option | New research |
| dp4-fluid-path-topology.md | ✅ Still valid | None from new findings | None |
| assumption-audit.md | ✅ Still valid | Vindicated by new research | None |
| layout-alternatives-research.md | ⚠️ Needs update | Some dimensions may be wrong | New research |
| visions/diagonal-interleave.md | ✅ Still valid | Strengthened by new research | None |
| decision-points/README.md | ✅ Still valid | None | None |
| gpio-planning.md | ✅ Still valid | None from new findings | DP4 |
| bill-of-materials.md | ⚠️ Needs update | Fitting costs conditional on DP3 | DP1, DP3, DP4 |
