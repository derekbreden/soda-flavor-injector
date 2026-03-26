#!/usr/bin/env python3
"""
SVG Adjacency Checker — finds elements that float in space, disconnected
from the geometry they should be attached to.

In engineering drawings, every sub-component (connector, protrusion, fastener)
must physically touch the parent body it's attached to. This tool detects
elements whose bounding boxes don't touch or overlap any other element's
bounding box within a configurable tolerance.

Usage:
    python tools/svg_adjacency_check.py path/to/drawing.svg [--tolerance 2]
"""

import argparse
import math
import re
import sys
import xml.etree.ElementTree as ET


NS = {"svg": "http://www.w3.org/2000/svg"}


def parse_transform(transform_str):
    """Extract translate(x, y) from a transform attribute. Returns (tx, ty)."""
    if not transform_str:
        return 0.0, 0.0
    m = re.search(r"translate\(\s*([^,\s]+)[\s,]+([^)]+)\)", transform_str)
    if m:
        return float(m.group(1)), float(m.group(2))
    m = re.search(r"translate\(\s*([^)]+)\)", transform_str)
    if m:
        return float(m.group(1)), 0.0
    return 0.0, 0.0


def local_name(tag):
    """Strip namespace from tag."""
    if "}" in tag:
        return tag.split("}", 1)[1]
    return tag


def get_bbox(elem, tx=0.0, ty=0.0):
    """
    Compute axis-aligned bounding box for a shape element.
    Returns (x1, y1, x2, y2) in absolute coords, or None if not a shape.
    """
    tag = local_name(elem.tag)

    try:
        if tag == "rect":
            x = float(elem.get("x", 0))
            y = float(elem.get("y", 0))
            w = float(elem.get("width", 0))
            h = float(elem.get("height", 0))
            if w == 0 or h == 0:
                return None
            return (x + tx, y + ty, x + w + tx, y + h + ty)

        elif tag == "circle":
            cx = float(elem.get("cx", 0))
            cy = float(elem.get("cy", 0))
            r = float(elem.get("r", 0))
            if r == 0:
                return None
            return (cx - r + tx, cy - r + ty, cx + r + tx, cy + r + ty)

        elif tag == "ellipse":
            cx = float(elem.get("cx", 0))
            cy = float(elem.get("cy", 0))
            rx = float(elem.get("rx", 0))
            ry = float(elem.get("ry", 0))
            if rx == 0 or ry == 0:
                return None
            return (cx - rx + tx, cy - ry + ty, cx + rx + tx, cy + ry + ty)

        elif tag == "line":
            x1 = float(elem.get("x1", 0))
            y1 = float(elem.get("y1", 0))
            x2 = float(elem.get("x2", 0))
            y2 = float(elem.get("y2", 0))
            return (
                min(x1, x2) + tx, min(y1, y2) + ty,
                max(x1, x2) + tx, max(y1, y2) + ty,
            )

        elif tag == "polyline" or tag == "polygon":
            points_str = elem.get("points", "")
            coords = re.findall(r"[-+]?[0-9]*\.?[0-9]+", points_str)
            if len(coords) < 4:
                return None
            xs = [float(coords[i]) for i in range(0, len(coords), 2)]
            ys = [float(coords[i]) for i in range(1, len(coords), 2)]
            return (min(xs) + tx, min(ys) + ty, max(xs) + tx, max(ys) + ty)

        elif tag == "path":
            # Simple path bbox: extract all numbers that look like coordinates
            d = elem.get("d", "")
            nums = re.findall(r"[-+]?[0-9]*\.?[0-9]+", d)
            if len(nums) < 4:
                return None
            # Pair them as x,y - crude but works for simple paths
            xs = [float(nums[i]) for i in range(0, len(nums) - 1, 2)]
            ys = [float(nums[i]) for i in range(1, len(nums), 2)]
            if not xs or not ys:
                return None
            return (min(xs) + tx, min(ys) + ty, max(xs) + tx, max(ys) + ty)

    except (ValueError, TypeError):
        return None

    return None


