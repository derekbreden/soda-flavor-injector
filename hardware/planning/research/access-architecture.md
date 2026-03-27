# Access Architecture Research

How the user physically accesses the internals of an under-sink enclosure. Only two user interactions exist:

1. **Hopper refill** — most frequent (weekly for families, monthly for moderate users). Pour concentrate into a funnel.
2. **Cartridge swap** — rare (every 18-36 months). Disconnect and remove pump cartridge.

---

## Options Evaluated

### A. Fixed Tower (Current Assumption) — $0

No moving parts. User reaches into dark cabinet.

- **Hopper refill:** Poor. User crouches, reaches 300-500mm into dark cabinet, pours by feel. Spill risk significant — can't see funnel opening clearly.
- **Cartridge swap:** Fair. Front-loading slot with cam lever works by feel. Adequate for a 2-year event.
- **When this is right:** Monthly refills (moderate single user), cost/simplicity paramount, or the cabinet already has a pull-out shelf.

### B. Full Drawer — $10-20

Entire internal assembly on ball-bearing drawer slides.

- Knape & Vogt or generic 250mm full-extension slides: 12.7mm wide per side, 100 lb rating, $4-15/pair
- Width consumed: **25.4mm total** (reduces interior from 272mm to 246mm, or enclosure widens to 306mm)
- Fluid lines cross the moving boundary — need 250mm service loops (4 lines × ~10mm OD, coiled behind drawer)
- Electrical: power cable service loop
- Needs a latch (magnetic catch $1-3, or push-to-open $2-5)
- **Hopper refill:** Fair — everything exposed from above, but funnels at top-rear of extended drawer, user leans over
- **Cartridge swap:** Very good — full visibility and access
- **Structural:** 9kg cantilevered on slides; mounting points need heat-set inserts, not just screws into plastic

### C. Partial Drawer (Bottom/Middle Only) — $10-20

Only bag + cartridge zone slides out. Electronics stay fixed on top.

- Reduces cantilevered weight to ~6-7kg
- Motor wires must cross the moving boundary (4 wires, flexible, manageable)
- **Does NOT improve hopper access** — hoppers are in the fixed upper section
- Better for cartridge swap, worse for the frequent task
- Only useful if primary goal is cartridge access, not hopper access

### D. Clamshell / Hinged Top — $3-10

Top ~90mm hinges open along rear edge, exposing hopper funnels from above.

**Hinge options:**
- Metal butt hinges (25-50mm): $1-3/pair
- Piano hinge (continuous, 300mm): $3-8
- 3D-printed living hinge: $0 but fatigues after ~100-500 cycles (1-10 years at weekly use)

**Lid hold-open options:**
- Neodymium magnets at ~110° open: $0.50, zero wear
- Friction hinge: $1-2
- Gas strut (overkill): $8-16/pair

**Headroom requirement:** Lid is ~90mm tall. At 90° open, total height = enclosure height + lid ≈ 490mm. Fits in most cabinets (500-600mm clearance) but tight in some.

**Critical design point:** Hopper funnels should be mounted on the fixed lower section (just below hinge line at ~310mm), NOT in the lid. The lid simply exposes them by removing the obstruction above. This avoids fluid lines crossing the hinge — only electrical wires cross.

- **Hopper refill:** Good. Open lid, pour into exposed funnels, close. One-hand flip + one-hand pour.
- **Cartridge swap:** No change from fixed tower (cartridge is below hinge line).

### E. Hinged Front Panel — $3-5

Front face swings open like a door.

- Adds a step before cartridge access (open door first) without improving it
- Does almost nothing for hopper access (funnels face upward, not forward)
- Enclosure loses front-face rigidity — remaining U-shell must be self-supporting
- **Not recommended.** Adds complexity without solving either user task.

### F. Slide-Out Tray — $15-35 ★

The enclosure sits on a separate pull-out shelf mounted in the cabinet. The enclosure itself is unchanged.

**Products:**
- Lynk Professional 11"W × 21"D (279 × 533mm): ~$30-35 — near-perfect dimensional match
- Rev-A-Shelf 4SBSU (under-sink specific): ~$30-60, designed to work around plumbing
- Generic Amazon: ~$15-30

