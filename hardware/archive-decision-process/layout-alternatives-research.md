# Layout Alternatives: Beyond Horizontal Zones

Research into spatial arrangements that abandon the "horizontal layer cake" assumption pervading all existing design documents.

## Context

Every existing research document assumes components stack in horizontal slices: bags at bottom, dock in middle, electronics on top. This was never justified — it was just the first arrangement tried, and everything built on it. This document explores what else is possible.

Reference enclosure: 280W x 250D x 400H mm (not locked — these are the current working assumption).

---

## Key Geometry Facts

| Measurement | Value | Notes |
|---|---|---|
| Depth-height diagonal | 471.7mm | Side cross-section, corner to corner |
| Width-height diagonal | 488.3mm | Front cross-section |
| Full 3D diagonal | 548.5mm | Corner to opposite corner |
| 2L Platypus bag length | ~350mm | Estimated, needs physical measurement |
| 2L Platypus bag width | ~140mm | Estimated, needs physical measurement |
| 2L Platypus bag thickness (full) | ~60mm max | Expands from flat; easily compressed to 40mm across full length without spillage |
| 2L Platypus bag thickness (compressed) | ~40mm | Light hand pressure, no spillage — this is the design thickness |
| 1L Platypus bag length | ~250mm | Estimated |
| Two bags side by side | 280mm | Exactly equals enclosure width — zero clearance |
| Two bags stacked (perpendicular to diagonal) | 80mm compressed | At 60°+, this fits within the enclosure depth-height envelope |

The depth-height diagonal (471.7mm) comfortably exceeds a 2L bag's 350mm length by 122mm. This is why diagonal mounting is geometrically promising.

### Diagonal Stacking Math (CORRECTED)

Previous research incorrectly concluded stacking two bags along a diagonal was infeasible. It only checked 45° and gave up. Steeper angles work.

For a bag of length L at angle θ from horizontal, with two bags stacked (total perpendicular thickness T):
- **Total depth consumed** = L × cos(θ) + T × sin(θ)
- **Total height consumed** = L × sin(θ) + T × cos(θ)

With L=350mm, T=80mm (two bags compressed to 40mm each):

| Angle | Total depth | Total height | Depth margin | Height margin | Feasible? |
|-------|-------------|--------------|--------------|---------------|-----------|
| 45° | 304.1mm | 304.1mm | -54.1mm | 95.9mm | ❌ depth exceeds 250mm |
| 55° | 266.3mm | 332.6mm | -16.3mm | 67.4mm | ❌ depth exceeds 250mm |
| 58° | 254.9mm | 339.7mm | -4.9mm | 60.3mm | ❌ barely over |
| **60°** | **244.3mm** | **343.1mm** | **5.7mm** | **56.9mm** | **✅** |
| 63° | 230.2mm | 348.1mm | 19.8mm | 51.9mm | ✅ comfortable |
| 65° | 220.4mm | 351.0mm | 29.6mm | 49.0mm | ✅ good clearance |

**At 60°, two stacked 2L bags fit.** At 63-65°, there is comfortable clearance on depth AND ~50mm of headroom above the bag stack. This headroom is available for electronics, hopper funnels, or structural elements.

**Key insight:** Stacking (not side-by-side) means each bag is only 140mm wide, freeing ~130mm of enclosure width for the cartridge, tubing, and other components alongside the bag stack.

---

## Approaches Explored

### 1. Diagonal Interleave (Owner's Vision)

Bags stretch from top-front to lower-back, using the depth-height diagonal. Other components fill the triangular voids.

**Two arrangement options exist: side-by-side and stacked.**

**Option A: Side-by-side (two bags across the width at the same diagonal angle)**
- Two 2L bags at 140mm wide each = 280mm total = exactly enclosure width (zero clearance)
- At 45° angle: each bag spans 247mm depth x 247mm height, leaving ~150mm of headroom
- Width is the binding constraint — may need enclosure widened to 300mm+ for clearance

**Option B: Stacked (one bag on top of the other, both at the same diagonal angle)** ★
- See the stacking math table in Key Geometry Facts above
- At 60°+, two bags compressed to 40mm each fit within the 250mm depth AND 400mm height
- At 63°: 230mm depth (20mm margin), 348mm height (52mm headroom)
- Bags are only 140mm wide total, freeing ~130mm of width for other components alongside
- **This is the owner's original vision and it works**

**Stacked is geometrically superior** because it doesn't fight the width constraint. Side-by-side consumes the entire 280mm width; stacked consumes only 140mm and leaves room for the cartridge, electronics, or other components to sit beside the bag stack rather than strictly above or below it.

**The angle is not a single "sweet spot" — it's a continuous tradeoff:**
- Shallower (55-58°): more headroom, but depth gets tight. Single bags fit, stacked doesn't.
- 60°: stacking becomes feasible with compressed bags. 5.7mm depth margin — tight.
- 63-65°: comfortable margins on both axes. ~50mm headroom. **Recommended range for stacked layout.**
- Steeper (70°+): bags are nearly vertical, headroom shrinks, and the "diagonal" advantage diminishes.

