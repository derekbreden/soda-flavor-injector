#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Adafruit_NeoPixel.h>
#include <lvgl.h>
#include "CST816D.h"
#include "font_ratio_64.h"

// ════════════════════════════════════════════════════════════
//  ESP32-S3 Config Display — Soda Flavor Injector
// ════════════════════════════════════════════════════════════
// Pin assignments for Meshnology ESP32-S3 1.28" Round Rotary Display

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

// ── Touch (CST816D) ──
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

// ── Hardware objects ──
Arduino_ESP32SPI *spi_bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED, FSPI, true);
Arduino_GC9A01 *hw_display = new Arduino_GC9A01(spi_bus, TFT_RST, 0, true);
Adafruit_NeoPixel leds(LED_COUNT, LED_DATA, NEO_GRB + NEO_KHZ800);
CST816D touch(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

// ── LVGL display buffer ──
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *lvgl_buf;

// ── Config state (synced from ESP32 via UART) ──
uint8_t flavor1Image = 0;
uint8_t flavor2Image = 1;
uint8_t flavor1Ratio = 20;
uint8_t flavor2Ratio = 20;

// ── UART to ESP32 (Serial0 = UART0, J34 connector) ──
#define UART_BUF_SIZE 128
char uartBuf[UART_BUF_SIZE];
int uartBufIdx = 0;
bool configSynced = false;
unsigned long lastGetConfig = 0;

// ── Menu ──
enum MenuItem { MENU_F1_IMAGE, MENU_F1_RATIO, MENU_F2_IMAGE, MENU_F2_RATIO, MENU_COUNT };
const char* menuLabels[] = { "Flavor 1 Image", "Flavor 1 Ratio", "Flavor 2 Image", "Flavor 2 Ratio" };

int menuIndex = 0;
bool editing = false;

// ── Encoder state ──
int lastClk = HIGH;

// ── Touch state ──
unsigned long lastTapTime = 0;
bool lastTouchState = false;

// ── Circular image rendering ──
// Browse: 90px diameter, Edit: 128px diameter (matches external RP2040 display)
#define THUMB_BROWSE 90
#define THUMB_EDIT   128
#define THUMB_MAX    128
static lv_color_t thumb_buf[THUMB_MAX * THUMB_MAX];

void renderCircularThumb(uint8_t imgIdx, int size) {
  const uint16_t *src = images[imgIdx];
  int radius = size / 2;
  int r2 = radius * radius;
  for (int y = 0; y < size; y++) {
    int dy = y - radius;
    int srcY = y * 240 / size;
    for (int x = 0; x < size; x++) {
      int dx = x - radius;
      if (dx * dx + dy * dy <= r2) {
        int srcX = x * 240 / size;
        thumb_buf[y * size + x].full = src[srcY * 240 + srcX];
      } else {
        thumb_buf[y * size + x].full = 0;
      }
    }
  }
}

// ── LVGL flush callback ──
void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;
  hw_display->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);
}

// ── Helpers ──
uint8_t getCurrentImage() {
  return (menuIndex == MENU_F1_IMAGE) ? flavor1Image : flavor2Image;
}

uint8_t getCurrentRatio() {
  return (menuIndex == MENU_F1_RATIO) ? flavor1Ratio : flavor2Ratio;
}

bool isImageItem() {
  return (menuIndex == MENU_F1_IMAGE || menuIndex == MENU_F2_IMAGE);
}

// Forward declaration (needed by UART handler)
void drawScreen();

// ── UART communication ──

void parseConfigResponse(const char* line) {
  // Parse: CONFIG:F1_RATIO=20,F2_RATIO=20,F1_IMAGE=0,F2_IMAGE=1,NUM_IMAGES=3
  if (strncmp(line, "CONFIG:", 7) != 0) return;

  const char* p = line + 7;
  int f1r = 0, f2r = 0, f1i = 0, f2i = 0;

  if (sscanf(p, "F1_RATIO=%d,F2_RATIO=%d,F1_IMAGE=%d,F2_IMAGE=%d",
             &f1r, &f2r, &f1i, &f2i) == 4) {
    flavor1Ratio = constrain(f1r, 6, 24);
    flavor2Ratio = constrain(f2r, 6, 24);
    flavor1Image = constrain(f1i, 0, NUM_IMAGES - 1);
    flavor2Image = constrain(f2i, 0, NUM_IMAGES - 1);
    configSynced = true;
    Serial.printf("Config synced: F1_RATIO=%d F2_RATIO=%d F1_IMAGE=%d F2_IMAGE=%d\n",
                  flavor1Ratio, flavor2Ratio, flavor1Image, flavor2Image);
    drawScreen();
  }
}

