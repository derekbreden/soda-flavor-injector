# Enclosure — Functional Requirements

## What the enclosure is

The main body of a commercial under-sink flavor injection system. It houses two permanent flavor bags on a diagonal cradle, a removable pump cartridge, a hopper funnel for refilling bags, valve assemblies for flow routing, displays, and all electronics. It sits in a side zone of an under-sink cabinet, beside the plumbing center zone.

## Environment

- Under a kitchen sink — limited visibility, awkward reach, possibly wet
- Side zone of cabinet: 280-330mm wide (33"+ US), 480-510mm usable depth, 380-420mm under the sink bowl
- Competing for space with P-trap, supply lines, possibly RO filter or disposal
- Must be stable on a flat cabinet floor; not wall-mounted

## Layout Architecture: Diagonal Interleave (Vision 1)

Components share vertical and depth space rather than stacking in horizontal zones. Bags stretch diagonally from top-front to bottom-back. The cartridge sits in the front-bottom void below the bags. The hopper sits at the top-front. Electronics occupy the top-back corner. See `diagonal-interleave.md` for the full spatial vision.

## Enclosure Dimensions

| Dimension | Exterior | Interior (4mm walls) | Notes |
|---|---|---|---|
| Width | 280mm | 272mm | Fits 33"+ US and 800mm+ EU side zones |
| Depth | 300mm | 292mm | Intentionally constrained; research at 2L bag angles is ongoing |
| Height | 400mm | 392mm | Fits typical under-sink clearance (380-420mm) |

**On depth:** Research indicates 2L bags at 35-40° need ~319-333mm of depth, which exceeds the 292mm interior. The 300mm constraint is intentional — the assumption that 2L bags require 350mm depth at moderate angles is being questioned. Future research should explore whether bag compression, mounting geometry, or slight angle adjustments can make 2L bags work at 300mm depth. Depth is the unconstrained dimension under sinks (480-510mm available), so increasing to 350mm remains a fallback.

## Bags

- **2 bags, one per flavor line**
- **2-liter Platypus Platy bags** (350mm long × 190mm wide, ~40mm compressed thickness per bag, ~80mm stacked)
- **Permanent fixtures** — installed once during manufacturing, never touched by the user
- Mounted on a diagonal cradle, sealed end at top-front, connector end at bottom-back
- Connector end at the low point for gravity-assisted drainage
- Refilled via the hopper funnel, not by removing or handling the bags

## Hopper Funnel

- Sits at the top of the enclosure, at the very front
- The primary user interaction for refilling — user pours concentrated flavor syrup into the funnel
- Single funnel for both flavors; flavor routing controlled by valves and firmware (user selects which bag to fill via the display or a button)
- Small capacity (~200-300ml) — functions as a funnel, not a reservoir; pump-assisted fill drains it as the user pours
- Removable for cleaning (dishwasher-safe silicone)
- Refill frequency: weekly for families, monthly for moderate users

## Pump Cartridge

- Slides into a dock at the **front-bottom** of the enclosure, below the diagonal bags
- Contains **2 Kamoer peristaltic pumps** (one per flavor line)
- The only user-serviceable internal component — replaced every 18-36 months when pump tubes wear out
- Front-loading: user opens front panel, slides cartridge out, slides new one in
- See `cartridge/planning/requirements.md` for cartridge-specific requirements

## Valves

- **4 valves per pump (8 total), mounted in the main body of the enclosure** (not in the cartridge)
- Each pump's 4 valves control flow routing:
  - Hopper → pump → bag (fill mode)
  - Bag → pump → dispensing line (dispense mode)
- Valve type TBD (solenoid pinch valves likely — research needed)
- Valves are firmware-controlled; the user never interacts with them directly

## Displays

- **2 round displays** mounted on the front face of the enclosure
  - One driven by ESP32-S3
  - One driven by RP2040
- **Detachable** — each display connects via a **retracting cat6 cable (1 meter length)**
- The displays can be pulled out from under the sink and placed on a countertop, mounted to a cabinet face, or attached magnetically to a fridge — wherever the user wants visual feedback
- When retracted, displays sit flush or near-flush on the enclosure front face
- Cat6 provides power + data over a single cable (specific pinout TBD)

## Back Panel

All external fluid and electrical connections enter/exit through the back of the enclosure:

| Connection | Type | Purpose |
|---|---|---|
| Tap water inlet | 1/4" push-to-connect | Fresh water supply for rinsing/cleaning cycles |
| Carbonated soda water inlet | 1/4" push-to-connect | From the carbonation system upstream |
| Carbonated soda water outlet | 1/4" push-to-connect | Returns to the soda line downstream |
| 120V AC power | IEC C14 inlet or hardwired cord | Mains power for PSU, pumps, electronics |

**The carbonated soda water inlet and outlet are for a flow meter only.** Soda water passes straight through the enclosure via a flow meter that measures volume dispensed. No flavor mixing happens inside the enclosure on the soda water line.

## Flavor Dispensing

- **Two silicone tube exits** route from the enclosure up to the faucet area
- Each tube carries one flavor
- Flavor is dispensed at the faucet, synchronized with soda water flow (measured by the internal flow meter)
- Tube routing from enclosure to faucet is external to the enclosure (through cabinet, up to countertop)

## Electronics

- ESP32-S3: main controller, drives one display, controls valves and pumps
- RP2040: drives the second display
- L298N motor drivers: pump control at 12V via PWM (in the main body, not the cartridge)
- Solenoid drivers for 8 valves
- Flow meter signal input
- Power supply: 120V AC → 12V DC (for pumps) + 5V/3.3V (for logic)
- Located in the **top-back corner** of the enclosure, above and behind the diagonal bags
- Must be isolated from fluid paths (splash protection, vertical separation)

## Access Architecture

- **Slide-out tray** preferred — the entire enclosure slides forward on the tray for access
- When slid out, the top panel opens or removes to expose the hopper funnel for pouring
- Front panel opens or removes to expose the cartridge slot
- No tools required for any user-facing operation (refill or cartridge swap)

## Key Constraints

- **Bags are permanent.** The user never removes, replaces, or handles the bags.
- **Cartridge is the only replaceable module.** Everything else is serviced by the manufacturer or is maintenance-free.
- **All user operations happen from the front or top** — never the back or sides (those face cabinet walls).
- **Food-safe materials** on all fluid-contact surfaces (silicone tubing, PP/Tritan funnels, food-grade bag material).
- **No flavor mixing inside the enclosure on the soda water line.** Soda water passes through for metering only. Flavor exits separately to the faucet.

## Research

Enclosure research documents exploring spatial layout, component placement, and design trade-offs:

### Vision 1 Research (current direction)
- [Diagonal Interleave Vision](diagonal-interleave.md) — the guiding spatial concept
- [Diagonal Bag Placement](research/v1-diagonal-bag-placement.md) — angle sweeps, enclosure fits, mounting
- [Hopper Integration](research/v1-hopper-integration.md) — funnel design, fill routing, two-flavor management
- [Master Spatial Layout](research/v1-master-spatial-layout.md) — synthesized layout with coordinates and cross-sections

### General Research (layout-independent)
- [Under-Sink Constraints](research/under-sink-constraints.md) — real cabinet dimensions across markets
- [Bag Dimensions Survey](research/bag-dimensions-survey.md) — corrected Platypus bag measurements
- [Diagonal Stacking Geometry](research/diagonal-stacking-geometry.md) — angle/dimension sweep tables
- [Diagonal Risks and Failure Modes](research/diagonal-risks-and-failure-modes.md) — devil's advocate analysis
- [Access Architecture](research/access-architecture.md) — comparison of access approaches
- [Dip Tube Analysis](research/dip-tube-analysis.md) — fluid dynamics of the dip tube
- [Drip Tray Shelf Analysis](research/drip-tray-shelf-analysis.md) — leak scenario analysis
- [Pump-Assisted Filling](research/pump-assisted-filling.md) — hopper-to-bag fill topology
- [Back Panel and Routing](research/back-panel-and-routing.md) — external connections and tube routing