**Component placement in the stacked diagonal layout:**
- Bags: stacked at 63°, occupying ~230mm depth x 348mm height x 140mm width. Positioned to one side of the enclosure.
- Cartridge (150W x 80H x 130D): beside the bag stack in the remaining ~130mm of width, at a convenient height for front-loading. OR in the front-lower triangle beneath the bag stack's lower end.
- Electronics: in the ~52mm headroom above bag tops, spanning full width. OR beside the bag stack above the cartridge.
- Hopper: angled funnel on the top-front of the device, mouth accessible from above, body angled to follow the bag slope below it.
- Displays: front face, mid-height. Thin — don't compete for depth.

**Tube routing:** Bag connectors at lower-back corner. Cartridge beside or below bags. Short tube runs along the enclosure floor or side wall.

**Bag installation:** Bags slide into a cradle or channel that defines the diagonal angle. Top bag loads first (closer to the back), bottom bag loads second (closer to the front). A front-opening or clamshell enclosure allows access.

**Verdict: Stacked diagonal at 63° is geometrically sound, uses 2L bags, fits in 280x250x400mm, and leaves room for all other components. The width constraint that kills side-by-side arrangements does not apply. Worth physical prototyping.**

---

### 2. Vertical Partitioning (Left-Right, Front-Back, L/T-Shape)

Explored dividing the enclosure into vertical columns or front/back sections.

**Left-Right Split: Does not work.** Each bag is 140mm wide; half of the 272mm interior is 136mm. Bags are 4mm too wide. The cartridge at 150mm wide also exceeds a half-width column. Widening the enclosure to ~320mm+ would be needed, which is excessive.

**Front-Back Split: Marginal.** Front section (~130mm deep) holds the cartridge. Back section (~110mm deep) holds bags — but bags must lie flat (no incline, poor drainage). Tube routing crosses the internal divider. User access to bags requires reaching to the back of the enclosure. Worse than current design in most ways.

**L/T-Shape: Reduces to the same failures.** Any shape that constrains bags to less than the full floor footprint runs into the same dimensional conflicts.

**U-Wrap (bags on sides of cartridge): Barely feasible.** Each bag in a 40mm-wide side channel flanking the cartridge. Fits dimensionally with 4mm total clearance. Zero room for mounting hardware, tubing, or user hands. Impractical.

**Verdict: Vertical partitioning does not work at 280mm width. The bags are too wide relative to the enclosure for any left-right division. Front-back division sacrifices incline drainage. These approaches fight the geometry.**

---

### 3. Unconventional Approaches

#### 3a. Bags as Structural Walls
Mount bags flat against the back wall. Two side-by-side at 140mm each = 280mm wide, 250mm tall, 40mm thick.

**Fails on height.** Bags consume 250mm of the 392mm interior height. Remaining 142mm cannot hold cartridge (80mm) + lever (40mm) + electronics (90mm) = 210mm. 68mm short.

**Verdict: Arithmetic doesn't close.**

#### 3b. Vertical Bag Tubes (Standing on Short Edge)
Stand each bag upright on its 140mm edge: 140mm tall x 250mm deep x 40mm wide.

**Dimensionally feasible.** Two bags = 100mm wide, 250mm deep, 140mm tall. Fits in a lower corner. Leaves room for cartridge and electronics above.

**Functionally worse.** The dip tube orientation creates a drainage vs. filling tradeoff. With the connector at the bottom, the dip tube opens at the top of the bag — liquid below the tube opening becomes dead volume (50-80ml). The incline mount in the current design elegantly keeps the tube submerged throughout drainage.

**Verdict: Works spatially, fails functionally.**

#### 3c. Concentric/Nested (Bags Wrapping Cartridge)
**Physically infeasible.** Platypus bags are flat pouches, not conformable membranes. A full bag is a rigid pillow. It cannot wrap around a rectangular cartridge. Would require custom-made cylindrical bags that don't exist.

**Verdict: Not feasible.**

#### 3d. DRAWER/TRAY ARCHITECTURE ★

The entire internal assembly (or the lower portion) slides out on ball-bearing drawer slides.

**This is the strongest unconventional concept found.** It solves the fundamental ergonomic problem — reaching into a dark box inside a dark cabinet — without introducing geometry or physics problems.

**How it works:**
- Standard kitchen drawer slides (ball bearing, side mount, 250mm length, 25-50kg rating): $10-15/pair
- Full-drawer: everything slides out. Partial-drawer: bags + cartridge + valves slide out, electronics stay fixed on top.
- Water supply lines: flexible service loops (250-300mm of slack coiled behind the drawer). Same approach as dishwasher and refrigerator water lines. Proven in millions of appliances.
- Electrical: 12V barrel jack on a pigtail in the service loop. Display cables with slack or ribbon cable.

