# Spatial Layout — Master Coordinate Table

Reference document for CAD agents. All positions in millimeters. Current design: 220mm wide, 10-valve topology.

---

## 1. Coordinate System

| Axis | Direction | Range |
|------|-----------|-------|
| **X** | Width, left to right | 0 to 220 (exterior) |
| **Y** | Depth, front to back | 0 to 300 (exterior) |
| **Z** | Height, bottom to top | 0 to 400 (exterior) |

**Origin:** exterior front-bottom-left corner of enclosure.

**Interior bounds (4mm walls):** X = 4 to 216, Y = 4 to 296, Z = 4 to 396.

Interior usable: 212W x 292D x 392H mm.

---

## 2. Master Component Table

### 2a. Enclosure Shell

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Exterior shell | 0 | 220 | 0 | 300 | 0 | 400 | Full enclosure boundary |
| Interior cavity | 4 | 216 | 4 | 296 | 4 | 396 | 4mm walls all sides |
| Front panel | 0 | 220 | 0 | 4 | 0 | 400 | Front wall |
| Back panel | 0 | 220 | 296 | 300 | 0 | 400 | Rear wall |
| Left wall | 0 | 4 | 0 | 300 | 0 | 400 | |
| Right wall | 216 | 220 | 0 | 300 | 0 | 400 | |
| Floor | 0 | 220 | 0 | 300 | 0 | 4 | |
| Ceiling / top panel | 0 | 220 | 0 | 300 | 396 | 400 | Removable; hopper hole centered |

### 2b. Cartridge Dock Zone

Cartridge is 148W x 130D x 80H mm, centered in 212mm interior width.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Cartridge envelope | 36 | 184 | 4 | 134 | 4 | 84 | Centered in X; sits on floor |
| Cartridge slot opening (front panel) | 36 | 184 | 0 | 4 | 0 | 84 | 148W x 84H cutout in front wall |
| Chamfered entrance zone | 31 | 189 | 0 | 24 | 0 | 94 | 5mm chamfer, 20mm deep funnel (estimated) |
| Dock floor rails (2x) | 39, 178 | 42, 181 | 4 | 134 | 4 | 6 | 3mm wide x 2mm tall, full depth (estimated) |
| Dock side guides (2x) | 36, 183 | 37.5, 184.5 | 4 | 134 | 4 | 84 | 1.5mm wide rails (estimated) |
| Dock back wall | 36 | 184 | 134 | 169 | 4 | 84 | ~35mm thick; holds JG bulkhead fittings + pogo mount |

### 2c. Valve Rack Zone

10 Beduan solenoid valves: 63.5L x 34.0W x 58.4H mm each. Arranged 5-wide x 2-high. Valve long axis along Y (depth). Pitch in X: 36mm (34mm body + 2mm gap). 5 valves at 36mm pitch = 180mm span, centered in 212mm interior.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Valve rack frame | 20 | 200 | 169 | 233 | 4 | 125 | Behind dock back wall |
| Row 1 (bottom, 5 valves) | 20 | 200 | 169 | 233 | 4 | 62 | 58.4mm valve height |
| Row 2 (top, 5 valves) | 20 | 200 | 169 | 233 | 66 | 125 | 4mm gap between rows |
| Valve 1 (row 1, leftmost) | 20 | 56 | 169 | 233 | 4 | 62 | 36mm pitch |
| Valve 2 | 56 | 92 | 169 | 233 | 4 | 62 | |
| Valve 3 | 92 | 128 | 169 | 233 | 4 | 62 | |
| Valve 4 | 128 | 164 | 169 | 233 | 4 | 62 | |
| Valve 5 (row 1, rightmost) | 164 | 200 | 169 | 233 | 4 | 62 | |
| Valve 6 (row 2, leftmost) | 20 | 56 | 169 | 233 | 66 | 125 | |
| Valve 7 | 56 | 92 | 169 | 233 | 66 | 125 | |
| Valve 8 | 92 | 128 | 169 | 233 | 66 | 125 | |
| Valve 9 | 128 | 164 | 169 | 233 | 66 | 125 | |
| Valve 10 (row 2, rightmost) | 164 | 200 | 169 | 233 | 66 | 125 | |

Note: Y span = 63.5mm valve body length + ~0.5mm clearance. Actual Y range 169-233 = 64mm. QC fittings protrude beyond valve body on both Y faces; tube routing space extends forward to dock back wall (Y=134-169) and rearward toward bags.

### 2d. Bag Cradle Zone