def bbox_gap(a, b):
    """
    Compute the minimum gap between two bounding boxes.
    Returns 0 if they overlap or touch, positive distance if separated.
    """
    ax1, ay1, ax2, ay2 = a
    bx1, by1, bx2, by2 = b

    # Gap in X direction (negative means overlap)
    dx = max(0, max(ax1 - bx2, bx1 - ax2))
    # Gap in Y direction
    dy = max(0, max(ay1 - by2, by1 - ay2))

    # If both dx and dy > 0, elements are diagonally separated
    if dx > 0 and dy > 0:
        return math.sqrt(dx * dx + dy * dy)
    # Otherwise, the gap is the max of the two (one will be 0 for adjacent)
    return max(dx, dy)


def classify_element(elem):
    """Return a human-readable description of the element."""
    tag = local_name(elem.tag)
    cls = elem.get("class", "")
    eid = elem.get("id", "")
    parts = [tag]
    if cls:
        parts.append(f'class="{cls}"')
    if eid:
        parts.append(f'id="{eid}"')
    return " ".join(parts)


def bbox_str(bbox):
    """Format bbox as readable string."""
    return f"({bbox[0]:.1f}, {bbox[1]:.1f})..({bbox[2]:.1f}, {bbox[3]:.1f})"


# Tags that represent non-geometric elements (annotations, dimensions, text)
ANNOTATION_CLASSES = {"dim", "dim-text", "dim-sm", "ext", "label", "note",
                      "title-text", "subtitle", "viewlbl", "title",
                      "dim-text-left", "dim-text-right"}

# Tags that are inherently annotation/decoration
SKIP_TAGS = {"text", "marker", "style", "defs", "title", "desc", "pattern"}


def is_annotation(elem):
    """Check if element is a dimension/label/annotation rather than geometry."""
    cls = elem.get("class", "")
    for c in cls.split():
        if c in ANNOTATION_CLASSES:
            return True
    return False


def is_dimension_line(elem):
    """Check if a line element is a dimension/extension line rather than geometry."""
    tag = local_name(elem.tag)
    if tag != "line":
        return False
    cls = elem.get("class", "")
    for c in cls.split():
        if c in ("dim", "ext", "center", "hidden", "thin-hidden"):
            return True
    # Lines with markers are dimension lines
    if elem.get("marker-start") or elem.get("marker-end"):
        return True
    # Lines without any class are typically separator rules or callout leaders,
    # not structural geometry
    if not cls:
        return True
    return False


def is_structural_geometry(elem):
    """
    Check if an element represents actual physical geometry (bodies, connectors,
    ports, etc.) rather than annotations, dimensions, or construction lines.
    """
    tag = local_name(elem.tag)
    if tag in SKIP_TAGS:
        return False
    if is_annotation(elem):
        return False
    if is_dimension_line(elem):
        return False

    cls = elem.get("class", "")
    class_set = set(cls.split())

    # Center lines and hidden lines are construction geometry, not physical bodies.
    # However, some hidden lines represent real features — we skip them from
    # adjacency checking since they're projections, not physical surfaces.
    if class_set & {"center", "hidden", "thin-hidden"}:
        return False

    # Arrow markers in defs
    if class_set & {"arrow"}:
        return False

    # Rack envelope (dashed outline representing design boundary, not a physical part)
    if "rack" in class_set:
        return False

    return True


def collect_elements(root):
    """
    Walk the SVG tree and collect all structural geometry elements with their
    bounding boxes and group membership.
    Returns list of dicts: {elem, bbox, group_id, group_label, description}
    """
    results = []

    for elem in root:
        tag = local_name(elem.tag)

        if tag == "g":
            gid = elem.get("id", "unnamed-group")
            gtx, gty = parse_transform(elem.get("transform"))

            # Also check for nested <g> elements
            groups_to_process = [(elem, gid, gtx, gty)]

            while groups_to_process:
                group, group_id, tx, ty = groups_to_process.pop()

                for child in group:
                    ctag = local_name(child.tag)

                    if ctag == "g":
                        # Nested group — accumulate transforms
                        ctx, cty = parse_transform(child.get("transform"))
                        child_id = child.get("id", group_id)
                        groups_to_process.append(
                            (child, child_id, tx + ctx, ty + cty)
                        )
                        continue

                    if not is_structural_geometry(child):
                        continue

                    bbox = get_bbox(child, tx, ty)
                    if bbox is None:
                        continue

                    results.append({
                        "elem": child,
                        "bbox": bbox,
                        "group_id": group_id,
                        "description": classify_element(child),
                    })

        else:
            # Top-level elements (outside any group)
            if not is_structural_geometry(elem):
                continue
            bbox = get_bbox(elem)
            if bbox is None:
                continue
            results.append({
                "elem": elem,
                "bbox": bbox,
                "group_id": "(top-level)",
                "description": classify_element(elem),
            })

    return results


