#include <FastLED.h>
#include <Effect.h>
#include "Confetti.h"

Confetti::Confetti(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0)
  : Effect(LEDs, numLEDs, BLENDWAVE, offset) {
    hue = 50;
    hueRange = 256;
    hueIncrement = 1;
    fadeTime = 8;
    saturation = 100;
    brightness = 255;
    animationDelay = 5;
    animationTimer = CEveryNMillis(animationDelay);
  }
void Confetti::render() {
  // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  uint8_t secondHand = (millis() / 1000) % 15;
  // Static variable, means it's only defined once. This is our 'debounce' variable.
  static uint8_t lastSecond = 99;
  // Debounce to make sure we're not repeating an assignment.
  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    switch(secondHand) {
      // You can change values here, one at a time , or altogether.
      case  0: hueIncrement=1; hue=192; saturation=255; fadeTime=2; hueRange=256; break;
      case  5: hueIncrement=2; hue=128; fadeTime=8; hueRange=64; break;
      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 10: hueIncrement=1; hue=random16(255); fadeTime=1; hueRange=16; break;
      // Here's the matching 15 for the other one.
      case 15: break;
    }
  }
  // Low values = slower fade.
  fadeToBlackBy(&LEDs[offset], numLEDs, fadeTime);
  // Pick an LED at random.
  int pos = random16(numLEDs);
  // I use 12 bits for hue so that the hue increment isn't too quick.
  LEDs[offset+pos] += CHSV((hue + random16(hueRange))/4 , saturation, brightness);
  hue = hue + hueIncrement;
}
