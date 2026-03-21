#pragma once

#include "uart_proto.h"

// ════════════════════════════════════════════════════════════
//  ProtoQueue: Non-blocking command queue for ProtoLink.
//  Manages application-level request/response flows on top
//  of TinyProto's reliable transport.
// ════════════════════════════════════════════════════════════

// ── Queue sizing ──
#define PROTOQUEUE_SIZE       16
#define PROTOQUEUE_TEXT_MAX  200
#define PROTOQUEUE_CHUNK_SIZE 512

// ── Timeouts (application-level only — transport retries are TinyProto's job) ──
#define PROTOQUEUE_READY_TIMEOUT   5000
#define PROTOQUEUE_DONE_TIMEOUT    5000
#define PROTOQUEUE_TEXT_TIMEOUT     2000
#define PROTOQUEUE_QUERY_TIMEOUT   2000
#define PROTOQUEUE_DELETE_TIMEOUT  5000
#define PROTOQUEUE_SWAP_TIMEOUT    5000

// ── Forward declaration ──
struct ProtoQueue;

// ── Callbacks ──
typedef void (*ProtoQueueUploadCb)(ProtoQueue *q, uint8_t slot, bool success);
typedef void (*ProtoQueueTextCb)(ProtoQueue *q, const char *response);
typedef void (*ProtoQueueQueryCb)(ProtoQueue *q, uint8_t count, bool success);
typedef void (*ProtoQueueDeleteCb)(ProtoQueue *q, uint8_t slot, bool success);
typedef void (*ProtoQueueSwapCb)(ProtoQueue *q, uint8_t slotA, uint8_t slotB, bool success);
typedef void (*ProtoQueueMessageCb)(ProtoQueue *q, uint8_t msgType, const uint8_t *payload, uint16_t len);

// ── Current operation state ──
enum QueueOp : uint8_t {
  OP_IDLE,
  OP_UPLOAD_WAIT_READY,
  OP_UPLOAD_SENDING,
  OP_UPLOAD_WAIT_DONE,
  OP_TEXT_WAIT_RESPONSE,
  OP_QUERY_WAIT_RESPONSE,
  OP_DELETE_WAIT_RESPONSE,
  OP_SWAP_WAIT_RESPONSE,
};

// ── Queue entry types ──
enum QueueEntryType : uint8_t {
  QE_NONE,
  QE_TEXT,           // text command (wait for text response)
  QE_TEXT_NOACK,     // fire-and-forget text
  QE_UPLOAD,         // file-based image upload
  QE_PNG_UPLOAD,     // file-based PNG upload
  QE_RP_UPLOAD,      // buffer-based RP2040 RGB565 upload
  QE_DELETE,
  QE_SWAP,
  QE_QUERY,
};

// ── Priority levels ──
#define QUEUE_PRI_HIGH   0  // prime tick, clean abort, config changes
#define QUEUE_PRI_NORMAL 1  // user-initiated operations
#define QUEUE_PRI_LOW    2  // image sync, boot sync

// ── Queue entry ──
struct QueueEntry {
  QueueEntryType type = QE_NONE;
  uint8_t priority = QUEUE_PRI_NORMAL;

  // Text
  char text[PROTOQUEUE_TEXT_MAX];

  // Upload
  char filePath[32];
  const uint8_t *bufPtr;
  uint32_t bufSize;
  uint8_t slot;
  uint8_t startMsgType;

  // Delete / Swap
  uint8_t slotA;
  uint8_t slotB;

  // Callbacks (per-entry)
  ProtoQueueUploadCb onUpload;
  ProtoQueueTextCb   onText;
  ProtoQueueQueryCb  onQuery;
  ProtoQueueDeleteCb onDelete;
  ProtoQueueSwapCb   onSwap;
};

// ════════════════════════════════════════════════════════════
//  ProtoQueue struct
// ════════════════════════════════════════════════════════════

struct ProtoQueue {
  ProtoLink *link;
  const char *name;

  // ── Current operation state ──
  QueueOp op = OP_IDLE;
  unsigned long sentAt = 0;
  unsigned long timeoutMs = 0;

  // ── Upload state ──
  uint8_t uploadSlot = 0;
  uint8_t uploadStartMsg = 0;
  uint32_t uploadOffset = 0;
  uint32_t uploadSize = 0;
  uint32_t uploadCrc = 0;
  bool uploadFromFile = false;
  char uploadPath[32];
  const uint8_t *uploadBufPtr = nullptr;

