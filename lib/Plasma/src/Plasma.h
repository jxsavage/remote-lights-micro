#ifndef PLASMA_H
#define PLASMA_H
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
class Plasma: public Effect {
  protected:
    CRGBPalette16 currentPalette;                                 // Palette definitions
    CRGBPalette16 targetPalette;
    TBlendType currentBlending;
    CEveryNMillis renderTimer;
    CEveryNMillis palletteBlendTimer;
    CEveryNSeconds newPalleteTimer;

  public:
		Plasma(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
    virtual ~Plasma() = default;
		void render();
};

#endif