#include <FastLED.h>
#include <Effect.h>
#include "Blur.h"

Blur::Blur(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0)
  : Effect(LEDs, numLEDs, BLUR, offset) {
  }
void Blur::render() {
  // A sinewave at 3 Hz with values ranging from 64 to 192.
  uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );
  // Apply some blurring to whatever's already on the strip, which will eventually go black.
  blur1d( &LEDs[offset], numLEDs, blurAmount);
  
  uint8_t  i = beatsin8(  9, 0, numLEDs);
  uint8_t  j = beatsin8( 7, 0, numLEDs);
  uint8_t  k = beatsin8(  5, 0, numLEDs);
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  LEDs[(offset+i+j)/2] = CHSV( ms / 29, 200, 255);
  LEDs[(offset+j+k)/2] = CHSV( ms / 41, 200, 255);
  LEDs[(offset+k+i)/2] = CHSV( ms / 73, 200, 255);
  LEDs[(offset+k+i+j)/3] = CHSV( ms / 53, 200, 255);
}