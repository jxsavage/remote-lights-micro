#include <FastLED.h>
#include <Effect.h>
#include "Beatwave.h"

BeatWave::BeatWave(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0)
  : Effect(LEDs, numLEDs, BLENDWAVE, offset) {
    currentBlending = LINEARBLEND;
    newBlendTime = CEveryNMillis(100);
    newPatternTime = CEveryNSeconds(5);
  }
void BeatWave::render() {

  uint8_t wave1 = beatsin8(9, 0, 255);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i = 0; i < numLEDs; i++) {
    LEDs[offset+i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending);
  }
  if (newBlendTime) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }
  if (newPatternTime) {
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
}