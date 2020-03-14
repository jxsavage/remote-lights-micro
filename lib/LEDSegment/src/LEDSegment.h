#ifndef LED_SEGMENT_H
#define LED_SEGMENT_H
#include <FastLED.h>
#include <Effect.h>
#include <EffectFactory.h>
class LEDSegment {
  CRGB* LEDs;
  uint16_t offset;
  uint16_t numLEDs;
  boolean reversed;
  Effect* currentEffect;
  public:
    uint16_t segmentLength;
    LEDSegment(CRGB* LEDs, uint16_t totalLEDs, EffectType effect, uint16_t offset) {
      this->LEDs = LEDs;
      this->numLEDs = totalLEDs;
      this->offset = offset;
      this->reversed = false;
      setEffect(effect);
    }
    ~LEDSegment() {
      delete currentEffect;
    }
    CRGB* getLEDs() {
      return LEDs;
    }
    void setOffset(uint16_t offset) {
      this->offset = offset;
    }
    boolean isReversed() {
      return reversed ? false : true;
    }
    void reverseSegment() {
      for(int i = offset; i < offset + numLEDs/2; ++i) {
        CRGB temp = LEDs[i];
        LEDs[i] = LEDs[numLEDs - 1 - i];
        LEDs[numLEDs - 1 - i] = temp;
      }
    }
    void setEffect(EffectType effect) {
      if(currentEffect) {
        delete currentEffect;
      }
      currentEffect = 
        createEffect(effect, LEDs, numLEDs, offset);
    }
    void renderEffect() {
      currentEffect->render();
    }
};
#endif