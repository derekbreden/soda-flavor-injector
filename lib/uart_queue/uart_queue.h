#pragma once
#include <SerialTransfer.h>
#include "uart_st.h"

// ════════════════════════════════════════════════════════════
//  UartLink: Non-blocking state machine + command queue for
//  SerialTransfer links. Replaces all blocking waitStResponse()
//  patterns with async, queue-based operations.
// ════════════════════════════════════════════════════════════

// ── Queue sizing ──
#define UARTLINK_QUEUE_SIZE   16
#define UARTLINK_TEXT_MAX    200   // max text command length
#define UARTLINK_CHUNK_SIZE  128   // matches STORE_CHUNK_SIZE / MAX_CHUNK_SIZE
#define UARTLINK_MAX_RETRIES   5
#define UARTLINK_CHUNK_TIMEOUT 2000
#define UARTLINK_READY_TIMEOUT 3000
#define UARTLINK_DONE_TIMEOUT  5000
#define UARTLINK_TEXT_TIMEOUT   500
#define UARTLINK_QUERY_TIMEOUT  500
#define UARTLINK_DELETE_TIMEOUT 3000
#define UARTLINK_SWAP_TIMEOUT   3000

// ── Forward declaration ──
struct UartLink;

// ── Callbacks ──
typedef void (*UartLinkUploadCb)(UartLink *link, uint8_t slot, bool success);
typedef void (*UartLinkTextCb)(UartLink *link, const char *response);
typedef void (*UartLinkQueryCb)(UartLink *link, uint8_t count, bool success);
typedef void (*UartLinkDeleteCb)(UartLink *link, uint8_t slot, bool success);
typedef void (*UartLinkSwapCb)(UartLink *link, uint8_t slotA, uint8_t slotB, bool success);
typedef void (*UartLinkPacketCb)(UartLink *link, uint8_t pktId);

// ── Current link operation ──
enum LinkOp : uint8_t {
  LINK_IDLE,
  LINK_UPLOAD_WAIT_READY,
  LINK_UPLOAD_SENDING_CHUNK,
  LINK_UPLOAD_WAIT_CHUNK_ACK,
  LINK_UPLOAD_WAIT_DONE_ACK,
  LINK_TEXT_WAIT_ACK,
  LINK_QUERY_WAIT_RESP,
  LINK_DELETE_WAIT_ACK,
  LINK_SWAP_WAIT_ACK,
};

// ── Queue entry types ──
enum QueueEntryType : uint8_t {
  Q_NONE,
  Q_TEXT,           // text command with ack (#seq:CMD)
  Q_TEXT_NOACK,     // fire-and-forget text (no #seq: prefix)
  Q_UPLOAD,         // file-based image upload (PKT_UPLOAD_START)
  Q_PNG_UPLOAD,     // file-based PNG upload (PKT_UPLOAD_PNG_START)
  Q_RP_UPLOAD,      // buffer-based RP2040 RGB565 upload (PKT_UPLOAD_RP_START)
  Q_DELETE,         // PKT_DELETE_IMAGE
  Q_SWAP,           // PKT_SWAP_IMAGES
  Q_QUERY,          // PKT_QUERY_COUNT
};

// ── Priority levels ──
#define UARTLINK_PRI_HIGH   0  // prime tick, clean abort, config changes
#define UARTLINK_PRI_NORMAL 1  // user-initiated operations
#define UARTLINK_PRI_LOW    2  // image sync, boot sync

// ── Queue entry ──
struct QueueEntry {
  QueueEntryType type = Q_NONE;
  uint8_t priority = UARTLINK_PRI_NORMAL;

  // Text
  char text[UARTLINK_TEXT_MAX];

  // Upload
  char filePath[32];
  const uint8_t *bufPtr;   // for buffer-based uploads (Q_RP_UPLOAD)
  uint32_t bufSize;         // for buffer-based uploads
  uint8_t slot;
  uint8_t startPktId;

  // Delete / Swap
  uint8_t slotA;
  uint8_t slotB;

  // Callbacks (per-entry, override link defaults if non-null)
  UartLinkUploadCb onUpload;
  UartLinkTextCb   onText;
  UartLinkQueryCb  onQuery;
  UartLinkDeleteCb onDelete;
  UartLinkSwapCb   onSwap;
};

