# Cartridge Change Workflow — Complete User Experience Analysis

Exhaustive walkthrough of every step in a cartridge replacement, from the moment the user decides to change the cartridge to the moment they close the cabinet door. Covers the baseline design (horizontal slide-in, top lever, release plate) and four alternative architectures. Includes failure scenarios, time estimates, tool requirements, first-time setup, and residual water analysis.

This document exists to surface UX problems that are invisible in mechanical drawings. The details matter — a 2mm drip, a 3-second fumble in the dark, or a missing towel can turn a 30-second operation into a 10-minute ordeal.

---

## 1. The Complete Workflow — Baseline Design

**Architecture: Horizontal slide-in dock, eccentric cam lever on cartridge, release plate with stepped bores, 4 John Guest 1/4" push-to-connect fittings in dock wall, 3 pogo pins on dock, passive dock.**

### 1.1 Pre-Change

**How does the user know the cartridge needs changing?**

There is currently no automatic detection of tubing wear. The triggers are:

| Trigger | Detection Method | User Experience |
|---------|-----------------|-----------------|
| Pump tubing wear | Visual (flow rate drops, pump sounds different) | User notices flavor is weak or pump is laboring |
| Flavor change | User decision | User wants different flavors in the bags |
| Pump failure | Motor runs but no flow, or motor doesn't run | iOS app or S3 display could show "no flow detected" if flow meter reads zero during pump operation |
| Scheduled maintenance | Calendar reminder (user sets this themselves) | No system-initiated notification exists today |
| Cleaning | After clean cycle, user may want to swap | Clean cycle is a separate firmware feature |

**Future opportunity:** The ESP32 already has a flow meter. If pumps run but flow meter shows zero or very low flow for a sustained period, the system could push a "check cartridge" notification to the iOS app via BLE. This is not implemented today.

**Does the user need to shut anything off?**

- **Water supply**: No. The soda water line is pressurized (from ice maker kit), but the flavor injection lines are gravity-fed from platypus bags. There is no shutoff valve on the flavor lines — the John Guest fittings seal when the tube is removed. The soda water line itself is not part of the cartridge — it stays connected.
- **System power**: Not strictly necessary. The ESP32 should detect cartridge removal (pogo pin contact loss) and stop driving the pumps. However, as a safety measure, the user should stop any active dispensing. If the system is idle (no one is pouring soda water), it is safe to remove the cartridge without any software action.
- **iOS app**: Not required. But if the user wants to configure new flavors after the swap, they will need the app afterward.

**Required pre-change sequence:**
1. Run clean cycle (mandatory — see Section 1.1a)
2. Wait for clean cycle to complete
3. Open cabinet door

**Does the user need to prepare anything?**

- New cartridge (pre-assembled with pumps, tubing routed, tube stubs protruding)
- A towel or small container for the old cartridge (it will have residual water — see Section 7)
- Nothing else. No tools required.

### 1.1a Clean-Before-Remove — The Primary Drip Prevention Strategy

The clean cycle is not optional before cartridge removal. The software enforces this: the S3 touchscreen menu and iOS app require a completed clean cycle before presenting the "remove cartridge" option. This is the single most important UX design decision for cartridge changes.

**Why this matters:**

Without a clean cycle, the cartridge's internal tubing contains sticky flavor concentrate (~10mL). With a clean cycle, the tubing contains only water and air. The difference between cleaning up water drips and cleaning up sugar syrup is the difference between a 5-second towel wipe and a 10-minute sticky mess.

**Enforcement levels:**

| Level | Implementation | Bypass |
|-------|---------------|--------|
| Software gate | S3 menu and iOS app hide "remove cartridge" until clean cycle completes | User could physically remove without using the menu |
| Servo-actuated latch (production) | A small servo blocks the cam lever from opening until firmware unlocks it post-clean-cycle | Manual override with hex key (fail-safe) |

For the prototype, software enforcement is sufficient. The UI will make it unnatural to remove a cartridge without first running the clean cycle. If a physical lock is needed to prevent bypass, a servo-actuated latch on the cam lever provides that guarantee.

**What state is the system in after clean cycle?**

- Flavor lines (cartridge side): contain clean water and/or air
- Pump chambers: contain water (not concentrate)
- Solenoid valves: closed when not dispensing (backflow prevention)
- Electrical: 12V available at pogo pins, but L298N not driving motors when idle

### 1.2 Physical Access

**User opens the cabinet door.**

