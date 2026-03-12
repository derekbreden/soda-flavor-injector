#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Adafruit_NeoPixel.h>

// ════════════════════════════════════════════════════════════
//  ESP32-S3 Config Display — Soda Flavor Injector
// ════════════════════════════════════════════════════════════
// Pin assignments for Elecrow CrowPanel 1.28" ESP32-S3 Rotary Display

// ── Display SPI (GC9A01A, 240x240) ──
#define TFT_BLK  46
#define TFT_RST  14
#define TFT_CS    9
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_DC    3

// ── Rotary Encoder ──
#define ENCODER_CLK 45
#define ENCODER_DT  42
#define ENCODER_BTN 41

// ── Touch (CST816D, not used yet) ──
#define TOUCH_SDA  6
#define TOUCH_SCL  7
#define TOUCH_INT  5
#define TOUCH_RST 13

// ── RGB LEDs (WS2812, 5 units) ──
#define LED_DATA  48
#define LED_COUNT  5

// ── Flavor images (240x240 RGB565) ──
#include "images/flavor0_240.h"
#include "images/flavor1_240.h"
#include "images/flavor2_240.h"

#define NUM_IMAGES 3
const uint16_t* images[NUM_IMAGES] = { flavor0_240, flavor1_240, flavor2_240 };
const char* imageNames[NUM_IMAGES] = { "Pepsi", "MTN Dew", "Coke" };

Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED, FSPI, true);
Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, TFT_RST, 0, true);

Adafruit_NeoPixel leds(LED_COUNT, LED_DATA, NEO_GRB + NEO_KHZ800);

// ── Encoder state ──
int encoderPos = 0;
int lastClk = HIGH;

void showImage(int idx) {
  gfx->draw16bitRGBBitmap(0, 0, images[idx], 240, 240);
  Serial.printf("Showing image %d: %s\n", idx, imageNames[idx]);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32-S3 Config Display starting...");

  // LEDs - light up green immediately to confirm firmware is running
  leds.begin();
  leds.setBrightness(30);
  for (int i = 0; i < LED_COUNT; i++) {
    leds.setPixelColor(i, leds.Color(0, 255, 0));
  }
  leds.show();
  Serial.println("LEDs set to green.");

  // Power pins (required by CrowPanel hardware)
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);   // Power indicator LED off
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);   // Display power enable 1
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);   // Display power enable 2

  // Backlight
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);

  // Encoder
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);

  // Display init
  gfx->begin();
  gfx->fillScreen(0x0000);

  // Show first image
  showImage(0);
  lastClk = digitalRead(ENCODER_CLK);

  Serial.println("Ready. Rotate encoder to browse images, press to select.");
}

void loop() {
  // Read encoder
  int clk = digitalRead(ENCODER_CLK);
  if (clk != lastClk && clk == LOW) {
    int dt = digitalRead(ENCODER_DT);
    if (dt != clk) {
      encoderPos++;
    } else {
      encoderPos--;
    }
    // Wrap around
    int idx = encoderPos % NUM_IMAGES;
    if (idx < 0) idx += NUM_IMAGES;
    showImage(idx);
  }
  lastClk = clk;

  // Button press
  static bool lastBtn = HIGH;
  bool btn = digitalRead(ENCODER_BTN);
  if (btn == LOW && lastBtn == HIGH) {
    int idx = encoderPos % NUM_IMAGES;
    if (idx < 0) idx += NUM_IMAGES;
    Serial.printf("Button pressed on image %d: %s\n", idx, imageNames[idx]);
  }
  lastBtn = btn;

  delay(1);
}
