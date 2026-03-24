# Back Panel Design and Internal Routing

Research document covering the back panel layout, internal fluid routing, electrical routing, cartridge dock integration, display cable routing, thermal management, and internal layout strategy for the soda flavor injector enclosure.

The enclosure sits under a kitchen sink, pushed against or near the back wall of the cabinet. The back panel faces the cabinet wall and carries all "set and forget" connections — the user connects everything during installation and never touches the back again.

---

## 1. Back Panel Layout

### 1a. Connection Inventory

Six connection types cross the back panel:

| # | Connection | Fitting Type | Mounting Hole | Protrusion (outside) | Notes |
|---|-----------|-------------|---------------|---------------------|-------|
| 1 | Tap water inlet | John Guest PP1208W bulkhead, 1/4" | 5/8" (15.9mm) | ~30-40mm (fitting body + inserted tube) | Clean cycle supply |
| 2 | Carbonated water inlet | John Guest PP1208W bulkhead, 1/4" | 5/8" (15.9mm) | ~30-40mm | Main water stream |
| 3 | Carbonated water outlet | John Guest PP1208W bulkhead, 1/4" | 5/8" (15.9mm) | ~30-40mm | To faucet |
| 4 | 12V DC power | 5.5x2.1mm barrel jack, panel mount | 11mm (7/16") | ~15mm (plug body) | External 12V 2A+ PSU |
| 5 | Air switch tube | Rubber grommet, 3-4mm ID | 6-8mm | Flush (tube passes through) | Pneumatic line to countertop |
| 6 | USB port (optional) | Panel-mount Micro-B extension | 18x8mm rectangular cutout | ~10mm (plug body) | Firmware updates, rubber plug when unused |

### 1b. Connection Arrangement

Water connections go at the **bottom** of the back panel. Rationale:

- Any drip or condensation at push-connect fittings drips downward and away from electronics
- Water lines route downward from the carbonator (which is typically at the same height or higher), so bottom placement creates a natural downward entry
- Power and signal connections go at the **top**, away from any potential moisture
- Physical separation between wet and dry zones

Proposed layout (viewed from outside, looking at the back panel):

```
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │   [USB]                                [12V DC]     │  <- TOP ZONE (dry)
    │                                                     │
    │                          [AIR]                      │  <- MID (air switch grommet)
    │                                                     │
    │  ─────────────────────────────────────────────────  │  <- visual separator / drip dam
    │                                                     │
    │   [TAP IN]        [CARB IN]        [CARB OUT]       │  <- BOTTOM ZONE (wet)
    │    (blue)          (green)          (white)         │
    │                                                     │
    └─────────────────────────────────────────────────────┘
```

**Spacing between push-connect fittings:** The John Guest PP1208W bulkhead body is approximately 20mm across the hex nut. The user needs finger access to push the tube in (push the collet ring to release). Minimum center-to-center spacing: **35-40mm**. At 40mm spacing across three fittings, the water zone spans approximately **80mm (3.1") + margins**.

**Color coding and labeling:** Color alone is insufficient — the user is reaching behind the enclosure in a dark cabinet. Multiple identification methods, all present simultaneously:

| Fitting | Color Ring | Embossed Label | Tube Color |
|---------|-----------|----------------|------------|
| Tap water inlet | Blue | TAP IN | Blue tube (matches ice maker kit convention) |
| Carbonated water inlet | Green | CARB IN | Clear/natural tube |
| Carbonated water outlet | White/Red | TO FAUCET | Clear/natural tube |

Labels should be **embossed or debossed** into the 3D printed panel surface, not stickers (which peel in humid under-sink environments). Raised lettering can be felt by touch in the dark. Color rings can be printed snap-on collars around the bulkhead fitting nut.

Additionally: print or label matching tags at the carbonator end of each tube, so the user can verify the connection without tracing the tube visually.

### 1c. Panel Dimensions and Access Clearance

The back panel dimensions are determined by the enclosure. Based on the cartridge envelope research (140 x 90 x 100mm cartridge, plus dock, bags, electronics), the enclosure is roughly desktop-PC-tower-sized — estimated at approximately **250mm wide x 350mm tall x 250mm deep** (10" x 14" x 10"). The back panel is therefore approximately **250 x 350mm**.

**Connection zone allocation:**

```
    ┌───────────── 250mm (10") ────────────────┐
    │                                           │
    │  Top zone: 40mm tall                      │  USB + power
    │                                           │
    │  Mid zone: 30mm tall                      │  Air switch
    │                                           │
    │  Separator: 10mm                          │  Drip dam ridge
    │                                           │
    │  Bottom zone: 60mm tall                   │  3 water fittings
    │                                           │
    │  Bottom margin: 20mm                      │  Panel edge clearance
    │                                           │
    └───────────────────────────────────────────┘
```

The connection zones occupy approximately the **bottom 160mm** of the 350mm panel, leaving the top ~190mm as blank panel (structural, no penetrations).

**Access clearance:** The user reaches behind the enclosure to connect tubes. They need:

- **Vertical clearance above the fittings:** ~50mm minimum for fingers to push tubes in
- **Depth behind the panel:** ~80-100mm for the tube to insert into the fitting plus a gentle bend
- **Lateral clearance between fittings:** ~20mm clear around each fitting body

The enclosure should be positioned with at least **75-100mm (3-4 inches)** between the back panel and the cabinet wall. This accommodates:
- 30-40mm fitting protrusion
- 20mm tube insertion depth
- 20-30mm tube bend clearance before it routes away

### 1d. Panel Construction

**Recommended: Removable back panel, screw-mounted.**

The back panel should be a separate piece that attaches to the enclosure body with 4-6 screws (M3 or M4, threading into heat-set inserts in the enclosure body). Reasons:

1. **Internal access:** Removing the back panel exposes all internal plumbing and wiring for maintenance, without disconnecting external tubes
2. **Fitting installation:** Bulkhead fittings are installed from inside — the fitting body goes through the hole from inside, the hex nut tightens from outside. Much easier to assemble with the panel detached
3. **Replacement:** If a fitting hole wears or cracks, replace just the panel (small, cheap print)
4. **Print quality:** A flat panel prints better than an integrated panel on a large enclosure body

**Panel thickness:** 4mm minimum. The bulkhead fitting nut clamps against this thickness. Too thin and the nut bottoms out; too thick and the fitting body doesn't protrude enough to accept tubing on the outside.

**Sealing:** An O-ring or gasket between the panel and enclosure body is unnecessary for this application — the enclosure is not waterproof (it has ventilation openings). The panel-to-body joint is purely structural. However, a slight raised lip or channel around the water fitting zone can contain any drips and route them to the bottom edge.

**Bulkhead fitting installation detail:**

```
    CROSS SECTION — bulkhead fitting through back panel

    INSIDE                    PANEL (4mm)              OUTSIDE
    ────────────────────────┬─────────┬────────────────────────
                            │         │
    ○ tube from internal    │  ┌───┐  │    ○ tube to carbonator
    plumbing pushes in  ←── │  │   │  │ ──→ pushes in from outside
                            │  │fit│  │
                 collet ──> │  │ting│  │ <── collet
                            │  │body│  │
                            │  └───┘  │
    ────────────────────────┤  hex    ├────────────────────────
                            │  nut    │
                            └─────────┘
                              clamps on outside
```

### 1e. Strain Relief and Tube Management

**The problem:** After connecting all tubes, the user pushes the enclosure back toward the cabinet wall. If tubes exit the back panel perpendicular to the panel surface, they immediately hit the cabinet wall and kink at the fitting.

**Solution: 90-degree elbows on the outside of bulkhead fittings.**

Instead of using straight bulkhead fittings, use a straight bulkhead fitting on the inside and attach a **John Guest 1/4" push-connect 90-degree elbow (PP0308W)** to the outside of each bulkhead fitting. The tube then exits **parallel to the back panel** (downward or sideways), never perpendicular to the wall.

```
    SIDE VIEW — with 90-degree elbow on outside

    CABINET WALL
    │
    │   tube runs parallel to wall, then curves away
    │   ←───────────────────────○ tube to carbonator
    │                           │
    │              ┌────────────┘ 90° elbow
    │              │
    │   ┌──────────┤ bulkhead fitting
    │   │  PANEL   │
    │   └──────────┤
    │              │
    │              └──── tube to internal plumbing
    │
    ENCLOSURE INTERIOR
```

**Protrusion with elbow:** The 90-degree elbow adds approximately 25mm to the protrusion. Total protrusion from panel surface: ~50-55mm. But the tube exits parallel to the wall, so the enclosure can sit as close as **60mm (2.4 inches)** from the cabinet wall with no kink risk.

**Alternative: Recessed fitting area.** Instead of elbows, the back panel could have a recessed channel (trough) at the bottom where the fittings sit. The recess is 30-40mm deep, so the fittings protrude into the recess but not beyond the back panel's outermost surface. Tubes enter the recess from below (through an open bottom) and route upward into the fittings. This eliminates protrusion entirely but is more complex to print and restricts access.

**Recommended approach:** Use 90-degree elbows on the outside of the three water bulkhead fittings. They are cheap (~$1.50 each), universally available in the John Guest ecosystem, and solve the strain relief problem with zero complexity in the enclosure design.

**Cable strain relief:** The 12V power cable and air switch tube are both flexible and tolerate tight bends. A simple rubber grommet for the air tube and a panel-mount barrel jack for 12V are sufficient. No 90-degree adapters needed for non-water connections.

---

## 2. Internal Fluid Routing

### 2a. Complete Plumbing Diagram

The system has two independent flavoring channels sharing a common carbonated water main line and a common clean water supply line. Below is every fitting, valve, tube segment, and branch in the system.

**Notation:**
- `──` = 1/4" OD hard tubing (push-connect compatible)
- `~~` = soft silicone/BPT tubing (via hard-tube transition + zip tie)
- `[BH]` = bulkhead fitting (back panel)
- `[T]` = tee fitting
- `[NV]` = needle valve
- `[SV-C1]` = clean solenoid valve, flavor 1
- `[SV-C2]` = clean solenoid valve, flavor 2
- `[SV-D1]` = dispensing solenoid valve, flavor 1
- `[SV-D2]` = dispensing solenoid valve, flavor 2
- `[P1]` = peristaltic pump, flavor 1 (in cartridge)
- `[P2]` = peristaltic pump, flavor 2 (in cartridge)
- `[FM]` = flow meter
- `[DOCK]` = cartridge dock fluid connection (John Guest fitting in dock wall)

#### Master Plumbing Diagram

```
BACK PANEL                          INTERNAL ROUTING
──────────                          ────────────────

                                    ┌─── Platypus Bag 1
                                    │    (flavor concentrate)
                                    │
                                    ~~ soft silicone, ~300mm
                                    │
                                    ├── hard tube transition + zip tie
                                    │
                         ┌──────────[T1]──────────────[SV-D1]
                         │          tee               dispensing solenoid 1
                         │                                │
                         │                            ~~ soft silicone ~150mm
                         │                                │
                         │                            [DOCK inlet 1]
                         │                                │
                         │                            ════════════════
                         │                            ║ CARTRIDGE    ║
                         │                            ║   [P1]       ║
                         │                            ║ pump 1       ║
                         │                            ════════════════
                         │                                │
                         │                            [DOCK outlet 1]
                         │                                │
                         │                            ~~ soft silicone ~100mm
                         │                                │
[BH: TAP IN] ──(~150mm)──[T-CLEAN]──(~100mm)──[NV]──(~80mm)──[SV-C1]──(~100mm)──┘
back panel      hard tube  tee       hard tube  needle   hard     clean        hard tube
                          (splits     valve     solenoid 1      to tee T1
                           to both
                           flavors)
                              │
                              └──(~100mm)──[SV-C2]──(~100mm)──┐
                                 hard tube  clean              hard tube
                                           solenoid 2         to tee T2
                                                              │
                         ┌──────────[T2]──────────────[SV-D2]
                         │          tee               dispensing solenoid 2
                         │                                │
                         │                            ~~ soft silicone ~150mm
                         │                                │
                         │                            [DOCK inlet 2]
                         │                                │
                         │                            ════════════════
                         │                            ║ CARTRIDGE    ║
                         │                            ║   [P2]       ║
                         │                            ║ pump 2       ║
                         │                            ════════════════
                         │                                │
                         │                            [DOCK outlet 2]
                         │                                │
                         │                            ~~ soft silicone ~100mm
                         │                                │
                         └────────────────────────────────┘
                                                          │
                                                      [T-INJECT]
                                                      injection tee
                                                          │
[BH: CARB IN] ──(~100mm)──[FM]──(~80mm)──[T-INJECT]──(~100mm)──[BH: CARB OUT]
back panel       hard tube  flow   hard tube  injection   hard tube   back panel
                           meter              tee                    (to faucet)


                                    ┌─── Platypus Bag 2
                                    │    (flavor concentrate)
                                    │
                                    ~~ soft silicone, ~300mm
                                    │
                                    ├── hard tube transition + zip tie
                                    │
                                    └── connects to [T2] (see above)
```

#### Simplified Flow Diagram

```
                          ┌── Bag 1 ──── T1 ──── SV-D1 ──── DOCK IN 1 ──── [P1] ──── DOCK OUT 1 ─┐
                          │              ↑                                                          │
TAP IN ── T-CLEAN ── NV ─┤              SV-C1                                                      ├── T-INJECT ── CARB OUT
                          │              ↑                                                          │
                          └── Bag 2 ──── T2 ──── SV-D2 ──── DOCK IN 2 ──── [P2] ──── DOCK OUT 2 ─┘
                                                                                                    ↑
                                                                                              CARB IN ── FM
```

#### Operating Modes

**Normal dispensing (Flavor 1 active):**
- SV-D1: OPEN, SV-D2: CLOSED, SV-C1: CLOSED, SV-C2: CLOSED
- P1: ON (duty-cycled by flow meter), P2: OFF
- Flow: Bag 1 -> T1 -> SV-D1 -> DOCK IN 1 -> P1 -> DOCK OUT 1 -> T-INJECT -> CARB OUT
- Carbonated water: CARB IN -> FM -> T-INJECT (flavor injected here) -> CARB OUT

**Normal dispensing (Flavor 2 active):**
- SV-D1: CLOSED, SV-D2: OPEN, SV-C1: CLOSED, SV-C2: CLOSED
- P1: OFF, P2: ON (duty-cycled)
- Flow: Bag 2 -> T2 -> SV-D2 -> DOCK IN 2 -> P2 -> DOCK OUT 2 -> T-INJECT -> CARB OUT

**Clean cycle fill (water into Bag 1):**
- SV-D1: CLOSED, SV-C1: OPEN, SV-C2: CLOSED, P1: OFF
- Flow: TAP IN -> T-CLEAN -> NV -> SV-C1 -> T1 -> Bag 1
- Water fills the bag (gravity/pressure-fed through the needle valve)

**Clean cycle flush (pump Bag 1 out to faucet):**
- SV-D1: OPEN, SV-C1: CLOSED, P1: ON
- Flow: Bag 1 -> T1 -> SV-D1 -> DOCK IN 1 -> P1 -> DOCK OUT 1 -> T-INJECT -> CARB OUT (to faucet)
- Pump pushes rinse water through the dispensing path and out

**Hopper refill (concentrate from hopper into bag, future):**
- Separate hopper pump (not through cartridge pumps) — cartridge pumps are unidirectional peristaltic pumps and should not be reversed
- Hopper system is a separate fluid path that feeds into the bag from above or through a separate port
- Does not interact with the cartridge or the back panel plumbing

**Idle (all solenoids closed):**
- SV-D1: CLOSED, SV-D2: CLOSED, SV-C1: CLOSED, SV-C2: CLOSED
- All pumps OFF
- Carbonated water line is pressurized but static (no flow through FM)
- Flavor lines are at atmospheric pressure (sealed by peristaltic pump roller occlusion + closed solenoid)

### 2b. Tube Routing Inside the Enclosure

The enclosure interior can be divided into routing zones. The key routing constraint is that tubes must travel from the back panel fittings to components distributed throughout the enclosure, with minimum bend radius of 30-40mm for hard tubing (cold bend) or 15-20mm for soft silicone/BPT.

**Routing from back panel to components:**

```
    TOP VIEW (looking down into enclosure, back panel at top)

    ┌─────────── BACK PANEL ──────────────────────┐
    │  [TAP IN]        [CARB IN]      [CARB OUT]  │
    │     │               │               ↑       │
    │     │               │               │       │
    │     ▼               ▼               │       │
    │  [T-CLEAN]       [FM]              │       │
    │   │    │            │               │       │
    │   │    │            ▼               │       │
    │   │    │         [T-INJECT]─────────┘       │
    │   │    │            ↑                       │
    │   │    └──[NV]      │                       │
    │   │        │        │                       │
    │   │   ┌────┴────┐   │                       │
    │   │   │[SV-C1]  │   │                       │
    │   │   │  [SV-C2]│   │                       │
    │   │   └────┬────┘   │                       │
    │   │        │        │                       │
    │   │    ┌───┴───┐    │                       │
    │   │    │ [T1]  │    │                       │
    │   │    │ [T2]  │    │                       │
    │   │    └──┬────┘    │                       │
    │   │       │         │                       │
    │  ┌┴───────┴─────────┴──┐                    │
    │  │   [SV-D1] [SV-D2]  │                    │
    │  │   DOCK INLET 1 & 2 │                    │
    │  │                     │                    │
    │  │   ═══════════════   │                    │
    │  │   ║ CARTRIDGE   ║   │                    │
    │  │   ║ [P1]  [P2]  ║   │                    │
    │  │   ═══════════════   │                    │
    │  │                     │                    │
    │  │   DOCK OUTLET 1 & 2 │                    │
    │  └─────────┬───────────┘                    │
    │            │                                │
    │            └── to T-INJECT (above)          │
    │                                             │
    │        [Bag 1]          [Bag 2]             │
    │                                             │
    └──────────── FRONT ──────────────────────────┘
```

**Routing strategy:**

1. **Carbonated water main line:** Enters back panel (CARB IN), runs ~100mm straight forward to the flow meter, then ~80mm to the injection tee (T-INJECT), then returns ~100mm back to the back panel (CARB OUT). This is a U-shaped path: in from back, forward to the injection point, back out. The injection tee should be positioned near the dock outlets to minimize the tube run from pump outlets to the injection point.

2. **Clean water supply:** Enters back panel (TAP IN), runs ~150mm to the clean tee (T-CLEAN), which splits to both clean solenoids via the needle valve. The needle valve sits between T-CLEAN and the first clean solenoid. Total clean supply tube run: ~150mm + 100mm (NV) + 80mm (SV-C1) + 100mm (to T1), plus a similar branch to SV-C2/T2.

3. **Flavor lines (bag to tee):** Soft silicone from each bag (~300mm) routes to the flavor tees (T1, T2). These are the most flexible tubes in the system and can route around obstacles.

4. **Dispensing solenoid to dock:** From T1/T2 through SV-D1/SV-D2 to the dock inlet fittings. Short runs (~150mm each), can be soft silicone with hard tube transitions at the push-connect fittings.

5. **Dock outlets to injection tee:** From the dock outlet fittings to T-INJECT. Both pump outlets must merge into the carbonated water line. A second tee or manifold merges both pump outputs before reaching T-INJECT, or T-INJECT is a 4-way cross (uncommon in push-connect ecosystem, so two tees in series is more practical).

**Merging pump outputs into carbonated water line:**

```
    DOCK OUT 1 ──(~80mm)── [T-MERGE] ──(~50mm)── [T-INJECT] ── CARB OUT
                              ↑                       ↑
    DOCK OUT 2 ──(~80mm)─────┘                   CARB IN ── FM
```

T-MERGE is a tee that combines both pump outputs. T-INJECT is a tee where the combined flavor line meets the carbonated water main line. Both are standard 1/4" push-connect tees from the ice maker kit.

### 2c. Fitting and Tube Inventory

#### Push-Connect Fittings

| Fitting | Type | Size | Qty | Notes |
|---------|------|------|-----|-------|
| Back panel bulkheads | John Guest PP1208W bulkhead union | 1/4" x 1/4" | 3 | TAP IN, CARB IN, CARB OUT |
| External 90-degree elbows | John Guest PP0308W elbow | 1/4" | 3 | Strain relief on outside of back panel |
| Clean water supply tee | Union tee | 1/4" | 1 | T-CLEAN: splits tap water to both clean solenoids |
| Flavor 1 tee | Union tee | 1/4" | 1 | T1: connects bag, clean solenoid, dispensing solenoid |
| Flavor 2 tee | Union tee | 1/4" | 1 | T2: same for flavor 2 |
| Pump output merge tee | Union tee | 1/4" | 1 | T-MERGE: combines both pump outputs |
| Injection tee | Union tee | 1/4" | 1 | T-INJECT: flavor line meets carbonated water |
| Dock wall fittings | John Guest fitting (in dock) | 1/4" | 4 | 2 inlets, 2 outlets on dock wall |
| **Total tees** | | | **5** | |
| **Total bulkheads** | | | **3** | |
| **Total elbows** | | | **3** | (external strain relief) |
| **Total dock fittings** | | | **4** | |
| **Grand total push-connect fittings** | | | **15** | Plus fittings integral to solenoid valves |

#### Solenoid Valves (Each Has Integral Push-Connect Fittings)

| Valve | Type | Qty | Notes |
|-------|------|-----|-------|
| Dispensing solenoid | Beduan 12V NC, 1/4" push-connect | 2 | SV-D1, SV-D2 |
| Clean solenoid | Beduan 12V NC, 1/4" push-connect | 2 | SV-C1, SV-C2 |
| **Total solenoid valves** | | **4** | |

#### Other Inline Components

| Component | Qty | Notes |
|-----------|-----|-------|
| Needle valve (YKEBVPW) | 1 | NV: flow restriction on clean water supply |
| Flow meter | 1 | FM: inline with carbonated water |

#### Tubing Estimate

| Segment | Type | Length | Qty | Subtotal |
|---------|------|--------|-----|----------|
| Back panel to T-CLEAN | 1/4" hard | ~150mm | 1 | 150mm |
| T-CLEAN to NV | 1/4" hard | ~100mm | 1 | 100mm |
| NV to SV-C1 | 1/4" hard | ~80mm | 1 | 80mm |
| NV to SV-C2 (via branch from T-CLEAN) | 1/4" hard | ~180mm | 1 | 180mm |
| SV-C1 to T1 | 1/4" hard | ~100mm | 1 | 100mm |
| SV-C2 to T2 | 1/4" hard | ~100mm | 1 | 100mm |
| Back panel (CARB IN) to FM | 1/4" hard | ~100mm | 1 | 100mm |
| FM to T-INJECT | 1/4" hard | ~80mm | 1 | 80mm |
| T-INJECT to back panel (CARB OUT) | 1/4" hard | ~100mm | 1 | 100mm |
| Dock outlets to T-MERGE | Soft silicone + hard transition | ~80mm each | 2 | 160mm |
| T-MERGE to T-INJECT | 1/4" hard | ~50mm | 1 | 50mm |
| Bag to T1 (with transition) | Soft silicone + hard | ~300mm | 1 | 300mm |
| Bag to T2 (with transition) | Soft silicone + hard | ~300mm | 1 | 300mm |
| T1 to SV-D1 | 1/4" hard or soft | ~80mm | 1 | 80mm |
| T2 to SV-D2 | 1/4" hard or soft | ~80mm | 1 | 80mm |
| SV-D1 to dock inlet 1 | Soft silicone + hard | ~150mm | 1 | 150mm |
| SV-D2 to dock inlet 2 | Soft silicone + hard | ~150mm | 1 | 150mm |
| **Total hard tubing** | | | | **~1,200mm (4 ft)** |
| **Total soft silicone** | | | | **~1,100mm (3.6 ft)** |
| **Grand total tubing** | | | | **~2,300mm (7.5 ft)** |

The ice maker kit includes a large spool of 1/4" hard tubing (typically 25 feet), which is more than sufficient. Soft silicone is from the existing 6m spool (also sufficient).

**Hard-to-soft transitions needed:** ~8 locations (each bag connection, each dock fitting connection, each solenoid-to-soft-tube connection). Each transition uses ~30mm of hard tube as a stub inserted into the soft tube, secured with a zip tie.

### 2d. Accessibility for Maintenance

**Most likely service needs:**

1. **Bag replacement** (most frequent): User removes old Platypus bag, connects new one. The bag-to-silicone-tube connection is a zip-tie joint — needs to be easily accessible from the front or top of the enclosure. Bags should hang or sit in a position where the user can reach the zip-tie joint without removing any panels.

2. **Cartridge replacement** (periodic): The dock and cartridge are designed for tool-free replacement from the front. No panel removal needed.

3. **Leaking push-connect fitting** (rare): If an internal fitting leaks, the user needs to access it. The removable back panel exposes the plumbing zone. Additionally, a removable side panel or top panel provides visual inspection access.

4. **Solenoid valve replacement** (very rare): Solenoid valves have integral push-connect fittings — replacement is push-in/push-out. Accessible if the plumbing zone is reachable through a removable panel.

**Recommendation:** The enclosure should have a **removable back panel** (for plumbing access) and a **removable top panel** (for visual inspection and bag replacement). The front face is occupied by the cartridge dock opening and display holders, so it cannot be removable. The sides could be fixed (structural).

---

## 3. Internal Electrical Routing

### 3a. Electronics Mounting Layout

| Component | Dimensions (approx) | Mount Location | Rationale |
|-----------|---------------------|----------------|-----------|
| ESP32 on DIN rail breakout | ~90 x 50 x 25mm | Upper rear wall (inside, near back panel) | Close to back panel for power input; central for wire routing to all peripherals |
| L298N Board A (pump 1 + valve 1) | ~43 x 43 x 27mm | Upper side wall, near dock | Short wire runs to dock pogo pins and solenoid valve 1 |
| L298N Board B (pump 2 + valve 2) | ~43 x 43 x 27mm | Upper side wall, opposite side of dock | Mirror of Board A |
| L298N Board C (clean solenoids, future) | ~43 x 43 x 27mm | Upper rear wall, near ESP32 | Controls clean solenoids; can be near ESP32 since wire runs to solenoids are short |
| DS3231 RTC | ~38 x 22 x 14mm | Adjacent to ESP32, on DIN rail or piggyback | I2C bus — needs to be within ~200mm of ESP32 |
| Flow meter | ~60 x 35 x 20mm | Inline with carbonated water pipe | Position constrained by plumbing — near T-INJECT |

**Layout principle:** Electronics mount in the **upper half** of the enclosure. Plumbing (valves, tees, needle valve) occupies the **lower half**. The cartridge dock sits in the **middle**, bridging both zones (electrical connections on top of dock via pogo pins, fluid connections on the dock wall below). Bags hang or sit in the **lower front** area.

```
    SIDE VIEW (cross section, back panel on right)

    ┌───────────────────────────────────────────┐
    │ ┌─────────┐                    ┌────────┐ │
    │ │Display  │   ESP32  L298N x3  │ 12V IN │ │  UPPER: electronics
    │ │holders  │   RTC              │        │ │
    │ ├─────────┤                    ├────────┤ │
    │ │         │                    │        │ │
    │ │ DOCK    │  Solenoids, NV, FM │ plumb  │ │  MIDDLE: dock + valves
    │ │ (cart.) │  Tees, tubing      │ zone   │ │
    │ │         │                    │        │ │
    │ ├─────────┤                    ├────────┤ │
    │ │         │                    │        │ │
    │ │ Bags    │  Bag area          │        │ │  LOWER: bags, drip tray
    │ │         │                    │        │ │
    │ └─────────┘                    └────────┘ │
    └───────────────────────────────────────────┘
      FRONT                               BACK
```

### 3b. Wire Routing Strategy

**Current state: DuPont jumper wires.** Fine for prototyping but fragile for a permanent enclosure. For the enclosure build, wiring should be upgraded to:

| Connection | Wire Type | Length Est. | Notes |
|-----------|-----------|-------------|-------|
| ESP32 to L298N A (6 wires) | 22AWG stranded, bundled | ~200mm | ENA, IN1, IN2, ENB, IN3, IN4 |
| ESP32 to L298N B (6 wires) | 22AWG stranded, bundled | ~200mm | Same pin set, different board |
| ESP32 to L298N C (4 wires) | 22AWG stranded, bundled | ~150mm | ENB x2, IN pairs |
| ESP32 UART1 to S3 display (4 wires) | Cat6 cable (using 4 of 8 conductors) | ~500mm | TX, RX, 3.3V, GND |
| ESP32 UART2 to RP2040 display (4 wires) | Cat6 cable (using 4 of 8 conductors) | ~500mm | TX, RX, 5V, GND |
| ESP32 to flow meter (3 wires) | 22AWG stranded | ~250mm | VCC, GND, signal (GPIO 23) |
| ESP32 to air switch (2 wires) | 22AWG stranded | ~300mm | GPIO 13, GND |
| ESP32 I2C to RTC (4 wires) | 22AWG stranded | ~100mm | SDA, SCL, VCC, GND |
| L298N A motor out to dock pogo (3 wires) | 18AWG stranded (motor current) | ~150mm | GND, Motor A+, Motor B+ |
| L298N B motor out to dock pogo (3 wires) | 18AWG stranded | ~150mm | Same |
| L298N A valve out to SV-D1 (2 wires) | 20AWG stranded | ~200mm | 12V switched |
| L298N B valve out to SV-D2 (2 wires) | 20AWG stranded | ~200mm | 12V switched |
| L298N C to SV-C1 (2 wires) | 20AWG stranded | ~200mm | 12V switched |
| L298N C to SV-C2 (2 wires) | 20AWG stranded | ~200mm | 12V switched |
| 12V power distribution (see 3c) | 16AWG stranded | varies | Main power bus |

**Wire management options:**

1. **Printed wire channels:** Channels (U-shaped troughs with snap-on lids) printed into the enclosure walls. Route wires neatly along wall surfaces. Easy to print, clean appearance, protects wires from snagging.

2. **Cable ties to printed anchor points:** Small loops or posts printed on the enclosure walls at ~50mm intervals. Wire bundles zip-tied to them. Simpler to design, easier to modify routing.

3. **Ribbon cable for multi-wire runs:** The ESP32-to-L298N connections (6 wires each) could use 6-conductor ribbon cable with IDC connectors. Cleaner than individual jumpers, but requires IDC crimping tools.

**Recommended:** Printed wire channels on the upper walls for the main signal runs (ESP32 to L298N boards, ESP32 to displays). Cable ties for shorter runs (flow meter, air switch, RTC). This provides a clean, maintainable installation.

**Wire protection from water:** The upper-electronics/lower-plumbing separation is the primary defense. Additionally:
- A **drip shield** (horizontal shelf or overhang) between the electronics zone and the plumbing zone catches any upward spray or dripping from a leaking fitting
- All wire connections should use **crimp terminals** (spade or ferrule) rather than bare wire, reducing corrosion risk
- The bag area should have a **drip tray** at the bottom of the enclosure to catch any bag leaks

### 3c. Power Distribution

**Power budget:**

| Consumer | Voltage | Current (max) | Power | Duty |
|----------|---------|--------------|-------|------|
| Pump 1 (Kamoer KPHM400) | 12V | 0.7A | 8.4W | Intermittent (duty-cycled during pour) |
| Pump 2 (Kamoer KPHM400) | 12V | 0.7A | 8.4W | Intermittent (only one active at a time) |
| Solenoid valve x1 (active) | 12V | 0.46A | 5.5W | One dispensing + one clean max at a time |
| Solenoid valve x1 (clean) | 12V | 0.46A | 5.5W | During clean cycle only |
| ESP32 (via VIN or regulator) | 12V -> 3.3V | ~0.15A at 12V | ~1.8W | Continuous |
| L298N quiescent (x3 boards) | 12V | ~0.02A each | ~0.7W total | Continuous |
| DS3231 RTC | 3.3V (from ESP32) | negligible | <0.01W | Continuous |
| RP2040 display (via UART cable) | 5V (from ESP32 or separate) | ~0.1A | 0.5W | Continuous |
| S3 display (via UART cable) | 5V (from ESP32 USB or separate) | ~0.15A | 0.75W | Continuous |

**Worst case simultaneous draw:** 1 pump + 2 solenoids (dispensing during clean overlap scenario) + ESP32 + displays:
- 0.7A + 0.46A + 0.46A + 0.15A + 0.25A = **~2.0A at 12V** = **~24W**

**Typical operating draw:** 1 pump + 1 solenoid + ESP32 + displays:
- 0.7A + 0.46A + 0.15A + 0.25A = **~1.56A at 12V** = **~18.7W**

The existing 12V 2A wall adapter is borderline for worst-case. A **12V 3A (36W) adapter** provides comfortable headroom and is recommended.

**Power distribution architecture:**

```
    12V IN (back panel barrel jack)
         │
         ▼
    [FUSED TERMINAL BLOCK]  ← blade fuse, 3A
         │
         ├─── 12V bus (+) ──┬── L298N Board A (12V input)
         │                  ├── L298N Board B (12V input)
         │                  ├── L298N Board C (12V input)
         │                  └── ESP32 VIN pin
         │
         └─── GND bus (-) ──┬── L298N Board A (GND)
                            ├── L298N Board B (GND)
                            ├── L298N Board C (GND)
                            └── ESP32 GND
```

**Fuse protection:** A single 3A automotive blade fuse at the 12V input protects the entire system. The L298N boards have their own internal thermal shutdown but no fusing. The input fuse prevents cable/connector fires if a short circuit occurs downstream.

**Implementation:** A small DIN-rail-mount fuse holder and terminal block assembly. The DIN rail for the ESP32 breakout can also mount the fuse holder and terminal blocks. Total DIN rail length needed: ~150-200mm (6-8").

Alternatively, a simpler approach: a screw terminal distribution block (not DIN rail). A 4-position screw terminal block with one input and three outputs for 12V, plus matching GND block. Mounts anywhere with two screws.

**ESP32 power:** The ESP32 dev board's VIN pin accepts 7-12V and has an onboard regulator. Feeding 12V to VIN is within spec but makes the onboard regulator work hard (dropping 12V to 3.3V at ~200mA = ~1.7W dissipated in the regulator). This is acceptable for a ventilated enclosure. The alternative is a separate 12V-to-5V buck converter feeding the ESP32's 5V/USB pin, which is more efficient but adds a component.

### 3d. Signal Integrity

At 115200 baud, bit time is ~8.7 microseconds — this is very tolerant of noise. However, basic routing hygiene avoids intermittent communication issues:

**Keep apart:**
- UART signal wires (to S3, RP2040) should not run parallel to motor power wires (L298N outputs to pumps) for more than ~50mm
- If they must cross, cross at 90 degrees

**Keep together:**
- Each UART pair (TX + RX) should run with its GND wire as a group — the cat6 cable already does this with twisted pairs
- I2C (SDA + SCL) should run together with GND

**Not a concern:**
- The flow meter signal (GPIO 23) is interrupt-driven pulses, not high-speed data. Noise immunity is high.
- The air switch is a simple digital input with a pull-up resistor. Debounced in software.

**Practical routing:** Since the electronics are in the upper zone and the L298N motor outputs route downward to the dock/solenoids, the signal wires (routing horizontally to displays on the front face) naturally separate from the motor power wires (routing vertically downward). The printed wire channels should maintain this separation by having signal channels on the upper walls and power channels on the side walls.

---

## 4. Cartridge Dock Internal Connections

### 4a. Dock as a Structural Sub-Assembly

The dock should be a **separate structural assembly** that mounts inside the enclosure. Reasons:

1. **Print tolerance:** The dock requires tight alignment tolerances (0.3-0.5mm per side for guide rails). Printing the dock as part of the enclosure body would make the entire enclosure a precision print. A separate dock can be printed, tested, and reprinted independently.

2. **Material choice:** The dock may benefit from a stiffer material (PLA for rigidity) while the enclosure body may be PETG (for impact resistance). Separate assemblies allow different materials.

3. **Alignment adjustment:** The dock can be shimmed or adjusted after mounting to ensure perfect cartridge engagement.

4. **Replacement:** If the dock's guide rails or fitting mounts wear, replace just the dock (small print) without reprinting the enclosure.

**Mounting method:** The dock mounts to the enclosure's internal ribs or walls using M4 bolts through the dock's back plate into heat-set inserts in the enclosure. 4 mounting points for rigidity. The dock's back plate is a flat surface that registers against a flat internal surface of the enclosure.

### 4b. Dock Connections

**Fluid connections (4 total):**

```
    DOCK WALL (facing cartridge)        DOCK BACK (facing enclosure interior)

    ┌──────────────────────────────────────────────────────┐
    │                                                      │
    │    [JG fitting]  [JG fitting]          tubes to      │
    │     inlet 1       inlet 2         ←── solenoid valves│
    │                                                      │
    │    [JG fitting]  [JG fitting]          tubes to      │
    │     outlet 1      outlet 2        ←── T-MERGE tee    │
    │                                                      │
    └──────────────────────────────────────────────────────┘
```

The John Guest fittings are press-fit or bonded into the dock wall. Tubes from the enclosure's internal plumbing push into the back side of these fittings. The cartridge's tube stubs push into the front side during insertion.

**Electrical connections (3 pogo pins):**

The dock has 3 spring-loaded pogo pins on its top surface (or a bracket above the cartridge slot). These contact flat pads on the cartridge's top face when docked. Wires from the pogo pins route to the nearest L298N board (motor driver outputs).

**Guide rails:**

Two guide rails run the full depth of the dock interior, one on each side. These are structural ribs printed into the dock body. The cartridge has matching grooves. The rails provide:
- Lateral alignment (left-right)
- Vertical alignment (up-down)
- Smooth insertion (the cartridge slides on the rails)

### 4c. Structural Integration

The dock is the most structurally demanding component inside the enclosure. During cartridge insertion, the user pushes the cartridge firmly into the dock — this force must be absorbed by the enclosure frame through the dock mounting points.

**Load path:** User push force (5-10N for fitting insertion) -> cartridge -> dock guide rails -> dock back plate -> mounting bolts -> enclosure internal ribs -> enclosure base.

The enclosure should have **reinforced internal ribs** at the dock mounting area. These ribs connect the dock mounting surface to the enclosure walls and base, distributing the insertion force across the structure.

---

## 5. Display Cable Routing

### 5a. Cable Specifications

The S3 and RP2040 displays connect to the ESP32 via UART using cat6 cable as a multi-conductor cable:

| Parameter | Standard Cat6 | Flat Cat6 |
|-----------|--------------|-----------|
| OD / cross-section | ~6mm round | ~1.5mm x 6mm |
| Minimum bend radius | ~25mm (4x OD) | ~10-15mm (due to thin axis) |
| Conductors | 8 (4 twisted pairs) | 8 (4 pairs, not always twisted) |
| Conductors used | 4 (TX, RX, VCC, GND) | 4 |
| Jacket material | PVC or LSZH | PVC |

**Flat cat6 is strongly preferred** for this application: tighter bend radius means the cable can coil in a smaller space inside the enclosure, and the thin profile fits through smaller grommets.

### 5b. Retraction Mechanism Options

**Option 1: Spring-loaded retractable reel**

Badge reels and small cable retractors use a clock-spring mechanism to retract thin cables. However:
- Commercial badge reels are designed for ~2mm cord, not 6mm cat6
- The Stage Ninja CAT6-25-S is a professional retractable cat6 reel, but it is 15-65 feet and physically large (designed for stage/AV use, not enclosure mounting)
- No small (~1m range) retractable cat6 reels exist commercially

Custom-building a small retractable reel is possible but adds significant mechanical complexity. **Not recommended** for this project.

**Option 2: Coiled cable (telephone handset style)**

A coiled (helical) cable naturally extends when pulled and retracts (partially) when released. This is the approach used by telephone handset cords.

| Parameter | Estimate |
|-----------|----------|
| Coiled length (relaxed) | ~150-200mm |
| Extended length | ~1,000-1,500mm |
| Retraction ratio | 5:1 to 8:1 typical for PVC coil |
| Cable OD | ~3-4mm (custom 4-conductor) |
| Coil OD | ~20-30mm |

**Custom coiled cables** with 4+ conductors in the 0.5-1.5m extension range are available from Cable Science and similar manufacturers. A coiled 4-conductor cable with RJ45 or bare-wire termination on each end would serve perfectly.

Alternatively, a **standard coiled telephone handset cord (RJ9 termination, 4 conductors)** can be repurposed. These are commodity items ($3-5), available in lengths from 6" coiled / 6' extended to 2' coiled / 25' extended. The 4 conductors (26AWG) are sufficient for 115200 baud UART over 1.5m.

**Recommended:** Use standard **coiled telephone handset cords** (4P4C / RJ9 connectors) for the display cables. Crimp a matching RJ9 jack onto the ESP32's UART header and onto each display's UART header. The coiled cord provides natural strain relief, self-retraction, and no mechanical complexity.

**Option 3: Fixed cable with slack loop**

The simplest approach: a fixed-length flat cat6 cable (~1.5m) with excess length coiled inside the enclosure when the display is docked. When the display is popped out, the slack uncoils. A small spring clip or bungee inside the enclosure keeps the slack tidy.

This works but risks the slack loop catching on the cartridge or bags during insertion/removal. A cable guide channel along the enclosure wall mitigates this.

### 5c. Cable Exit Points

When the display is popped out of its holder:

**Recommended exit: front face, adjacent to the display holder slot.**

The cable exits through a rubber grommet immediately beside the display holder opening. When the display is docked, the cable loops back inside through the same grommet. When popped out, the cable extends through the grommet to the external mounting point.

```
    FRONT FACE

    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │   ┌─────────────┐  ○ grommet    ┌─────────────┐  ○ │
    │   │ S3 display  │  (S3 cable)   │RP2040 display│    │
    │   │ holder      │               │ holder       │    │
    │   └─────────────┘               └─────────────┘    │
    │                                                     │
    │              ┌───────────────────┐                  │
    │              │  CARTRIDGE DOCK   │                  │
    │              │  OPENING          │                  │
    │              └───────────────────┘                  │
    │                                                     │
    └─────────────────────────────────────────────────────┘
```

**Grommet specs:** A rubber grommet with ~8mm ID (for flat cat6: ~7mm diagonal) in a 12mm panel hole. The grommet is tight enough to provide friction on the cable (preventing it from rattling) but loose enough to slide when pulled.

**Alternative exit: side panel.** Less visible, but the cable has to turn a corner to reach the front-mounted display. Adds unnecessary routing complexity.

**Alternative exit: back panel.** Most discreet, but the cable must route from the front display holder, through the entire enclosure, to the back panel, then around the outside of the enclosure to the external mount point. Far too long and complex.

---

## 6. Thermal Routing

### 6a. Heat Sources and Dissipation

| Component | Max Power Dissipation | Notes |
|-----------|----------------------|-------|
| L298N Board A (pump 1 + valve 1) | ~3-4W | V_drop ~2V at 0.7A (pump) + ~2V at 0.46A (solenoid) |
| L298N Board B (pump 2 + valve 2) | ~3-4W | Same as A (only one active at a time, typically) |
| L298N Board C (clean solenoids) | ~2W | Lower duty, only during clean cycle |
| Pump motor 1 (inside cartridge) | ~4-5W | 8.4W input - ~3-4W mechanical work = ~4-5W heat |
| Pump motor 2 (inside cartridge) | ~4-5W | Same (only one active at a time) |
| ESP32 voltage regulator | ~1.5W | 12V to 3.3V drop at ~170mA |
| ESP32 compute | ~0.5W | WiFi off, BLE active intermittently |

**L298N voltage drop:** The L298N is a bipolar H-bridge with ~2V total saturation drop across the output transistors. For a 12V system driving a 12V motor, this means the motor sees ~10V and the L298N dissipates ~2V x I_motor as heat. At 0.7A: ~1.4W from the motor channel. The solenoid channel adds ~0.92W. Total per board during active operation: **~2.3W**.

**Total heat dissipation during active dispensing:** One L298N active (~2.3W) + ESP32 (~2W) + pump motor in cartridge (~4.5W) = **~8.8W**.

**Total heat dissipation during clean cycle peak:** Two L298N boards active (~4.6W) + ESP32 (~2W) + pump motor (~4.5W) = **~11.1W**.

**Worst case (brief):** All three L298N boards + both pumps: ~14W. This would only occur during a clean cycle if, for some reason, both channels were flushing simultaneously (not the current firmware design).

### 6b. Thermal Management Strategy

**~10-15W of heat in a ~15-liter enclosure volume** is modest. For comparison, a desktop PC idle generates ~30-50W in a similar volume. However, the enclosure is inside a closed cabinet with limited airflow.

**Ambient temperature range:** Under-sink cabinet in a climate-controlled home: 65-80 deg F (18-27 deg C) typical, could reach 85 deg F (29 deg C) in summer without AC.

**Target internal temperature:** Below 50 deg C (122 deg F) — well within operating range for all components. The L298N thermal shutdown triggers at ~150 deg C junction temperature, so even without active cooling, the 2-3W per board dissipation on a board with a large heatsink pad should be fine.

**Ventilation approach: Passive convection with vents.**

```
    SIDE VIEW

    ┌───────────── ENCLOSURE ───────────────┐
    │ ▓▓▓▓▓▓▓▓  top vents (exhaust)  ▓▓▓▓▓ │ <- warm air exits
    │                                       │
    │   L298N boards (heat sources)         │ <- hot components at top
    │   ESP32                               │
    │                                       │
    │   ─────── drip shield ──────────      │
    │                                       │
    │   Solenoids, dock, plumbing           │
    │                                       │
    │   Bags                                │
    │                                       │
    │ ▓▓▓▓▓▓▓▓  bottom vents (intake)▓▓▓▓▓ │ <- cool air enters
    └───────────────────────────────────────┘
```

- **Top vents:** Horizontal louvers or a grid of holes on the top panel. Warm air rises and exits naturally.
- **Bottom vents:** Slots or holes on the bottom or lower side panels. Cool cabinet air enters.
- **Chimney effect:** The vertical separation between heat sources (top) and cool air intake (bottom) creates natural convection. With a 200mm vertical separation and 10W heat input, the natural convection flow rate is roughly 0.5-1 L/min — enough to maintain a 10-15 deg C internal-to-ambient temperature differential.

**Vent design for water protection:** The vents must not allow water to enter if a bag leaks or a fitting drips internally:
- Top vents: OK as-is (water drips down, not up through top vents)
- Bottom vents: Use downward-facing louvers (not straight-through holes) so drips from inside fall into a drip tray, not out of the vents. External water (unlikely under a sink) hits the louver surface and drips away.

**L298N heatsink:** The L298N module's built-in heatsink is adequate for the 2-3W per board in this application. No additional heatsinking or fans needed. Mount the L298N boards with the heatsink fins oriented vertically (parallel to airflow direction) and with 15-20mm clearance above and below each board for convection.

**Pump heat in cartridge:** The pump motors dissipate ~4-5W inside the sealed cartridge body. This heat must conduct through the cartridge walls and dock structure into the enclosure air. The cartridge's PETG walls have low thermal conductivity (~0.2 W/m-K), so the cartridge interior will be warm during operation (estimated 40-50 deg C). This is acceptable — the pump motors are rated for continuous operation at elevated temperatures, and the BPT pump tubing is rated to 80 deg C.

To improve cartridge heat dissipation, the cartridge body could include **ventilation slots** on the top and sides (the mating face and guide rail faces must remain solid for sealing and alignment). Alternatively, the dock's guide rails could have air gaps that allow airflow along the cartridge sides.

---

## 7. Comparison of Internal Layout Strategies

### 7a. Zoned Approach

Fluid zone (bags, valves, pumps, plumbing) on one side; electronics zone (ESP32, L298N boards, wiring) on the other side. A vertical divider wall separates the zones.

```
    TOP VIEW

    ┌──────────────────┬──────────────────┐
    │                  │                  │
    │  FLUID ZONE      │  ELECTRONICS     │
    │                  │  ZONE            │
    │  Bags            │  ESP32           │
    │  Solenoids       │  L298N x3        │
    │  Tees, NV        │  RTC             │
    │  Dock (fluid)    │  Wiring          │
    │  Flow meter      │  Power distrib.  │
    │                  │                  │
    └──────────────────┴──────────────────┘
          BACK PANEL
```

| Criteria | Score (1-5) | Notes |
|----------|------------|-------|
| Routing simplicity | 2 | Long wire runs from electronics zone to dock pogo pins and solenoid valves in fluid zone. Motor power wires must cross the divider. |
| Water safety | 5 | Complete physical separation. A leak in the fluid zone cannot reach electronics. |
| Thermal management | 3 | Electronics zone has concentrated heat but good access to vents. Fluid zone is cool. |
| Maintenance access | 4 | Each zone is independently accessible (removable panel per zone). |
| Build complexity | 3 | Divider wall adds a component; routing through divider needs grommets. |

### 7b. Central Bus Approach

A central vertical spine (printed rib or rail) runs front-to-back. Plumbing routes on one side, wiring on the other. Components mount to the spine or to the enclosure walls.

```
    TOP VIEW

    ┌─────────────────────────────────────┐
    │                                     │
    │  Plumbing     │ SPINE │    Wiring   │
    │  side         │       │    side     │
    │               │       │             │
    │  Tees, NV     │       │   ESP32     │
    │  Solenoids    │       │   L298N     │
    │               │ DOCK  │             │
    │  Flow meter   │       │   RTC       │
    │               │       │             │
    │  Bags         │       │   Power     │
    │               │       │             │
    └─────────────────────────────────────┘
```

| Criteria | Score (1-5) | Notes |
|----------|------------|-------|
| Routing simplicity | 4 | The spine provides a natural organizing structure. Short cross-spine connections for motor power. |
| Water safety | 3 | Separation is present but not complete — no physical barrier like the zoned approach. |
| Thermal management | 4 | Electronics on one side can have dedicated vents. Plumbing side stays cool. |
| Maintenance access | 3 | Accessing the spine area requires reaching past components on both sides. |
| Build complexity | 4 | The spine is a natural structural element (stiffens the enclosure). Clean design. |

### 7c. Layered Approach

Bottom layer: plumbing and valves. Middle layer: cartridge dock. Top layer: electronics and displays. Horizontal shelves separate the layers.

```
    SIDE VIEW

    ┌─────────────────────────────────────┐
    │   [Displays]   ESP32   L298N        │  TOP LAYER: electronics
    │─────────────── shelf ───────────────│
    │   [DOCK]   Solenoids   Flow meter   │  MIDDLE LAYER: dock + valves
    │─────────────── shelf ───────────────│
    │   [Bags]   Tees   NV   Tube runs    │  BOTTOM LAYER: plumbing + bags
    └─────────────────────────────────────┘
```

| Criteria | Score (1-5) | Notes |
|----------|------------|-------|
| Routing simplicity | 4 | Vertical wire drops from top layer to middle layer (short). Tube routing on bottom layer is unconstrained. |
| Water safety | 5 | Water cannot rise from bottom layer to top layer. The middle shelf is a physical barrier. Drips from the dock drain to the bottom layer. |
| Thermal management | 5 | Heat sources at the top — natural convection carries heat straight up and out through top vents. Coolest layer at bottom (where bags benefit from cool temps). |
| Maintenance access | 4 | Removable top panel exposes electronics. Removable front or side panel exposes dock and bags. Each layer is independently accessible. |
| Build complexity | 3 | Shelves must support component weight and allow wire/tube pass-throughs. More structural design than the other approaches. |

### 7d. Scoring Summary

| Criteria | Weight | Zoned | Central Bus | Layered |
|----------|--------|-------|-------------|---------|
| Routing simplicity | 4 | 2 | 4 | 4 |
| Water safety | 5 | 5 | 3 | 5 |
| Thermal management | 3 | 3 | 4 | 5 |
| Maintenance access | 3 | 4 | 3 | 4 |
| Build complexity | 2 | 3 | 4 | 3 |
| **Weighted total** | | **59** | **60** | **73** |

### 7e. Recommendation

**The layered approach scores highest** and is recommended. Its advantages are decisive:

1. **Water safety is inherent** — gravity keeps water in the bottom layer, electronics in the top layer. No additional sealing or barriers needed beyond the structural shelves.

2. **Thermal management is optimal** — hot components at the top with direct vent access. Cool air enters at the bottom, warms as it rises past the dock and plumbing, and exits through top vents. The bags in the bottom layer stay coolest (beneficial for concentrate shelf life).

3. **Routing is natural** — wires drop vertically from ESP32/L298N (top) to solenoid valves and dock pogo pins (middle). Tubes route horizontally on the bottom layer from back panel fittings to tees and valves in the middle layer. Display cables route forward from the top layer to front-face holders.

4. **Maintenance access is zoned by layer** — remove the top panel to access electronics, open the front to access the dock and bags.

The middle shelf should have openings/cutouts for:
- Wire pass-throughs (small holes with grommets, positioned away from water)
- Tube pass-throughs (larger holes for 1/4" tubing, positioned above the plumbing zone)
- The dock body itself (the dock may span the middle and bottom layers)

---

## 8. Fitting and Parts Summary

### Complete Parts List for Back Panel and Internal Routing

#### Back Panel Hardware

| Part | Qty | Source | Est. Cost |
|------|-----|--------|-----------|
| John Guest PP1208W 1/4" bulkhead union | 3 | [Home Depot](https://www.homedepot.com/p/John-Guest-1-4-in-Push-to-Connect-Bulkhead-Fitting-10-Pack-PP1208W-US/335236458) (10-pack) | ~$14 for 10 |
| John Guest PP0308W 1/4" 90-degree elbow | 3 | [Home Depot](https://www.homedepot.com/p/John-Guest-1-4-in-Push-To-Connect-90-Degree-Polypropylene-Elbow-Fitting-804529/303347813) or [Amazon](https://www.amazon.com/JG-Speedfit-PP0308WP-4-Inch-Union/dp/B005S4O0AY) | ~$5 for 10 |
| DC barrel jack 5.5x2.1mm panel mount, waterproof | 1 | [Amazon (DaierTek)](https://www.amazon.com/5-5x2-1MM-Pre-Wired-Connector-Waterproof-Appliances/dp/B0BD46CP5Y) | ~$8 for 6 |
| Rubber grommet, 3-4mm ID / 6mm panel hole | 1 | [Amazon](https://www.amazon.com/grommet-cable-pass-through/s?k=grommet+cable+pass+through) or [Essentra Components](https://www.essentracomponents.com/en-us/s/cable-grommets/rubber) | ~$5 for assortment |
| Panel-mount USB Micro-B extension | 1 | [Adafruit #3258](https://www.adafruit.com/product/3258) | ~$3 |
| M3 x 8mm screws (panel attachment) | 6 | hardware kit | ~$2 |
| M3 heat-set inserts | 6 | hardware kit | ~$3 |

#### Internal Plumbing Hardware

| Part | Qty | Source | Est. Cost |
|------|-----|--------|-----------|
| John Guest 1/4" union tee (from ice maker kit) | 5 | Already on hand | $0 |
| 1/4" OD hard tubing (from ice maker kit) | ~4 ft | Already on hand | $0 |
| 1/8" ID x 1/4" OD soft silicone tubing | ~4 ft | Already on hand (6m spool) | $0 |
| Beduan 12V solenoid valve NC, 1/4" push-connect | 4 | [Amazon (B07NWCQJK9)](https://www.amazon.com/Beduan-Solenoid-Normally-Reverse-Osmosis/dp/B07NWCQJK9) | 2 on hand + 2 ordered ~$18 |
| YKEBVPW 1/4" needle valve | 1 | [Amazon (B0FBFVTNLM)](https://www.amazon.com/dp/B0FBFVTNLM) | On hand ~$7.50 |
| Zip ties (small, for tube transitions) | ~20 | hardware kit | ~$2 |

#### Electrical/Power Hardware

| Part | Qty | Source | Est. Cost |
|------|-----|--------|-----------|
| 12V 3A DC power adapter (5.5x2.1mm barrel) | 1 | Amazon | ~$10 |
| Screw terminal block (4-position, x2 for +/-) | 2 | Amazon | ~$5 |
| 3A ATC blade fuse + inline fuse holder | 1 | Amazon / auto parts store | ~$3 |
| 22AWG stranded wire (assorted colors) | ~5m total | Amazon | ~$8 |
| 18AWG stranded wire (for motor leads) | ~1m | Amazon | ~$3 |
| Crimp terminals (spade + ferrule assortment) | 1 kit | Amazon | ~$10 |

#### Display Cable Hardware

| Part | Qty | Source | Est. Cost |
|------|-----|--------|-----------|
| Coiled telephone handset cord (4P4C, ~12" coiled / 7' extended) | 2 | Amazon / office supply | ~$5 each |
| RJ9/4P4C modular jacks (for ESP32 + display headers) | 4 | Amazon | ~$5 for pack |
| Rubber grommet for cable exit (~8mm ID) | 2 | hardware kit | ~$2 |

**Total estimated additional cost for back panel + routing hardware: ~$60-75** (not counting parts already on hand).

---

## Sources

- [John Guest PP1208W 1/4" Bulkhead Union — Home Depot](https://www.homedepot.com/p/John-Guest-1-4-in-Push-to-Connect-Bulkhead-Fitting-10-Pack-PP1208W-US/335236458)
- [John Guest PP1208W — Amazon](https://www.amazon.com/John-Guest-Speedfit-PP1208W-10-Pack/dp/B003YKF1SY)
- [John Guest Bulkhead Union — Official](https://www.johnguest.com/us/en/od-tube-fittings/polypropylene-white/bulkhead/bulkhead-union)
- [John Guest 1/4" 90-Degree Elbow — Home Depot](https://www.homedepot.com/p/John-Guest-1-4-in-Push-To-Connect-90-Degree-Polypropylene-Elbow-Fitting-804529/303347813)
- [John Guest PP0308W Elbow — Amazon](https://www.amazon.com/JG-Speedfit-PP0308WP-4-Inch-Union/dp/B005S4O0AY)
- [Beduan 12V 1/4" Solenoid Valve — Amazon](https://www.amazon.com/Beduan-Solenoid-Normally-Reverse-Osmosis/dp/B07NWCQJK9)
- [DaierTek DC Barrel Jack Panel Mount Waterproof — Amazon](https://www.amazon.com/5-5x2-1MM-Pre-Wired-Connector-Waterproof-Appliances/dp/B0BD46CP5Y)
- [Borsuer DC Power Jack Panel Mount — Amazon](https://www.amazon.com/Borsuer-5-5x2-1mm-Threaded-Connectors-Electronics/dp/B0GGN5MWH5)
- [Adafruit Panel Mount USB Micro-B Extension #3258](https://www.adafruit.com/product/3258)
- [IP67 Waterproof Panel-Mount USB Micro-B — DataPro](https://www.datapro.net/products/ip67-waterproof-panel-mount-usb-micro-b-2-0-male-female-extension-cable.html)
- [L298N Motor Driver Guide — Digi-Electronics](https://www.digi-electronics.com/en/blogs/l298n-motor-driver-guide-pinout-wiring-pwm-speed-control-troubleshooting/260.html)
- [L298N Motor Driver — Last Minute Engineers](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/)
- [L298N Motor Driver Specs — BYU](https://brightspotcdn.byu.edu/cd/87/bbf866d84c06a0c52fa995396f30/l298n-motor-driver-quick-start-v6.pdf)
- [The Motor Driver Myth — Rugged Circuits](https://www.rugged-circuits.com/the-motor-driver-myth)
- [Kamoer KPHM400 Datasheet — DirectIndustry](https://pdf.directindustry.com/pdf/kamoer-fluid-tech-shanghai-co-ltd/kphm400-peristaltic-pump-data-sheet/242598-1017430.html)
- [Kamoer KPHM400 — Amazon](https://www.amazon.com/peristaltic-Brushed-Kamoer-KPHM400-Liquid/dp/B09MS6C91D)
- [Stage Ninja Retractable CAT6 Cable Reel](https://www.stageninja.com/stage-ninja-products/professional-retractable-cable-reels/retractable-data-cable-reels/cat-6/cat6-25-s/)
- [Cable Science Custom Coiled Cords](https://www.cablescience.com/coils/electronic-1.html)
- [Coiled Telephone Handset Cord — ShowMeCables](https://www.showmecables.com/by-category/cables/telephone/coiled-handset-cords)
- [DIN Rail Mount DC Fuse Distribution Module — Amazon](https://www.amazon.com/Mount-Distribution-Module-12-Position-Terminal/dp/B0DXLD14LK)
- [HCDC 8-Channel DIN Rail Power Distribution — Amazon](https://www.amazon.com/Pluggable-Terminal-Distribution-HCDC-HD064/dp/B0C72HHSZG)
- [CZH-LABS 12-Position DC Power Fuse Distribution Strip](https://czh-labs.com/products/din-rail-mount-12-position-dc-power-fuse-distribution-strip-module)
- [Essentra Rubber Cable Grommets](https://www.essentracomponents.com/en-us/s/cable-grommets/rubber)
- [PrimoChill Cable/Tubing Rubber Pass-Through Grommet](https://www.primochill.com/products/primochill-3-8-inch-cable-tubing-rubber-pass-thru-grommet)
- [IEC C14 Panel Mount — BC Robotics](https://bc-robotics.com/shop/panel-mount-inlet-socket-lighted-switch-iec320-c14/)
- [Bulgin Fused IEC Inlet Snap Panel Mount](https://www.bulgin.com/us/products/fused-iec-inlet-snap-panel-mount-320-c14-pf0011-series.html)
