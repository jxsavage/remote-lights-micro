#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
#include <FastLED.h>
#include <LEDSegment.h>
#include <vector>
#include <EffectFactory.h>
#include <EEPROMSettings.h>
#include <logger.h>
using namespace std;

class LEDController {
  CRGB* allLEDs;
  uint16_t totalLEDs;
  vector<LEDSegment> segments;
  public:
    LEDController() = default;
    LEDController(EEPROMSettings* settings, CRGB* leds) {
      this->allLEDs = leds;
      this->totalLEDs = settings->getTotalLEDs();
      FastLED.setBrightness(settings->getDefaultBrightness());
      for(int i = 0; i < settings->getNumSegments(); i++) {
        const RemoteLightsSegmentSettings segment = settings->getSegment(i);
        uint16_t offset = segment.offset;
        uint16_t numLEDs = segment.numLEDs;
        EffectType effect = segment.effect;
        addSegment(numLEDs, effect, offset);
      };
    }
    void addSegment(uint16_t numLEDs, EffectType effectType, uint16_t offset, uint32_t id = 0) {
      uint32_t segmentId;
      segmentId = (id == 0) ? random(1, 2147483647) : id;
      segments.push_back(LEDSegment(allLEDs, numLEDs, effectType, offset, segmentId));
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