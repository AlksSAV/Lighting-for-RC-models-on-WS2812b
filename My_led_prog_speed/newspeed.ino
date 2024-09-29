#include <FastLED.h>

// Константы
const int NUM_LEDS = 60;
const int DATA_PIN = 5;
const int pwmIn = 9;
const int valMin = 980;
const int valMax = 2000;
const int dlay = 500;
const int pulse1_pin1 = 3;

// Переменные
int ch1 = 0;
int prog = 1;
int last_prog = prog;
int steps = 0;
long nowMillis = 0;
long toRead = 0;
boolean next_step = false;
boolean lamp_on = false;

CRGB leds[NUM_LEDS];

// Функции
void setup() {
  pinMode(pulse1_pin1, INPUT);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  leds[0] = CRGB::White;
  FastLED.show();
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void updateProg() {
  if (millis() - toRead > dlay) {
    ch1 = pulseIn(pulse1_pin1, HIGH, 25000);

    if (ch1 < 1100) {
      if (prog != 1) {
        prog = 1;
        next_step = true;
      }
    } else if (ch1 < 1400) {
      if (prog != 2) {
        prog = 2;
        steps = 0;
      }
    } else if (ch1 < 1900) {
      if (prog != 3) {
        prog = 3;
        steps = 0;
      }
    }
    toRead = millis();
  }
}

void runProg1() {
  // Код для прог 1
}

void runProg2() {
  int val = pulseIn(pwmIn, HIGH);
  int mappedHue = map(val, valMin, valMax, 80, 255);
  int numLedsToLight = map(val, valMin, valMax, 0, NUM_LEDS);

  val = constrain(val, 0, 255);

  FastLED.clear();
  for (int led = 0; led < numLedsToLight; led++) {
    leds[led] = CHSV(mappedHue, 255, 255);
  }
  FastLED.show();
}

void runProg3() {
  static uint8_t hue = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(15);
  }

  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(15);
  }
}

void loop() {
  nowMillis = millis();
  updateProg();

  switch (prog) {
    case 1:
      runProg1();
      break;
    case 2:
      runProg2();
      break;
    case 3:
      runProg3();
      break;
  }
}

