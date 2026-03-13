#!/usr/bin/env python3
"""Manage images on the RP2040 display via ESP32 USB serial bridge.

Upload, delete, swap, list, and label images stored in the RP2040's LittleFS.

Usage:
    python3 upload_image.py <image.png> [--slot N] [--port PORT]
    python3 upload_image.py --list [--port PORT]
    python3 upload_image.py --delete N [--port PORT]
    python3 upload_image.py --swap A B [--port PORT]
    python3 upload_image.py --label N NAME [--port PORT]
    python3 upload_image.py --query [--port PORT]

Dependencies:
    pip install pyserial Pillow
"""

import argparse
import binascii
import glob
import struct
import sys
import time
from pathlib import Path

import serial

SCREEN_W = 128
SCREEN_H = 115
IMAGE_BYTES = SCREEN_W * SCREEN_H * 2  # 29440
CHUNK_SIZE = 128
BAUD_USB = 115200

# Protocol constants
STX = 0x02
CMD_UPLOAD_START = 0x01
CMD_CHUNK_DATA = 0x02
CMD_UPLOAD_DONE = 0x03
CMD_QUERY_COUNT = 0x04

RESP_READY = 0x10
RESP_CHUNK_OK = 0x11
RESP_UPLOAD_OK = 0x12
RESP_DELETE_OK = 0x13
RESP_COUNT = 0x14
RESP_SWAP_OK = 0x15


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021 if crc & 0x8000 else crc << 1) & 0xFFFF
    return crc


def crc32(data: bytes) -> int:
    return binascii.crc32(data) & 0xFFFFFFFF


def png_to_rgb565(path: str) -> bytes:
    from PIL import Image

    img = Image.open(path).convert("RGB")
    img = img.resize((SCREEN_W, SCREEN_H), Image.LANCZOS)
    pixels = list(img.getdata())
    result = bytearray()
    for r, g, b in pixels:
        rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
        result += struct.pack("<H", rgb565)
    return bytes(result)


def find_port():
    for pattern in [
        "/dev/cu.usbserial-*",
        "/dev/cu.SLAB_*",
        "/dev/ttyUSB*",
        "/dev/ttyACM*",
    ]:
        matches = sorted(glob.glob(pattern))
        if matches:
            return matches[0]
    return None


def send_text_cmd(ser, cmd: str) -> str:
    ser.reset_input_buffer()
    ser.write((cmd + "\n").encode())
    time.sleep(0.3)
    resp = ser.read(ser.in_waiting or 512).decode("utf-8", errors="replace").strip()
    return resp


def read_text_lines(ser, end_marker="END", timeout=3.0):
    """Read text lines from serial until end_marker or timeout."""
    lines = []
    start = time.time()
    buf = ""
    while time.time() - start < timeout:
        if ser.in_waiting:
            buf += ser.read(ser.in_waiting).decode("utf-8", errors="replace")
            while "\n" in buf:
                line, buf = buf.split("\n", 1)
                line = line.strip()
                if line == end_marker:
                    return lines
                if line:
                    lines.append(line)
                start = time.time()  # reset timeout on activity
        time.sleep(0.01)
    return lines


def read_binary_response(ser, timeout: float = 5.0) -> bytes:
    """Read a 6-byte binary response (STX STX code extra crc16)."""
    start = time.time()
    buf = bytearray()
    while time.time() - start < timeout:
        if ser.in_waiting:
            buf += ser.read(ser.in_waiting)
            idx = buf.find(bytes([STX, STX]))
            if idx >= 0 and len(buf) >= idx + 6:
                return bytes(buf[idx : idx + 6])
        time.sleep(0.01)
    return b""


def make_upload_start(slot: int, size: int) -> bytes:
    msg = struct.pack("<BBBBI", STX, STX, CMD_UPLOAD_START, slot, size)
    crc = crc16(msg)
    return msg + struct.pack("<H", crc)


def make_chunk(seq: int, data: bytes) -> bytes:
    msg = struct.pack("<BBBBH", STX, STX, CMD_CHUNK_DATA, seq, len(data)) + data
    crc = crc16(msg)
    return msg + struct.pack("<H", crc)


