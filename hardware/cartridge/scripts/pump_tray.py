"""
Pump Tray — Parametric CadQuery Model
======================================

The pump tray is a flat PETG plate that sits inside the cartridge outer shell.
It carries two Kamoer KPHM400-SW3B25 peristaltic pumps mounted side-by-side,
with heat-set M3 brass insert bosses, tube strain-relief C-clips, and a wire
routing channel.

Construction: tray + shell assembly (pump-mounting.md recommendation #1).
The tray prints flat (horizontal) so screw bosses have maximum layer-bond
strength — screw axes run parallel to print layers.

Reference documents:
  - hardware/cartridge/planning/research/pump-mounting.md
  - hardware/cartridge/planning/research/cartridge-envelope.md
  - hardware/cartridge/planning/requirements.md

Usage:
  conda activate cadquery  # or however you set up the environment
  python pump_tray.py      # exports STEP + STL to ./output/

Requires: cadquery (pip install cadquery)
"""

import cadquery as cq
import os

# =============================================================================
# PARAMETERS — Change these, re-run the script. All dimensions in mm.
# =============================================================================

# --- Base plate ---
# From cartridge-envelope.md sec 6a: calculated envelope is 148mm wide
# (2x 68.6mm pumps + 5mm gap + 6mm walls). The tray sits inside the shell
# walls, so tray width = envelope width minus shell wall allowance.
# Width = across pumps (left-right), Depth = along motor axis (front-back)
PLATE_WIDTH = 148.0    # mm — spans both pumps side-by-side + boss clearance
PLATE_DEPTH = 116.0    # mm — along pump motor axis
PLATE_THICKNESS = 6.0  # mm — pump-mounting.md: recommended 6 mm minimum

# --- Pump arrangement ---
# Two KPHM400 pumps side-by-side. Each pump: 115.6 x 68.6 x 62.7 mm (L x W x H)
# pump-mounting.md sec 6, cartridge-envelope.md sec 5b
PUMP_WIDTH = 68.6      # mm — single pump width (left-right when side-by-side)
PUMP_DEPTH = 115.6     # mm — single pump depth (motor axis)
PUMP_CENTER_GAP = 5.0  # mm — gap between the two pumps (cartridge-envelope.md sec 6a)

# Pump 1 center X = -(PUMP_WIDTH + PUMP_CENTER_GAP) / 2
# Pump 2 center X = +(PUMP_WIDTH + PUMP_CENTER_GAP) / 2
# (Centered on the tray origin)

# --- Heat-set insert bosses (M3) ---
# pump-mounting.md sec 3: M3 x 4-5mm brass knurled inserts, installed at 245C
# Pilot hole: 4.0 mm dia for M3 insert in PETG
# Boss OD: minimum 8 mm (4.0 mm hole + 2x 2.0 mm wall)
# Boss height: minimum 6 mm (insert length + 1 mm clearance)
INSERT_PILOT_DIA = 4.0    # mm — hole for M3 heat-set insert
BOSS_OD = 8.0             # mm — outer diameter of cylindrical boss
BOSS_HEIGHT = 6.0         # mm — total height (equals plate thickness by design)

# --- Pump mounting hole pattern (TBD — MEASURE FROM PHYSICAL PUMP) ---
# pump-mounting.md: "inferred ~55-65 mm x 40-50 mm" from KK series scaling.
# These are (x, y) offsets from each pump's center on the tray.
# Positive X = toward plate edge, positive Y = toward plate front.
#
# >>> UPDATE THESE after measuring the actual KPHM400 bracket holes <<<
#
PUMP_MOUNT_HOLES_X_SPAN = 60.0  # mm — TBD: center-to-center horizontal
PUMP_MOUNT_HOLES_Y_SPAN = 44.0  # mm — TBD: center-to-center vertical
PUMP_MOUNT_HOLE_COUNT = 4       # 4 holes per pump assumed; could be 2

