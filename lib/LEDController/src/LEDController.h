#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
#include <vector>
#include <unordered_map>
#include <FastLED.h>
#include <LEDSegment.h>
#include <EEPROMSettings.h>
#include <enums.h>
#include <logger.h>


typedef std::vector<segmentId> SegmentIds;
typedef std::unordered_map<segmentId, LEDSegment> SegmentsMap;

class LEDController {
  CRGB* allLEDs;
  uint16_t totalLEDs;
  EEPROMSettings* settings;
  SegmentIds segmentIds;
  SegmentsMap segments;
 public:
    LEDController() = default;
    LEDController(EEPROMSettings* settings, CRGB* leds);

    void renderEffects();
    uint16_t getTotalLEDs();
    SegmentsMap* getSegments();
    SegmentIds* getSegmentIds();
    void setId(uint32_t);
    void setSegmentId(segmentId oldId, segmentId newId);
    void mergeSegments(Direction, segmentId);
    void setSegmentEffect(EffectType, segmentId);
    void resizeSegmentsFromBoundaries(JsonArray boundaries);
    void addSegment(segmentNumLEDs, EffectType, segmentOffset, segmentId);
    void splitSegment(EffectType newEffect, Direction, segmentId, segmentId newId);

 private:
    segmentOffset getSegmentOffset(segmentId);
    segmentNumLEDs getSegmentNumLEDs(segmentId);
    EffectType getSegmentEffectType(segmentId);
    segmentId getAdjacentSegId(Direction, segmentId);
    bool mapHasSegment(segmentId);
    void removeSegmentId(segmentId);
    void removeSegmentFromMap(segmentId);
    void removeSegment(segmentId);
};
#endif