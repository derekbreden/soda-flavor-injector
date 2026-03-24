# Cartridge Envelope — Bounding Volume Design

Research and design for the overall dimensions of the replaceable pump cartridge. The cartridge holds 2 Kamoer KPHM400 peristaltic pumps, routes tubing from pump inlet/outlet ports to 4 tube stubs on the mating face, and must fit into a front-loading slot in the enclosure.

This document was originally written for an open under-sink installation. It has been rewritten to reflect the current design context: the cartridge lives inside a self-contained enclosure (~250W x 200D x 450H mm, Tall Tower layout from layout-spatial-planning.md). The pump dimension data is unchanged. The spatial constraints, arrangement rankings, and depth analysis are all new.

**Key context documents:**
- layout-spatial-planning.md — Enclosure dimensions, zone allocation, Tall Tower layout
- mating-face.md — Tube port 2x2 grid at 15mm C-C, lever placement, release plate on cartridge
- release-plate.md — Stepped bore geometry, 6mm plate thickness, guide pin layout
- cam-lever.md — Eccentric cam, 1-1.5mm eccentricity, over-center locking
- guide-alignment.md — FDM sliding clearance 0.3-0.5mm per side, tapered pin alignment
- pump-mounting.md — Kamoer mounting brackets, vibration isolation, M3 screw bosses
- electrical-mating.md — 3 pogo pins on dock, flat pads on cartridge top face
- front-face-interaction-design.md — Display holders, cartridge slot on front panel
- back-panel-and-routing.md — Dock fittings connect to valves and back panel
- hopper-and-bag-management.md — Pump-assisted filling (gravity fill does not work), bags above cartridge

---

## 1. Enclosure Context — The Slot This Cartridge Must Fit

### Enclosure Dimensions

The enclosure is a Tall Tower layout (layout-spatial-planning.md, Section 3a):

| Parameter | Value | Notes |
|-----------|-------|-------|
| Enclosure exterior | ~250W x 200D x 450H mm | 10" x 8" x 18" |
| Wall thickness | 3-4mm per side (PETG) | |
| Interior width | ~242-244mm | 250 - 2x3mm walls |
| Interior depth | ~192-194mm | 200 - 2x3mm walls |

### Zone Allocation (Top to Bottom)

```
    FRONT VIEW                    SIDE VIEW
    ┌───────────────┐            ┌──────────────┐
    │  ELECTRONICS  │ ~80mm      │  ELECTRONICS │
    │  ESP32, L298N │            │              │
    ├───────────────┤            ├──────────────┤
    │               │            │              │
    │  CARTRIDGE    │ ~130mm     │  CARTRIDGE   │
    │  DOCK ZONE    │ (inc.     │  DOCK        │
    │               │  lever)   │  + VALVES    │
    ├───────────────┤            ├──────────────┤
    │               │            │              │
    │  BAG ZONE     │ ~240mm    │  BAGS        │
    │  (2x bags,    │ (10-12"  │  (hanging or │
    │   hanging)    │  bag ht)  │   tilted)    │
    └───────────────┘            └──────────────┘
         250 W                        200 D
```

The cartridge dock sits in the middle zone. The key dimensional constraints are:

### Available Slot Dimensions

| Dimension | Available | Consumed By | Remaining for Cartridge |
|-----------|-----------|-------------|------------------------|
| **Width** | ~242mm interior | Dock walls + rail clearance (~20mm total) | ~220mm usable |
| **Height** | ~130mm zone allocation | Lever swing (~40mm above cartridge) + dock floor (~5mm) | ~85mm for cartridge body |
| **Depth** | ~192mm interior | Dock back wall + fittings + tube routing behind dock (~40-50mm) | ~142-152mm for cartridge depth |

Width is generous — the enclosure is far wider than the cartridge needs. Height is the tightest constraint. Depth is moderate — the cartridge can extend ~140-150mm into the enclosure before hitting the back-wall routing zone.

### Front-Loading Constraints

The cartridge is front-loading: it slides in from the enclosure's front face (front-face-interaction-design.md). The design implications:

1. **The cartridge front face must be flush with the enclosure front panel.** When docked, the lever, any visible trim, and the cartridge's front surface should be flush or slightly recessed relative to the enclosure's front panel. This is an aesthetic requirement — the cartridge slot should look like an integrated part of the enclosure, not a protruding afterthought.