# Per-pump hole offsets relative to pump center (rectangular 4-hole pattern)
# If the pump only has 2 holes, delete the bottom two entries.
def _pump_hole_offsets():
    """Return list of (dx, dy) offsets for one pump's mounting holes."""
    hx = PUMP_MOUNT_HOLES_X_SPAN / 2
    hy = PUMP_MOUNT_HOLES_Y_SPAN / 2
    if PUMP_MOUNT_HOLE_COUNT == 4:
        return [(-hx, -hy), (hx, -hy), (-hx, hy), (hx, hy)]
    elif PUMP_MOUNT_HOLE_COUNT == 2:
        # 2-hole variant: top pair only (common for smaller brackets)
        return [(-hx, hy), (hx, hy)]
    else:
        return [(-hx, -hy), (hx, -hy), (-hx, hy), (hx, hy)]

# Pump center positions on the tray (X, Y) where Y=0 is tray center depth-wise
PUMP1_CENTER_X = -(PUMP_WIDTH + PUMP_CENTER_GAP) / 2.0
PUMP2_CENTER_X = +(PUMP_WIDTH + PUMP_CENTER_GAP) / 2.0
PUMP_CENTER_Y = 0.0  # Pumps centered on tray depth axis

# --- Rubber grommet clearance ---
# pump-mounting.md sec 2: M3 screws pass through rubber grommets (~6-8 mm OD, ~3 mm ID)
# The boss pilot hole is sized for the heat-set insert; the grommet sits between
# the pump bracket and the boss top surface. No change to boss geometry needed —
# the grommet sits on top of the boss, not inside it.
# (Noted here for assembly reference; does not affect the CAD model.)

# --- Tube strain-relief C-clips ---
# pump-mounting.md sec 4: printed C-clips for BPT tubing (4.8 ID x 8.0 OD)
# Clip ID = tube OD + 0.3 mm (press-fit snap)
# Clip opening width = tube OD - 1.5 mm
TUBE_OD_BPT = 8.0              # mm — BPT pump tubing outer diameter
CLIP_ID = TUBE_OD_BPT + 0.3    # mm — inner bore of C-clip (8.3)
CLIP_OPENING = TUBE_OD_BPT - 1.5  # mm — slot width for snap-in (6.5)
CLIP_OD = CLIP_ID + 3.0        # mm — outer diameter of C-clip ring (wall ~1.5 mm)
CLIP_HEIGHT = 6.0               # mm — same as plate thickness; clips are flush bosses
CLIP_WALL = (CLIP_OD - CLIP_ID) / 2.0  # ~1.5 mm

# C-clip positions: 2 clips per pump (near pump head, near barb transition)
# Positioned along top edge of each pump zone on the tray.
# Y positions: near front of pump (+Y side) and near rear (-Y side)
CLIP_Y_NEAR_HEAD = PUMP_DEPTH / 2.0 - 10.0    # 10mm in from pump head end
CLIP_Y_NEAR_MOTOR = -(PUMP_DEPTH / 2.0 - 15.0)  # 15mm in from motor end

# Each pump has 2 clips, placed at the pump's X center (tubes exit from pump head top)
# Clips for pump 1 at X = PUMP1_CENTER_X, clips for pump 2 at X = PUMP2_CENTER_X

# --- Wire routing channel ---
# pump-mounting.md sec 5: channel runs from pump motor ends to one edge of the tray
# where wires exit toward the mating face / electrical pads.
# Channel runs along the center gap between the two pumps.
WIRE_CHANNEL_WIDTH = 6.0    # mm — wide enough for 2x 22AWG wires side by side
WIRE_CHANNEL_DEPTH = 3.0    # mm — half the plate thickness
WIRE_CHANNEL_X = 0.0        # mm — centered between pumps (in the gap)

