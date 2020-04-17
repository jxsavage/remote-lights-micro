#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
using namespace std;
#include <vector>
#include <FastLED.h>
#include <LEDSegment.h>
#include <EEPROMSettings.h>
#include <logger.h>

enum Direction: uint8_t {
  LEFT,
  RIGHT
};
class LEDController {
  CRGB* allLEDs;
  uint16_t totalLEDs;
  EEPROMSettings* settings;
  vector<LEDSegment> segments;
  public:
    LEDController() = default;
    LEDController(EEPROMSettings* settings, CRGB* leds);

    void renderEffects();
    uint16_t getTotalLEDs();
    vector<LEDSegment>* getSegments();
    void resizeSegmentsFromBoundaries(JsonArray boundaries);
    void mergeSegments(Direction direction, uint8_t segmentIndex);
    void setSegmentEffect(EffectType effectType, uint8_t segmentIndex);
    void splitSegment(EffectType newEffect, Direction direction, uint8_t segmentIndex);
    void addSegment(uint16_t numLEDs, EffectType effectType, uint16_t offset, uint32_t id = 0);
};
#endif