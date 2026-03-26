#!/usr/bin/env python3
"""SVG label collision checker for engineering drawings.

Parses an SVG file, computes bounding boxes for all <text> elements and
<rect>/<line> geometry, then reports overlapping pairs.

Collision types reported:
  TEXT-TEXT:    Two text labels overlap each other (always a bug)
  TEXT-BORDER: Text crosses a rect edge (partially inside, partially outside)
  TEXT-LINE:   Text overlaps a line element

NOT reported (false positives filtered out):
  - Text fully inside a rect (label inside its view boundary — normal)
  - Text fully outside a rect (no overlap — obviously fine)
  - Dimension text on its own dimension line (matched by proximity heuristic)

Usage:
    python3 tools/svg_label_check.py hardware/path/to/drawing.svg

Exit code: 0 if clean, 1 if collisions found.
"""

import sys
import math
import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass

# Approximate character width as fraction of font-size (sans-serif)
CHAR_WIDTH_RATIO = 0.6

# Vertical metrics as fraction of font-size
ASCENT_RATIO = 0.8
DESCENT_RATIO = 0.2

# Minimum overlap in pixels to report (avoid sub-pixel false positives)
OVERLAP_THRESHOLD = 2.0


@dataclass
class BBox:
    x_min: float
    y_min: float
    x_max: float
    y_max: float
    label: str
    kind: str  # "text", "rect", "line"

    @property
    def width(self):
        return self.x_max - self.x_min

    @property
    def height(self):
        return self.y_max - self.y_min

    def contains(self, other: "BBox") -> bool:
        """Check if self fully contains other (with small tolerance)."""
        tol = 2.0
        return (self.x_min - tol <= other.x_min and
                self.y_min - tol <= other.y_min and
                self.x_max + tol >= other.x_max and
                self.y_max + tol >= other.y_max)

    def overlaps(self, other: "BBox") -> bool:
        """Check if two bboxes overlap by more than OVERLAP_THRESHOLD."""
        ox = min(self.x_max, other.x_max) - max(self.x_min, other.x_min)
        oy = min(self.y_max, other.y_max) - max(self.y_min, other.y_min)
        return ox > OVERLAP_THRESHOLD and oy > OVERLAP_THRESHOLD

    def overlap_area(self, other: "BBox") -> float:
        ox = min(self.x_max, other.x_max) - max(self.x_min, other.x_min)
        oy = min(self.y_max, other.y_max) - max(self.y_min, other.y_min)
        if ox > 0 and oy > 0:
            return ox * oy
        return 0.0

    def crosses_border(self, container: "BBox") -> bool:
        """Check if self crosses any edge of container (partially in, partially out)."""
        if not self.overlaps(container):
            return False
        if container.contains(self):
            return False  # Fully inside — not a border crossing
        # We overlap but aren't fully contained → we cross a border
        return True


def parse_float(s: str, default: float = 0.0) -> float:
    try:
        return float(s)
    except (TypeError, ValueError):
        return default


def get_font_size(elem: ET.Element, default: float = 10.0) -> float:
    """Extract font-size from element attributes, class, or style."""
    fs = elem.get("font-size")
    if fs:
        return parse_float(fs.replace("px", ""), default)

    style = elem.get("style", "")
    m = re.search(r"font-size:\s*([\d.]+)", style)
    if m:
        return parse_float(m.group(1), default)

    return default


def get_text_anchor(elem: ET.Element) -> str:
    anchor = elem.get("text-anchor", "start")
    style = elem.get("style", "")
    m = re.search(r"text-anchor:\s*(\w+)", style)
    if m:
        anchor = m.group(1)
    return anchor


def parse_rotate_transform(transform: str):
    """Parse transform='rotate(angle,cx,cy)' and return (angle_deg, cx, cy) or None."""
    if not transform:
        return None
    m = re.search(r"rotate\(\s*([-\d.]+)(?:\s*,\s*([-\d.]+)\s*,\s*([-\d.]+))?\s*\)", transform)
    if m:
        angle = float(m.group(1))
        cx = float(m.group(2)) if m.group(2) else 0.0
        cy = float(m.group(3)) if m.group(3) else 0.0
        return (angle, cx, cy)
    return None


def rotate_point(px, py, angle_deg, cx, cy):
    """Rotate point (px,py) around (cx,cy) by angle_deg degrees."""
    rad = math.radians(angle_deg)
    dx, dy = px - cx, py - cy
    rx = dx * math.cos(rad) - dy * math.sin(rad)
    ry = dx * math.sin(rad) + dy * math.cos(rad)
    return cx + rx, cy + ry


