# Cartridge CAD Scripts

Parametric 3D models for the soda flavor injector cartridge, generated with [CadQuery](https://cadquery.readthedocs.io/) (Python).

## Why CadQuery

CadQuery is a Python library built on the OpenCASCADE solid modeling kernel. It produces real B-rep solids (not mesh approximations) and exports to STEP, STL, and other standard formats. Its fluent API maps directly to the same operations available in OnShape — sketch, extrude, fillet, hole, boolean, pattern — making it straightforward to describe parts to an LLM and get working code back.

The conversation that led to this choice is documented in:
`hardware/cartridge/planning/conversations/1 - AI-generated CAD models with Python.md`

## Setup

CadQuery depends on OpenCASCADE, which is easiest to install via conda.

### Option A: Conda (recommended)

```bash
# Create a dedicated environment
conda create -n cadquery python=3.10 -y
conda activate cadquery

# Install CadQuery from conda-forge
conda install -c conda-forge cadquery -y

# Optional: CQ-editor for interactive viewing
pip install cq-editor
```

### Option B: pip (if conda is not available)

```bash
pip install cadquery
```

Note: pip installation requires that the OCP (OpenCASCADE) wheel is available for your platform. This works on most x86_64 Linux and macOS systems. Apple Silicon (M1/M2/M3/M4) may need conda.

### Verify installation

```bash
python -c "import cadquery; print('CadQuery OK')"
```

## Running a script

```bash
conda activate cadquery   # if using conda
cd hardware/cartridge/scripts/
python pump_tray.py
```

Output files (STEP, STL) are written to `./output/`.

## Viewing output

- **OnShape**: Import the `.step` file (File > Import). OnShape is read-only for these — the Python script is the source of truth.
- **CQ-editor**: `cq-editor pump_tray.py` opens an interactive viewer with live code editing.
- **PrusaSlicer / BambuStudio**: Import the `.stl` directly for print prep.
- **FreeCAD**: Open the `.step` file for measurement and inspection.

## Parametric design philosophy

Every dimension is a named variable at the top of the script. To change the design:

1. Open the `.py` file
2. Edit the parameter values in the `PARAMETERS` section
3. Re-run the script
4. New STEP/STL files appear in `output/`

Dimensions that have not been physically measured are marked with `# TBD` comments. The most important TBD is the pump mounting hole pattern — this must be measured from the physical Kamoer KPHM400-SW3B25 pump with calipers before printing.

## Scripts

| Script | Description | Status |
|--------|-------------|--------|
| `pump_tray.py` | Internal pump mounting tray (flat plate + bosses + clips + wire channel) | First draft — TBD mounting holes |

## File structure

```
scripts/
  pump_tray.py      # parametric model source
  README.md         # this file
  output/           # generated STEP and STL files (gitignored)
```