# --- Tray-to-shell mounting holes ---
# pump-mounting.md sec 7: 4x M3 screws with heat-set inserts (2 per end of tray)
TRAY_MOUNT_INSET = 6.0  # mm — distance from tray edge to mount hole center
TRAY_MOUNT_HOLES = [
    (-PLATE_WIDTH / 2 + TRAY_MOUNT_INSET, -PLATE_DEPTH / 2 + TRAY_MOUNT_INSET),
    (+PLATE_WIDTH / 2 - TRAY_MOUNT_INSET, -PLATE_DEPTH / 2 + TRAY_MOUNT_INSET),
    (-PLATE_WIDTH / 2 + TRAY_MOUNT_INSET, +PLATE_DEPTH / 2 - TRAY_MOUNT_INSET),
    (+PLATE_WIDTH / 2 - TRAY_MOUNT_INSET, +PLATE_DEPTH / 2 - TRAY_MOUNT_INSET),
]

# --- Fillet ---
PLATE_EDGE_FILLET = 2.0  # mm — rounded edges on base plate for print quality

# --- Output ---
OUTPUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "output")
EXPORT_STEP = True
EXPORT_STL = True


# =============================================================================
# MODEL CONSTRUCTION
# =============================================================================

def build_pump_tray():
    """Build the complete pump tray solid and return the CadQuery assembly."""

    # -------------------------------------------------------------------------
    # Step 1: Base plate
    # A flat rectangular plate, centered at origin. Z=0 is the bottom face;
    # the top face at Z=PLATE_THICKNESS is where pumps sit.
    # -------------------------------------------------------------------------
    tray = (
        cq.Workplane("XY")
        .box(PLATE_WIDTH, PLATE_DEPTH, PLATE_THICKNESS, centered=(True, True, False))
    )

    # Round the vertical edges for print quality and handling
    tray = tray.edges("|Z").fillet(PLATE_EDGE_FILLET)

    # -------------------------------------------------------------------------
    # Step 2: Pump mounting screw bosses with heat-set insert pilot holes
    # Bosses are raised cylinders on the top face. Since BOSS_HEIGHT == PLATE_THICKNESS,
    # the bosses are flush with the plate surface (they are the plate itself) and the
    # pilot holes go through the full plate thickness.
    #
    # For a thicker boss (if BOSS_HEIGHT > PLATE_THICKNESS), we would extrude
    # cylindrical bosses upward from the top face. Here they are flush, so we
    # just drill the pilot holes.
    # -------------------------------------------------------------------------

    # Collect all pump mount hole positions (absolute coordinates on tray)
    all_pump_holes = []
    for pump_cx in [PUMP1_CENTER_X, PUMP2_CENTER_X]:
        for dx, dy in _pump_hole_offsets():
            all_pump_holes.append((pump_cx + dx, PUMP_CENTER_Y + dy))

    # If bosses are taller than the plate, add boss extrusions
    boss_extra = BOSS_HEIGHT - PLATE_THICKNESS
    if boss_extra > 0:
        for hx, hy in all_pump_holes:
            tray = (
                tray
                .faces(">Z")
                .workplane()
                .pushPoints([(hx, hy)])
                .circle(BOSS_OD / 2.0)
                .extrude(boss_extra)
            )

    # Drill pilot holes for heat-set inserts (through the full boss height)
    for hx, hy in all_pump_holes:
        tray = (
            tray
            .faces(">Z")
            .workplane()
            .pushPoints([(hx, hy)])
            .hole(INSERT_PILOT_DIA, depth=BOSS_HEIGHT)
        )

    # -------------------------------------------------------------------------
    # Step 3: Tray-to-shell mounting holes (corners of the tray)
    # Same heat-set insert spec as pump mounts.
    # -------------------------------------------------------------------------
    for mx, my in TRAY_MOUNT_HOLES:
        tray = (
            tray
            .faces(">Z")
            .workplane()
            .pushPoints([(mx, my)])
            .hole(INSERT_PILOT_DIA, depth=PLATE_THICKNESS)
        )

    # -------------------------------------------------------------------------
    # Step 4: Tube strain-relief C-clips
    # C-shaped rings on the top face that snap around BPT tubing.
    # Modeled as a cylinder with a bore and a slot cut out of one side.
    # -------------------------------------------------------------------------
    clip_positions = []
    for pump_cx in [PUMP1_CENTER_X, PUMP2_CENTER_X]:
        clip_positions.append((pump_cx, CLIP_Y_NEAR_HEAD))
        clip_positions.append((pump_cx, CLIP_Y_NEAR_MOTOR))

    for cx, cy in clip_positions:
        # Outer cylinder of the C-clip (raised above plate top face)
        clip_ring = (
            cq.Workplane("XY")
            .workplane(offset=PLATE_THICKNESS)
            .pushPoints([(cx, cy)])
            .circle(CLIP_OD / 2.0)
            .circle(CLIP_ID / 2.0)
            .extrude(CLIP_HEIGHT)
        )
        tray = tray.union(clip_ring)

        # Cut the slot opening in the C-clip (oriented toward the plate edge,
        # i.e., in the +X direction for pump 1, -X for pump 2... but for
        # simplicity, all slots open in the +X direction — tubes snap in from
        # the side). The slot is a rectangular cut through the clip ring.
        slot = (
            cq.Workplane("XY")
            .workplane(offset=PLATE_THICKNESS)
            .pushPoints([(cx + CLIP_OD / 2.0, cy)])
            .rect(CLIP_OD, CLIP_OPENING)
            .extrude(CLIP_HEIGHT)
        )
        tray = tray.cut(slot)

    # -------------------------------------------------------------------------
    # Step 5: Wire routing channel
    # A shallow groove cut into the top face of the plate, running along the
    # center gap between the two pumps (X=0) for the full depth of the tray.
    # Motor wires from both pumps drop into this channel and route toward the
    # mating face (rear, +Y direction).
    # -------------------------------------------------------------------------
    wire_channel = (
        cq.Workplane("XY")
        .workplane(offset=PLATE_THICKNESS - WIRE_CHANNEL_DEPTH)
        .pushPoints([(WIRE_CHANNEL_X, 0)])
        .rect(WIRE_CHANNEL_WIDTH, PLATE_DEPTH - 2 * TRAY_MOUNT_INSET)
        .extrude(WIRE_CHANNEL_DEPTH + 0.01)  # small overcut to ensure clean boolean
    )
    tray = tray.cut(wire_channel)

    return tray


