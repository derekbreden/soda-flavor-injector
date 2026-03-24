# V1 Hopper Integration: Diagonal Interleave Layout

Research on how the hopper/funnel system integrates with Vision 1's diagonal bag arrangement. The hopper is the primary user interaction point for refilling concentrate — it must be accessible, foolproof, and clean.

---

## 1. Hopper Position and Geometry

### 1a. Where the Space Is

In Vision 1, two bags stretch diagonally from sealed ends at the top-front to connectors at the bottom-back. The hopper sits at the top of the enclosure. Understanding where free space exists above the bags is essential.

Looking at a side cross-section (height vs. depth):

```
FRONT                                    BACK
┌────────────────────────────────────────────┐ ← top (392mm interior)
│                                            │
│  ███ ← sealed bag ends           [ELEC]   │
│   ██ (top-front)                           │
│    ╲╲                                      │
│     ╲╲  bags slope                         │
│      ╲╲  downward                          │
│       ╲╲                                   │
│        ╲╲                                  │
│ [CART]  ╲╲                          ╲╲     │
│          ╲╲                          ╲╲    │
│           ╲╲  ← connectors at bottom-back  │
└────────────────────────────────────────────┘ ← bottom
```

The sealed (non-connector) ends of the bags sit at the top-front corner. The bags descend toward the back, meaning the top-back area has open space (bags are low there, electronics pocket occupies the corner). The space available for a hopper is the wedge above the diagonal bag slab, which grows larger toward the back of the enclosure.

The problem: the largest open space above the bags is at the BACK — exactly where the user CANNOT easily reach. The space at the front-top is minimal because the sealed bag ends are near the top-front corner.

### 1b. Position Options

**Option 1: Hopper nestled above the diagonal, spanning front to mid-depth**

The hopper follows the same diagonal angle as the bags, sitting in the wedge above them. Wide pour opening at the front, narrowing toward the back. The pour opening is at the top-front edge of the enclosure.

- Pros: Compact, uses the natural wedge geometry, pour opening is at the most accessible point
- Cons: Limited volume (the wedge is narrow at the front), the pour opening competes with bag sealed ends for the same top-front real estate

**Option 2: Hopper above the bag plane, spanning the full top**

A horizontal or gently sloped hopper mounted at the enclosure ceiling, above the diagonal bags entirely. The bags must clear below it.

- Pros: Maximum volume, horizontal pour opening is natural
- Cons: Steals height from the enclosure — every mm the hopper takes pushes the bags (or other components) down. Only works if the bags sit low enough to leave room.

**Option 3: Hopper integrated into top panel (hinged or removable lid)**

The top panel of the enclosure IS the hopper, or contains the hopper opening. With a clamshell top or removable lid, the user pours directly into a funnel recessed in the top face.

- Pros: Maximum accessibility (the entire top surface is the pour zone), no internal volume competition
- Cons: Requires a hinged or removable panel (mechanical complexity), seal integrity when closed

**Option 4: Hopper at front-top edge, vertical orientation**

A small funnel mounted on the front face near the top, angled to receive a pour from the front. Does not extend deep into the enclosure.

- Pros: Most accessible from the cabinet opening, minimal internal space usage
- Cons: Small capacity (funnel only, not reservoir), awkward pour angle (nearly horizontal), splash risk

### 1c. Recommended Position

Option 3 (top-panel integration) combined with the slide-out tray access architecture is the strongest approach. When the user pulls the tray forward, the top of the enclosure is fully accessible. A hinged lid or removable cap exposes the hopper funnel(s). The user pours from above with both hands free, clear line of sight, and natural gravity-assist.

This avoids the geometric conflict of Options 1-2 (where the hopper competes with bag sealed ends for top-front space) and the ergonomic problems of Option 4 (horizontal pouring).

---

## 2. Fill Routing

### 2a. The Routing Challenge

Syrup enters the hopper at the TOP of the enclosure. The bag connectors (dip tube caps) are at the BOTTOM-BACK. The fill path must traverse nearly the full diagonal of the enclosure.

Route distances (approximate):

| Configuration | Hopper Location | Bag Connector Location | Straight-Line Distance | Tubing Run (with bends) |
|---|---|---|---|---|
| 1L at 35deg in 300D x 400H | Top-front (~0D, ~380H) | Bottom-back (~280D, ~100H) | ~400mm | ~450-500mm |
| 2L at 35deg in 350D x 400H | Top-front (~0D, ~380H) | Bottom-back (~330D, ~80H) | ~460mm | ~500-550mm |

