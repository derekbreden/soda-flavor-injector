#!/usr/bin/env python3
"""Upload a PNG image to the RP2040 display via ESP32 USB serial bridge.

Converts PNG to 128x115 RGB565 binary, then uploads via the chunked
binary protocol through the ESP32's transparent serial bridge.

Usage:
    python3 upload_image.py <image.png> [--slot N] [--port PORT]

Dependencies:
    pip install pyserial Pillow
"""

import argparse
import binascii
import glob
import struct
import sys
import time

import serial
from PIL import Image

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
RESP_COUNT = 0x14


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
    img = Image.open(path).convert("RGB")
    img = img.resize((SCREEN_W, SCREEN_H), Image.LANCZOS)
    pixels = list(img.getdata())
    result = bytearray()
    for r, g, b in pixels:
        rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
        result += struct.pack("<H", rgb565)
    return bytes(result)


def find_port():
    for pattern in ["/dev/cu.usbserial-*", "/dev/cu.SLAB_*", "/dev/ttyUSB*", "/dev/ttyACM*"]:
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


def read_binary_response(ser, timeout: float = 5.0) -> bytes:
    """Read a 6-byte binary response (STX STX code extra crc16)."""
    start = time.time()
    buf = bytearray()
    while time.time() - start < timeout:
        if ser.in_waiting:
            buf += ser.read(ser.in_waiting)
            # Look for STX STX pattern
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
    msg = struct.pack("<BBBI", STX, STX, CMD_UPLOAD_DONE, slot) + struct.pack(
        "<I", image_crc32
    )
    # Repack properly: STX STX CMD SLOT CRC32
    msg = bytes([STX, STX, CMD_UPLOAD_DONE, slot]) + struct.pack("<I", image_crc32)
    crc = crc16(msg)
    return msg + struct.pack("<H", crc)


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


def main():
    parser = argparse.ArgumentParser(
        description="Upload a PNG image to the RP2040 display via ESP32"
    )
    parser.add_argument("image", help="PNG image file to upload")
    parser.add_argument("--slot", type=int, default=None, help="Image slot (0-99)")
    parser.add_argument("--port", default=None, help="Serial port")
    args = parser.parse_args()

    port = args.port or find_port()
    if not port:
        print("ERROR: No serial port found. Use --port to specify.")
        sys.exit(1)

    print(f"Converting {args.image} to {SCREEN_W}x{SCREEN_H} RGB565...")
    image_data = png_to_rgb565(args.image)
    print(f"  {len(image_data)} bytes")

    print(f"Connecting to {port}...")
    ser = serial.Serial(port, BAUD_USB, timeout=2)
    time.sleep(2)  # wait for ESP32 boot messages
    ser.read(ser.in_waiting or 4096)  # drain

    if args.slot is None:
        resp = send_text_cmd(ser, "QUERY_IMAGES")
        if "NUM_IMAGES=" in resp:
            current = int(resp.split("NUM_IMAGES=")[1].split()[0])
            slot = current  # next available
            print(f"Current image count: {current}, uploading to slot {slot}")
        else:
            slot = 3  # safe default after the 3 seed images
            print(f"Could not query count ({resp}), defaulting to slot {slot}")
    else:
        slot = args.slot

    upload(ser, slot, image_data)
    ser.close()


if __name__ == "__main__":
    main()
