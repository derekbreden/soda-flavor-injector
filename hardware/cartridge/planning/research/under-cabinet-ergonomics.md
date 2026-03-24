# Under-Cabinet Ergonomics — Cartridge Dock Placement & Interaction Design

Research into how real under-sink cabinet conditions constrain the cartridge insertion/removal experience. The cartridge is approximately 140 x 90 x 100mm and weighs ~2.1 lbs (940g). It contains two peristaltic pumps and connects via 4 John Guest 1/4" push-to-connect fittings. A lever mechanism releases all 4 collets simultaneously for removal.

This document explores how the cabinet environment dictates where the dock can live, how the user reaches it, and which lever positions and insertion directions actually work in practice.

---

## 1. Real Under-Sink Cabinet Anatomy

### 1.1 Standard US Sink Base Cabinet Dimensions

US kitchen sink base cabinets follow KCMA (Kitchen Cabinet Manufacturers Association) standards. The most common configuration:

| Dimension | Standard Value | Range | Notes |
|---|---|---|---|
| Exterior width | 36" (914mm) | 30-42" (762-1067mm) | 36" is the most common for double-basin sinks |
| Interior width | ~34" (864mm) | 28-40" (711-1016mm) | After face frame (~3/4" per side) |
| Exterior depth | 24" (610mm) | 24" standard | Matches countertop depth |
| Interior depth | ~22" (559mm) | 21-23" (533-584mm) | After face frame at front |
| Total height (floor to countertop underside) | ~34.5" (876mm) | 34-36" (864-914mm) | Before countertop |
| Toe kick height | 4-4.5" (102-114mm) | — | Recessed area at floor level |
| Usable interior height | ~28-30" (711-762mm) | — | Depends on sink depth |
| Sink bowl depth below countertop | 8-10" (203-254mm) | 6-12" | Deeper sinks eat more vertical space |

**Interior volume of a typical 36" sink base cabinet: roughly 34" x 22" x 28" (864 x 559 x 711mm).** But almost none of this is actually usable as-is.

### 1.2 Where the Plumbing Actually Is

The plumbing occupies the center and back of the cabinet. Here is a realistic cross-section:

```
    TOP VIEW — Looking down into cabinet (front at bottom)

    ┌────────────────────────────────────────────────────────────┐
    │                       BACK WALL                            │
    │                                                            │
    │        hot shutoff ●          ● cold shutoff               │
    │                    │          │                             │
    │                    │  drain   │                             │
    │        supply ─────┤  pipe    ├───── supply                │
    │        lines       │  (1.5")  │      lines                 │
    │                    │    │     │                             │
    │                    │    │     │                             │
    │                    │  ┌─┴─┐   │                             │
    │   ZONE A           │  │P- │   │           ZONE B           │
    │   (left side)      │  │trap│  │           (right side)     │
    │   10-14" wide      │  │   │   │           10-14" wide      │
    │                    │  └───┘   │                             │
    │                    │          │                             │
    │                                                            │
    │                      FRONT OPENING                         │
    └────────────────────────────────────────────────────────────┘
          ◄────── 34" interior width ──────►
```

```
    SIDE VIEW — Looking at cabinet from the left side

    ┌──────────────────────────────────────────┐ ← countertop underside
    │                                          │
    │          sink bowl hangs down             │
    │       ┌──────────────────────┐           │   8-10" deep
    │       │   ////sink////       │           │
    │       └──────────┬───────────┘           │
    │                  │ drain                  │
    │            ┌─────┴─────┐                 │
    │            │ disposal  │  8" dia         │   12-13" tall
    │            │ (if any)  │                 │
    │            └─────┬─────┘                 │
    │                  │                       │
    │              ┌───┴───┐                   │
    │              │P-trap │                   │
    │              └───┬───┘                   │
    │                  │→ to wall drain        │
    │                                          │
    │    ← 22" depth →                         │
    │                                          │
    └──────────────────────────────────────────┘ ← cabinet floor
    ▲ back wall                    ▲ front opening
```

### 1.3 The Garbage Disposal Problem

If a garbage disposal is present (and roughly 50% of US kitchens have one), it dominates the center of the cabinet:

| Component | Typical Dimensions | Position |
|---|---|---|
| InSinkErator Badger 5 (1/2 HP) | 6.3" dia x 11.5" tall | Hangs from sink drain, center of cabinet |
| InSinkErator Evolution (3/4-1 HP) | 8.25" dia x 12.25" tall | Same position, larger |
| P-trap (1.5" kitchen) | ~6" wide x 6" tall x 4" deep | Below disposal or drain, roughly centered |
| Supply lines (hot + cold) | 3/8" flex lines, ~4" from back wall | Back wall, 8-12" apart |
| Shutoff valves | Protrude ~3" from back wall | Back wall, below supply lines |

**With a disposal installed, the center 8-10" of cabinet width is fully blocked from sink level down to about 16" below the countertop.** The P-trap sits below the disposal and routes to the wall drain, typically 4-8" from the back wall.

### 1.4 Available Zones

After plumbing claims its space, the cabinet has two primary usable zones:

```
    FRONT VIEW — Looking into open cabinet doors

    ┌─────────────────────────────────────────────────────────────┐
    │                     countertop underside                    │
    │  ┌──────────┐    ┌─────────────────┐    ┌──────────┐       │
    │  │          │    │   sink bowl      │    │          │       │
    │  │  ZONE A  │    │   + disposal     │    │  ZONE B  │       │
    │  │  LEFT    │    │   + P-trap       │    │  RIGHT   │       │
    │  │          │    │   (blocked)      │    │          │       │
    │  │ 10-14"W  │    │                  │    │ 10-14"W  │       │
    │  │ 20-22"D  │    │                  │    │ 20-22"D  │       │
    │  │ 20-28"H  │    │                  │    │ 20-28"H  │       │
    │  │          │    │                  │    │          │       │
    │  │          │    │                  │    │          │       │
    │  └──────────┘    └─────────────────┘    └──────────┘       │
    │                       cabinet floor                         │
    └─────────────────────────────────────────────────────────────┘
```

**Zone A / Zone B dimensions (each side of plumbing):**

| Dimension | Value | Notes |
|---|---|---|
| Width | 10-14" (254-356mm) | Depends on cabinet width and disposal diameter |
| Depth | 20-22" (508-559mm) | Full cabinet depth, but back 4" partially blocked by supply lines |
| Height | 20-28" (508-711mm) | From cabinet floor to sink bowl underside; shorter if disposal present on that side |
| Usable depth (after supply lines) | 16-18" (406-457mm) | Front of supply line shutoffs to front of cabinet |

**Additional zones:**

- **Floor zone (below P-trap):** Full cabinet width, but only 4-8" (102-203mm) tall. Good for flat trays, not for our cartridge.
- **Ceiling zone (above P-trap, between sink and disposal):** Narrow, obstructed, inaccessible. Not usable.
- **Back wall (above supply lines):** Technically mountable, but supply lines and shutoff valves are in the way, and this is maximum reach depth.
- **Side walls:** Full height and depth available. Common mounting surface for water filter brackets.
- **Cabinet door interior:** Available for small items, but swings with the door. Not suitable for plumbed connections.

### 1.5 What Else Lives Under the Sink

The cartridge dock competes for space with any of:

| Item | Typical Size | Mounting | Common Location |
|---|---|---|---|
| Under-sink RO system (APEC, iSpring) | 15" x 5" x 18" (tank) + filter head | Floor-standing + wall bracket | Zone A or B floor |
| Inline water filter (3M, Waterdrop) | 3" dia x 10-14" long | Wall/side bracket | Side wall or back wall |
| Instant hot water dispenser tank | 4" dia x 10" tall | Bracket on side wall | Zone A or B, side wall |
| Soap dispenser pump bottle | 4" dia x 8" tall | Free-standing | Zone A or B floor |
| Pull-out trash/recycling | 10-14" wide | Drawer slides on cabinet floor | Takes an entire zone |
| Cleaning supply caddy | 8-12" wide | Free-standing or pull-out | Floor, front of zone |
| Fire extinguisher | 3.5" dia x 14" tall | Bracket on side wall | Varies |

**Key insight:** Most under-sink accessories mount to the **side wall** or sit on the **cabinet floor**. The back wall is avoided because of plumbing. Our dock should follow this pattern — side wall mounting is the most conventional and leaves the most accessible space.

### 1.6 Sight Lines and Lighting

What the user actually sees when they open the cabinet and crouch down:

```
    USER'S VIEW — Kneeling at cabinet opening, looking in

    ┌─────────────────────────────────────────────────────────────┐
    │░░░░░░░░░░░░░░░░░░░░ DARK ZONE ░░░░░░░░░░░░░░░░░░░░░░░░░░░│
    │░░░░░░░░░░  (back 8-10" is in deep shadow)  ░░░░░░░░░░░░░░░│
    │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│
    │▓▓▓▓▓▓▓▓▓▓   DIM ZONE   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│
    │▓▓▓▓▓▓▓▓▓▓   (middle 6-8", partially visible)  ▓▓▓▓▓▓▓▓▓▓│
    │                                                             │
    │            VISIBLE ZONE                                     │
    │            (front 6-8", well lit by room light)             │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘
    ▲ front opening (user kneels here)
```

**Visibility by depth:**

| Depth from front | Visibility | Notes |
|---|---|---|
| 0-6" (0-152mm) | Good | Room light reaches here easily |
| 6-12" (152-305mm) | Fair | Visible if user leans in, may need to tilt head |
| 12-16" (305-406mm) | Poor | Deep shadow, user must lean in significantly or use a light |
| 16-22" (406-559mm) | Very poor | Effectively blind without a flashlight; user working by feel |

**The critical implication:** Anything mounted more than ~12" (305mm) from the cabinet front opening requires the user to either use a flashlight or work by feel. The dock should be as far forward as plumbing allows.

### 1.7 Reach Depth from a Kneeling Position

Under-sink work is performed from one of three postures:

**Posture 1: Kneeling at the cabinet opening**
- Torso roughly upright
- Arms extend forward into the cabinet
- Comfortable forward reach (forearm only, elbow at side): ~14" (356mm)
- Extended reach (full arm extension, shoulder forward): ~22-24" (559-610mm)
- Maximum reach (leaning torso into cabinet): ~28-30" (711-762mm), but very uncomfortable

**Posture 2: Crouching/squatting**
- Lower body position, head can enter the cabinet opening
- Better visibility deeper into cabinet
- Similar reach to kneeling, but less stable for two-handed tasks

**Posture 3: Lying on back (reaching up into cabinet)**
- Used for installations, not for routine maintenance
- Irrelevant for cartridge swaps — if this is required, the design has failed

**Reach zones for kneeling posture:**

```
    SIDE VIEW — User kneeling at cabinet

                          cabinet ceiling
    ┌────────────────────────────────────────────┐
    │                                            │
    │    ZONE 3          ZONE 2       ZONE 1     │
    │    (struggle)      (stretch)    (comfort)  │
    │                                            │
    │    18-22"          12-18"       0-12"       │
    │    from front      from front   from front  │
    │                                            │
    └────────────────────────────────────────────┘
          ▲ back wall                  ▲ front
                                       │
                                    ┌──┴──┐
                                    │     │ ← user kneeling
                                    │ ☺   │
                                    │/│\  │
                                    │/ \  │
```

**Ergonomic research establishes:**
- **Zone 1 (Neutral Reach Zone, 0-14" / 0-356mm):** Forearm reach with bent elbow. Comfortable for repetitive tasks, minimal strain. This is where the lever should be.
- **Zone 2 (Extended Reach Zone, 14-19" / 356-483mm):** Full arm extension. Acceptable for occasional tasks but not comfortable for fine manipulation.
- **Zone 3 (Maximum Reach Zone, 19-24" / 483-610mm):** Requires leaning into cabinet, shoulder strain, reduced grip strength. Acceptable only for initial installation, not routine maintenance.

**For a cartridge that the user swaps periodically:** The lever and grip point should be within Zone 1 (0-14" from front). The mating face (where the cartridge plugs in) can be in Zone 2 (up to ~18"), since the insertion motion is a single push.

---

## 2. Lever Position Analysis

The lever must accomplish three things: (1) lock the cartridge, (2) release all 4 collets for removal, (3) serve as a handle/grip during insertion and extraction. Each position is analyzed below for all mounting scenarios.

### 2.1 Top-Mounted Lever

The lever is on the top face of the cartridge front, folding flat when closed (bicycle QR style). It flips upward to unlock.

```
    SIDE VIEW — Top lever (closed / open)

    Closed:                          Open:
    ┌──────────────┐                 ┌──────────────┐
    │ ═══lever═══  │ ← flat          │     ╱lever   │ ← flipped up
    │              │                 │    ╱         │
    │  cartridge   │                 │  cartridge   │
    │              │                 │              │
    └──────────────┘                 └──────────────┘
```

| Criterion | Assessment |
|---|---|
| **Reach without pulling dock out** | POOR if dock is deep. The lever is on TOP, requiring the hand to go over and above the cartridge. At 12"+ depth, the user's hand must reach to the back of the cartridge and then curl over the top to grab the lever. At 16"+, this is extremely awkward. |
| **One-handed flip while holding cartridge** | GOOD. The thumb naturally rests on top. User grips the cartridge body with fingers underneath, thumb flips the lever. |
| **Vertical clearance required** | YES, ~80-100mm above the cartridge for the lever to swing open. If the cartridge is near the cabinet ceiling (under the sink bowl), this is a problem. Minimum clearance above cartridge: ~4" (100mm). |
| **Works in the dark / by feel** | MODERATE. The flat lever is distinct from the cartridge body and easy to find by feel. Flipping direction is intuitive (up = open). |
| **Dock at back wall** | VERY POOR. The lever is the hardest thing to reach — it's the farthest point from the user AND requires a hand-over motion. |
| **Dock at front of cabinet** | GOOD. Short reach, lever is immediately accessible. |
| **Dock at floor level** | GOOD. User reaches down, hand naturally approaches from above. Top lever is the most natural position for floor-level access. |
| **Dock at mid-height** | MODERATE. Works if the dock is at or below elbow height when kneeling. |
| **Dock near cabinet ceiling** | VERY POOR. No vertical clearance for lever swing. User reaches UP, hand approaches from below — top lever is on the wrong side. |
| **Handle utility** | EXCELLENT. The lever doubles as a lift handle. User flips lever up, grips it, and pulls the cartridge straight out. This is the strongest argument for top placement. |

**Summary:** Top lever excels at floor-level installations near the front of the cabinet, and the handle-as-lever feature is compelling. It fails at depth (back wall mounting) and at height (ceiling mounting). It requires 4" of vertical clearance above.

### 2.2 Bottom-Mounted Lever

The lever is on the bottom face of the cartridge, flipping downward to unlock.

```
    SIDE VIEW — Bottom lever (closed / open)

    Closed:                          Open:
    ┌──────────────┐                 ┌──────────────┐
    │              │                 │              │
    │  cartridge   │                 │  cartridge   │
    │              │                 │              │
    │ ═══lever═══  │ ← flat          │    ╲         │
    └──────────────┘                 │     ╲lever   │ ← flipped down
                                     └──────────────┘
```

| Criterion | Assessment |
|---|---|
| **Reach without pulling dock out** | MODERATE. The bottom is accessible by reaching under the cartridge, which is possible if there's clearance below. Less natural than top. |
| **One-handed flip while holding cartridge** | POOR. Holding a 2.1 lb cartridge while flipping a lever on the bottom is very awkward — the hand must grip from above while fingers reach underneath. |
| **Vertical clearance required** | YES, ~80-100mm below the cartridge for the lever to swing. Impossible if the dock is on the cabinet floor. |
| **Works in the dark / by feel** | POOR. The bottom of anything mounted in a cabinet is the hardest surface to reach and feel. |
| **Dock at back wall** | POOR. Same depth problem as top, plus the added difficulty of reaching under. |
| **Dock at front of cabinet** | MODERATE. Accessible but unnatural. |
| **Dock at floor level** | IMPOSSIBLE. No clearance below for lever swing. The cartridge would need to be elevated on a platform, wasting space. |
| **Dock at mid-height or ceiling** | MODERATE to GOOD. If the dock is at waist height or above, the bottom is more accessible (user reaches up, hand approaches from below). |
| **Handle utility** | POOR. A bottom lever is not useful as a lift handle. The user must separately grip the cartridge body to carry it. |

**Summary:** Bottom lever has almost no advantages. It fails at floor level (no clearance), is awkward for one-handed operation, and doesn't serve as a handle. The only scenario where it's acceptable is ceiling-mounted docks where the user reaches up — but that's a poor dock location for other reasons.

### 2.3 Left-Side Lever

The lever is on the left side of the cartridge, swinging outward (leftward) to unlock.

```
    FRONT VIEW — Left side lever (closed / open)

    Closed:                          Open:
    ┌──────────────┐                     ┌──────────────┐
    │              │                     │              │
    │  cartridge   │               lever─┤  cartridge   │
    │              │               ╱     │              │
    │ lever ══     │              ╱      │              │
    └──────────────┘                     └──────────────┘
```

| Criterion | Assessment |
|---|---|
| **Reach without pulling dock out** | GOOD — if the left side is toward the center of the cabinet (user-facing side). The lever is accessible at the same depth as the front face. If the left side is against the cabinet side wall, it's POOR (blocked). |
| **One-handed flip while holding cartridge** | MODERATE. Right-handed users can grip the cartridge with right hand and flip with left thumb/fingers. Left-handed users do the opposite. One-handed operation requires a thumb reach to the side, which is doable but not as natural as top. |
| **Horizontal clearance required** | YES, ~80-100mm to the left of the cartridge. If the dock is against the left cabinet wall, this is impossible. |
| **Works in the dark / by feel** | MODERATE. Side of the cartridge is findable by feel. Lever direction (left = open) is less intuitive than up = open. |
| **Dock against left wall** | IMPOSSIBLE. Lever is blocked by the wall. |
| **Dock against right wall** | GOOD. Left side faces the cabinet interior, lever swings freely. |
| **Handle utility** | POOR. A side lever doesn't help with lifting or carrying. The user must separately grip the cartridge. |

**Summary:** Side levers are position-dependent. They work well when the lever side faces open space, but fail completely when the dock is against a wall on the lever side. This creates an installation constraint: the dock MUST be oriented so the lever side faces away from walls. The handle utility is poor.

### 2.4 Right-Side Lever

Mirror image of left-side. Same analysis applies, flipped. Works when mounted against the left wall, fails against the right wall. The majority of right-handed users would find a right-side lever slightly easier to reach, since the right hand naturally approaches from the right side of the body.

### 2.5 Front-Facing Lever

The lever is on the front face of the cartridge (same face as the mating connections), below or beside the tube ports, swinging forward/downward to unlock.

```
    SIDE VIEW — Front lever (closed / open)

    Closed:                          Open:
    ┌──────────────┐dock             ┌──────────────┐dock
    │              │  │              │              │  │
    │  cartridge   │  │              │  cartridge   │  │
    │              │  │              │              │  │
    │         lever══ │              │         ╱    │  │
    └──────────────┘  │              │        ╱lever│  │
                      │              └──────────────┘  │
                                          ↑ swings toward user
```

Wait — the front face is the mating face, which presses against the dock wall. The lever on the mating face would be inaccessible once the cartridge is inserted, because the mating face is inside the dock.

**This only works if the lever is on the exposed front face of the DOCK, not the cartridge.** Or if the lever is on the cartridge's front face but protrudes forward past the dock opening.

Reframing: a front-facing lever is a lever on the dock's front panel, or a lever on the cartridge that the user accesses from the cabinet-facing side.

| Criterion | Assessment |
|---|---|
| **Reach without pulling dock out** | EXCELLENT. The front face is always the closest point to the user. This is the easiest position to reach at any depth. |
| **One-handed flip while holding cartridge** | MODERATE. The lever is in the same plane as the insertion motion. The user must shift grip from insertion to lever operation, since you can't push in and flip a front lever simultaneously. |
| **Vertical clearance required** | Depends on swing direction. If the lever swings downward, it needs clearance below the dock front face. If it rotates (like a butterfly nut), it needs clearance on both sides. |
| **Works in the dark / by feel** | EXCELLENT. The front face is the first thing the user's hand touches when reaching into the cabinet. |
| **Dock at back wall** | MODERATE. The front face is still the closest part, but at 18-22" depth, even the front is in Zone 2/3. |
| **Dock at any wall** | GOOD. No wall interference — the front always faces the cabinet interior. |
| **Handle utility** | POOR to MODERATE. A front-facing lever doesn't naturally serve as a carry handle. However, if it's a fold-down handle (like a military ammo can), it could work for pulling the cartridge forward during removal. |

**Summary:** Front-facing lever has the best reach characteristics of any position — it's always the closest point to the user regardless of dock orientation. But it doesn't integrate well with the insertion motion (you push, then flip) and has limited handle utility. It also competes for space with the mating connections on the front face.

### 2.6 Integrated Handle (Separate from Lever)

What if the lever and the handle are separate features? A fixed handle on top (or front) for carrying/inserting/removing, and a small lever, button, or latch somewhere accessible for locking/release.

```
    SIDE VIEW — Handle on top, lever on front

    ┌──────────────────┐
    │ ╔══handle══╗     │
    │ ║          ║     │
    │  cartridge       │──── lever (small, front-accessible)
    │                  │
    └──────────────────┘
```

| Criterion | Assessment |
|---|---|
| **Handle for carrying** | EXCELLENT. A dedicated handle can be sized and positioned for optimal grip without compromise. |
| **Lever for locking/release** | GOOD. A small lever or button on the front face can be positioned for easy access without interfering with the mating connections. |
| **One-handed operation** | POOR. Two features (handle + lever) typically require two hands or two separate motions. Defeats the one-handed goal. |
| **Complexity** | HIGHER. Two separate features instead of one dual-purpose feature. More parts, more potential failure points. |

**Summary:** Separating handle and lever solves the "reach" problem (handle where you grip, lever where you reach) but sacrifices one-handed operation. This is worth considering as a fallback if no single lever position works for all scenarios.

### 2.7 Lever Position Summary

| Position | Floor Mount | Mid-Height | Ceiling | Back Wall | Side Wall | Handle? | One-Hand? | Blind? |
|---|---|---|---|---|---|---|---|---|
| **Top** | GOOD | MODERATE | POOR | POOR | OK | YES | YES | MODERATE |
| **Bottom** | FAIL | MODERATE | OK | POOR | OK | NO | POOR | POOR |
| **Left side** | OK | OK | OK | OK | DEPENDS | NO | MODERATE | MODERATE |
| **Right side** | OK | OK | OK | OK | DEPENDS | NO | MODERATE | MODERATE |
| **Front** | GOOD | GOOD | GOOD | MODERATE | GOOD | POOR | MODERATE | GOOD |
| **Handle + lever** | GOOD | GOOD | OK | MODERATE | GOOD | YES | POOR | GOOD |

---

## 3. Insertion Direction Analysis

### 3.1 Horizontal Slide-In from Front (Current Design)

The cartridge slides straight back into a dock mounted on the back wall, a side wall, or a free-standing bracket. The mating face (with tube stubs) faces rearward, engaging fittings in the dock.

```
    TOP VIEW — Horizontal slide-in

    ┌──────────────────────────────────────┐
    │              DOCK                    │ ← back wall (or bracket)
    │         ┌──────────┐                │
    │         │ fittings │                │
    │         └────┬─────┘                │
    │              │                      │
    │     ═══cartridge══► (slides in)     │
    │              │                      │
    │              ▼                      │
    └──────────────────────────────────────┘
    ▲ user kneels here
```

**Grip during insertion:** The user holds the cartridge from behind (the rear face) or from the sides. Natural grip: palm on the rear face, fingers wrapped around sides. For a 140mm wide x 90mm tall cartridge, this is a comfortable palmar grip — like holding a thick book spine-out.

**Natural hand/arm motion:** Push straight forward, like sliding a book onto a shelf. The arm extends naturally into the cabinet. This is the most intuitive motion for under-cabinet work.

**Gravity:** Neutral. Gravity acts downward, perpendicular to the insertion axis. The user must support the cartridge weight but doesn't fight gravity during insertion. Guide rails in the dock bear the weight once the cartridge is partially inserted.

**Alignment visibility:** POOR to MODERATE. The user must align tube stubs with fittings that are hidden inside the dock. The mating face faces away from the user. The user cannot see the alignment — they must rely on guide rails for coarse alignment and feel for final seating. Chamfered entry features on the dock help.

**Feedback for full seating:** The John Guest fittings produce a tactile "push through" feel as the tube stubs seat past the collet teeth. With 4 fittings engaging simultaneously, the user feels a distinct increase in resistance followed by a release as the tubes pop past the collets. This is clear feedback. The lever then locks to confirm.

**Removal:** User opens the lever (releasing collets), then pulls the cartridge straight toward themselves. This is the most natural removal motion — pulling things toward you from inside a cabinet.

**Drip management:** When the cartridge is removed, residual water in the tubes will drip from the 4 tube stubs on the cartridge's front face (now facing the user). The drips fall downward. If the user tilts the cartridge forward slightly during removal, drips fall onto the cabinet floor (manageable with a drip tray). If the cartridge exits level, drips may fall on the user's hand. A small drip tray under the dock catches most drips during the transition.

**Overall:** This is the most natural insertion direction for under-cabinet work. The push motion, the guide rail support, and the pull-to-remove all feel intuitive. The main weakness is alignment visibility — the user can't see the mating face during insertion.

### 3.2 Horizontal Slide-In from Side

The cartridge slides left-to-right (or right-to-left) into a dock mounted on a side wall.

```
    TOP VIEW — Side slide-in

    ┌──────────────────────────────────────┐
    │                                      │
    │         DOCK ═══► cartridge          │ ← right wall
    │         (on right wall)              │
    │                                      │
    │                                      │
    └──────────────────────────────────────┘
    ▲ user kneels here
```

**Grip during insertion:** The user holds the cartridge with the insertion end facing the wall. The grip is on the side closest to the user (the end they'll push). This is like pushing a drawer closed sideways — workable but not as natural as pushing straight ahead.

**Natural hand/arm motion:** The arm extends to the side and pushes laterally. This is an awkward motion from a kneeling position — the shoulder must rotate and the elbow extends sideways. It's biomechanically weaker than a straight push.

**Gravity:** Neutral (same as front slide-in).

**Alignment visibility:** MODERATE. Depending on which side, the user may be able to look along the insertion axis and see the mating engagement. If the dock is on the right wall and the user leans right, they can see the tubes approaching the fittings. Better than front slide-in for visibility, worse for reach comfort.

**Feedback for full seating:** Same tactile feedback from John Guest fittings.

**Removal:** User must pull the cartridge laterally (toward themselves if the dock is on the near wall, away if on the far wall). Pulling sideways from a kneeling position is awkward, especially with a 2.1 lb cartridge and dripping tubes.

**Drip management:** Tube stubs face sideways. Drips fall from the side of the cartridge, potentially onto the cabinet floor in a hard-to-tray location. Less contained than front-entry drips.

**Overall:** Side entry has some visibility advantages but suffers from awkward arm ergonomics and removal difficulty. It makes sense only if the dock MUST go on a side wall and there's no other option.

### 3.3 Vertical Drop-In from Above

The cartridge drops into a cradle or well from above. Like inserting a water filter cartridge or a battery into a flashlight.

```
    SIDE VIEW — Vertical drop-in

                    cartridge
                    ┌──────┐
                    │      │
                    │  ▼   │ (drops down)
                    │      │
                    └──┬───┘
    ┌──────────────────┴───────────────────┐
    │              DOCK (cradle)            │
    │         ┌──────────┐                 │
    │         │ fittings │ ← at bottom     │
    │         └──────────┘                 │
    └──────────────────────────────────────┘
```

**Grip during insertion:** The user holds the cartridge from above (pinch grip on top edges) or from the sides (palmar grip). For a 140mm x 100mm footprint at 2.1 lbs, a side grip is comfortable. The user lowers it into the cradle like setting down a coffee mug.

**Natural hand/arm motion:** Lowering an object downward is one of the most natural human motions. Gravity assists. The arm motion is comfortable and controlled.

**Gravity:** HELPS. Gravity pulls the cartridge toward the fittings. The user's task is controlled lowering, not pushing. The weight of the cartridge can even help seat the tube stubs into the fittings.

**Alignment visibility:** EXCELLENT. The user looks down into the cradle and can see the alignment features (guide pins, tube stubs approaching fittings). This is the only insertion direction where the user has a clear view of the mating engagement.

**Feedback for full seating:** The cartridge drops to a stop. The fittings engage with the same tactile feel. Gravity holds it in place. A lever or latch confirms docking.

**Removal:** User opens the lever/latch, then lifts the cartridge straight up. This requires gripping the cartridge (handle on top is ideal) and overcoming the John Guest fitting retention plus the cartridge weight. Total extraction force: fitting retention (~5-10 lbs per fitting pulling out, so ~20-40 lbs total for 4 fittings) plus 2.1 lbs weight. This is the problem — extracting tubes from John Guest fittings requires pressing each collet. The release plate is essential here, and it must be operated BEFORE lifting.

**Drip management:** GOOD. When the cartridge is lifted out, tube stubs point downward. Drips fall back into the cradle (which can have a drain or absorbent pad). The user never gets dripped on. This is the best drip management of any direction.

**Overall:** Vertical drop-in has outstanding visibility, natural motion, gravity assistance, and drip management. The challenges: (1) it requires vertical space above the cradle for insertion/removal (cartridge height of 90mm + hand clearance = ~200mm minimum above the cradle), (2) the lever/release mechanism must be operated before lifting, adding a step, and (3) the cradle occupies floor space and makes the area around it less usable. The water filter industry has settled on this direction for good reason.

### 3.4 Vertical Push-Up from Below

The cartridge pushes upward into a dock mounted on the ceiling of the cabinet (underside of the sink or countertop).

```
    SIDE VIEW — Vertical push-up

    ┌──────────────────────────────────────┐
    │              DOCK (ceiling)           │ ← underside of sink/countertop
    │         ┌──────────┐                 │
    │         │ fittings │ ← pointing down │
    │         └──────────┘                 │
    └──────────────────┬───────────────────┘
                       │
                    ┌──┴───┐
                    │      │
                    │  ▲   │ (pushes up)
                    │      │
                    └──────┘
                    cartridge
```

**Grip during insertion:** The user holds the cartridge from below, pressing it upward. For a 2.1 lb cartridge, this is manageable but tiring if alignment takes time. The user grips the bottom and sides of the cartridge, palms up.

**Natural hand/arm motion:** Pushing upward is work — the user fights gravity for the full duration of alignment and insertion. Arms tire quickly when raised overhead (or in this case, above elbow height inside a cabinet).

**Gravity:** HURTS. Gravity fights the insertion motion. The user must support the full cartridge weight continuously. If they let go before the fittings engage, the cartridge falls.

**Alignment visibility:** POOR. The user looks up into a dark ceiling-mounted dock. The mating face is above them, in shadow. Alignment is essentially blind.

**Feedback for full seating:** Fittings engage, but the user may not feel it clearly because they're fighting gravity — the "push through" feel is masked by the constant upward force.

**Removal:** Release the lever, and gravity drops the cartridge down. The user must catch a 2.1 lb cartridge that suddenly releases. This is uncontrolled and potentially dangerous (dropping the cartridge, splashing residual water).

**Drip management:** TERRIBLE. When the cartridge releases and drops, residual water in the tube stubs is now pointing up — it sprays/drips as the tubes pull free from the fittings. Water drips down onto the user, the cabinet floor, everything.

**Overall:** Push-up insertion is the worst option. It fights gravity, has poor visibility, requires sustained effort, has dangerous removal dynamics, and sprays water. The only reason to consider it is if floor and wall space are completely unavailable and the ceiling is the only mounting surface — an extremely unusual situation.

### 3.5 Angled Insertion (30-45 degrees)

The cartridge inserts at an angle, combining horizontal and downward components. Think of a toaster slot tilted 30-45 degrees from horizontal.

```
    SIDE VIEW — Angled insertion (30° from horizontal)

    ┌──────────────────────────────────────┐
    │         DOCK (angled bracket)         │
    │              ╲                        │
    │               ╲ fittings             │
    │                ╲                      │
    │                 ╲                     │
    │          cartridge ╲                  │
    │          slides in  ╲                 │
    │          at angle    ╲                │
    └──────────────────────────────────────┘
    ▲ user kneels here
```

**Grip during insertion:** The user holds the cartridge with a natural grip and pushes it forward-and-downward. The angle means the wrist is in a neutral position (not fully pronated or supinated). Comfortable.

**Natural hand/arm motion:** The push-down-and-forward motion is quite natural — it's how you'd push something into a mailbox slot or feed bread into a toaster. Gravity partially assists (the downward component gets a gravity boost).

**Gravity:** PARTIALLY HELPS. The vertical component of insertion is assisted by gravity. The cartridge weight helps seat the fittings. During removal, the user lifts slightly (fighting gravity) while pulling forward — but only the vertical component, not the full weight.

**Alignment visibility:** MODERATE to GOOD. The mating face is angled toward the user. Depending on the angle, the user may be able to see the tube stubs approaching the fittings. At 30 degrees from horizontal, the mating face is partially visible from a kneeling position. Better than horizontal, not as good as vertical drop-in.

**Feedback for full seating:** Same tactile feedback, enhanced slightly by the gravity-assisted seating force.

**Removal:** User opens lever, then pulls forward and slightly upward. The cartridge slides out along the angled track. Gravity provides a slight "pulling" assist as the cartridge clears the dock — the weight wants to drop, which helps extraction.

**Drip management:** GOOD. When the cartridge is removed, tube stubs point backward-and-downward at 30-45 degrees. Drips fall toward the back of the dock rather than toward the user. A drip tray at the base of the angled cradle catches them.

**Overall:** Angled insertion is an excellent compromise. It gets many of the visibility and gravity benefits of vertical drop-in while maintaining the intuitive push motion of horizontal slide-in. The angle also means the dock takes less vertical space than a full vertical cradle (the cartridge's height is projected along the angle, reducing required vertical clearance). The main complexity is the angled bracket/rail design and the less intuitive "which angle?" question during assembly.

### 3.6 Insertion Direction Summary

| Direction | Grip | Motion | Gravity | See Alignment | Seated Feel | Removal | Drips |
|---|---|---|---|---|---|---|---|
| **Horiz. front** | Rear palm | Push forward | Neutral | Poor | Good | Pull toward self | Moderate |
| **Horiz. side** | Side palm | Push sideways | Neutral | Moderate | Good | Pull sideways | Poor |
| **Vert. drop-in** | Top/sides | Lower down | Helps | Excellent | Good | Lift up (after release) | Excellent |
| **Vert. push-up** | Bottom | Push up | Fights | Poor | Masked | Catch falling unit | Terrible |
| **Angled (30-45°)** | Rear palm | Push forward+down | Partially helps | Moderate-Good | Good | Pull forward+up | Good |

---

## 4. The "Reach Depth" Problem

### 4.1 Core Problem Statement

The dock is mounted inside a cabinet. The user operates it from outside the cabinet. The distance between the user's comfortable reach and the dock's location is the fundamental ergonomic constraint.

The question isn't "can the user reach it?" — the question is "can the user comfortably, repeatedly, one-handedly operate a lever at this depth while holding a 2.1 lb cartridge?"

### 4.2 Depth Limits by Lever Position

The effective reach limit depends on what the user needs to do at that depth:

**For simple pushing/pulling (insertion/removal):**
- Comfortable limit: ~16" (406mm) from cabinet front
- Maximum: ~22" (559mm) — full arm extension

**For fine manipulation (flipping a lever, pressing a button):**
- Comfortable limit: ~12" (305mm) from cabinet front
- Maximum: ~16" (406mm) — extended arm, reduced dexterity

**For simultaneous grip + manipulation (holding cartridge + operating lever):**
- Comfortable limit: ~10" (254mm) from cabinet front
- Maximum: ~14" (356mm) — at this depth, the user struggles to maintain grip while operating the lever

This means the LEVER must be within ~10-14" of the cabinet front opening. The mating face (where the cartridge actually connects) can be 4-6" deeper than the lever.

### 4.3 How Lever Position Affects Depth Budget

```
    SIDE VIEW — Depth budget for horizontal slide-in

    ├── 22" cabinet interior depth ──────────────────────────────────►│
    │                                                                  │
    │  back wall                                          front opening│
    │    │                                                      │     │
    │    │◄── dock body ──►│◄── cartridge depth ──►│            │     │
    │    │    (~25mm)       │    (~100mm / 4")      │            │     │
    │    │                  │                       │            │     │
    │    │                  │                       ├─ lever ─►  │     │
    │    │                  │                       │            │     │
    │    │◄────── total protrusion: ~5.5" ────────►│            │     │
    │    │                                                      │     │
    │    │◄──────── distance to lever: ~16.5" ─────────────────►│     │
    │                                                                  │
```

**With a back-wall-mounted dock (worst case):**
- Dock back plate: flush with back wall (22" from front)
- Dock body: 1" (25mm) deep
- Cartridge depth: 4" (100mm)
- Front face of cartridge (where lever would be): ~17" (432mm) from front
- Lever is at ~17" depth — this is in Zone 3 (maximum reach), beyond comfortable lever operation

**This means back-wall mounting does not work for horizontal slide-in unless the dock pulls out.**

**With a side-wall-mounted dock:**
- Dock projects from side wall into cabinet
- The front face of the cartridge can be positioned at any depth
- If the dock is mounted with the cartridge front face at 8-12" from the cabinet front, the lever is comfortably reachable

**With a front-of-cabinet mounting:**
- Dock projects from the front, cartridge slides backward
- Lever is at 0-4" depth — trivially accessible
- But the dock blocks the cabinet opening

### 4.4 How Lever Position on the Cartridge Affects the Depth Budget

If the lever is on the **front** of the cartridge (user-facing side):
- Lever depth = dock depth + cartridge depth from mating face
- The lever is at the closest point to the user

If the lever is on the **top** of the cartridge:
- Lever depth = dock depth + (some fraction of cartridge depth)
- The lever is at the same depth as the middle of the cartridge, but requires reaching OVER the top — effectively deeper by 2-3"

If the lever is on the **back** of the cartridge (mating face side):
- Lever depth = dock depth
- The lever is at the DEEPEST point — terrible for reach

**The front-facing lever wins the depth budget.** It adds zero depth to the reach requirement — the lever is always at the user-facing edge of the cartridge.

### 4.5 Solutions to the Depth Problem

#### Solution A: Pull-Out Shelf / Drawer Slides

Mount the dock on a pull-out shelf with ball-bearing drawer slides. The user pulls the entire dock forward to access the cartridge, operates the lever, swaps the cartridge, then pushes the dock back.

**Advantages:**
- Moves the lever into Zone 1 regardless of where the dock is permanently mounted
- Allows back-wall mounting without reach issues
- Familiar paradigm (pull-out trash cans, pull-out under-sink organizers are common)
- Full-extension slides (like Blum Tandem) provide smooth, controlled motion

**Disadvantages:**
- Adds mechanical complexity (drawer slides, mounting rails)
- Fluid connections (water lines to the dock) must flex with the slide motion — requires flexible tubing with enough slack
- Electrical connections to the dock must also flex
- Adds cost: good full-extension slides are $15-40 per pair
- Adds depth: the slides themselves occupy 1-2" of depth when retracted

**Product reference:** Rev-A-Shelf (rev-a-shelf.com) makes U-shaped under-sink pull-out organizers specifically designed to work around P-traps. Their 544 series uses Blum Tandem soft-close slides and fits 30-36" sink base cabinets. Concept is directly applicable.

#### Solution B: Angled Dock Mounting

Mount the dock at an angle (30-45 degrees from the back wall), so the cartridge insertion path aims toward the cabinet opening rather than straight back.

```
    TOP VIEW — Angled dock on side wall

    ┌──────────────────────────────────────┐
    │                                      │
    │              ╲ dock                  │ ← right wall
    │               ╲                      │
    │     cartridge  ╲                     │
    │     slides      ╲                    │
    │     at angle     ╲                   │
    │                                      │
    └──────────────────────────────────────┘
    ▲ user kneels here (lever is close)
```

**Advantages:**
- Reduces effective reach depth (the lever is closer to the front even though the dock is on a side/back wall)
- No moving parts
- Simple bracket fabrication

**Disadvantages:**
- Uses floor space less efficiently (angled footprint)
- Cartridge must be inserted at an angle, which is less intuitive
- Tube routing from dock to plumbing is more complex

#### Solution C: Front-Accessible Lever (Lever on User-Facing Side)

Design the lever to protrude from the front of the dock, regardless of dock orientation. The lever connects to the cam/release plate via a linkage that transmits motion around the corner.

**Advantages:**
- Lever is always at minimum depth
- Dock can be mounted anywhere

**Disadvantages:**
- Adds linkage complexity to the mechanism
- The lever is no longer on the cartridge — it's on the dock, and the user can't carry the cartridge by the lever

#### Solution D: Side-Wall Mounting Near Front

Mount the dock on a side wall, positioned so the front face of the cartridge is 8-10" from the cabinet front opening.

```
    TOP VIEW — Side-wall dock positioned near front

    ┌──────────────────────────────────────┐
    │                                      │
    │                                      │
    │                                      │
    │                                      │
    │          DOCK ═══► cartridge         │ ← right wall
    │          │         │                 │
    │          │    8-10" from front       │
    │          │                           │
    └──────────────────────────────────────┘
    ▲ user kneels here
```

**Advantages:**
- Simple — just mount the dock in the right position
- No moving parts, no linkages, no slides
- Lever is within comfortable reach

**Disadvantages:**
- Doesn't use all available cabinet depth (wastes the back 10-12")
- May conflict with plumbing if the near-front zone overlaps with supply lines or P-trap
- Side-wall mounting means the insertion direction is sideways (see Section 3.2)

### 4.6 How Existing Under-Sink Systems Handle Reach

| System | Reach Solution | Notes |
|---|---|---|
| **Waterdrop twist-lock filter** | Filter hangs from a head unit mounted at mid-height on back wall. Quarter-turn release, cartridge drops straight down. | Head unit is only 3-4" from back wall. Replacement filter is grabbed from below. Total reach: 18-20" but motion is simple (twist + pull down). |
| **3M/Aqua-Pure twist-lock** | Same pattern: head on wall, filter hangs. | The head is the deepest point; user only touches the filter body, which hangs closer to the front. |
| **APEC RO system (traditional)** | Canisters sit on cabinet floor, near the front. Canister unscrews from a head above. | Canisters are intentionally positioned 6-10" from front. The wrench operation requires both hands and good reach of the canister body. |
| **Soap dispenser pump** | Bottle sits on cabinet floor, user reaches in to swap. | No mechanism — just lift out the old bottle and set in a new one. Always positioned near the front for easy access. |
| **Instant hot water tank** | Mounted to side wall with bracket. Rarely accessed (only for replacement). | Not optimized for frequent access. Installation depth varies. |
| **Pull-out trash can** | Drawer slides, full extension. User pulls entire bin to cabinet front. | This is the gold standard for under-sink reach: bring the thing to the user, don't make the user reach to the thing. |

**The pattern is clear:** Systems designed for frequent user interaction (filters, trash cans) either (a) position themselves near the front of the cabinet or (b) use pull-out mechanisms to come to the user. Systems that are rarely accessed (hot water tanks, soap dispensers) accept deeper placement.

Our cartridge is replaced periodically (maybe every few months). It's not daily access, but it needs to be easy enough that the user doesn't dread it. This puts it in the "near the front or pull-out" category.

---

## 5. Grip and Handle Ergonomics

### 5.1 The Lever-as-Handle Concept

If the lever doubles as a handle, the user can:
1. Flip the lever to unlock (releasing collets)
2. Grip the lever
3. Pull the cartridge out, using the lever as a handle
4. Carry the cartridge to the counter by the lever
5. Insert the new cartridge by holding the lever
6. Push it in until it seats
7. Flip the lever to lock

This is a beautiful single-feature design — **one part serves two functions in a continuous workflow**. The bicycle quick-release is the proof: the lever is both the locking mechanism and the thing you grab to remove the wheel.

### 5.2 Handle Sizing for Comfort

Ergonomic research on power grip handles (tools, carry handles):

| Parameter | Recommended | Source |
|---|---|---|
| Handle diameter (cylindrical) | 30-50mm (1.2-2.0"), ideal ~33-38mm | CCOHS, PMC research |
| Handle length (minimum) | 100mm (4") | CCOHS — to span the palm and prevent soft tissue compression |
| Handle length (comfortable) | 110-130mm (4.3-5.1") | Accommodates 95th percentile male hand breadth |
| Weight limit for one-handed tools | 1.4 kg (3.1 lbs) | CCOHS — tools above this should have counterbalance/support |

**Our cartridge at 940g (2.1 lbs) is within the one-handed weight limit.** A handle 110-130mm long and 30-40mm in diameter would be comfortable for nearly all users.

For a lever that doubles as a handle:
- The lever must be at least 100mm long (this is already in the cam-lever.md range of 50-100mm; we should target the upper end)
- The lever cross-section should be oval or D-shaped, 30-40mm wide, to fill the palm
- The lever must be rigid enough to support 940g without flexing (PETG at 4mm wall thickness is adequate)

### 5.3 Lever State During Carry

**When carrying the cartridge to/from the dock, should the lever be open or closed?**

**Option A: Carry with lever OPEN (unlocked)**
- The lever is flipped up (if top-mounted), sticking up above the cartridge
- The user grips the open lever like a suitcase handle
- Pro: The lever is already in the "ready to dock" position — just push in and flip closed
- Con: The open lever may flop or rotate during carry if not designed with friction/detent
- Con: The lever's open position adds height to the package during carry

**Option B: Carry with lever CLOSED (locked)**
- The lever is folded flat against the cartridge
- The user grips the cartridge body directly (no handle assist)
- Pro: Compact profile during carry
- Con: After insertion, the user must separately flip the lever open (it's already closed/locked, which is the desired state) — wait, this doesn't make sense. The lever must be open for insertion (release plate retracted) and closed after insertion (locked). So the user carries it with the lever in whatever state they left it.

**Recommended flow:**
1. User opens lever on OLD cartridge (collets release, plate extends)
2. User grips lever, pulls old cartridge out (lever is OPEN, serving as handle)
3. User sets down old cartridge, picks up new cartridge
4. New cartridge's lever is in OPEN position (from its previous removal, or factory default)
5. User grips new cartridge by the open lever, slides it in
6. User flips lever CLOSED (locking)

**The lever is OPEN during carry in both directions.** This means the handle is always available when the user needs to carry the cartridge.

### 5.4 Grip During Insertion and Removal

For horizontal slide-in with a top lever:

```
    Insertion grip:

              lever (open, pointing up)
               ╱
    ┌─────────╱───────┐
    │ ╔══════╝        │
    │ ║   thumb on    │
    │ ║   lever       │◄── fingers wrap
    │ ║               │    around sides
    │ ╚═══════════════│    and bottom
    └─────────────────┘
         push ──────►
```

The user's thumb rests on the lever (or grips the lever from above), with fingers wrapped around the sides and bottom of the cartridge. The push motion is natural. To lock: thumb pushes lever flat (forward and down). One hand throughout.

For removal:

```
    Removal grip:

              lever (being flipped open)
               ╱
    ┌─────────╱───────┐
    │ ╔══════╝        │ ← thumb flips lever up
    │ ║               │
    │ ║               │◄── fingers wrapped
    │ ║               │    around sides
    │ ╚═══════════════│
    └─────────────────┘
         ◄────── pull
```

The user reaches in, finds the lever by feel (the distinct raised shape on top), flips it up with the thumb, then wraps fingers around and pulls. One hand throughout.

### 5.5 Alternative Handle Forms

| Handle Type | Description | Grip Quality | Lever Integration | Manufacturing |
|---|---|---|---|---|
| **Bar handle (lever = bar)** | Lever is a straight bar, 100-130mm long, 30-40mm cross-section | Excellent | Natural — the lever IS the handle | Easy to 3D print |
| **Strap handle** | Flexible strap (webbing or silicone) attached to cartridge top | Good for carry, poor for insertion | Separate from lever | Requires non-printed material |
| **Recessed grip** | Finger holes molded into cartridge sides | Moderate | Separate from lever | Easy to print but adds width |
| **T-handle** | Lever has a crossbar at the end (like a corkscrew handle) | Excellent for pull, poor for push | Integrated | Adds protrusion |
| **D-ring** | Metal D-ring on top, folds flat | Good for pull | Separate from lever | Requires hardware |

**The bar handle (lever = bar) is the clear winner.** It's the simplest, integrates lever and handle into one part, and is trivially 3D printable. The bicycle QR lever is exactly this — a flat bar that pivots.

### 5.6 Prior Art: How Other Cartridge Systems Handle Carry + Insert

| Product | How You Carry It | How You Insert It | How You Lock It |
|---|---|---|---|
| **DeWalt 20V battery** | Grip body (no handle) | Slide into rails until click | Automatic spring latch |
| **Waterdrop RO filter** | Grip cylindrical body | Push up into head | Quarter-turn twist |
| **HP ink cartridge** | Pinch between fingers (tiny) | Drop into slot | Push down lever from above |
| **Keurig K-Cup** | Pinch between fingers | Drop into holder | Close lid |
| **Dyson V-series battery** | Grip body, press release button | Slide in until click | Automatic latch |
| **Server blade (HP ProLiant)** | Grip ejector levers (two, one per side) | Slide into rails, push levers closed | Cam lever locks |
| **Bicycle wheel (QR)** | Grip axle or tire | Set into dropouts | Flip QR lever closed |

**The server blade is the closest analog to our design.** The ejector levers serve as both the insertion/extraction mechanism AND the handles. Two levers (one per side) provide balanced grip. For our smaller, lighter cartridge, a single lever on top is sufficient.

---

## 6. Accessibility and Edge Cases

### 6.1 Reduced Grip Strength (Older Users, Arthritis)

Our cartridge at 2.1 lbs with a lever-handle requires:
- **Grip force to hold:** ~20-25N (4.5-5.5 lbs). Average grip strength for 65+ women is ~220N (49 lbs), so this is well within range even for elderly users.
- **Lever force to flip:** The cam mechanism provides 10:1+ mechanical advantage. With 12-20N total collet force, the user applies ~1.2-2N (4-7 oz) at the lever tip. Trivial for any grip strength.
- **Pinch force for fine manipulation:** Minimal — the lever is large enough for a full palmar grip, not a pinch.

**Assessment:** The mechanism forces are very low. The main challenge for reduced-grip users is holding the cartridge steady while operating the lever one-handed. A two-step approach (set cartridge on a shelf/tray, then operate lever separately) would accommodate users who can't do both simultaneously.

### 6.2 One Hand Occupied

Scenarios where the user has only one free hand:
- **Holding a flashlight:** Very common under sinks. The cabinet interior is dark. A headlamp solves this, but many users will use a phone flashlight or handheld light.
- **Holding the cabinet door open:** Spring-loaded hinges on some cabinets. A door prop or magnetic catch solves this permanently.
- **Bracing against the cabinet frame:** Some users steady themselves with one hand when kneeling.

**The lever-as-handle design is critical here.** If the lever and handle are the same part, the user needs only ONE hand for the entire operation: grip lever, flip to unlock, pull out, carry, insert new, flip to lock. If lever and handle are separate, the user needs two hands or two motions per step.

### 6.3 Wet or Soapy Hands

The cartridge lives under a kitchen sink. Water drips, soap splashes, and condensation are all possible.

**Design responses:**
- Lever/handle surface: textured grip (knurling, ribs, or rubberized overmold). 3D-printed surface can have printed-in texture at no cost.
- Cartridge body: avoid smooth flat surfaces. Printed layer lines actually help grip (unintentionally textured).
- Electrical contacts: already isolated from water (different face, per mating-face.md)
- Guide rails: PTFE or silicone lubricant on rail surfaces to prevent sticking when wet

### 6.4 Hand Size Variation

| Measurement | 5th %ile Female | 50th %ile Male | 95th %ile Male |
|---|---|---|---|
| Hand length | 160mm (6.3") | 189mm (7.4") | 205mm (8.1") |
| Hand breadth | 69mm (2.7") | 89mm (3.5") | 97mm (3.8") |
| Grip span (power grip) | 43mm (1.7") | 51mm (2.0") | 58mm (2.3") |

The cartridge at 140mm wide x 90mm tall is grippable for all hand sizes:
- Small hands: grip the 90mm dimension (height) with fingers reaching around ~half the 140mm width. Comfortable.
- Large hands: can nearly span the 140mm width. Very comfortable.

The lever at 100-130mm long accommodates all hand breadths (69-97mm). The lever bar diameter of 30-40mm is within the comfortable power grip range for all sizes.

### 6.5 Left-Handed vs Right-Handed Users

For a **top-mounted lever:**
- Ambidextrous. The lever runs left-right across the top. Either hand can flip it, grip it, and operate it. No handedness preference.

For a **side-mounted lever:**
- Left-side lever: easier for right-handed users (right hand grips cartridge, left thumb flips lever). Harder for left-handed users.
- Right-side lever: opposite.
- Either choice disadvantages ~10% of users.

**Top lever is the most inclusive position for handedness.**

### 6.6 The "I Don't Want to Get on the Floor" Scenario

Some users (elderly, back problems, in business attire) will strongly prefer not to kneel. Can the cartridge be changed while standing and reaching down into the cabinet?

**Standing reach into a cabinet:**
- Cabinet opening is at ~4.5" height (toe kick) to ~34.5" (countertop)
- User stands in front of open cabinet doors, bends at the waist
- Arm reaches down and forward into the cabinet
- Effective reach: reduced compared to kneeling — the user's arm is fighting gravity AND reaching forward simultaneously
- Comfortable reach depth (from front) while standing and bending: ~8-10" (203-254mm)
- Maximum: ~14-16" (356-406mm) with significant stooping

**If the dock is in the front 10" of the cabinet, standing operation is feasible.** The user bends at the waist, reaches in, finds the lever by feel, operates it, and pulls out/pushes in the cartridge. This strongly favors:
- Front-of-cabinet dock placement
- Top lever (the hand approaches from above when standing)
- Lever that can be found by feel

**A pull-out shelf (Section 4.5, Solution A) is the ultimate standing-friendly solution.** Pull the shelf out to the cabinet front, then operate the lever while barely bending.

---

## 7. Comparison Matrix

### 7.1 Scoring Key

Each combination is scored 1-5:
- **5** = Excellent, works naturally in nearly all scenarios
- **4** = Good, works well with minor limitations
- **3** = Acceptable, workable but not ideal
- **2** = Poor, significant ergonomic compromise
- **1** = Impractical, should be avoided

### 7.2 Lever Position x Insertion Direction Matrix

#### Horizontal Slide-In from Front + Various Lever Positions

| Criterion (weight) | Top Lever | Front Lever | Left Side | Right Side | Handle + Separate Lever |
|---|---|---|---|---|---|
| Reach comfort (5) | 3 | 4 | 3 | 3 | 4 |
| Lever access (5) | 4 | 5 | 3 | 3 | 4 |
| Grip during insertion (4) | 5 | 3 | 3 | 3 | 4 |
| Removal ease (4) | 5 | 4 | 3 | 3 | 4 |
| Blind operation (3) | 4 | 5 | 3 | 3 | 4 |
| Drip management (2) | 3 | 3 | 3 | 3 | 3 |
| Handle utility (3) | 5 | 2 | 2 | 2 | 5 |
| **Weighted Total (/130)** | **107** | **101** | **78** | **78** | **104** |

#### Vertical Drop-In + Various Lever Positions

| Criterion (weight) | Top Lever | Front Lever | Left Side | Right Side | Handle + Separate Lever |
|---|---|---|---|---|---|
| Reach comfort (5) | 3 | 4 | 4 | 4 | 4 |
| Lever access (5) | 3 | 4 | 4 | 4 | 4 |
| Grip during insertion (4) | 3 | 3 | 4 | 4 | 4 |
| Removal ease (4) | 4 | 3 | 3 | 3 | 4 |
| Blind operation (3) | 3 | 4 | 3 | 3 | 3 |
| Drip management (2) | 5 | 5 | 5 | 5 | 5 |
| Handle utility (3) | 5 | 2 | 2 | 2 | 5 |
| **Weighted Total (/130)** | **91** | **94** | **93** | **93** | **106** |

#### Angled Insertion (30-45°) + Various Lever Positions

| Criterion (weight) | Top Lever | Front Lever | Left Side | Right Side | Handle + Separate Lever |
|---|---|---|---|---|---|
| Reach comfort (5) | 4 | 4 | 3 | 3 | 4 |
| Lever access (5) | 4 | 5 | 3 | 3 | 4 |
| Grip during insertion (4) | 4 | 3 | 3 | 3 | 4 |
| Removal ease (4) | 4 | 4 | 3 | 3 | 4 |
| Blind operation (3) | 4 | 5 | 3 | 3 | 4 |
| Drip management (2) | 4 | 4 | 4 | 4 | 4 |
| Handle utility (3) | 5 | 2 | 2 | 2 | 5 |
| **Weighted Total (/130)** | **107** | **101** | **80** | **80** | **106** |

### 7.3 Top Combinations Ranked

| Rank | Combination | Score | Key Strengths | Key Weaknesses |
|---|---|---|---|---|
| **1** | **Horiz. front slide-in + top lever** | **107** | Handle = lever, natural push/pull, good one-hand operation | Alignment visibility poor, needs 4" clearance above, depth-dependent |
| **1 (tie)** | **Angled (30-45°) + top lever** | **107** | Same handle benefits, better visibility, gravity assists | Angled bracket adds complexity, less intuitive insertion angle |
| **3** | **Angled + handle + separate lever** | **106** | Best reach, best handle, flexible lever placement | Two features, two-hand risk, more complex design |
| **3 (tie)** | **Vert. drop-in + handle + separate lever** | **106** | Best drip management, best visibility | Two features, vertical space needed, two-hand risk |
| **5** | **Horiz. front + handle + separate lever** | **104** | Good all-around, flexible | Two features, complexity |
| **6** | **Horiz. front + front lever** | **101** | Best lever access and blind operation | Poor handle utility, lever competes with mating face |

### 7.4 Analysis of Top Picks

**The horizontal front slide-in with top lever is the strongest overall design.** It scores highest or ties for highest in every analysis. The top lever's dual role as handle is a major ergonomic advantage that no other lever position matches. The main risks are:

1. **Depth-dependent reach:** If the dock is more than ~14" from the cabinet front, the top lever becomes hard to reach. This is solvable by (a) mounting the dock on a side wall near the front, (b) using a pull-out shelf, or (c) keeping the dock near the front of the cabinet.

2. **Vertical clearance:** The lever needs ~4" above the cartridge to swing. This rules out ceiling-height dock placement but is fine for floor and mid-height.

3. **Alignment visibility:** The user can't see the mating face during horizontal insertion. This is solved by good guide rail design (coarse-then-fine alignment, chamfered entries, tactile feedback at full seating).

**The angled insertion variant ties the horizontal design** and adds better visibility plus gravity assistance. The trade-off is bracket complexity and a less intuitive "which angle?" during use. For a V2 design or if visibility proves problematic in testing, the angled approach is worth exploring.

**The vertical drop-in with a separate handle scores well** and has the best drip management and visibility. But it sacrifices the lever-as-handle integration and risks requiring two hands. It's the water filter industry's answer, and it works, but it doesn't achieve the one-handed, lever-as-handle elegance of the horizontal + top lever design.

---

## 8. Recommendations

### 8.1 Primary Recommendation: Horizontal Front Slide-In + Top Lever

Proceed with the current design direction. The horizontal slide-in with a top-mounted lever (bicycle QR style) is the best overall combination for the constraints of this project.

**Critical design constraints that flow from this recommendation:**

1. **Dock placement depth:** The lever must be within 12-14" (305-356mm) of the cabinet front opening. For a 22" deep cabinet, the dock should be mounted no deeper than 8-10" from the front. This rules out back-wall mounting unless a pull-out shelf is used.

2. **Vertical clearance:** Allow a minimum of 100mm (4") above the installed cartridge for lever swing. Do not mount the dock within 190mm (7.5") of the cabinet ceiling (90mm cartridge height + 100mm lever swing).

3. **Lever dimensions:** Target 110-130mm long, 30-40mm wide, flat oval cross-section. This provides comfortable grip for 5th-95th percentile hands while serving as an effective carry handle.

4. **Guide rail design quality is critical:** Since the user cannot see the mating face during horizontal insertion, the guide rails MUST provide confident, sloppy-tolerant alignment. Wide entry funnels (5-10mm wider than the cartridge at the mouth, tapering to final fit over the last 20-30mm) are essential.

5. **Drip tray:** Include a removable drip tray under the dock. Horizontal removal causes tube stubs to drip toward the user. The tray catches these drips and the user empties it during cartridge swaps.

### 8.2 Fallback: Pull-Out Shelf for Deep Installations

If the dock must be mounted deep in the cabinet (e.g., back wall is the only option due to plumbing conflicts), add a pull-out shelf with full-extension drawer slides. This brings the dock to the cabinet front for cartridge swaps, then pushes back for storage. The fluid lines from the dock to the house plumbing need 6-8" of slack flexible tubing to accommodate the slide travel.

### 8.3 Future Exploration: Angled Insertion

If prototype testing reveals that alignment visibility is a persistent problem (users struggling to seat the cartridge by feel), consider an angled insertion variant (30-45 degrees). This provides a partial view of the mating engagement during insertion while retaining most of the ergonomic benefits of horizontal slide-in. The top lever still works well at this angle.

---

## Sources

### Cabinet Dimensions and Under-Sink Anatomy
- [Sink Base Cabinet Dimensions Guide — Casta Cabinetry (2026)](https://castacabinetry.com/post/sink-base-cabinet-dimensions/)
- [Standard Kitchen Cabinet Size and Dimension Guide — Fabuwood](https://www.fabuwood.com/blog/standard-kitchen-cabinet-size-and-dimension-guide)
- [Kitchen Cabinet Dimensions — Builders Surplus](https://www.builderssurplus.net/kitchen-cabinet-dimensions-and-measurements-guide/)
- [Standard Kitchen Cabinet Dimensions — Family Handyman](https://www.familyhandyman.com/article/kitchen-cabinet-dimensions/)
- [Standard Kitchen Cabinet Sizes — Cabinet Select](https://cabinetselect.com/standard-kitchen-cabinet-sizes/)

### Garbage Disposal Dimensions
- [Garbage Disposal Comparison Chart — InSinkErator](https://www.insinkerator.com/en-us/insinkerator-products/garbage-disposals/disposal-comparison-chart)
- [Garbage Disposal Dimensions — Dispozal](https://dispozal.com/garbage-disposal-dimensions/)
- [What Size Garbage Disposal Do I Need — Angi](https://www.angi.com/articles/what-size-garbage-disposal-do-i-need.htm)

### Plumbing Layout and P-Trap
- [Standard Sink Drain Height — Angi](https://www.angi.com/articles/sink-drain-height.htm)
- [Determine Your P-Trap Size for Kitchen Sinks — Flex P-Trap](https://flexp-trap.com/size-kitchen-sinks)
- [Kitchen Sink Dimensions Guide — Horow](https://horow.com/blogs/guide/kitchen-sink-dimensions-guide-choosing-the-right-sink-size)

### Ergonomics and Reach Envelopes
- [Human Reach Envelope and Zone Differentiation — Yang (2009), Wiley](https://onlinelibrary.wiley.com/doi/pdf/10.1002/hfm.20135)
- [Ergonomic Reach Zones — BOSTONtec](https://www.bostontec.com/ergonomics/ergonomic-reach-zones/)
- [Reach Envelope — Cornell University DEA 651](https://ergo.human.cornell.edu/DEA6510/dea65197/reach.htm)
- [Neutral Reach Zone — BTOD](https://www.btod.com/blog/what-is-neutral-reach-zone/)
- [Postural Optimization During Functional Reach While Kneeling — PMC](https://pmc.ncbi.nlm.nih.gov/articles/PMC5011597/)

### Handle and Grip Ergonomics
- [Hand Tool Ergonomics — Tool Design — CCOHS](https://www.ccohs.ca/oshanswers/ergonomics/handtools/tooldesign.html)
- [Ergonomic Guidelines for Selecting Hand and Power Tools — EHS Today](https://www.ehstoday.com/health/article/21908634/ergonomic-guidelines-for-selecting-hand-and-power-tools)
- [Optimum Tool Handle Diameter — ScienceDirect](https://www.sciencedirect.com/science/article/abs/pii/S0894113003001601)
- [Investigation of Grip Forces and Handle Size — PMC](https://pmc.ncbi.nlm.nih.gov/articles/PMC9089462/)
- [Ergonomic Handle Design Considerations — VelocityEHS](https://www.ehs.com/blogs/ergonomic-handle-design-considerations/)
- [Ergonomic Grip Design Guide — Avient (PDF)](https://www.avient.com/sites/default/files/2021-11/avient-design-ergonomic-design-guide.pdf)

### Under-Sink Water Filter Systems
- [Waterdrop Under-Sink Ultra Instruction Manual — ManualsLib](https://www.manualslib.com/manual/2085132/Waterdrop-Under-Sink-Ultra.html)
- [3M Under Sink Reverse Osmosis Systems — Solventum](https://www.solventum.com/en-us/home/f/b5005118094/)
- [Quick Change Twist-Lock RO Filters — Home Water Purifiers](https://www.home-water-purifiers-and-filters.com/quick-change-ro.php)
- [APEC Under Sink RO Systems](https://www.apecwater.com/collections/under-sink-reverse-osmosis-ro-system)

### Under-Sink Pull-Out Organizers
- [Sink Base Pullout Organizers — Rev-A-Shelf](https://rev-a-shelf.com/kitchen/sink-base/pullout-organizers)
- [Sliding Undersink Organizer — Home Depot (HOMEIBRO)](https://www.homedepot.com/p/Sliding-Undersink-Organizer-Pull-Out-Cabinet-Shelf-Organization-and-Storage-Undersink-15x17/326721274)

### Anthropometry
- [Human Factors/Ergonomics Handbook — DOE Standard 1140](https://www.standards.doe.gov/standards-documents/1100/1140-bhdbk-2001-pt3/@@images/file)
- [Anthropometry and Biomechanics — NASA MSIS](https://msis.jsc.nasa.gov/sections/section03.htm)
- [A Checklist for Handle Design — Patkin](https://mpatkin.org/ergonomics/handle_checklist.htm)
