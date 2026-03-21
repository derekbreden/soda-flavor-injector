#pragma once

#include <TinyProtocolFd.h>
#include "uart_msg.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

// ════════════════════════════════════════════════════════════
//  ProtoLink: TinyProto Fd wrapper for Stream-based serial I/O
// ════════════════════════════════════════════════════════════
//
// Wraps a TinyProto Fd instance and a serial Stream. Call
// service() every loop() iteration to pump rx/tx. Incoming
// messages are dispatched to the onMessage callback with the
// message type byte already parsed out.

// MTU: 520 bytes covers 1 type byte + 512-byte chunk + headroom.
// Window: 4 frames allows pipelined transmission for throughput.
#define PROTOLINK_MTU     520
#define PROTOLINK_WINDOW  4

// Buffer size: computed by TinyProto's own formula for our MTU and window.
#define PROTOLINK_BUF_SIZE  tiny_fd_buffer_size_by_mtu(PROTOLINK_MTU, PROTOLINK_WINDOW)

struct ProtoLink {
  tinyproto::FdD proto{PROTOLINK_BUF_SIZE};
  Stream *serial = nullptr;
  const char *name = "";
  unsigned long lastStatusLog = 0;

  // Application callback — fires for each received message.
  // msgType is payload[0], payload points past the type byte,
  // len is the payload length (excluding type byte).
  void (*onMessage)(ProtoLink *link, uint8_t msgType,
                    const uint8_t *payload, uint16_t len) = nullptr;

  void begin(Stream &ser, const char *linkName) {
    serial = &ser;
    name = linkName;

    proto.enableCrc16();
    proto.setWindowSize(PROTOLINK_WINDOW);
    proto.setSendTimeout(0);  // non-blocking sends
    proto.setUserData(this);
    proto.setReceiveCallback(onReceiveStatic);
    proto.setConnectEventCallback(onConnectStatic);
    proto.begin();

    // Log the init result (IFd::begin doesn't expose the return value
    // from tiny_fd_init, so check status as a proxy)
    int status = proto.getStatus();
    Serial.printf("[%s] init: buf=%d bytes, status=%d\n",
                  name, PROTOLINK_BUF_SIZE, status);
  }

  void end() {
    proto.end();
  }

  // Call every loop() iteration to pump rx and tx.
  // Uses buffer-based API (not callback-based) because the
  // callback versions only process 4 bytes per call.
  void service() {
    if (!serial) return;

    // Periodic status logging (every 5s for first 30s)
    unsigned long now = millis();
    if (now < 30000 && now - lastStatusLog >= 5000) {
      lastStatusLog = now;
      Serial.printf("[%s] status=%d avail=%d\n", name, proto.getStatus(), serial->available());
    }

    // Read all available bytes from serial and feed to protocol
    int avail = serial->available();
    while (avail > 0) {
      uint8_t buf[256];
      int toRead = (avail > (int)sizeof(buf)) ? (int)sizeof(buf) : avail;
      int got = serial->readBytes(buf, toRead);
      if (got > 0) {
        proto.run_rx(buf, got);
      }
      avail -= got;
      if (got <= 0) break;
    }

    // Send all pending frames to serial (loop until empty)
    for (;;) {
      uint8_t txBuf[256];
      int len = proto.run_tx(txBuf, sizeof(txBuf));
      if (len <= 0) break;
      serial->write(txBuf, len);
    }
  }

  // Send a message (convenience wrapper).
  int send(uint8_t msgType, const void *data, uint16_t len) {
    int r = protoSend(proto, msgType, data, len);
    if (r < 0) Serial.printf("[%s] send(0x%02X, %u bytes) FAILED: %d\n", name, msgType, len, r);
    return r;
  }

  int sendText(const char *text) {
    int r = protoSendText(proto, text);
    if (r < 0) Serial.printf("[%s] sendText(%s) FAILED: %d\n", name, text, r);
    return r;
  }

  int sendResponse(uint8_t msgType, uint8_t value) {
    int r = protoSendResponse(proto, msgType, value);
    if (r < 0) Serial.printf("[%s] sendResponse(0x%02X) FAILED: %d\n", name, msgType, r);
    return r;
  }

  int sendEmpty(uint8_t msgType) {
    int r = protoSendEmpty(proto, msgType);
    if (r < 0) Serial.printf("[%s] sendEmpty(0x%02X) FAILED: %d\n", name, msgType, r);
    return r;
  }

  int sendChunk(const uint8_t *data, uint16_t len) {
    int r = protoSendChunk(proto, data, len);
    if (r < 0) Serial.printf("[%s] sendChunk(%u bytes) FAILED: %d\n", name, len, r);
    return r;
  }

  bool isConnected() {
    return proto.getStatus() == TINY_SUCCESS;
  }

private:
  // TinyProto receive callback — parses message type and dispatches
  static void onReceiveStatic(void *userData, uint8_t addr, tinyproto::IPacket &pkt) {
    ProtoLink *self = (ProtoLink *)userData;
    if (!self->onMessage) return;

    int pktSize = pkt.size();
    if (pktSize < 1) return;

    uint8_t *data = (uint8_t *)pkt.data();
    uint8_t type = data[0];
    const uint8_t *payload = (pktSize > 1) ? &data[1] : nullptr;
    uint16_t payloadLen = (pktSize > 1) ? pktSize - 1 : 0;

    self->onMessage(self, type, payload, payloadLen);
  }

  // TinyProto connect/disconnect callback
  static void onConnectStatic(void *userData, uint8_t addr, bool connected) {
    ProtoLink *self = (ProtoLink *)userData;
    Serial.printf("[%s] %s\n", self->name, connected ? "connected" : "disconnected");
  }
};