def bbox_area(bbox):
    """Area of a bounding box."""
    w = bbox[2] - bbox[0]
    h = bbox[3] - bbox[1]
    return max(0, w) * max(0, h)


def find_orphans(elements, tolerance):
    """
    For each element, find the nearest neighbor. If the nearest neighbor
    is farther than tolerance, it's an orphan.

    Large elements (top 15% by area in their group) are exempt — they are
    parent bodies/outlines, not sub-components that should attach to something.
    This prevents false positives for view outlines in ungrouped SVGs.

    Returns list of (element_info, nearest_info, distance).
    """
    orphans = []
    n = len(elements)

    # Compute area threshold per group: elements larger than the 85th
    # percentile are considered parent bodies and exempt from orphan checks
    groups = {}
    for e in elements:
        groups.setdefault(e["group_id"], []).append(e)

    area_thresholds = {}
    for gid, group_elems in groups.items():
        areas = sorted(bbox_area(e["bbox"]) for e in group_elems)
        if len(areas) >= 4:
            idx = int(len(areas) * 0.85)
            area_thresholds[gid] = areas[min(idx, len(areas) - 1)]
        else:
            # Small group — no exemption
            area_thresholds[gid] = float("inf")

    for i in range(n):
        ei = elements[i]
        gid = ei["group_id"]

        # Skip large parent bodies
        if bbox_area(ei["bbox"]) >= area_thresholds[gid]:
            continue

        min_dist = float("inf")
        nearest = None

        for j in range(n):
            if i == j:
                continue
            ej = elements[j]
            # Only check adjacency within the same group
            if ej["group_id"] != gid:
                continue

            dist = bbox_gap(ei["bbox"], ej["bbox"])
            if dist < min_dist:
                min_dist = dist
                nearest = ej

        if nearest is None:
            # Only element in its group — skip
            continue

        if min_dist > tolerance:
            orphans.append((ei, nearest, min_dist))

    return orphans


def main():
    parser = argparse.ArgumentParser(
        description="Check SVG engineering drawings for floating/disconnected elements"
    )
    parser.add_argument("svg_file", help="Path to SVG file")
    parser.add_argument(
        "--tolerance", type=float, default=2.0,
        help="Max gap (px) before an element is considered disconnected (default: 2)"
    )
    args = parser.parse_args()

    tree = ET.parse(args.svg_file)
    root = tree.getroot()

    # Strip namespace for easier processing
    for elem in root.iter():
        if "}" in elem.tag:
            elem.tag = elem.tag.split("}", 1)[1]

    elements = collect_elements(root)
    print(f"File: {args.svg_file}")
    print(f"Tolerance: {args.tolerance}px")
    print(f"Structural geometry elements found: {len(elements)}")
    print()

    # Group summary
    groups = {}
    for e in elements:
        gid = e["group_id"]
        groups.setdefault(gid, []).append(e)
    for gid, elems in sorted(groups.items()):
        print(f"  Group '{gid}': {len(elems)} elements")
    print()

    orphans = find_orphans(elements, args.tolerance)

    if not orphans:
        print("OK: All structural elements are adjacent to at least one neighbor "
              f"(within {args.tolerance}px tolerance).")
        return 0

    print(f"WARNINGS: {len(orphans)} element(s) appear disconnected:\n")

    # Group orphans by group_id
    by_group = {}
    for orphan, nearest, dist in orphans:
        gid = orphan["group_id"]
        by_group.setdefault(gid, []).append((orphan, nearest, dist))

    for gid, items in sorted(by_group.items()):
        print(f"  === Group: {gid} ===")
        for orphan, nearest, dist in sorted(items, key=lambda x: -x[2]):
            print(f"    FLOAT: {orphan['description']}")
            print(f"           bbox: {bbox_str(orphan['bbox'])}")
            print(f"           gap:  {dist:.1f}px to nearest element")
            print(f"           near: {nearest['description']}")
            print(f"                 bbox: {bbox_str(nearest['bbox'])}")
            print()

    return 1


if __name__ == "__main__":
    sys.exit(main())
