#ifndef LED_SEGMENT_H
#define LED_SEGMENT_H
#include <logger.h>
#include <FastLED.h>
#include <Effect.h>
#include <EffectFactory.h>
#include <memory>
class LEDSegment {
  CRGB* LEDs;
  uint16_t offset;
  uint16_t numLEDs;
  boolean reversed;
  std::unique_ptr<Effect> currentEffect;
  public:
    uint16_t segmentLength;
    LEDSegment(CRGB* LEDs, uint16_t totalLEDs, EffectType effect, uint16_t offset) {
      this->LEDs = LEDs;
      this->numLEDs = totalLEDs;
      this->offset = offset;
      this->reversed = false;
      setEffect(effect);
    }
    CRGB* getLEDs() {
      return LEDs;
    }
    void setOffset(uint16_t offset) {
      this->offset = offset;
    }
    uint16_t getOffset() {
      return offset;
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
      if(effect == ET_ColorWaves){
				currentEffect = std::unique_ptr<Effect>(new ColorWaves(LEDs, numLEDs, offset));
			} else if (effect == ET_BlendWave) {
        currentEffect = std::unique_ptr<Effect>(new BlendWave(LEDs, numLEDs, offset));
      }
    }
    void renderEffect() {
      currentEffect->render();
    }
    uint16_t getNumLEDs() {
      return numLEDs;
    }
    EffectType getEffect() {
      return currentEffect->getEffect();
    }
};
#endif