// ── #seq: text ack parsing ──

// Parse #seq: prefix. Returns seq (0-255) or -1 if no prefix.
// On success, *cmdStart points past the "#seq:" prefix.
inline int stParseTextSeq(const char *text, const char **cmdStart) {
  if (text[0] != '#') return -1;
  const char *colon = strchr(text + 1, ':');
  if (!colon) return -1;
  // Parse the number between # and :
  char numBuf[8];
  int len = colon - text - 1;
  if (len <= 0 || len > 3) return -1;
  memcpy(numBuf, text + 1, len);
  numBuf[len] = '\0';
  int seq = atoi(numBuf);
  if (seq < 0 || seq > 255) return -1;
  *cmdStart = colon + 1;
  return seq;
}

// ════════════════════════════════════════════════════════════
//  UartLink struct
// ════════════════════════════════════════════════════════════

struct UartLink {
  SerialTransfer *st;
  const char *name;  // for debug logging ("S3", "RP2040", "ESP32")

  // ── Current operation state ──
  LinkOp op = LINK_IDLE;
  unsigned long sentAt = 0;
  unsigned long timeoutMs = 0;
  uint8_t retries = 0;
  uint8_t maxRetries = 0;
  uint8_t expectedPktId = 0;

  // ── Upload state (kept across loop iterations) ──
  uint8_t uploadSlot = 0;
  uint8_t uploadStartPktId = 0;
  uint32_t uploadOffset = 0;
  uint32_t uploadSize = 0;
  uint8_t uploadSeq = 0;
  uint32_t uploadCrc = 0;
  bool uploadFromFile = false;
  bool uploadFileOpen = false;
  // File handle — platform-specific. On ESP32/S3 this is LittleFS File.
  // We store it as a raw buffer and reconstruct via placement new.
  // Simpler: just keep a path and reopen + seek on each chunk.
  // Actually simplest: open once, keep open across loop iterations.
  // LittleFS File on ESP32 is a class with move semantics.
  // We'll store the path and reopen+seek each time (safe, ~1ms overhead).
  char uploadPath[32];
  const uint8_t *uploadBufPtr = nullptr;  // for buffer-based uploads

  // ── Text ack state ──
  uint8_t textSeqCounter = 0;
  char pendingTextCmd[UARTLINK_TEXT_MAX];
  int pendingTextSeq = -1;

  // ── Current entry callbacks ──
  UartLinkUploadCb currentUploadCb = nullptr;
  UartLinkTextCb   currentTextCb = nullptr;
  UartLinkQueryCb  currentQueryCb = nullptr;
  UartLinkDeleteCb currentDeleteCb = nullptr;
  UartLinkSwapCb   currentSwapCb = nullptr;

  // ── Default callbacks ──
  UartLinkUploadCb defaultUploadCb = nullptr;
  UartLinkTextCb   defaultTextCb = nullptr;
  UartLinkQueryCb  defaultQueryCb = nullptr;
  UartLinkDeleteCb defaultDeleteCb = nullptr;
  UartLinkSwapCb   defaultSwapCb = nullptr;

  // ── Unsolicited packet handler ──
  UartLinkPacketCb onPacket = nullptr;

  // ── Queue (ring buffer) ──
  QueueEntry queue[UARTLINK_QUEUE_SIZE];
  uint8_t queueCount = 0;

  // ── Last response value (readable from callbacks) ──
  uint8_t lastResponseValue = 0;

  // ── Stats ──
  uint32_t totalSent = 0;
  uint32_t totalRetries = 0;
  uint32_t totalFailed = 0;

  // ────────────────────────────────────────────────
  //  Init
  // ────────────────────────────────────────────────
  void init(SerialTransfer *_st, const char *_name) {
    st = _st;
    name = _name;
    op = LINK_IDLE;
    queueCount = 0;
    textSeqCounter = 0;
  }

  // ────────────────────────────────────────────────
  //  Queue operations
  // ────────────────────────────────────────────────

  bool queueFull() { return queueCount >= UARTLINK_QUEUE_SIZE; }
  bool busy() { return op != LINK_IDLE || queueCount > 0; }