def text_bbox(elem: ET.Element) -> BBox | None:
    """Compute axis-aligned bounding box for a <text> element."""
    text = elem.text
    if not text or not text.strip():
        return None

    x = parse_float(elem.get("x"))
    y = parse_float(elem.get("y"))
    font_size = get_font_size(elem)
    anchor = get_text_anchor(elem)
    text_len = len(text)

    char_w = CHAR_WIDTH_RATIO * font_size
    total_w = text_len * char_w
    h_ascent = ASCENT_RATIO * font_size
    h_descent = DESCENT_RATIO * font_size

    if anchor == "middle":
        x_min = x - total_w / 2
    elif anchor == "end":
        x_min = x - total_w
    else:
        x_min = x

    x_max = x_min + total_w
    y_min = y - h_ascent
    y_max = y + h_descent

    transform = elem.get("transform", "")
    rot = parse_rotate_transform(transform)
    if rot:
        angle, cx, cy = rot
        corners = [
            (x_min, y_min), (x_max, y_min),
            (x_max, y_max), (x_min, y_max),
        ]
        rotated = [rotate_point(px, py, angle, cx, cy) for px, py in corners]
        xs = [p[0] for p in rotated]
        ys = [p[1] for p in rotated]
        x_min, x_max = min(xs), max(xs)
        y_min, y_max = min(ys), max(ys)

    return BBox(x_min, y_min, x_max, y_max, label=f'"{text.strip()}"', kind="text")


def rect_bbox(elem: ET.Element) -> BBox | None:
    """Compute bbox for a <rect> element."""
    x = parse_float(elem.get("x"))
    y = parse_float(elem.get("y"))
    w = parse_float(elem.get("width"))
    h = parse_float(elem.get("height"))
    if w == 0 or h == 0:
        return None

    # Skip canvas background
    if w > 1000 and h > 700:
        return None

    return BBox(x, y, x + w, y + h, label=f"rect({x},{y} {w}x{h})", kind="rect")


def line_bbox(elem: ET.Element) -> BBox | None:
    """Compute bbox for a <line> element (with small padding for collision)."""
    x1 = parse_float(elem.get("x1"))
    y1 = parse_float(elem.get("y1"))
    x2 = parse_float(elem.get("x2"))
    y2 = parse_float(elem.get("y2"))

    pad = 1.0
    x_min = min(x1, x2) - pad
    y_min = min(y1, y2) - pad
    x_max = max(x1, x2) + pad
    y_max = max(y1, y2) + pad

    return BBox(x_min, y_min, x_max, y_max,
                label=f"line({x1},{y1}→{x2},{y2})", kind="line")


def is_dimension_text(text_bb: BBox) -> bool:
    """Heuristic: is this text a dimension value (number, diameter, etc.)?"""
    text = text_bb.label.strip('"')
    # Short numeric values, diameter symbols, "TYP", "x4", scale ratios
    return bool(re.match(
        r'^[⌀ø]?\d+\.?\d*'  # "55", "6.0", "⌀15.3"
        r'|^\d+:\d+$'        # "8:1", "10:1"
        r'|^x\d+$'           # "x4", "x6"
        r'|^TYP$'            # "TYP"
        r'|^[A-Z]$'          # Single letter section labels "A"
        , text
    ))


def is_dimension_label_for_line(text_bb: BBox, line_bb: BBox) -> bool:
    """Heuristic: is this text a dimension label for this specific line?"""
    if not is_dimension_text(text_bb):
        return False

    tx = (text_bb.x_min + text_bb.x_max) / 2
    ty = (text_bb.y_min + text_bb.y_max) / 2
    lx = (line_bb.x_min + line_bb.x_max) / 2
    ly = (line_bb.y_min + line_bb.y_max) / 2

    dist = math.hypot(tx - lx, ty - ly)
    line_len = math.hypot(line_bb.width, line_bb.height)

    return dist < max(line_len * 0.6, 20)


def get_ancestor_translate(elem: ET.Element, parent_map: dict) -> tuple[float, float]:
    """Walk up the tree accumulating translate(tx,ty) from <g> ancestors."""
    tx_total, ty_total = 0.0, 0.0
    node = elem
    while node in parent_map:
        node = parent_map[node]
        transform = node.get("transform", "")
        m = re.search(r"translate\(\s*([-\d.]+)[\s,]+([-\d.]+)\s*\)", transform)
        if m:
            tx_total += float(m.group(1))
            ty_total += float(m.group(2))
    return tx_total, ty_total