2. **The lever is on the front face of the cartridge.** It doubles as an extraction handle. The user flips the lever to release the collets, then grips the lever to slide the cartridge out. This means the lever swings in the vertical plane on the front face, not on top of the cartridge.

3. **The insertion axis is the depth axis.** The cartridge slides along the depth axis (front-to-back). The mating face (tube stubs, alignment features) is on the back of the cartridge — the face that enters the dock first and mates with the John Guest fittings in the dock's back wall.

4. **The dock back wall holds the John Guest fittings, alignment pins, and pogo pins.** Tubes route from these fittings rearward to solenoid valves and then to the back panel.

### Coordinate System

For all dimensions in this document:
- **Width (W)**: left-right as viewed from the front of the enclosure
- **Height (H)**: top-bottom (gravity direction)
- **Depth (D)**: front-to-back (insertion axis; mating face at the back of the cartridge, front face with lever at the front)

---

## 2. Pump Dimensions — Kamoer KPHM400

### Source Data

The pump is the **Kamoer KPHM400-SW3B25**: 12V DC brushed motor, 3 rollers, 400ml/min flow rate, BPT tubing (4.8mm ID x 8.0mm OD). Dimensional data is from the official Kamoer KPHM400 datasheet and Amazon listing.

Model code breakdown: KPHM400-**SW**3**B25** — SW = 12V brushed motor, 3 = 3 rollers, B25 = BPT tube 4.8x8mm.

### Overall Dimensions

| Dimension | Value | Notes |
|-----------|-------|-------|
| Overall size (L x W x H) | 115.6 x 68.6 x 62.7 mm | Per Kamoer product page (verified) |
| Weight | ~306 g | Per Kamoer specs (verified) |
| Motor type | 12V DC brushed | Model suffix SW |
| Power | 10W | Verified |

Note: The overall length of 115.6mm includes the motor body extending behind the pump head. The pump head itself is approximately 16mm deep; the motor body extends ~52.7mm behind it, and the mounting bracket extends further behind the motor.

### Mounting Hole Pattern

From pump-mounting.md: the pump uses M3 screw holes on a mounting bracket. The exact hole pattern must be measured from the physical pump (inferred spacing ~55-65mm x 40-50mm based on KK series proportional scaling). The mounting plate is perpendicular to the motor axis.

### Inlet/Outlet Port Positions

The inlet and outlet barb fittings protrude from the **top of the pump head**:

| Parameter | Value |
|-----------|-------|
| Port orientation | Vertical (upward from pump head top face) |
| Port spacing (center-to-center) | ~20-25 mm (estimated) |
| Barb OD | ~8 mm (matches BPT tubing) |
| Barb protrusion above pump body | ~15-20 mm |
| Position along length axis | Both near the front face of pump head |

The ports exit perpendicular to the motor axis (pointing up when mounted motor-horizontal). Tubing must route from these vertical barbs to the mating face at the back of the cartridge.

### Key Takeaway

Each pump occupies approximately a **116 x 69 x 63 mm** box (L x W x H), plus ~20mm above for barb fittings. Two pumps side by side are approximately 138mm wide; stacked they are approximately 126mm tall.

---

## 3. Pump Arrangements — Re-evaluated for the Enclosure Slot

The original analysis evaluated five arrangements for an open under-sink installation. The enclosure changes the constraints fundamentally:

- **Width is no longer the limiting factor.** The enclosure interior is ~242mm wide — more than enough for any arrangement.
- **Height is now the tightest constraint.** The cartridge zone is ~85mm tall (after lever clearance). The pump is 62.7mm tall, leaving only ~22mm for housing walls and barb clearance.
- **Depth is moderate but limited.** ~142-152mm available. The pump length of 115.6mm (motor axis) consumes most of this.
- **The lever is on the front face**, not the top. This changes how lever clearance interacts with the cartridge height.

### Motor Axis Orientation

A critical question for each arrangement: which direction does the motor axis point?

**Motor axis along the depth axis (front-to-back):** The 115.6mm pump length goes into the enclosure. The pump head faces rearward (toward the mating face and dock fittings). This is the most natural orientation — pump heads near the mating face means shorter tube runs from pump barbs to tube stubs.

**Motor axis along the width axis (left-right):** The 115.6mm pump length goes across the enclosure. Pump heads face sideways. Tube routing from the sideways-facing barbs to the rear mating face is longer and more complex.