**Dimensional impact:**
- Drawer slides are ~13mm wide each. Two slides consume 26mm of interior width.
- At 280mm enclosure: drawer interior = 246mm. With stacked diagonal bags (140mm wide), this leaves 106mm beside the bags for cartridge and other components. Tight but workable.
- At 300mm enclosure: drawer interior = 266mm. 126mm beside bags — comfortable.
- Note: the drawer width concern was originally analyzed assuming side-by-side bags (280mm needed). With stacked bags at 140mm, the drawer width penalty is much less consequential.

**Ergonomic win:**
- Pull drawer: full access to all components from above and front
- Hopper refill (the most frequent interaction): pour concentrate into funnel without reaching into a box
- Cartridge swap: pull out, flip lever (or whatever interface), slide cartridge out. Everything at arm level.
- Bag installation: bags visible and accessible from above
- No flashlight needed — components come to you

**Cost:** ~$15-25 in hardware (stainless slides + extra tubing), 20mm additional width, slightly more complex assembly.

**Risk:** Drawer slides in humid under-sink environment may corrode. Stainless steel slides mitigate this (~$15-20/pair).

**Verdict: Genuinely improves usability. Modest cost increase. Worth serious consideration. Works as an enhancement to any internal layout (horizontal zones, diagonal, or otherwise). This is orthogonal to the zone question — it's about how the user accesses the internals, not how the internals are arranged.**

#### 3e. Split Enclosure (Two Boxes)
Box A: cartridge + electronics + displays + valves (~220x140x220mm). Box B: bags + hopper (simple tray).

**Feasible but not better.** Tube runs between boxes add 8-32ml dead volume per line, potential leak points, and installation complexity. The current single enclosure already fits under a sink. Splitting distributes the volume without reducing it.

**Verdict: Keep as fallback for unusual installation constraints. Not recommended as primary design.**

#### 3f. Rotating Carousel
**Worst concept explored.** Bags exceed turntable diameter. Fluid lines twist and kink. User interaction is worse than current design. Bearing adds weight and failure modes.

**Verdict: Do not pursue.**

---

## Summary: What Actually Challenges the Horizontal Zone Assumption?

| Approach | Challenges zones? | Geometrically feasible? | Functionally better? | Worth pursuing? |
|---|---|---|---|---|
| Diagonal interleave (stacked) | Yes — fundamentally | **Yes** (stacked at 60-65°) | Likely — enables 2L bags in 400mm height | **Yes** |
| Left-right split | Yes | No (bags too wide) | No | No |
| Front-back split | Partially | Marginal | No (loses incline drainage) | No |
| Bags as walls | Yes | No (height budget fails) | N/A | No |
| Vertical tubes | Yes | Yes | No (dip tube problem) | No |
| Nested/concentric | Yes | No (bags can't wrap) | N/A | No |
| **Drawer** | **No** (orthogonal) | **Yes** | **Yes** (ergonomics) | **Yes** |
| Split enclosure | Yes | Yes | No (adds complexity) | Fallback only |

**Two ideas survive scrutiny:**

1. **Diagonal interleave (stacked at 63°)** — the only layout that genuinely challenges horizontal zones and works geometrically. Two 2L bags stacked at 63° fit within 230mm depth × 348mm height × 140mm width. This leaves ~130mm of width and ~52mm of headroom for all other components. The width constraint that kills side-by-side arrangements does not apply to stacked bags.

2. **Drawer architecture** — doesn't challenge the zone layout but dramatically improves usability. Compatible with any internal arrangement (horizontal zones, diagonal, or otherwise). Could be combined with diagonal interleave.

Note: With the stacked diagonal layout, the "critical unknown" shifts from bag width (which only matters for side-by-side) to **bag thickness when compressed** (which determines the perpendicular stack height). Owner reports 40mm is easily achievable — this should be verified with a filled 2L bag.

---

## Research Gaps Identified

1. **The side-by-side partitioning research was conducted using 1L bag dimensions (250mm) and "400mm locked" as assumptions** — exactly the tunnel vision this exercise was meant to break. A second pass with 2L bags and unlocked dimensions would yield different results.

2. **No research explored combining diagonal bags with a drawer.** This combination — bags diagonal inside a sliding tray — could be the best of both worlds.

3. **No research explored what happens if the enclosure is wider (300-320mm) but shorter (350mm).** A wider, shorter box might fit bags flat while still fitting under a sink. The 280mm width was never justified — it was inherited from early research.

4. **No research explored combining the stacked diagonal layout with non-zone component placement.** If bags occupy 140mm of width at 63° through the center, the remaining 130mm of width could hold the cartridge, electronics, and valves in a vertical column beside the bags — eliminating horizontal zones entirely.
