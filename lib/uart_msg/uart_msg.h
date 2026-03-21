#pragma once

#include <stdint.h>
#include <string.h>
#include <TinyProtocolFd.h>

// ════════════════════════════════════════════════════════════
//  Message type constants for inter-MCU protocol over TinyProto
// ════════════════════════════════════════════════════════════
//
// Each message is sent as a TinyProto I-frame with the message
// type in payload[0] and application data in payload[1..N].

// Commands (ESP32 → device)
constexpr uint8_t MSG_UPLOAD_START     = 0x01;
constexpr uint8_t MSG_CHUNK_DATA       = 0x02;
constexpr uint8_t MSG_UPLOAD_DONE      = 0x03;
constexpr uint8_t MSG_QUERY_COUNT      = 0x04;
constexpr uint8_t MSG_DELETE_IMAGE     = 0x05;
constexpr uint8_t MSG_SWAP_IMAGES      = 0x06;
constexpr uint8_t MSG_UPLOAD_PNG_START = 0x07;
constexpr uint8_t MSG_UPLOAD_RP_START  = 0x08;
constexpr uint8_t MSG_DEVICE_READY     = 0x09;  // device → ESP32: "I'm ready" + image count

// Responses (device → ESP32)
constexpr uint8_t MSG_RESP_READY       = 0x10;
constexpr uint8_t MSG_RESP_UPLOAD_OK   = 0x12;
constexpr uint8_t MSG_RESP_DELETE_OK   = 0x13;
constexpr uint8_t MSG_RESP_COUNT       = 0x14;
constexpr uint8_t MSG_RESP_SWAP_OK     = 0x15;

// Error responses (device → ESP32)
constexpr uint8_t MSG_ERR_SLOT_INVALID   = 0xE1;
constexpr uint8_t MSG_ERR_NO_SPACE       = 0xE2;
constexpr uint8_t MSG_ERR_BUSY           = 0xE3;
constexpr uint8_t MSG_ERR_WRITE          = 0xE6;
constexpr uint8_t MSG_ERR_SIZE_MISMATCH  = 0xE7;
constexpr uint8_t MSG_ERR_CRC32_MISMATCH = 0xE8;

// Text wrapper
constexpr uint8_t MSG_TEXT = 0xFE;

// ════════════════════════════════════════════════════════════
//  Payload structs (packed, little-endian)
// ════════════════════════════════════════════════════════════

struct __attribute__((packed)) UploadStartPayload {
  uint8_t  slot;
  uint32_t size;
};

struct __attribute__((packed)) UploadDonePayload {
  uint8_t  slot;
  uint32_t crc32;
};

struct __attribute__((packed)) SlotPayload {
  uint8_t slot;
};

struct __attribute__((packed)) SwapPayload {
  uint8_t slotA;
  uint8_t slotB;
};

struct __attribute__((packed)) ResponsePayload {
  uint8_t value;
};

// ════════════════════════════════════════════════════════════
//  CRC-32 (application-level, whole-image verification)
// ════════════════════════════════════════════════════════════

inline uint32_t uartCrc32Update(uint32_t prev, const uint8_t *data, size_t len) {
  uint32_t crc = ~prev;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
    }
  }
  return ~crc;
}

// ════════════════════════════════════════════════════════════
//  Send helpers — prepend message type byte to payload
// ════════════════════════════════════════════════════════════

// Send a message with type byte + binary payload.
// Returns result of IFd::write() (positive on success).
inline int protoSend(tinyproto::IFd &proto, uint8_t msgType, const void *data, uint16_t len) {
  uint8_t buf[len + 1];
  buf[0] = msgType;
  if (len > 0 && data) memcpy(buf + 1, data, len);
  return proto.write((const char *)buf, len + 1);
}

// Send a text string as MSG_TEXT.
inline int protoSendText(tinyproto::IFd &proto, const char *text) {
  uint16_t textLen = strlen(text);
  if (textLen > 510) textLen = 510;
  uint8_t buf[textLen + 1];
  buf[0] = MSG_TEXT;
  memcpy(buf + 1, text, textLen);
  return proto.write((const char *)buf, textLen + 1);
}

// Send a single-byte response (e.g., MSG_RESP_READY with count value).
inline int protoSendResponse(tinyproto::IFd &proto, uint8_t msgType, uint8_t value) {
  ResponsePayload resp{value};
  return protoSend(proto, msgType, &resp, sizeof(resp));
}

// Send a message with type byte only (no payload).
inline int protoSendEmpty(tinyproto::IFd &proto, uint8_t msgType) {
  return protoSend(proto, msgType, nullptr, 0);
}

// Send an upload data chunk (MSG_CHUNK_DATA + raw data).
inline int protoSendChunk(tinyproto::IFd &proto, const uint8_t *data, uint16_t len) {
  uint8_t buf[len + 1];
  buf[0] = MSG_CHUNK_DATA;
  memcpy(buf + 1, data, len);
  return proto.write((const char *)buf, len + 1);
}

// ════════════════════════════════════════════════════════════
//  Receive helpers — extract type and payload from frame
// ════════════════════════════════════════════════════════════

inline uint8_t msgType(const uint8_t *frame) { return frame[0]; }
inline const uint8_t *msgPayload(const uint8_t *frame) { return frame + 1; }
inline uint16_t msgPayloadLen(int frameLen) { return (frameLen > 1) ? frameLen - 1 : 0; }