Under a standard kitchen sink cabinet (36" wide is most common), the interior is cluttered with:
- P-trap and drain pipes (center)
- Hot/cold water supply lines and shutoff valves (back wall)
- Garbage disposal (if present, hangs from sink)
- Cleaning supplies, sponges, trash bags (typical cabinet contents)
- The soda flavor injector enclosure (self-contained tower on the cabinet floor)

The enclosure is a self-contained 280 x 250 x 400mm tower on the cabinet floor, with the cartridge slot on the front face.

**User assesses the situation.**

```
    CABINET FRONT VIEW (door open, looking in)

    ┌─────────────────────────────────────────────┐
    │  ┌─────┐                         ┌─────┐   │
    │  │     │    ┌── P-trap ──┐       │     │   │
    │  │ L   │    │            │       │  R  │   │
    │  │ side│    │  drain     │       │side │   │
    │  │     │    │  pipes     │       │     │   │
    │  │     │    └────────────┘       │     │   │
    │  │     │                         │     │   │
    │  │ [ENCL]◄── cartridge slot      │     │   │
    │  │     │     (front face)        │     │   │
    │  │ supply                        │     │   │
    │  │ lines                         │     │   │
    │  └─────┘                         └─────┘   │
    │                                             │
    └─────────────────────────────────────────────┘
              ▲ cabinet opening (door removed)
```

The cartridge slot is at approximately 226mm (~9") from the cabinet floor — the center of Zone B on the front face of the enclosure.

**User positions themselves.**

Most common body position: **kneeling on one knee** or **crouching** in front of the open cabinet. The cabinet opening is typically 28-30 inches wide and 20-24 inches tall (after subtracting the face frame). The user must look into the cabinet and reach 10-20 inches deep.

```
    SIDE VIEW (user at cabinet)

    ┌──────── countertop ────────┐
    │                            │
    │   ← cabinet interior →     │
    │                            │
    │        [ENCLOSURE with     │
    │         CARTRIDGE SLOT]    │
    │                            │
    └────────────────────────────┘
         │
         │  ← user reaches in
         │
        /│\  ← user kneeling
       / │ \
      /  │  \
```

**Key ergonomic facts:**
- User's dominant hand reaches into the cabinet
- Line of sight is at an angle — looking slightly upward into a dark space
- The back of the cabinet is typically unlit
- The enclosure front face is oriented toward the cabinet opening for direct access

**Lighting.**

Under-sink cabinets have no built-in lighting. The user relies on:
1. Ambient room light filtering in through the open door
2. Phone flashlight (requires one hand)
3. A clip-on or adhesive LED cabinet light (if installed)

For a task that should be done by feel (like the baseline lever design), lighting is helpful but not critical. For a task requiring visual alignment (like manually pressing 4 individual collets), lighting is essential.

### 1.3 Disconnection

**User reaches the cartridge.**

The cartridge front face (with lever) faces outward, toward the cabinet opening. The user can see the lever handle protruding from the top of the cartridge/dock assembly.

**User locates the lever.**

The lever is on top of the cartridge (per mating-face.md — the cam pivot is on the cartridge). The lever handle is approximately 50-100mm long, protruding above the cartridge body. In a dark cabinet, the user finds it by:
1. Visual: the lever is the most prominent feature on the cartridge front
2. Touch: reaching to the top of the dock assembly and feeling for the protruding handle

**User flips the lever.**

The lever rotates approximately 180 degrees from the "locked" position to the "released" position. This is the same motion as opening a bicycle quick-release skewer.

| Phase | Lever Angle | What Happens | Force Required | Tactile Feedback |
|-------|-------------|--------------|----------------|------------------|
| Start | 0 deg (locked, lever flat against cartridge top) | Nothing yet | ~5N to begin rotation | Lever feels firm, held by over-center cam |
| Over-center | ~10 deg | Cam passes dead center, lever suddenly becomes easy to move | Peaks at ~10-15N, then drops | Distinct "break" feel — the cam releases |
| Mid-swing | 10-170 deg | Cam rotates freely, release plate begins to advance | ~2-5N (just overcoming friction) | Light, smooth rotation |
| Plate engages collets | ~170 deg | Release plate stepped bores contact all 4 collet rings | Force increases as plate pushes collets | Resistance builds — user feels the collets compressing |
| Full open | 180 deg | All 4 collets fully depressed, tubes released | ~15-25N at lever tip (12-20N at plate, multiplied by ~1.5x lever MA) | Firm stop at fully open position |

The lever should have a detent or stop at the fully open position so it stays open during cartridge withdrawal. If the lever swings closed during withdrawal, the collets re-grip the tubes and the cartridge gets stuck partway out.

**What happens to the water in the tubes?**

After the mandatory clean cycle, the cartridge's internal tubing contains clean water and air — not sticky concentrate. When the collets release and the user begins withdrawing the cartridge:
1. The 4 tube stubs slide out of the John Guest fittings
2. The John Guest fittings **seal immediately** when the tube is removed — the O-ring closes against the fitting bore. No water drips from the dock side.
3. The tube stubs on the cartridge **do not seal** — they are open tube ends. Residual water in the stubs and internal tubing will begin to drip from the stub tips due to gravity.
4. The pump chambers retain water (trapped between rollers) until the cartridge is tilted or shaken.

Because the clean cycle has already run, this residual fluid is water — it will not stain surfaces or leave sticky residue.

**Is there residual pressure from the soda water line?**

No. The flavor injection lines are separated from the pressurized soda water line by solenoid valves (normally closed when not dispensing). The solenoid valves are between the pump outlets and the soda water tee. When the system is idle, the solenoid valves are closed, so no soda water pressure reaches the cartridge lines. The only pressure on the cartridge side is gravity head from the flavor bags (~0.1-0.3 PSI, negligible).

**User withdraws the cartridge.**

The user grips the cartridge body (or the lever, if it is sturdy enough to serve as a handle) and slides it straight out along the guide rails.

| Phase | Distance | What User Feels |
|-------|----------|-----------------|
| Initial pull | 0-5mm | Pogo pins lose contact (light spring resistance, ~0.5N total for 3 pins). Tube stubs begin exiting fittings. |
| Tube withdrawal | 5-15mm | Tube stubs sliding out of fitting O-rings. Very light resistance (~1-2N total). O-rings provide a slight "suck" as they release. |
| Free slide | 15-130mm | Cartridge slides freely on guide rails. Only rail friction (~2-5N depending on fit and lubrication). |
| Full withdrawal | 130mm+ | Cartridge clears the dock. User has cartridge in hand. |

Total withdrawal force: ~5-10N peak (about 1-2 lbs). Easily one-handed.

The slide should feel smooth and controlled. Any binding, catching, or jerky motion indicates a guide rail tolerance issue.

**Where does the user put the old cartridge?**

This is a critical UX detail. The old cartridge:
- Weighs ~820g (1.8 lbs)
- Has 4 open tube stubs that will drip water (see Section 7 for volume)
- Has wet pump chambers
- Is held in one hand

Options:
1. **Set it on the cabinet floor** — likely on a towel. Water drips onto the towel.
2. **Place it in a plastic bag or container** — user needs to have this ready beforehand.
3. **Hand it to someone else** — requires a second person.
4. **Set it on the counter above** — requires reaching up while kneeling, potentially dripping on the counter.

**Design opportunity:** A printed cap that covers the tube stubs would reduce dripping during transport. A silicone cap that press-fits over the 4 tube stubs (like a dust cap) would contain drips. Since the residual is water (post-clean-cycle), the drip concern is modest — a towel handles it easily.

### 1.4 Transition

The user now has the old cartridge in one hand (or set down) and needs to pick up the new cartridge.

**Sequencing:**
1. Withdraw old cartridge with dominant hand
2. Set old cartridge down on towel/tray (dominant hand now free)
3. Pick up new cartridge with dominant hand
4. Insert new cartridge

This requires a surface within arm's reach to place the old cartridge. The cabinet floor directly in front of the enclosure is the natural spot.

**Is the new cartridge pre-prepared?**

For the baseline design, the new cartridge should arrive fully assembled:
- Pumps mounted internally
- BPT tubing routed from pumps to barb fittings to hard tube stubs
- Tube stubs protruding from the mating face at the correct length (~15-20mm past the release plate)
- Motor wires routed to pogo target pads
- Lever in the **open** position (release plate extended, so tube stubs protrude freely)

**What if the lever is in the wrong position?**

Per mating-face.md: when the lever is **open**, the release plate is **extended** (pushed forward toward the dock). When the lever is **closed**, the release plate is **retracted** (pulled back toward the cartridge body). This means:

- **For insertion**: Lever must be **closed** (plate retracted, so tube stubs protrude past the plate and can enter the fittings freely)
- **For locking**: Lever stays closed — the over-center cam holds the cartridge in position
- **For removal**: Lever is **opened** — plate extends, pushes collets, releases tubes

So the new cartridge should have its lever in the **closed** position for insertion. If the user accidentally has the lever open (plate extended), the plate's stepped bores would block the tube stubs from entering the fittings — the plate face would hit the dock wall before the tubes reach the fittings.

**This is a potential confusion point.** The cartridge inserts with lever closed and removes with lever open. This is counterintuitive — "closed" means ready-to-insert and "locked", "open" means ready-to-remove. But this matches the bicycle QR paradigm: lever closed = wheel locked, lever open = wheel removed.

### 1.5 Connection

**User picks up new cartridge.**

The cartridge is ~150 x 80 x 130mm and ~820g. It fits comfortably in one hand, oriented so the mating face (with tube stubs) faces forward and the lever handle is on top.

```
    USER'S HAND HOLDING CARTRIDGE (right hand, palm down)

    ┌──────────────────────────┐
    │     lever handle         │ ← fingers wrap around top/sides
    │   ┌──────────────────┐   │
    │   │   cartridge body │   │
    │   │                  │   │ ← thumb on one side, fingers on other
    │   │   mating face    │   │
    │   │   ════ stubs ════│──►│ ← tube stubs point forward
    │   │                  │   │
    │   └──────────────────┘   │
    └──────────────────────────┘

    Grip: similar to holding a thick paperback book
    with the spine facing forward
```

**User aligns cartridge with dock opening.**

The dock has a funnel entrance — the guide rails flare outward at the dock mouth (per guide-alignment.md: tapered lead-in, 15-20 degree taper on alignment pins, rail chamfers). The cartridge slot has a 5mm x 45-degree chamfer on all edges, providing a funnel for sloppy insertion. The user aims the cartridge at the dock opening and pushes.

Even in a dark cabinet, the funnel entrance provides ~10-15mm of tolerance in each direction. The user does not need to achieve precise alignment by hand — only get the cartridge roughly aimed at the opening.

**User slides cartridge in.**

| Phase | Distance | What Happens | Force Required |
|-------|----------|--------------|----------------|
| Funnel entry | 0-20mm | Tapered lead-in captures the cartridge, corrects lateral and vertical misalignment | ~2-5N (light push) |
| Rail engagement | 20-50mm | Guide rails fully engage, cartridge is constrained to 1-axis motion | ~3-5N (rail friction) |
| Pogo pin contact | 50-80mm | Pogo pins on dock contact cartridge pads, electrical connection made | ~1-2N additional (3 pogo springs, ~0.5N each) |
| Tube stub entry | 80-115mm | 4 tube stubs enter the John Guest fitting bores, O-rings not yet reached | ~5N (guiding stubs into bore openings) |
| Tube seating | 115-130mm | Tube stubs push past O-rings and gripper teeth, fittings latch | ~20-40N peak (4 fittings x 5-10N each for O-ring compression + tooth deflection) |
| Bottomed out | 130mm | Tube stubs hit the internal tube stops in the fittings. Cartridge is fully seated. | Firm stop — distinct feel |

**Total insertion force at the seating phase: ~20-40N (4.5-9 lbs).** This is the most significant force in the entire operation. The user must push firmly for the last 15-20mm to seat all 4 tubes past the O-rings simultaneously. This force is applied through a comfortable grip on a ~150mm wide cartridge, so it feels like a firm push, not a struggle.

**How does the user know it's fully seated?**

Multiple feedback mechanisms:
1. **Mechanical stop**: Tube stubs bottom out against fitting tube stops. The cartridge stops moving.
2. **Tactile**: The transition from sliding friction to a hard stop is distinct.
3. **Audible**: The fittings may produce a soft "click" as the gripper teeth engage the tube OD. Four simultaneous clicks.
4. **Electrical**: ESP32 detects pogo pin contact and could trigger the S3 display to show "Cartridge Connected" or the RP2040 display to change state.
5. **Lever**: The lever is already in the closed/locked position, so no further action is needed for locking. The cam is in its over-center position.

The lever should already be closed for insertion. If the cam is over-center, it holds the cartridge locked. The lever-closed state is both the insertion configuration and the locked configuration.

**This is elegant.** Insert with lever closed, remove by opening lever. One step fewer than a system that requires insert-then-lock.

### 1.6 Verification

**System detection:**

When the pogo pins make contact:
1. ESP32 can detect voltage/continuity on the motor pins (a simple pull-down resistor on the motor lines would show the circuit is closed when the cartridge's motor winding provides a path)
2. ESP32 sends a status update via BLE to the iOS app: "Cartridge connected"
3. S3 config display could show "Cartridge: Connected" in the status area
4. RP2040 display could show a status icon

**Priming:**

A new cartridge has dry pumps and dry internal tubing. The tubes from the flavor bags to the pump inlets, and from the pump outlets through the cartridge to the soda line, are all filled with air. The pumps must be primed to draw liquid from the bags through the system.

The system already has a software prime feature (heartbeat safety model, 2s timeout, 60s ceiling, controllable from S3 touchscreen or iOS app). The priming procedure:

1. Ensure flavor bags are connected to the inlet fittings on the dock (these are separate from the cartridge — the bags connect to the dock's inlet fittings, not the cartridge)
2. Open iOS app or use S3 touchscreen
3. Select "Prime Pump 1" — pump runs continuously (with heartbeat safety)
4. Watch for liquid to appear in the clear tubing downstream of the pump
5. When liquid is flowing steadily (no air bubbles), stop priming
6. Repeat for Pump 2

Estimated prime time: 15-45 seconds per pump, depending on tubing length and bag head height. Gravity-fed system with ~1-2 feet of head should prime in under 30 seconds.

**Configuration:**

If the cartridge swap is just for tubing replacement (same flavors), no configuration needed.

If switching flavors:
1. Open iOS app
2. Change flavor 1 name and image
3. Change flavor 2 name and image
4. App pushes config to ESP32 via BLE
5. ESP32 pushes updated images/config to RP2040 and S3 via UART/ProtoLink

### 1.7 Cleanup

1. User closes cabinet door
2. Old cartridge: set aside for disposal or refurbishment
   - **Disposal**: The cartridge is 3D printed PETG + 2 Kamoer pumps + brass fittings + tubing. The pumps are the expensive part (~$30-40 each). If the pumps are still functional and only the tubing is worn, the user can replace the BPT tubing in the pump head (Kamoer sells replacement tubing) and reassemble the cartridge.
   - **Refurbishment**: Remove old BPT tubing from pump heads, install new tubing, reconnect barb fittings, verify tube stubs are undamaged (trim if scored from collet teeth). The cartridge body itself should last indefinitely.
   - **Trash**: If the user does not want to refurbish, the cartridge goes in the trash. PETG is recyclable but not commonly accepted in curbside recycling.
3. Clean up any dripped water (typically a few mL — towel handles it in seconds, since it is water, not concentrate)

---

## 2. Alternative Workflow Variants

### 2a. Twist-Lock (Quarter-Turn Bayonet)

**Architecture:** Cartridge has 2-3 protruding lugs. Dock has matching bayonet slots. User pushes cartridge in, then rotates 90 degrees to lock. Tubes seat during the push-in phase; rotation locks the cartridge and provides the "docked" feel. No release plate — collet release would need a separate mechanism or manual release of each fitting.

**Disconnection workflow:**

1. User reaches into cabinet, grips cartridge body
2. User twists cartridge 90 degrees counter-clockwise (left hand: awkward; right hand: natural wrist supination)
3. Lugs disengage from retention slots
4. User pulls cartridge straight out

**Key differences from baseline:**

| Aspect | Baseline (Lever) | Twist-Lock |
|--------|-------------------|------------|
| Collet release | Simultaneous via release plate | Must be separate (twist doesn't push collets) |
| Grip | One hand on lever | Both hands may be needed (twist + pull) |
| Force | Low (lever MA) | Moderate (direct twist against friction) |
| Feedback | Over-center break feel | Click at 90-degree stop |
| Orientation sensitivity | None (slide is 1-axis) | Must rotate correct direction |

**The critical problem with twist-lock for this application:** Rotating the cartridge does not release the John Guest collets. The collets grip the tube OD and resist axial withdrawal regardless of rotation. A twist-lock could lock/unlock the cartridge housing, but the tubes would still be gripped by the fittings. The user would need to either:
- Manually press each collet before pulling (defeats the purpose)
- Have a separate release mechanism (adds a step)
- Use fittings that release with rotation (not how John Guest works)

**Verdict:** Twist-lock is a poor match for John Guest fittings. It works well for water filters with integrated seals (where the twist compresses/releases an O-ring), but our design uses separate push-to-connect fittings that require axial collet depression for release.

**Hand position for twist:**

```
    GRIP FOR TWIST (right hand, palm facing left)

         ┌─────────────┐
    ─────┤  cartridge   ├─────
    thumb│  body        │fingers
    on   │             │wrap
    near │  ──twist──► │around
    side │             │far side
         └─────────────┘

    Wrist rotation: supination (palm up → palm down)
    Range of motion: ~90 degrees
    Force: 5-15N at the cartridge perimeter (~0.5-1.5 Nm torque)
```

The grip requires wrapping fingers around the cartridge body. At 150mm wide and 80mm tall, the cartridge is grippable but wide — the user must use a palm grip. This works but is less precise than a lever grip. In a dark, cramped cabinet, finding the correct rotational starting position by feel is harder than finding a lever by feel.

### 2b. Pull-Out Dock (Dock on Drawer Slides)

**Architecture:** The dock is mounted on heavy-duty drawer slides (like a keyboard tray or server rack). The user pulls the entire dock assembly out to the front of the cabinet. With the dock pulled out, the cartridge is at waist height in the open, fully lit, easily accessible. The cartridge change uses any mechanism (lever, twist, or pull).

**Workflow:**

1. User opens cabinet door
2. User pulls dock forward on drawer slides (like opening a drawer). Dock slides out ~300-400mm, fully exposing the cartridge.
3. User changes the cartridge at a comfortable height, in full light, with both hands free
4. User pushes dock back into the cabinet
5. User closes cabinet door

**Key differences:**

| Aspect | Fixed Dock | Pull-Out Dock |
|--------|-----------|---------------|
| Access | Reach into dark cabinet | Pull to front, work in the open |
| Lighting | Dark, need flashlight | Ambient room light, excellent |
| Body position | Kneeling, crouching | Standing or kneeling comfortably |
| Both hands free | One hand reaches in | Yes, dock supports itself |
| Tube/wire flex | None (dock is static) | All connections must flex during slide-out |

**The critical problem with pull-out dock:** All connections between the dock and the permanent plumbing must flex during the slide-out motion:
- 4 flexible tubes from the flavor bags to the dock inlet fittings (must have enough slack for ~400mm of travel)
- 3 wires from the ESP32/L298N to the dock pogo pins (must flex with travel)
- Flavor bag tubing could kink or pull on the bags during slide-out

This is solvable with service loops (extra tubing coiled behind the dock) and flexible wire harnesses, but it adds complexity and potential failure points. Every pull-out/push-in cycle fatigues the tubing and wires.

**Drawer slide hardware:**
- Ball-bearing full-extension slides rated for the dock + cartridge weight (~2-3 kg total): widely available, $15-30 per pair
- Mounting: slides attach to cabinet side walls
- Travel: 300-400mm (12-16 inches) — standard drawer slide lengths
- Side clearance: slides add ~25mm (1 inch) to each side of the dock

**Verdict:** Pull-out dock is the best UX for the actual cartridge change moment — working in the open with both hands is dramatically better than reaching into a dark cabinet. The cost is mechanical complexity (slides, service loops, flex harness) and additional failure modes. For a production product, this would be worth the investment. For a prototype, the fixed dock is simpler.

### 2c. Vertical Drop-In (Ceiling-Mounted Dock)

**Architecture:** The dock is mounted to the underside of the countertop (ceiling of the cabinet space). The cartridge inserts upward into the dock. Gravity assists removal (pull lever, cartridge drops into your hand). Gravity resists insertion (must push cartridge up and hold it while locking).

**Workflow:**

1. User opens cabinet door, looks up at the dock on the cabinet ceiling
2. User reaches up, flips the lever to release
3. Cartridge drops downward into user's waiting hand (gravity-assisted removal)
4. User sets old cartridge aside
5. User picks up new cartridge, holds it up against the dock
6. User aligns and pushes upward until tubes seat in fittings
7. User flips lever to lock (while still holding cartridge up with other hand)

**Key differences:**

| Aspect | Horizontal Slide-In | Vertical Drop-In |
|--------|--------------------|--------------------|
| Removal | Pull out (easy) | Drops into hand (very easy — gravity does the work) |
| Insertion | Push in (easy) | Push up and hold (harder — fighting gravity + seating force) |
| One-handed | Yes (slide + lever) | Difficult (must hold cartridge up while operating lever) |
| Drip direction | Forward/down from tube stubs | Straight down into hand/lap |
| Visibility | Look straight in | Look up (neck strain) |

**The weight problem:** The cartridge weighs ~820g (1.8 lbs). Holding it overhead while aligning it with a dock opening and pushing 4 tubes into fittings requires:
- Lifting force: 8.0N (gravity) + 20-40N (fitting seating force) = ~28-48N (~6-11 lbs) upward
- This force must be sustained with one hand while the other operates the lever
- Arm fatigue: holding 1.8 lbs overhead is fine for a few seconds, but alignment fumbling could extend this to 15-30 seconds

**Drip problem:** When the old cartridge drops, the tube stubs point upward (toward the dock). Residual water in the cartridge drains down — some toward the pump chambers (internal), some out the tube stubs (dripping onto whatever is below). Since the clean cycle has already run, this is water, not concentrate — annoying but not damaging.

**Verdict:** Vertical drop-in makes removal trivially easy (gravity) but makes insertion meaningfully harder (fighting gravity, two-hand operation). For infrequent replacements (every few months), the insertion difficulty is tolerable. For a system that might see weekly swaps (during prototyping), it would be annoying. The ergonomics of looking up into a dark cabinet ceiling are also poor — neck strain and water dripping downward.

### 2d. No Release Plate ("Just Pull" with Manual Collet Release)

**Architecture:** No lever, no cam, no release plate. The dock has 4 John Guest fittings. To remove the cartridge, the user manually presses each collet with thumb/finger and pulls the corresponding tube, one at a time. To insert, just push the cartridge in — John Guest fittings latch automatically.

**Removal workflow:**

1. User opens cabinet door, reaches in
2. User locates the cartridge (by feel or sight)
3. User locates the first fitting on the dock wall (by feel — fittings protrude slightly from the dock face)
4. User presses the collet ring on the first fitting with thumb while pulling the corresponding tube stub with the other hand (or simultaneously pulling the cartridge slightly)
5. Repeat for fitting #2, #3, #4
6. User slides the cartridge out (now freed from all 4 fittings)

**Time per fitting:**

| Step | Time | Notes |
|------|------|-------|
| Locate fitting | 2-5s | By feel in a dark cabinet, harder for fittings #3 and #4 which are further from the hand |
| Position thumb on collet | 1-2s | Must center on the collet ring to avoid cocking it |
| Press collet + pull tube | 1-3s | Must hold collet depressed while withdrawing tube |
| **Per fitting total** | **4-10s** | Faster with practice, slower in awkward positions |
| **All 4 fittings** | **16-40s** | Just for disconnection — not counting access time |

**The ergonomic problem:**

Pressing a collet requires positioning a fingertip precisely on the collet ring face (~11mm diameter target) and pushing straight inward (~2mm travel, ~3-5N force). In an under-sink cabinet:
- The user is reaching 10-20 inches into a dark space
- The fittings are spaced ~22mm apart (2x2 grid)
- The user's fingers are large relative to the collet — risk of pressing off-center (see collet-release.md Section 4: cocking failure)
- The two fittings furthest from the user are the hardest to reach with proper thumb/finger positioning

**Verdict:** No release plate eliminates mechanical complexity (no plate, no cam, no lever) at the cost of a significantly worse removal experience. The 4 sequential collet releases are fiddly, time-consuming, and error-prone in a dark cabinet. This is acceptable for a prototype (where you're testing other aspects of the design) but not for a user-facing product. The insertion experience is actually the best of all variants — no lever to worry about, just push until it stops.

### 2e. Quick-Disconnect Fittings (CPC/Swagelok-Style)

**Architecture:** Replace the 4 John Guest push-to-connect fittings with CPC-style valved quick-disconnect couplings. Each coupling has a body (dock side) and an insert (cartridge side). Connection: push insert into body until it clicks. Disconnection: press thumb latch on body, pull insert out. The valved versions automatically shut off flow when disconnected.

**Removal workflow:**

1. User reaches in, locates the cartridge
2. For each of the 4 couplings: press the thumb latch on the coupling body and pull the cartridge slightly
3. Once all 4 are released, slide cartridge out

**Key differences from John Guest:**

| Aspect | John Guest Push-to-Connect | CPC Quick-Disconnect |
|--------|---------------------------|---------------------|
| Connection | Push tube into fitting | Push insert into body, clicks |
| Disconnection | Press collet ring (small, fiddly) | Press thumb latch (larger, ergonomic) |
| Auto-shutoff | O-ring seals when tube removed (no flow) | Valved versions: automatic shutoff valve closes |
| Drip on disconnect | Minimal (O-ring seals fitting bore) | Valved: zero drip. Non-valved: similar to JG. |
| Cost per fitting | $1-3 | $5-15 (valved), $3-8 (non-valved) |
| Size | ~12.7mm body OD | ~15-20mm body OD (larger) |
| Tube compatibility | 1/4" OD hard tube | Push-to-connect or barbed, model-dependent |

**The advantage of CPC valved couplings:** When disconnected, the valve on the dock side closes automatically. No water drips from the dock. The valve on the cartridge insert side also closes — no water drips from the cartridge stubs. This virtually eliminates the drip problem.

**The disadvantage:** CPC couplings are larger than John Guest fittings. The 2x2 grid spacing would need to increase from ~22mm to ~28-30mm center-to-center, making the mating face larger. They are also more expensive ($20-60 total for 4 mated pairs vs. $4-12 for 4 JG fittings).

**The thumb latch is meaningfully easier to press than a JG collet.** CPC couplings are designed for frequent connect/disconnect (that is their purpose). The latch is large enough for a gloved hand, and the actuation is an intuitive squeeze rather than a precise axial push on a tiny ring.

**Could CPC couplings eliminate the need for a release plate entirely?** Yes — if each coupling has its own thumb latch, the user presses each individually. But this is still 4 sequential operations. Alternatively, a mechanical linkage could press all 4 latches simultaneously (similar to the release plate concept, but pressing latches instead of collets).

**Verdict:** CPC valved quick-disconnects are the premium option. They solve the drip problem, provide easier individual disconnection, and are designed for frequent cycling. The tradeoffs are cost and size. For a production product where UX matters, CPC fittings might justify the cost. For a prototype, John Guest fittings with a release plate are more practical.

---

## 3. Failure Scenarios and Recovery

### 3a. User Inserts Cartridge Wrong

**Backwards (mating face facing away from dock):**

The tube stubs would face away from the dock. The guide rails prevent insertion — the cartridge rail profile is asymmetric (per guide-alignment.md: keyed geometry prevents reversed insertion). The cartridge simply will not enter the dock opening backwards.

If the user forces it: the asymmetric rail profile means the cartridge jams within the first 10-20mm of insertion. No damage occurs — the 3D printed rails flex slightly and the cartridge stops. The user feels immediate resistance and knows something is wrong.

**Upside down:**

The guide rail profile is also asymmetric top-to-bottom (one rail wider than the other, or different shapes). The cartridge does not fit upside down. Same jam-and-stop behavior as backwards insertion.

**Wrong angle (cartridge cocked):**

The funnel entrance tolerates ~10-15mm of lateral and vertical misalignment. Beyond that, the cartridge hits the dock frame and doesn't enter. The user corrects the angle and tries again. No damage.

**How does the user realize the mistake?**

Immediate tactile feedback: the cartridge doesn't slide. The guide rails are the first engagement feature and they reject incorrect orientation within the first 10-20mm. There is no scenario where the cartridge travels 50+ mm in a wrong orientation — the rails prevent it.

### 3b. Partial Insertion

**Cartridge not fully seated (tubes partially in fittings):**

| Insertion Depth Past Fitting Entry | Tube State | Seal? | Grip? | Danger? |
|------------------------------------|-----------|-------|-------|---------|
| 0-5mm | Tube in fitting bore but not past O-ring or teeth | No seal | No grip | Tube falls out freely. Safe. |
| 5-10mm | Tube past O-ring but not past gripper teeth | Weak seal | No grip | Water might weep past O-ring under pressure. Tube can be pulled freely. |
| 10-15mm | Tube past O-ring AND gripper teeth, but not at tube stop | Sealed | Gripped | Fitting holds tube. Connection is functional but tube is not at the designed insertion depth. May have reduced grip strength. |
| 15mm (full) | Tube at tube stop | Fully sealed | Fully gripped | Correct insertion. |

**Electrical contacts with partial insertion:**

If the cartridge is only partially inserted, the pogo pins may not contact the pads (they are positioned to engage at ~50-80mm of insertion). A partially inserted cartridge (e.g., 40mm) would have no electrical connection. The ESP32 would not detect the cartridge and would not drive the pumps. This is a safe failure mode — the system simply doesn't operate.

**Can the pump run with a partially-seated cartridge?**

Only if the pogo pins are making contact AND the tubes are gripped by the fittings. This requires the cartridge to be at least ~80% inserted. At that depth, the tubes are likely past the gripper teeth (functional, if not fully seated). The pump would run but with a slightly less secure tube connection. Risk: tube could slip out under vibration if not at the tube stop. Consequence: water leaks inside the dock (post-clean-cycle, this is water, not concentrate). Not catastrophic but messy.

**Detection:** The system could detect partial insertion by checking for a "cartridge present" signal (pogo pin continuity) without a "fully seated" confirmation. A mechanical microswitch at the full-insertion position (activated only when the cartridge is fully bottomed out) would provide definitive detection. This is not implemented today.

### 3c. Lever Stuck or Jammed

**Release plate doesn't fully engage all 4 collets:**

If the plate's guide pins bind or the plate tilts during travel, some collets may engage while others don't. The user flips the lever but the cartridge doesn't release.

Recovery:
1. Push the lever harder (within reason). The cam provides mechanical advantage, so additional hand force may overcome binding.
2. If the lever moves fully but one fitting still holds: the release plate may be slightly warped or one fitting's collet may be stuck. The user can manually press the stuck collet while pulling the cartridge.
3. If the lever is completely stuck: the cam pivot may have debris or the plate guide pins may be jammed. The user needs to work the lever back and forth gently to free it.

**One fitting's collet is stuck (corrosion, mineral deposits):**

Under-sink environment with occasional moisture can cause mineral deposits on the collet mechanism over months/years. The collet ring may become stiff or fail to move.

Recovery:
1. Release plate force may overcome the stuck collet (the cam provides ~10:1 MA)
2. If not, manually press the stuck collet with a finger or the flat end of a screwdriver while pulling the tube
3. Preventive: occasional application of food-grade silicone lubricant on collet rings
4. Replacement: swap out the affected John Guest fitting in the dock (they are press-fit or threaded — replaceable)

### 3d. Water Spill

**User drops the old cartridge:**

The cartridge contains approximately 10 mL of residual water (see Section 7). After the mandatory clean cycle, this is clean water — not sticky concentrate. If dropped:
- On the cabinet floor: a small puddle, easily wiped up
- On carpet or clothing: water, no staining
- On electronics: the electronics are in Zone A (above the dock, at ~310-400mm). The cartridge would have to be flung upward to reach them. Not a realistic concern.

**A tube comes loose during insertion:**

The tubes are rigidly mounted in the cartridge — they do not come loose during normal insertion. However, if a barb fitting inside the cartridge is poorly secured, a tube could pop off when the insertion force is applied (the John Guest fitting pushes back on the tube, which pushes back on the barb connection). If this happens:
- A small amount of water (from the tube interior) leaks inside the cartridge body
- The insertion fails (the tube doesn't reach the fitting)
- The user must remove the cartridge and repair the internal connection

**How much water is in the cartridge?**

See Section 7 for detailed volume calculation. Summary: approximately 10 mL total. This is a tablespoon or less — trivially manageable with a towel. Because the clean cycle has already run, this is water, not concentrate.

### 3e. Wrong Cartridge

**Today (single cartridge design):**

There is only one cartridge type. The keyed rail profile prevents any non-matching object from entering the dock. There is no "wrong cartridge" scenario today.

**Future (multiple cartridge variants):**

If different pump sizes or tubing configurations are introduced, the cartridge variants could be keyed with different rail profiles (different widths, different asymmetric features). Each variant would only fit its matching dock.

**Electronic identification:**

A small resistor across two of the pogo pin pads (e.g., between GND and a sense pin) could identify the cartridge type. Different resistor values = different cartridge types. The ESP32 reads the resistance at insertion and identifies the cartridge. This requires adding a 4th pogo pin (or multiplexing a motor pin for sensing before driving).

Alternatively, a small NFC tag or I2C EEPROM on the cartridge could store cartridge type, manufacturing date, and usage hours. This is overkill for the prototype but interesting for productization.

---

## 4. Time Analysis

### 4.1 Time Breakdown by Phase

All times in seconds. Assumes the user is already in front of the cabinet. The clean cycle runs before the user opens the cabinet — its time is not included here.

| Phase | Optimistic | Realistic | Pessimistic |
|-------|-----------|-----------|-------------|
| | (experienced, smooth) | (average, minor fumbling) | (first time, dark, slight problem) |
| **Access/positioning** | | | |
| Open cabinet door | 2 | 3 | 5 |
| Move items aside (if needed) | 0 | 5 | 15 |
| Kneel/crouch | 2 | 3 | 5 |
| Locate cartridge/lever | 1 | 3 | 8 |
| **Disconnection** | | | |
| Flip lever to release | 2 | 3 | 5 |
| Withdraw cartridge | 2 | 3 | 5 |
| Set old cartridge aside | 2 | 4 | 8 |
| **Cartridge swap** | | | |
| Pick up new cartridge | 2 | 3 | 5 |
| Orient correctly | 1 | 2 | 5 |
| **Connection** | | | |
| Align with dock opening | 1 | 3 | 8 |
| Slide in to seating | 3 | 5 | 10 |
| Confirm seated | 1 | 2 | 5 |
| **Verification** | | | |
| Check system detection (app/display) | 3 | 10 | 30 |
| Prime pumps | 30 | 60 | 120 |
| **Cleanup** | | | |
| Stand up | 2 | 3 | 5 |
| Close cabinet door | 2 | 2 | 3 |
| Wipe up water drips | 0 | 3 | 5 |
| **TOTAL (without priming)** | **23s** | **44s** | **94s** |
| **TOTAL (with priming)** | **53s** | **104s** | **214s** |

### 4.2 Comparison Across Architectures

Times for disconnection + swap + connection only (excludes access, verification, priming).

| Architecture | Optimistic | Realistic | Pessimistic |
|-------------|-----------|-----------|-------------|
| **Baseline (lever + release plate)** | 14s | 23s | 51s |
| **Twist-lock** (if collet release solved) | 12s | 20s | 45s |
| **Pull-out dock** (any mechanism) | 10s | 18s | 35s |
| **Vertical drop-in** | 16s | 28s | 60s |
| **No release plate** (manual collets) | 30s | 55s | 120s |
| **CPC quick-disconnects** | 20s | 35s | 70s |

The pull-out dock wins on the cartridge change itself because the user has excellent access, lighting, and both hands free. The baseline lever design is second-best and has the best balance of simplicity and speed.

---

## 5. Tool Requirements

| Item | Baseline Lever | Twist-Lock | Pull-Out Dock | Vertical Drop-In | Manual Collets | CPC QD |
|------|---------------|------------|---------------|-------------------|----------------|--------|
| Flashlight | Helpful | Helpful | Not needed | Helpful | Essential | Helpful |
| Towel/rag | Recommended | Recommended | Recommended | Recommended | Recommended | Not needed (valved) |
| Phone (for app) | For priming/config | Same | Same | Same | Same | Same |
| Any hand tools | None | None | None | None | None | None |
| Surface for old cartridge | Yes | Yes | Yes (dock provides it when pulled out) | Yes | Yes | Yes |

**The baseline design is tool-free.** The only items the user needs are things they would naturally have nearby (a towel, their phone). No wrenches, screwdrivers, or specialty tools. The towel is for water drips only — since the clean cycle runs before removal, there is no sticky residue to deal with.

---

## 6. The "Unopened Box" Experience

### 6.1 What's in the Box

A new soda flavor injector system ships with:

1. **Enclosure** (self-contained tower with ESP32, L298N motor drivers, flow meter, solenoid valves, dock, wiring harness) — pre-assembled
2. **Cartridge** — fully assembled (pumps, tubing, electrical pads, lever)
3. **RP2040 display module** — separate unit with cable
4. **ESP32-S3 config display** — separate unit with cable
5. **Tubing kit** — 1/4" OD tubing for connecting bags to dock, ice maker T-fitting, saddle valve or ice maker adapter
6. **Hardware bag** — mounting screws, cable ties, etc.
7. **Quick start guide** — single sheet with QR code to video

### 6.2 First-Time Installation

This is a one-time operation, not a cartridge change. The user must:

1. **Place the enclosure** in the cabinet (it is a free-standing tower — no mounting required)
2. **Connect tubing** from the ice maker T-fitting through the flow meter and solenoid valves to the dock outlet fittings (pre-routed inside the enclosure)
3. **Install flavor bags** on the incline mounts inside the bag zone (snap connector into U-clip, hang sealed end on J-hook)
4. **Connect flavor bag tubing** from platypus bag drink tube adapters to the dock inlet fittings
5. **Wire the display modules** (UART cables from ESP32 to RP2040 and S3)
6. **Insert the cartridge** for the first time
7. **Power on the system**
8. **Download the iOS app** and pair via BLE
9. **Prime the pumps**
10. **Configure flavors**

### 6.3 First-Time Cartridge Insertion

The first cartridge insertion follows the same procedure as Section 1.5 (Connection), but the user has never done it before.

**What's confusing for a first-time user?**

1. **Which way does the cartridge go in?** The lever on top is the most visible orientation cue. The tube stubs sticking out of the mating face indicate "this end goes in first." Keyed rails prevent wrong-way insertion. But the user may still hesitate.

2. **How hard do I push?** The seating force (~20-40N / 4.5-9 lbs) may surprise a first-time user. They may push gently, feel resistance at the tube seating phase, and think something is wrong. A clear instruction — "push firmly until the cartridge stops" — is needed.

3. **Is it in all the way?** Without prior experience, the user doesn't know what "fully seated" feels like. The mechanical stop is definitive, but the user may not trust it. A visual indicator (a line on the cartridge that aligns with the dock edge when fully seated) would help.

4. **Do I need to do something with the lever?** If the cartridge ships with the lever in the correct (closed) position, the user doesn't need to touch it for insertion. But they might try to "lock" it (flip the lever), which would open it and extend the release plate — now the cartridge can't be fully inserted. The instructions must clearly say "lever stays as-is for insertion."

5. **Priming**: New user has no intuition for how long priming takes or what "primed" looks like. The iOS app or S3 display should guide this: "Running pump 1... watch for steady liquid flow."

### 6.4 Instructions

A QR code on the quick start guide linking to a 60-second installation video is the most effective instruction format. The video shows:
1. Placing the enclosure in the cabinet (5 seconds)
2. Installing bags and connecting tubing (20 seconds)
3. Inserting the cartridge (10 seconds) — close-up of the push, the stop, the sound
4. Powering on and app pairing (15 seconds)
5. Priming (10 seconds)

Paper instructions with clear diagrams are the fallback.

---

## 7. Residual Water and Drip Analysis

### 7.1 Volume Calculations

After the mandatory clean cycle, all residual fluid in the cartridge is clean water (not flavor concentrate). The volume calculations remain the same regardless of fluid type.

**Tube stubs (4 total, protruding from cartridge mating face):**

Each stub is ~20mm long, 1/4" OD (6.35mm), with hard tubing. Assuming 1/8" ID (3.175mm):

```
Volume per stub = pi * (1.5875mm)^2 * 20mm = 158.4 mm^3 = 0.16 mL
Total (4 stubs) = 0.63 mL
```

**BPT tubing inside cartridge (from pump barbs to barb reducers):**

Each pump has 2 tube runs (inlet and outlet), each ~80mm long. BPT tubing is 4.8mm ID.

```
Volume per run = pi * (2.4mm)^2 * 80mm = 1,447 mm^3 = 1.45 mL
Total (4 runs) = 5.79 mL
```

**Hard tubing inside cartridge (from barb reducers to tube stubs):**

Each run is ~30mm long, 1/8" ID (3.175mm):

```
Volume per run = pi * (1.5875mm)^2 * 30mm = 237.5 mm^3 = 0.24 mL
Total (4 runs) = 0.95 mL
```

**Pump chamber residual:**

A peristaltic pump with 3 rollers has approximately 2/3 of the tubing arc filled with liquid at rest (2 of 3 sectors are not compressed by a roller). The tubing arc inside the pump head has a swept length of approximately:

```
Pump head inner circumference * (2/3) = pi * ~35mm * 0.667 ≈ 73mm
Volume = pi * (2.4mm)^2 * 73mm = 1,321 mm^3 = 1.32 mL per pump
Total (2 pumps) = 2.64 mL
```

**Summary:**

| Component | Volume (mL) |
|-----------|-------------|
| Tube stubs (4x) | 0.6 |
| BPT tubing runs (4x) | 5.8 |
| Hard tubing runs (4x) | 1.0 |
| Pump chambers (2x) | 2.6 |
| **Total** | **~10.0 mL** |

10 mL is approximately **2 teaspoons**. After the mandatory clean cycle, this is clean water — it will not stain or leave sticky residue.

### 7.2 Where Does the Water Go?

**At the moment of disconnection:**

When the collets release and the tube stubs begin to withdraw from the fittings:

1. **Dock-side fittings**: The O-ring seals against the fitting bore as soon as the tube clears it. A tiny amount of water (~0.1 mL per fitting) may weep from between the O-ring and the tube surface as the tube passes through. Total dock-side drip: approximately 0-0.4 mL. The fitting effectively seals.

2. **Cartridge tube stubs**: As the stubs exit the fittings, their tips are wet. A meniscus of water hangs on each stub tip. Surface tension holds approximately 0.05 mL per stub (a small water drop). As the cartridge moves away from the dock, these drops may fall.

3. **Gravity drainage from cartridge interior**: With the cartridge horizontal and the tube stubs pointing forward (toward where the dock was), gravity pulls the internal water toward the stubs. The BPT tubing runs and pump chambers drain slowly toward the stub tips. This is a slow drip, not a gush — the 4.8mm ID BPT tubing has enough surface tension to impede free flow.

**Drip rate estimate:**

- Immediate drip (first 5 seconds): ~0.5-1 mL from meniscus and near-stub water
- Slow drain (over 30-60 seconds): ~3-5 mL as internal tubing drains through stubs
- Pump chamber (retained until tilted): ~2.6 mL stays trapped between rollers until the cartridge is tilted or shaken

**Total expected drip:** 3-5 mL within the first minute. The remaining 5-7 mL stays in the cartridge until it is tilted or cleaned. Since this is water (post-clean-cycle), the cleanup is trivial — a quick towel wipe.

### 7.3 Where Does the Dock Drip?

The John Guest fittings seal well when the tube is removed. The dock itself should not drip significantly. However:
- A tiny amount of water (~0.1 mL total) may weep from the fitting faces immediately after tube withdrawal
- If a fitting O-ring is worn or has mineral deposits, it may drip slowly

**Total dock-side drip: 0-0.5 mL.** Negligible in practice.

### 7.4 Mitigation Strategies

The primary drip mitigation is the mandatory clean-before-remove workflow (Section 1.1a). Because all residual fluid is water after the clean cycle, the drip problem is dramatically reduced — water drips are trivially managed compared to sticky concentrate.

**Dock drip channel:**
- A small channel or groove below the dock fittings routes any weepage to the dock shelf
- Shallow (~3mm deep, ~80mm x 40mm) — holds a few mL
- Integral to the dock shelf print (no separate part)
- Cost: zero (just a groove in the CAD model)

**Tube stub caps (cartridge-mounted):**
- 4 small silicone or 3D printed caps that press-fit over the tube stub tips after removal
- The user pops the caps on after removing the cartridge — prevents dripping during transport
- Could be attached to the cartridge by a flexible tether (so they don't get lost)
- Alternative: a single plate or sleeve that covers all 4 stubs simultaneously

**Pre-removal drain:**
- Before removing the cartridge, run the pumps briefly in reverse (if the motor driver supports bidirectional operation, which L298N does). This pulls water back toward the bags, partially emptying the cartridge-side tubing.
- Reduces residual volume from ~10 mL to ~3-5 mL (the pump chambers still retain fluid)
- Requires firmware support for reverse pumping (already feasible — pump reversal is used for hopper filling)

**Absorbent pad in dock:**
- A small sponge or absorbent pad in the dock below the fittings
- Soaks up the 0-0.5 mL of dock-side weepage
- Replace periodically

### 7.5 How Water Filter Systems Handle This

Under-sink water filter systems face the same problem and handle it in several ways:

**Traditional screw-on housings (e.g., standard RO systems):**
- The filter canister is full of water (~500-2000 mL depending on size)
- User turns off water supply and relieves pressure by opening the faucet
- User places a bucket or towel under the canister
- User unscrews the canister — water pours out
- This is the worst UX for drips. Amazon reviews for these systems commonly mention "make sure you have a bucket" and "water goes everywhere if you forget to depressurize."

**Quick-change twist-lock systems (e.g., Waterdrop, 3M, GE):**
- The filter cartridge is sealed — the user never touches water
- Internal auto-shutoff valves close when the cartridge is removed
- Minimal to zero drip
- This is the gold standard for drip-free replacement
- Advertised as "3-second replacement, no mess"

**Lesson for our design:** The baseline John Guest fitting approach with enforced clean-before-remove falls closer to the quick-change end of the spectrum. The residual is ~10mL of water (not concentrate), versus 500-2000mL for a screw-on canister. For the prototype, a towel is sufficient. For a production product, CPC valved fittings or a pre-removal drain cycle could eliminate drips entirely.

---

## 8. Prior Art — User Experience

### 8.1 Under-Sink Water Filters

**Amazon review themes for traditional screw-on filter housings:**
- "Getting the canister off was a nightmare — the wrench barely fits in the space under my sink"
- "Water went everywhere when I unscrewed it. Wish they included a bucket warning in the instructions"
- "I had to lay on my back to reach the filter. My plumber charges $50 to come change it for me"
- "The O-ring was stuck to the cap. Took 20 minutes to find it and re-seat it"

**Amazon review themes for quick-change twist-lock systems (Waterdrop, etc.):**
- "Filter change took literally 3 seconds. Best upgrade from my old system"
- "Just twist off, twist on. No tools, no mess, no shutting off water"
- "Even my non-handy spouse can change these"
- "The twist-lock mechanism gives a satisfying click when it's seated"

**Key insights:**
- The single biggest complaint across all under-sink systems is **accessibility** — dark, cramped, hard to reach
- The second biggest is **leaks and drips** during replacement
- Tool-free replacement is universally praised
- Clear tactile feedback ("click", "stop") eliminates uncertainty
- Quick-change designs get dramatically better reviews than traditional screw-on designs

### 8.2 SodaStream CO2 Cylinders

SodaStream has evolved through two generations of cylinder replacement UX:

**Original screw-in cylinders:**
- Screw the cylinder into the back of the machine
- Requires moderate hand strength to achieve a gas-tight seal
- Cross-threading is possible if not started straight
- Takes ~10-15 seconds

**Quick Connect cylinders (newer models — Terra, Art, Enso):**
- "Snap your gas cylinder into place without any twisting or screwing"
- Raise the cylinder handle, remove the old cylinder, snap in the new one
- Takes ~3-5 seconds
- Designed for a countertop appliance (perfect lighting, easy access)

**Lesson:** SodaStream's evolution from screw-in to snap-in mirrors the water filter industry's evolution from screw-on to twist-lock. The trend is toward fewer steps, less force, and no tools. Our lever-based design is philosophically aligned with this trend.

### 8.3 Printer Ink Cartridges

**Common complaints:**
- "The cartridge wouldn't click into place. I pushed and pushed. Turned out I forgot to remove the protective tape."
- "I put the wrong cartridge in the wrong slot. The colors were labeled but hard to read in my dim office."
- "After installing the new cartridge, the printer said it was incompatible. Turned out it was a regional lock."
- "Ink got on my fingers and on the desk. The old cartridge dripped when I pulled it out."

**Key insights:**
- **Protective packaging** (tape, clips, caps) that must be removed before installation is a common source of user error
- **Color coding and labeling** matter enormously for multi-cartridge systems
- **Drip from old cartridges** is a universal complaint
- **Electronic detection** (cartridge chip) provides definitive "installed correctly" feedback but also enables frustrating DRM

**Relevance to our design:**
- The new cartridge should have minimal packaging to remove before installation (ideally: take it out of the box and insert it)
- If we add cartridge identification in the future (resistor ID or NFC), it should be purely informational (not DRM)
- The drip problem is shared — but our enforced clean cycle means residual is water, not sticky concentrate

### 8.4 Power Tool Battery Packs

**What works well:**
- Slide-in rails are universally intuitive — users of all skill levels understand "slide it in until it clicks"
- The click (spring latch engaging) provides unmistakable confirmation
- One-button release (press button, slide out) is simple and reliable
- Battery shape and rail profile prevent wrong-brand or wrong-tool insertion

**What doesn't work:**
- Occasional latch failure (spring tab breaks after years of use)
- Dirty rail contacts from construction site dust — requires wiping
- Some users report difficulty finding the release button by feel (gloved hands)

**Lesson:** Our baseline design (slide in on rails, lever for release) borrows directly from this paradigm. The lever replaces the spring latch, providing the same "locked" confirmation with the addition of simultaneous collet release.

### 8.5 UX Patterns to Borrow

| Pattern | Source | Application to Our Design |
|---------|--------|--------------------------|
| Slide-until-click (one motion insert) | Power tool batteries | Rail-guided insertion with firm mechanical stop at full seating |
| Tool-free replacement | Quick-change water filters | Lever is the only interface — no wrenches, no screwdrivers |
| Tactile feedback at lock | All successful designs | Over-center cam break feel when lever is opened/closed |
| Keyed geometry (poka-yoke) | Printer cartridges, batteries | Asymmetric rail profile prevents wrong-way insertion |
| Clean-before-remove | Enforced workflow | Software gate (or physical lock) ensures residual is water, not concentrate |
| First-time guidance | SodaStream, printer setup wizards | QR-to-video quick start guide |

### 8.6 UX Patterns to Avoid

| Anti-Pattern | Source | How to Avoid |
|-------------|--------|-------------|
| Requiring tools (wrench) for routine replacement | Traditional RO filter housings | Lever + release plate, no tools ever |
| Large water volume released during swap | Screw-on filter canisters | Gravity-fed system with minimal residual volume (~10mL water post-clean) |
| Multi-step sequence with unclear order | Complex printer cartridge replacement (remove tape, open lid, press tab, insert, close lid) | Single motion: slide in. Single motion: flip lever + slide out. |
| DRM / artificial incompatibility | Printer cartridge region locking | No electronic restrictions on cartridge acceptance |
| Requiring water shutoff before swap | Traditional water filter systems | John Guest fittings self-seal; no water shutoff needed |
| Sticky residue on removal | Allowing removal without cleaning | Mandatory clean cycle before removal — residual is always water |

---

## Sources

### Product User Experience and Reviews
- [How to Connect and Disconnect John Guest Fittings -- ESP Water Products](https://espwaterproducts.com/pages/how-do-john-guest-fittings-work)
- [How to Fix Leaking Quick-Connect Fittings -- Fresh Water Systems](https://www.freshwatersystems.com/blogs/blog/how-to-fix-leaking-quick-connect-fittings)
- [How to Change an Under-Sink Water Filter -- Quality Water Lab](https://qualitywaterlab.com/undersink/how-to-change-an-under-sink-water-filter/)