**How it works:**
1. Open cabinet door
2. Pull tray forward ~200-300mm
3. Enclosure is now at cabinet door opening, in ambient room light
4. Pour into hopper funnel with clear visibility, natural angle, both hands free
5. For cartridge swap: front face is right at the cabinet opening
6. Push tray back, close door

**Key advantages:**
- **Zero enclosure modifications** — the device is a fixed tower sitting on a shelf
- No fluid lines or electrical cross any moving boundary inside the enclosure
- External plumbing needs ~250-300mm of slack (standard for any under-sink appliance)
- Best visibility of any option — room light reaches the device

**Key limitation:** Adds 25-40mm of height below the enclosure. In a 500mm cabinet, headroom above drops from ~100mm to ~60-75mm.

### G. Removable Panels — $1-3

Magnets or snap-fits hold panels that lift off for access.

- Small hopper cover (not full top panel): practical, $1-2 in magnets
- Full panel removal: user has nowhere to put a 280×250mm panel in a dark cabinet while holding a bottle
- **Works only in combination** with a slide-out tray (user can set panel on counter)

### H. Rotating / Tilting — $10-20

Enclosure pivots forward on rear-bottom pins at 30-45°.

- Good visibility and pouring angle when tilted
- **Tipping risk:** mandatory restraint (gas strut or strap)
- Fluid lines stressed at an awkward angle
- 5-8kg device requires effort to tilt without gas struts
- **Not recommended.** More complex than tray or clamshell, with more failure modes.

---

## Rankings

### For Hopper Refill (The Frequent Task)

| Rank | Approach | Why |
|---|---|---|
| 1 | **F. Slide-Out Tray** | Best visibility, natural angle, both hands free, zero enclosure mods |
| 2 | **D. Clamshell Top** | Good exposure from above, one-hand operation |
| 3 | **H. Tilting** | Good angle but requires effort, fluid stress |
| 4 | **B. Full Drawer** | Everything exposed but funnels at awkward position (top-rear of extended drawer) |
| 5 | **A. Fixed Tower** | Functional but dark and awkward |
| 6-8 | C, E, G | Don't meaningfully improve hopper access |

### For Cartridge Swap (The Rare Task)

| Rank | Approach | Why |
|---|---|---|
| 1 | **B. Full Drawer** | Full visibility of dock, both hands, everything exposed |
| 2 | **F. Slide-Out Tray** | Front face at cabinet opening, good light |
| 3 | **A. Fixed Tower** | Front-loading slot designed for one-hand blind operation — adequate for a 2-year event |
| 4-8 | Others | Marginal or no improvement |

---

## Recommended Approaches

### Best Single Option: F. Slide-Out Tray

- $15-35, zero enclosure modifications
- Transforms both user interactions by bringing the device into the light
- Off-the-shelf products exist in near-perfect dimensions
- Compatible with every internal layout (horizontal zones, diagonal, any)

### Best Combination: F + D (Slide-Out Tray + Clamshell Top)

- $20-40 total
- Tray handles positioning; hinged lid handles hopper exposure
- Weekly refills become: pull tray, flip lid, pour, close, push back
- Cartridge swap benefits from tray positioning alone
- Enclosure modification limited to splitting top 90mm into a hinged lid (2 hinges + magnetic hold-open)

### Pragmatic Minimum: A + LED Light

- $0-5 (adhesive cabinet LED, motion-activated)
- Adequate for moderate users (monthly refills)
- Wide-mouth hopper funnel (40-50mm) reduces spill risk for blind pouring

---

## Cost / Complexity Summary

| Approach | Hardware Cost | Enclosure Mods | Fluid Lines Cross? | Width Impact | Failure Modes |
|---|---|---|---|---|---|
| A. Fixed Tower | $0 | None | No | 0 | None |
| D. Clamshell Top | $3-10 | Moderate (split top) | No* | 0 | Hinge wear |
| F. Slide-Out Tray | $15-35 | None | No** | 0 | Tray slide wear (10yr+) |
| F+D Combined | $20-40 | Moderate | No | 0 | Tray + hinge wear |
| B. Full Drawer | $10-20 | Major (shell redesign) | Yes (4 lines) | +25mm | Slide wear, fluid fatigue |
| G. Small Hopper Cover | $1-2 | Minor | No | 0 | Lost cover |

*If hoppers mount below hinge line. **External plumbing needs slack but nothing inside the enclosure crosses a boundary.