Two 2L Platypus bags (each 190W x 350L, lens-shaped cross-section peaking at ~40mm thick per bag) stacked on a diagonal cradle at 35 degrees from horizontal. Sealed ends pinned to back wall at top; cap/connector ends at front-low.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Cradle structure | 10 | 210 | 29 | 296 | 125 | 396 | 200mm wide cradle, 6mm margin per side |
| Bag cap end (front-low) | 15 | 205 | 29 | 60 | 125 | 155 | Connector zone; ~30mm thick (estimated) |
| Bag midpoint (thickest) | 15 | 205 | 140 | 180 | 240 | 320 | ~80mm two-bag stack (estimated) |
| Bag sealed end (top-back) | 15 | 205 | 270 | 296 | 370 | 396 | ~2mm thick, pinned to back wall |
| Back-wall bag pin/clamp | 55 | 165 | 288 | 296 | 380 | 396 | Holds sealed ends against wall (estimated) |

Diagonal line equation (approximate center of bag stack): Z = 125 + (Y - 29) * tan(35 deg) = 125 + 0.70 * (Y - 29).

### 2e. Hopper Zone

Single curved funnel at top-front of enclosure, above the bag sealed ends. Shared by both flavors.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Hopper funnel body | 60 | 160 | 8 | 78 | 326 | 396 | ~100mm dia, ~70mm tall (estimated) |
| Hopper top opening | 60 | 160 | 8 | 78 | 396 | 400 | 100mm dia hole in top panel |
| Hopper outlet | 106 | 114 | 40 | 48 | 326 | 330 | ~10mm dia, bottom of funnel (estimated) |

### 2f. Electronics Zone

All electronics in top-back corner, above and behind the diagonal bags. Isolated from fluid paths.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Electronics zone | 4 | 216 | 200 | 296 | 326 | 396 | Above bags, back half (estimated) |
| ESP32 + MCP23017 + L298N | 4 | 120 | 220 | 296 | 340 | 396 | Main controller cluster (estimated) |
| PSU (12V + 5V/3.3V) | 120 | 216 | 250 | 296 | 340 | 396 | Near IEC inlet on back panel (estimated) |
| DS3231 RTC | 4 | 40 | 220 | 250 | 340 | 360 | I2C, small board (estimated) |

### 2g. Display Reel Zone

Two retractable flat cat6 reels in front panel, mid-height. Each reel ~55mm diameter x ~22mm deep.

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Reel 1 (left display, RP2040) | 32 | 87 | 4 | 26 | 248 | 303 | 55mm dia, 22mm deep (estimated) |
| Reel 2 (right display, S3) | 133 | 188 | 4 | 26 | 248 | 303 | Symmetric with reel 1 (estimated) |
| Dock recess 1 (front panel) | 32 | 82 | 0 | 5 | 250 | 300 | 50mm dia x 5mm deep magnetic puck seat |
| Dock recess 2 (front panel) | 138 | 188 | 0 | 5 | 250 | 300 | Symmetric (estimated) |
| Cable exit hole 1 | 55 | 63 | 0 | 4 | 273 | 281 | ~8mm dia for flat cat6 |
| Cable exit hole 2 | 159 | 167 | 0 | 4 | 273 | 281 | Symmetric |

### 2h. Back Panel Connections

Viewed from the rear (mirrored X). Positions below are in standard coordinate system (from front-left).