  // Find insertion point (maintain priority order is not needed —
  // we scan for highest priority on dequeue instead)
  bool enqueue(const QueueEntry &entry) {
    if (queueFull()) {
      Serial.printf("[%s] Queue full, dropping %d\n", name, entry.type);
      return false;
    }
    queue[queueCount++] = entry;
    return true;
  }

  // Dequeue highest priority (lowest number) entry
  int dequeueIndex() {
    if (queueCount == 0) return -1;
    int bestIdx = 0;
    for (int i = 1; i < queueCount; i++) {
      if (queue[i].priority < queue[bestIdx].priority) {
        bestIdx = i;
      }
    }
    return bestIdx;
  }

  QueueEntry dequeue() {
    int idx = dequeueIndex();
    if (idx < 0) {
      QueueEntry empty;
      empty.type = Q_NONE;
      return empty;
    }
    QueueEntry entry = queue[idx];
    // Shift remaining entries down
    for (int i = idx; i < queueCount - 1; i++) {
      queue[i] = queue[i + 1];
    }
    queueCount--;
    return entry;
  }

  // Check if queue has any high-priority items waiting
  bool hasHighPriority() {
    for (int i = 0; i < queueCount; i++) {
      if (queue[i].priority == UARTLINK_PRI_HIGH) return true;
    }
    return false;
  }

  // ────────────────────────────────────────────────
  //  Public API: queue operations
  // ────────────────────────────────────────────────

  bool queueText(const char *cmd, bool needsAck = true, uint8_t priority = UARTLINK_PRI_NORMAL,
                 UartLinkTextCb cb = nullptr) {
    QueueEntry e;
    e.type = needsAck ? Q_TEXT : Q_TEXT_NOACK;
    e.priority = priority;
    strncpy(e.text, cmd, UARTLINK_TEXT_MAX - 1);
    e.text[UARTLINK_TEXT_MAX - 1] = '\0';
    e.onText = cb;
    return enqueue(e);
  }

  bool queueUpload(uint8_t slot, const char *path, uint8_t startPktId = PKT_UPLOAD_START,
                   uint8_t priority = UARTLINK_PRI_LOW, UartLinkUploadCb cb = nullptr) {
    QueueEntry e;
    e.type = (startPktId == PKT_UPLOAD_PNG_START) ? Q_PNG_UPLOAD : Q_UPLOAD;
    e.priority = priority;
    e.slot = slot;
    e.startPktId = startPktId;
    strncpy(e.filePath, path, sizeof(e.filePath) - 1);
    e.filePath[sizeof(e.filePath) - 1] = '\0';
    e.bufPtr = nullptr;
    e.bufSize = 0;
    e.onUpload = cb;
    return enqueue(e);
  }

  bool queueBufferUpload(uint8_t slot, const uint8_t *data, uint32_t size,
                         uint8_t startPktId = PKT_UPLOAD_RP_START,
                         uint8_t priority = UARTLINK_PRI_LOW, UartLinkUploadCb cb = nullptr) {
    QueueEntry e;
    e.type = Q_RP_UPLOAD;
    e.priority = priority;
    e.slot = slot;
    e.startPktId = startPktId;
    e.bufPtr = data;
    e.bufSize = size;
    e.filePath[0] = '\0';
    e.onUpload = cb;
    return enqueue(e);
  }

  bool queueDelete(uint8_t slot, uint8_t priority = UARTLINK_PRI_NORMAL,
                   UartLinkDeleteCb cb = nullptr) {
    QueueEntry e;
    e.type = Q_DELETE;
    e.priority = priority;
    e.slotA = slot;
    e.onDelete = cb;
    return enqueue(e);
  }

  bool queueSwap(uint8_t slotA, uint8_t slotB, uint8_t priority = UARTLINK_PRI_NORMAL,
                 UartLinkSwapCb cb = nullptr) {
    QueueEntry e;
    e.type = Q_SWAP;
    e.priority = priority;
    e.slotA = slotA;
    e.slotB = slotB;
    e.onSwap = cb;
    return enqueue(e);
  }