void processUartLine(const char* line) {
  Serial.printf("UART RX: %s\n", line);

  if (strncmp(line, "CONFIG:", 7) == 0) {
    parseConfigResponse(line);
  } else if (strncmp(line, "OK:", 3) == 0) {
    // Acknowledgment — no action needed, value already set locally
    Serial.printf("ESP32 confirmed: %s\n", line);
  } else if (strncmp(line, "ERR:", 4) == 0) {
    // Error — log it. Could add UI feedback later.
    Serial.printf("ESP32 error: %s\n", line);
  }
}

void checkUart() {
  while (Serial0.available()) {
    char c = Serial0.read();
    if (c == '\n' || c == '\r') {
      if (uartBufIdx > 0) {
        uartBuf[uartBufIdx] = '\0';
        processUartLine(uartBuf);
        uartBufIdx = 0;
      }
    } else if (uartBufIdx < UART_BUF_SIZE - 1) {
      uartBuf[uartBufIdx++] = c;
    }
  }
}

void sendSetCommand(const char* key, int value) {
  Serial0.printf("SET:%s=%d\n", key, value);
  Serial.printf("UART TX: SET:%s=%d\n", key, value);
}

void sendSave() {
  Serial0.println("SAVE");
  Serial.println("UART TX: SAVE");
}

void sendCurrentValue() {
  switch (menuIndex) {
    case MENU_F1_IMAGE:
      sendSetCommand("F1_IMAGE", flavor1Image);
      break;
    case MENU_F1_RATIO:
      sendSetCommand("F1_RATIO", flavor1Ratio);
      break;
    case MENU_F2_IMAGE:
      sendSetCommand("F2_IMAGE", flavor2Image);
      break;
    case MENU_F2_RATIO:
      sendSetCommand("F2_RATIO", flavor2Ratio);
      break;
  }
  sendSave();
}

// ── UI drawing ──

void drawNavDots() {
  int dotSpacing = 16;
  int totalWidth = (MENU_COUNT - 1) * dotSpacing;
  int startX = (240 - totalWidth) / 2;
  for (int i = 0; i < MENU_COUNT; i++) {
    lv_obj_t *dot = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, 6, 6);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(dot,
      (i == menuIndex) ? lv_color_white() : lv_color_hex(0x303030), 0);
    lv_obj_set_pos(dot, startX + i * dotSpacing - 3, 207);
  }
}

void drawBrowse() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

  // Title
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0x808080), 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 38);

  if (isImageItem()) {
    renderCircularThumb(getCurrentImage(), THUMB_BROWSE);
    lv_obj_t *canvas = lv_canvas_create(scr);
    lv_canvas_set_buffer(canvas, thumb_buf, THUMB_BROWSE, THUMB_BROWSE, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 10);
  } else {
    char buf[8];
    snprintf(buf, sizeof(buf), "1:%d", getCurrentRatio());
    lv_obj_t *ratio = lv_label_create(scr);
    lv_label_set_text(ratio, buf);
    lv_obj_set_style_text_font(ratio, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(ratio, lv_color_hex(0x808080), 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }

  drawNavDots();
}

void drawEdit() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

  // Title — always white when editing
  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, menuLabels[menuIndex]);
  lv_obj_set_style_text_color(title, lv_color_white(), 0);

  if (isImageItem()) {
    renderCircularThumb(getCurrentImage(), THUMB_EDIT);
    lv_obj_t *canvas = lv_canvas_create(scr);
    lv_canvas_set_buffer(canvas, thumb_buf, THUMB_EDIT, THUMB_EDIT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 8);

    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 28);
  } else {
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);

    char buf[8];
    snprintf(buf, sizeof(buf), "1:%d", getCurrentRatio());
    lv_obj_t *ratio = lv_label_create(scr);
    lv_label_set_text(ratio, buf);
    lv_obj_set_style_text_font(ratio, &font_ratio_64, 0);
    lv_obj_set_style_text_color(ratio, lv_color_white(), 0);
    lv_obj_align(ratio, LV_ALIGN_CENTER, 0, 10);
  }
}

