#include <Arduino.h>
#include <Button2.h>
#include <FastLED.h>

#include "pins.hpp"

bool testRunning = false;
uint8_t hostPins[NUM_LEDS] = {PIN_VBUS_HOST, PIN_DP_HOST, PIN_DN_HOST,
                              PIN_GND_HOST};
uint8_t devicePins[NUM_LEDS] = {PIN_VBUS_DEVICE, PIN_DP_DEVICE, PIN_DN_DEVICE,
                                PIN_GND_HOST};
bool continuities[NUM_LEDS] = {true, true, true, true};
bool shorts[NUM_LEDS] = {false, false, false, false};
CRGB leds[NUM_LEDS];
Button2 testButton(PIN_TEST_SWITCH);

void syncColors();
void performTest(Button2& btn);

void setup() {
  delay(100);
  testButton.setClickHandler(performTest);
  FastLED.addLeds<WS2812B, PIN_LEDS, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.setMaxPowerInMilliWatts(400);
}

void loop() {
  EVERY_N_SECONDS(1) {
    syncColors();
    FastLED.show();
  }

  testButton.loop();
}

void syncColors() {
  if (testRunning) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Yellow;
    }
    return;
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    if (shorts[i]) {
      leds[i] = CRGB::Red;
    } else if (!continuities[i]) {
      leds[i] = CRGB::Orange;
    } else {
      leds[i] = CRGB::Green;
    }
  }
}

void performTest(Button2& btn) {
  if (testRunning) {
    return;
  }

  testRunning = true;
  uint8_t testIndex = 0;
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    digitalWrite(hostPins[i], testIndex == i ? HIGH : LOW);
    continuities[i] = true;
    shorts[i] = false;
  }
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t level = digitalRead(devicePins[i]);

    if (testIndex == i && level == LOW) {
      continuities[i] = false;
    } else if (testIndex != i && level == HIGH) {
      shorts[i] = true;
    }
  }
  testRunning = false;
}