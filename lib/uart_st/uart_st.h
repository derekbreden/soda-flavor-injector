#pragma once
#include <SerialTransfer.h>

// ════════════════════════════════════════════════════════════
//  Shared UART packet IDs for SerialTransfer inter-MCU links
// ════════════════════════════════════════════════════════════
//
// SerialTransfer uses COBS encoding with 0x7E start / 0x81 stop bytes.
// Packet ID is passed to sendData() and returned by available().

// Commands (ESP32 → device)
constexpr uint8_t PKT_UPLOAD_START     = 0x01;
constexpr uint8_t PKT_CHUNK_DATA       = 0x02;
constexpr uint8_t PKT_UPLOAD_DONE      = 0x03;
constexpr uint8_t PKT_QUERY_COUNT      = 0x04;
constexpr uint8_t PKT_DELETE_IMAGE     = 0x05;
constexpr uint8_t PKT_SWAP_IMAGES      = 0x06;
constexpr uint8_t PKT_UPLOAD_PNG_START = 0x07;
constexpr uint8_t PKT_UPLOAD_RP_START  = 0x08;

// Responses (device → ESP32)
constexpr uint8_t PKT_RESP_READY       = 0x10;
constexpr uint8_t PKT_RESP_CHUNK_OK    = 0x11;
constexpr uint8_t PKT_RESP_UPLOAD_OK   = 0x12;
constexpr uint8_t PKT_RESP_DELETE_OK   = 0x13;
constexpr uint8_t PKT_RESP_COUNT       = 0x14;
constexpr uint8_t PKT_RESP_SWAP_OK     = 0x15;

// Error responses (device → ESP32)
constexpr uint8_t PKT_ERR_SLOT_INVALID   = 0xE1;
constexpr uint8_t PKT_ERR_NO_SPACE       = 0xE2;
constexpr uint8_t PKT_ERR_BUSY           = 0xE3;
constexpr uint8_t PKT_ERR_CRC            = 0xE4;
constexpr uint8_t PKT_ERR_SEQ            = 0xE5;
constexpr uint8_t PKT_ERR_WRITE          = 0xE6;
constexpr uint8_t PKT_ERR_SIZE_MISMATCH  = 0xE7;
constexpr uint8_t PKT_ERR_CRC32_MISMATCH = 0xE8;

// Text wrapper
constexpr uint8_t PKT_TEXT = 0xFE;

// ════════════════════════════════════════════════════════════
//  Payload structs (packed, little-endian)
// ════════════════════════════════════════════════════════════

struct __attribute__((packed)) UploadStartPayload {
  uint8_t  slot;
  uint32_t size;
};

struct __attribute__((packed)) ChunkDataPayload {
  uint8_t seq;
  // Followed by N bytes of data (up to 128)
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
//  Helper functions
// ════════════════════════════════════════════════════════════

// Send a text string wrapped in a PKT_TEXT packet
inline void stSendText(SerialTransfer &st, const char *text) {
  uint16_t len = strlen(text);
  if (len > 254) len = 254;  // SerialTransfer max payload
  memcpy(st.packet.txBuff, text, len);
  st.sendData(len, PKT_TEXT);
}

// Send a single-byte response packet
inline void stSendResponse(SerialTransfer &st, uint8_t pktId, uint8_t value) {
  ResponsePayload resp;
  resp.value = value;
  st.txObj(resp);
  st.sendData(sizeof(resp), pktId);
}

// Send a response packet with no meaningful payload
// (SerialTransfer rejects zero-length payloads, so send 1 dummy byte)
inline void stSendEmptyResponse(SerialTransfer &st, uint8_t pktId) {
  st.packet.txBuff[0] = 0;
  st.sendData(1, pktId);
}
