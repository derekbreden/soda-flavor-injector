# Enclosure Layout & Spatial Planning

Research into the overall layout and spatial planning for a self-contained soda flavor injector enclosure. This unit sits inside an under-sink cabinet and houses every component in the system: bags, pumps, electronics, valves, plumbing, displays, and the hopper inlet.

---

## 1. Component Inventory with Dimensions

### 1a. Master Dimension Table

All dimensions are L x W x H unless otherwise noted. Dimensions include reasonable tolerances and mounting hardware where applicable.

| Component | Dimensions (mm) | Weight | Orientation Constraints | Connection Points | Thermal Notes |
|---|---|---|---|---|---|
| **Platypus 2L bag (full, x2)** | 350 x 190 x ~60 | ~2.0 kg each (water weight) | Must drain downward; outlet at bottom or end of bag. Gravity-fed to pump inlet. | Drink tube kit outlet at one end (barb fitting) | None |
| **Platypus 2L bag (empty)** | 350 x 190 x ~5 | ~30 g | Collapses flat | Same | None |
| **Pump cartridge (assembled)** | 140 x 90 x 100 (W x H x D) | ~940 g / 2.1 lbs | Horizontal slide-in, mating face forward. Lever needs ~100mm vertical clearance above. | 4x John Guest 1/4" push-connect on dock wall, 3 pogo pin electrical contacts on top face | Motors: ~7W each under load, ~14W total |
| **Cartridge dock (housing)** | ~180 x 130 x 130 | ~500 g | Fixed mount. Lever extends ~110mm above dock top. | 4x JG fittings (back plate), 3 pogo pins, guide rails | None |
| **ESP32-DevKitC-32E on DIN rail breakout** | 110 x 90 x 35 | ~80 g | Any orientation. USB port needs access for firmware updates. | USB micro (top), GPIO headers (sides), UART wires to RP2040 and S3 | Minimal (<0.5W) |
| **L298N motor driver (x2)** | 43 x 43 x 27 | ~30 g each | Any orientation. Heatsink tab faces up or outward for convection. | Screw terminals (motor out, power in, logic in), pin header (enable/PWM) | Significant: 1.8-3.2V drop at 1A per channel = 2-6W each. Up to ~12W total for both. |
| **DIGITEN flow meter** | 65 x 30 x 28 | ~50 g | Inline with water flow. Horizontal preferred (no air traps). | 1/2" threaded inlet/outlet (with adapters to 1/4" push-connect) | None |
| **Beduan 12V solenoid valve (x4)** | ~80 x 35 x 45 | ~120 g each | Any orientation for normally-closed valves. Vertical preferred for draining. | 1/4" push-connect or NPT on each end, 2-wire power leads | Coil: 5.5W each when energized. Only on during dispensing or clean cycle (intermittent). |
| **Needle valve (x1)** | ~55 x 25 x 25 | ~60 g | Inline, any orientation | 1/4" compression or NPT on each end | None |
| **DS3231 RTC module** | 38 x 22 x 14 | ~8 g | Any orientation | I2C (4 wires), CR2032 battery access | None |
| **S3 display (Meshnology 1.28" round)** | 48 x 48 x 33 | ~50 g | Face outward for viewing. Rotary knob needs finger access. | Retractable Cat6 cable to ESP32 | None |
| **RP2040 display (Waveshare 0.99" round)** | ~38 x 38 x 25 (est. with case) | ~30 g | Face outward for viewing | Retractable Cat6 cable to ESP32 | None |
| **12V power supply (internal, if used)** | ~100 x 50 x 30 (typical 12V 2A enclosed PSU) | ~100 g | Ventilation needed around it | IEC inlet or barrel jack in, 12V DC out | 2-4W heat at full load |
| **Air switch pneumatic tube** | 4mm OD tube, ~2m length | Negligible | Flexible routing, pass-through to enclosure | One end at countertop button, one end at air switch sensor inside | None |
| **Wiring harness** | ~300mm long bundles | Negligible | Flexible, needs strain relief at connectors | Dupont headers, spade connectors, UART cables | None |

### 1b. Bag Dimensions — Critical Detail

The Platypus 2L bags are the largest single components by volume. When full, each bag is roughly a soft pillow shape:

- **Flat dimensions**: 350 x 190mm (13.8" x 7.5")
- **Thickness when full**: ~60mm (2.4") — this varies depending on how much the bag is constrained
- **Thickness when half-full**: ~35-40mm
- **Volume per bag when full**: roughly 350 x 190 x 60 = ~4.0L of bounding box for 2L of liquid (bags are not perfectly rectangular)

Two bags side by side: 350 x 380 x 60mm (13.8" x 15" x 2.4")
Two bags stacked: 350 x 190 x 120mm (13.8" x 7.5" x 4.7")

The bags are soft-sided, so they can conform to available space to some degree. They can also be oriented vertically (hanging) or laid flat/tilted.

### 1c. Cartridge Dock — Critical Detail

The dock is the second-largest discrete component and drives the enclosure's front-panel layout:

```
    FRONT VIEW (looking at dock opening)

    ┌─────────────────────────────┐
    │         lever swing         │  ← ~100mm clearance above dock
    │         clearance           │
    │  ┌─────────────────────┐   │
    │  │                     │   │
    │  │   Cartridge slot    │   │  90mm tall
    │  │   140mm wide        │   │
    │  │                     │   │
    │  └─────────────────────┘   │
    │                             │
    └─────────────────────────────┘
         ~180mm wide x ~130mm tall
         (including lever clearance: ~230mm)
         ~130mm deep
```

The cam lever needs approximately 100mm of vertical clearance above the cartridge top to flip from locked to released position. This means the dock zone needs ~230mm (9") of unobstructed vertical space.

### 1d. Aggregate Volume Estimate

| Category | Estimated Volume (L) | Notes |
|---|---|---|
| 2x Platypus bags (full) | ~8.0 | Bounding box, actual liquid volume is 4.0L |
| Cartridge dock (with clearance) | ~4.3 | 180 x 230 x 130mm including lever swing |
| Electronics (ESP32, L298N x2, RTC) | ~0.6 | With DIN rail and wiring clearance |
| Solenoid valves (x4) | ~0.5 | 80 x 35 x 45mm each |
| Flow meter + needle valve | ~0.2 | Small inline components |
| Displays (x2, with holders) | ~0.2 | Flush-mounted, minimal internal volume |
| Power supply | ~0.2 | If internal |
| Tubing routing space | ~1.0 | Bends, runs, slack |
| Hopper/funnel | ~0.5 | Depends on design |
| **Total component volume** | **~15.5** | |
| **Packing efficiency (~60%)** | **~26 L** | Enclosure gross internal volume needed |

A 26L internal volume corresponds to roughly a 300 x 300 x 300mm (12" x 12" x 12") cube, or equivalent rectangular prism. This is a reasonable starting point.

---

## 2. The PC Tower Analogy

### 2a. Where the Analogy Holds

| PC Tower Feature | Enclosure Equivalent | Fit Quality |
|---|---|---|
| Front I/O panel (USB, audio, power button) | Displays (S3, RP2040), status LEDs | Strong — daily glance interaction, rarely touched |
| 5.25" drive bay (CD/DVD) | Cartridge dock slot | Strong — slide-in/out replaceable module, front access |
| Internal volume (motherboard, PSU, drives) | Bags, electronics, valves, wiring | Strong — user never sees this after assembly |
| Back panel (I/O shield, power inlet, fan) | Water connections, power, air switch pass-through | Strong — set-and-forget connections |
| Top panel (optional I/O, ventilation) | Hopper inlet | Moderate — top access works well for pouring |
| Side panels (removable for service) | Removable panel for bag replacement / internal service | Strong — needed for initial setup and troubleshooting |
| Cable management (tie-downs, channels) | Tubing routing channels, wire management | Strong |

### 2b. Where the Analogy Breaks Down

**Weight distribution**: A PC tower's center of gravity is roughly centered. This enclosure has 4+ kg of liquid (two full bags) that shifts as fluid is consumed. The enclosure needs a wide base or low center of gravity to avoid tipping.

**Fluid handling**: PCs are dry systems. This enclosure has internal water/flavoring, external water connections, and the potential for leaks. A PC doesn't need drip trays, sealed compartments, or drainage paths.

**The hopper**: PCs have nothing analogous to a top-loading funnel that receives liquid poured by the user. This is the biggest departure from the tower metaphor.

**Organic shapes**: Bags are soft and irregularly shaped. PCs have rigid rectangular components that pack neatly. Bags need room to expand and contract.

### 2c. Alternative Product Archetypes

| Archetype | What It Borrows | Where It Falls Short |
|---|---|---|
| **Under-sink water filter system** (3M, Waterdrop) | Back-wall mount, front cartridge access, water connections, under-sink environment | Too small — doesn't accommodate bags, hopper, or displays |
| **Mini fridge / wine cooler** | Self-contained appliance, front door access, internal reservoirs | Too heavy, too large, implies cooling (which we don't need) |
| **Coffee machine** (bean-to-cup) | Top hopper for beans, front dispensing, internal reservoirs, user-facing displays | Very strong analogy. Water reservoir + bean hopper + front UI + internal pumps/valves. Size is similar. |
| **Kegerator / draft beer system** | Liquid bags, pump system, dispensing, under-counter installation | Good for bag storage and pump concepts, but kegerators are much larger |
| **Aquarium canister filter** | Sealed unit with pumps, tubing connections, under-cabinet placement | Good for sealed-unit concept, but no user interaction beyond connection |

**Strongest analogy: Bean-to-cup coffee machine.** It has a top hopper (beans = flavoring concentrate), internal reservoirs (water tank = bags), front-panel UI (display = our displays), a replaceable module (drip tray / brew group = our cartridge), and back/bottom water connections. The overall interaction pattern is nearly identical. The enclosure should feel like an under-counter version of a high-end coffee machine.

### 2d. Hybrid Mental Model

The recommended mental model combines elements:
- **PC tower** for the box shape, front I/O, back panel, and internal organization
- **Coffee machine** for the hopper, cartridge, and user interaction flow
- **Under-sink water filter** for the mounting context and plumbing conventions

---

## 3. Layout Options

### Coordinate System for All Layouts

- **Width (W)**: left-right as viewed from the front
- **Depth (D)**: front-to-back
- **Height (H)**: floor-to-top

### 3a. Tall Tower (Vertical Emphasis)

Bags hang vertically in the upper portion. Cartridge slides in from the front in the middle. Electronics below. Hopper on top.

**Estimated dimensions: 250W x 200D x 450H mm (10" x 8" x 18")**

```
    FRONT VIEW                    SIDE VIEW                   TOP VIEW
    ┌───────────────┐            ┌──────────────┐            ┌──────────────┐
    │  ╔═══════════╗│            │  ╔══════════╗ │            │              │
    │  ║  HOPPER   ║│            │  ║  HOPPER  ║ │            │  ┌────────┐  │
    │  ║  INLET    ║│            │  ║          ║ │            │  │ hopper │  │
    │  ╚═══════════╝│            │  ╚══════════╝ │            │  │ funnel │  │
    │               │            │               │            │  └────────┘  │
    │  ┌───────────┐│            │ ┌───────────┐ │            │              │
    │  │ BAG 1     ││            │ │  BAG 1    │ │            └──────────────┘
    │  │ (hanging) ││            │ │  BAG 2    │ │                 200 D
    │  │ BAG 2     ││            │ │ (behind)  │ │
    │  │ (behind)  ││            │ └───────────┘ │
    │  └───────────┘│            │               │
    │               │            │ ┌───────────┐ │
    │ ┌─────────────┤            │ │ CARTRIDGE │ │
    │ │ CARTRIDGE  ◄│ slide in   │ │  DOCK     │ │
    │ │ DOCK       │ from front  │ │           │ │
    │ └─────────────┤            │ └───────────┘ │
    │               │            │               │
    │ ┌─────────────┐            │ ┌───────────┐ │
    │ │ ELECTRONICS ││           │ │ VALVES    │ │
    │ │ ESP32, L298N││           │ │ ELECTR.   │ │
    │ │ VALVES      ││           │ │ FLOW MTR  │ │
    │ └─────────────┘│           │ └───────────┘ │
    └───────────────┘            └──────────────┘
         250 W                        200 D
```

**User interactions:**
- Hopper: pour from above, very accessible (top of unit)
- Cartridge: slide in/out from front at mid-height (~250mm up from floor, comfortable reach under sink)
- Displays: front face, above cartridge slot
- Back panel: water connections and power, set-and-forget

**Internal routing:**
- Bags drain downward to cartridge dock (gravity-assisted, short tube runs)
- Cartridge dock connects to valves below via short vertical runs
- Electronics at bottom, protected from any drips by being below the fluid zone (but also most vulnerable to floor water)

**Pros:**
- Natural gravity flow: hopper -> bags -> pumps -> dispensing
- Hopper at top is the most natural pouring position
- Smallest footprint (250 x 200mm = 500 cm^2 floor space)
- PC tower feel — familiar form factor
- Cartridge at mid-height is ergonomically good for under-sink access

**Cons:**
- 450mm (18") tall — needs to fit under sink with clearance for hopper access. Under-sink usable height is 500-710mm, so this fits but the hopper may be at the top of the cabinet, making pouring awkward if the sink bowl is above it
- Bags hanging vertically require a suspension system (clips, hooks, or a rack)
- Narrow (250mm) may make internal access cramped
- If tipped, 4+ kg of liquid creates a splash hazard

---

### 3b. Wide/Short (Horizontal Emphasis)

Bags lay flat or on a slight incline. Cartridge on front face. Wider footprint, lower profile.

**Estimated dimensions: 400W x 220D x 250H mm (16" x 9" x 10")**

```
    FRONT VIEW                    SIDE VIEW                   TOP VIEW
    ┌─────────────────────────┐  ┌──────────────┐            ┌──────────────────────────┐
    │ [S3] [RP]  HOPPER DOOR  │  │  ┌────────┐  │            │                          │
    │                         │  │  │ BAG 1   │  │            │  ┌──────┐ ┌──────┐       │
    │ ┌───────────┐  ┌──────┐│  │  │ BAG 2   │  │            │  │ BAG1 │ │ BAG2 │       │
    │ │ CARTRIDGE │  │HOPPER││  │  │ (flat)  │  │            │  │      │ │      │       │
    │ │ DOCK     ◄│  │CHUTE ││  │  └────────┘  │            │  └──────┘ └──────┘       │
    │ │           │  │      ││  │  ┌────────┐   │            │  ┌──────┐  ┌──────┐      │
    │ └───────────┘  └──────┘│  │  │CART.   │   │            │  │ DOCK │  │ELECT.│      │
    │ ┌─────────────────────┐│  │  │VALVES  │   │            │  └──────┘  └──────┘      │
    │ │  ELECTRONICS/VALVES  ││  │  │ELECTR. │   │            │                          │
    │ └─────────────────────┘│  │  └────────┘   │            └──────────────────────────┘
    └─────────────────────────┘  └──────────────┘                      220 D
              400 W                    220 D
```

**User interactions:**
- Hopper: front-accessible door or angled chute on the right side of the front face. Less natural than pouring from above — requires a funnel design that routes liquid to the bags.
- Cartridge: front face, lower-left area
- Displays: top-left of front face
- Back panel: standard

**Internal routing:**
- Bags sit flat above the electronics/valve layer
- Tubing runs are longer (horizontal from bags to dock)
- More complex routing due to horizontal layout

**Pros:**
- Low profile (250mm / 10") — fits easily under sinks with garbage disposals that limit headroom
- Wide footprint gives a stable base (no tipping risk)
- Bags lying flat are naturally supported (no suspension needed)
- Easy internal access with a top-removable lid

**Cons:**
- 400mm (16") wide — may not fit in the available 250-350mm side zones next to plumbing. Would need to straddle the center or use a wider cabinet.
- Hopper access from the front is less natural than pouring from above
- Bags lying flat don't drain as well (liquid pools at the lowest point, not necessarily at the outlet)
- Larger floor footprint

---

### 3c. Cube

Roughly equal dimensions. Components arranged around a central volume.

**Estimated dimensions: 300W x 300D x 300H mm (12" x 12" x 12")**

```
    FRONT VIEW                    SIDE VIEW                   TOP VIEW
    ┌───────────────────┐        ┌───────────────────┐       ┌───────────────────┐
    │  [S3]  [RP]       │        │   ╔═══HOPPER═══╗  │       │  ╔═══════════╗    │
    │                   │        │   ║             ║  │       │  ║  HOPPER   ║    │
    │  ┌─────────────┐  │        │   ╚═════════════╝  │       │  ╚═══════════╝    │
    │  │  CARTRIDGE  │  │        │  ┌──────┐┌──────┐  │       │ ┌──────┐┌──────┐  │
    │  │  DOCK      ◄│  │        │  │BAG 1 ││BAG 2 │  │       │ │BAG 1 ││BAG 2 │  │
    │  │             │  │        │  │      ││      │  │       │ │      ││      │  │
    │  └─────────────┘  │        │  │      ││      │  │       │ └──────┘└──────┘  │
    │  ┌─────────────┐  │        │  └──────┘└──────┘  │       │ ┌──────────────┐  │
    │  │ ELECTRONICS │  │        │  ┌──────────────┐  │       │ │   DOCK       │  │
    │  │ + VALVES    │  │        │  │ VALVES+ELECT │  │       │ │  ELECTRONICS │  │
    │  └─────────────┘  │        │  └──────────────┘  │       │ └──────────────┘  │
    └───────────────────┘        └───────────────────┘       └───────────────────┘
           300 W                        300 D                       300 D
```

**User interactions:**
- Hopper: top, accessible (similar to tall tower)
- Cartridge: front face, mid-height
- Displays: front face, above cartridge
- Back panel: standard

**Internal routing:**
- Bags arranged side by side in the upper half, behind the hopper
- Dock in the middle of the front face
- Electronics and valves in the lower rear
- Moderate tube run lengths

**Pros:**
- Compact — 27L total volume, efficient packing
- Balanced proportions, looks like a product
- 300mm (12") on each side fits well in under-sink side zones (250-350mm wide)
- Hopper on top is natural
- Good gravity flow (bags above pumps)

**Cons:**
- 300mm (12") wide may be tight for a 140mm-wide cartridge dock plus bags side by side
- Internal access requires removing multiple panels or a clamshell design
- Dense packing makes routing and maintenance harder
- Cartridge dock at 300mm deep limits the depth available for bags behind it

---

### 3d. L-Shape / Stepped

Main body is shorter, with a raised section for the hopper. Lower profile where it matters, taller where access is needed.

**Estimated dimensions: 300W x 220D x 350H (tall section) / 200H (short section) mm**

```
    FRONT VIEW                    SIDE VIEW
    ┌─────────┐                  ┌─────────┐
    │ HOPPER  │                  │ HOPPER  │
    │ FUNNEL  │                  │ FUNNEL  │
    │         │                  │         │
    │ ┌─────┐ │                  │ ┌─────┐ │
    │ │BAGS │ │  ┌───────────┐  │ │BAGS │ │ ┌──────────┐
    │ │     │ │  │[S3] [RP]  │  │ │     │ │ │          │
    │ │     │ │  │           │  │ │     │ │ │ ELECTR.  │
    │ │     │ │  │ CARTRIDGE◄│  │ └─────┘ │ │ VALVES   │
    │ └─────┘ │  │ DOCK      │  │ ┌─────┐ │ │          │
    │ VALVES  │  │           │  │ │DOCK │ │ │          │
    └─────────┘  └───────────┘  │ └─────┘ │ └──────────┘
       150 W        150 W       └─────────┘
    ├────── 300 W total ──────┤      220 D
```

**User interactions:**
- Hopper: top of the tall section on the left — natural pour position
- Cartridge: front face of the shorter right section — mid-height, very accessible
- Displays: front face of shorter section, above cartridge
- Back panel: spans full width

**Internal routing:**
- Bags hang or sit in the tall left section above the valves
- Tubes run laterally from bags to the dock in the right section
- Electronics in the right section below/behind the dock

**Pros:**
- Hopper elevated for easy pouring, but the rest of the enclosure stays low-profile
- Cartridge access is at a comfortable height in the shorter section
- Interesting visual form factor — looks intentionally designed, not just a box
- The tall section can be placed against the back wall, short section facing forward

**Cons:**
- More complex enclosure shape — harder to manufacture (more panels, joints)
- Lateral tube runs from bags to dock add complexity
- L-shape may not fit as neatly in the rectangular under-sink zones
- The tall section (350mm) still needs adequate headroom under the sink

---

### 3e. Front-Loading Tower

Optimized for all user interaction from the front face. Cartridge slides in from the front. Hopper has a front-accessible door/funnel. Displays on front. The user never needs to access the top or sides.

**Estimated dimensions: 280W x 250D x 400H mm (11" x 10" x 16")**

```
    FRONT VIEW                    SIDE VIEW                   TOP VIEW
    ┌───────────────────┐        ┌───────────────────┐       ┌───────────────────┐
    │  ┌─────────────┐  │        │                   │       │                   │
    │  │ HOPPER DOOR │  │        │  ┌─HOPPER CHUTE─┐ │       │   ┌───────────┐   │
    │  │ (flip open, │  │        │  │  angled to   │ │       │   │   BAGS    │   │
    │  │  pour in)   │  │        │  │  bags behind │ │       │   │  (behind  │   │
    │  └─────────────┘  │        │  └──────────────┘ │       │   │   hopper) │   │
    │                   │        │  ┌──────────────┐ │       │   └───────────┘   │
    │  [S3]      [RP]   │        │  │  BAGS        │ │       │   ┌───────────┐   │
    │                   │        │  │  (tilted,    │ │       │   │   DOCK    │   │
    │  ┌─────────────┐  │        │  │   drain fwd) │ │       │   │  + VALVES │   │
    │  │ CARTRIDGE  ◄│  │        │  └──────────────┘ │       │   └───────────┘   │
    │  │ DOCK        │  │        │  ┌──────────────┐ │       │                   │
    │  │             │  │        │  │  DOCK +      │ │       └───────────────────┘
    │  └─────────────┘  │        │  │  CARTRIDGE   │ │             250 D
    │                   │        │  └──────────────┘ │
    │  ┌─────────────┐  │        │  ┌──────────────┐ │
    │  │ ELECTRONICS │  │        │  │  ELECTRONICS │ │
    │  │   + VALVES  │  │        │  │  + VALVES    │ │
    │  └─────────────┘  │        │  └──────────────┘ │
    └───────────────────┘        └───────────────────┘
           280 W                       250 D
```

**User interactions:**
- Hopper: flip-open door on the front face (top section). A molded chute angles liquid backward and down to the bags. The user pours from the front without reaching over or behind the unit.
- Cartridge: front face, mid-height
- Displays: front face, between hopper door and cartridge
- Back panel: all permanent connections

**Internal routing:**
- Bags sit behind the hopper chute, tilted slightly forward so liquid drains toward the front (where pump inlets connect)
- Dock below bags, tubes run straight down from bag outlets to dock fittings
- Electronics and valves at the bottom
- Back panel connections route through the rear

**Pros:**
- All user interactions are on one face — the user never reaches around, over, or behind the unit
- Works well pushed against a side wall with only the front exposed
- Hopper door is accessible even if the top of the unit is close to the underside of the sink
- Bags tilted toward the front aid gravity drainage to pump inlets
- Clean product feel — one interaction face, everything else hidden

**Cons:**
- Front-loading hopper requires an internal chute that takes up volume
- 400mm (16") tall — needs headroom, though less than the tall tower
- 280mm (11") wide may be tight for some under-sink side zones
- The hopper door mechanism adds complexity (hinge, seal, drip management)
- Bags behind the hopper are not visible or accessible from the front without removing a panel

---

### 3f. Deep Shelf (Depth-Optimized)

Uses the full 500mm+ depth of the under-sink cabinet. Very compact front profile, extends deep into the cabinet.

**Estimated dimensions: 250W x 450D x 280H mm (10" x 18" x 11")**

```
    FRONT VIEW                    SIDE VIEW (key view)
    ┌───────────────┐            ┌──────────────────────────────────────┐
    │ HOPPER        │            │                                      │
    │ (top, front)  │            │ HOPPER    BAG 1    BAG 2            │
    │               │            │ (front)   (middle) (rear)           │
    │ [S3]   [RP]   │            │                                      │
    │               │            │ ┌──────┐ ┌────────────────────────┐  │
    │ ┌───────────┐ │            │ │DOCK  │ │    BAGS (tilted,      │  │
    │ │CARTRIDGE ◄│ │            │ │      │ │    drain forward)     │  │
    │ │DOCK       │ │            │ └──────┘ └────────────────────────┘  │
    │ └───────────┘ │            │ ┌──────────────────────────────────┐ │
    │               │            │ │  ELECTRONICS  VALVES  FLOW MTR  │ │
    │ ELECTRONICS   │            │ └──────────────────────────────────┘ │
    └───────────────┘            └──────────────────────────────────────┘
         250 W                                  450 D

    TOP VIEW
    ┌──────────────────────────────────────┐
    │  HOPPER   │  BAG 1     │  BAG 2     │
    │  + DOCK   │            │            │
    │  (front)  │  (middle)  │  (rear)    │
    │           │            │            │
    │  VALVES   │  ELECTRONICS            │
    └──────────────────────────────────────┘
    ▲ front                          rear ▲
         250 W
```

**User interactions:**
- Hopper: top-front corner, accessible
- Cartridge: front face, mid-height
- Displays: front face
- Back panel: rear — very deep, set-and-forget (which is fine since those connections are install-once)

**Internal routing:**
- Bags extend deep into the cabinet behind the dock/electronics
- Long tube runs from rear bags to front dock
- Electronics near the front for any future USB access

**Pros:**
- Smallest front profile (250 x 280mm = 700 cm^2) — fits the tightest under-sink side zones
- Uses otherwise wasted cabinet depth
- Low profile (280mm) works with garbage disposals limiting height
- Good stability (wide base relative to height)

**Cons:**
- 450mm (18") deep — extends far into the cabinet. Anything behind the unit is inaccessible.
- Long tube runs from bags to pumps increase fluid path length
- Bags in the rear are hard to inspect or replace
- The unit itself is hard to slide out for maintenance (heavy with full bags)

---

## 4. Gravity and Fluid Flow Considerations

### 4a. Vertical Arrangement Priorities

The system has several fluid flow paths that benefit from specific elevation relationships:

```
    IDEAL VERTICAL ARRANGEMENT

    TOP:    Hopper inlet (pour in)
              │
              ▼ gravity
    UPPER:  Bags (reservoirs)
              │
              ▼ gravity assists flow to pump inlets
    MIDDLE: Pump cartridge (peristaltic pumps)
              │
              ▼ pump pressure drives fluid out
    LOWER:  Solenoid valves → flow meter → dispensing faucet
              │
              (exits enclosure via back panel)

    ALSO:   Tap water inlet (back panel, any height)
              │
              ▼ or → clean cycle solenoids → needle valve → bags
```

### 4b. Why Bags Above Pumps Matters

Peristaltic pumps create suction by deforming a tube, but they work significantly better when pre-primed (fluid already at the pump inlet). If bags are above the pumps:

- **Gravity pre-primes the pump inlet tubing** — no air gap between bag and pump
- **Reduced cavitation risk** — the pump doesn't need to pull fluid up against gravity
- **More consistent flow rate** — the pump works with gravity, not against it
- **Bag drainage is complete** — liquid drains to the lowest point (the outlet), so less residual liquid in a "empty" bag

If bags are below or at the same level as pumps:
- Pumps must create suction to pull fluid up (peristaltic pumps can do this, but it's less reliable)
- Air bubbles are more likely to form in the inlet line
- Bags may not drain completely — liquid pools at the lowest point, which may not be the outlet

**Conclusion**: Bags should be above the pump cartridge. A minimum of 50-100mm of elevation difference provides meaningful gravity assist.

### 4c. Hopper-to-Bag Flow

The hopper is where the user pours flavoring concentrate. The concentrate needs to reach the bags. Options:

1. **Gravity-fed**: Hopper above bags, concentrate flows down through a tube. Simple, reliable, but requires the hopper to be the highest point.
2. **Pump-assisted**: A separate small pump moves concentrate from the hopper to the bags. Allows hopper at any height but adds complexity and cost.
3. **Direct pour**: The user opens the bag and pours directly. No hopper needed, but requires bag access (removing a panel, opening a door). Least convenient for a weekly interaction.

For the MVP, gravity-fed from a hopper above the bags is the simplest approach. This means the hopper must be at or near the top of the enclosure.

### 4d. Clean Cycle Flow Path

The clean cycle routes tap water through the system to flush residual concentrate:

```
    Tap water inlet (back panel)
         │
         ▼
    Clean cycle solenoid valve (12V, normally closed)
         │
         ▼
    Needle valve (flow rate control)
         │
         ▼
    Tee into bag inlet line (or directly into bag)
         │
         ▼
    Bag fills with clean water
         │
         ▼
    Peristaltic pump draws from bag, pushes through dispensing line
         │
         ▼
    Dispensing solenoid (normally closed, opens during clean)
         │
         ▼
    Out to dispensing faucet → drain
```

Elevation requirements:
- Tap water has its own pressure (~40-60 PSI), so it can flow upward — the clean solenoid and needle valve can be at any height
- The tee connection to the bag line should be at or above the bag to prevent backflow into the tap water line when the clean solenoid is closed (or use a check valve)
- The bag-to-pump path follows the same gravity rules as normal operation

### 4e. Bag Drainage Geometry

When a bag is nearly empty, the remaining liquid must collect at the outlet. For a hanging bag (vertical orientation), gravity naturally pulls liquid to the bottom where the outlet is. For a flat bag:

- If the outlet is at one end and the bag is tilted even 5-10 degrees toward the outlet, liquid collects there
- If the bag is truly flat, liquid pools randomly and the pump may draw air before the bag is empty
- A slight tilt mechanism (3D printed cradle with an angled floor) solves this

**Recommendation**: Either hang bags vertically (outlet at bottom) or lay them on a 5-10 degree incline toward the outlet. Vertical hanging is more space-efficient; inclined flat is easier to support structurally.

---

## 5. Thermal Considerations

### 5a. Heat Sources

| Component | Duty Cycle | Power (W) | Heat per Hour (Wh) | Notes |
|---|---|---|---|---|
| Kamoer KPHM400 pump x2 | ~5% (active during dispensing) | ~14W (both) | ~0.7 | Short bursts: a 12oz glass takes ~10-15 seconds |
| L298N motor driver x2 | Same as pumps | ~6-12W (both) | ~0.3-0.6 | Voltage drop loss, proportional to current |
| Solenoid valves (dispensing x2) | Same as pumps | ~11W (both) | ~0.6 | Only energized during dispensing |
| Solenoid valves (clean x2) | Rare (clean cycle only) | ~11W (both) | Negligible | Clean cycle runs a few minutes every few weeks |
| ESP32 | 100% (always on) | ~0.5W | ~0.5 | WiFi/BLE active |
| Power supply losses | 100% | ~1-2W | ~1-2 | Transformer/regulator inefficiency |
| **Peak (during dispensing)** | | **~33W** | | All pumps + valves + drivers simultaneously |
| **Idle** | | **~2W** | | ESP32 + PSU losses only |

### 5b. Thermal Analysis

The enclosure is in an enclosed cabinet with limited natural airflow. Key questions:

**Peak vs. sustained**: Peak power is ~33W but only occurs during dispensing (seconds at a time). Sustained power is ~2W. The thermal design needs to handle brief 33W bursts, not continuous 33W. Over a 24-hour period with moderate use (20 glasses/day x 15 seconds = 5 minutes of active time), total heat generation is about:

- Active: 33W x 300s = 9,900 J = 2.75 Wh
- Idle: 2W x 23.9h = 47.8 Wh
- **Total daily: ~50 Wh** — about 180 kJ

This is modest. For comparison, a 60W light bulb generates this much heat in under an hour.

**Temperature rise estimate**: The enclosure has a surface area of roughly 0.5 m^2 (for a 300x300x300 cube). With 2W of sustained heat in a poorly ventilated cabinet at 25C ambient:

- Natural convection from a box: roughly 5-10 W/(m^2*K)
- Temperature rise = Power / (h * A) = 2W / (7.5 * 0.5) = ~0.5C above cabinet ambient

This is negligible. Even at peak, the temperature rise would be temporary and under 10C.

**Does the enclosure need ventilation openings?**

No forced ventilation is needed. Passive ventilation (a few vent holes or slots near the top and bottom) is sufficient and good practice for moisture management, but active cooling (fans) would be over-engineering for this thermal load.

### 5c. Heat Source Placement

Even though thermal management is not critical, following good practices:

- **L298N motor drivers**: Mount in the upper portion of the electronics zone, near vent slots. Heat rises — let it exit.
- **Pumps**: Inside the cartridge, so their heat exits with the cartridge. During insertion, the dock provides some thermal mass.
- **Solenoid valves**: Intermittent use, minimal concern. Can be placed anywhere.
- **ESP32**: Low power, no placement constraint from thermal perspective.

### 5d. Cabinet Environment

The under-sink cabinet itself is a semi-enclosed space. With the cabinet doors closed, temperatures can be 2-5C above room temperature due to hot water pipes. The enclosure components are all rated for 0-40C or better operating range. No concerns.

---

## 6. Water Protection

### 6a. External Water Threats

The enclosure sits under a kitchen sink. Water hazards include:

| Threat | Likelihood | Severity | Notes |
|---|---|---|---|
| Splash from sink drain leak | Medium | High | P-trap connections can weep; drips land on anything below |
| Splash during garbage disposal use | Low | Medium | Contained in the sink bowl, but spray can exit |
| Supply line leak | Low | Very High | Pressurized — would spray or drip continuously |
| Floor water (mopping, spill from above) | Medium | Medium | Water pools on cabinet floor |
| Condensation on cold water pipes | Low | Low | Minor dripping in humid weather |

### 6b. Internal Water Threats

| Threat | Likelihood | Severity | Notes |
|---|---|---|---|
| Bag leak | Low | High | 2L of flavoring concentrate on electronics would be catastrophic |
| Tube connection leak (push-connect) | Low-Medium | Medium | Small drips, not pressurized |
| Cartridge drip during change | Medium | Low | Small amount of residual fluid in tubes |
| Hopper overflow | Low | Medium | User overfills the hopper |
| Clean cycle fitting failure | Low | High | Pressurized tap water |

### 6c. Protection Strategies

**Strategy 1: Sealed electronics sub-compartment**
- All electronics (ESP32, L298N x2, RTC) in a sealed inner box with cable glands for wire pass-throughs
- Pros: maximum protection, IP-rated if desired
- Cons: cable glands for every wire, harder to access for debugging, heat retention
- **Verdict**: Worthwhile for production; overkill for prototype. A reasonable middle ground is an electronics shelf with a drip shield above it.

**Strategy 2: Drip tray under the enclosure**
- The enclosure sits on rubber feet above a shallow tray (10-15mm deep)
- External drips from above land on the enclosure top (sloped to shed water) and run off
- Floor water is contained by the tray
- **Verdict**: Simple, effective, should be included in all layouts.

**Strategy 3: Vertical separation — electronics below fluids with a drip barrier**
- A solid shelf between the fluid zone (bags, dock, valves) and the electronics zone
- Any internal leaks drip onto the shelf, which has a drain channel to the front (visible leak indicator)
- **Verdict**: The most practical approach for a prototype. Easy to implement with a 3D-printed or sheet-metal shelf.

**Strategy 4: Electronics above fluids**
- Invert the typical layout: electronics at the top, bags and plumbing below
- Gravity means leaks never reach electronics
- **Problem**: This conflicts with the ideal gravity flow (bags above pumps) and puts the ESP32's USB port at the top of the unit
- **Verdict**: Not recommended. The gravity flow advantage is more important than the leak protection advantage.

### 6d. Cartridge Dock Water Management

When the user removes the cartridge, residual fluid in the tubes can drip:

- The dock should have a drip channel below the fitting openings that routes to the front (visible) or to a small tray
- The front face of the dock should have a slight lip to prevent drips from running down the enclosure front
- The enclosure should tolerate a few mL of drips without any damage

### 6e. Recommended Protection Architecture

1. **Enclosure shell**: Solid on all sides (no open-frame design). Top surface slightly crowned or sloped to shed external drips.
2. **Internal drip shelf**: Solid horizontal shelf between the fluid zone (upper) and electronics zone (lower). Sloped to drain toward a visible indicator or small reservoir at the front.
3. **Drip tray**: Under the enclosure, integral or separate. Catches anything that exits the enclosure bottom.
4. **Vent slots on sides**: Not top or bottom (water ingress). Horizontal louvers that reject drips while allowing airflow.
5. **Cartridge dock drip channel**: Below the fitting openings, routes to the front or to the drip shelf.

---

## 7. Comparison Matrix

Scoring each layout 1-5 (5 = best):

| Criteria | Weight | 3a Tall Tower | 3b Wide/Short | 3c Cube | 3d L-Shape | 3e Front-Loading | 3f Deep Shelf |
|---|---|---|---|---|---|---|---|
| Hopper accessibility | 5 | 5 (top) | 3 (front door) | 5 (top) | 4 (tall section top) | 4 (front door) | 4 (top-front) |
| Cartridge accessibility | 5 | 4 (mid-height front) | 4 (front) | 4 (front) | 5 (short section, easy reach) | 4 (front) | 4 (front) |
| Display visibility | 3 | 4 (front, above dock) | 4 (front) | 4 (front) | 4 (front) | 5 (front, dedicated zone) | 4 (front) |
| Internal routing simplicity | 4 | 5 (vertical, gravity) | 3 (horizontal runs) | 4 (moderate) | 3 (lateral runs) | 4 (vertical, with hopper chute) | 3 (long runs to rear bags) |
| Under-sink fit (various cabinets) | 5 | 3 (18" tall, tight fit) | 2 (16" wide, too wide for side zones) | 4 (12" cube fits most zones) | 3 (complex shape) | 4 (11"W x 16"H fits most) | 5 (10" front, uses depth) |
| Manufacturability (3D print + off-shelf) | 3 | 4 (rectangular) | 4 (rectangular) | 4 (rectangular) | 2 (L-shape, complex) | 3 (front door mechanism) | 4 (rectangular) |
| Water safety | 4 | 3 (electronics at bottom = floor water risk) | 4 (electronics below bags but on solid shelf) | 4 (drip shelf works) | 3 (complex water paths) | 4 (drip shelf, vertical sep.) | 3 (electronics near front floor) |
| Thermal management | 2 | 4 (heat rises to top vents) | 4 (wide surface area) | 4 (adequate) | 3 (complex airflow) | 4 (adequate) | 4 (adequate) |
| Aesthetics / product feel | 3 | 4 (classic tower) | 3 (boxy, not distinctive) | 4 (compact, modern) | 3 (unusual shape) | 5 (clean single-face design) | 3 (deep box, not much front presence) |
| Floor space consumed | 4 | 5 (500 cm^2) | 2 (880 cm^2) | 4 (900 cm^2) | 3 (mixed) | 4 (700 cm^2) | 5 (1125 cm^2 but narrow front) |
| Gravity flow optimization | 4 | 5 (ideal vertical stack) | 2 (bags flat, poor drainage) | 4 (bags above dock) | 3 (lateral flow) | 4 (bags above dock, tilted) | 3 (bags far from pumps) |

### Weighted Scores

| Layout | Score (out of 210) |
|---|---|
| **3a Tall Tower** | 175 |
| **3b Wide/Short** | 131 |
| **3c Cube** | 173 |
| **3d L-Shape** | 137 |
| **3e Front-Loading Tower** | 174 |
| **3f Deep Shelf** | 160 |

### Ranking

**1. Tall Tower (175)** — Best gravity flow, smallest footprint, natural hopper-on-top. The main risk is height: 450mm (18") needs to fit under the sink with clearance for hopper access. If the under-sink cabinet has 500mm+ of usable height in the side zone, this is the strongest layout.

**2. Front-Loading Tower (174)** — Nearly tied with Tall Tower. Wins on aesthetics and the single-interaction-face principle. Slightly penalized for the hopper door mechanism complexity and the internal chute volume. This is the "product-ready" layout — the one you'd see in a Kickstarter render.

**3. Cube (173)** — Very close third. Excellent all-around balance. Slightly smaller than the Tall Tower, slightly harder to optimize internal routing. A strong compromise if height is constrained.

**4. Deep Shelf (160)** — Good for tight spaces where width and height are constrained but depth is available. Practical but less product-like.

**5. L-Shape (137)** — Interesting concept but the manufacturing complexity and routing challenges push it down. Could work if the cabinet has an unusual shape that rewards an L profile.

**6. Wide/Short (131)** — The 400mm width is the fatal flaw. Most under-sink side zones are 250-350mm wide. This layout only works in a very wide cabinet or in the center (which is blocked by plumbing).

### Recommended Starting Point

**The Tall Tower (3a) or Front-Loading Tower (3e)** should be explored first. Both use the natural vertical stacking (hopper > bags > dock > electronics) that gravity and the user interaction hierarchy demand.

The choice between them depends on whether the top of the enclosure is accessible:
- **If the hopper can be at the very top** and there's room to pour (50-100mm above the hopper opening to the underside of the sink or countertop): **Tall Tower**.
- **If the top of the enclosure is close to the underside of the sink** (no room to pour): **Front-Loading Tower** with a front-accessible hopper door.

A practical approach: design the Tall Tower as the baseline, and add a front hopper door option as a variant if top access proves insufficient in the actual cabinet.

---

## 8. Next Steps

1. **Measure the actual under-sink cabinet** where this will be installed. The zone width, depth, and height (accounting for the sink bowl and any disposal) determines which layouts are feasible.
2. **Prototype the bag suspension/cradle** — the bag arrangement (hanging vs. tilted flat) drives the upper section of the enclosure.
3. **Design the hopper-to-bag connection** — gravity funnel with tubing to each bag, or a splitter valve?
4. **Define the back panel layout** — 4x 1/4" push-connect bulkhead fittings + barrel jack + air tube pass-through. This can be prototyped independently.
5. **Build a cardboard mockup** at the target dimensions and test it in the actual cabinet for fit and interaction ergonomics.

---

## Sources

### Component Dimensions
- [Kamoer KPHM400 Datasheet (Amazon PDF)](https://m.media-amazon.com/images/I/A1at7U9PyNL.pdf) — Pump dimensions, mounting pattern
- [Kamoer KPHM400 Amazon Listing](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D) — Weight
- [Platypus Platy 2L Bottle (Cascade Designs)](https://cascadedesigns.com/products/platy-2l-bottle) — Bag dimensions: 7.5" x 13.8" flat
- [Platypus Platy 2L (Garage Grown Gear)](https://www.garagegrowngear.com/products/platy-2l-bottle-collapsible-bottle-by-platypus) — Bag flat dimensions
- [ESP32-DevKitC V4 User Guide (Espressif)](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html) — PCB dimensions: 55.3 x 28.0 x 12.9mm
- [DIN Rail Mount ESP32 Breakout (CZH-Labs)](https://czh-labs.com/products/din-rail-mount-screw-terminal-block-breakout-module-board-for-esp32-devkitc) — Breakout board dimensions
- [L298N Motor Driver Module (Components101)](https://components101.com/modules/l293n-motor-driver-module) — Module dimensions: 43 x 43 x 27mm
- [DS3231 RTC Module (Components101)](https://components101.com/modules/ds3231-rtc-module-pinout-circuit-datasheet) — Module dimensions: 38 x 22 x 14mm
- [Elecrow CrowPanel 1.28" Rotary Display](https://www.elecrow.com/crowpanel-1-28inch-hmi-esp32-rotary-display-240-240-ips-round-touch-knob-screen.html) — Display dimensions: 48 x 48 x 33mm, 50g
- [Waveshare RP2040-LCD-0.99-B Wiki](https://www.waveshare.com/wiki/RP2040-LCD-0.99-B) — Display diameter: 33mm
- [DIGITEN Flow Sensor (DIGITEN Shop)](https://www.digiten.shop/collections/counter) — G1/2" sensor: 65 x 30 x 28mm

### Thermal and Electrical
- [L298N Motor Driver Heat Dissipation (Rugged Circuits)](https://www.rugged-circuits.com/the-motor-driver-myth) — Voltage drop: 1.8-3.2V at 1A, thermal resistance 35C/W
- [L298N Thermal Issues (Arduino Forum)](https://forum.arduino.cc/t/l298n-driver-heats-up-very-much/1164864) — Practical heat management
- [L298N Tutorial (LastMinuteEngineers)](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/) — Power dissipation details
- [Kamoer KPHM400 Data Sheet (DirectIndustry)](https://pdf.directindustry.com/pdf/kamoer-fluid-tech-shanghai-co-ltd/kphm400-peristaltic-pump-data-sheet/242598-1017430.html) — Motor power specs

### Under-Sink Environment
- [Sink Base Cabinet Dimensions (Casta Cabinetry)](https://castacabinetry.com/post/sink-base-cabinet-dimensions/) — Standard dimensions
- [Kitchen Cabinet Sizes (Kitchen Cabinet Kings)](https://kitchencabinetkings.com/guides/kitchen-cabinet-sizes) — Cabinet interior clearances
- Prior research: [under-cabinet-ergonomics.md](../../cartridge/planning/research/under-cabinet-ergonomics.md) — Detailed zone analysis, sight lines, reach depth
- Prior research: [dock-mounting-strategies.md](../../cartridge/planning/research/dock-mounting-strategies.md) — Mounting location survey and scoring

### Cartridge Design (Prior Research)
- [cartridge-envelope.md](../../cartridge/planning/research/cartridge-envelope.md) — Pump arrangements, envelope sizing, weight
- [cam-lever.md](../../cartridge/planning/research/cam-lever.md) — Lever mechanism, clearance requirements
- [mating-face.md](../../cartridge/planning/research/mating-face.md) — Fitting layout, electrical contacts
- [requirements.md](../../cartridge/planning/requirements.md) — Functional requirements, constraints

### Product Analogies
- [3M Under Sink RO Systems (Solventum)](https://www.solventum.com/en-us/home/f/b5005118094/) — Water filter form factor
- [DuPont QuickTwist Filtration (Amazon)](https://www.amazon.com/DuPont-WFQT390005-QuickTwist-Drinking-Filtration/dp/B007VZ2PH8) — Under-sink filter reference
- [Waterdrop TSU System](https://www.waterdropfilter.com/) — Compact under-sink filter dimensions
