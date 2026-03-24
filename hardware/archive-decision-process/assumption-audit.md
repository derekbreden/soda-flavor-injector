# Assumption Audit: Cross-Pollination Gaps Across Research Documents

Every research document is listed below. For each one:
- **Baked-in assumptions** — things treated as given that are actually open questions
- **Blind spots** — possibilities from sibling documents that would change this doc's conclusions
- **What changes if...** — specific ways this doc's conclusions shift under different DP outcomes

---

## Cartridge Research (`hardware/cartridge/planning/`)

### requirements.md

**Baked-in assumptions:**
- Front-loading slide-in cartridge (never considers top-loading, side-loading, or drawer-style)
- 4 John Guest 1/4" push-to-connect fittings (never considers CPC couplings, barb fittings, O-ring compression, or dry-break connectors)
- Lever mechanism with locked/released states (never considers no-lever designs like CPC pull-disconnect)
- Collet release requires even concentric pressure (true for John Guest, irrelevant if fittings change)

**Blind spots:**
- `release-mechanism-alternatives.md` evaluated CPC couplings and found them viable but expensive — requirements.md doesn't acknowledge this possibility space
- If DP3 goes to CPC couplings, half the requirements (collet release, lever states, concentric pressure) evaporate
- If DP3 goes to hand disconnect, the "single motion" requirement drops out

**What changes if:**
- DP3 ≠ cam lever → The three sub-problems (guide/align, seat fluid, secure/release) collapse to two or even one. Requirements doc needs to be rewritten as conditional on interface choice.
- DP4 = bags-in-cartridge → Cartridge requirements expand massively (must hold bags + pumps, much heavier, different form factor entirely)

---

### cam-lever.md

**Baked-in assumptions:**
- John Guest fittings are the connection method (entire doc is irrelevant if fittings change)
- 4-8 lbs total release force (derived from collet-release.md estimates, which are inferred not measured)
- 2-3mm collet displacement (same — inferred, not measured)
- Lever is on the cartridge front face (never considers dock-side actuation)

