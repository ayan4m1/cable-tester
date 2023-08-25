#pragma once

/**
 * KB2040 Pinout
 *
 * |----------|
 * |D+      D-|
 * |TX      5V|
 * |RX     GND|
 * |GND    RST|
 * |GND    3V3|
 * |D2      A3|
 * |D3      A2|
 * |D4      A1|
 * |D5      A0|
 * |D6      18|
 * |D7      20|
 * |D8      19|
 * |D9      10|
 * |----------|
 */

#define PIN_VBUS_HOST D2
#define PIN_DP_HOST D3
#define PIN_DN_HOST D4
#define PIN_GND_HOST D5

#define PIN_VBUS_DEVICE D6
#define PIN_DP_DEVICE D7
#define PIN_DN_DEVICE D8
#define PIN_GND_DEVICE D9

#define PIN_TEST_SWITCH A3
#define PIN_LEDS A2
#define PIN_STATUS_LED 17
#define NUM_LEDS 4