def apply_translate(bb: BBox, tx: float, ty: float) -> BBox:
    """Offset a bbox by a translate amount."""
    if tx == 0 and ty == 0:
        return bb
    return BBox(bb.x_min + tx, bb.y_min + ty, bb.x_max + tx, bb.y_max + ty,
                bb.label, bb.kind)


def parse_svg(filepath: str) -> tuple[list[BBox], list[BBox]]:
    """Parse SVG and return (text_bboxes, geometry_bboxes)."""
    tree = ET.parse(filepath)
    root = tree.getroot()

    ns = ""
    if root.tag.startswith("{"):
        ns = root.tag.split("}")[0] + "}"

    # Build parent map for ancestor traversal
    parent_map = {child: parent for parent in root.iter() for child in parent}

    texts = []
    geometry = []

    for elem in root.iter():
        tag = elem.tag.replace(ns, "")

        if tag == "text":
            bb = text_bbox(elem)
            if bb:
                tx, ty = get_ancestor_translate(elem, parent_map)
                texts.append(apply_translate(bb, tx, ty))
        elif tag == "rect":
            bb = rect_bbox(elem)
            if bb:
                tx, ty = get_ancestor_translate(elem, parent_map)
                geometry.append(apply_translate(bb, tx, ty))
        elif tag == "line":
            bb = line_bbox(elem)
            if bb:
                tx, ty = get_ancestor_translate(elem, parent_map)
                geometry.append(apply_translate(bb, tx, ty))

    return texts, geometry


def check_collisions(texts: list[BBox], geometry: list[BBox]) -> list[str]:
    """Check for real collisions, filtering out false positives."""
    issues = []

    rects = [g for g in geometry if g.kind == "rect"]
    lines = [g for g in geometry if g.kind == "line"]

    # Text vs text — always a bug
    for i in range(len(texts)):
        for j in range(i + 1, len(texts)):
            if texts[i].overlaps(texts[j]):
                area = texts[i].overlap_area(texts[j])
                issues.append(
                    f"TEXT-TEXT ({area:.0f}px²): "
                    f"{texts[i].label}  ×  {texts[j].label}"
                )

    # Text vs rect — only if text CROSSES the border (not fully inside)
    # Skip: dimension labels, and long notes text (> 25 chars is notes, not a label)
    for t in texts:
        text_content = t.label.strip('"')
        if is_dimension_text(t):
            continue
        if len(text_content) > 25:
            continue  # Long text is notes/descriptions, not positioning labels
        for r in rects:
            if t.crosses_border(r):
                issues.append(
                    f"TEXT-BORDER: {t.label} crosses edge of {r.label}"
                )

    # Text vs line — skip if it's a dimension label for that line
    for t in texts:
        for ln in lines:
            if t.overlaps(ln) and not is_dimension_label_for_line(t, ln):
                area = t.overlap_area(ln)
                issues.append(
                    f"TEXT-LINE ({area:.0f}px²): "
                    f"{t.label}  ×  {ln.label}"
                )

    return issues


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 svg_label_check.py <file.svg>")
        sys.exit(1)

    filepath = sys.argv[1]
    texts, geometry = parse_svg(filepath)

    print(f"Parsed {len(texts)} text elements, {len(geometry)} geometry elements\n")

    issues = check_collisions(texts, geometry)

    if not issues:
        print("No collisions found.")
    else:
        # Group by type
        tt = [i for i in issues if i.startswith("TEXT-TEXT")]
        tb = [i for i in issues if i.startswith("TEXT-BORDER")]
        tl = [i for i in issues if i.startswith("TEXT-LINE")]

        print(f"{len(issues)} collision(s) found:\n")

        if tt:
            print(f"── Text vs Text ({len(tt)}) ──")
            for i in sorted(tt, reverse=True):
                print(f"  • {i}")
            print()

        if tb:
            print(f"── Text vs Border ({len(tb)}) ──")
            for i in sorted(tb):
                print(f"  • {i}")
            print()

        if tl:
            print(f"── Text vs Line ({len(tl)}) ──")
            for i in sorted(tl, reverse=True):
                print(f"  • {i}")
            print()

    return 1 if issues else 0


if __name__ == "__main__":
    sys.exit(main())