**Blind spots:**
- If DP3 = CPC couplings, this entire document is unnecessary
- If DP3 = hand disconnect, this entire document is unnecessary
- `dock-mounting-strategies.md` places the lever on the cartridge but never explores dock-side lever actuation, which could simplify cartridge design
- No consideration of motorized/servo actuation (the cartridge-change-workflow.md mentions servo-actuated latch as a possibility but cam-lever.md doesn't explore motorized release)

**What changes if:**
- DP3 ≠ cam lever → Document becomes historical reference only
- John Guest collet force/displacement differs from estimates → Eccentricity value, mechanical advantage calculations, and over-center analysis all change
- Lever moves to dock side → Push rod routing through cartridge interior is eliminated, but dock becomes more complex

---

### cartridge-change-workflow.md

**Baked-in assumptions:**
- Eccentric cam lever + release plate is the baseline (entire workflow modeled around it)
- 19-25 second swap time is the target (derived from this specific mechanism)
- Clean cycle runs before removal (software enforced) — treats this as mandatory
- User kneels at cabinet opening (assumes under-cabinet mounting; never considers countertop or wall-mount enclosure placement)

**Blind spots:**
- Swap time comparison (19-25s vs 75-135s for hand disconnect) assumes the cam lever mechanism works as designed — no acknowledgment that the release plate is unproven and could fail, making actual swap time worse
- Never considers that CPC couplings could achieve similar speed (~15-20s) without any custom mechanism
- Cartridge swaps are an 18-36 month maintenance event (peristaltic pump tube wear), not a regular operation. Bag refills are separate (weekly to monthly depending on usage). The doc never separates these two very different interactions.
- Because swaps are infrequent, the UX priority shifts: the mechanism must be *obvious and foolproof after 2 years of not doing it*, not fast. A self-explanatory single-motion mechanism (cam lever, CPC pull-disconnect) may matter more than raw speed. Hand disconnect (4 sequential operations you'd have to re-learn from a manual) becomes *worse* in this light, not better.

**What changes if:**
- DP3 = CPC → Workflow simplifies dramatically: pull out, push in. No lever, no release plate concerns.
- DP3 = hand disconnect → Workflow is slower but has zero custom parts to fail
- DP4 = in-place refill works → Cartridge swaps become maintenance events (yearly?), not regular operations. Swap speed becomes nearly irrelevant.
- DP4 = manual bag swap → Bag removal/reinstallation becomes the frequent workflow, not cartridge swap. This doc should model bag swap ergonomics, not just cartridge swap.

---

### cartridge-envelope.md

**Baked-in assumptions:**
- Side-by-side pump arrangement is "the ONLY viable option" (true at 80mm height, but 80mm is derived from 400mm enclosure → zone budgets → dock height, which traces back to DP1)
- 150 x 80 x 130mm envelope (entirely derived from current zone budget)
- Electrical contacts on TOP face, fluid on REAR face (reasonable but never explores alternatives like all-rear or all-bottom)
- 2 Kamoer KPHM400 pumps specifically (never considers different pump models, smaller pumps, or different pump counts)

**Blind spots:**
- If DP1 = 450mm enclosure, dock zone gets taller, cartridge height could be 100-110mm, and stacked pump arrangement might become viable — this doc calls stacked "impossible" but that's only true at current height
- `pump-mounting.md` designs around side-by-side but acknowledges stacked would be preferable for tube routing if height allowed
- Different pumps (e.g., smaller Kamoer models, or non-peristaltic pumps) would change the entire envelope calculation
- If DP3 = CPC couplings, the depth budget changes (no release plate, no tube stubs protruding through plate)

**What changes if:**
- DP1 = taller enclosure → Cartridge height budget increases. Stacked pumps become viable. Envelope could be narrower and taller (e.g., 100W x 110H x 110D). Completely different internal layout.
- DP3 = CPC couplings → Rear face depth budget shrinks (no release plate = ~6mm saved, no tube stub protrusion through plate). Cartridge could be shallower.
- DP3 = hand disconnect → No push rod routing needed. Internal cartridge layout simplifies.
- Different pump model → Entire envelope calculation must restart from scratch.

---

### collet-release.md

**Baked-in assumptions:**
- John Guest 1/4" push-to-connect is the fitting type (entire doc is fitting-specific)
- Collet dimensions are inferred from patents and tool geometry, NOT measured on physical fittings
- 1.5-2.0mm collet travel (inferred)
- 2-5N release force per fitting (inferred)
- Stepped bore geometry is required (true for John Guest, irrelevant otherwise)

**Blind spots:**
- This document's entire existence depends on DP3 = John Guest fittings with release plate. If DP3 goes any other direction, this doc is void.
- `release-mechanism-alternatives.md` identifies that CPC couplings eliminate the collet problem entirely
- No consideration of metric push-to-connect fittings (SMC, Festo) which have different collet geometry and may be easier to release
- No consideration of quick-connect fittings designed for tool-less disconnect (they exist in beverage industry)

**What changes if:**
- DP3 ≠ John Guest release plate → Document becomes irrelevant
- Physical measurement of actual John Guest fittings reveals different dimensions → Stepped bore geometry, release plate thickness, and cam lever eccentricity all need recalculating
- Beverage-industry quick-connects are used → No collet release problem at all

---

### dock-mounting-strategies.md

**Baked-in assumptions:**
- Dock is at a specific vertical position (~226mm from floor) derived from current zone budget
- Dock is a structural shelf spanning full enclosure depth
- Cartridge slot opening is centered at one specific height
- Three-zone vertical layout (electronics top, dock middle, bags bottom)

**Blind spots:**
- Never considers bags-above-dock layout (bags on top, dock on bottom) — which would be viable if gravity fill is used (DP4 Option B) and might improve ergonomics (dock lower = easier reach)
- Never considers that if DP1 changes, the dock position changes significantly
- `under-cabinet-ergonomics.md` says 226mm is comfortable, but that analysis assumed the current zone layout, not alternatives
- If DP4 = bags-in-cartridge, there's no separate bag zone and the dock could be anywhere

**What changes if:**
- DP1 = different height → Dock position shifts. All ergonomic analysis in under-cabinet-ergonomics.md needs redoing.
- DP2 = different bag strategy → Zone heights change. Dock shelf position moves.
- DP2 = external bags → Bag zone disappears from enclosure. Dock could be at floor level. Enclosure shrinks dramatically.
- DP4 = bags-in-cartridge → No bag zone. Dock is the dominant zone.
- Zone layout inverted (bags top, dock bottom) → Dock is lower, possibly easier to reach, but gravity fill from hopper becomes harder.

---

### electrical-mating.md

**Baked-in assumptions:**
- 3 contacts only (GND, Motor A+, Motor B+)
- Electrical on TOP face, separate from fluid on REAR face
- Pogo pins in dock ceiling pressing down onto cartridge top pads

**Blind spots:**
- If the product adds sensors in the cartridge (temperature, flow, NFC tag for flavor identification), contact count increases and pogo pin design changes
- Never considers wireless power or data (BLE from cartridge MCU, inductive coupling) — extreme but worth noting as possibility space
- If DP3 = bayonet/twist-lock, contacts could be ring-style (like headphone jacks) rather than linear pads
- `gpio-planning.md` mentions cartridge detection via MCP23017 interrupt — this implies a detection contact might be needed (4th pin), which this doc doesn't plan for

**What changes if:**
- Cartridge gets an NFC/RFID tag for flavor identification → Additional contact or antenna needed
- DP3 = different interface → Contact geometry changes (not necessarily top-face pogo pins)
- Product adds cartridge-side sensing → More contacts needed, different connector
- DP1 = different dimensions → Dock ceiling height changes, pogo pin travel requirements change

---

### guide-alignment.md

**Baked-in assumptions:**
- Front-loading slide-in motion (rails parallel to insertion axis)
- Rectangular profile rails with FDM 3D printing tolerances
- Two-stage alignment: coarse rails + fine tapered pins

**Blind spots:**
- If DP3 = CPC couplings, alignment requirements are different (couplings are more tolerant of misalignment than John Guest fittings with a release plate)
- If DP3 = bayonet, alignment is rotational not linear
- Never considers alignment via the fittings themselves (CPC couplings can be self-aligning with tapered noses)
- `front-face-interaction-design.md` proposes display holders with magnetic retention — similar magnetic alignment could apply to cartridge (noted in DP3 Option D but not explored in this doc)
- Rail printing orientation constraints assume PETG FDM; SLA, SLS, or injection molding changes all tolerance assumptions

**What changes if:**
- DP3 = CPC → Alignment tolerance relaxes significantly. Simpler rails, possibly no tapered pins needed.
- DP3 = magnetic → Self-aligning. Rails become optional guides, not precision features.
- Production method changes from FDM to injection molding → All tolerance numbers change.

---

### mating-face.md

**Baked-in assumptions:**
- Tube stubs protrude from rear face through release plate into dock fittings
- 2x2 grid at 15mm center-to-center (derived from John Guest fitting body size)
- Eccentric cam lever on front face drives push rod to rear face
- Tapered pins at corners for fine alignment
- Asymmetric rail profile for poka-yoke

**Blind spots:**
- This is the most assumption-heavy document — it integrates conclusions from cam-lever, collet-release, release-plate, electrical-mating, and guide-alignment as if all are decided
- If ANY of those change, mating-face must be substantially rewritten
- Never considers a mating face with CPC couplings (which would have protruding coupling halves instead of tube stubs, no release plate, no push rod)
- Never considers a mating face with no rear fluid connections at all (e.g., if fluid connects from the bottom of the cartridge via drop-in ports)

**What changes if:**
- DP3 = anything other than cam lever + release plate → Entire rear face design changes. Push rod eliminated. Release plate eliminated. Tube stub geometry changes or is eliminated.
- DP3 = CPC → Mating face becomes: CPC inserts protruding from rear, pogo pads on top. Much simpler.
- 15mm C-C spacing proves too tight for printed parts → Fallback to 18mm C-C changes the port zone from 33.5mm to 39.5mm square. Cascades to release plate, cartridge width.

---

### pump-mounting.md

**Baked-in assumptions:**
- Side-by-side arrangement is mandatory (inherits from cartridge-envelope.md's height constraint)
- Heat-set M3 inserts in PETG tray
- Tray + shell assembly approach
- BPT to 1/4" hard tube transition via brass barb fittings

**Blind spots:**
- If DP1 allows taller cartridge, stacked arrangement becomes viable and this doc's layout is wrong
- Never considers vibration-absorbing dock mounting (isolate at dock level rather than per-pump inside cartridge)
- `back-panel-and-routing.md` mentions passive ventilation is adequate, but pump heat dissipation inside a sealed cartridge shell isn't analyzed
- If different pumps are used (smaller, quieter, different form factor), entire mounting design changes

**What changes if:**
- DP1 = taller enclosure → Stacked pump arrangement viable. Tray geometry totally different.
- Different pump model → Start over.
- DP3 = no push rod → More internal space available. Tube routing simplifies.

---

### release-mechanism-alternatives.md

**Baked-in assumptions:**
- Retains eccentric cam + release plate as "optimal" (conclusion, not neutral analysis)
- Rejects CPC primarily on cost ($40-60) without weighing the engineering simplification
- Rejects hand disconnect on UX speed, but the real concern for an 18-36 month maintenance event is intuition-after-long-gap, not speed. Hand disconnect actually fares *worst* on that metric (4 sequential operations to re-learn), while CPC (just pull) and cam lever (single obvious flip) both fare well.

**Blind spots:**
- Cost analysis is narrow: $40-60 for CPC couplings vs. $5 for John Guest fittings, but doesn't account for engineering time to design/test/iterate the release plate, cam lever, push rod, and stepped bore tolerances
- Never considers total cost of ownership: if the release plate mechanism fails in the field, repair cost + user frustration may exceed the $40-60 CPC premium
- Cartridge swaps and bag refills are fundamentally different operations (18-36 month pump replacement vs. weekly/monthly concentrate top-up), but this doc lumps them together when evaluating mechanism importance
- Never considers hybrid approaches (e.g., 2 CPC couplings for output lines + 2 John Guest for input lines, or CPC for fluid + pogo for electrical)

**What changes if:**
- Product context (selling to consumers) → Cartridge swaps are 18-36 month maintenance events. The interface must be intuitive after years of not doing it. This favors self-explanatory mechanisms (cam lever's single obvious flip, CPC's pull-to-disconnect) and *disfavors* hand disconnect (4 sequential operations requiring re-learned technique). Speed is secondary to intuition and reliability.
- Cost tolerance is higher → CPC becomes clearly preferable (proven, no custom parts, both sides auto-seal). The $40-60 premium is trivial against the cost of engineering, testing, and field-supporting a custom release plate mechanism.

---

### release-plate.md

**Baked-in assumptions:**
- Stepped bore geometry is required (true only for John Guest fittings)
- 6mm plate thickness, specific bore diameters (derived from collet-release.md's inferred dimensions)
- PETG 3D print is acceptable for prototype
- 2x2 grid at 15mm C-C

**Blind spots:**
- Entire document is void if DP3 ≠ John Guest + release plate
- Bore dimensions are inferred from patents, not measured — if wrong, the plate doesn't work
- `collet-release.md` acknowledges this uncertainty but release-plate.md builds on the numbers as solid
- No fallback design if stepped bores don't achieve clean collet release

**What changes if:**
- DP3 ≠ release plate → Document is void
- Physical John Guest measurements differ from inferred values → All bore dimensions, plate thickness, and cam stroke need recalculating
- 3D print circularity is inadequate → Must machine the plate (adds cost, changes prototyping approach)

---

### under-cabinet-ergonomics.md

**Baked-in assumptions:**
- Standard US 36" sink base cabinet (one country, one cabinet style)
- Enclosure sits on cabinet floor (never considers shelf-mounted, wall-mounted, or suspended)
- Cartridge slot at ~226mm from enclosure floor (derived from current zone layout)

**Blind spots:**
- Product for sale means diverse installation environments: different countries, different cabinet standards, freestanding vs. built-in, different plumbing configurations
- Never considers adjustable mounting height or adjustable feet
- Never considers that the enclosure could sit on a shelf inside the cabinet (raising the base height, changing all reach calculations)
- `front-face-interaction-design.md` designs the front panel for a specific height relationship that would change if the enclosure is elevated

**What changes if:**
- DP1 = different dimensions → All ergonomic reach calculations change
- DP2 = different zone layout → Cartridge slot height changes, all ergonomic analysis changes
- Product targets multiple markets → Need to accommodate range of cabinet sizes, not just US 36" standard
- Enclosure is wall-mounted or shelf-mounted → Fundamentally different ergonomic constraints

---

## Enclosure Research (`hardware/enclosure/research/`)

### back-panel-and-routing.md

**Baked-in assumptions:**
- 280x250x400mm enclosure (DP1 assumed)
- Layered architecture: bags bottom, dock middle, electronics top
- 3 water bulkhead fittings at bottom, power/signal at top
- Passive ventilation is adequate (peak ~33W)
- Cat6 cable for display connections

**Blind spots:**
- If DP1 changes, all panel dimensions and fitting positions change
- If DP2 = external bags, water routing is completely different (longer runs to external bags)
- If DP4 = simplified (no hopper, no clean), several bulkhead fittings and routing paths are eliminated
- Never considers wireless display connection (ESP-NOW, BLE) which would eliminate display cable routing entirely
- `gpio-planning.md` already uses BLE on the ESP32-S3 — extending this to eliminate physical display cables is plausible

**What changes if:**
- DP1 = different dimensions → Panel redesign
- DP4 = simplified → Fewer bulkhead fittings, simpler routing
- Wireless displays → Cable management section becomes irrelevant

---

### dip-tube-analysis.md

**Baked-in assumptions:**
- Platypus Drink Tube Kit is the connection method (specific product)
- Bag orientation is vertical or incline with connector at bottom
- 28mm threaded cap seal is adequate

**Blind spots:**
- Never considers alternative bag-to-tube connection methods (e.g., Camelbak Quick Link, custom welded ports, bag-in-box style taps)
- Never considers rigid containers instead of bags (bottles, tanks, bag-in-box)
- `hopper-and-bag-management.md` depends heavily on this doc's sealed-path conclusion, but that conclusion is specific to the Platypus product — a different container would need its own analysis
- If the product uses a different reservoir (not Platypus bags), this entire document is irrelevant

**What changes if:**
- Different reservoir type → Document must be rewritten or discarded
- Different bag brand/model → Dip tube geometry, cap seal, and threading all change
- Rigid container instead of bag → No collapse behavior, no dip tube needed (gravity feed or pressurized)

---

### drip-tray-shelf-analysis.md

**Baked-in assumptions:**
- Bags are permanent (refilled in place, never removed) — true only if DP4 = in-place refill
- No condensation (no refrigeration) — true for current design, but never considers future refrigeration
- Clean-before-remove enforcement prevents concentrate exposure

**Blind spots:**
- If DP4 = manual bag swap, bags ARE removed regularly, and drip containment during bag handling becomes important
- If DP4 = MVP dispensing only, there's no clean cycle, so concentrate IS present in lines during removal
- Recommends removing the drip tray, but if bags are swapped manually, liquid will drip during disconnection
- `cartridge-change-workflow.md` acknowledges "residual water drips from stub tips" — without a drip tray, this drips onto bags below

**What changes if:**
- DP4 = manual bag swap → Drip tray removal is questionable. Bags get handled, liquid drips, containment matters.
- DP4 = no clean cycle → Concentrate (not just water) is in lines. Drip is sticky/staining. Containment is more important.
- Product context → Consumer spills and messes are warranty/support issues. Internal drip containment may be worth the space cost.

---

### front-face-interaction-design.md

**Baked-in assumptions:**
- Two round displays (S3 48x48x33mm, RP2040 33mm diameter)
- Cartridge loads from front face with cam lever handle
- Dark navy (#1a1a2e) branding
- Layout F is the product-quality target

**Blind spots:**
- If DP3 ≠ cam lever, there's no lever handle on the front face — the entire front panel aesthetic changes
- If DP3 = CPC pull-disconnect, the cartridge face might just be a flat panel with a pull handle (much simpler)
- Never considers a single larger display instead of two small round ones
- Never considers no physical displays at all (phone-only interface via BLE)
- `gpio-planning.md` shows BLE is already on the S3 — a phone-as-display approach would eliminate two displays, all cable routing, and the front panel display cutouts

**What changes if:**
- DP3 = no lever → Front face design simplifies (no lever clearance zone, different handle/grip)
- Phone-only interface → Front panel is just the cartridge opening. Much simpler. Saves ~$30-50 in display hardware.
- Single display → Different layout options entirely

---

### hopper-and-bag-management.md

**Baked-in assumptions:**
- Bags are refilled in place via hopper (DP4 assumed = in-place refill)
- Two funnels, one per flavor
- Pump-assisted filling via pump reversal
- FDC1004 capacitive sensing for level detection
- 1L Platypus bags (DP2 assumed)
- Incline mounting at 18-20° (DP2 assumed)

**Blind spots:**
- `pump-assisted-filling.md` explicitly corrects parts of this document, but this document wasn't updated
- If DP4 = manual bag swap, this entire hopper design is unnecessary
- If DP4 = MVP dispensing only, no hopper needed at all
- Never considers a single larger hopper with a valve to direct flow to either bag (vs. two separate funnels)
- Never considers pre-mixed concentrate cartridges or pods (like a Keurig model) that would eliminate bags entirely
- Capacitive sensing adds firmware complexity and a dedicated I2C chip — simpler approaches (float switch, weight sensor) aren't compared

**What changes if:**
- DP4 = manual bag swap → Hopper system is eliminated entirely
- DP4 = gravity fill → Hopper design is simpler (no pump reversal, no sealed hopper, just a funnel with a valve)
- DP2 = 2L bags → Hopper capacity and fill time calculations change
- Different sensing approach → gpio-planning.md FDC1004 allocation changes

---

### incline-bag-mounting.md

**Baked-in assumptions:**
- 1L Platypus bags (DP2 assumed)
- 400mm enclosure height (DP1 assumed)
- 165mm bag zone height (derived from DP1 + DP2)
- 18-20° incline is the sweet spot
- Incline was explored as a solution to fit 1L bags in a constrained zone

**Blind spots:**
- **The biggest missed opportunity in this doc:** Incline mounting could also enable 2L bags in a 400mm enclosure — especially without a drip tray freeing up floor space. A 2L Platypus bag is ~350mm long; at a shallow incline (say 10-15°), the vertical rise would be ~60-90mm, meaning a single bag could fit in well under 100mm of vertical space. Two stacked bags at incline could plausibly fit 2L each within the same or similar zone budget that currently holds 1L bags. This is never explored because the doc locked onto "1L bags are forced by height" before considering that incline geometry is exactly what *unforces* that constraint.
- The doc only explores incline angles in the context of 1L bags. The geometry for 2L bags at incline (longer bag, shallower angle, different mounting point spacing) is a completely unexplored possibility space.
- If DP1 = 450mm, bags could hang vertically and incline may be unnecessary — but incline at 450mm could enable even more capacity or a more relaxed layout.
- Never considers that bag dimensions are estimated — actual bags of either size might not match the calculated geometry.
- `bag-zone-geometry.md` treats this doc's conclusions as decided, creating a circular dependency.
- Never considers mixed strategies (e.g., one bag inclined, one bag flat beneath it, or bags at different angles).

**What changes if:**
- 2L bags at incline are viable in 400mm → The entire reason for choosing 1L bags disappears. DP2's option space opens up dramatically without requiring DP1 to change.
- DP1 = taller enclosure → Vertical hanging becomes viable. Incline may be unnecessary, or could enable even larger reservoirs.
- Physical bag measurements differ from estimates → All clearance calculations change. This is especially critical for 2L bags where the thickness when full is a key unknown.
- Drip tray is removed → Additional ~15mm of floor space available for bag zone, further improving 2L viability.

---

### layout-spatial-planning.md

**Baked-in assumptions:**
- 280x250x400mm enclosure (DP1)
- 1L incline bags (DP2)
- Cam lever cartridge interface (DP3)
- Full topology with hopper (DP4)
- Specific zone heights for every layer
- Bags bottom, dock middle, electronics top

**Blind spots:**
- This is the "master integration" document, so it inherits EVERY assumption from EVERY other doc
- If any DP changes, this document needs substantial revision
- Never considers alternative zone ordering (e.g., electronics bottom in a sealed compartment, bags on top for gravity feed)
- Never considers modular/reconfigurable internal layout
- Treats all research conclusions as design decisions

**What changes if:**
- Any DP changes → This document must be rewritten. It is the most assumption-dependent document in the project.

---

### pump-assisted-filling.md

**Baked-in assumptions:**
- Peristaltic pump reversal works bidirectionally (theoretically true, not tested)
- Sealed dip tube path is proven (by existing priming, which is a reasonable inference)
- Check valve at dispensing point is sufficient
- 85-95% fill via pump pressure is acceptable

**Blind spots:**
- Claims to "correct" hopper-and-bag-management.md but never considers that its own conclusions are also unproven
- 85-95% fill is theoretical — trapped air behavior in a flexible bag at low pressure is hard to predict
- Never considers alternative fill methods that don't use the peristaltic pump: dedicated fill pump, pressurized hopper, syringe-style manual fill
- `plumbing.md` documents a leak failure mode at tube joints under pump backpressure — pump reversal puts backpressure on the same joints in the opposite direction. Not analyzed.
- Never considers that pump reversal might pull concentrate into the pump head in a way that's hard to clean (forward direction pumps water through to clean; reverse direction pulls concentrate backward through already-clean lines)

**What changes if:**
- DP4 = gravity fill → This document is informational only, not part of the design
- DP4 = manual bag swap → Document is irrelevant
- Pump reversal fails in testing → Fall back to gravity fill or manual swap
- Contamination from reverse-direction pumping is a problem → Need dedicated fill pump or manual swap

---

## Top-Level Hardware Docs

### bag-zone-geometry.md

**Baked-in assumptions:**
- 400mm enclosure (DP1)
- 1L bags incline-mounted (DP2)
- Drip tray removed (treated as decided)
- 176mm bag zone
- All dimensions from incline-bag-mounting.md treated as decided

**Blind spots:**
- Circular dependency: cites incline-bag-mounting.md as source, which cites bag-zone-geometry.md for constraints
- Physical bag dimensions are estimated throughout
- If DP1 or DP2 change, every number in this doc changes

**What changes if:**
- DP1 or DP2 change → Complete rewrite

---

### bill-of-materials.md

**Baked-in assumptions:**
- 6 solenoid valves (assumes DP4 = full topology)
- John Guest fittings throughout (assumes DP3 = John Guest)
- Specific pump model (Kamoer KPHM400)
- Two round displays
- FDC1004 capacitive sensing

**Blind spots:**
- If DP3 = CPC, fitting costs increase but custom mechanism parts decrease
- If DP4 = simplified, solenoid count drops to 2-4
- Never includes engineering time or iteration costs (filament for failed prints, replacement parts)
- If product removes physical displays (phone-only), ~$30-50 in display hardware drops out

**What changes if:**
- Any DP changes → BOM line items change. This should be the LAST document updated after decisions are made, not a planning input.

---

### dimensions-reconciliation.md

**Baked-in assumptions:**
- States values as "locked" when they're research conclusions
- 280x250x400mm as resolved
- 150x80x130mm cartridge as resolved

**Blind spots:**
- "Locked" framing discourages reconsideration
- Should explicitly state which values are derived from which DPs, so that when a DP is decided, derived values can be recalculated

**What changes if:**
- Any DP changes → Must be recalculated, not just updated

---

### gpio-planning.md

**Baked-in assumptions:**
- 6 solenoids (DP4 = full topology)
- FDC1004 for capacitive sensing (hopper exists, DP4 assumed)
- 3 electrical contacts for cartridge (no sensing, no ID)
- Two physical displays connected via UART

**Blind spots:**
- If DP4 = simplified, several GPIO allocations are unnecessary
- If product adds cartridge NFC/RFID, need additional GPIO or I2C device
- If displays go wireless, UART pins free up
- MCP23017 expander may be unnecessary if solenoid count drops

**What changes if:**
- DP4 = simplified → Fewer solenoids, possibly no FDC1004, MCP23017 may be unnecessary
- Phone-only interface → Free up 4 UART pins, remove display GPIO allocations
- Cartridge ID tag → Add NFC reader GPIO or I2C address

---

### plumbing.md

**Baked-in assumptions:**
- Silicone tubing for concentrate, hard tubing for water
- John Guest push-to-connect throughout
- Zip-tie compression joints for silicone-to-hard transitions
- Clean cycle uses needle valve and dedicated solenoids

**Blind spots:**
- Documents a leak failure mode under pump backpressure but doesn't trace implications to pump-assisted-filling.md (which applies backpressure in reverse)
- If DP3 = CPC, the plumbing topology changes at the cartridge interface
- If DP4 = simplified, clean cycle plumbing is deferred
- Never considers crimp fittings or hose clamps as alternatives to zip ties (more reliable in production)
- Never considers food-grade certification requirements for a product (all materials must be FDA/NSF compliant)

**What changes if:**
- DP3 = different fittings → Plumbing at cartridge interface changes
- DP4 = simplified → Clean cycle section is deferred
- Product context → All tubing, fittings, and adhesives must meet food-safety standards. This isn't analyzed anywhere.

---

## Meta-Observations

### Biggest Blind Spots Across All Documents

1. **The "horizontal zone" architecture is the deepest unquestioned assumption in the entire project.** Every document treats the enclosure as a layer cake — horizontal slices stacked vertically: bags at bottom, dock in middle, electronics on top. This forces components to compete for vertical budget: a taller bag zone steals from the dock zone. But there is no physical law requiring this. Components can overlap in depth, share vertical ranges, and be arranged diagonally. Bags could stretch from one corner to the opposite corner, using the full ~470mm diagonal of a 280x250x400mm enclosure, easily fitting 2L bags without increasing height. The cartridge could sit at the front of the same vertical range the bags pass through at a different depth. Electronics could pocket into a back corner. This "diagonal interleave" approach (see `visions/diagonal-interleave.md`) is just one alternative, but the point is broader: **no document ever asks "should we use horizontal zones at all?"** — and that unasked question constrains every spatial calculation in the project.

2. **No document considers food safety certification.** For a product, every wetted material must be FDA/NSF compliant. This isn't mentioned anywhere.

2. **No document considers failure modes from a consumer product perspective.** What happens when a fitting leaks under someone's sink while they're at work? What's the warranty/support implication?

3. **No document considers the Keurig/Nespresso model** — pre-filled, sealed flavor cartridges that eliminate bags, hoppers, and refill entirely. This is arguably the most consumer-friendly approach.

4. **Pump model is never questioned.** Every document assumes Kamoer KPHM400. Different pumps could change the cartridge envelope, noise profile, flow rate, and cost.

5. **Display configuration is never questioned.** Two small round displays is inherited from the current prototype. A single display, phone-only, or no-display approach could simplify the entire front face, cable routing, and firmware.

6. **No document considers manufacturing at scale.** Everything assumes 3D printing. If this is a product, injection molding, sheet metal, or off-the-shelf enclosures change every tolerance assumption, cost estimate, and design constraint.

7. **Cartridge swaps and bag refills are never clearly distinguished.** Cartridge swaps are 18-36 month maintenance events (pump tube wear). Bag refills are weekly-to-monthly depending on reservoir size and usage. Several documents conflate these, leading to confused analysis of what the cartridge interface mechanism needs to optimize for. The real criterion for a rare maintenance operation is *intuitive after a long gap*, not *fast*.