### 2b. Fill Mechanism Options

**Gravity-only fill (through dedicated tubes)**

Syrup drains from the hopper down dedicated fill tubes to the bag connectors. No pump involvement.

- Driving pressure: Height differential of ~300mm = ~0.43 PSI (for water; syrup at 1.2-1.4 SG gives ~0.52-0.60 PSI)
- Flow rate limited by counter-current air displacement through the dip tube's 6.35mm bore
- Estimated fill time for 1L: 8-15 minutes. For 2L: 15-30 minutes
- Tubing dead volume: 450mm of 4.5mm ID tubing = ~7.2 ml per line (14.4 ml total for 2 lines)
- Pros: No electronics, no pump involvement, simple
- Cons: Slow, open-air system (ambient air enters bag during every refill), requires separate fill tubes (doubles tubing count), dead volume in long tubes wastes syrup

**Pump-assisted fill (through the existing pump circuit)**

The existing Kamoer peristaltic pump reverses direction to pull syrup from the hopper and push it into the bag. The hopper connects at TEE2 (pump outlet side) via a solenoid valve. A check valve at the dispensing point prevents air ingestion.

- Driving pressure: 2-5 PSI from the pump
- Estimated fill time for 1L: 2.5-5 minutes at 200-400 ml/min
- Tubing run: Hopper to TEE2 is shorter (~200-300mm) because TEE2 is near the pump (front-bottom triangle), and the existing pump-to-bag tubing handles the rest
- Dead volume: ~200-300mm of 4.5mm ID hopper tubing = ~3.2-4.8 ml per line
- Pros: 3-5x faster, sealed system (minimal air exchange), uses existing pump hardware, shorter dedicated tubing run
- Cons: Requires pump reversal firmware (trivial — 3 lines of code), check valves at dispensing point ($6-10), hopper solenoid valves ($18)

**Pump-assisted fill (dedicated fill pumps)**

Separate small peristaltic pumps on each hopper line push syrup into the bags through the bag-side tee (TEE1).

- Cost: $10-24 for 2 small pumps + $1-3 for drivers
- Pros: Independent of main pump circuit, simpler topology (no pump reversal)
- Cons: $30-50 additional cost, 2 more pumps to mount and wire, GPIO pressure on ESP32, additional failure points

### 2c. Recommended Fill Mechanism

Pump-assisted fill through the existing pump circuit (Option 1 from the pump-assisted-filling research). The cost delta is minimal ($6-10 for check valves), fill time is 3-5x faster than gravity, and the sealed system preserves concentrate freshness. The long diagonal routing distance (400-500mm for gravity) makes gravity fill even less attractive than in a horizontal layout — more tubing means more dead volume and slower flow.

### 2d. Dead Volume Analysis

Dead volume = syrup trapped in tubing after hopper empties. Wasted on each refill unless flushed.

| Fill Approach | Dedicated Tube Run | Tube ID | Dead Volume (per line) | Total (2 lines) |
|---|---|---|---|---|
| Gravity (dedicated tubes, top-to-bottom-back) | ~450mm | 4.5mm | 7.2 ml | 14.4 ml |
| Pump-assist via TEE2 (hopper to TEE2 only) | ~250mm | 4.5mm | 4.0 ml | 8.0 ml |
| Pump-assist via dedicated pump (hopper to TEE1) | ~400mm | 4.5mm | 6.4 ml | 12.7 ml |

Pump-assist via TEE2 has the lowest dead volume because the hopper tube only needs to reach TEE2 in the front-bottom triangle — the existing pump-to-bag tubing handles the rest, and that tubing is already filled with concentrate from normal dispensing.

### 2e. Backflow Prevention