**Motor axis along the height axis (top-bottom):** The pump stands vertically. The 115.6mm length goes up-down. This consumes height, which is the scarcest dimension.

For all arrangements below, motor axis along the depth axis is assumed unless stated otherwise.

### Arrangement A: Side by Side, Motors Forward

Both pumps mounted with motor axes parallel to the insertion axis, side by side horizontally. Pump heads face rearward (toward mating face).

```
    FRONT VIEW (looking at the front face / lever side)

    ┌─────────────────────────────────┐
    │    ↑barbs ↑      ↑barbs ↑      │
    │    Pump 1        Pump 2        │
    │    [motor]       [motor]       │
    │    [head→]       [head→]       │
    └─────────────────────────────────┘

    TOP VIEW (looking down)

    ┌─────────────────────────────────┐
    │  [motor 1 ──► head]  [motor 2 ──► head]  │
    └─────────────────────────────────┘
    ▲ front face                    mating face ▲
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~138 mm | 2 x 69mm pump width |
| Height | ~83 mm | 63mm pump + 20mm barb clearance |
| Depth | ~116 mm | Single pump length (motor axis) |

**Fit in enclosure slot:**
- Width (138mm) vs available (220mm): fits easily, 82mm to spare
- Height (83mm) vs available (85mm): extremely tight — only 2mm margin. Housing walls would push this over.
- Depth (116mm) vs available (142-152mm): fits, 26-36mm to spare

**Port positions:** All 4 barbs exit from the top. Tubes bend rearward to the mating face. Short routing distance since pump heads are near the back.

**Verdict:** Height is the problem. The 83mm raw dimension (63mm pump + 20mm barbs) leaves no room for housing walls (need 6mm minimum for top + bottom walls). Actual envelope would be ~89mm, exceeding the ~85mm budget. The barb clearance might be reducible if tubes route immediately sideways rather than straight up, but this is very tight.

### Arrangement B: Stacked Vertically

Both pumps stacked one above the other, motor axes along depth.

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~69 mm | Single pump width |
| Height | ~146 mm | 2 x 63mm + 20mm barb clearance |
| Depth | ~116 mm | Single pump length |

**Fit in enclosure slot:**
- Height (146mm) vs available (85mm): **does not fit.** Far too tall.

**Verdict:** Eliminated. The cartridge zone height cannot accommodate two vertically stacked pumps.

### Arrangement C: Head-to-Tail (Motors Opposing)

Same footprint as Arrangement A with motors pointing in opposite directions. One pump has its head at the front, the other at the back.

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~138 mm | Same as A |
| Height | ~83 mm | Same as A |
| Depth | ~116 mm | Same as A |

Same height problem as Arrangement A. Additionally, one pump's head faces away from the mating face, requiring longer tube routing.

**Verdict:** Same height issue as A, plus worse tube routing. No advantage.

### Arrangement D: Perpendicular / L-Shaped

One pump rotated 90 degrees relative to the other.

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~185 mm | 116mm (pump 1 length) + 69mm (pump 2 width) |
| Height | ~83 mm | Still barb-limited |
| Depth | ~185 mm | 116mm (pump 2 length) + 69mm (pump 1 width) |

**Fit in enclosure slot:**
- Depth (185mm) vs available (142-152mm): **does not fit.**

**Verdict:** Eliminated. Too deep and too wide (though width isn't the binding constraint, the depth is).

### Arrangement E: Inline (Motors End-to-End)

Both pumps in a line along the depth axis, one behind the other.

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~69 mm | Single pump width |
| Height | ~83 mm | 63mm + 20mm barb clearance |
| Depth | ~237 mm | 2 x 116mm pump length + 5mm gap |

**Fit in enclosure slot:**
- Depth (237mm) vs available (142-152mm): **does not fit.** Nearly double the available depth.

**Verdict:** Eliminated. Far too deep.

### Arrangement F: Side by Side, Motors Sideways (NEW)

Both pumps side by side, but with motor axes pointing left-right (across the width of the enclosure) instead of front-to-back. Pump heads face inward or outward.

```
    TOP VIEW (looking down)

    ┌─────────────────────────────────────────┐
    │                                         │
    │   [head ◄── motor 1]  [motor 2 ──► head]│
    │                                         │
    └─────────────────────────────────────────┘
    ▲ front face                  mating face ▲
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~232 mm | 2 x 116mm pump length |
| Height | ~83 mm | 63mm + 20mm barb clearance |
| Depth | ~69 mm | Single pump width (now along depth axis) |