  bool queueQuery(uint8_t priority = UARTLINK_PRI_NORMAL, UartLinkQueryCb cb = nullptr) {
    QueueEntry e;
    e.type = Q_QUERY;
    e.priority = priority;
    e.onQuery = cb;
    return enqueue(e);
  }

  // ────────────────────────────────────────────────
  //  Service — call every loop() iteration
  // ────────────────────────────────────────────────

  void service() {
    // 1. Check for incoming packets
    if (st->available()) {
      handleIncoming(st->currentPacketID());
    }

    // 2. Check timeout on current operation
    if (op != LINK_IDLE && (millis() - sentAt >= timeoutMs)) {
      handleTimeout();
    }

    // 3. Between upload chunks: check for high-priority queue items
    if (op == LINK_UPLOAD_SENDING_CHUNK && hasHighPriority()) {
      // We just got a chunk ack and are about to send the next chunk.
      // Pause upload, let the high-priority item run first.
      // Actually, LINK_UPLOAD_SENDING_CHUNK means we're ready to send
      // the next chunk (not waiting for ack). We can defer and pick up
      // the high-priority item instead.
      // No — this state means we're about to sendChunk(). We should
      // send the high-priority item first, then come back.
      // But we can only have one op at a time. So we need to
      // interrupt the upload temporarily.
      // Simplest: just interleave — after chunk ack, if high-priority
      // item exists, dequeue and run it. When it completes, resume upload.
      // For now: we handle this in dequeueAndStart by not interrupting
      // in-flight uploads. High priority items wait until between chunks.
    }

    // 4. If idle, start next queued operation
    if (op == LINK_IDLE && queueCount > 0) {
      dequeueAndStart();
    }

    // Special: LINK_UPLOAD_SENDING_CHUNK = ready to send next chunk (no wait)
    if (op == LINK_UPLOAD_SENDING_CHUNK) {
      sendNextChunk();
    }
  }

private:

  // ────────────────────────────────────────────────
  //  Incoming packet dispatch
  // ────────────────────────────────────────────────

  void handleIncoming(uint8_t pktId) {
    if (op == LINK_IDLE) {
      // Not waiting for anything — pass to unsolicited handler
      if (onPacket) onPacket(this, pktId);
      return;
    }

    // Check if this is the expected response
    if (pktId == expectedPktId) {
      handleExpectedResponse(pktId);
      return;
    }

    // Check if it's an error packet
    if (pktId >= 0xE0 && pktId <= 0xEF) {
      handleErrorResponse(pktId);
      return;
    }

    // It's a text ack response while we're waiting for a binary ack?
    // Or a binary packet while waiting for text?
    // Either way: pass to unsolicited handler, don't consume the wait.
    if (pktId == PKT_TEXT && op != LINK_TEXT_WAIT_ACK) {
      // Unsolicited text while doing binary op — handle it
      if (onPacket) onPacket(this, pktId);
      return;
    }

    // Unexpected packet during wait — could be from previous stale op.
    // Pass to handler and continue waiting.
    if (onPacket) onPacket(this, pktId);
  }