def make_upload_done(slot: int, image_crc32: int) -> bytes:
    msg = bytes([STX, STX, CMD_UPLOAD_DONE, slot]) + struct.pack("<I", image_crc32)
    crc = crc16(msg)
    return msg + struct.pack("<H", crc)


def connect(port_name):
    port = port_name or find_port()
    if not port:
        print("ERROR: No serial port found. Use --port to specify.")
        sys.exit(1)
    print(f"Connecting to {port}...")
    ser = serial.Serial(port, BAUD_USB, timeout=2)
    time.sleep(2)
    ser.read(ser.in_waiting or 4096)  # drain boot messages
    return ser


def query_count(ser) -> int:
    resp = send_text_cmd(ser, "QUERY_IMAGES")
    if "NUM_IMAGES=" in resp:
        return int(resp.split("NUM_IMAGES=")[1].split()[0])
    return -1


def list_images(ser):
    """Send LIST_IMAGES and display all slots with labels."""
    ser.reset_input_buffer()
    ser.write(b"LIST_IMAGES\n")
    time.sleep(0.3)

    lines = read_text_lines(ser, end_marker="END", timeout=3.0)
    if not lines:
        print("No images found (or RP2040 not responding)")
        return

    print(f"RP2040 images ({len(lines)} slots):")
    for line in lines:
        # Format: IMG:slot:label
        if line.startswith("IMG:"):
            parts = line.split(":", 2)
            if len(parts) == 3:
                slot = parts[1]
                label = parts[2] if parts[2] else "(unlabeled)"
                print(f"  Slot {slot}: {label}")
            else:
                print(f"  {line}")


def upload(ser, slot: int, image_data: bytes):
    assert len(image_data) == IMAGE_BYTES

    # Enter bridge mode on ESP32
    resp = send_text_cmd(ser, f"UPLOAD_IMG:{slot}")
    if "OK:BRIDGE_MODE" not in resp:
        print(f"ERROR: ESP32 rejected upload: {resp}")
        sys.exit(1)
    time.sleep(0.2)

    # Send UPLOAD_START
    ser.write(make_upload_start(slot, IMAGE_BYTES))
    resp = read_binary_response(ser, timeout=3.0)
    if len(resp) < 6 or resp[2] != RESP_READY:
        code = f"0x{resp[2]:02X}" if len(resp) >= 3 else "timeout"
        print(f"ERROR: RP2040 not ready: {code}")
        sys.exit(1)
    print(f"RP2040 ready for slot {slot}")

    # Send chunks
    seq = 0
    offset = 0
    total_chunks = (IMAGE_BYTES + CHUNK_SIZE - 1) // CHUNK_SIZE
    while offset < IMAGE_BYTES:
        chunk = image_data[offset : offset + CHUNK_SIZE]
        pkt = make_chunk(seq & 0xFF, chunk)

        for attempt in range(5):
            ser.write(pkt)
            resp = read_binary_response(ser, timeout=2.0)
            if len(resp) >= 6 and resp[2] == RESP_CHUNK_OK:
                break
            elif len(resp) >= 6:
                print(
                    f"\n  Error 0x{resp[2]:02X} on chunk {seq}, "
                    f"retrying ({attempt + 1}/5)"
                )
            else:
                print(f"\n  Timeout on chunk {seq}, retrying ({attempt + 1}/5)")
        else:
            print(f"\nFATAL: Failed to send chunk {seq} after 5 attempts")
            sys.exit(1)

        offset += CHUNK_SIZE
        seq += 1
        pct = min(100, int(seq * 100 / total_chunks))
        print(f"\r  [{pct:3d}%] Chunk {seq}/{total_chunks}", end="", flush=True)

    print()

    # Send UPLOAD_DONE with CRC-32 of entire image
    img_crc = crc32(image_data)
    ser.write(make_upload_done(slot, img_crc))
    resp = read_binary_response(ser, timeout=5.0)
    if len(resp) >= 6 and resp[2] == RESP_UPLOAD_OK:
        new_count = resp[3]
        print(f"Upload complete! RP2040 now has {new_count} images.")
    else:
        code = f"0x{resp[2]:02X}" if len(resp) >= 3 else "timeout"
        print(f"ERROR: Upload verification failed ({code})")
        sys.exit(1)

    # Wait for bridge mode to timeout on ESP32
    time.sleep(1)