**Fit in enclosure slot:**
- Width (232mm) vs available (220mm): marginally too wide. Could possibly fit with tight packing (pumps at 115.6mm each = 231.2mm, barely over 220mm usable).
- Height (83mm) vs available (85mm): same tight fit as Arrangement A.
- Depth (69mm) vs available (142-152mm): very shallow, leaves 73-83mm of spare depth.

**Verdict:** Width is marginal (10-12mm over). Height remains tight. The very shallow depth is attractive. If the pumps can be packed with minimal wall thickness along the width axis, this could work — but 232mm in a 220mm usable space requires trimming walls to near zero on the sides, which isn't structural. More critically, the barbs point upward from a pump head that now faces sideways, making tube routing to the rear mating face significantly more complex.

### Arrangement G: Side by Side, Motors Pointing Rearward, Pumps Dropped Low (REFINED A)

This is Arrangement A re-examined with the height constraint addressed directly. The barb clearance of 20mm assumed tubes going straight up. If the BPT tubing from the barbs is immediately bent sideways or rearward (toward the mating face) using its 15-20mm bend radius, the barbs don't need 20mm of headroom — they need only enough for the tube to exit and begin its bend, perhaps 10-12mm.

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~138 mm | 2 x 69mm pump width |
| Height | ~76 mm | 63mm pump + 10mm reduced barb routing + 3mm top/bottom walls |
| Depth | ~122 mm | 116mm pump + 6mm walls |

With silicone tubing for the internal routing (confirmed: most internal tubing can be silicone, only transition points need hard tubing), the bend radius at the barbs drops to 10-15mm. The tubes exit the barbs, immediately curve rearward and downward, routing along the sides of the pump bodies to reach the mating face at the back.

**Fit in enclosure slot:**
- Width (138mm) vs available (220mm): comfortable
- Height (76mm) vs available (85mm): **fits with 9mm margin**
- Depth (122mm) vs available (142-152mm): fits with 20-30mm margin

**This is the viable version of side-by-side.** The key insight is that silicone tubing's tight bend radius eliminates the need for 20mm of vertical barb clearance. 10mm above the pump body is sufficient for the tubes to begin their rearward bend.

### Arrangement H: Staggered Side by Side (Motors Rearward, Offset)

One pump shifted rearward relative to the other, so the pump heads are offset along the depth axis. This creates a staircase pattern.

```
    TOP VIEW (looking down)

    ┌─────────────────────────────────────────┐
    │                                         │
    │  [motor 1 ──────► head]                 │
    │                                         │
    │       [motor 2 ──────► head]            │
    │                                         │
    └─────────────────────────────────────────┘
    ▲ front face                  mating face ▲
```

| Dimension | Value | Notes |
|-----------|-------|-------|
| Width | ~138 mm | Same as side-by-side |
| Height | ~76 mm | Same as refined A |
| Depth | ~152 mm | 116mm + 30mm offset + 6mm walls |

The stagger adds depth but gains nothing — the pumps still need the same height and width. The tube routing is actually worse because one pump's barbs are further from the mating face.

**Verdict:** Eliminated. Deeper with no benefit.

---

## 4. The Lever on the Front Face

The lever is on the front face of the cartridge, not the top (mating-face.md, Section 4). It pivots in the vertical plane — the handle swings upward to release, downward to lock. This changes the clearance analysis compared to the original document's top-mounted lever assumption.

### Lever Clearance

When the lever is closed (locked), it sits flush against the cartridge front face — contributing essentially zero to the cartridge's height or width. When open (released), the lever handle swings upward approximately 90-120 degrees from the cartridge face.

The lever handle length is 50-80mm (cam-lever.md). When swung upward, it extends vertically above the cartridge. The ~40mm of lever swing clearance allocated in the zone budget is above the cartridge slot opening, not above the cartridge body itself.

**Impact on cartridge envelope:** The lever adds ~10mm to the cartridge's front-face depth (the cam body and pivot housing protrude from the front wall). This is internal to the cartridge — it does not add to the insertion depth. The lever handle, when closed, may extend ~5-10mm below the cartridge's bottom edge as a grip tab for extraction.

### Flush Front Face Requirement

When docked, the cartridge front face must be flush with the enclosure's front panel. This means:

1. The cartridge body terminates at the enclosure's front panel plane.
2. The lever is recessed into or flush with the front panel.
3. No part of the cartridge protrudes forward of the enclosure's front surface.

The dock rails must position the cartridge so its front face aligns precisely with the enclosure opening. A small recessed bezel (1-2mm setback) around the cartridge slot provides visual framing and accommodates tolerance.

---

## 5. Tubing Routing Constraints

### Tubing Specifications

| Tubing | ID | OD | Material | Use |
|--------|----|----|----------|-----|
| Pump internal | 4.8 mm | 8.0 mm | BPT (PharMed) | Inside pump head, pre-installed |
| Internal cartridge routing | 4.8 mm | 8.0 mm | Silicone | Pump barbs to transition fittings |
| Mating face stubs | 1/8" (3.2 mm) | 1/4" (6.35 mm) | Hard tubing (nylon/polyethylene) | Push-connect compatibility |

Most internal tubing is silicone — confirmed viable. Only the tube stubs at the mating face need hard 1/4" OD tubing for reliable John Guest push-connect engagement. A few barb reducer fittings at transition points bridge the silicone tubing to hard stubs.

### Minimum Bend Radius

**Soft silicone tubing (4.8mm ID x 8mm OD):**

| Source | Minimum Bend Radius |
|--------|---------------------|
| Conservative (2-2.5x OD) | 16-20 mm |
| Practical (thick-wall soft silicone) | 10-15 mm |

