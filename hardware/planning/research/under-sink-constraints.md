# Under-Sink Space Constraints Survey

Research into actual available space under kitchen sinks across markets. This informs enclosure dimension decisions — no dimensions are locked.

---

## Cabinet Standards by Market

### United States (most common: 36" / 914mm wide)

| Dimension | 30" Cabinet | 33" Cabinet | 36" Cabinet |
|---|---|---|---|
| Exterior Width | 762mm | 838mm | 914mm |
| Interior Width (face-frame) | ~711mm | ~800mm | ~876mm |
| Interior Width (frameless/IKEA) | ~724mm | ~813mm | ~889mm |
| Interior Depth | ~560mm | ~560mm | ~560mm |
| Cabinet Box Height (no countertop) | 762mm | 762mm | 762mm |
| Toe kick | 114mm | 114mm | 114mm |

### EU / UK (most common: 800mm wide)

| Dimension | 600mm Unit | 800mm Unit | 1000mm Unit |
|---|---|---|---|
| Interior Width | ~564mm | ~764mm | ~964mm |
| Interior Depth | ~540mm | ~540mm | ~540mm |
| Cabinet Box Height | 720mm | 720mm | 720mm |

### Australia (most common: 900mm wide)

| Dimension | 600mm Unit | 900mm Unit |
|---|---|---|
| Interior Width | ~564mm | ~864mm |
| Interior Depth | ~560mm | ~560mm |
| Cabinet Box Height | 720mm | 720mm |

---

## What Eats Into the Space

### Center Plumbing Zone
- P-trap: ~125mm wide x 300mm front-to-back x 150mm tall
- With garbage disposal (~50% of US households, rare in EU/AU): 160-230mm diameter x 290-340mm tall, hanging from center of sink
- This creates a **200-250mm wide center zone** that is unusable for appliances
- In a 36" cabinet, this divides the space into two side zones of **280-330mm wide each**
- In a 33" cabinet: side zones **240-290mm each**
- In a 30" cabinet: side zones **200-250mm each**

### Back Wall Obstructions
- Hot/cold supply lines: emerge ~350-400mm above cabinet floor, offset 100-150mm left/right of center
- Shut-off valves: protrude ~50-75mm from back wall
- Electrical outlet (for disposal): protrudes ~40-50mm, usually offset to one side
- **Net effect:** back ~50-75mm of depth is partially obstructed

### Other Space Competitors
- RO filtration systems: 280x280x381mm (tank alone) + filter bank — the biggest competitor for space
- Simple cartridge filters: small (~80mm dia x 250mm tall), wall-mounted
- Pull-out trash bins: if present in sink cabinet, consume an entire side zone

---

## Actual Usable Space

### Vertical Clearance (the tightest dimension)

| Scenario | Floor to underside of sink bowl |
|---|---|
| Best case (shallow 200mm sink) | ~430-450mm |
| **Typical case (230mm sink)** | **~380-420mm** |
| Worst case (deep 254mm sink, thick countertop) | ~330-360mm |

**Critical note:** Side zones have MORE vertical clearance than the center — the sink bowl only dips down in the middle. At the sides, clearance extends to the underside of the countertop, which is ~648mm above cabinet floor. An appliance in a side zone that is taller than the sink bowl depth can extend upward beside the sink.

### Usable Depth
- Full interior: ~560mm
- After back-wall obstructions: **~480-510mm**
- This is far more than needed. Depth is NOT a binding constraint.

### Usable Width (per side zone)
- 36" US cabinet: **280-330mm per side**
- 33" US cabinet: **240-290mm per side**
- 800mm EU cabinet: **250-280mm per side**
- 30" US / 600mm EU: **200-250mm per side** (tight)

---

## Existing Under-Sink Appliances (Comparison)

| Product | Width | Depth | Height | Notes |
|---|---|---|---|---|
| InSinkErator hot tank | 156mm | 156mm | 276mm | Very compact cylinder |
| 3-stage filter bank | 250mm | 130mm | 350mm | Wall-mountable |
| APEC RO tank | 280mm | 280mm | 381mm | Floor-standing cylinder |
| Tankless RO (Frizzlife) | 390mm | 130mm | 360mm | Wall-mountable |
| **Quooker CUBE** | **223mm** | **340mm** | **500mm** | Closest comparable product |
| GROHE Blue system | 600mm | 500mm | 430mm | Very large, premium only |

The **Quooker CUBE** is the most relevant comparison — it's an under-sink carbonation/chilling unit. At 223x340x500mm it is taller and deeper than our working assumption but narrower.

---

## Assessment of Current Working Dimensions (280W x 250D x 400H mm)

| Dimension | Current | Assessment | Possibility Range |
|---|---|---|---|
| **Width (280mm)** | Good | Fits 33"+ US and 800mm+ EU cabinets. Excludes 30" US / 600mm EU. | 200-330mm viable |
| **Depth (250mm)** | Very conservative | ~480-510mm available. Could go much deeper. | 250-350mm viable without concern |
| **Height (400mm)** | Borderline | Fits most but NOT deep sinks + thick countertops. 380mm would be safer for broad compatibility. Side-zone placement allows taller. | 330-450mm depending on placement |

### Key Insight: Depth is the Unconstrained Dimension

The existing research treats 250mm depth as nearly locked. But there is **230-260mm of unused depth** available in virtually all cabinets. Going to 300mm depth costs nothing in compatibility and buys significant internal volume. Going to 350mm is still comfortable.

**This changes the diagonal stacking math substantially.** At 300mm depth instead of 250mm:
- A 350mm bag at 60° consumes 175mm depth (from the bag) + 69mm (from stack thickness) = 244mm. At 250mm depth that's 6mm margin. At 300mm depth that's **56mm margin** — room for mounting hardware, wall thickness, and tube routing.
- Or: shallower angles become feasible. At 55° with 300mm depth: 266mm total depth consumption. That fits with 34mm margin.

### Key Insight: Height Could Be Shorter, Not Taller

If depth is increased to 300mm, we may not need 400mm of height at all. A 300W x 300D x 350H mm enclosure might fit everything with diagonal bags at a shallower angle, while being more universally compatible with under-sink spaces.

---

## Safe Design Envelope Recommendations

| Compatibility Target | Width | Depth | Height |
|---|---|---|---|
| Fits nearly all (30" US, 600mm EU) | 200mm | 250mm | 330mm |
| Fits most (33"+ US, 800mm+ EU) | 280mm | 300mm | 400mm |
| Fits large cabinets (36" US, 900mm+ AU) | 330mm | 350mm | 450mm |

The "fits most" row is the likely sweet spot, but note that depth has been increased from 250mm to 300mm — this is free real estate that the current design leaves on the table.