  // ── Current entry callbacks ──
  ProtoQueueUploadCb currentUploadCb = nullptr;
  ProtoQueueTextCb   currentTextCb = nullptr;
  ProtoQueueQueryCb  currentQueryCb = nullptr;
  ProtoQueueDeleteCb currentDeleteCb = nullptr;
  ProtoQueueSwapCb   currentSwapCb = nullptr;

  // ── Default callbacks ──
  ProtoQueueUploadCb defaultUploadCb = nullptr;
  ProtoQueueTextCb   defaultTextCb = nullptr;
  ProtoQueueQueryCb  defaultQueryCb = nullptr;
  ProtoQueueDeleteCb defaultDeleteCb = nullptr;
  ProtoQueueSwapCb   defaultSwapCb = nullptr;

  // ── Unsolicited message handler ──
  ProtoQueueMessageCb onMessage = nullptr;

  // ── Queue (array with priority scan) ──
  QueueEntry queue[PROTOQUEUE_SIZE];
  uint8_t queueCount = 0;

  // ── Last response value ──
  uint8_t lastResponseValue = 0;

  // ── Stats ──
  uint32_t totalSent = 0;
  uint32_t totalFailed = 0;

  // ────────────────────────────────────────────────
  //  Init
  // ────────────────────────────────────────────────
  void init(ProtoLink *_link, const char *_name) {
    link = _link;
    name = _name;
    op = OP_IDLE;
    queueCount = 0;
  }

  // ────────────────────────────────────────────────
  //  Queue operations
  // ────────────────────────────────────────────────

  bool queueFull() { return queueCount >= PROTOQUEUE_SIZE; }
  bool busy() { return op != OP_IDLE || queueCount > 0; }

  bool enqueue(const QueueEntry &entry) {
    if (queueFull()) {
      Serial.printf("[%s] Queue full, dropping %d\n", name, entry.type);
      return false;
    }
    queue[queueCount++] = entry;
    return true;
  }

  int dequeueIndex() {
    if (queueCount == 0) return -1;
    int bestIdx = 0;
    for (int i = 1; i < queueCount; i++) {
      if (queue[i].priority < queue[bestIdx].priority) bestIdx = i;
    }
    return bestIdx;
  }

  QueueEntry dequeue() {
    int idx = dequeueIndex();
    if (idx < 0) { QueueEntry e; e.type = QE_NONE; return e; }
    QueueEntry entry = queue[idx];
    for (int i = idx; i < queueCount - 1; i++) queue[i] = queue[i + 1];
    queueCount--;
    return entry;
  }

  bool hasHighPriority() {
    for (int i = 0; i < queueCount; i++) {
      if (queue[i].priority == QUEUE_PRI_HIGH) return true;
    }
    return false;
  }

  // ────────────────────────────────────────────────
  //  Public API: queue operations
  // ────────────────────────────────────────────────

  bool queueText(const char *cmd, bool needsAck = true, uint8_t priority = QUEUE_PRI_NORMAL,
                 ProtoQueueTextCb cb = nullptr) {
    QueueEntry e;
    e.type = needsAck ? QE_TEXT : QE_TEXT_NOACK;
    e.priority = priority;
    strncpy(e.text, cmd, PROTOQUEUE_TEXT_MAX - 1);
    e.text[PROTOQUEUE_TEXT_MAX - 1] = '\0';
    e.onText = cb;
    return enqueue(e);
  }

  bool queueUpload(uint8_t slot, const char *path, uint8_t startMsg = MSG_UPLOAD_START,
                   uint8_t priority = QUEUE_PRI_LOW, ProtoQueueUploadCb cb = nullptr) {
    QueueEntry e;
    e.type = (startMsg == MSG_UPLOAD_PNG_START) ? QE_PNG_UPLOAD : QE_UPLOAD;
    e.priority = priority;
    e.slot = slot;
    e.startMsgType = startMsg;
    strncpy(e.filePath, path, sizeof(e.filePath) - 1);
    e.filePath[sizeof(e.filePath) - 1] = '\0';
    e.bufPtr = nullptr;
    e.bufSize = 0;
    e.onUpload = cb;
    return enqueue(e);
  }

  bool queueBufferUpload(uint8_t slot, const uint8_t *data, uint32_t size,
                         uint8_t startMsg = MSG_UPLOAD_RP_START,
                         uint8_t priority = QUEUE_PRI_LOW, ProtoQueueUploadCb cb = nullptr) {
    QueueEntry e;
    e.type = QE_RP_UPLOAD;
    e.priority = priority;
    e.slot = slot;
    e.startMsgType = startMsg;
    e.bufPtr = data;
    e.bufSize = size;
    e.filePath[0] = '\0';
    e.onUpload = cb;
    return enqueue(e);
  }

