#ifndef BLENDWAVE_H
#define BLENDWAVE_H
#include <FastLED.h>
#include <Effect.h>

class BlendWave: public Effect {
  protected:
    CRGB blendWaveColor1;
    CRGB blendWaveColor2;
    uint8_t blendWaveSpeed;
    uint8_t blendWaveLocation;
  public:
		BlendWave(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
    virtual ~BlendWave() = default;
		void render();
};
void blendwave(CRGB*, uint16_t);

#endif
