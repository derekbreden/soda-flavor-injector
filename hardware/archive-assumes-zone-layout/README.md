# Archive: Research Assuming Horizontal Zone Layout

These documents were produced by agents that assumed a horizontal "layer cake" architecture — bags in a bottom zone, cartridge dock in a middle zone, electronics on top — with 250mm enclosure depth and 1L bags at 18°.

**Why they're archived:**
- They make spatial conclusions without considering diagonal interleave (User Vision 1)
- Many use bag dimensions that were later found to be wrong (250mm × 140mm; actual is 280mm × 152mm for 1L, 350mm × 190mm for 2L)
- The 250mm depth assumption is broken — 1L bags at 18° need 282mm, exceeding the 242mm interior

**They are NOT deleted.** The research methods, component specs, and engineering analysis within them are often sound — just applied to a specific layout that may not be the chosen direction. They remain valuable reference if a zone-based layout is revisited.

**For current research direction, see instead:**
- `decision-points/research/` — new research with corrected dimensions and layout-agnostic analysis
- `decision-points/visions/diagonal-interleave.md` — the spatial vision currently being explored
- `decision-points/cascade-matrix.md` — decision dependencies and pathways
- `decision-points/assumption-index.md` — categorization of all documents by their assumptions

## Documents in this archive

| Document | Original Location | Key Assumptions |
|---|---|---|
| `layout-spatial-planning.md` | `enclosure/research/` | Master zone layout with specific height bands |
| `bag-zone-geometry.md` | `hardware/` | 1L bags at 18° in 176mm zone, wrong bag dimensions |
| `dimensions-reconciliation.md` | `hardware/` | 280×250×400mm locked, zone height budgets |
| `incline-bag-mounting.md` | `enclosure/research/` | 18-20° sweet spot from wrong bag length |
| `hopper-and-bag-management.md` | `enclosure/research/` | Hopper positioned in zone layout |
| `front-face-interaction-design.md` | `enclosure/research/` | Cartridge slot + lever at zone-derived heights |
| `dock-mounting-strategies.md` | `cartridge/planning/research/` | Dock as horizontal shelf at zone boundary |
| `cartridge-envelope.md` | `cartridge/planning/research/` | 150×80×130mm from zone height budget |
| `under-cabinet-ergonomics.md` | `cartridge/planning/research/` | Cartridge slot at ~226mm from zone layout |
| `cartridge-change-workflow.md` | `cartridge/planning/research/` | Workflow at zone heights with cam lever |
| `mating-face.md` | `cartridge/planning/research/` | Port layout built around release plate + zone positioning |
| `bill-of-materials.md` | `hardware/` | Quantities based on zone layout + JG fittings |