If a bag is pressurized (compressed air from a partial fill, or squeeze from the bag's own weight on the incline), concentrate could flow backward through the dip tube, through the tubing, through TEE2, and up into the hopper — especially if the hopper solenoid valve leaks.

Prevention:
- The hopper solenoid is normally-closed (NC). When de-energized, it seals by spring pressure. Backflow requires the solenoid to fail open.
- A check valve on the hopper line (oriented to allow flow FROM hopper TOWARD TEE2) provides passive backup. Cost: $3-5 per line.
- Bag pressure is low (~0.5-2 PSI from compressed trapped air). Even a weak check valve blocks this.

---

## 3. Two-Flavor Management

Two bags, two flavors. The user must direct syrup to the correct bag during refill. Cross-contamination (pouring cola syrup into the lemon-lime bag) is a real failure mode.

### 3a. Option A: Two Separate Hopper Openings

Two funnels side by side on the top of the enclosure, each permanently plumbed to one bag.

- Funnel opening: 75-100mm diameter each
- Total width needed: 2 x 100mm + 30mm gap = 230mm (fits within 272mm interior width)
- Each funnel is labeled (color-coded, embossed, or with a snap-on colored ring)
- User pours into the correct funnel. No switching, no valves, no electronics.

| Metric | Assessment |
|---|---|
| Cost | Lowest — no selector hardware |
| Complexity | Lowest — pure plumbing, no moving parts in the selection path |
| User error risk | Moderate — user could pour into wrong funnel. Labeling and color-coding mitigate |
| Cross-contamination | None if user pours correctly; if wrong funnel, only that bag is contaminated |
| Cleaning | 2 funnels to clean instead of 1 |
| Space | 230mm width, 50-75mm depth each — fits comfortably |

### 3b. Option B: One Hopper with Mechanical Diverter

A single funnel with a manual lever or rotating selector that directs flow to one of two outlet tubes.

- Lever positions: LEFT (flavor 1), RIGHT (flavor 2)
- The diverter sits at the funnel outlet, before the tubing
- Materials: food-grade PP or silicone rotating valve body

| Metric | Assessment |
|---|---|
| Cost | Low-moderate — custom molded or 3D-printed diverter body + lever |
| Complexity | Moderate — moving part in the fluid path, must seal well |
| User error risk | Lower than Option A if labeled clearly (one pour point, explicit selection) |
| Cross-contamination | Risk at the diverter — residual syrup from flavor 1 contaminates flavor 2 unless diverter is rinsed between switches |
| Cleaning | Diverter body needs disassembly or flush path |
| Space | Single funnel saves ~100mm of width vs. two funnels |

### 3c. Option C: One Hopper with Electronic Valve Control

A single funnel with a solenoid-controlled diverter. The user selects the flavor on the display or app, then pours. Firmware opens the correct solenoid.

- Two solenoid valves at the funnel outlet, one for each bag line
- User selects flavor on touchscreen, pours, initiates fill on touchscreen
- Firmware controls which valve opens

| Metric | Assessment |
|---|---|
| Cost | Higher — 2 solenoid valves ($18), electronics integration |
| Complexity | Higher — firmware state management, display integration |
| User error risk | Lowest — firmware prevents pouring into wrong bag (wrong solenoid stays closed). However: if user selects wrong flavor on screen, same problem as Option A |
| Cross-contamination | Same risk as Option B — shared funnel means residue from previous pour |
| Cleaning | Shared funnel + 2 valve bodies |
| Space | Single funnel, compact |

Note: The pump-assisted fill topology already requires per-line hopper solenoid valves (Beduan 12V NC, $9 each, already in the BOM). Option C uses these same valves — no additional hardware beyond what pump-assisted filling already requires. The "electronic valve control" is essentially free if pump-assisted filling is chosen.

### 3d. Option D: Fill Through the Pump Circuit (Implicit Selection)

With pump-assisted filling, each flavor line has its own pump. The user pours syrup into a single shared hopper. The firmware activates the correct pump (reversed) to pull syrup from the hopper into the selected bag.

- The hopper has ONE opening
- Each line has its own hopper solenoid valve at TEE2
- User selects flavor on display/app, pours, initiates fill
- Firmware opens the correct hopper solenoid and reverses the correct pump

| Metric | Assessment |
|---|---|
| Cost | Zero additional beyond pump-assist topology (solenoids already present) |
| Complexity | Firmware only — valve + pump selection logic |
| User error risk | Same as Option C — user must select correctly on screen |
| Cross-contamination | Same shared-funnel risk |
| Cleaning | One funnel to clean |
| Space | One funnel, most compact |

This is effectively Option C with the pump doing the work instead of gravity. Since pump-assisted filling is the recommended approach, this is the natural two-flavor strategy.

### 3e. Recommendation

**For pump-assisted filling: Option D** (single hopper, firmware-controlled pump/valve selection). The hopper solenoid valves already exist in the pump-assist topology. The user pours into one funnel, selects the flavor, and firmware handles routing. No additional hardware.

**For gravity filling: Option A** (two separate funnels). Without electronic valves in the fill path, physical separation is the safest way to prevent cross-contamination.

**Regardless of approach: color-code everything.** Funnel rings, tubing, display labels, and bag cradle positions should all use consistent color coding (e.g., red = flavor 1, blue = flavor 2).

---

## 4. Hopper Capacity and Dimensions

### 4a. How Much Must the Hopper Hold?

The hopper serves one of two roles:

**Role 1: Reservoir (holds the full refill volume)**

The user pours the entire refill quantity into the hopper at once, caps it, initiates fill, and walks away. The hopper must hold the full volume.

| Bag Size | Refill Volume (leaving 10% headspace) | Hopper Capacity Needed |
|---|---|---|
| 1L | ~900 ml | ~1000 ml (with margin for splash) |
| 2L | ~1800 ml | ~2000 ml |

A 1L reservoir hopper is feasible. A 2L reservoir is large — roughly the size of a 2L soda bottle — and would consume significant enclosure volume.

**Role 2: Funnel (small, drains as user pours)**

The hopper is a small funnel (~150-300 ml capacity). The user pours slowly, the pump pulls syrup out as fast as it enters, and the funnel never fills completely. The user watches the funnel level and pours at a matching rate.

| Pump Fill Rate | Funnel Capacity | Pour Time for 1L | Pour Time for 2L |
|---|---|---|---|
| 200 ml/min | 200 ml | ~5 min (user pours steadily) | ~10 min |
| 400 ml/min | 200 ml | ~2.5 min | ~5 min |
| 400 ml/min | 300 ml | ~2.5 min (more buffer) | ~5 min |

With pump-assisted filling at 200-400 ml/min, a 200-300 ml funnel is sufficient. The user pours at a comfortable rate and the pump keeps up. If the user pours faster than the pump drains, the funnel buffers the excess — 200-300 ml of buffer handles a fast 2-3 second glug from a bottle.

**Role 3: Hybrid (moderate capacity, pour-and-walk-away for small refills)**

A 500 ml hopper handles a full 1L bag refill in two pours (pour 500ml, wait for pump to drain, pour 500ml more) or a single pour for partial top-ups. For 2L bags, it requires 4 pours — less convenient but still manageable.

### 4b. Physical Dimensions

Approximate hopper dimensions (assuming a truncated cone / funnel shape):

| Capacity | Top Diameter | Bottom Diameter | Height | Approximate Volume |
|---|---|---|---|---|
| 200 ml | 90mm | 15mm (outlet) | 60mm | ~200 ml |
| 300 ml | 100mm | 15mm | 70mm | ~300 ml |
| 500 ml | 110mm | 15mm | 85mm | ~500 ml |
| 1000 ml | 130mm | 15mm | 120mm | ~1000 ml |
| 2000 ml | 150mm | 15mm | 170mm | ~2000 ml |

For a funnel-only approach (200-300 ml), the hopper is compact: ~100mm diameter, ~70mm tall. Two of these fit side by side in 272mm of interior width with room to spare.

For a reservoir approach (1000 ml), the hopper is 130mm diameter and 120mm tall — a significant internal volume commitment.

### 4c. Recommendation

**200-300 ml funnel** with pump-assisted filling. The pump drains the funnel fast enough that the user can pour continuously from a bottle. The refill experience is: pull tray, open lid, select flavor, pour from bottle into funnel while pump runs, close lid, push tray. Total time: 1-3 minutes for a 1L refill. The small funnel size minimizes the space impact above the diagonal bags.

---

## 5. Access Architecture

### 5a. Slide-Out Tray (Recommended from access-architecture research)

The enclosure sits on a pull-out shelf mounted in the cabinet. The user pulls the tray forward ~200-300mm, bringing the entire enclosure to the cabinet door opening under ambient room light.

With the tray pulled out, the top of the enclosure is fully accessible from above. This is the ideal configuration for a top-mounted hopper:

```
CABINET (side view)

┌─────────────────────────────────┐ ← cabinet shelf
│                                 │
│    pulled-out position:         │
│                                 │
│    ┌───[LID]───┐               │
│    │  [FUNNEL] │               │
│    │           │               │
│    │ enclosure │               │
│    │           │               │
│    └───────────┘               │
│    ════════════════  ← tray    │
│    (at cabinet door opening)   │
└─────────────────────────────────┘ ← cabinet floor
     ↑ cabinet door
```

The user opens the cabinet door, pulls the tray, and the hopper funnel(s) are right at the front edge, at waist height, in full light. Both hands are free for pouring. This is the best possible ergonomic configuration.

### 5b. Clamshell Top (Complementary)

Combined with the slide-out tray, a clamshell top (hinged at the rear edge) exposes the hopper funnel(s) from above. The user flips the lid with one hand and pours with the other.

Lid height: ~40-80mm (covers the funnel zone). At 90-degree open, total height = enclosure height + lid. In a 500mm cabinet with 400mm enclosure + 35mm tray height, the lid needs to open within ~65mm of vertical clearance. A 40mm lid opens to 90 degrees within this budget; a larger lid may need to lean back against the cabinet shelf.

Alternative: a removable top panel (magnets or snap-fits) instead of a hinge. With the tray pulled out, the user lifts the panel off and sets it on the counter. This avoids the hinge mechanism entirely and has no height constraint. The panel is a simple flat piece.

### 5c. Fill Level Visibility

How does the user know when the funnel is full, draining, or empty?

- **Translucent funnel:** Food-grade silicone or Tritan (copolyester) is naturally translucent. The user can see the syrup level through the funnel wall.
- **Display feedback:** The capacitive sensor on the hopper line (already in the pump-assist topology, SEN-A) detects when the funnel is empty. The display shows "Filling..." with a progress indicator, then "Complete" when the sensor detects air.
- **Audible feedback:** A brief tone when the fill cycle completes.

### 5d. Spill Containment

What if the user overfills the funnel or misses entirely?

- **Funnel lip:** A raised rim (5-10mm) around the funnel opening catches minor spills.
- **Drip tray:** The top surface of the enclosure (below the lid) should be a shallow tray with raised edges, sloping toward a drain or collection point. Any spilled syrup pools here instead of dripping down the enclosure sides.
- **Overflow path:** If the funnel overflows, syrup goes into the drip tray. The drip tray should be removable for cleaning (lift out, rinse in sink).
- **Splash guard:** A short collar or baffle around the funnel opening prevents splashes during vigorous pouring.

Failure scenario: user pours into the wrong funnel (two-funnel setup) or selects the wrong flavor (single-funnel setup). In both cases, the bag receives the wrong concentrate. Recovery: run a clean cycle on the contaminated line, then refill with the correct flavor. This wastes one bag-volume of concentrate — annoying but not catastrophic. Color-coding and clear labeling are the primary mitigation.

---

## 6. Cleaning and Maintenance

### 6a. Hopper Funnel Cleaning

Syrup residue (sugar, flavoring, coloring) will accumulate on the funnel interior, especially at the narrow outlet. In a warm under-sink environment, this becomes a microbial growth risk within days.

**Cleaning approaches:**

| Approach | Method | Frequency | Effort | Effectiveness |
|---|---|---|---|---|
| Removable funnel | Lift out, wash in sink or dishwasher | Each refill or weekly | Low (30 seconds) | Excellent |
| Rinse-in-place | Pour hot water through the funnel, pump drains it | Each refill | Very low (10 seconds) | Good for sugar; insufficient for biofilm over time |
| Self-clean cycle | Firmware runs water through the hopper line during clean cycle | With scheduled clean cycle | Zero (automated) | Adequate if combined with periodic manual wash |
| Fixed funnel | User wipes with a cloth or brush | Weekly | Moderate | Fair — hard to reach the outlet |

**Recommendation:** Removable funnel. Food-grade silicone is ideal — flexible (pops out of a mounting ring), dishwasher safe, non-porous (resists biofilm), translucent (user can see residue), and inexpensive. A silicone funnel that press-fits into a hard mounting ring in the enclosure top gives the best combination of easy cleaning and secure mounting.

### 6b. Fill Tube Cleaning

The tubing between the hopper and TEE2 contains residual syrup after each fill cycle. Over time, this residue supports microbial growth.

**The pump-assist topology already supports hopper line flushing.** The valve state table from the existing research includes a "Hopper Line Flush" mode:
- Hopper solenoid: OPEN
- Pump: FORWARD
- Dispensing solenoid: OPEN
- Check valve: passes flow
- Flow path: Water in hopper -> TEE2 -> check valve -> dispensing point

The user pours clean water into the funnel after the concentrate fill. Firmware runs the flush cycle, pushing water through the hopper line and out the dispensing point. This clears the ~4 ml of dead volume.

For the bag-side tubing (TEE2 to TEE1 to bag), the existing clean cycle (clean solenoid + pump) already flushes this path.

### 6c. Microbial Risk Assessment

Sugar syrup at 50-67 Brix (the concentration of typical flavor syrups) has low water activity (aw ~0.80-0.86), which inhibits most bacterial growth. However:

- Diluted residue (from incomplete rinsing) has higher water activity and supports growth
- Mold species (Aspergillus, Penicillium) can grow at aw < 0.80
- The under-sink environment is warm (20-30C) and dark — ideal for mold
- Biofilm on tubing interior surfaces is the primary risk, not bulk liquid contamination

**Mitigation:**
- Regular clean cycles (weekly, automated by firmware schedule)
- Hopper line flush after each refill
- Removable funnel for periodic deep clean
- Smooth, non-porous materials throughout (silicone tubing, PP/Tritan funnel, no 3D-printed surfaces in the fluid path)

### 6d. Material Selection

| Component | Recommended Material | Why |
|---|---|---|
| Funnel body | Food-grade silicone (Shore 40-60A) | Flexible (removable), translucent, dishwasher safe, non-porous |
| Funnel mounting ring | PP or ABS (injection molded or machined) | Rigid support for silicone funnel, chemical resistant |
| Hopper tubing | Food-grade silicone (platinum-cured) | Flexible, non-porous, compatible with peristaltic pump if needed |
| Funnel lid/cap | Food-grade silicone or PP | Snap-fit or press-fit, with optional duckbill valve for sealed hopper operation |
| Drip tray | PP or ABS | Rigid, chemical resistant, removable for washing |

---

## 7. Geometric Integration with Diagonal Bags

### 7a. Configuration 1: 1L Bags at 35deg in 280W x 300D x 400H

Interior dimensions: 272W x 292D x 392H (4mm walls).

From the diagonal-stacking-geometry research:
- Bag stack depth consumed: 258mm
- Bag stack height consumed: 212mm
- Depth margin: 292 - 258 = 34mm
- Height above bag stack top: 392 - 212 = 180mm

**Where the bag sealed ends sit (top-front corner):**

The bags are mounted with sealed ends at the top-front, connectors at the bottom-back. At 35 degrees, the top of the bag stack at the front wall is at:

- Stack height at front wall = bag length x sin(35deg) + stack thickness x cos(35deg)
- = 280 x 0.574 + 50 x 0.819 = 161 + 41 = 202mm above the bottom of the stack
- If the bottom of the bag stack sits at the enclosure floor (0mm), the top of the stack at the front wall is at ~202mm
- BUT the bag connectors at the back are at roughly: stack thickness x sin(35deg) = 50 x 0.574 = 29mm height
- And the bag stack bottom-back corner is at roughly floor level

More precisely, the bag stack cross-section (side view) forms a parallelogram:

```
FRONT                                    BACK

392 ┬─────────────────────────────────────────┐
    │                                         │
    │   ★ HOPPER ZONE ★                      │
    │   (180mm tall at front,                 │
    │    growing toward back)            [E]  │
212 ┤   ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲      │
    │    ╲╲ bag stack (50mm thick) ╲╲╲╲╲╲     │
    │     ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲     │
    │      ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
    │                                   ╲╲    │
    │  [CARTRIDGE]                       ╲╲   │
    │  (front-bottom                      ╲╲  │
    │   triangle)                          ╲  │
  0 ┴─────────────────────────────────────────┘
    0                                    292mm
                    DEPTH →
```

**Available hopper zone at the front wall:**
- Top of bag stack at depth=0: 212mm height
- Enclosure top: 392mm
- Space above bags at front wall: 392 - 212 = **180mm**

**Available hopper zone at mid-depth (146mm):**
- Bag stack top at mid-depth: 212 - (146 x tan(35deg)) ... no, more precisely:
- The top surface of the bag stack descends at 35deg from front to back
- At depth D from front wall, the top of bags is at: 212 - D x tan(35deg) = 212 - 0.700 x D
- At D=146mm (mid-depth): 212 - 102 = 110mm
- Space above: 392 - 110 = **282mm**

**Available hopper zone at back wall (D=258mm, where bags end):**
- Bag stack top: 212 - 258 x 0.700 = 212 - 181 = 31mm
- Space above: 392 - 31 = **361mm**
- But electronics occupy the top-back corner (~50mm zone)

**Hopper volume estimate (funnel sitting above bag surface, front half of enclosure):**

If we place a funnel in the top-front zone with:
- Width: 200mm (leaving 36mm each side for walls and structure)
- Depth: 100mm (front quarter of interior)
- Height: 70mm (funnel from rim to outlet)
- Position: top of enclosure, front edge

The funnel sits well clear of the bags (180mm clearance at front). A 200ml funnel (100mm diameter, 70mm tall) fits easily.

Even a 500ml reservoir (110mm diameter, 85mm tall) fits in this zone with generous clearance.

### 7b. Configuration 2: 2L Bags at 35deg in 280W x 350D x 400H

Interior dimensions: 272W x 342D x 392H.

From the diagonal-stacking-geometry research:
- Bag stack depth consumed: 333mm
- Bag stack height consumed: 266mm
- Depth margin: 342 - 333 = 9mm (tight)
- Height above bag stack top: 392 - 266 = 126mm

**Available hopper zone at front wall:**
- Top of bag stack at depth=0: 266mm height
- Space above: 392 - 266 = **126mm**

This is tighter than the 1L configuration. A 200ml funnel (70mm tall) fits with 56mm clearance. A 500ml reservoir (85mm tall) fits with 41mm clearance. A 1000ml reservoir (120mm tall) does NOT fit — only 126mm available, and the reservoir lid/cap needs clearance too.

**Available hopper zone at mid-depth (171mm):**
- Bag top: 266 - 171 x 0.700 = 266 - 120 = 146mm
- Space above: 392 - 146 = **246mm**

At mid-depth, there is ample room. A hopper that extends from front to mid-depth gains increasing headroom.

```
FRONT                                          BACK

392 ┬───────────────────────────────────────────────┐
    │ ★ 126mm ★                                     │
266 ┤   ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
    │    ╲╲ bag stack (80mm thick) ╲╲╲╲╲╲╲╲╲╲╲╲╲    │
    │     ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲   │
    │      ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲  │
    │       ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲ │
    │  [CARTRIDGE]                             ╲╲╲╲╲│
    │  (front-bottom triangle)                  ╲╲╲╲│
  0 ┴───────────────────────────────────────────────┘
    0                                          342mm
                       DEPTH →
```

**Key finding for 2L configuration:** The 126mm above the bag stack at the front wall is sufficient for a funnel-style hopper (200-300 ml capacity, 60-70mm tall) but NOT for a large reservoir. This reinforces the recommendation for a small funnel with pump-assisted draining rather than a large reservoir.

### 7c. Cross-Section Diagrams

**1L configuration (280W x 300D x 400H, 35deg):**

```
FRONT                                    BACK
┌────────────────────────────────────────────┐ 392mm
│ ┌────┐                                     │
│ │FNLS│ ← funnels (70mm tall,          [E]  │
│ └─┬──┘   200mm wide)                       │
│   │tubing                                  │
│   ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲         │
│    ╲╲ 1L bags (50mm stack) ╲╲╲╲╲╲╲╲        │
│     ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲     │
│      ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
│ ┌─────────┐                          ╲╲╲   │
│ │CARTRIDGE│                           ╲╲   │
│ │(150x130 │                            ╲   │
│ │ x80mm)  │                   [connectors] │
│ └─────────┘                                │
└────────────────────────────────────────────┘ 0mm
0mm                                     292mm
```

Hopper zone clearance at front: 180mm (ample).
Funnel pour opening is at ~322-392mm height — accessible from above when lid is open.

**2L configuration (280W x 350D x 400H, 35deg):**

```
FRONT                                          BACK
┌───────────────────────────────────────────────────┐ 392mm
│┌────┐                                             │
││FNLS│ ← funnels (70mm tall)               [ELEC] │
│└─┬──┘                                             │
│  │                                                │
│  ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲      │
│   ╲╲ 2L bags (80mm stack) ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲     │
│    ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲    │
│     ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲   │
│      ╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲╲  │
│ ┌─────────┐                                 ╲╲╲╲╲ │
│ │CARTRIDGE│                                  ╲╲╲╲ │
│ └─────────┘                         [connectors]  │
└───────────────────────────────────────────────────┘ 0mm
0mm                                            342mm
```

Hopper zone clearance at front: 126mm (adequate for funnel, not for large reservoir).

---

## 8. Comparison of Hopper Approaches

| Approach | Cost | Complexity | User Experience | Cleaning | Risk | Best For |
|---|---|---|---|---|---|---|
| **A. Two funnels (200ml each), gravity fill** | $0-5 (funnels only) | Very low — no electronics in fill path | Fair — slow fill (8-15 min), user must wait or check back | 2 funnels to wash, long tube dead volume (14ml) | Cross-contamination if wrong funnel; open-air bag exposure each refill | Lowest-cost prototype |
| **B. Two funnels (200ml each), pump-assist fill** | $6-10 (check valves) + $18 (hopper solenoids) | Low — pump reversal firmware, 2 solenoid valves | Good — fast fill (2-5 min), labeled funnels, pour-and-go | 2 funnels to wash, low dead volume (8ml), automated line flush | Cross-contamination if wrong funnel | Moderate cost, values simplicity |
| **C. Single funnel (300ml), pump-assist, firmware flavor select** | $6-10 (check valves) + $18 (hopper solenoids, already needed) | Moderate — firmware UI for flavor selection | Very good — single pour point, fast fill, display-guided | 1 funnel to wash, low dead volume (4ml per active line) | Wrong flavor if user selects incorrectly on display; shared funnel residue | Best balance of UX and cost |
| **D. Single funnel with mechanical diverter, gravity fill** | $5-15 (diverter mechanism) | Moderate — custom moving part in fluid path | Fair — manual lever selection, slow fill | Diverter body is hard to clean, residue in mechanism | Diverter seal failure, cross-contamination in shared path | Not recommended — combines worst of both worlds |
| **E. Single large reservoir (1L), pump-assist, firmware select** | $6-10 + $18 (same as C) + $5-10 (larger hopper) | Moderate — same as C plus larger hopper mounting | Excellent — pour full refill at once, walk away completely | Large funnel to wash, but less frequent (only after full refill) | Overflow if user pours more than bag capacity; large hopper takes internal volume | Users who refill infrequently and want a pour-once experience |
| **F. Two funnels (200ml each), dedicated fill pumps** | $28-42 (2 pumps + drivers) + $18 (solenoids) | High — 2 extra pumps, drivers, GPIOs, mounting | Good — similar to B but independent of main pump | Same as B | More failure points (2 extra pumps), GPIO pressure on ESP32 | Over-engineered — not recommended unless pump reversal is infeasible |

### 8a. Summary of Trade-offs

**Cost axis:** A < B = C < E < F < D (D is not cost-effective for what it delivers)

**UX axis:** E > C > B > F > A > D

**Cleaning axis:** C > E > A = B > F > D

**Reliability axis:** A > B = C > E > F > D

### 8b. Recommended Approach

**Approach C: Single funnel (200-300 ml), pump-assisted fill, firmware-controlled flavor selection.** This is the natural outcome of combining:
- Pump-assisted filling (already recommended for speed and sealed operation)
- Single hopper (saves space, one cleaning point)
- Per-line hopper solenoid valves (already in the pump-assist BOM)
- Display/app-driven flavor selection (firmware only, no additional hardware)

The user experience: pull tray, flip lid, select flavor on display, pour from bottle into funnel (2-5 min pour for 1L), display shows "Complete", optionally rinse funnel with water to flush the line, close lid, push tray.

---

## 9. Open Questions

1. **Dip tube air counter-flow at diagonal angles:** At 35 degrees, the bag is significantly tilted. The dip tube enters from the low end (connector at bottom-back). During pump-assisted fill, concentrate is pushed UP through the dip tube into the bag interior. Air displaced from the bag must travel DOWN through the dip tube against the flow. Does the 35-degree angle help or hurt compared to a more vertical bag orientation? Steeper angles may allow better air separation (air rises to the sealed end, away from the dip tube opening).

2. **Hopper solenoid placement:** Where physically does the hopper solenoid sit? Near the funnel (top of enclosure) or near TEE2 (front-bottom triangle)? Near TEE2 minimizes the tubing volume between the solenoid and the pump, reducing dead volume. Near the funnel makes the fill path shorter from pour to valve. Recommend near TEE2 to minimize dead volume in the always-pressurized section.

3. **Sealed vs. open hopper during fill:** The pump-assist topology supports a sealed hopper (silicone cap with duckbill valve). A sealed hopper prevents ambient air from entering the bag during fill — the duckbill valve lets air into the hopper as it drains, but the air stays in the hopper and doesn't reach the bag. An open hopper is simpler (no cap) but allows air exchange. For weekly refills of sugar syrup, the freshness impact is likely minimal, but a sealed hopper is better practice.

4. **Funnel material for production:** Silicone is ideal for removability and cleaning, but injection-molded PP or Tritan is cheaper at scale. A hybrid (rigid PP funnel body with a silicone gasket at the mounting interface) may be the best production solution.

5. **Overfill detection:** Can the capacitive sensor on the hopper line detect a full bag? When the bag is full, pump pressure increases and flow rate drops. Firmware could monitor pump current draw or use a second capacitive sensor on the bag to detect fullness and stop the fill automatically. Without this, the user must estimate the correct pour volume.
