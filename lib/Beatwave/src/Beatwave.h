#ifndef BEATWAVE_H
#define BEATWAVE_H
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
class BeatWave: public Effect {
  protected:
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    TBlendType currentBlending;
    CEveryNSeconds newPatternTime;
		CEveryNMillis newBlendTime;
  public:
		BeatWave(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
    virtual ~BeatWave() = default;
		void render();
};

#endif