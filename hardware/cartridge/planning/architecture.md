# Pump Cartridge Architecture

The pump cartridge is the only user-replaceable module in the system. It contains exactly two Kamoer KPHM400 peristaltic pumps -- one per flavor line -- and nothing else. No valves, no motor drivers, no active electronics. The cartridge is a box with two pumps, four fluid ports, and a few passive electrical contact pads. All 10 solenoid valves and both L298N motor drivers live in the main enclosure body. Only 12V motor power crosses the cartridge interface via pogo pins.

---

## 1. Envelope and Pump Layout

| Parameter | Value |
|-----------|-------|
| Envelope | **148W x 130D x 80H mm** |
| Pump count | 2 |
| Pump model | Kamoer KPHM400-SW3B25 (12V DC brushed, 400ml/min) |
| Single pump dimensions | 68.6W x 115.6D x 62.7H mm |
| Combined pump width (side-by-side) | ~137.2mm |
| Cartridge mass (estimated) | ~820g (2x 306g pumps + shell) |
| Replacement interval | 18-36 months (pump tube wear) |

The two pumps sit side-by-side with motors facing the same direction (toward the rear). This is the simplest arrangement: symmetric tube routing, both pump heads face forward, electrical leads exit from the same side. The 148mm width accommodates 137.2mm of pumps with ~5.5mm margin per side for shell walls and clearance. The 130mm depth fits the 115.6mm motor axis plus mounting hardware and connector protrusion. The 80mm height clears the 62.7mm pumps with room for tubing routing above.

The envelope is constrained by the pumps, not by the available void. The triangular space below the diagonal bags is far larger than needed.

```
TOP VIEW (cartridge, lid removed)

    148mm
  <-------->
  +------------------------------------------+  ---
  |                                          |   ^
  |  +-------------+  +-------------+       |   |
  |  |   PUMP 1    |  |   PUMP 2    |       |   |
  |  |   motor ->  |  |   motor ->  |       |  130mm
  |  | <- tubes    |  | <- tubes    |       |   |
  |  +-------------+  +-------------+       |   |
  |       |                  |               |   |
  |    [tube routing to rear fluid ports]    |   v
  +------------------------------------------+  ---
       FRONT                           REAR
  (pull handle)                  (fluid connections)
```

---

## 2. Dock Position

The cartridge dock occupies the front-bottom of the enclosure, in the triangular void below the diagonal bag slab. The bags run from the top-front (sealed end pinned to back wall near Z=392) to the bottom-back (cap/connector end near the floor). This creates a large trapezoidal region at the front-bottom with over 290mm of height at the front wall and 130mm+ at mid-depth -- far more than the 80mm cartridge needs.

```
SIDE CROSS-SECTION (height vs. depth)

Z(mm)
392 +---------------------------------------------+
    |                          [sealed end pinned] |
    |   [HOPPER]                to back wall       |
    |   [ELECTRONICS]     \\\\\\\\\\\\\\\\\\\\\\\\ |
    |                 \\\\\\\\\\\\\\\\\\\\\\\\\\\\ |
    |            \\\\\\\\ bag slab (2x 2L) \\\\\\\ |
    |         \\\\\\\\ 35 deg diagonal  \\\\\\\\\\ |
    |      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ |
125 |  cap end \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  |
    |                                              |
 84 |  +----------+ +--------+                     |
    |  |CARTRIDGE | | VALVES |===tubes to bags====>|
    |  |148x130x80| | 2x5    |                     |
    |  +----------+ +--------+      [BACK PANEL]   |
  0 +---------------------------------------------+
    0 (front)      165  240              292 (back)
```

The cartridge slides in along the cabinet floor through a slot at the bottom of the front panel (0-84mm height range). The user crouches, rests the cartridge on the cabinet floor, and pushes it straight in. Gravity assists seating. The valve rack sits directly behind the cartridge dock, using the depth dimension (Y=165-240).

See `research/dock-placement.md` for the full triangle geometry analysis, ergonomic assessment, and tube routing paths.

---

## 3. Fluid Connections (Decision Open)

Four fluid connections mate at the dock back wall: 2 pump inlets and 2 pump outlets. These connect the cartridge pumps to the enclosure's valve-routed fluid paths. The valve assemblies are in the main body; the cartridge carries only the pump side.

The fitting choice is the single biggest remaining design decision for the cartridge. It determines whether the cartridge needs a release mechanism, how the user disconnects, and whether fluid drips during a swap.

### Path A: John Guest Push-to-Connect -- $8 for 4 fittings

Four JG 1/4" push-to-connect fittings mount in the dock back wall. The cartridge carries four tube stubs (1/4" OD hard nylon, ~30mm protrusion) that insert into the fittings as the cartridge slides in. Collets grip automatically.

**Release mechanism required.** A cam lever on the cartridge front face drives a push rod through the cartridge body to a release plate on the rear face. The release plate has four stepped bores (8.0/10.5/12.5mm) that engage all four JG collet rings simultaneously. Flipping the lever pushes the plate rearward ~3mm, depressing all collets, and the user pulls the cartridge out by the lever handle.

