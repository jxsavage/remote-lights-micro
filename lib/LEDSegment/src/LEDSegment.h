#ifndef LED_SEGMENT_H
#define LED_SEGMENT_H
#include <logger.h>
#include <FastLED.h>
#include <Effect.h>
#include <Colorwaves.h>
#include <Blendwave.h>
#include <memory>
#include <enums.h>

typedef uint32_t segmentId;
typedef int16_t segmentOffset;
typedef int16_t segmentNumLEDs;
typedef std::unique_ptr<Effect> LEDSegmentEffect;

class LEDSegment {
  CRGB* LEDs;
  segmentOffset offset;
  segmentNumLEDs numLEDs;
  boolean reversed;
  segmentId id;
  LEDSegmentEffect currentEffect;
  public:
    uint16_t segmentLength;
    LEDSegment(CRGB* LEDs, segmentNumLEDs totalLEDs, EffectType effect, segmentOffset offset, segmentId id) {
      this->LEDs = LEDs;
      this->offset = offset;
      this->reversed = false;
      this->numLEDs = totalLEDs;
      this->id = id;
      setEffect(effect);
    }
    LEDSegment(){};
    segmentId getId() {
      return id;
    }
    CRGB* getLEDs() {
      return LEDs;
    }
    void setOffset(segmentOffset offset) {
      this->offset = offset;
      currentEffect->setOffset(offset);
    }
    segmentOffset getOffset() {
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
      if(effect == COLORWAVES){
				currentEffect = LEDSegmentEffect(new ColorWaves(LEDs, numLEDs, offset));
			} else if (effect == BLENDWAVE) {
        currentEffect = LEDSegmentEffect(new BlendWave(LEDs, numLEDs, offset));
      }
    }
    void renderEffect() {
      currentEffect->render();
    }
    segmentNumLEDs getNumLEDs() {
      return numLEDs;
    }
    void setNumLEDs(segmentNumLEDs numLEDs) {
      this->numLEDs = numLEDs;
      currentEffect->setNumLEDs(numLEDs);
    }
    EffectType getEffect() {
      return currentEffect->getEffect();
    }
    void setId(segmentId segId) {
      id = segId;
    }
};
#endif