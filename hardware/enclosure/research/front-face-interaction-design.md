# Front Face & User Interaction Surface Design

Research and design exploration for the front-facing panel of the soda flavor injector enclosure. The enclosure sits inside a kitchen sink cabinet. When the user opens the cabinet door, the front face is the primary interaction surface. This document covers display holders, cable management, cartridge slot integration, hopper access, visual hierarchy, layout options, and surface finish.

---

## 1. Display Holder Design

The front face hosts two round displays:

| Display | Module | Visible Diameter | Overall Module Size | Resolution | Function |
|---------|--------|-----------------|---------------------|------------|----------|
| **S3 config** | Meshnology/Elecrow CrowPanel 1.28" | ~33mm (1.28" panel) | **48 x 48 x 33mm** (aluminum + plastic + acrylic shell) | 240x240 | Touch + rotary encoder. Settings, prime, clean, BLE bridge. |
| **RP2040 flavor** | Waveshare RP2040-LCD-0.99-B | ~28mm visible (0.99" panel) | ~33mm dia x ~12mm thick (CNC metal case) | 128x115 | Current flavor image. Glance-only, no touch. |

The S3 module weighs ~50g. The RP2040 module weighs approximately 25-30g (CNC aluminum case). Both are light enough for any retention mechanism.

### 1a. Pop-Out Mechanism Options

The user can pop either display out of its holder for external mounting (e.g., on the cabinet door, a shelf, or above the counter). The holder must look clean when occupied and allow easy removal.

#### Option 1: Magnetic Retention

**How it works:** 3-4 neodymium disc magnets (6x3mm) embedded in the holder ring and matching magnets (or ferrous steel discs) in the display housing/adapter. The display drops into a shallow recess and the magnets snap it into place.

**What the user sees (display in place):** A flush circular display face sitting in a smooth ring. No clips, tabs, or hardware visible. The display face and the surrounding holder ring form a continuous surface with a ~0.5mm gap.

**What the user sees (display removed):** A clean circular recess (socket) with a smooth interior. The magnets are embedded in the plastic and not visible. The recess could be painted/finished to match the front face. If desired, a thin magnetic cover disc could snap into the empty socket.

**What it feels like:** The user grips the display edges and pulls straight out. There is a satisfying magnetic "click" as it detaches. Re-docking: align and push in, magnets guide the last ~2mm of travel and snap it flush. Very similar to placing an AirPods case on a MagSafe charger.

**Magnet sizing:** For a 48mm module, 4x 6x3mm N52 neodymium disc magnets provide ~2.5-3 lbs of pull force total. This is more than enough to hold a 50g display firmly against vibration, but easily overcome by a deliberate pull. For the smaller 33mm RP2040 module, 3x 6x3mm magnets provide ~2 lbs.

**Magnet proximity to electronics:** The displays use capacitive touch (S3) and SPI LCD (RP2040). Neither is sensitive to static magnetic fields. The magnets are in the holder and adapter shell, not on the PCB. UART signals are unaffected. The only concern would be a mechanical watch on the user's wrist, which is not relevant here.

**Pros:**
- Cleanest appearance of any option. No visible mechanical features.
- Self-aligning (magnets pull into position).
- Infinite cycle life (magnets don't wear).
- Satisfying tactile feedback.

**Cons:**
- Requires embedding magnets during printing (pause-and-insert) or gluing into printed pockets.
- Retention force is fixed (can't be adjusted without swapping magnets).
- Magnets are a small additional cost (~$0.10-0.30 each).
- Weaker than mechanical retention for side-load resistance (but side-loads are minimal in a cabinet).

**Verdict:** Best option for a product-quality front face.

#### Option 2: Snap-Fit Clips

**How it works:** 2-4 printed cantilever clips on the holder engage detents or ridges on the display housing. The display pushes in and the clips flex outward, then snap over the ridge to lock.

**What the user sees (display in place):** The display face is flush, but 2-4 small clip tabs are visible around the perimeter (each ~2-3mm wide, protruding ~0.5mm from the holder surface). On a 48mm circle, these are noticeable but not ugly.

**What the user sees (display removed):** The clips protrude slightly inward into the empty socket. Less clean than the magnetic option.

**What it feels like:** Push in firmly until the clips snap over (satisfying click). To remove, either pull hard enough to flex the clips back, or press release tabs on the clips. The click is mechanical and definitive.

**Clip design for FDM:** Formlabs recommends cantilever beams 5-10mm long, 0.5mm deflection for PETG, with a 30-45 degree entry angle and 90 degree retention face. For repeated insertion/removal, 0.3-0.5mm deflection is safer to avoid fatigue cracking. PETG is significantly better than PLA for snap-fit longevity.

**Pros:**
- No additional parts (fully printed).
- Positive mechanical retention.
- Definitive tactile click.

**Cons:**
- Clips are visible on the front face.
- FDM print quality must be high (clip tips are small features).
- Clips wear over time with repeated cycles (~50-200 cycles before loosening, depending on design).
- Harder to get the "just right" force balance with FDM tolerances.

**Verdict:** Good functional option but inferior aesthetics.

#### Option 3: Bayonet Twist-Lock

**How it works:** The display housing has 2-3 L-shaped tabs. Push in, rotate 15-30 degrees, and the tabs lock behind matching features in the holder. Reverse to remove.

**What the user sees:** A thin ring with bayonet slots visible around the display. Functional but industrial.

**What it feels like:** Push and twist. Feels like installing a light bulb or a camera lens.

**Pros:**
- Very secure retention.
- Well-understood mechanism.
- Can be printed without additional parts.

**Cons:**
- Rotation is awkward for a small round display in a recessed holder.
- Bayonet slots are visible and add visual complexity.
- Requires precise print tolerances for the L-channels.
- The S3 has a rotary encoder ring — accidental rotation while trying to bayonet-lock could trigger encoder input.

**Verdict:** Over-engineered for this application.

#### Option 4: Friction Fit in a Precision Recess

**How it works:** The display housing is a close-tolerance fit in a cylindrical pocket. No clips, no magnets — just a snug fit.

**What the user sees:** A flush display in a smooth socket. Cleanest possible appearance (even cleaner than magnetic, since there are no embedded features).

**What it feels like:** Push in with slight resistance. Pull out with slight resistance. No click, no snap — just friction.

**Pros:**
- Simplest design. Zero additional parts.
- Cleanest look.

**Cons:**
- FDM tolerances of +/- 0.2-0.3mm make this unreliable. Too tight and the display won't seat; too loose and it falls out.
- No positive retention — vibration or a bump could unseat the display.
- Temperature and humidity changes alter fit (PETG thermal expansion).
- No tactile feedback — the user is never sure if it's "in."

**Verdict:** Unreliable. Not recommended.

#### Recommendation: Magnetic Retention

Magnetic retention provides the best balance of aesthetics, reliability, and user experience. The cost is minimal ($1-2 in magnets per display holder), the design is straightforward, and the result looks and feels like a consumer product.

**Specification:**
- S3 holder: 4x 6x3mm N52 neodymium disc magnets, evenly spaced around a 48mm ring
- RP2040 holder: 3x 6x3mm N52 magnets around a 33mm ring
- Matching steel discs (not magnets) in the display adapter to avoid polarity issues during docking
- Recess depth: module thickness + 0.5mm (so the display face sits ~0.5mm proud of the front panel, creating a subtle "watch face" effect)

---

### 1b. Retracting Cat6 Cable Mechanism

Both displays connect to the ESP32 via UART over cat6 cable. When popped out, the cable must extend. When re-docked, excess cable must be managed.

#### Cat6 Cable Specifications

| Parameter | Value |
|-----------|-------|
| Conductors | 8 (4 twisted pairs) |
| Wire gauge (standard) | 24 AWG |
| Cable outer diameter (standard) | 5.5-6.2mm |
| Cable outer diameter (slim/28 AWG) | 3.8-4.0mm |
| Minimum bend radius (standard) | 25mm (4x OD) |
| Minimum bend radius (slim) | 15mm (4x OD) |
| Conductors actually used | 4 (TX, RX, VCC, GND) — only 2 of 4 pairs needed |

**Why cat6?** It is a convenient, cheap, readily available multi-conductor cable with reliable RJ45 connectors. The UART signals (115200 baud) are well within cat6 capabilities. Only 4 of the 8 conductors are used (UART TX, RX, 5V power, GND).

**Alternative cable:** Since only 4 conductors are needed, a thinner cable could be used. A 4-conductor 28AWG cable would be ~3mm OD with a bend radius of ~12mm. This would make retraction mechanisms smaller and easier. However, cat6 is already in use and the RJ45 connectors are convenient.

#### Cable Length Requirements

| Mounting Scenario | Cable Length Needed |
|-------------------|-------------------|
| Display in holder (flush) | ~100mm (internal routing from holder to enclosure interior) |
| Display on cabinet door | ~600-900mm (hinge side to back of cabinet) |
| Display on adjacent shelf | ~900-1200mm (up and over to a shelf) |
| Display above counter (through drilled hole) | ~1500-2000mm (up through cabinet, through counter/wall) |

A **1-meter (3.3 ft) retractable cable** covers the most common external mounting scenarios. A 2-meter cable covers the above-counter scenario.

#### Option 1: Spring-Loaded Retractable Reel

**How it works:** A small spring-loaded spool inside the enclosure, identical in principle to a retractable badge reel or retractable USB cable. The cable winds around the spool. The user pulls the display, cable extends. Release or push back, and the spring retracts the cable.

**Commercially available mechanisms:**
- Badge reel mechanisms: ~25mm diameter spool, 15-24" (380-600mm) cord, designed for thin cord (~1mm). Too small for cat6.
- Retractable USB cable reels: Available in compact housings ~80x80x30mm for 1m cable lengths. These use a flat coil spring similar to a tape measure.
- Hunter Spring & Reel makes professional retractable data cable reels, but their smallest is designed for 20+ feet and is far too large (wall/ceiling mount, industrial).
- Small custom reels from Chinese manufacturers (Yongsheng, SuperReels) are available in compact sizes: ~80mm diameter spool for 1m of 4-6mm cable. Minimum order quantities apply but individual samples are often available.
- The YOTQUSKI cord reel stores up to 3.25 ft of cable in a 80x80x30mm (3.15"x3.15"x1.18") housing.

**Design constraints for cat6:**
- Cat6 standard (6mm OD): minimum spool core diameter ~50mm (to maintain bend radius). With 1m of cable wound, spool OD ~80mm. Housing: ~90x90x15mm per reel.
- Cat6 slim (4mm OD): minimum spool core diameter ~30mm. With 1m of cable, spool OD ~55mm. Housing: ~65x65x15mm.

**Pros:**
- Clean, automatic cable management.
- The user never deals with slack cable.
- Feels premium and intentional.

**Cons:**
- Takes up space inside the enclosure (two reels, each ~65-90mm diameter).
- Spring mechanism adds complexity and a potential failure point.
- Cat6 cable is stiff — the spring must be strong enough to reel it in, which means more pull resistance when extending.
- Not a standard off-the-shelf part for this cable type; would need adaptation or custom build.
- Retracting force can tug on the display if mounted externally (annoying).

#### Option 2: Coiled Cable (Telephone Handset Style)

**How it works:** The cable between the enclosure and the display is a coiled/spiral cable, like an old telephone handset cord. When relaxed, it coils to a compact ~150-200mm length. When stretched, it extends to 1-2m. The coil's elastic memory provides retraction.

**Commercially available coiled cables:**
- Telephone handset cords: 4P4C (RJ9/RJ22), 4 conductors, coiled length ~350mm (14"), stretched length ~3m (10 ft). Available from CableWholesale, Belkin, NAC Wire. Cost: $3-8.
- Coiled Ethernet cables: Rare commercially, but custom coiled cat5e/cat6 cables exist from specialty suppliers.
- Coiled multi-conductor cables (Lapp Tannehill): Industrial retractile cables with 4+ conductors, custom lengths. More expensive but designed for exactly this use case.

**DIY approach:** Buy standard cat6 cable, wrap it tightly around a mandrel (e.g., a 12mm dowel), heat with a heat gun to set the coil shape, and cool. This works well with PVC-jacketed cable. The resulting coil has a natural retraction and extends smoothly.

**Coiled cable dimensions:**
- 4-conductor coiled cable (telephone style): ~8-10mm coil OD, ~14" (350mm) coiled length for ~10 ft stretched
- Cat6 coiled (DIY): ~15mm coil OD (6mm cable on 12mm mandrel), ~200mm coiled length for ~1m stretched
- The coiled cable hangs from the display when popped out, visible but tidy

**Pros:**
- Simple, no mechanism, no moving parts inside the enclosure.
- Self-retracting by material memory (no spring to fail).
- Cheap ($3-8 for a telephone cord; free if DIY from existing cat6).
- The coiled aesthetic is retro/fun and communicates "this is connected" clearly.

**Cons:**
- Coiled cable is always visible (hanging from the display when external, dangling in the holder area when internal).
- The coil takes up space — ~200mm of coiled cable needs to be accommodated somewhere.
- Cat6 is stiffer than telephone cord; DIY coiling may not hold shape as well.
- The coil can catch on things inside the cabinet.
- Less "polished product" feel than a concealed retractable reel.

#### Option 3: Simple Slack Loop

**How it works:** A fixed-length cable (1-2m) with excess stored as a loose loop inside the enclosure. The cable exits through a rubber grommet in the front face. When the display is in its holder, the excess cable is coiled and tucked inside the enclosure body.

**Pros:**
- Simplest possible solution. Zero mechanisms.
- Any cable type works.
- No failure modes beyond cable wear.
- Easy to replace the cable.

**Cons:**
- No retraction — when the display is re-docked, the user must manually tuck the cable back inside.
- Loose cable inside the enclosure is messy and can interfere with other components.
- The cable exit grommet is visible on the front face.
- Does not feel like a product; feels DIY.

#### Option 4: Magnetic Breakaway Connector

**How it works:** The display has no permanently attached cable. Instead, a magnetic pogo-pin connector (MagSafe-style) on the display mates with a matching connector on a short pigtail that exits the enclosure. When the display is in its holder, the magnetic connector snaps together automatically. When popped out, the connector detaches cleanly. For external mounting, the user plugs in a longer cable (1-2m extension with magnetic connectors on both ends, or magnetic-to-RJ45 adapter).

**Available magnetic pogo connectors:**
- Adafruit DIY Magnetic Connector, 4-pin right angle (product #5358): 4 pins at 2.54mm pitch, ~2A per pin, embedded magnets with polarity protection. $4.95 per pair.
- Adafruit 6-pin straight (product #5467): 6 pins at 2.2mm pitch. $7.95 per pair.
- Generic magnetic pogo connectors from Amazon: 4-pin, 2A, ~$5-8 per pair.
- Custom connectors from Pomagtor, QH Industrial: MOQ required but designed for exactly this use case.

**For UART (TX, RX, VCC, GND):** A 4-pin connector is sufficient. The Adafruit 4-pin at 2.54mm pitch is an exact match.

**Pros:**
- Cleanest docking experience — display drops in, connection made automatically.
- No cable to manage when docked.
- Breakaway prevents cable damage if the display is yanked.
- The display is truly wireless when undocked (no cable dangling).
- External mounting uses a separate cable, not a mechanism inside the enclosure.

**Cons:**
- The display is disconnected when removed from the holder (no live external use without a separate cable).
- Additional cost per display ($5-10 for the magnetic connector pair).
- Contact resistance of pogo pins can be higher than a soldered connection (but UART at 115200 baud is very tolerant).
- Alignment must be precise for reliable contact (the magnetic self-alignment helps).
- For external mounting, the user needs to purchase/make an extension cable with matching magnetic ends.

#### Option 5: Hybrid — Magnetic Breakaway + Coiled Extension

**How it works:** Combines options 2 and 4. The display has a magnetic breakaway connector for clean docking. When the user wants to mount the display externally, they attach a coiled extension cable with a magnetic connector on one end and a pass-through into the enclosure on the other.

**Pros:**
- Best of both worlds: clean docking AND external mounting capability.
- The coiled cable is only present when needed (not always installed).
- The front face stays clean in the default configuration.

**Cons:**
- More parts (magnetic connectors + coiled cable).
- The user must own and manage an accessory cable.

#### Recommendation: Magnetic Breakaway (Option 4) with Optional Extension

For the "factory default" experience, magnetic breakaway is the best choice. The displays dock flush with zero cable management. For external mounting (a secondary use case), the user attaches a coiled extension cable. This keeps the default experience clean and moves cable complexity to an optional accessory.

**Specification:**
- Adafruit 4-pin magnetic pogo connector ($4.95/pair) at each display
- Internal wiring from connector to ESP32 UART headers (permanent, inside enclosure)
- Optional accessory: 1m coiled 4-conductor cable with magnetic connectors on both ends, for external display mounting

---

### 1c. Display Layout on the Front Face

Two round displays of different sizes need to share the front face with the cartridge slot and possibly hopper access.

#### Display Hierarchy

| Display | Size | Interaction Frequency | Interaction Type | Visual Priority |
|---------|------|----------------------|------------------|----------------|
| RP2040 (flavor) | 28mm visible | Daily | Glance only | **Highest** — the "face" of the system |
| S3 (config) | 33mm visible (48mm with knob) | Weekly | Touch + rotate | **Second** — operational control |

The RP2040 flavor display is the identity of the system. It shows a colorful flavor image and is what the user looks at every time they open the cabinet. It should be the visual focal point.

The S3 config display is the brain. It is larger (including the rotary encoder ring) but used less frequently. It should be accessible but not dominant.

#### Arrangement Options

**Arrangement A: Side by Side (S3 left, RP2040 right)**
```
    ○ S3 (48mm)    ○ RP2040 (33mm)
```
- Total width: ~48 + 15mm gap + 33 = ~96mm
- Natural left-to-right reading: "controls" then "status"
- Both at the same height — equal visual weight
- The S3 is larger and draws the eye first (bad — RP2040 should be focal point)

**Arrangement B: Side by Side (RP2040 left, S3 right)**
```
    ○ RP2040 (33mm)    ○ S3 (48mm)
```
- Same dimensions as A, reversed
- Western reading order puts RP2040 first (better)
- The S3 is still larger and may dominate

**Arrangement C: Stacked (RP2040 on top, S3 below)**
```
        ○ RP2040 (33mm)
        ○ S3 (48mm)
```
- Total height: ~33 + 12mm gap + 48 = ~93mm
- Total width: ~48mm (widest element)
- The RP2040 is elevated (higher = more prominent in visual hierarchy)
- Compact horizontal footprint
- The S3 is below — the user naturally looks up first, sees the flavor, then looks down to interact

**Arrangement D: Stacked (S3 on top, RP2040 below)**
```
        ○ S3 (48mm)
        ○ RP2040 (33mm)
```
- The larger display is on top (visually heavier at the top looks unbalanced)
- The flavor display is below — less prominent

**Arrangement E: Offset / Asymmetric**
```
    ○ RP2040
              ○ S3
```
- RP2040 upper-left, S3 lower-right (or any diagonal)
- Creates visual interest and a sense of designed intention
- More complex to lay out alongside other front face elements

#### Recommendation: Arrangement C (RP2040 on Top, S3 Below)

This arrangement:
1. Elevates the RP2040 (flavor display) to the top — the first thing the eye hits
2. Places the S3 (config) below and slightly larger — naturally secondary
3. Is narrow (~48mm total width), leaving horizontal space for the cartridge slot
4. Creates a "totem" or "pillar" of displays that anchors one side of the front face

---

### 1d. Empty Holder Appearance

When a display is popped out, its magnetic holder is visible as an empty socket.

**Option 1: Open socket, finished interior**

The recess interior is smooth, printed in the same navy color as the front face. The magnetic pogo connector sits at the bottom of the recess (small, unobtrusive). The empty socket reads as "this is where something goes" — similar to an empty MagSafe charger puck.

**Option 2: Snap-in blank cover**

A thin magnetic disc (navy-colored, printed) snaps into the holder when the display is removed. Keeps the front face solid. The user stores the blank somewhere when the display is installed.

**Option 3: LED ring illumination**

A small LED ring around the holder illuminates when the display is removed, indicating "display disconnected." This turns a missing display into a deliberate status indicator rather than a gap. Adds wiring and components.

**Recommendation:** Option 1 (open socket, finished interior). Keeping it simple. The empty socket is clean enough on its own, and the magnetic pogo connector at the bottom provides a subtle "connection point" aesthetic. If the user removes a display, they are already in a "tinkering" mindset and an empty socket is fine.

---

## 2. Cartridge Slot on the Front Face

The replaceable pump cartridge (target envelope: **140 x 90 x 100mm**, ~940g) must be accessible. If it loads from the front, the front face needs a substantial slot.

### 2a. Slot Dimensions

| Parameter | Value | Notes |
|-----------|-------|-------|
| Cartridge cross-section (W x H) | 140 x 90mm | Side-by-side pump arrangement (Arrangement A from envelope research) |
| Guide rail clearance | +3mm per side | FDM tolerance for sliding fit |
| Slot opening (W x H) | ~146 x 96mm | Cartridge + rail clearance |
| With chamfered entrance | ~155 x 105mm | Outer edge of the chamfer/funnel |
| Slot depth (visible from front) | ~5-10mm | The chamfered lip before the cartridge face |

This is a large opening: 146 x 96mm is roughly the size of a large smartphone. It is the single largest feature on the front face.

### 2b. Lever Position for Front-Loading

If the cartridge slides in horizontally from the front, the cam lever (from cam-lever.md: ~50-100mm lever length, 1-1.5mm eccentricity, 180-degree swing) needs to be accessible.

**Option A: Lever on the Front Face of the Cartridge**

The lever is integrated into the cartridge's front face — the part the user sees when the cartridge is fully inserted. The lever swings horizontally (left-right) or vertically (up-down) in the plane of the front face.

- **Horizontal swing:** The lever folds flat against the cartridge front face when locked. The user swings it 180 degrees to one side to unlock. This works if the cartridge slot is wide enough to accommodate the lever sweep.
- **Vertical swing:** The lever flips up (or down) from the cartridge face. Clearance needed above/below the slot.

**Interaction:** The user grasps the lever, swings it to unlock, then uses the lever as a handle to pull the cartridge out. This is very intuitive — the lever serves double duty as release mechanism and extraction handle.

**Option B: Lever on the Enclosure (Dock-Side)**

The lever is mounted on the dock, not the cartridge. It sits beside or above the cartridge slot on the front face.

- The user operates the lever first (releasing the collets), then slides the cartridge out by gripping its front face.
- Requires a separate grip feature on the cartridge front face (a pull tab or recessed handle).
- More complex (lever mechanism permanently mounted in the enclosure).

**Option C: Lever on the Top of the Cartridge (Current Design)**

The lever is on the top face of the cartridge. For a front-loading configuration, this means the lever is inside the enclosure, accessible through a slot in the enclosure's top surface above the cartridge bay. The user reaches over/into the enclosure to flip the lever.

- Works well if the enclosure has a clear top, but contradicts the "all interactions from the front" design goal.

**Recommendation: Option A (Lever on Cartridge Front Face)**

The lever on the cartridge front face is the most intuitive for front-loading. The user sees one element (the cartridge face with its lever), performs one action (swing lever + pull), and the cartridge comes out. The lever doubles as the handle. This is analogous to a blade server being pulled from a rack by its front handle.

**Front face of cartridge with lever:**
```
    ┌──────────────────────────────────┐
    │                                  │
    │    ┌────────────────────┐        │
    │    │                    │        │  90mm
    │    │   Cartridge Body   │        │
    │    │                    │        │
    │    └────────────────────┘        │
    │                                  │
    │  ═══════════════╗  ← lever      │
    │                 ║    (swings     │
    │                 ║     left)      │
    └──────────────────────────────────┘
                   140mm
```

When locked, the lever folds flat against the cartridge face (within the slot opening). When unlocked, it swings 180 degrees to the side, protruding ~50-80mm from the cartridge face. The user then grips the lever and pulls.

### 2c. Cartridge Slot Aesthetics

A 146 x 96mm rectangular opening is significant. Making it look intentional:

**Chamfered entrance:** A 5mm 45-degree chamfer around all four edges of the slot opening creates a visual frame and acts as a guide funnel for cartridge insertion. The chamfer catches light differently from the flat face, creating depth.

**Recessed border/reveal:** A 2mm step-down around the slot opening (like a picture frame) separates the slot from the rest of the front face. This is a common consumer electronics detail (think of the bezel around a TV screen).

**Color accent:** The chamfered inner surface or the recessed border could be a contrasting color. Against a dark navy face:
- Matte silver/aluminum chamfer (like an Apple product)
- Gloss navy chamfer (subtle depth)
- A thin bright accent line (sky blue, matching the app's accent color)

**Status indicator:** A small LED (or LED strip) above or beside the slot indicates cartridge status:
- Green: cartridge present, locked, operational
- Amber: cartridge present but unlocked
- Off/Red: cartridge absent

**Door/flap:** A hinged door that covers the slot when the cartridge is fully inserted, like a CD/DVD drive door. When the user wants to remove the cartridge, they press/open the door first. This hides the slot entirely when not in use but adds mechanical complexity and another thing that can break.

**Recommendation:** Chamfered entrance with recessed border. No door (the cartridge face itself fills the opening and becomes part of the front face aesthetic). A single status LED above the slot.

### 2d. Cartridge Slot Position Relative to Displays

The cartridge slot (155 x 105mm with chamfer) and the display cluster (~48mm wide x ~93mm tall) must coexist on the front face.

**Key constraint:** The cartridge slot is 155mm wide. The displays are 48mm wide. Together with spacing, the front face needs at least 155 + 20mm gap + 48 = ~223mm of horizontal width. Alternatively, they can be stacked vertically.

This is explored in detail in Section 5 (Layout Options).

---

## 3. Hopper Access on the Front Face

The hopper is where the user pours flavor concentrate. This is the most frequent physical interaction.

### 3a. Front-Accessible Hopper Concepts

**Concept 1: Flip-Up Funnel Door**

A hinged door on the upper front face that flips open to reveal a funnel. The funnel directs concentrate into a reservoir inside the enclosure. The door is flush with the front face when closed.

- Door size: ~80 x 60mm (large enough to pour from a bottle without spilling)
- Hinge: top-mounted, the door swings up and back
- Funnel throat: ~30mm diameter (receives the pour, narrows to tubing inside)
- Drip tray: a small lip below the funnel catches drips when the door is open

**Concept 2: Pull-Out Funnel (Cup Holder Style)**

A spring-loaded drawer on the front face that the user pushes to release, and it slides out revealing a funnel. Similar to a car cup holder or a CD tray.

- Drawer extends ~60mm from the front face when open
- The funnel is built into the drawer
- Spring-loaded return: push to open, push again to close (push-push latch)
- Compact when closed: only a thin line visible on the front face

**Concept 3: Removable Pour Cup**

A small cup or funnel that clips magnetically to a port on the front face. The user removes the cup, fills it, then pours into the port. Or pours directly into the port through the cup.

- Cup diameter: ~50mm
- Port on the front face: ~30mm hole with a check valve or flap to prevent evaporation/spills when the cup is removed
- The cup could double as a measuring cup (marked graduations)

**Concept 4: Top-Mounted Hopper (Not Front)**

If the top of the enclosure is accessible (tower height allows), a top-mounted hopper avoids adding complexity to the front face. The hopper is a wide-mouth funnel on the top surface with a screw-on lid.

- The user pours from above — gravity does all the work
- No mechanism needed (just a funnel and lid)
- Keeps the front face clean
- But requires the top of the tower to be reachable (within 10-14" of the cabinet front, from the ergonomics research)

### 3b. Hopper Placement Decision

The under-cabinet ergonomics research established that comfortable reach is 10-14" from the cabinet front. If the enclosure is positioned near the front of the cabinet (which it should be, for front-face access), the top of the enclosure is within easy reach.

A tower enclosure standing on the cabinet floor (~28" clearance height) with a height of, say, 300-400mm (12-16") puts the top at ~300-400mm from the floor of the cabinet. The user reaches in (10-14" depth) and pours downward. This is natural and easy.

**Recommendation: Top-mounted hopper as the default.** The front face is already crowded with displays and the cartridge slot. Adding a hopper to the front adds complexity without clear benefit, since the top is accessible. The hopper on top also allows a wider funnel opening (no space constraints from neighboring front-face elements) and uses gravity naturally.

If the top is not accessible (enclosure pushed to the back of the cabinet, or tall items on top), a flip-up funnel door (Concept 1) on the upper front face is the fallback.

---

## 4. Visual Hierarchy and Information Architecture

### 4a. Visual Priority Map

When the user opens the cabinet door and looks at the front face:

| Priority | Element | Frequency | Type | Location |
|----------|---------|-----------|------|----------|
| 1 | RP2040 flavor display | Daily | Glance | Upper area, prominent |
| 2 | Cartridge status LED | Daily | Glance | Near cartridge slot |
| 3 | S3 config display | Weekly | Interact | Below RP2040 |
| 4 | Cartridge lever/face | Quarterly | Interact | Center/lower area |
| 5 | Power indicator | Rare | Glance | Bottom corner |

### 4b. At-a-Glance Information

Without touching anything, the user should be able to see:
1. **Which flavor is active** — RP2040 display (image)
2. **System is running** — power LED (solid green/blue)
3. **Cartridge is installed and locked** — status LED near slot (green)

Information that requires interaction or the iOS app:
- Flavor ratios (S3 display or iOS app)
- Bag levels (not currently sensed — future feature)
- Statistics (iOS app)

### 4c. Labeling and Iconography

**Minimal labeling.** The front face should communicate through spatial design and color, not text. Product references:

- **Apple products**: zero labels on front faces. The design is self-explanatory.
- **Sonos speakers**: a single LED indicator. No labels on the front.
- **Keurig machines**: labeled buttons but the labels are subtle (debossed, same color as the surface).

For the soda flavor injector:
- **No text labels on the front face.** The displays are self-explanatory (you can see what they show).
- **Status LEDs** use color language: green = good, amber = attention, red = error.
- **The cartridge slot** is self-evident (it is a large rectangular opening with a lever).
- **A small debossed logo** (if desired) in a bottom corner — subtle branding.

If labels are desired later, they should be:
- Debossed (recessed into the surface, not protruding)
- Same color as the surface (visible by shadow, not by contrast)
- Positioned below or beside the element they describe, not above

---

## 5. Overall Front Face Layout Options

### Reference Dimensions

| Element | Width | Height | Depth |
|---------|-------|--------|-------|
| S3 display holder (with bezel) | 55mm | 55mm | 35mm |
| RP2040 display holder (with bezel) | 40mm | 40mm | 15mm |
| Cartridge slot (with chamfer) | 155mm | 105mm | — |
| Status LED | 5mm | 5mm | — |
| Front face panel thickness | — | — | 5mm |
| Margin from panel edge | 10mm minimum | | |

### Layout A: Displays Top-Left, Cartridge Center-Right

```
    ┌──────────────────────────────────────────────┐
    │                                              │
    │   ○ RP2040     ┌────────────────────────┐   │
    │   (40mm)       │                        │   │
    │                │    CARTRIDGE SLOT       │   │
    │   ○ S3         │    (155 x 105mm)       │   │
    │   (55mm)       │                        │   │
    │                └────────────────────────┘   │
    │                            ● status LED     │
    │                                              │
    │   ● power LED                                │
    └──────────────────────────────────────────────┘

    Dimensions: ~260mm wide x ~150mm tall
    (10.2" x 5.9")
```

**Interaction:** Displays on the left side, grouped vertically (display totem). Cartridge loads from the right side of the front face. The user's left hand operates displays, right hand operates cartridge. Hopper on top.

**Displays removed:** Two circular recesses on the left, cartridge slot dominates the right. The layout still looks balanced because the cartridge face fills the right side.

**Pros:**
- Wide, low profile — fits well in a landscape orientation
- Clear visual separation between display zone and cartridge zone
- Natural two-handed operation

**Cons:**
- Wide enclosure (260mm = 10.2") takes more horizontal cabinet space
- Displays are visually subordinate to the much larger cartridge slot

### Layout B: Displays Centered Above, Cartridge Below

```
    ┌──────────────────────────────────────────────┐
    │                                              │
    │         ○ RP2040       ○ S3                  │
    │         (40mm)         (55mm)                │
    │                                              │
    │   ┌──────────────────────────────────────┐   │
    │   │                                      │   │
    │   │         CARTRIDGE SLOT               │   │
    │   │         (155 x 105mm)                │   │
    │   │                                      │   │
    │   └──────────────────────────────────────┘   │
    │                  ● status       ● power      │
    └──────────────────────────────────────────────┘

    Dimensions: ~180mm wide x ~210mm tall
    (7.1" x 8.3")
```

**Interaction:** Displays at the top (eye level first), cartridge below (hand level). Displays are side-by-side for minimum height. Hopper on top.

**Displays removed:** Upper area has two empty sockets; still balanced since the cartridge dominates the lower half.

**Pros:**
- Narrower profile (180mm vs 260mm)
- Displays above cartridge follows visual hierarchy (information first, then interaction)
- Cartridge area below is naturally at hand level when crouching

**Cons:**
- Taller enclosure (210mm front face height)
- Displays side-by-side are horizontally centered but the size difference (40mm vs 55mm) creates slight asymmetry

### Layout C: Compact Tower (Displays Stacked Left, Cartridge Right)

```
    ┌───────────────────────────────────────┐
    │                                       │
    │  ○ RP2040  ┌──────────────────────┐  │
    │  (40mm)    │                      │  │
    │            │   CARTRIDGE SLOT     │  │
    │  ○ S3      │   (155 x 105mm)     │  │
    │  (55mm)    │                      │  │
    │            └──────────────────────┘  │
    │  ● power          ● status          │
    └───────────────────────────────────────┘

    Dimensions: ~230mm wide x ~140mm tall
    (9.1" x 5.5")
```

**Interaction:** Same as Layout A but tighter spacing. The display totem (stacked vertically) occupies the left column. The cartridge slot fills the right. This is the most space-efficient side-by-side arrangement.

**Displays removed:** Left column has two sockets. The overall layout still reads correctly.

**Pros:**
- Compact footprint
- Shortest height of any layout with front-loading cartridge
- Display totem is a strong visual anchor

**Cons:**
- The S3 display holder (55mm) is nearly as tall as the cartridge slot (105mm), creating visual tension between left and right columns
- Still wide (230mm)

### Layout D: Cartridge NOT Front-Loading (Side or Rear)

If the cartridge loads from the side or rear of the enclosure, the front face has only displays and indicators:

```
    ┌────────────────────────┐
    │                        │
    │       ○ RP2040         │
    │       (40mm)           │
    │                        │
    │       ○ S3             │
    │       (55mm)           │
    │                        │
    │   ● power   ● status  │
    └────────────────────────┘

    Dimensions: ~80mm wide x ~170mm tall
    (3.1" x 6.7")
```

**Interaction:** The front face is a narrow display panel. All physical interaction (cartridge, hopper) happens from the top or side. The user opens the cabinet, glances at the front face for status, then reaches to the top for the hopper or side/rear for the cartridge.

**Pros:**
- Extremely clean, minimal front face
- The front face is almost entirely "information" — no physical interaction needed
- Tiny footprint (80mm wide)
- The display totem is the entire front face, giving it maximum visual impact

**Cons:**
- Cartridge access from the side or rear may be difficult in a packed cabinet (the ergonomics research showed side access is workable but less natural than front access)
- The front face is so narrow that it might not look like a "product face" — more like a strip of controls on the side of something

### Layout E: Displays on Front, Everything Else on Top

```
    ═══════════════════════════════  ← top surface
    ║  HOPPER    CARTRIDGE SLOT  ║
    ═══════════════════════════════

    ┌────────────────────────────┐  ← front face
    │                            │
    │     ○ RP2040               │
    │     (40mm)                 │
    │                            │
    │     ○ S3                   │
    │     (55mm)                 │
    │                            │
    │  ● power       ● status   │
    └────────────────────────────┘

    Front face: ~100mm wide x ~170mm tall
    Top surface: ~260mm wide x ~200mm deep
    (with top-loading cartridge and hopper)
```

**Interaction:** The front face is display-only. The top surface is the interaction surface for physical tasks (pouring concentrate, swapping cartridge). This separates "information" (front) from "action" (top).

**Pros:**
- Cleanest possible front face (display totem only)
- Top-loading cartridge uses gravity to assist insertion (drop-in from above)
- Hopper on top is the most natural pour position
- No large openings on the front face

**Cons:**
- Top access requires reaching into the cabinet
- If the enclosure is pushed to the back of the cabinet, the top is harder to reach
- Two interaction surfaces (front for looking, top for doing) instead of one

### Layout F: Landscape with Integrated Cartridge Face

The cartridge face, when fully inserted, becomes part of the front panel aesthetic. The cartridge front face is finished in the same navy color/material. The lever sits flush. When the cartridge is removed, a recessed cavity is visible.

```
    ┌──────────────────────────────────────────────┐
    │                                              │
    │  ○ RP2040                                    │
    │  (40mm)     ╔══════════════════════════╗     │
    │              ║                          ║     │
    │  ○ S3        ║   CARTRIDGE FACE        ║     │
    │  (55mm)      ║   (flush, navy finish)  ║     │
    │              ║   lever ═══╗            ║     │
    │              ╚══════════════════════════╝     │
    │                                              │
    │  ● power                     ● status        │
    └──────────────────────────────────────────────┘

    Dimensions: ~250mm wide x ~150mm tall
    (9.8" x 5.9")
```

**Interaction:** Similar to Layout A/C, but the cartridge face is designed as an integral part of the front panel. When installed, the cartridge face is flush and color-matched. The only hint of a separate element is the thin gap around the cartridge perimeter. The lever (flat when locked) is the only differentiated feature on the cartridge face.

**Pros:**
- The front face looks like a single unified panel when the cartridge is installed
- Product-quality appearance: the slot "disappears" when the cartridge is in place
- The lever is subtle but discoverable
- When removed, the cavity clearly communicates "something goes here"

**Cons:**
- Requires the cartridge front face to be finished to the same standard as the enclosure (adds cost/complexity to the cartridge)
- Tighter tolerances on the cartridge-to-enclosure gap for a flush fit

---

## 6. Material and Finish

The front face is the visible surface that defines the enclosure's personality. The dark navy theme (#1a1a2e) is already established across the iOS app, S3 display UI, and app icon.

### Color Reference

**#1a1a2e** is a very dark navy, almost black with blue undertones. In print/physical terms, it is close to RAL 5004 (Black Blue) or Pantone 296 C. It reads as "premium dark" in most lighting — the blue is only apparent when compared to true black.

### Option 1: Navy PETG Filament (Direct Print)

**Available filaments:**
- Atomic Filament Navy Blue PETG Pro (1.75mm, 1kg): deep navy, matte finish. ~$30/kg.
- IIIDMAX Navy Blue PETG: similar tone, +/- 0.05mm tolerance. ~$25/kg.
- Spectrum PET-G Premium Navy Blue (RAL 5002): slightly brighter navy. ~$28/kg.
- Polymaker PolyLite PETG Dark Blue: good layer adhesion, slightly more blue than navy.

**Appearance:** FDM layer lines are visible on the front face, especially under side lighting. The surface has a subtle ridged texture. From arm's length (typical viewing distance inside a cabinet), layer lines are noticeable but not ugly. The color is consistent throughout the print (no paint to chip).

**Post-processing to reduce layer lines:**
- Sanding (220-400 grit) + clear coat: removes most visible lines, adds a semi-gloss finish. Labor-intensive for a large flat panel.
- Heat gun smoothing: PETG responds to heat, but warping risk is high on flat panels.
- Printing the front face as a separate flat panel at 0.1mm layer height (slower but finer) and attaching it to the structural body printed at 0.2mm.

**Pros:**
- Simplest approach. Print and done.
- Color is integral (no peeling, no chipping).
- PETG is durable, chemical-resistant, and UV-stable.

**Cons:**
- Layer lines visible on the front face.
- The exact shade of navy depends on the filament brand; matching #1a1a2e precisely is unlikely.
- Matte/satin finish only (FDM can't produce gloss without post-processing).

### Option 2: Black PETG + Vinyl Wrap

**Process:** Print the enclosure in black PETG (cheaper, more widely available). Apply a navy vinyl wrap to the front face.

**Available wraps:**
- 3M 1080 M227 Matte Blue Metallic: a muted dark blue with metallic flake. Very close to dark navy. 3.5 mil thickness. ~$10/sq ft.
- 3M 2080 M227 (updated series): same color, improved adhesive.
- Available in small quantities (1 ft x 5 ft rolls on Amazon, ~$15).

**Application to 3D prints:**
- Vinyl wrap adheres well to smooth surfaces. FDM layer lines can telegraph through thin vinyl.
- **Prep:** sand the front face with 220-grit to remove the worst layer lines, apply a thin coat of filler primer, sand smooth with 400-grit, then apply vinyl.
- Alternatively, print the front face panel in clear/translucent PETG at 0.1mm layer height for a smoother substrate.
- Vinyl wrap is applied with a squeegee and heat gun, conforming to curves and flat surfaces.

**Pros:**
- Very smooth, professional finish.
- Exact color match possible (many navy shades available).
- Hides all layer lines.
- Easy to replace if damaged (peel and re-wrap).
- Wraps around edges cleanly.

**Cons:**
- Prep work required (sanding + primer) for best results.
- Vinyl can peel at edges if not properly heat-sealed.
- Adds a processing step beyond printing.
- The wrap covers up any debossed details (logos, labels).

### Option 3: Spray Paint over Sanded Print

**Process:** Print in any PETG color (gray is common for spray-paint base). Sand the front face (220 then 400 grit). Apply filler primer. Sand again (400 grit). Spray with navy paint. Clear coat.

**Paint options:**
- Rust-Oleum Painter's Touch 2X Ultra Cover in Navy Blue: very close to target color. Bonds to plastic. ~$5/can.
- Montana GOLD spray paint in "Gonzo" (dark blue/navy): artist-quality, smoother finish. ~$8/can.
- Automotive spray paint (Dupli-Color) in a custom-mixed dark navy: most precise color match, highest gloss.

**Pros:**
- Exact color achievable with custom mixing.
- Smooth, professional finish when properly prepped.
- Gloss or matte finish selectable via clear coat.

**Cons:**
- Multi-step process (print, sand, prime, sand, paint, clear coat).
- Paint can chip if the enclosure is bumped.
- Requires a well-ventilated painting area.
- Drying time adds days to the build process.

### Option 4: Separate Front Panel (Non-Printed)

**Concept:** The front face is a separate flat panel made from a different material, screwed or magnetically attached to the 3D printed structural body.

**Material options:**
- **Laser-cut acrylic (3mm):** Available in dark blue/navy tints. Can be edge-lit. Glossy and smooth. ~$5-15 for a 250x150mm panel (from Ponoko or similar).
- **Anodized aluminum (1-2mm):** Custom-anodized in dark navy. Extremely premium feel. ~$20-40 for a custom panel (from SendCutSend).
- **PCB (1.6mm FR4):** A custom PCB used as a face plate. Solder mask color can be dark blue. Silkscreen provides perfect white text/graphics. Mounting holes are trivially precise. ~$5-10 for 5 panels from JLCPCB. Not traditionally used as an enclosure face, but increasingly common in maker projects.

**Pros:**
- Perfect surface finish (no layer lines).
- Precise cutouts for display holders and cartridge slot.
- Material feels premium (metal, glass, or smooth acrylic).
- PCB approach allows embedded LEDs, labels, and graphics at negligible cost.

**Cons:**
- Added cost and lead time.
- Requires precise alignment between the panel and the printed body.
- Mixed-material aesthetic (if the rest of the enclosure is printed PETG, the front panel feels different).

### Recommendation

For a **first prototype**: Navy PETG filament (Atomic Filament Navy Blue PETG Pro) with the front panel printed separately at 0.1mm layer height. Sand lightly and apply a matte clear coat. This gets the look close enough to evaluate the overall design.

For **production quality**: Black PETG body + 3M 1080 M227 Matte Blue Metallic vinyl wrap on the front face. This provides the smoothest finish with the least effort and is easily repeatable. The vinyl can be replaced if damaged.

For **maximum polish**: Anodized aluminum front panel (custom-cut from SendCutSend) attached to a printed PETG structural body. This is how premium consumer electronics achieve their look.

---

## 7. The "Factory Default" Experience

When the user first opens their cabinet after installing the enclosure:

### What They See

1. A dark navy box, roughly the size of a small desktop PC, sitting upright on the cabinet floor or mounted to the side wall
2. The front face is clean and intentional:
   - Two round display windows glowing with color (RP2040 showing a flavor image, S3 showing the home screen)
   - A flush cartridge face (navy, matching the enclosure) with a subtle lever
   - A small green LED indicating "all good"
   - No visible wiring, no raw openings, no exposed PCBs
3. A hopper/funnel on top with a clean lid
4. Tubing exits neatly from the rear, routed to the soda water line

### Product Comparisons

**Sonos speaker:** The front face is a single grille with no controls. Status: one LED. Our enclosure is more complex (two displays, a cartridge slot) but can aim for the same intentionality — every element looks designed, nothing looks accidental.

**Gaming PC tower (NZXT H510, etc.):** Front I/O panel with USB ports, power button, LED strip. Clean tempered glass side panel. The aesthetic is "deliberately minimal with a few high-quality details." Our enclosure can achieve this with the display totem as the "hero element" and the cartridge slot as the "functional feature."

**Keurig K-Supreme:** Front face has a control panel (buttons, small display) on top and a large K-Cup pod chamber in the middle. The chamber door is color-matched. Our cartridge slot is analogous to the K-Cup chamber — a functional opening that is designed to look like part of the product.

**Home audio receiver (Denon, Yamaha):** Front face dominated by a display window, a volume knob, and a few buttons. Inputs hidden behind a drop-down door. The display + knob combination maps directly to our RP2040 + S3 rotary encoder layout.

### Key Design Principles from These Products

1. **Color consistency:** Everything on the front face is the same color family (navy).
2. **Minimal seams:** The gap between the cartridge face and the enclosure face should be < 1mm. Display holder bezels should be thin.
3. **Active elements glow:** The displays provide color and light that draws the eye. Status LEDs are tiny but visible.
4. **Passive elements recede:** The cartridge face, the structural body, the holder bezels — all dark navy, all recede into the background.
5. **No exposed fasteners on the front face:** Screws, nuts, bolt heads — all hidden. Use internal clips, rear-access screws, or snap-fits for structural assembly.

---

## 8. Comparison Matrix

Scoring each layout on 8 criteria (1-5, 5 = best):

| Criteria | Weight | A: Displays Left, Cart Right | B: Displays Top, Cart Below | C: Compact Tower | D: No Front Cart | E: Front Displays, Top Actions | F: Integrated Cart Face |
|----------|--------|-----|-----|-----|-----|-----|-----|
| Visual clarity | 4 | 4 | 4 | 3 | 5 | 5 | 4 |
| Interaction ergonomics | 4 | 4 | 4 | 4 | 3 | 3 | 4 |
| Aesthetics (product feel) | 5 | 3 | 3 | 3 | 4 | 4 | 5 |
| Pop-out display accommodation | 3 | 4 | 3 | 4 | 5 | 5 | 4 |
| Cartridge access ease | 4 | 4 | 4 | 4 | 2 | 3 | 5 |
| Hopper access | 2 | 4 | 4 | 4 | 4 | 5 | 4 |
| Manufacturability | 3 | 3 | 3 | 3 | 5 | 4 | 2 |
| Robustness | 2 | 4 | 4 | 4 | 5 | 4 | 3 |
| **Weighted Total** | | **100** | **99** | **96** | **105** | **108** | **111** |

### Ranking

1. **Layout F: Integrated Cartridge Face** (111) — The cartridge face becomes part of the front panel. Highest aesthetic score. The front face looks like a single designed surface. The trade-off is manufacturing complexity (the cartridge front must be finished to the same standard).

2. **Layout E: Front Displays + Top Actions** (108) — Clean separation of information (front) and physical interaction (top). Very clean front face with only the display totem and LEDs. Hopper and cartridge on top is the most natural interaction. Works best if the enclosure top is accessible.

3. **Layout D: No Front Cartridge** (105) — The ultimate minimal front face. Only displays and LEDs. All physical interaction from the side/rear/top. Highest score on visual clarity and display prominence. Loses points on cartridge access (side/rear loading is less natural).

4. **Layout A: Displays Left, Cartridge Right** (100) — Classic split layout. Functional and clear. Wider enclosure.

5. **Layout B: Displays Top, Cartridge Below** (99) — Taller enclosure. Good hierarchy but the front face is busy.

6. **Layout C: Compact Tower** (96) — Most space-efficient, but the competing sizes of the display column and cartridge slot create visual tension.

### Overall Recommendation

**Layout F (Integrated Cartridge Face)** is the long-term target for a product-quality enclosure. It requires the most design work (the cartridge face must be precisely fitted and color-matched) but produces the best result.

**Layout E (Front Displays + Top Actions)** is the best starting point for prototyping. It keeps the front face simple (display totem only) while deferring cartridge and hopper design to the top surface, which is easier to iterate on. If the enclosure is positioned where the top is reachable, this layout is excellent on its own merits and may not need to be "upgraded" to Layout F.

**Prototype path:** Start with Layout E. Print the front panel as a separate piece (0.1mm layer height, navy PETG, sanded + matte clear coat). Iterate on display holder magnet sizing and fit. Once the display totem is dialed in, add the cartridge slot (move to Layout F or keep Layout E depending on cabinet ergonomics testing).

---

## Sources

- [Elecrow CrowPanel 1.28" ESP32 Rotary Display — Product Page](https://www.elecrow.com/crowpanel-1-28inch-hmi-esp32-rotary-display-240-240-ips-round-touch-knob-screen.html) — S3 display dimensions (48x48x33mm, 50g)
- [Waveshare RP2040-LCD-0.99-B Wiki](https://www.waveshare.com/wiki/RP2040-LCD-0.99-B) — RP2040 display specs (33mm visible diameter, CNC case)
- [Waveshare RP2040-LCD-0.99-B Amazon Listing](https://www.amazon.com/waveshare-RP2040-0-99inch-LCD-Accelerometer/dp/B0CTSPYND2) — RP2040 product reference
- [Meshnology ESP32 1.28" Round Rotary Display Amazon](https://www.amazon.com/Meshnology-240x240-1-28-Development-Capacitive-Compatible/dp/B0G5Q4LXVJ) — S3 display Amazon listing
- [Formlabs — How to Design Snap-Fit Enclosures](https://formlabs.com/blog/designing-3d-printed-snap-fit-enclosures/) — Snap-fit design guidelines for 3D printed parts
- [trueCABLE — Calculating Wire Bend Radius](https://www.truecable.com/blogs/cable-academy/minimum-bend-radius) — Cat6 cable diameter and bend radius specifications
- [Comms Express — Cable Bend Radius Guide](https://www.comms-express.com/infozone/article/bend-radius/) — Bend radius calculation for copper cables
- [Wikipedia — Category 6 Cable](https://en.wikipedia.org/wiki/Category_6_cable) — Cat6 cable specifications reference
- [Adafruit DIY Magnetic Connector — 4 Pin Right Angle (#5358)](https://www.adafruit.com/product/5358) — MagSafe-style magnetic pogo pin connector, 4 pins, 2.54mm pitch
- [Adafruit DIY Magnetic Connector — 6 Pin Straight (#5467)](https://www.adafruit.com/product/5467) — 6-pin magnetic pogo connector, 2.2mm pitch, $7.95
- [Hunter Spring & Reel — Cat6 Retractable Data Cable Reels](https://www.hunterspringandreel.com/products/retractable-data-cord-reels/cat6-retractable-data-reels) — Professional retractable cat6 reels
- [Hunter Spring & Reel — USB Retractable Data Cable Reels](https://www.hunterspringandreel.com/products/retractable-data-cord-reels/usb-retractable-cable-data-reels) — Compact retractable USB reels
- [CableWholesale — Telephone Handset Cord 4P4C RJ22](https://www.cablewholesale.com/products/network-phone/telephone-cables/product-8104-54112bk.php) — Coiled 4-conductor handset cable specs
- [Lapp Tannehill — Retractile/Coiled/Spiral Control Cable](https://www.lapptannehill.com/wire-cable/multi-conductor-cable/retractile-coiled-spiral-cable) — Industrial retractile cable options
- [Atomic Filament — Navy Blue PETG Pro](https://atomicfilament.com/products/navy-blue-petg-pro) — Navy PETG filament for 3D printing
- [IIIDMAX — Navy Blue PETG Filament](https://www.iiidmax.com/product/navy-blue-petg-filament/) — Navy PETG filament option
- [Spectrum Filaments — PET-G Premium Navy Blue (RAL 5002)](https://shop.spectrumfilaments.com/product-eng-1218-Filament-Spectrum-PET-G-Premium-1-75mm-NAVY-BLUE-1kg-RAL-5002.html) — RAL-matched navy PETG
- [3M 1080 M227 Matte Blue Metallic Vinyl Wrap](https://www.amazon.com/3M-M227-Matte-Metallic-Vinyl/dp/B008MMY7SE) — Dark navy matte vinyl wrap for surface finishing
- [3M Wrap Film 1080 Series Color Card](https://multimedia.3m.com/mws/media/742852O/color-card-3m-wrap-film-serie-1080.pdf) — Full 3M 1080 series color reference
- [QH Industrial — Magnetic Pogo Pin Connectors](https://www.connectors-cables.com/magnetic-pogo-pin-connector/) — Custom magnetic pogo connectors
- [Pomagtor — Multi-Pin Magnetic Pogo Pin Connectors](https://www.pomagtor.net/magnetic-connector/) — Custom magnetic connector manufacturer
- [MW Components — Reel Retrievers / Mini Reels](https://www.mwcomponents.com/reel-retrievers-mini-reels) — Small retractable cable reel mechanisms
- [SpecialistID — Retractable Badge Reels Guide](https://www.specialistid.com/blogs/news/retractable-badge-reels-everything-you-need-to-know) — Badge reel mechanism internals
