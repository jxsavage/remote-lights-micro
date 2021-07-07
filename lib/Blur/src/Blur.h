#ifndef BLUR_H
#define BLUR_H
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
class Blur: public Effect {
  public:
		Blur(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
    virtual ~Blur() = default;
		void render();
};

#endif