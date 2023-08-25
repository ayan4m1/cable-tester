#include <Arduino.h>
#include <Button2.h>
#include <FastLED.h>

#include "pins.hpp"

#define MAX_POWER_MW 400
#define MAX_BRIGHTNESS 127
#define UPDATE_INTERVAL_MS 250

// uncomment to enable serial debugging
// #define SERIAL_DEBUG

uint8_t hostPins[NUM_LEDS] = {PIN_VBUS_HOST, PIN_DP_HOST, PIN_DN_HOST,
                              PIN_GND_HOST};
uint8_t devicePins[NUM_LEDS] = {PIN_VBUS_DEVICE, PIN_DP_DEVICE, PIN_DN_DEVICE,
                                PIN_GND_HOST};
bool testStarted = false;
bool testComplete = false;
bool continuities[NUM_LEDS] = {true, true, true, true};
bool shorts[NUM_LEDS] = {false, false, false, false};
CRGB leds[NUM_LEDS];
Button2 testButton(PIN_TEST_SWITCH);

void syncColors();
void performTest(Button2& btn);

void setup() {
  delay(100);
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println(F("Connected!"));
#endif
  testButton.setClickHandler(performTest);
  FastLED.addLeds<WS2812B, PIN_LEDS, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.setMaxPowerInMilliWatts(MAX_POWER_MW);
#ifdef SERIAL_DEBUG
  Serial.println(F("Initialized!"));
#endif
}

void loop() {
  EVERY_N_MILLISECONDS(UPDATE_INTERVAL_MS) { syncColors(); }

  FastLED.show();
  testButton.loop();
}

void syncColors() {
  if (!testStarted && !testComplete) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
#ifdef SERIAL_DEBUG
    Serial.println(F("Waiting for test..."));
#endif
    return;
  } else if (!testComplete) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Yellow;
    }
#ifdef SERIAL_DEBUG
    Serial.println(F("Testing..."));
#endif
    return;
  }

#ifdef SERIAL_DEBUG
  Serial.println(F("Displaying test results..."));
#endif
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
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
  if (testStarted && !testComplete) {
    return;
  }

  // reset state variables
  testStarted = true;
  testComplete = false;
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    continuities[i] = true;
    shorts[i] = false;
  }
#ifdef SERIAL_DEBUG
  Serial.println(F("Starting test..."));
#endif

  // run the test for each pin
  for (uint8_t testIndex = 0; testIndex < NUM_LEDS; testIndex++) {
    // set all pin states to expected values
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
#ifdef SERIAL_DEBUG
      Serial.printf("Setting pin %d to %d\n", hostPins[i],
                    testIndex == i ? HIGH : LOW);
#endif
      digitalWrite(hostPins[i], testIndex == i ? HIGH : LOW);
    }
    // read pin states and flag any faults
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      uint8_t level = digitalRead(devicePins[i]);
#ifdef SERIAL_DEBUG
      Serial.printf("Read pin %d as %d\n", devicePins[i], level);
#endif

      if (testIndex == i && level == LOW) {
#ifdef SERIAL_DEBUG
        Serial.println(F("Discontinuity detected"));
#endif
        continuities[i] = false;
      } else if (testIndex != i && level == HIGH) {
#ifdef SERIAL_DEBUG
        Serial.println(F("Short detected"));
#endif
        shorts[i] = true;
      }
    }
  }
  testComplete = true;
#ifdef SERIAL_DEBUG
  Serial.println(F("Test complete!"));
#endif
}