  void handleExpectedResponse(uint8_t pktId) {
    switch (op) {
      case LINK_UPLOAD_WAIT_READY:
        // Device is ready — start sending chunks
        uploadOffset = 0;
        uploadSeq = 0;
        uploadCrc = 0;
        op = LINK_UPLOAD_SENDING_CHUNK;  // will send on next service()
        break;

      case LINK_UPLOAD_WAIT_CHUNK_ACK: {
        // Chunk accepted — advance offset past this chunk, send next
        uint16_t ackLen = uploadSize - uploadOffset;
        if (ackLen > UARTLINK_CHUNK_SIZE) ackLen = UARTLINK_CHUNK_SIZE;
        uploadOffset += ackLen;
        uploadSeq++;
        op = LINK_UPLOAD_SENDING_CHUNK;

        // Between chunks: check for high-priority queue items
        if (hasHighPriority()) {
          pauseUploadAndRunPriority();
        }
        break;
      }

      case LINK_UPLOAD_WAIT_DONE_ACK: {
        // Upload complete
        ResponsePayload resp;
        st->rxObj(resp);
        Serial.printf("[%s] Upload slot %d complete (count=%d)\n", name, uploadSlot, resp.value);
        auto cb = currentUploadCb ? currentUploadCb : defaultUploadCb;
        op = LINK_IDLE;
        totalSent++;
        if (cb) cb(this, uploadSlot, true);
        break;
      }

      case LINK_TEXT_WAIT_ACK: {
        // Read the text response
        char resp[UARTLINK_TEXT_MAX];
        uint16_t len = st->bytesRead;
        if (len > UARTLINK_TEXT_MAX - 1) len = UARTLINK_TEXT_MAX - 1;
        memcpy(resp, st->packet.rxBuff, len);
        resp[len] = '\0';

        // Strip #seq: prefix from response if present
        const char *body = resp;
        const char *stripped;
        int seq = stParseTextSeq(resp, &stripped);
        if (seq >= 0 && seq == pendingTextSeq) {
          body = stripped;
        }

        auto cb = currentTextCb ? currentTextCb : defaultTextCb;
        op = LINK_IDLE;
        totalSent++;
        if (cb) cb(this, body);
        break;
      }

      case LINK_QUERY_WAIT_RESP: {
        ResponsePayload resp;
        st->rxObj(resp);
        auto cb = currentQueryCb ? currentQueryCb : defaultQueryCb;
        op = LINK_IDLE;
        totalSent++;
        if (cb) cb(this, resp.value, true);
        break;
      }

      case LINK_DELETE_WAIT_ACK: {
        ResponsePayload resp;
        st->rxObj(resp);
        lastResponseValue = resp.value;
        auto cb = currentDeleteCb ? currentDeleteCb : defaultDeleteCb;
        uint8_t slot = uploadSlot;  // reusing for delete slot
        op = LINK_IDLE;
        totalSent++;
        if (cb) cb(this, slot, true);
        break;
      }

      case LINK_SWAP_WAIT_ACK: {
        auto cb = currentSwapCb ? currentSwapCb : defaultSwapCb;
        uint8_t a = uploadSlot;  // reusing
        uint8_t b = uploadSeq;   // reusing
        op = LINK_IDLE;
        totalSent++;
        if (cb) cb(this, a, b, true);
        break;
      }

      default:
        break;
    }
  }

  void handleErrorResponse(uint8_t pktId) {
    Serial.printf("[%s] Error 0x%02X during op %d\n", name, pktId, op);

    // For uploads, some errors are retryable (CRC, SEQ), others are fatal
    if (pktId == PKT_ERR_CRC || pktId == PKT_ERR_SEQ) {
      if (op == LINK_UPLOAD_WAIT_CHUNK_ACK && retries < maxRetries) {
        // Retry the chunk
        retries++;
        totalRetries++;
        resendCurrentChunk();
        return;
      }
    }

    // Fatal error — fail the operation
    failCurrentOp();
  }

  // ────────────────────────────────────────────────
  //  Timeout handling
  // ────────────────────────────────────────────────

  void handleTimeout() {
    if (op == LINK_UPLOAD_WAIT_CHUNK_ACK && retries < maxRetries) {
      retries++;
      totalRetries++;
      Serial.printf("[%s] Chunk %d timeout, retry %d\n", name, uploadSeq, retries);
      resendCurrentChunk();
      return;
    }

    if (op == LINK_TEXT_WAIT_ACK && retries < maxRetries) {
      retries++;
      totalRetries++;
      Serial.printf("[%s] Text ack timeout, retry %d: %.40s\n", name, retries, pendingTextCmd);
      sendTextPacket(pendingTextCmd, pendingTextSeq);
      sentAt = millis();
      return;
    }

    // All other timeouts are fatal
    Serial.printf("[%s] Timeout in op %d after %lums\n", name, op, timeoutMs);
    failCurrentOp();
  }

