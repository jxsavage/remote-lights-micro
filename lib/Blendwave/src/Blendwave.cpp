#include <FastLED.h>

void blendwave(CRGB* leds, uint16_t numLEDs) {

  uint8_t blendwaveSpeed = beatsin8(6,0,255);

  CRGB blendwaveClr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), blendwaveSpeed);
  CRGB blendwaveClr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), blendwaveSpeed);

  uint8_t blendwaveLoc1 = beatsin8(10,0,numLEDs-1);
  
  fill_gradient_RGB(leds, 0, blendwaveClr2, blendwaveLoc1, blendwaveClr1);
  fill_gradient_RGB(leds, blendwaveLoc1, blendwaveClr2, numLEDs-1, blendwaveClr1);

}