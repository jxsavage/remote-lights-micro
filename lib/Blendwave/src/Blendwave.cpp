#include <FastLED.h>
#include <Effect.h>
#include "Blendwave.h"

BlendWave::BlendWave(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0)
  : Effect(LEDs, numLEDs, BLENDWAVE, offset) {
    blendWaveSpeed = beatsin8(6,0,255);
    blendWaveColor1 = CRGB(blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), blendWaveSpeed));
    blendWaveColor2 = CRGB(blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), blendWaveSpeed));
    blendWaveLocation = beatsin8(6,0,255);
  }
void BlendWave::render() {

  uint8_t blendWaveSpeed = beatsin8(6,0,255);

  CRGB blendWaveColor1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), blendWaveSpeed);
  CRGB blendWaveColor2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), blendWaveSpeed);

  uint8_t blendWaveLocation = beatsin8(10,0,numLEDs-1);
  // CRGB LED = &LEDs[offset];
  // CRGB* _LED;
  // _LED = LED;
  fill_gradient_RGB(&LEDs[offset], 0, blendWaveColor2, blendWaveLocation, blendWaveColor1);
  fill_gradient_RGB(&LEDs[offset], blendWaveLocation, blendWaveColor2, numLEDs-1, blendWaveColor1);

}