  bool queueDelete(uint8_t slot, uint8_t priority = QUEUE_PRI_NORMAL,
                   ProtoQueueDeleteCb cb = nullptr) {
    QueueEntry e;
    e.type = QE_DELETE;
    e.priority = priority;
    e.slotA = slot;
    e.onDelete = cb;
    return enqueue(e);
  }

  bool queueSwap(uint8_t slotA, uint8_t slotB, uint8_t priority = QUEUE_PRI_NORMAL,
                 ProtoQueueSwapCb cb = nullptr) {
    QueueEntry e;
    e.type = QE_SWAP;
    e.priority = priority;
    e.slotA = slotA;
    e.slotB = slotB;
    e.onSwap = cb;
    return enqueue(e);
  }

  bool queueQuery(uint8_t priority = QUEUE_PRI_NORMAL, ProtoQueueQueryCb cb = nullptr) {
    QueueEntry e;
    e.type = QE_QUERY;
    e.priority = priority;
    e.onQuery = cb;
    return enqueue(e);
  }

  // ────────────────────────────────────────────────
  //  Service — call every loop() iteration
  // ────────────────────────────────────────────────

  void service() {
    // Check timeout on current operation
    if (op != OP_IDLE && (millis() - sentAt >= timeoutMs)) {
      handleTimeout();
    }

    // If idle, start next queued operation
    if (op == OP_IDLE && queueCount > 0) {
      dequeueAndStart();
    }

    // If sending chunks, send next one
    if (op == OP_UPLOAD_SENDING) {
      sendNextChunk();
    }
  }

  // Called by ProtoLink's onMessage handler to feed responses
  // back into the queue's state machine. Returns true if the
  // message was consumed by the current operation.
  bool handleIncoming(uint8_t msgType, const uint8_t *payload, uint16_t len) {
    if (op == OP_IDLE) return false;

    // Check if this is the expected response for current op
    switch (op) {
      case OP_UPLOAD_WAIT_READY:
        if (msgType == MSG_RESP_READY) {
          uploadOffset = 0;
          uploadCrc = 0;
          op = OP_UPLOAD_SENDING;
          return true;
        }
        if (msgType >= 0xE0 && msgType <= 0xEF) { failCurrentOp(); return true; }
        return false;

      case OP_UPLOAD_WAIT_DONE:
        if (msgType == MSG_RESP_UPLOAD_OK) {
          uint8_t newCount = (payload && len >= 1) ? payload[0] : 0;
          Serial.printf("[%s] Upload slot %d complete (count=%d)\n", name, uploadSlot, newCount);
          auto cb = currentUploadCb ? currentUploadCb : defaultUploadCb;
          op = OP_IDLE;
          totalSent++;
          if (cb) cb(this, uploadSlot, true);
          return true;
        }
        if (msgType >= 0xE0 && msgType <= 0xEF) { failCurrentOp(); return true; }
        return false;

      case OP_TEXT_WAIT_RESPONSE:
        if (msgType == MSG_TEXT) {
          char resp[PROTOQUEUE_TEXT_MAX];
          uint16_t respLen = (len > PROTOQUEUE_TEXT_MAX - 1) ? PROTOQUEUE_TEXT_MAX - 1 : len;
          if (payload && respLen > 0) memcpy(resp, payload, respLen);
          resp[respLen] = '\0';
          auto cb = currentTextCb ? currentTextCb : defaultTextCb;
          op = OP_IDLE;
          totalSent++;
          if (cb) cb(this, resp);
          return true;
        }
        return false;

      case OP_QUERY_WAIT_RESPONSE:
        if (msgType == MSG_RESP_COUNT) {
          uint8_t count = (payload && len >= 1) ? payload[0] : 0;
          auto cb = currentQueryCb ? currentQueryCb : defaultQueryCb;
          op = OP_IDLE;
          totalSent++;
          if (cb) cb(this, count, true);
          return true;
        }
        return false;

      case OP_DELETE_WAIT_RESPONSE:
        if (msgType == MSG_RESP_DELETE_OK) {
          lastResponseValue = (payload && len >= 1) ? payload[0] : 0;
          auto cb = currentDeleteCb ? currentDeleteCb : defaultDeleteCb;
          uint8_t slot = uploadSlot;
          op = OP_IDLE;
          totalSent++;
          if (cb) cb(this, slot, true);
          return true;
        }
        if (msgType >= 0xE0 && msgType <= 0xEF) { failCurrentOp(); return true; }
        return false;

      case OP_SWAP_WAIT_RESPONSE:
        if (msgType == MSG_RESP_SWAP_OK) {
          auto cb = currentSwapCb ? currentSwapCb : defaultSwapCb;
          uint8_t a = uploadSlot;
          uint8_t b = uploadCrc;  // reusing
          op = OP_IDLE;
          totalSent++;
          if (cb) cb(this, a, b, true);
          return true;
        }
        if (msgType >= 0xE0 && msgType <= 0xEF) { failCurrentOp(); return true; }
        return false;

      default:
        return false;
    }
  }

private:
  // ────────────────────────────────────────────────
  //  Timeout handling
  // ────────────────────────────────────────────────

