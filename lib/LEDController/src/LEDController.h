#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
#include <FastLED.h>
#include <LEDSegment.h>
#include <vector>
#include <EffectFactory.h>
using namespace std;
class LEDController {
  CRGB* allLEDs;
  uint16_t totalLEDs;
  vector<LEDSegment> segments;
  public:
    LEDController(CRGB* LEDs, uint16_t numLEDs/*, uint8_t pinSegmentCount, ...*/) {
      this->allLEDs = LEDs;
      this->totalLEDs = numLEDs;
      // va_list pinSegmentOffsets;
      // va_start(pinSegmentOffsets, pinSegmentCount);
      // uint16_t pinSegmentOffset;
      // uint16_t currentOffset = 0;
      // for(int i = 0; i <= pinSegmentCount-1; i++) {
      //   uint16_t totalOffset = numLEDs - va_arg(pinSegmentOffsets, uint16_t) - 1;
      //   //LEDSegment newSegment = 
      //   pinSegments.push_back(LEDSegment(&LEDs[currentOffset], numLEDs, ET_BlendWave, totalOffset));
      // }
      // va_end(pinSegmentOffsets);
    }
    void addSegment(uint16_t numLEDs, EffectType effectType, uint16_t offset) {
      segments.push_back(LEDSegment(allLEDs, numLEDs, effectType, offset));
    }
    void renderEffects() {
      vector<LEDSegment>::iterator seg;
      for (seg = segments.begin(); seg != segments.end(); seg++){
        seg->renderEffect();
      }
    }
    vector<LEDSegment>* getSegments() {
      return &segments;
    }
    uint16_t getTotalLEDs() {
      return totalLEDs;
    }
    void setSegmentEffect(EffectType effectType, uint8_t segmentNum) {
      segments[segmentNum].setEffect(effectType);
    }

};
#endif