  void failCurrentOp() {
    totalFailed++;
    LinkOp failedOp = op;
    op = LINK_IDLE;

    switch (failedOp) {
      case LINK_UPLOAD_WAIT_READY:
      case LINK_UPLOAD_WAIT_CHUNK_ACK:
      case LINK_UPLOAD_WAIT_DONE_ACK:
      case LINK_UPLOAD_SENDING_CHUNK: {
        auto cb = currentUploadCb ? currentUploadCb : defaultUploadCb;
        if (cb) cb(this, uploadSlot, false);
        break;
      }
      case LINK_TEXT_WAIT_ACK: {
        auto cb = currentTextCb ? currentTextCb : defaultTextCb;
        if (cb) cb(this, nullptr);  // null = failed
        break;
      }
      case LINK_QUERY_WAIT_RESP: {
        auto cb = currentQueryCb ? currentQueryCb : defaultQueryCb;
        if (cb) cb(this, 0, false);
        break;
      }
      case LINK_DELETE_WAIT_ACK: {
        auto cb = currentDeleteCb ? currentDeleteCb : defaultDeleteCb;
        if (cb) cb(this, uploadSlot, false);
        break;
      }
      case LINK_SWAP_WAIT_ACK: {
        auto cb = currentSwapCb ? currentSwapCb : defaultSwapCb;
        if (cb) cb(this, uploadSlot, uploadSeq, false);
        break;
      }
      default: break;
    }
  }

  // ────────────────────────────────────────────────
  //  Start operations from queue
  // ────────────────────────────────────────────────

  void dequeueAndStart() {
    QueueEntry e = dequeue();
    if (e.type == Q_NONE) return;

    switch (e.type) {
      case Q_TEXT:
        startTextAck(e.text, e.onText);
        break;
      case Q_TEXT_NOACK:
        // Fire and forget — send immediately, stay IDLE
        stSendText(*st, e.text);
        totalSent++;
        break;
      case Q_UPLOAD:
      case Q_PNG_UPLOAD:
        startFileUpload(e.slot, e.filePath, e.startPktId, e.onUpload);
        break;
      case Q_RP_UPLOAD:
        startBufferUpload(e.slot, e.bufPtr, e.bufSize, e.startPktId, e.onUpload);
        break;
      case Q_DELETE:
        startDelete(e.slotA, e.onDelete);
        break;
      case Q_SWAP:
        startSwap(e.slotA, e.slotB, e.onSwap);
        break;
      case Q_QUERY:
        startQuery(e.onQuery);
        break;
      default: break;
    }
  }

  // ────────────────────────────────────────────────
  //  Text command with ack
  // ────────────────────────────────────────────────

  void startTextAck(const char *cmd, UartLinkTextCb cb) {
    currentTextCb = cb;
    pendingTextSeq = textSeqCounter++;
    strncpy(pendingTextCmd, cmd, UARTLINK_TEXT_MAX - 1);
    pendingTextCmd[UARTLINK_TEXT_MAX - 1] = '\0';

    sendTextPacket(cmd, pendingTextSeq);

    op = LINK_TEXT_WAIT_ACK;
    expectedPktId = PKT_TEXT;
    sentAt = millis();
    timeoutMs = UARTLINK_TEXT_TIMEOUT;
    retries = 0;
    maxRetries = 3;
  }

  void sendTextPacket(const char *cmd, int seq) {
    char buf[UARTLINK_TEXT_MAX + 8];
    if (seq >= 0) {
      snprintf(buf, sizeof(buf), "#%d:%s", seq, cmd);
    } else {
      strncpy(buf, cmd, sizeof(buf) - 1);
      buf[sizeof(buf) - 1] = '\0';
    }
    stSendText(*st, buf);
  }

  // ────────────────────────────────────────────────
  //  File-based upload
  // ────────────────────────────────────────────────

  void startFileUpload(uint8_t slot, const char *path, uint8_t startPktId, UartLinkUploadCb cb) {
    currentUploadCb = cb;
    uploadSlot = slot;
    uploadStartPktId = startPktId;
    uploadFromFile = true;
    uploadBufPtr = nullptr;
    strncpy(uploadPath, path, sizeof(uploadPath) - 1);
    uploadPath[sizeof(uploadPath) - 1] = '\0';

    // Get file size
    #if defined(ESP32) || defined(ESP_PLATFORM)
    File f = LittleFS.open(path, "r");
    if (!f) {
      Serial.printf("[%s] Upload: %s not found\n", name, path);
      if (cb) cb(this, slot, false);
      return;
    }
    uploadSize = f.size();
    f.close();
    #endif

    // Send UPLOAD_START
    UploadStartPayload startPl{slot, uploadSize};
    st->txObj(startPl);
    st->sendData(sizeof(startPl), startPktId);

    op = LINK_UPLOAD_WAIT_READY;
    expectedPktId = PKT_RESP_READY;
    sentAt = millis();
    timeoutMs = UARTLINK_READY_TIMEOUT;
    retries = 0;
    maxRetries = 0;  // no retry on READY
  }

