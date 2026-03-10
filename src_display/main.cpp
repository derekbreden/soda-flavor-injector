#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <LittleFS.h>
#include <SerialPIO.h>
#include "flavor1_bitmap.h"
#include "flavor2_bitmap.h"
#include "flavor3_bitmap.h"

// ── Flavor switch (same physical toggle as the ESP32 reads) ──
// Wire this to one of the exposed GPIOs on the SH1.0 connector.
#define FLAVOR_SW_PIN  29  // GP29 (ADC3) – adjust if you use a different pin

// ── UART from ESP32 (PIO-based, since GP26 is not a hardware UART pin) ──
#define UART_RX_PIN    26  // GP26 – receives config from ESP32
SerialPIO pioSerial(NOPIN, UART_RX_PIN);  // TX=none, RX=GP26

// ── Display wiring (fixed on the RP2040-LCD-0.99-B board) ──
#define LCD_DC   8
#define LCD_CS   9
#define LCD_CLK  10
#define LCD_DIN  11
#define LCD_RST  13
#define LCD_BL   25

#define SCREEN_W 128
#define SCREEN_H 115

// ── Display bus & driver ──
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(
    LCD_DC, LCD_CS, LCD_CLK, LCD_DIN, -1 /* MISO */, spi1);

Arduino_GFX *gfx = new Arduino_GC9107(
    bus, LCD_RST, 0 /* rotation */, true /* IPS */,
    SCREEN_W, SCREEN_H,
    0 /* col offset */, 13 /* row offset */);

// ── All available flavor images ──
// To add a new image: include the header, add pointer here.
//   0 = Diet Wild Cherry Pepsi
//   1 = Diet Mountain Dew
//   2 = Diet Coke
static const uint16_t *bitmaps[] = {
  flavor1_bitmap,   // index 0
  flavor2_bitmap,   // index 1
  flavor3_bitmap,   // index 2
};
static const uint8_t NUM_IMAGES = sizeof(bitmaps) / sizeof(bitmaps[0]);

// ── Image mapping: imageMap[flavorPosition] = bitmap index ──
static uint8_t imageMap[2] = { 0, 1 };   // defaults match original behavior
#define CONFIG_PATH "/config.txt"

// ── State ──
static int8_t activeFlavor = -1;  // force initial draw

// Forward declaration
void drawFlavor(uint8_t flavor);

// ────────────────────────────────────────────────────────────
//  LittleFS config persistence
// ────────────────────────────────────────────────────────────

void loadImageMap() {
  if (!LittleFS.exists(CONFIG_PATH)) return;
  File f = LittleFS.open(CONFIG_PATH, "r");
  if (!f) return;
  String line = f.readStringUntil('\n');
  f.close();
  int comma = line.indexOf(',');
  if (comma > 0) {
    uint8_t a = line.substring(0, comma).toInt();
    uint8_t b = line.substring(comma + 1).toInt();
    if (a < NUM_IMAGES && b < NUM_IMAGES) {
      imageMap[0] = a;
      imageMap[1] = b;
      Serial.printf("Loaded image map from flash: 0->%d, 1->%d\n", a, b);
    }
  }
}

void saveImageMap() {
  File f = LittleFS.open(CONFIG_PATH, "w");
  if (!f) return;
  f.printf("%d,%d\n", imageMap[0], imageMap[1]);
  f.close();
  Serial.printf("Saved image map to flash: 0->%d, 1->%d\n",
                imageMap[0], imageMap[1]);
}

// ────────────────────────────────────────────────────────────
//  UART command parsing
// ────────────────────────────────────────────────────────────

void checkUART() {
  if (!pioSerial.available()) return;
  String msg = pioSerial.readStringUntil('\n');
  msg.trim();
  if (!msg.startsWith("MAP:")) return;

  String payload = msg.substring(4);
  int comma = payload.indexOf(',');
  if (comma <= 0) return;

  uint8_t a = payload.substring(0, comma).toInt();
  uint8_t b = payload.substring(comma + 1).toInt();

  if (a >= NUM_IMAGES || b >= NUM_IMAGES) {
    Serial.printf("MAP rejected: index out of range (%d,%d), max=%d\n",
                  a, b, NUM_IMAGES - 1);
    return;
  }

  if (a != imageMap[0] || b != imageMap[1]) {
    imageMap[0] = a;
    imageMap[1] = b;
    saveImageMap();
    drawFlavor(activeFlavor);
    Serial.printf("MAP applied: 0->%d, 1->%d\n", a, b);
  }
}

// ────────────────────────────────────────────────────────────
void drawFlavor(uint8_t flavor) {
  gfx->draw16bitRGBBitmap(0, 0, bitmaps[imageMap[flavor]], SCREEN_W, SCREEN_H);
}

// ────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Init LittleFS and load saved mapping
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed, formatting...");
    LittleFS.format();
    LittleFS.begin();
  }
  loadImageMap();

  // UART from ESP32
  pioSerial.begin(9600);
  pioSerial.setTimeout(100);

  pinMode(FLAVOR_SW_PIN, INPUT_PULLUP);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  gfx->begin();

  // Read initial state and draw (using persisted mapping)
  activeFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;
  drawFlavor(activeFlavor);

  Serial.printf("Display ready – flavor %d selected (image %d)\n",
                activeFlavor + 1, imageMap[activeFlavor]);
}

void loop() {
  checkUART();

  uint8_t newFlavor = (digitalRead(FLAVOR_SW_PIN) == LOW) ? 1 : 0;

  if (newFlavor != activeFlavor) {
    activeFlavor = newFlavor;
    drawFlavor(activeFlavor);
    Serial.printf("Switched to flavor %d (image %d)\n",
                  activeFlavor + 1, imageMap[activeFlavor]);
  }

  delay(50);  // debounce / poll interval
}