| Component | X min | X max | Y min | Y max | Z min | Z max | Notes |
|-----------|-------|-------|-------|-------|-------|-------|-------|
| Tap water inlet (1/4" JG bulkhead) | 160 | 180 | 296 | 300 | 30 | 50 | Lower zone, right from rear (estimated) |
| Soda water in (1/4" JG bulkhead) | 100 | 120 | 296 | 300 | 30 | 50 | Lower zone, center (estimated) |
| Soda water out (1/4" JG bulkhead) | 60 | 80 | 296 | 300 | 30 | 50 | Lower zone, center-left (estimated) |
| Flow meter (DIGITEN, inline) | 60 | 124 | 270 | 296 | 30 | 68 | 63.5L x 30.5W x 38.1H, between soda in/out (estimated) |
| Flavor line 1 exit (PG7/PG9 gland) | 170 | 190 | 296 | 300 | 150 | 170 | Mid zone (estimated) |
| Flavor line 2 exit (PG7/PG9 gland) | 30 | 50 | 296 | 300 | 150 | 170 | Mid zone (estimated) |
| IEC C14 power inlet | 30 | 80 | 296 | 300 | 340 | 380 | Upper zone (estimated) |

---

## 3. Key Interface Points

Specific XYZ coordinates where components connect or mate.

### 3a. Dock Tube Stub Positions

Four 1/4" OD hard nylon tube stubs protrude from the dock back wall (Y=134-169). These are the enclosure-side bare stubs that insert into JG fittings carried by the cartridge. Arranged in a 2x2 grid, 15mm center-to-center.

| Stub | X | Y | Z | Connection |
|------|---|---|---|------------|
| Pump 1 inlet | 90 | 151 | 30 | b1 → i1 (direct, no valve) (estimated) |
| Pump 1 outlet | 90 | 151 | 45 | o1 (to valves v1, v6) (estimated) |
| Pump 2 inlet | 130 | 151 | 30 | b2 → i2 (direct, no valve) (estimated) |
| Pump 2 outlet | 130 | 151 | 45 | o2 (to valves v2, v8) (estimated) |

Stubs protrude ~30mm rearward from dock back wall face (Y=151 is mid-wall). The cartridge's JG fittings engage these stubs on insertion.

### 3b. Pogo Pin Contact Point

| Parameter | Value |
|-----------|-------|
| X center | 110 (centered in cartridge width) (estimated) |
| Y | 140-150 (dock ceiling, near back wall) (estimated) |
| Z | 84 (dock ceiling = cartridge top face) |
| Pin row | 3 pins at 10mm pitch: X = 100, 110, 120 (estimated) |
| Contact order | GND, Motor A 12V, Motor B 12V |

Pins are spring-loaded, pressing downward from dock ceiling onto flat pads on cartridge top face.

### 3c. Hopper Outlet Position

| Parameter | Value |
|-----------|-------|
| X | 110 (centered) (estimated) |
| Y | 44 (front-biased) (estimated) |
| Z | 328 (bottom of funnel) (estimated) |
| Fitting | 1/4" barb, connects to tube routed to valves v5/v7 |

### 3d. Valve Inlet/Outlet Positions

QC fittings on each valve. Front-facing ports (Y=169) connect to dock/bag/hopper tubes. Rear-facing ports (Y=233) connect to bags, dispense lines, and back panel.

| Valve | X center | Z center | Front port (Y=169) | Rear port (Y=233) |
|-------|----------|----------|---------------------|---------------------|
| v1 | 38 | 33 | from o1 | to d1 (dispense line 1) |
| v2 | 74 | 33 | from o2 | to d2 (dispense line 2) |
| v3 | 110 | 33 | from w1 | to b1 (bag 1 fill) |
| v4 | 146 | 33 | from w2 | to b2 (bag 2 fill) |
| v5 | 182 | 33 | from h1 | to i1 (pump 1 inlet) |
| v6 | 38 | 96 | from o1 | to b1 (bag 1 reverse fill) |
| v7 | 74 | 96 | from h2 | to i2 (pump 2 inlet) |
| v8 | 110 | 96 | from o2 | to b2 (bag 2 reverse fill) |
| v9 | 146 | 96 | from t1 | to i1 (dip tube 1 evacuation) |
| v10 | 182 | 96 | from t2 | to i2 (dip tube 2 evacuation) |

All positions estimated. Valve assignment to grid positions is logical grouping, not yet physically validated.

### 3e. Back Panel Fitting Positions (approximate centers)

| Fitting | X | Y | Z | Notes |
|---------|---|---|---|-------|
| Tap water inlet | 170 | 298 | 40 | 1/4" JG bulkhead (estimated) |
| Soda water in | 110 | 298 | 40 | 1/4" JG bulkhead (estimated) |
| Soda water out | 70 | 298 | 40 | 1/4" JG bulkhead (estimated) |
| Flavor line 1 exit | 180 | 298 | 160 | PG7/PG9 cable gland (estimated) |
| Flavor line 2 exit | 40 | 298 | 160 | PG7/PG9 cable gland (estimated) |
| IEC C14 power | 55 | 298 | 360 | Panel-mount inlet with fuse (estimated) |

### 3f. Bag Connection Points

| Point | X | Y | Z | Notes |
|-------|---|---|---|-------|
| Bag 1 cap (P1 + P2) | 80 | 29 | 130 | Front-low position, fluid + dip tube ports (estimated) |
| Bag 2 cap (P1 + P2) | 140 | 29 | 130 | Front-low position (estimated) |
| Bag 1 sealed end (pin) | 80 | 290 | 388 | Pinned flat to back wall (estimated) |
| Bag 2 sealed end (pin) | 140 | 290 | 388 | Pinned flat to back wall (estimated) |

---

## 4. Zone Clearance Summary

Vertical stack at the front wall (Y=4):

| Z range | Zone |
|---------|------|
| 4 - 84 | Cartridge dock |
| 84 - 125 | Open / tube routing between dock and bag cap end |
| 125 - 155 | Bag cap / connector end |
| 155 - 248 | Bag slab (diagonal, moving rearward as Z increases) |
| 248 - 303 | Display reels |
| 303 - 326 | Open |
| 326 - 396 | Hopper funnel |

Depth stack at the floor (Z=4):

| Y range | Zone |
|---------|------|
| 4 - 134 | Cartridge dock |
| 134 - 169 | Dock back wall |
| 169 - 233 | Valve rack |
| 233 - 296 | Tube routing to bags / back panel |