| Aspect | Detail |
|--------|--------|
| Cost | ~$8 (fittings) + ~$5-10 (cam lever, push rod, release plate) |
| Auto-shutoff | No -- open bore when disconnected, drips during swap |
| Disconnect UX | Flip lever, pull out. Not self-evident after a 2-year gap. |
| Cartridge complexity | Higher -- cam lever, push rod, release plate, dowel pins |
| Retention | JG collets provide ~20N grip (4 fittings). Lever adds rigidity. |

Research: `research/collet-release.md`, `research/cam-lever.md`, `research/release-plate.md`, `research/release-mechanism-alternatives.md`

### Path B: CPC Quick-Disconnect -- $70 for 4 pairs

Four CPC PLC NSF valved couplings mount in the dock back wall (female bodies, permanent). The cartridge carries four male inserts. Inserts engage bodies on insertion; thumb latches click. Auto-shutoff valves on both halves close when separated.

| Aspect | Detail |
|--------|--------|
| Cost | ~$70 (4 CPC pairs). Dock bodies permanent; replacement cartridges need 4 male inserts (~$6-8 each). |
| Auto-shutoff | Yes -- both sides valve closed on disconnect. Zero drips. |
| Disconnect UX | Squeeze-and-pull each CPC, or a pull handle that actuates all 4. Audible click confirms connection. |
| Cartridge complexity | Dramatically lower -- no cam lever, no push rod, no release plate. Just a box with pumps and CPC inserts. |
| Retention | CPC coupling latches provide ~15-25N positive retention. No additional mechanism needed. |
| Food safety | NSF 169 (food equipment -- strongest cert available) |
| Body OD | ~22mm (vs ~12mm for JG). Space is unconstrained in this layout. |

Research: `research/fitting-alternatives.md`

### Owner's Position

The owner prefers JG for cost. However, CPC eliminates the entire release mechanism (cam lever, push rod, release plate, dowel pins), prevents drips at the worst possible location (enclosure floor, under the bags), and simplifies the cartridge to a box with two pumps. The dock-placement research recommends CPC for the front-bottom position specifically because drips pool on the enclosure floor and auto-shutoff has high product value in this geometry.

Both paths are viable. The cartridge envelope (148W x 130D x 80H) works with either fitting type. This decision should be resolved before Phase 3 (sketching).

---

## 4. Electrical Interface

Pogo pins on the dock ceiling press onto flat pads on the cartridge top face. This separation from the fluid connections (rear face) is the primary moisture defense: water drips downward, away from the ceiling-mounted pins.

| Parameter | Value |
|-----------|-------|
| Minimum contacts | 3 (GND, Motor A 12V, Motor B 12V) |
| Optional contacts | Cartridge ID (resistor divider), temp sensor (thermistor) -- up to 6 total |
| Pad size | 8mm x 5mm each, 10mm center-to-center |
| Pad material | Nickel-plated brass (corrosion resistant, adequate conductivity) |
| Pin type | Spring-loaded pogo pins (P75 or P100 series), 2-3mm diameter, 1-2mm stroke |
| Current | ~0.85A per motor typical, ~3A stall transient |
| Wipe action | Natural -- pin tip drags across elongated pad during slide-in insertion |

The guide rails position the cartridge within ~0.5mm lateral tolerance. The oversized pads (8mm target for a 2mm pin tip) tolerate 2-3mm of misalignment, so electrical contact is effectively guaranteed.

Moisture mitigation: a drainage channel molded into the dock ceiling slopes away from pin pockets. Conformal coating protects PCB traces and solder joints on the dock side; contact surfaces remain bare metal.

See `research/electrical-mating.md` for the full connector technology survey, moisture analysis, and implementation sketch.

---

## 5. Guide and Alignment

A three-stage alignment strategy gets the cartridge from blind insertion in a dark cabinet to sub-millimeter precision at the mating face.

**Stage 1 -- Chamfered entrance (coarse capture).** A 5mm chamfer on the slot lip accepts the cartridge even with 10-15mm of initial misalignment. The funnel narrows to the rail clearance over the first 15-20mm.

**Stage 2 -- Floor rails and side guides (guided travel).** Two parallel floor rails (2mm tall, 3mm wide, full slot depth) carry the cartridge weight and provide primary depth guidance. Side wall guides (1.5mm wide rails, 0.3-0.5mm clearance per side) prevent lateral wobble. The cartridge slides ~130mm along these rails.

**Stage 3 -- Last-stage precision.** The final 10-15mm of travel constrains to ~1mm tolerance so fluid connections align. With JG fittings, tapered alignment pins at the mating face correct the last 1-2mm. With CPC fittings, the coupling geometry self-centers during engagement.

Material: PETG for all sliding surfaces (lower friction than PLA, better layer adhesion, adequate stiffness). Rail clearance: 0.3-0.5mm per side for FDM.

