#!/usr/bin/env python3
"""
Auto-reconnecting serial logger for ESP32 and ESP32-S3.

Watches for known USB serial devices, logs their output to files in logs/,
and auto-reconnects on disconnect or reboot. Pauses when /tmp/serial_logger_pause
exists (used by tools/flash.sh to release ports during flashing).

Usage:
    python3 tools/serial_logger.py          # log to files + stdout
    python3 tools/serial_logger.py --quiet  # log to files only

Logs written to:
    logs/esp32.log
    logs/s3.log
"""

import argparse
import glob
import os
import sys
import threading
import time
from datetime import datetime
from pathlib import Path

import serial
import serial.tools.list_ports

# ── Device definitions ──────────────────────────────────────────────
# Each entry: (name, vid, pid, baud, logfile)
DEVICES = [
    {
        "name": "ESP32",
        "vid": 0x10C4,   # Silicon Labs CP210x
        "pid": 0xEA60,
        "baud": 115200,
        "logfile": "esp32.log",
        "fallback_glob": "/dev/cu.usbserial-*",
    },
    {
        "name": "S3",
        "vid": 0x303A,   # Espressif
        "pid": 0x1001,   # USB JTAG/serial debug unit
        "baud": 115200,
        "logfile": "s3.log",
        "fallback_glob": "/dev/cu.usbmodem*",
    },
]

PAUSE_FILE = "/tmp/serial_logger_pause"
RECONNECT_INTERVAL = 1.0  # seconds between reconnect attempts
LOG_DIR = Path(__file__).parent.parent / "logs"


def find_device_port(device):
    """Find the serial port for a device by VID/PID, falling back to glob."""
    for p in serial.tools.list_ports.comports():
        if p.vid == device["vid"] and p.pid == device["pid"]:
            return p.device

    # Fallback: glob pattern
    matches = sorted(glob.glob(device["fallback_glob"]))
    if matches:
        return matches[0]
    return None


def status(msg):
    """Print a timestamped status message."""
    ts = datetime.now().strftime("%H:%M:%S")
    print(f"[{ts}] {msg}", flush=True)


class DeviceLogger(threading.Thread):
    """Thread that monitors and logs a single serial device."""

    def __init__(self, device, log_dir, quiet=False):
        super().__init__(daemon=True)
        self.device = device
        self.quiet = quiet
        self.log_path = log_dir / device["logfile"]
        self.port = None
        self.ser = None
        self.running = True
        self.connected = False

    def run(self):
        while self.running:
            # Check for pause
            if os.path.exists(PAUSE_FILE):
                if self.connected:
                    self._disconnect("paused (flash in progress)")
                time.sleep(0.5)
                continue

            # Try to connect if not connected
            if not self.connected:
                port = find_device_port(self.device)
                if port:
                    try:
                        # Open without auto-open to prevent DTR toggle (reboot)
                        self.ser = serial.Serial()
                        self.ser.port = port
                        self.ser.baudrate = self.device["baud"]
                        self.ser.timeout = 0.5
                        self.ser.dsrdtr = False
                        self.ser.rtscts = False
                        self.ser.dtr = False
                        self.ser.rts = False
                        self.ser.open()
                        self.port = port
                        self.connected = True
                        status(f"{self.device['name']}: connected on {port}")
                        # Write separator to log file
                        with open(self.log_path, "a") as f:
                            ts = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                            f.write(f"\n[{ts}] ── connected on {port} ──\n")
                    except (serial.SerialException, OSError) as e:
                        time.sleep(RECONNECT_INTERVAL)
                        continue
                else:
                    time.sleep(RECONNECT_INTERVAL)
                    continue

            # Read lines
            try:
                line = self.ser.readline()
                if line:
                    text = line.decode("utf-8", errors="replace").rstrip("\r\n")
                    ts = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                    log_line = f"[{ts}] {text}\n"

                    # Append to log file
                    with open(self.log_path, "a") as f:
                        f.write(log_line)

                    # Print to stdout
                    if not self.quiet:
                        prefix = self.device["name"].ljust(5)
                        print(f"  {prefix} | {text}", flush=True)

            except (serial.SerialException, OSError):
                self._disconnect("disconnected")
                time.sleep(RECONNECT_INTERVAL)

    def _disconnect(self, reason):
        if self.connected:
            status(f"{self.device['name']}: {reason}")
            self.connected = False
        if self.ser:
            try:
                self.ser.close()
            except Exception:
                pass
            self.ser = None

    def stop(self):
        self.running = False
        self._disconnect("stopped")


def main():
    parser = argparse.ArgumentParser(description="Auto-reconnecting serial logger")
    parser.add_argument("--quiet", "-q", action="store_true",
                        help="Log to files only, no stdout")
    args = parser.parse_args()

    # Ensure log directory exists
    LOG_DIR.mkdir(exist_ok=True)

    # Remove stale pause file
    if os.path.exists(PAUSE_FILE):
        os.remove(PAUSE_FILE)
        status("Removed stale pause file")

    status("Serial logger starting...")
    status(f"Logs: {LOG_DIR}/")
    status(f"Pause file: {PAUSE_FILE}")
    status("Press Ctrl+C to stop")
    print()

    # Start a logger thread per device
    loggers = []
    for device in DEVICES:
        logger = DeviceLogger(device, LOG_DIR, quiet=args.quiet)
        logger.start()
        loggers.append(logger)

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        status("Shutting down...")
        for logger in loggers:
            logger.stop()


if __name__ == "__main__":
    main()
