#!/usr/bin/env python3
"""Serial integration test for ESP32 config UART command parser.

Requires the ESP32 to be connected via USB. Automatically detects the port.
Run with: ~/.platformio/penv/bin/python3 tools/test_serial.py
"""

import serial
import serial.tools.list_ports
import sys
import time


def find_port():
    import glob
    # pyserial's list_ports sometimes misses /dev/cu.usbserial-* on macOS
    for pattern in ["/dev/cu.usbserial-*", "/dev/ttyUSB*", "/dev/ttyACM*"]:
        matches = sorted(glob.glob(pattern))
        if matches:
            return matches[0]
    for p in serial.tools.list_ports.comports():
        if "usbserial" in p.device or "USB" in p.description:
            return p.device
    return None


def send_cmd(ser, cmd):
    ser.write((cmd + "\n").encode())
    time.sleep(0.3)
    resp = ser.read(ser.in_waiting or 512).decode("utf-8", errors="replace").strip()
    return resp


def assert_contains(resp, expected, label):
    if expected in resp:
        print(f"  PASS: {label}")
    else:
        print(f"  FAIL: {label}")
        print(f"    expected '{expected}' in response")
        print(f"    got: {resp}")
        return False
    return True


def reboot(ser):
    ser.dtr = False
    ser.rts = True
    time.sleep(0.1)
    ser.rts = False
    time.sleep(2)
    ser.read(ser.in_waiting or 4096)  # drain boot messages


def main():
    port = find_port()
    if not port:
        print("ERROR: No USB serial device found. Is the ESP32 connected?")
        sys.exit(1)

    print(f"Using port: {port}")
    ser = serial.Serial(port, 115200, timeout=2)
    time.sleep(2)
    ser.read(ser.in_waiting or 4096)  # drain boot messages

    passed = 0
    failed = 0

    def check(resp, expected, label):
        nonlocal passed, failed
        if assert_contains(resp, expected, label):
            passed += 1
        else:
            failed += 1

    # --- Test defaults ---
    print("\n1. GET_CONFIG (defaults)")
    resp = send_cmd(ser, "GET_CONFIG")
    check(resp, "CONFIG:", "returns CONFIG response")
    check(resp, "NUM_IMAGES=3", "includes NUM_IMAGES")

    # --- Test SET commands ---
    print("\n2. SET commands")
    resp = send_cmd(ser, "SET:F1_RATIO=10")
    check(resp, "OK:F1_RATIO=10", "set F1_RATIO=10")

    resp = send_cmd(ser, "SET:F2_RATIO=15")
    check(resp, "OK:F2_RATIO=15", "set F2_RATIO=15")

    resp = send_cmd(ser, "SET:F1_IMAGE=2")
    check(resp, "OK:F1_IMAGE=2", "set F1_IMAGE=2")

    resp = send_cmd(ser, "SET:F2_IMAGE=0")
    check(resp, "OK:F2_IMAGE=0", "set F2_IMAGE=0")

    # --- Verify changes reflected ---
    print("\n3. GET_CONFIG (after SET)")
    resp = send_cmd(ser, "GET_CONFIG")
    check(resp, "F1_RATIO=10", "F1_RATIO updated")
    check(resp, "F2_RATIO=15", "F2_RATIO updated")
    check(resp, "F1_IMAGE=2", "F1_IMAGE updated")
    check(resp, "F2_IMAGE=0", "F2_IMAGE updated")

    # --- Range validation ---
    print("\n4. Range validation")
    resp = send_cmd(ser, "SET:F1_RATIO=30")
    check(resp, "ERR:", "rejects ratio > 24")

    resp = send_cmd(ser, "SET:F1_RATIO=5")
    check(resp, "ERR:", "rejects ratio < 6")

    resp = send_cmd(ser, "SET:F1_IMAGE=3")
    check(resp, "ERR:", "rejects image >= NUM_IMAGES")

    # --- Boundary values ---
    print("\n5. Boundary values")
    resp = send_cmd(ser, "SET:F1_RATIO=6")
    check(resp, "OK:F1_RATIO=6", "accepts ratio=6 (min)")

    resp = send_cmd(ser, "SET:F1_RATIO=24")
    check(resp, "OK:F1_RATIO=24", "accepts ratio=24 (max)")

    resp = send_cmd(ser, "SET:F1_IMAGE=0")
    check(resp, "OK:F1_IMAGE=0", "accepts image=0 (min)")

    resp = send_cmd(ser, "SET:F1_IMAGE=2")
    check(resp, "OK:F1_IMAGE=2", "accepts image=2 (max)")

    # --- SAVE + reboot persistence ---
    print("\n6. NVS persistence")
    send_cmd(ser, "SET:F1_RATIO=8")
    send_cmd(ser, "SET:F2_RATIO=12")
    send_cmd(ser, "SET:F1_IMAGE=1")
    send_cmd(ser, "SET:F2_IMAGE=2")
    resp = send_cmd(ser, "SAVE")
    check(resp, "OK:SAVED", "SAVE succeeds")

    reboot(ser)
    resp = send_cmd(ser, "GET_CONFIG")
    check(resp, "F1_RATIO=8", "F1_RATIO persisted after reboot")
    check(resp, "F2_RATIO=12", "F2_RATIO persisted after reboot")
    check(resp, "F1_IMAGE=1", "F1_IMAGE persisted after reboot")
    check(resp, "F2_IMAGE=2", "F2_IMAGE persisted after reboot")

    # --- Restore defaults ---
    print("\n7. Restore defaults")
    send_cmd(ser, "SET:F1_RATIO=20")
    send_cmd(ser, "SET:F2_RATIO=20")
    send_cmd(ser, "SET:F1_IMAGE=0")
    send_cmd(ser, "SET:F2_IMAGE=1")
    resp = send_cmd(ser, "SAVE")
    check(resp, "OK:SAVED", "defaults restored and saved")

    ser.close()

    # --- Summary ---
    total = passed + failed
    print(f"\n{'=' * 40}")
    print(f"Results: {passed}/{total} passed", end="")
    if failed:
        print(f", {failed} FAILED")
        sys.exit(1)
    else:
        print()
        sys.exit(0)


if __name__ == "__main__":
    main()