  // ────────────────────────────────────────────────
  //  Buffer-based upload (for S3 → ESP32 forwarding)
  // ────────────────────────────────────────────────

  void startBufferUpload(uint8_t slot, const uint8_t *data, uint32_t size,
                         uint8_t startPktId, UartLinkUploadCb cb) {
    currentUploadCb = cb;
    uploadSlot = slot;
    uploadStartPktId = startPktId;
    uploadFromFile = false;
    uploadBufPtr = data;
    uploadSize = size;
    uploadPath[0] = '\0';

    UploadStartPayload startPl{slot, size};
    st->txObj(startPl);
    st->sendData(sizeof(startPl), startPktId);

    op = LINK_UPLOAD_WAIT_READY;
    expectedPktId = PKT_RESP_READY;
    sentAt = millis();
    timeoutMs = UARTLINK_READY_TIMEOUT;
    retries = 0;
    maxRetries = 0;
  }

  // ────────────────────────────────────────────────
  //  Chunk sending (non-blocking, one per service())
  // ────────────────────────────────────────────────

  // Read a chunk from file or buffer at the given offset.
  // Returns bytes read, or 0 on failure.
  uint16_t readChunk(uint32_t offset, uint8_t *buf, uint16_t maxLen) {
    uint16_t chunkLen = uploadSize - offset;
    if (chunkLen > maxLen) chunkLen = maxLen;

    if (uploadFromFile) {
      #if defined(ESP32) || defined(ESP_PLATFORM)
      File f = LittleFS.open(uploadPath, "r");
      if (!f) {
        Serial.printf("[%s] Upload: can't reopen %s\n", name, uploadPath);
        return 0;
      }
      f.seek(offset);
      int n = f.read(buf, chunkLen);
      f.close();
      if (n <= 0) return 0;
      return (uint16_t)n;
      #else
      return 0;
      #endif
    } else {
      memcpy(buf, uploadBufPtr + offset, chunkLen);
      return chunkLen;
    }
  }

  // uploadOffset = start of the chunk we're about to send (or just sent).
  // It advances ONLY after chunk ack, not before.
  void sendNextChunk() {
    if (uploadOffset >= uploadSize) {
      // All chunks sent — send DONE
      UploadDonePayload donePl{uploadSlot, uploadCrc};
      st->txObj(donePl);
      st->sendData(sizeof(donePl), PKT_UPLOAD_DONE);

      op = LINK_UPLOAD_WAIT_DONE_ACK;
      expectedPktId = PKT_RESP_UPLOAD_OK;
      sentAt = millis();
      timeoutMs = UARTLINK_DONE_TIMEOUT;
      retries = 0;
      maxRetries = 0;
      return;
    }

    uint8_t chunkBuf[UARTLINK_CHUNK_SIZE];
    uint16_t chunkLen = readChunk(uploadOffset, chunkBuf, UARTLINK_CHUNK_SIZE);
    if (chunkLen == 0) {
      failCurrentOp();
      return;
    }

    st->packet.txBuff[0] = uploadSeq;
    memcpy(st->packet.txBuff + 1, chunkBuf, chunkLen);
    st->sendData(1 + chunkLen, PKT_CHUNK_DATA);

    // CRC accumulated here on first send; retries use resendCurrentChunk()
    // which doesn't touch uploadCrc
    uploadCrc = uartCrc32Update(uploadCrc, chunkBuf, chunkLen);

    op = LINK_UPLOAD_WAIT_CHUNK_ACK;
    expectedPktId = PKT_RESP_CHUNK_OK;
    sentAt = millis();
    timeoutMs = UARTLINK_CHUNK_TIMEOUT;
    retries = 0;
    maxRetries = UARTLINK_MAX_RETRIES;
  }

