# Bill of Materials (BOM) -- Soda Flavor Injector

Consolidated BOM extracted from all project documentation. Prices as of March 2026.

**Status key:**
- **OWNED** -- Already purchased and in-hand
- **NEEDED** -- Must purchase for enclosure/cartridge build
- **OPTIONAL** -- Nice to have, not required for core functionality

---

## 1. Electronics

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 1.1 | ESP32-DevKitC-32E | [Amazon B09MQJWQN2](https://www.amazon.com/dp/B09MQJWQN2) | 1 | $11.00 | $11.00 | OWNED | README |
| 1.2 | ESP32 DIN Rail Breakout Board | [Amazon B0BW4SJ5X2](https://www.amazon.com/dp/B0BW4SJ5X2) | 1 | $25.99 | $25.99 | OWNED | README |
| 1.3 | L298N Dual H-Bridge Motor Driver (4-pack) | [Amazon B0C5JCF5RS](https://www.amazon.com/dp/B0C5JCF5RS) | 1 pack | $9.99 | $9.99 | OWNED | README |
| 1.4 | L298N Motor Driver (additional, for clean solenoids) | From 4-pack above | 1 | -- | -- | OWNED | layout-spatial-planning, hopper-and-bag-management |
| 1.5 | DS3231 RTC Module | (generic, I2C) | 1 | ~$3.00 | ~$3.00 | OWNED | README (pin table), layout-spatial-planning |
| 1.6 | MCP23017 I2C GPIO Expander | (generic breakout) | 1 | ~$2.00 | ~$2.00 | NEEDED | layout-spatial-planning, cartridge-envelope, dock-mounting-strategies |
| 1.7 | FDC1004 Capacitive Sensor Breakout | (I2C, for liquid/air detection) | 1 | ~$5.00 | ~$5.00 | NEEDED | cartridge-envelope, layout-spatial-planning, hopper-and-bag-management |
| 1.8 | Dupont Jumper Wires (120-pack M/F, M/M, F/F) | [Amazon B0BRTJXND9](https://www.amazon.com/dp/B0BRTJXND9) | 1 | $5.97 | $5.97 | OWNED | README |
| 1.9 | Female Spade Crimp Terminals (60-pack) | [Amazon B0B9MZJ2ML](https://www.amazon.com/dp/B0B9MZJ2ML) | 1 | $9.99 | $9.99 | OWNED | README |
| 1.10 | Male Quick Disconnect Spade Connectors (100-pack) | [Amazon B01MZZGAJP](https://www.amazon.com/dp/B01MZZGAJP) | 1 | $5.99 | $5.99 | OWNED | README |
| 1.11 | Pogo Pins (P75 or P100 series, 100-pack) | Amazon/AliExpress | 1 pack | ~$5.00 | ~$5.00 | NEEDED | electrical-mating, cartridge-envelope, dock-mounting-strategies |
| 1.12 | Nickel-Plated Brass Pads (cartridge contacts) | Hobby brass strip or small PCB from JLCPCB | 3 | ~$1.50 | ~$5.00 | NEEDED | electrical-mating, cartridge-envelope |
| 1.13 | N-channel MOSFET (IRLZ44N or similar, for hopper solenoids) | Generic logic-level MOSFET | 2 | ~$1.00 | ~$2.00 | NEEDED | hopper-and-bag-management |
| 1.14 | Flyback Diodes (1N4007 or similar, for solenoid protection) | Generic | 2 | ~$0.10 | ~$0.20 | NEEDED | hopper-and-bag-management |

**Electronics Subtotal: ~$91.14**

---

## 2. Pumps & Valves

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 2.1 | Kamoer KPHM400 Peristaltic Pump (400ml/min, 12V) | [Amazon B09MS6C91D](https://www.amazon.com/dp/B09MS6C91D) | 2 | $32.55 | $65.10 | OWNED | README, plumbing, cartridge-envelope, pump-mounting |
| 2.2 | Beduan 12V Solenoid Valve NC 1/4" (dispensing) | [Amazon B07NWCQJK9](https://www.amazon.com/dp/B07NWCQJK9) | 2 | $8.99 | $17.98 | OWNED | README, plumbing |
| 2.3 | Beduan 12V Solenoid Valve NC 1/4" (clean cycle) | [Amazon B07NWCQJK9](https://www.amazon.com/dp/B07NWCQJK9) | 2 | $8.99 | $17.98 | NEEDED | plumbing, layout-spatial-planning, back-panel-and-routing |
| 2.4 | Beduan 12V Solenoid Valve NC 1/4" (hopper fill) | [Amazon B07NWCQJK9](https://www.amazon.com/dp/B07NWCQJK9) | 2 | $8.99 | $17.98 | NEEDED | hopper-and-bag-management, layout-spatial-planning |
| 2.5 | YKEBVPW 1/4" Needle Valve Flow Control | [Amazon B0FBFVTNLM](https://www.amazon.com/dp/B0FBFVTNLM) | 1 | $7.49 | $7.49 | NEEDED | plumbing, layout-spatial-planning, back-panel-and-routing |

**Pumps & Valves Subtotal: ~$126.53**

---

## 3. Plumbing

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 3.1 | DIGITEN G3/8" Hall Effect Flow Sensor | [Amazon B07QQW4C7R](https://www.amazon.com/dp/B07QQW4C7R) | 1 | $7.99 | $7.99 | OWNED | README, layout-spatial-planning |
| 3.2 | Westbrass Cold Water Dispenser Faucet (Matte Black) | [Amazon B0BXFW1J38](https://www.amazon.com/dp/B0BXFW1J38) | 1 | $30.00 | $30.00 | OWNED | README |
| 3.3 | Platypus 2L Collapsible Bottle | [Amazon B000J2KEGY](https://www.amazon.com/dp/B000J2KEGY) | 2 | $15.94 | $31.88 | OWNED | README, plumbing, hopper-and-bag-management |
| 3.4 | Platypus Hydration Drink Tube Kit | [Amazon B07N1T6LNW](https://www.amazon.com/dp/B07N1T6LNW) | 1 | $24.95 | $24.95 | OWNED | README, plumbing |
| 3.5 | Silicone Tubing 1/8" ID x 1/4" OD (6m spool) | [Amazon B0BM4KQ6RT](https://www.amazon.com/dp/B0BM4KQ6RT) | 1 | $12.99 | $12.99 | OWNED | README, plumbing |
| 3.6 | Waterdrop 15UC-UF Inline Water Filter | [Amazon B085G9TZ4L](https://www.amazon.com/dp/B085G9TZ4L) | 1 | $62.99 | $62.99 | OWNED | README, plumbing |
| 3.7 | John Guest PP1208W Bulkhead Union 1/4" | John Guest / ice maker suppliers | 3 | ~$5.00 | ~$15.00 | NEEDED | back-panel-and-routing, layout-spatial-planning |
| 3.8 | John Guest PP0308W 90-Degree Elbow 1/4" | John Guest | 3 | ~$1.50 | ~$4.50 | NEEDED | back-panel-and-routing, layout-spatial-planning |
| 3.9 | John Guest 1/4" Union Tee (push-connect) | From ice maker kit or individually | 5 | ~$3.00 | ~$15.00 | NEEDED | back-panel-and-routing, layout-spatial-planning |
| 3.10 | John Guest 1/4" Push-Connect Fittings (dock wall) | Standard 1/4" push-to-connect | 4 | ~$2.50 | ~$10.00 | NEEDED | requirements, collet-release, dock-mounting-strategies |
| 3.11 | 1/4" OD Hard Tubing (nylon/polyethylene) | From ice maker kit (~25 ft spool) | ~1.2m needed | (from kit) | ~$5.00 | NEEDED | back-panel-and-routing, plumbing, cartridge-envelope |
| 3.12 | Barb Reducer Fittings (BPT-to-1/4" transition) | Brass or nylon barb, 1/4" x 3/16" | 4 | ~$1.50 | ~$6.00 | NEEDED | cartridge-envelope, pump-mounting |
| 3.13 | Ice Maker Installation Kit (tee fittings, hard tubing) | Generic, hardware store | 1 | ~$15.00 | ~$15.00 | OWNED | plumbing (noted "already on hand") |

**Plumbing Subtotal: ~$241.30**

---

## 4. Mechanical / Hardware

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 4.1 | Heat-Set M3 Brass Knurled Inserts (M3x4 or M3x5) | Generic (pack of 50-100) | ~30 needed | ~$0.05 | ~$5.00 | NEEDED | pump-mounting, cartridge-envelope, layout-spatial-planning, dock-mounting-strategies |
| 4.2 | M3 Socket Head Cap Screws (various lengths) | Generic assortment | ~30 | ~$0.05 | ~$3.00 | NEEDED | pump-mounting, dock-mounting-strategies |
| 4.3 | Steel Dowel Pins 3mm dia (release plate guides) | McMaster or Amazon | 4 | ~$0.50 | ~$2.00 | NEEDED | cartridge-envelope, collet-release |
| 4.4 | Tapered Alignment Pins (8-10mm base, 15-20 deg taper) | 3D printed or purchased steel taper pins | 2 | ~$1.00 | ~$2.00 | NEEDED | guide-alignment, cartridge-envelope |
| 4.5 | Rubber Grommet Isolators (~6-8mm OD, neoprene) | Generic, for pump vibration isolation | 8 | ~$0.15 | ~$1.20 | NEEDED | pump-mounting, dock-mounting-strategies |
| 4.6 | Push Rod (~3mm steel rod, ~118mm length) | Steel rod stock | 1 | ~$1.00 | ~$1.00 | NEEDED | cartridge-envelope |
| 4.7 | Zip Ties (200-pack) | [Amazon B0BC1VH4XB](https://www.amazon.com/dp/B0BC1VH4XB) | 1 | $3.99 | $3.99 | OWNED | README, plumbing |
| 4.8 | #8 x 1/2" Wood Screws (100-pack) | [Home Depot 204275505](https://www.homedepot.com/p/204275505) | 1 | $6.87 | $6.87 | OWNED | README |
| 4.9 | 12"x24" Laminate Shelf (original mounting panel) | [Home Depot 328395734](https://www.homedepot.com/p/328395734) | 1 | $12.98 | $12.98 | OWNED | README |
| 4.10 | Small Hose Clamps (for BPT-to-hard tube transitions) | Generic, small diameter | 8 | ~$0.30 | ~$2.40 | NEEDED | pump-mounting |
| 4.11 | Lock Washers / Nyloc Nuts (M3, for pump mounting) | Generic assortment | ~16 | ~$0.05 | ~$0.80 | NEEDED | pump-mounting |

**Mechanical / Hardware Subtotal: ~$41.24**

---

## 5. Enclosure Materials

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 5.1 | PETG Filament (dark navy or matte black, 1kg spool) | Generic (Bambu, Overture, etc.) | ~2 spools | ~$20.00 | ~$40.00 | NEEDED | layout-spatial-planning, front-face-interaction-design, pump-mounting |
| 5.2 | Rubber Bumper Feet (12-15mm dia, 6-8mm tall, 40-60A) | Amazon, pack of 4-8 | 4 | ~$0.50 | ~$2.00 | NEEDED | dock-mounting-strategies |
| 5.3 | Rubber Grommets (tube pass-through, ~10mm ID) | Generic, for shelf pass-throughs | 4 | ~$0.25 | ~$1.00 | NEEDED | dock-mounting-strategies |
| 5.4 | DIN Rail (~200mm length) | Generic 35mm DIN rail | 1 | ~$3.00 | ~$3.00 | NEEDED | layout-spatial-planning |
| 5.5 | DIN Rail Terminal Blocks | Screw terminal, for power distribution | ~4 | ~$1.00 | ~$4.00 | NEEDED | layout-spatial-planning |

**Enclosure Materials Subtotal: ~$50.00**

---

## 6. Displays & UI

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 6.1 | Waveshare RP2040-LCD-0.99 Round Display | [Amazon B0CTSPYND2](https://www.amazon.com/dp/B0CTSPYND2) | 1 | $23.99 | $23.99 | OWNED | README |
| 6.2 | Meshnology ESP32-S3 1.28" Round Rotary Display | [Amazon B0G5Q4LXVJ](https://www.amazon.com/dp/B0G5Q4LXVJ) | 1 | $47.76 | $47.76 | OWNED | README |
| 6.3 | KRAUS Garbage Disposal Air Switch (Matte Black) | [Amazon B096319GMV](https://www.amazon.com/dp/B096319GMV) | 1 | $39.95 | $39.95 | OWNED | README |
| 6.4 | 7mm Momentary Push Buttons (12-pack) | [Amazon B0F43GYWJ6](https://www.amazon.com/dp/B0F43GYWJ6) | 1 | $7.39 | $7.39 | OWNED | README |
| 6.5 | Adafruit 4-Pin Magnetic Pogo Connector (pair) | [Adafruit #5358](https://www.adafruit.com/product/5360), $4.95/pair | 2 pairs | $4.95 | $9.90 | NEEDED | front-face-interaction-design, layout-spatial-planning |
| 6.6 | 6x3mm N52 Neodymium Disc Magnets (display retention) | Generic, pack of 20+ | ~14 | ~$0.15 | ~$2.10 | NEEDED | front-face-interaction-design, layout-spatial-planning |
| 6.7 | Steel Discs (matching magnets, for display adapters) | Generic, 6mm dia | ~7 | ~$0.10 | ~$0.70 | NEEDED | front-face-interaction-design |
| 6.8 | Cat6 Cable (slim, 28AWG preferred, for display UART) | Standard or slim cat6 | ~2m | ~$3.00 | ~$3.00 | NEEDED | front-face-interaction-design |
| 6.9 | Status LEDs (power + cartridge status) | Generic 3mm or 5mm LED | 2 | ~$0.10 | ~$0.20 | NEEDED | layout-spatial-planning |

**Displays & UI Subtotal: ~$134.99**

---

## 7. Power

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 7.1 | 12V 2A Power Supply (current, may need upgrade) | [Amazon B0DZGTTBGZ](https://www.amazon.com/dp/B0DZGTTBGZ) | 1 | $9.99 | $9.99 | OWNED | README |
| 7.2 | 12V 3A Power Supply (upgrade for full enclosure) | Generic barrel jack PSU | 1 | ~$12.00 | ~$12.00 | NEEDED | layout-spatial-planning |
| 7.3 | 5.5x2.1mm Panel-Mount Barrel Jack | Generic, panel mount | 1 | ~$1.50 | ~$1.50 | NEEDED | back-panel-and-routing, layout-spatial-planning |
| 7.4 | 3A Blade Fuse + Inline Holder | Generic automotive blade fuse | 1 | ~$3.00 | ~$3.00 | NEEDED | layout-spatial-planning |

**Power Subtotal: ~$26.49**

---

## 8. Consumables & Supplies

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 8.1 | SodaStream Pepsi Wild Cherry Zero Sugar (4-pack) | [Amazon B0G4NRDQB8](https://www.amazon.com/dp/B0G4NRDQB8) | 1 | $28.99 | $28.99 | OWNED | README |
| 8.2 | SodaStream Diet MTN Dew (4-pack) | [Amazon B0CS191QMW](https://www.amazon.com/dp/B0CS191QMW) | 1 | ~$29.00 | ~$29.00 | OWNED | README |
| 8.3 | Food-Grade Silicone Funnels (75-100mm opening) | Kitchen funnel or silicone, 2 needed | 2 | ~$5.00 | ~$10.00 | NEEDED | hopper-and-bag-management, layout-spatial-planning |
| 8.4 | Silicone Plug/Cap with Lanyard (hopper caps) | Generic silicone plug, ~25-30mm | 2 | ~$2.00 | ~$4.00 | NEEDED | hopper-and-bag-management |
| 8.5 | 22 AWG Stranded Wire (silicone jacket) | Generic, assorted colors | ~5m | ~$5.00 | ~$5.00 | NEEDED | pump-mounting, dock-mounting-strategies |
| 8.6 | Heatshrink Tubing (assorted) | Generic | 1 kit | ~$5.00 | ~$5.00 | NEEDED | pump-mounting |
| 8.7 | Panel-Mount USB Micro-B Extension | Generic panel mount | 1 | ~$4.00 | ~$4.00 | OPTIONAL | back-panel-and-routing |
| 8.8 | Rubber Grommet (air switch tube pass-through, 3-4mm ID) | Generic | 1 | ~$0.25 | ~$0.25 | NEEDED | back-panel-and-routing |

**Consumables & Supplies Subtotal: ~$86.24**

---

## 9. Infrastructure (Carbonated Water System)

| # | Description | Part / Link | Qty | Unit Cost | Ext. Cost | Status | Source(s) |
|---|-------------|-------------|-----|-----------|-----------|--------|-----------|
| 9.1 | Lilium Under-Sink Carbonated Water Dispenser | [liliumfaucet.com](https://liliumfaucet.com/products/under-sink-carbonated-soda-maker-sparkling-water-dispenser-with-3-way-faucet) | 1 | $1,039.00 | $1,039.00 | OWNED | README |
| 9.2 | TAPRITE Dual-Gauge CO2 Regulator | [Amazon B00L38DRD0](https://www.amazon.com/dp/B00L38DRD0) | 1 | $92.95 | $92.95 | OWNED | README |
| 9.3 | 5 lb CO2 Tank + First Refill | Local welding/homebrew shop | 1 | $139.00 | $139.00 | OWNED | README |

**Infrastructure Subtotal: ~$1,270.95**

---

## Category Subtotals

| Category | Subtotal |
|----------|----------|
| 1. Electronics | ~$91.14 |
| 2. Pumps & Valves | ~$126.53 |
| 3. Plumbing | ~$241.30 |
| 4. Mechanical / Hardware | ~$41.24 |
| 5. Enclosure Materials | ~$50.00 |
| 6. Displays & UI | ~$134.99 |
| 7. Power | ~$26.49 |
| 8. Consumables & Supplies | ~$86.24 |
| 9. Infrastructure | ~$1,270.95 |
| **Grand Total** | **~$2,068.88** |

### Cost Breakdown by Status

| Status | Estimated Cost |
|--------|---------------|
| OWNED (already purchased) | ~$1,789.42 |
| NEEDED (must buy) | ~$275.46 |
| OPTIONAL | ~$4.00 |

---

## Ambiguities and Notes

1. **Power Supply (7.1 vs 7.2):** The current 12V 2A PSU (OWNED) is noted as "borderline for peak loads" in layout-spatial-planning.md. A 12V 3A PSU is recommended. You may keep the 2A and upgrade only if problems arise, or buy the 3A upfront. Both are listed.

2. **L298N Quantity:** The README lists a 4-pack purchased. The current system uses 2 boards. The enclosure design calls for a 3rd (for clean solenoids). A 4th may be needed if hopper solenoids are driven by L298N rather than individual MOSFETs. The 4-pack covers 3; the 4th is spare or for hopper use. The hopper-and-bag-management doc recommends MOSFETs instead of a 4th L298N.

3. **Solenoid Valve Total:** 2 owned (dispensing) + 2 needed (clean) + 2 needed (hopper) = 6 total Beduan valves. All the same part number. The hopper valves could be deferred if hopper fill is manual initially.

4. **Hopper Funnels (8.3):** Material recommendation varies: food-grade silicone preferred for production, PETG acceptable for prototyping. If 3D printing the funnels, no separate purchase needed (just filament). Silicone funnels are listed as a separate purchase.

5. **John Guest Fittings Sourcing:** The 5 tees, 3 bulkheads, 3 elbows, and 4 dock fittings total 15 push-connect fittings needed. Some tees may already be on hand from the ice maker kit. The ice maker kit is listed as OWNED but exact contents vary by kit.

6. **DS3231 RTC (1.5):** Referenced in ESP32 pin assignments (GPIO 21/22 I2C) and layout-spatial-planning component list but never appears in the README cost breakdown. Listed as OWNED based on wiring being in place. Cost is estimated.

7. **DIN Rail (5.4):** The ESP32 DIN Rail Breakout Board (1.2) is OWNED. A separate DIN rail segment for mounting inside the enclosure is needed. The existing system mounts on a laminate shelf; the enclosure design mounts on DIN rail.

8. **Filament Quantity (5.1):** Estimated ~2 spools for all printed parts (enclosure shell, dock shelf, cartridge body, pump tray, lid, display holders, hopper mounts). Actual consumption depends on infill settings and iteration count. Could be 1.5-3 spools.

9. **Cat6 vs 4-Conductor Cable (6.8):** The front-face-interaction-design doc notes that only 4 conductors are needed (UART TX, RX, VCC, GND). A thinner 4-conductor cable could replace cat6. Cat6 is currently in use and convenient.

10. **Magnetic Pogo Connector (6.5):** Listed as Adafruit #5358 (4-pin right-angle) in the front-face doc, but Adafruit #5360 (3-pin) is also referenced. The 4-pin version ($4.95/pair) is needed for TX, RX, VCC, GND. Confirm pin count before ordering.
