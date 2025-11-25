// GitHub Copilot Chat Assistant
// ESP32-C3 Super Mini adapted version of shitnado.ino
// Adapted again for the WRRF_XMAS display
// - Set SDA_PIN and SCL_PIN to the pins you wired the OLED to.
// - Uses esp_random() for seeding (ESP32-safe)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "wrrf_xmas.h"  // Contains WRRFxmas
#include <esp_system.h>        // for esp_random()

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Set these to the pins you used on your ESP32-C3 Super Mini.
// Example placeholders (change to match your wiring):
// For example, if you wired SDA to GPIO6 and SCL to GPIO7, set them accordingly.
// If you prefer to use the default I2C pins for your board, you can call Wire.begin(); instead.
#define SDA_PIN 3
#define SCL_PIN 4

// Number of snowflakes
const int numFlakes = 15;
int flakeX[numFlakes], flakeY[numFlakes];

// Poop sprite (8x8)
const unsigned char poop_sprite[] PROGMEM = {
  // 'poop, 8x8px
  0x02, 0x0c, 0x3c, 0x5a, 0x5a, 0xfe, 0x47, 0xff
};
static const unsigned char PROGMEM dlogo[] = {
 // 'd1b, 30x24px
  0x00, 0x0f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0xef, 0xe0, 0x00, 
  0x00, 0xfc, 0x70, 0x00, 0x01, 0xfd, 0xf8, 0x00, 0x01, 0x96, 0x7c, 0x00, 0x02, 0x7b, 0xfc, 0x00, 
  0x07, 0xef, 0xde, 0x00, 0x07, 0xdf, 0xde, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xeb, 0x00, 
  0x1f, 0xff, 0xfd, 0x80, 0x3f, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xfd, 0xc0, 0x7f, 0xff, 0xfb, 0xe0, 
  0x7f, 0xff, 0xfb, 0xe0, 0x7f, 0xfd, 0x77, 0xe0, 0x7f, 0x7c, 0x2f, 0xf0, 0xfe, 0xf8, 0x2f, 0xf0, 
  0xfe, 0xc0, 0x6f, 0xf0, 0xf8, 0x00, 0x7f, 0xf8, 0x70, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x0b, 0x40
};

// Reusing Shitnado's Currency symbol (ASCII) code but with an asterisk to emulate snowflakes
const char currency_symbols[] = {'*'};
const int num_symbols = sizeof(currency_symbols);

// -------------------- NEW: Falling logos --------------------
const int numLogos = 1;            // simultaneous logos
int logoX[numLogos], logoY[numLogos];

// -------------------- NEW: Falling currency --------------------
const int numCurrency = 6;        // simultaneous currency glyphs
int curX[numCurrency], curY[numCurrency];
char curSym[numCurrency];

// -------------------- NEW: Falling poops --------------------
const int numPoops = 3;        // simultaneous poop emoji
int poopX[numPoops], poopY[numPoops];


// Frame timing
unsigned long lastFrameTime = 0;
const int frameDelay = 150;  // SPEED IN ms (consider 120–60 for smoother)
int currentFrame = 0;

void setup() {
  // Initialize I2C for the OLED on the pins you selected
  Wire.begin(SDA_PIN, SCL_PIN);
  // If you prefer to use the default/built-in I2C pins for your board, use:
  // Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Failed to initialize display — you can add debugging here if desired
    for (;;) 
      delay(1000);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Seed random from hardware RNG on ESP32
  randomSeed((unsigned long)esp_random());

    // Initialize random positions for snowflakes
    for (int i = 0; i < numFlakes; i++) {
      flakeX[i] = random(0, SCREEN_WIDTH);
      flakeY[i] = random(0, SCREEN_HEIGHT);
    }

  // -------------------- NEW: Init logos along the top --------------------
  for (int i = 0; i < numLogos; i++) {
    logoX[i] = random(50, SCREEN_WIDTH - 30); // 30px bitmap width
    logoY[i] = random(-24, SCREEN_HEIGHT - 25);               // start above/at top for a clean entry
  }

  // -------------------- NEW: Init currency along the top --------------------
  for (int i = 0; i < numCurrency; i++) {
    curX[i]   = random(0, SCREEN_WIDTH - 6); // approx glyph width
    curY[i]   = random(-1, SCREEN_HEIGHT - 10);               // start above/at top
    curSym[i] = currency_symbols[random(0, num_symbols)];
  }

  // -------------------- NEW: Init logos along the top --------------------
  for (int i = 0; i < numPoops; i++) {
    poopX[i] = random(0, SCREEN_WIDTH - 8); // 8px bitmap width
    poopY[i] = random(-8, SCREEN_HEIGHT - 8);               // start above/at top for a clean entry
  }
}

void loop() {
  unsigned long now = millis();
  if (now - lastFrameTime >= frameDelay) {
    lastFrameTime = now;

    display.clearDisplay();

    // Draw WRRF xmas frame (from wrrf_xmas.h)
    display.drawBitmap(0, 0, WRRFxmas, 128, 64, SSD1306_WHITE);

    // Draw snowflakes
    for (int i = 0; i < numFlakes; i++) {
      display.drawPixel(flakeX[i], flakeY[i], SSD1306_WHITE);
      flakeY[i] += 1;        // Move flake down
      flakeX[i] += random(-1, 2); // slight drift (-1..2)

      // Reset snowflake if it moves off the screen
      if (flakeY[i] > SCREEN_HEIGHT || flakeX[i] > SCREEN_WIDTH) {
        flakeY[i] = 0;
        flakeX[i] = random(0, SCREEN_WIDTH);
      }
    }

    // -------------------- NEW: Falling township logos --------------------
    for (int i = 0; i < numLogos; i++) {
      display.drawBitmap(logoX[i], logoY[i], dlogo, 30, 24, SSD1306_WHITE);
      logoY[i] += 1;                          // fall speed (1 px/frame)
      logoX[i] += random(-1, 2);              // gentle side drift (-1..1)

      // Reset when off bottom or far right; respawn at top
      if (logoY[i] > SCREEN_HEIGHT+30 || logoX[i] > SCREEN_WIDTH) {
        logoY[i] = -24;                       // start just above the screen
        logoX[i] = random(0, SCREEN_WIDTH - 30);
      }
    }

    // -------------------- NEW: Falling currency symbols --------------------
    for (int i = 0; i < numCurrency; i++) {
      display.setCursor(curX[i], curY[i]);
      display.write(curSym[i]);
      curY[i] += 1;                           // fall speed
      curX[i] += random(-1, 2);               // gentle side drift

      if (curY[i] > SCREEN_HEIGHT || curX[i] > SCREEN_WIDTH) {
        curY[i]   = -8;                       // start just above the screen
        curX[i]   = random(0, SCREEN_WIDTH);
        curSym[i] = currency_symbols[random(0, num_symbols)];
      }
    }

    // -------------------- NEW: Falling poop emoji --------------------
    for (int i = 0; i < numPoops; i++) {
      display.drawBitmap(poopX[i], poopY[i], poop_sprite, 8, 8, SSD1306_WHITE);
      poopY[i] += 1;                          // fall speed (1 px/frame)
      poopX[i] += random(-1, 2);              // gentle side drift (-1..1)

      // Reset when off bottom or far right; respawn at top
      if (poopY[i] > SCREEN_HEIGHT || poopX[i] > SCREEN_WIDTH) {
        poopY[i] = -8;                       // start just above the screen
        poopX[i] = random(0, SCREEN_WIDTH);
      }
    }

    display.display();

    // Advance to next frame
    currentFrame = (currentFrame + 1);
  }
}
