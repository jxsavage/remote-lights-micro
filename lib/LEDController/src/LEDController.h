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
  CLEDController* FastLEDControllers;
  vector<LEDSegment> segments;
  public:
    LEDController(EEPROMSettings* settings) {
      const Settings microSettings = settings->getSettings();
      CRGB leds[microSettings.totalLEDs];
      this->allLEDs = leds;
      this->totalLEDs = settings->getSettings().totalLEDs;
      const FastLEDStripSettings* stripSettings = microSettings.strips;
      for(int i = 0; i <= 1; i++) {
        const uint16_t offset = stripSettings[i].offset;
        const uint16_t numLEDs = stripSettings[i].numLEDs;
        const EffectType effect = stripSettings[i].effect;
        REMOTE_LOG(99, "iteration", i, "offset", offset, "numLEDs", numLEDs, "effect", (int)effect);
        switch(i) {
          case 0:
            FastLED.addLeds<APA102, 7, 14, GRB, DATA_RATE_MHZ(2)>(leds, numLEDs).setCorrection(TypicalSMD5050);
            break;
          case 1:
            FastLED.addLeds<APA102, 21, 20, GRB, DATA_RATE_MHZ(2)>(leds, offset, numLEDs).setCorrection(TypicalSMD5050);
            break;
        }
        addSegment(numLEDs, effect, offset);
        FastLED.setBrightness(microSettings.defaultBrightness);
      };
      randomSeed(analogRead(0));
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