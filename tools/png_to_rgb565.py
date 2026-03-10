#!/usr/bin/env python3
"""Convert PNG images to RGB565 C header files for ESP32-S3 display (240x240)."""

import sys
from pathlib import Path
from PIL import Image

SIZE = 240

IMAGES = [
    ("diet_wild_cherry_pepsi.png", "flavor0_240", "Diet Wild Cherry Pepsi"),
    ("diet_mtn_dew.png",          "flavor1_240", "Diet Mountain Dew"),
    ("diet_coke.png",             "flavor2_240", "Diet Coke"),
]

def rgb888_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert(src_path, var_name, label, out_dir):
    img = Image.open(src_path).convert("RGB")
    img = img.resize((SIZE, SIZE), Image.LANCZOS)

    pixels = list(img.getdata())
    hex_vals = [f"0x{rgb888_to_rgb565(r, g, b):04X}" for r, g, b in pixels]

    out_path = out_dir / f"{var_name}.h"
    with open(out_path, "w") as f:
        f.write(f"// {label} - {SIZE}x{SIZE} RGB565 bitmap\n")
        f.write(f"// Auto-generated from {src_path.name}\n")
        f.write("#pragma once\n\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"const uint16_t {var_name}[{SIZE} * {SIZE}] PROGMEM = {{\n")

        for i in range(0, len(hex_vals), 16):
            line = ", ".join(hex_vals[i:i+16])
            comma = "," if i + 16 < len(hex_vals) else ""
            f.write(f"    {line}{comma}\n")

        f.write("};\n")

    print(f"  {out_path.name}: {len(pixels)} pixels ({out_path.stat().st_size} bytes)")

def main():
    project = Path(__file__).resolve().parent.parent
    out_dir = project / "src_config" / "images"
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"Converting {len(IMAGES)} images to {SIZE}x{SIZE} RGB565 headers...")
    for filename, var_name, label in IMAGES:
        src = project / filename
        if not src.exists():
            print(f"  SKIP: {filename} not found")
            continue
        convert(src, var_name, label, out_dir)

    print("Done.")

if __name__ == "__main__":
    main()
