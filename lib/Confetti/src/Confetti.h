#ifndef CONFETTI_H
#define CONFETTI_H
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
class Confetti: public Effect {
  protected:
    int hue;
    int hueRange;
    uint8_t hueIncrement;
    uint8_t fadeTime;
    uint8_t saturation;
    uint8_t brightness;
    uint8_t animationDelay;
    CEveryNMillis animationTimer;

  public:
		Confetti(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
    virtual ~Confetti() = default;
		void render();
};
#endif