  void resendCurrentChunk() {
    uint8_t chunkBuf[UARTLINK_CHUNK_SIZE];
    uint16_t chunkLen = readChunk(uploadOffset, chunkBuf, UARTLINK_CHUNK_SIZE);
    if (chunkLen == 0) {
      failCurrentOp();
      return;
    }

    st->packet.txBuff[0] = uploadSeq;
    memcpy(st->packet.txBuff + 1, chunkBuf, chunkLen);
    st->sendData(1 + chunkLen, PKT_CHUNK_DATA);

    sentAt = millis();
  }

  // ────────────────────────────────────────────────
  //  Priority interleaving during uploads
  // ────────────────────────────────────────────────

  // Stash upload state, run high-priority item, resume upload after
  // Implementation: we don't actually need to stash — the upload state
  // lives in the UartLink struct. We just need to run the priority item
  // and then resume. But we can only have one op at a time.
  //
  // Simplest approach: dequeue the high-priority text, send it as
  // fire-and-forget (even if it's Q_TEXT with ack), and continue the upload.
  // For prime ticks this is fine — they're already fire-and-forget.
  // For config commands that need ack, they should wait for the upload
  // to finish (they're not time-critical enough to need interleaving).
  //
  // Better approach: insert the high-priority item at the FRONT of the
  // queue, set op = LINK_IDLE temporarily, let dequeueAndStart() pick it
  // up, and when it completes, resume the upload. But we'd need to save
  // and restore upload state.
  //
  // For now: send high-priority texts as fire-and-forget between chunks.
  void pauseUploadAndRunPriority() {
    // Find and send high-priority items
    for (int i = 0; i < queueCount; ) {
      if (queue[i].priority == UARTLINK_PRI_HIGH) {
        // Send it now as fire-and-forget
        if (queue[i].type == Q_TEXT || queue[i].type == Q_TEXT_NOACK) {
          stSendText(*st, queue[i].text);
          totalSent++;
        }
        // Remove from queue
        for (int j = i; j < queueCount - 1; j++) {
          queue[j] = queue[j + 1];
        }
        queueCount--;
        // Don't increment i — next item shifted into this slot
      } else {
        i++;
      }
    }
    // Upload continues on next service() call
  }

  // ────────────────────────────────────────────────
  //  Delete
  // ────────────────────────────────────────────────

  void startDelete(uint8_t slot, UartLinkDeleteCb cb) {
    currentDeleteCb = cb;
    uploadSlot = slot;  // reuse field

    SlotPayload pl{slot};
    st->txObj(pl);
    st->sendData(sizeof(pl), PKT_DELETE_IMAGE);

    op = LINK_DELETE_WAIT_ACK;
    expectedPktId = PKT_RESP_DELETE_OK;
    sentAt = millis();
    timeoutMs = UARTLINK_DELETE_TIMEOUT;
    retries = 0;
    maxRetries = 0;
  }

  // ────────────────────────────────────────────────
  //  Swap
  // ────────────────────────────────────────────────

  void startSwap(uint8_t slotA, uint8_t slotB, UartLinkSwapCb cb) {
    currentSwapCb = cb;
    uploadSlot = slotA;  // reuse
    uploadSeq = slotB;   // reuse

    SwapPayload pl{slotA, slotB};
    st->txObj(pl);
    st->sendData(sizeof(pl), PKT_SWAP_IMAGES);

    op = LINK_SWAP_WAIT_ACK;
    expectedPktId = PKT_RESP_SWAP_OK;
    sentAt = millis();
    timeoutMs = UARTLINK_SWAP_TIMEOUT;
    retries = 0;
    maxRetries = 0;
  }

  // ────────────────────────────────────────────────
  //  Query count
  // ────────────────────────────────────────────────

  void startQuery(UartLinkQueryCb cb) {
    currentQueryCb = cb;

    st->packet.txBuff[0] = 0;
    st->sendData(1, PKT_QUERY_COUNT);

    op = LINK_QUERY_WAIT_RESP;
    expectedPktId = PKT_RESP_COUNT;
    sentAt = millis();
    timeoutMs = UARTLINK_QUERY_TIMEOUT;
    retries = 0;
    maxRetries = 0;
  }
};