# =============================================================================
# EXPORT
# =============================================================================

def export_model(model):
    """Export the model to STEP and/or STL files."""
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    if EXPORT_STEP:
        step_path = os.path.join(OUTPUT_DIR, "pump_tray.step")
        cq.exporters.export(model, step_path)
        print(f"Exported STEP: {step_path}")

    if EXPORT_STL:
        stl_path = os.path.join(OUTPUT_DIR, "pump_tray.stl")
        cq.exporters.export(model, stl_path)
        print(f"Exported STL:  {stl_path}")


# =============================================================================
# MAIN
# =============================================================================

if __name__ == "__main__":
    print("Building pump tray...")
    print(f"  Plate: {PLATE_WIDTH} x {PLATE_DEPTH} x {PLATE_THICKNESS} mm")
    print(f"  Pump mount holes per pump: {PUMP_MOUNT_HOLE_COUNT}")
    print(f"  Pump mount pattern (TBD): {PUMP_MOUNT_HOLES_X_SPAN} x {PUMP_MOUNT_HOLES_Y_SPAN} mm")
    print(f"  Boss OD: {BOSS_OD} mm, pilot hole: {INSERT_PILOT_DIA} mm")
    print(f"  C-clip count: {len([1 for _ in [PUMP1_CENTER_X, PUMP2_CENTER_X] for _ in [1,2]])}")
    print(f"  Wire channel: {WIRE_CHANNEL_WIDTH} x {WIRE_CHANNEL_DEPTH} mm")
    print()

    model = build_pump_tray()
    export_model(model)

    print("\nDone. Open the STEP file in OnShape or CQ-editor to inspect.")
    print(">>> Remember: pump mounting hole pattern is TBD — measure the physical pump! <<<")