void drawScreen() {
  if (editing) {
    drawEdit();
  } else {
    drawBrowse();
  }
  lv_refr_now(NULL);
}

// ── Input reading ──

int readEncoder() {
  int clk = digitalRead(ENCODER_CLK);
  int direction = 0;
  if (clk != lastClk && clk == LOW) {
    int dt = digitalRead(ENCODER_DT);
    direction = (dt != clk) ? 1 : -1;
  }
  lastClk = clk;
  return direction;
}

bool readTap() {
  uint16_t x, y;
  uint8_t gesture;
  bool touching = touch.getTouch(&x, &y, &gesture);
  bool tapped = (touching && !lastTouchState && millis() - lastTapTime > 300);
  if (tapped) lastTapTime = millis();
  lastTouchState = touching;
  return tapped;
}

// ── Menu logic ──

void handleNavigation(int dir) {
  if (dir == 0) return;

  if (editing) {
    switch (menuIndex) {
      case MENU_F1_IMAGE:
        flavor1Image = (flavor1Image + dir + NUM_IMAGES) % NUM_IMAGES;
        break;
      case MENU_F1_RATIO:
        flavor1Ratio = constrain(flavor1Ratio + dir, 6, 24);
        break;
      case MENU_F2_IMAGE:
        flavor2Image = (flavor2Image + dir + NUM_IMAGES) % NUM_IMAGES;
        break;
      case MENU_F2_RATIO:
        flavor2Ratio = constrain(flavor2Ratio + dir, 6, 24);
        break;
    }
    drawScreen();
  } else {
    menuIndex = (menuIndex + dir + MENU_COUNT) % MENU_COUNT;
    drawScreen();
  }
}

void handleTap() {
  if (editing) {
    editing = false;
    Serial.printf("Confirmed: %s\n", menuLabels[menuIndex]);
    sendCurrentValue();
  } else {
    editing = true;
    Serial.printf("Editing: %s\n", menuLabels[menuIndex]);
  }
  drawScreen();
}

// ── Arduino setup/loop ──

void setup() {
  Serial.begin(115200);
  Serial0.begin(9600, SERIAL_8N1, 44, 43);  // UART0 on J34 connector (RX=44, TX=43)
  delay(500);
  Serial.println("ESP32-S3 Config Display starting...");

  // RGB LEDs (unused, turned off)
  leds.begin();
  leds.clear();
  leds.show();

  // Power pins (required by board hardware)
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Backlight
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);

  // Encoder
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);

  // Touch
  touch.begin();

  // Display hardware init
  hw_display->begin();

  // LVGL init
  lv_init();

  // Allocate full-screen draw buffer (115KB — fits in ESP32-S3 SRAM)
  lvgl_buf = (lv_color_t *)malloc(240 * 240 * sizeof(lv_color_t));
  lv_disp_draw_buf_init(&draw_buf, lvgl_buf, NULL, 240 * 240);

  // Register display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = lvgl_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  lastClk = digitalRead(ENCODER_CLK);

  drawScreen();
  Serial.println("Ready. Rotate to navigate, tap to edit/confirm.");
}

void loop() {
  // Boot sync: request config from ESP32 every 500ms until synced
  if (!configSynced && millis() - lastGetConfig > 500) {
    Serial0.println("GET_CONFIG");
    Serial.println("UART TX: GET_CONFIG (boot sync)");
    lastGetConfig = millis();
  }

  // Check for incoming UART data
  checkUart();

  int dir = readEncoder();
  handleNavigation(dir);

  if (readTap()) {
    handleTap();
  }

  lv_timer_handler();
  delay(5);
}