def set_label(ser, slot: int, label: str):
    """Set a label for a slot via text command through ESP32."""
    resp = send_text_cmd(ser, f"SET_LABEL:{slot}={label}")
    if "OK:LABEL=" in resp:
        print(f"Label set: slot {slot} = {label}")
    else:
        print(f"Warning: could not set label ({resp})")


def delete_image(ser, slot: int):
    resp = send_text_cmd(ser, f"DELETE_IMG:{slot}")
    if "OK:DELETED=" in resp:
        print(resp)
    else:
        print(f"ERROR: {resp}")
        sys.exit(1)


def swap_images(ser, slot_a: int, slot_b: int):
    resp = send_text_cmd(ser, f"SWAP_IMG:{slot_a},{slot_b}")
    if "OK:SWAPPED=" in resp:
        print(resp)
    else:
        print(f"ERROR: {resp}")
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(
        description="Manage images on RP2040 display via ESP32 serial bridge"
    )
    parser.add_argument("image", nargs="?", default=None, help="PNG image to upload")
    parser.add_argument("--slot", type=int, default=None, help="Image slot (0-99)")
    parser.add_argument("--port", default=None, help="Serial port")
    parser.add_argument("--list", action="store_true",
                        help="List all images with labels")
    parser.add_argument("--delete", type=int, default=None, metavar="N",
                        help="Delete image at slot N (shifts remaining down)")
    parser.add_argument("--swap", type=int, nargs=2, default=None, metavar=("A", "B"),
                        help="Swap images at slots A and B")
    parser.add_argument("--label", nargs=2, default=None, metavar=("N", "NAME"),
                        help="Set label for slot N")
    parser.add_argument("--query", action="store_true",
                        help="Query current image count")
    args = parser.parse_args()

    # Validate arguments
    actions = sum([args.image is not None, args.delete is not None,
                   args.swap is not None, args.query, args.list,
                   args.label is not None])
    if actions == 0:
        parser.print_help()
        sys.exit(1)
    if actions > 1:
        print("ERROR: specify only one action")
        sys.exit(1)

    ser = connect(args.port)

    if args.list:
        list_images(ser)
        ser.close()
        return

    if args.query:
        count = query_count(ser)
        if count >= 0:
            print(f"RP2040 has {count} images (slots 0-{count - 1})")
        else:
            print("ERROR: could not query image count")
        ser.close()
        return

    if args.label is not None:
        slot = int(args.label[0])
        name = args.label[1]
        set_label(ser, slot, name)
        ser.close()
        return

    if args.delete is not None:
        delete_image(ser, args.delete)
        ser.close()
        return

    if args.swap is not None:
        swap_images(ser, args.swap[0], args.swap[1])
        ser.close()
        return

    # Upload mode
    print(f"Converting {args.image} to {SCREEN_W}x{SCREEN_H} RGB565...")
    image_data = png_to_rgb565(args.image)
    print(f"  {len(image_data)} bytes")

    if args.slot is None:
        count = query_count(ser)
        if count >= 0:
            slot = count  # next available
            print(f"Current image count: {count}, uploading to slot {slot}")
        else:
            slot = 3  # safe default after the 3 seed images
            print(f"Could not query count, defaulting to slot {slot}")
    else:
        slot = args.slot

    upload(ser, slot, image_data)

    # Auto-set label from filename (e.g., "orange_crush.png" → "orange_crush")
    label = Path(args.image).stem
    # After bridge mode exits, ESP32 is back to normal command mode
    time.sleep(6)  # wait for bridge mode 5s inactivity timeout
    ser.read(ser.in_waiting or 4096)  # drain any bridge exit messages
    set_label(ser, slot, label)

    ser.close()


if __name__ == "__main__":
    main()
