# Plumbing

## Tubing Types

Two types of tubing are used throughout the system:

- **Soft silicone tubing** (1/8" ID x 1/4" OD, food-grade, black) — used for flavoring concentrate lines. Flexible, easy to route, works with peristaltic pumps. Purchased as a 6m spool from Amazon (B0BM4KQ6RT).
- **Hard RO tubing** (1/4" OD, white/clear) — used for water lines (filtered tap water, cold water, carbonated water). Rigid, holds well in push-connect fittings. Sourced from an ice maker installation kit.

## Connection Methods

### Push-Connect Fittings

Push-connect (quick-connect) fittings are used on solenoid valves, tee splitters, elbows, and manual valves. They accept 1/4" OD tubing — push the tube in, the collet grabs it.

**Hard tubing works well in push-connect fittings.** The rigid tube seats properly and the O-ring seals against the smooth, firm outer surface.

**Soft silicone does NOT hold in push-connect fittings.** The silicone deforms under the collet grip and slips out. Never push silicone directly into a push-connect fitting.

### Silicone-to-Push-Connect Transition

To connect soft silicone tubing to a push-connect fitting, use hard tubing as an intermediary:

```
[push-connect fitting] ← hard tube → inserted into soft silicone ← zip tie
```

1. Push one end of a short piece of hard 1/4" OD tubing into the push-connect fitting.
2. Insert the other end of the hard tube *into* the soft silicone tubing (the hard tube's OD matches the silicone's ID, creating a snug interference fit).
3. Zip-tie the silicone over the hard tube to prevent slippage.

The interference fit of the hard tube stretching the silicone creates the seal. The zip ties are more about preventing the tube from sliding off than about sealing — the fit itself is what keeps liquid in.

### Platypus Bag Connection

The Platypus Hydration Drink Tube Kit (B07N1T6LNW) comes with blue tubing that has a slightly larger ID than the black silicone's OD. The black silicone slides *inside* the blue tube, creating an overlap/sleeve joint of a couple inches. Zip ties compress the blue tube onto the inner black tube to hold and seal.

Zip tie torque on this joint is a balancing act:

- **Too loose** — leaking under backpressure (see failure mode below).
- **Too tight** — collapses the inner black silicone tube, restricting or blocking flow. Possible but hasn't been an ongoing issue.

## Current Flavoring Line Path

Each of the two flavor lines follows this path:

```
Platypus bag
  → blue tube (from Platypus drink tube kit)
    → zip tie joint (blue over black silicone)
      → 1/4" OD black silicone tubing
        → hard tube transition + zip tie
          → [dispensing solenoid valve, Beduan 12V NC, 1/4" push-connect]
            → [peristaltic pump, Kamoer 400ml/min 12V]
              → dispensing point (faucet gooseneck)
```

The two flavor lines are independent and identical in layout. An air switch toggles which flavor is active.

## Water Supply

The Lilium under-sink carbonated water machine has three 1/4" push-connect outputs:

- **"Cold carbonated water"** — currently in use for the soda dispensing system.
- **"Tap water"** — filtered, room temperature (~55-65°F). Available for clean cycle.
- **"Cold water"** — filtered, chilled (35-40°F). Available but not preferred for cleaning.

All three outputs receive the same filtered water input (Waterdrop 15UC-UF inline filter). The carbonated output adds CO2 via TAPRITE dual-gauge regulator.

## Solenoid Valves

Beduan 12V 1/4" Inlet Water Solenoid Valve (B07NWCQJK9), normally closed. $8.99 each.

- Working pressure: 0.02-0.8 MPa (~3-116 PSI)
- 1/4" push-connect fittings (blue collets)
- 12V DC, 4.8W
- Currently 2 installed (one per flavor line, prevent backflow)
- 2 more planned for clean cycle

## Peristaltic Pumps

Kamoer peristaltic pumps (B09MS6C91D), 400ml/min at 12V. $32.55 each.

- Duty-cycle driven by flow meter readings (burst mode, not continuous)
- Burst duration: 50-300ms realistic range
- The pump's roller compression creates a natural seal when stopped — liquid doesn't free-flow through a stopped pump

## Observed Failure: Leak Under Pump Backpressure

One flavor line had insufficiently tightened zip ties at the blue-to-black-silicone joint. A kink (flat spot) formed in the platypus bag as it emptied. The peristaltic pump was pulling hard against the remaining small amount of liquid. The pressure differential was enough to push liquid past the loose zip-tie joint, causing leaking at the bag end of the line.

Fix: tightened the zip tie a small amount. Problem went away immediately.

**Takeaway**: all joints in the line must be snug enough to withstand the pump's suction pressure, especially when bags are near-empty and more prone to kinking.

## Clean Cycle Plumbing (Planned)

Two new solenoid valves (one per flavor line) and a needle valve for flow restriction. Tee fittings and hard tubing sourced from an existing ice maker kit.

### Layout

The tee connects between the bag and the dispensing solenoid, so clean water can fill the bag itself (not just the line downstream of the solenoid).

```
Platypus bag → TEE → [dispensing solenoid] → [pump] → dispensing point
                ↑
  water supply → needle valve → [clean solenoid]
```

Each flavor line gets its own clean solenoid and tee. The needle valve and water supply are shared — a second tee splits the water line to both clean solenoids.

### Why a Needle Valve Instead of a Pressure Regulator

Household water pressure is 40-80 PSI. The flavoring lines and zip-tie joints operate at well under 10 PSI. A pressure regulator in the 1/4" RO ecosystem is designed to protect RO membranes — they reduce to ~60 PSI, not the 5-10 PSI we need, and aren't reliable at the extreme low end of their range.

A needle valve restricts *flow* by physically narrowing the passage with a screw. Turn it down and only a trickle gets through, regardless of input pressure. The pressure issue solves itself because so little water can pass at once. Simpler, cheaper, and more reliable at low flow rates.

### Clean Cycle Sequence

1. **Fill bag with water**: Dispensing solenoid CLOSED, clean solenoid OPEN, pump OFF. Water flows through the needle valve, through the tee, into the bag. The bag fills with clean water, dissolving residual flavoring.
2. **Flush**: Dispensing solenoid OPEN, clean solenoid CLOSED, pump ON. Pump pulls the water (plus dissolved flavoring residue) from the bag, through the dispensing point and out.
3. **Repeat** steps 1-2 as many times as needed until water runs clear.
4. User refills bag with new flavoring (manually now, via hopper system later).

Room-temperature tap water is preferred over chilled — dissolves sugar/flavoring residue faster and doesn't deplete the chiller reservoir.

### Parts

- YKEBVPW 1/4" needle valve flow control (B0FBFVTNLM) — $7.49
- Beduan 12V 1/4" solenoid valve NC x2 (B07NWCQJK9) — $8.99 each
- Tee fittings and hard tubing: from ice maker kit (already on hand)
