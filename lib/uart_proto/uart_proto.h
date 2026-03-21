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

// Buffer size: generous estimate for Fd protocol internals.
// FD_MIN_BUF_SIZE lives in an internal header that pulls in too many
// dependencies, so we compute a safe over-estimate: ~4KB per window
// slot covers frame headers, CRC, and HDLC overhead.
#define PROTOLINK_BUF_SIZE  (4096 * PROTOLINK_WINDOW)

struct ProtoLink {
  tinyproto::FdD proto{PROTOLINK_BUF_SIZE};
  Stream *serial = nullptr;
  const char *name = "";

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
  }

  void end() {
    proto.end();
  }

  // Call every loop() iteration to pump rx and tx.
  void service() {
    if (!serial) return;

    // Read available bytes from serial and feed to protocol
    proto.run_rx(readStatic);

    // Send pending frames to serial
    proto.run_tx(writeStatic);
  }

  // Send a message (convenience wrapper).
  int send(uint8_t msgType, const void *data, uint16_t len) {
    return protoSend(proto, msgType, data, len);
  }

  int sendText(const char *text) {
    return protoSendText(proto, text);
  }

  int sendResponse(uint8_t msgType, uint8_t value) {
    return protoSendResponse(proto, msgType, value);
  }

  int sendEmpty(uint8_t msgType) {
    return protoSendEmpty(proto, msgType);
  }

  int sendChunk(const uint8_t *data, uint16_t len) {
    return protoSendChunk(proto, data, len);
  }

  bool isConnected() {
    return proto.getStatus() == TINY_SUCCESS;
  }

private:
  // TinyProto read callback — reads from the Stream
  static int readStatic(void *pdata, void *buffer, int size) {
    ProtoLink *self = (ProtoLink *)pdata;
    if (!self->serial) return 0;
    int avail = self->serial->available();
    if (avail <= 0) return 0;
    if (avail > size) avail = size;
    return self->serial->readBytes((uint8_t *)buffer, avail);
  }

  // TinyProto write callback — writes to the Stream
  static int writeStatic(void *pdata, const void *buffer, int size) {
    ProtoLink *self = (ProtoLink *)pdata;
    if (!self->serial) return 0;
    return self->serial->write((const uint8_t *)buffer, size);
  }

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