**Hard tubing (1/4" OD nylon):**

| Source | Minimum Bend Radius |
|--------|---------------------|
| Cold bend | 30-40 mm |
| Heat-formed | 15-20 mm |

### Recommended Routing Strategy

Use **silicone tubing** for all internal routing from pump barbs through bends. Transition to **hard 1/4" OD tube stubs** only for the last ~20-25mm that protrude through the mating face. This keeps bend radii at 10-15mm and the cartridge compact.

With silicone tubing at 10-15mm bend radius, the 90-degree turn from the pump barbs (vertical) to the mating face (horizontal, rearward) consumes approximately 10-15mm of vertical space above the pump body and 10-15mm of horizontal space behind the pump head. This is significantly tighter than the 20mm each assumed in the original analysis.

### Tube Stub Layout on Mating Face

From mating-face.md: the recommended arrangement is a **2x2 grid at 15mm center-to-center spacing**. This has been verified with parts in hand — 15mm C-C works with John Guest fittings. The tube stub zone occupies approximately **28 x 28 mm** on the mating face (fitting body clearance included at minimum).

```
    Pump 1 IN    Pump 2 IN
        O            O          15mm C-C
    Pump 1 OUT   Pump 2 OUT
        O            O
```

The 2x2 grid's compact footprint (33.5 x 33.5mm including release plate margins) fits easily on the back face of any of the viable cartridge arrangements.

---

## 6. Minimum Envelope Estimates

For each viable arrangement, we add:
- **Tube routing space**: +10mm above pump for silicone bends (reduced from 20mm)
- **Housing walls**: 3mm per side (PETG, structural minimum for FDM)
- **Mating face hardware**: tube stub protrusion is outside the cartridge envelope (extends into the dock)
- **Lever mechanism**: +10mm on the front face for cam housing (internal to cartridge depth)

### Arrangement G: Side by Side, Refined (RECOMMENDED)

```
    Width  = 2 x 69 (pumps) + 5 (center gap) + 2 x 3 (walls) = 149 mm
    Height = 63 (pump) + 10 (tube routing) + 2 x 3 (walls) = 79 mm
    Depth  = 116 (pump) + 10 (cam housing at front) + 2 x 3 (walls) = 135 mm
```

**Envelope: ~149 x 79 x 135 mm (W x H x D)**

| Constraint | Envelope | Available | Margin |
|-----------|----------|-----------|--------|
| Width | 149mm | 220mm | +71mm (comfortable) |
| Height | 79mm | 85mm | +6mm (adequate) |
| Depth | 135mm | 142-152mm | +7-17mm (tight but workable) |

This fits the enclosure slot. The height margin of 6mm is the tightest dimension — enough for guide rail clearance (0.3-0.5mm per side from guide-alignment.md) but not generous. If the pump's 62.7mm height proves larger than spec in practice, there is limited room to absorb it.

### Arrangement A: Side by Side, Original Spacing

If the center gap is eliminated and walls are reduced to minimum:

```
    Width  = 2 x 69 (pumps) + 2 (minimal center gap) + 2 x 3 (walls) = 146 mm
    Height = 63 (pump) + 10 (tube routing) + 2 x 3 (walls) = 79 mm
    Depth  = 116 (pump) + 2 x 3 (walls) = 122 mm (no cam housing if lever is flush-mounted)
```

**Envelope: ~146 x 79 x 122 mm** — slightly narrower and shallower than G, but requires the lever/cam to be truly flush with the front wall.

### Arrangement F: Motors Sideways (MARGINAL)

```
    Width  = 2 x 116 (pumps) + 2 x 3 (walls) = 238 mm
    Height = 63 (pump) + 10 (tube routing) + 2 x 3 (walls) = 79 mm
    Depth  = 69 (pump width) + 2 x 3 (walls) = 75 mm
```

**Envelope: ~238 x 79 x 75 mm** — 18mm wider than the 220mm available. Does not fit without eliminating housing walls on the sides entirely, which is not structural. The very shallow depth (75mm) is appealing but the width is the dealbreaker.

### Comparison Table

| Arrangement | W (mm) | H (mm) | D (mm) | Volume (L) | Fits Slot? | Notes |
|-------------|--------|--------|--------|------------|------------|-------|
| **G: Side by Side (refined)** | 149 | 79 | 135 | **1.59** | **Yes** | Recommended. 6mm height margin. |
| A: Side by Side (original) | 146 | 79 | 122 | **1.41** | **Yes** | Tighter if cam is truly flush. |
| F: Motors Sideways | 238 | 79 | 75 | **1.42** | **No** | 18mm too wide. |
| B: Stacked | 69 | 146+ | 122 | — | **No** | 60mm too tall. |
| E: Inline | 69 | 79 | 237+ | — | **No** | 85mm+ too deep. |
| D: Perpendicular | 185 | 79 | 185 | — | **No** | Too deep. |

Only the side-by-side arrangements (G and A) fit the enclosure slot. The enclosure's height constraint eliminates stacking, and the depth constraint eliminates inline and perpendicular layouts.

---

## 7. Depth Budget — What Lives Behind the Cartridge

The enclosure interior depth is ~192mm. The cartridge consumes 122-135mm of this. What fills the remaining space behind the cartridge?

```
    ← FRONT                                           BACK →

    ┌──────────┬──────────────────────────────┬──────────────────┐
    │ cartridge│        dock back wall        │   tube routing   │
    │  lever   │        + fittings            │   to valves &    │
    │  (flush) │  4x JG fittings: ~25mm deep │   back panel     │
    │          │  alignment pins: ~15mm       │   ~20-30mm       │
    │          │  pogo pins: ~10mm            │                  │
    │          │  back wall: ~5mm             │                  │
    └──────────┴──────────────────────────────┴──────────────────┘
     135mm cart.   ~35mm dock back wall          ~22mm routing
                                                 ─────────────
                                                 Total: ~192mm
```

| Zone | Depth | Notes |
|------|-------|-------|
| Cartridge body (including cam housing) | 135mm | Arrangement G |
| Dock back wall + fittings | ~35mm | JG fitting depth + wall material + alignment pin protrusion |
| Tube routing behind dock to valves | ~20-25mm | Silicone tubing bends from fittings to downward runs |
| **Total** | **~190-195mm** | Tight fit for 192mm interior |

This is tight. Options to recover depth:
1. Reduce cartridge cam housing depth from 10mm to 5mm (the cam itself needs only ~5mm of protrusion).
2. Use compact right-angle barb fittings behind the dock wall to minimize tube routing depth.
3. Accept that the depth budget is fully consumed and there is no spare room behind the dock.

With the cam housing reduced to 5mm: cartridge depth drops to 130mm, freeing 5mm for tube routing. The total becomes ~185-190mm, fitting within the 192mm interior.

**Recommended cartridge depth: 130mm** (with 5mm cam housing, tight packaging).

---

## 8. Cartridge Front Face Design

The front face is what the user sees and interacts with. It must accommodate:

1. **Lever handle** — pivots vertically, ~60-80mm long, acts as both release mechanism and extraction handle
2. **Visual indicator** — some way to tell if the lever is locked (cam over-center) or released
3. **Aesthetic surface** — flush with enclosure front panel when docked

```
    FRONT VIEW (user-facing side of cartridge)

    ┌──────────────────────────────────────────────────────────┐
    │                                                          │
    │                                                          │
    │              ● pivot                                     │
    │              ╠══════════════════╗                        │
    │              ║  LEVER HANDLE   ║                        │
    │              ╚══════════════════╝                        │
    │                                                          │
    │                                                          │
    │                                                          │
    └──────────────────────────────────────────────────────────┘
              149mm wide x 79mm tall

    When released, the lever swings upward ~100 degrees
    from flush to vertical, then the cartridge slides out.
    The lever handle also serves as the pull grip.
```

The lever pivot is near the top of the front face. In the locked position, the handle lies flat against the face, pointing downward. To release: flip the handle upward (unlocking the over-center cam), then grip the handle and pull the cartridge forward out of the dock.

---

## 9. Weight Estimate

| Component | Weight | Qty | Subtotal |
|-----------|--------|-----|----------|
| Kamoer KPHM400 pump | ~306 g | 2 | 612 g |
| Silicone tubing (internal routing, ~400mm total) | ~15 g | 1 | 15 g |
| Hard tube stubs (4x, ~30mm each) | ~2 g | 4 | 8 g |
| 3D printed PETG housing (3mm walls, ~149x79x130 shell) | ~80-120 g | 1 | ~100 g |
| Barb reducer fittings | ~5 g | 4 | 20 g |
| Brass electrical pads (3x) | ~5 g | 3 | 15 g |
| Release plate (PETG, ~34x34x6mm) | ~10 g | 1 | 10 g |
| Cam mechanism + lever (PETG + steel pin) | ~15 g | 1 | 15 g |
| Guide dowel pins (4x 3mm steel) | ~3 g | 4 | 12 g |
| **Total estimated** | | | **~807 g (~1.8 lbs)** |

The weight reduction from 940g (original estimate) to 807g reflects the corrected pump weight (306g per Kamoer specs vs. 380g from the Amazon listing used originally) and a smaller housing shell.

### Handling Assessment

- 807g / 1.8 lbs is easily one-handed
- The center of gravity is dominated by the two pumps (76% of total weight), distributed symmetrically left-right in the side-by-side arrangement
- The lever handle provides a natural grip point on the front face
- The weight is comparable to a medium hardcover book

---

## 10. Interdependencies with Other Components

### Release Plate (from release-plate.md)

The release plate is part of the cartridge. It slides along the tube stubs on the cartridge's rear (mating) face, driven by the cam mechanism. Key dimensions:
- Plate footprint: ~34 x 34mm (for 2x2 grid at 15mm C-C with margins)
- Plate thickness: 6mm
- Plate travel: 3mm
- Guide pins: 4x 3mm steel dowels press-fit into cartridge body wall

The plate sits within the cartridge's rear wall zone and does not add to the external envelope.

### Cam/Lever Mechanism (from cam-lever.md)

The eccentric cam with 1-1.5mm eccentricity provides 2-3mm of plate displacement. The lever pivots on the cartridge's front face. A push rod or yoke transmits the cam's output from the front (where the lever is) to the rear (where the release plate is), running through the interior of the cartridge body alongside the pumps.

The push rod length is approximately the cartridge depth minus the front and rear wall thicknesses: ~130 - 6 - 6 = ~118mm. This rod must be straight and rigid. A 3mm steel rod or a printed PETG rod with 4-5mm diameter would work.

### Guide Rails (from guide-alignment.md)

FDM tolerance: 0.3-0.5mm clearance per side. The cartridge slides into the dock on rails that run the full depth. Rail channels on the cartridge sides add ~3-5mm to the cartridge width (included in the wall allowance). The 6mm height margin accommodates 0.5mm clearance per side for the rails plus a small structural lip.

### Electrical Contacts (from electrical-mating.md)

3 pogo pins in the dock press against 3 flat brass pads on the cartridge's top face. The pads are flush-mounted, adding negligible height. Their position on the top face (not the mating face) keeps electrical contacts separated from water fittings for moisture isolation.

### Bags and Gravity Feed (from hopper-and-bag-management.md)

Bags hang in the zone below the cartridge dock. The bag zone is ~240mm tall (10-12" of realistic bag height). Bags collapse reliably with the inlet cap rigidly secured — they don't need to be full 2L Platypus bags; smaller bags work in the available space.

The bag outlets connect via silicone tubing upward to the dock's John Guest fittings (pump inlet side). The elevation difference between bag outlets and the dock provides gravity priming for the pump inlets.

Hopper filling is pump-assisted — gravity fill from the hopper to bags does not work. The peristaltic pumps run in reverse to pull concentrate from the hopper funnel into the bags.

### Solenoid Valves and Flow Path

Solenoid valves mount in the dock zone or just below it. Tubing routes from the dock's John Guest fittings (pump outlet side) downward to the solenoid valves, then to the flow meter and out the back panel. The tube routing behind the dock consumes ~20-25mm of depth, accounted for in the depth budget (Section 7).

### Capacitive Sensing (from project context)

FDC1004 capacitive sensing is confirmed working through BPT and silicone tubing for liquid/air detection. This does not affect the cartridge envelope — the sensing electrodes are on the external tubing outside the cartridge.

### GPIO Exhaustion (from project context)

GPIO exhaustion is solved by I2C expander — this is a routine firmware task that does not affect the cartridge's physical design.

---

## 11. Scenario Ranking and Recommendation

### Scoring (1-5, 5 = best)

| Criteria | Weight | G: Side-by-Side (refined) | B: Stacked | E: Inline | F: Motors Sideways |
|----------|--------|---------------------------|------------|-----------|-------------------|
| Fits enclosure slot | 5 | 5 | 1 | 1 | 2 |
| Compact volume | 3 | 4 | — | — | 4 |
| Tube routing simplicity | 4 | 5 | — | — | 2 |
| One-hand ergonomics | 3 | 5 | — | — | 3 |
| Symmetric weight | 2 | 5 | — | — | 5 |
| Depth margin | 3 | 3 | — | — | 5 |
| Height margin | 4 | 3 | — | — | 3 |
| Lever integration | 3 | 5 | — | — | 4 |
| **Weighted Score** | | **119** | — | — | — |

Arrangements B, D, and E are eliminated (do not fit). Arrangement F is marginal (does not fit width). Only the side-by-side family (A/G) is viable for this enclosure.

### Recommendation

**Side-by-side with motors along the depth axis (Arrangement G), target dimensions:**

```
    ┌───────────────────────────────────────────────────────┐
    │                                                       │
    │                149 mm (5.9")                          │
    │   ┌───────────────────────────────────────────────┐   │
    │   │                                               │   │ 79 mm
    │   │     Pump 1              Pump 2                │   │ (3.1")
    │   │     [════►]             [════►]               │   │
    │   │                                               │   │
    │   └───────────────────────────────────────────────┘   │
    │                130 mm (5.1") depth                     │
    └───────────────────────────────────────────────────────┘

    Weight: ~807g (1.8 lbs)
    Volume: ~1.53 L
```

**Target envelope: 150 x 80 x 130 mm (W x H x D)** — round numbers that provide 1-3mm of margin on each dimension beyond the calculated minimums.

### Why Side-by-Side Remains the Winner

The original analysis ranked side-by-side first for open under-sink use. The enclosure context reinforces this conclusion even more strongly: the enclosure's height constraint (~85mm) eliminates every arrangement except side-by-side. The enclosure's generous width (~220mm usable) accommodates the 149mm cartridge width without issue. The depth is the second-tightest constraint, and side-by-side's 130mm depth leaves just enough room for the dock back wall and tube routing.

The ranking didn't change because a different arrangement became better — it changed because the enclosure constraints eliminated all alternatives. Side-by-side is not just the best option; it is the only option that physically fits.

---

## Sources

- [Kamoer KPHM400 Datasheet / Product Manual (Amazon PDF)](https://m.media-amazon.com/images/I/A1at7U9PyNL.pdf) — Dimensional drawing, mounting hole pattern, tube specifications
- [Kamoer KPHM400 Amazon Listing (B09MS6C91D)](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D) — Product specifications
- [Kamoer KPHM400 Official Product Page](https://www.kamoer.com/us/product/detail.html?id=10014) — Verified dimensions: 115.6 x 68.6 x 62.7mm, 306g, 10W
- [KPHM400 Data Sheet — DirectIndustry](https://pdf.directindustry.com/pdf/kamoer-fluid-tech-shanghai-co-ltd/kphm400-peristaltic-pump-data-sheet/242598-1017430.html) — Technical specifications
- [Silicone Tubing Bend Radius — Zeus Inc.](https://www.zeusinc.com/resources/summary-material-properties/bend-radius/) — Bend radius engineering data
