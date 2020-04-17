
using namespace std;
#include <vector>
#include <FastLED.h>
#include <LEDSegment.h>
#include <EEPROMSettings.h>
#include <logger.h>
#include "LEDController.h"

LEDController::LEDController(EEPROMSettings* settings, CRGB* leds) {
  this->allLEDs = leds;
  this->settings = settings;
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
void LEDController::mergeSegments(Direction direction, uint8_t segmentIndex) {
  bool isLeftMerge = direction == LEFT;
  uint8_t mergeIndex = isLeftMerge ? segmentIndex - 1 : segmentIndex + 1;
  uint16_t offset = isLeftMerge ? segments[mergeIndex].getOffset() : segments[segmentIndex].getOffset();
  uint16_t numLEDs = segments[mergeIndex].getNumLEDs() + segments[segmentIndex].getNumLEDs();
  segments[segmentIndex].setOffset(offset);
  segments[segmentIndex].setNumLEDs(numLEDs);
  segments.erase(segments.begin() + mergeIndex);
}
void LEDController::addSegment(uint16_t numLEDs, EffectType effectType, uint16_t offset, uint32_t id = 0) {
  uint32_t segmentId;
  segmentId = (id == 0) ? random(1, 2147483647) : id;
  segments.push_back(LEDSegment(allLEDs, numLEDs, effectType, offset, segmentId));
}
void LEDController::renderEffects() {
  vector<LEDSegment>::iterator seg;
  for (seg = segments.begin(); seg != segments.end(); seg++){
    seg->renderEffect();
  }
}
void LEDController::splitSegment(EffectType newEffect, Direction direction, uint8_t segmentIndex) {
  uint16_t toSplitNumLEDs = segments[segmentIndex].getNumLEDs();
  uint16_t toSplitOffset = segments[segmentIndex].getOffset();
  uint16_t leftLen =  toSplitNumLEDs / 2;
  uint16_t rightLen = toSplitNumLEDs - leftLen;
  uint16_t rightOffset = toSplitOffset + leftLen;
  bool splitLeft = direction == LEFT;
  EffectType toSplitEffect = segments[segmentIndex].getEffect();
  EffectType leftEffect = splitLeft ? newEffect : toSplitEffect;
  EffectType rightEffect = splitLeft ? toSplitEffect : newEffect;
  uint32_t newId = random(1, 2147483647);
  uint32_t toSplitId = segments[segmentIndex].getId();
  uint32_t leftId = splitLeft ? newId : toSplitId;
  uint32_t rightId = splitLeft ? toSplitId : newId;
  const uint8_t insertIndex = splitLeft ? segmentIndex : segmentIndex + 1;
  // LEDSegment toInsert = splitLeft ? 
  //   LEDSegment(allLEDs, leftLen, leftEffect, toSplitOffset, leftId) :
  //   LEDSegment(allLEDs, rightLen, rightEffect, rightOffset, rightId);
  segments.insert(segments.begin() + insertIndex, splitLeft ? 
    LEDSegment(allLEDs, leftLen, leftEffect, toSplitOffset, leftId) :
    LEDSegment(allLEDs, rightLen, rightEffect, rightOffset, rightId));
  uint8_t newIndex = splitLeft ? segmentIndex + 1 : segmentIndex;
  switch(direction) {
    case LEFT:
      segments[newIndex].setNumLEDs(rightLen);
      segments[newIndex].setOffset(rightOffset);
      break;
    case RIGHT:
      segments[newIndex].setNumLEDs(leftLen);
      break;
  }
}
void LEDController::resizeSegmentsFromBoundaries(JsonArray boundaries) {
  uint8_t numBoundaries = boundaries.size();
  for(uint8_t i = 0; i < numBoundaries; i++) {
    uint16_t boundary = boundaries[i];
    bool start = i == 0;
    bool end = (i + 1) == numBoundaries;
    if(start) segments[i].setNumLEDs(boundary);
    if(!end && (segments.size() > 2)) {
      uint16_t nextBoundary = boundaries[i+1];
      uint16_t numLEDs = nextBoundary - boundary;
      segments[i+1].setOffset(boundary);
      segments[i+1].setNumLEDs(numLEDs);
    }
    if(end && (segments.size() > 1)) {
      uint16_t numLEDs = totalLEDs - boundary;
      segments[i+1].setOffset(boundary);
      segments[i+1].setNumLEDs(numLEDs);
    };
  }
}
vector<LEDSegment>* LEDController::getSegments() {
  return &segments;
}
uint16_t LEDController::getTotalLEDs() {
  return totalLEDs;
}
void LEDController::setSegmentEffect(EffectType effectType, uint8_t segmentNum) {
  segments[segmentNum].setEffect(effectType);
}