  void handleTimeout() {
    Serial.printf("[%s] Timeout in op %d after %lums\n", name, op, timeoutMs);
    failCurrentOp();
  }

  void failCurrentOp() {
    totalFailed++;
    QueueOp failedOp = op;
    Serial.printf("[%s] FAIL op=%d slot=%d offset=%lu/%lu\n",
                  name, failedOp, uploadSlot, uploadOffset, uploadSize);
    op = OP_IDLE;

    switch (failedOp) {
      case OP_UPLOAD_WAIT_READY:
      case OP_UPLOAD_SENDING:
      case OP_UPLOAD_WAIT_DONE: {
        auto cb = currentUploadCb ? currentUploadCb : defaultUploadCb;
        if (cb) cb(this, uploadSlot, false);
        break;
      }
      case OP_TEXT_WAIT_RESPONSE: {
        auto cb = currentTextCb ? currentTextCb : defaultTextCb;
        if (cb) cb(this, nullptr);
        break;
      }
      case OP_QUERY_WAIT_RESPONSE: {
        auto cb = currentQueryCb ? currentQueryCb : defaultQueryCb;
        if (cb) cb(this, 0, false);
        break;
      }
      case OP_DELETE_WAIT_RESPONSE: {
        auto cb = currentDeleteCb ? currentDeleteCb : defaultDeleteCb;
        if (cb) cb(this, uploadSlot, false);
        break;
      }
      case OP_SWAP_WAIT_RESPONSE: {
        auto cb = currentSwapCb ? currentSwapCb : defaultSwapCb;
        if (cb) cb(this, uploadSlot, uploadCrc, false);
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
    if (e.type == QE_NONE) return;

    switch (e.type) {
      case QE_TEXT:
        startText(e.text, e.onText);
        break;
      case QE_TEXT_NOACK:
        link->sendText(e.text);
        totalSent++;
        break;
      case QE_UPLOAD:
      case QE_PNG_UPLOAD:
        startFileUpload(e.slot, e.filePath, e.startMsgType, e.onUpload);
        break;
      case QE_RP_UPLOAD:
        startBufferUpload(e.slot, e.bufPtr, e.bufSize, e.startMsgType, e.onUpload);
        break;
      case QE_DELETE:
        startDelete(e.slotA, e.onDelete);
        break;
      case QE_SWAP:
        startSwap(e.slotA, e.slotB, e.onSwap);
        break;
      case QE_QUERY:
        startQuery(e.onQuery);
        break;
      default: break;
    }
  }

  // ────────────────────────────────────────────────
  //  Text command (wait for response)
  // ────────────────────────────────────────────────

  void startText(const char *cmd, ProtoQueueTextCb cb) {
    currentTextCb = cb;
    link->sendText(cmd);
    op = OP_TEXT_WAIT_RESPONSE;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_TEXT_TIMEOUT;
  }

  // ────────────────────────────────────────────────
  //  File-based upload
  // ────────────────────────────────────────────────

  void startFileUpload(uint8_t slot, const char *path, uint8_t startMsg, ProtoQueueUploadCb cb) {
    currentUploadCb = cb;
    uploadSlot = slot;
    uploadStartMsg = startMsg;
    uploadFromFile = true;
    uploadBufPtr = nullptr;
    strncpy(uploadPath, path, sizeof(uploadPath) - 1);
    uploadPath[sizeof(uploadPath) - 1] = '\0';

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

    UploadStartPayload startPl{slot, uploadSize};
    link->send(startMsg, &startPl, sizeof(startPl));

    op = OP_UPLOAD_WAIT_READY;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_READY_TIMEOUT;
  }

  // ────────────────────────────────────────────────
  //  Buffer-based upload
  // ────────────────────────────────────────────────

  void startBufferUpload(uint8_t slot, const uint8_t *data, uint32_t size,
                         uint8_t startMsg, ProtoQueueUploadCb cb) {
    currentUploadCb = cb;
    uploadSlot = slot;
    uploadStartMsg = startMsg;
    uploadFromFile = false;
    uploadBufPtr = data;
    uploadSize = size;
    uploadPath[0] = '\0';

    UploadStartPayload startPl{slot, size};
    link->send(startMsg, &startPl, sizeof(startPl));

    op = OP_UPLOAD_WAIT_READY;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_READY_TIMEOUT;
  }

  // ────────────────────────────────────────────────
  //  Chunk sending (fire-and-forget, TinyProto handles reliability)
  // ────────────────────────────────────────────────

  uint16_t readChunk(uint32_t offset, uint8_t *buf, uint16_t maxLen) {
    uint32_t remaining = uploadSize - offset;
    uint16_t chunkLen = (remaining > maxLen) ? maxLen : (uint16_t)remaining;

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

  void sendNextChunk() {
    if (uploadOffset >= uploadSize) {
      // All chunks sent — send DONE with CRC-32
      UploadDonePayload donePl{uploadSlot, uploadCrc};
      link->send(MSG_UPLOAD_DONE, &donePl, sizeof(donePl));
      op = OP_UPLOAD_WAIT_DONE;
      sentAt = millis();
      timeoutMs = PROTOQUEUE_DONE_TIMEOUT;
      return;
    }

    // Check for high-priority interleaving before sending next chunk
    if (hasHighPriority()) {
      pauseAndRunPriority();
    }

    uint8_t chunkBuf[PROTOQUEUE_CHUNK_SIZE];
    uint16_t chunkLen = readChunk(uploadOffset, chunkBuf, PROTOQUEUE_CHUNK_SIZE);
    if (chunkLen == 0) {
      failCurrentOp();
      return;
    }

    int result = link->sendChunk(chunkBuf, chunkLen);
    if (result <= 0) {
      // TinyProto queue full — will retry next service() call
      return;
    }

    uploadCrc = uartCrc32Update(uploadCrc, chunkBuf, chunkLen);
    uploadOffset += chunkLen;

    // Log progress periodically
    if ((uploadOffset / PROTOQUEUE_CHUNK_SIZE) % 20 == 0) {
      Serial.printf("[%s] Upload: %lu/%lu bytes\n", name, uploadOffset, uploadSize);
    }
  }

  // ────────────────────────────────────────────────
  //  Priority interleaving during uploads
  // ────────────────────────────────────────────────

  void pauseAndRunPriority() {
    for (int i = 0; i < queueCount; ) {
      if (queue[i].priority == QUEUE_PRI_HIGH) {
        if (queue[i].type == QE_TEXT || queue[i].type == QE_TEXT_NOACK) {
          link->sendText(queue[i].text);
          totalSent++;
        }
        for (int j = i; j < queueCount - 1; j++) queue[j] = queue[j + 1];
        queueCount--;
      } else {
        i++;
      }
    }
  }

  // ────────────────────────────────────────────────
  //  Delete
  // ────────────────────────────────────────────────

  void startDelete(uint8_t slot, ProtoQueueDeleteCb cb) {
    currentDeleteCb = cb;
    uploadSlot = slot;
    SlotPayload pl{slot};
    link->send(MSG_DELETE_IMAGE, &pl, sizeof(pl));
    op = OP_DELETE_WAIT_RESPONSE;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_DELETE_TIMEOUT;
  }

  // ────────────────────────────────────────────────
  //  Swap
  // ────────────────────────────────────────────────

  void startSwap(uint8_t slotA, uint8_t slotB, ProtoQueueSwapCb cb) {
    currentSwapCb = cb;
    uploadSlot = slotA;
    uploadCrc = slotB;  // reuse field for swap partner
    SwapPayload pl{slotA, slotB};
    link->send(MSG_SWAP_IMAGES, &pl, sizeof(pl));
    op = OP_SWAP_WAIT_RESPONSE;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_SWAP_TIMEOUT;
  }

  // ────────────────────────────────────────────────
  //  Query count
  // ────────────────────────────────────────────────

  void startQuery(ProtoQueueQueryCb cb) {
    currentQueryCb = cb;
    link->sendEmpty(MSG_QUERY_COUNT);
    op = OP_QUERY_WAIT_RESPONSE;
    sentAt = millis();
    timeoutMs = PROTOQUEUE_QUERY_TIMEOUT;
  }

public:
  // Cancel current operation and clear queue.
  void cancelAll() {
    if (op != OP_IDLE) failCurrentOp();
    queueCount = 0;
  }
};