See `research/guide-alignment.md` for the full mechanism family survey, tolerance analysis, 3D printing clearances, and consumer product prior art.

---

## 6. Insertion and Removal Sequence

### Insertion

1. User opens the front panel (or accesses the slot directly if no panel door).
2. User rests the cartridge on the cabinet floor, roughly aimed at the slot.
3. The chamfered entrance captures the cartridge and funnels it onto the floor rails.
4. The cartridge slides ~130mm along the rails. Side guides prevent lateral wobble.
5. At full insertion depth, the four fluid connections engage:
   - **JG path:** Tube stubs push into collet fittings, collets grip automatically.
   - **CPC path:** Male inserts engage female bodies, thumb latches click, auto-shutoff valves open.
6. Pogo pins on the dock ceiling make contact with pads on the cartridge top face. Wipe action cleans the contact surfaces.
7. The user feels a clear "seated" state:
   - **JG path:** Cam lever flips to locked position.
   - **CPC path:** Audible click of 4 CPC latches.

### Removal

1. User releases the fluid connections:
   - **JG path:** Flip cam lever to release all 4 collets simultaneously via the release plate.
   - **CPC path:** Squeeze-and-pull each CPC coupling (or use a pull handle for all 4). Auto-shutoff valves close on both halves -- zero drips.
2. User slides the cartridge forward along the rails and out of the slot.
3. Pogo pins retract into the dock ceiling as the cartridge withdraws.
4. New cartridge slides in (reverse of above).

Total swap time target: under 60 seconds, one-handed, in a dark cabinet.

---

## 7. Cartridge Body Construction

The recommended construction is a tray + shell assembly (from `research/pump-mounting.md`):

- **Pump tray:** Flat PETG plate (~140 x 120 x 6mm) with heat-set M3 insert bosses for the two pump brackets. Includes printed C-clips for tubing strain relief and a wire routing channel. Prints flat for maximum screw boss strength.
- **Outer shell:** Rectangular PETG box with slide rails on the exterior, fitting pockets on the rear wall, and a recess for the pogo target PCB on the top face. The tray drops in and screws to the shell floor.
- **Lid:** Flat plate closing the open side, secured with screws or snap clips. Provides assembly access.

Pump mounting uses the Kamoer bracket (2-4x M3 holes, exact pattern to be measured from the physical pumps). Optional rubber grommet isolators on mount screws reduce vibration transmission. Internal tubing transitions from BPT pump tubes (4.8mm ID x 8.0mm OD) to 1/4" OD hard tubing via brass barb fittings.

---

## 8. Open Questions

1. **JG vs CPC fittings** -- the biggest decision. Determines whether the cartridge needs the cam lever/release plate mechanism or becomes a simple box. Affects cost ($15-20 vs $70), UX, and drip behavior.
2. **Exact pump mounting hole pattern** -- must be measured from the physical KPHM400 pumps with calipers. The Kamoer datasheet drawing was not parseable; the KK series manual gives proportional reference only.
3. **Cartridge shell material and print strategy** -- PETG is recommended but wall thickness, infill density, and print orientation need prototyping.
4. **Cartridge ID pin** -- whether firmware will support identifying cartridge type/revision via a resistor divider on a 4th pogo contact. Not needed for MVP.
5. **CPC panel-mount vs inline** -- if CPC is chosen, the exact CPC model (panel-mount body vs inline with hose barb) determines dock wall design.
6. **Vibration isolation** -- rigid mount first; add rubber grommets if noise is objectionable in practice.

---

## 9. Research Index

| File | Description |
|------|-------------|
| `research/cam-lever.md` | Cam lever mechanism design for simultaneous JG collet release. Only relevant if JG fittings are chosen. |
| `research/collet-release.md` | JG collet behavior, release force, stepped bore geometry. Only relevant if JG fittings are chosen. |
| `research/dock-placement.md` | Front-bottom triangular void geometry, envelope optimization, ergonomics, tube routing for the diagonal interleave layout. |
| `research/electrical-mating.md` | Pogo pins vs blade vs edge connector vs magnetic -- full technology survey. Recommends pogo pins + flat pads. |
| `research/fitting-alternatives.md` | Comprehensive comparison of JG, CPC, barb, Luer, bayonet, magnetic, and press-fit options. Recommends CPC; JG as fallback. |
| `research/guide-alignment.md` | Rail, dovetail, funnel, tapered pin, and kinematic coupling survey. Recommends three-stage: chamfer + rails + fine alignment. |
| `research/pump-mounting.md` | Kamoer pump mounting features, vibration isolation, screw boss design for 3D printing, tray + shell construction, tube strain relief, wire routing. |
| `research/release-mechanism-alternatives.md` | Alternative release strategies beyond the cam lever. Only relevant if JG fittings are chosen. |
| `research/release-plate.md` | Stepped bore geometry and hole arrangement for the JG collet release plate. Only relevant if JG fittings are chosen. |
