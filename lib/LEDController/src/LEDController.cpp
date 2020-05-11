#include <FastLED.h>
#include <LEDSegment.h>
#include <EEPROMSettings.h>
#include <logger.h>
#include <enums.h>
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
    segmentId segId = segment.id;
    addSegment(numLEDs, effect, offset, segId);
  };
}
segmentOffset LEDController::getSegmentOffset(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getOffset();
  return -1;
};
segmentNumLEDs LEDController::getSegmentNumLEDs(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getNumLEDs();
  return -1;
};
EffectType LEDController::getSegmentEffectType(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getEffect();
  return INVALID_EFFECT;
};
segmentId LEDController::getAdjacentSegId(Direction dir, segmentId segId) {
  SegmentIds::iterator id;
  for (id = segmentIds.begin(); id != segmentIds.end(); id++) {
    if(*id.base() == segId) {
      dir == BEFORE ? id-- : id++;
      return *id.base();
    } 
  }
  return 0;
};
void LEDController::removeSegmentId(segmentId segId) {
  SegmentIds::iterator id;
  for (id = segmentIds.begin(); id != segmentIds.end(); id++) {
    if(*id.base() == segId) {
      segmentIds.erase(id);
      break;
    } 
  }
}
void LEDController::removeSegmentFromMap(segmentId segId) {
  segments.erase(segments.find(segId));
};
bool LEDController::mapHasSegment(segmentId segId) {
  return segments.find(segId) != segments.end();
};
void LEDController::removeSegment(segmentId segId) {
  removeSegmentId(segId);
  removeSegmentFromMap(segId);
};
void LEDController::mergeSegments(Direction direction, segmentId segId) {
  bool mergeBefore = direction == BEFORE;
  segmentId segIdToMerge = getAdjacentSegId(direction, segId);
  bool validMerge = mapHasSegment(segId) && mapHasSegment(segIdToMerge);
  if (validMerge) {
    segmentOffset offset = mergeBefore ?
      getSegmentOffset(segIdToMerge) :
      getSegmentOffset(segId);
    segmentNumLEDs numLEDs =
      getSegmentNumLEDs(segIdToMerge) + getSegmentNumLEDs(segId);
    segments[segId].setOffset(offset);
    segments[segId].setNumLEDs(numLEDs);
    removeSegment(segIdToMerge);
  } else {
    /* Invalid Merge */
    REMOTE_LOG(
      (int)COMMAND_FAILURE, (int)MERGE_SEGMENTS,
      "segId", segId,
      "segIdToMerge", segIdToMerge
    );
  }
}
void LEDController::addSegment(segmentNumLEDs numLEDs, EffectType effectType, segmentOffset offset, segmentId id) {
  segmentId segId;
  segId = (id == 0) ? LEDSegment::generateId() : id;
  segmentIds.push_back(segId);
  segments[segId] = LEDSegment(allLEDs, numLEDs, effectType, offset, segId);
}
void LEDController::renderEffects() {
  SegmentsMap::iterator seg;
  for (seg = segments.begin(); seg != segments.end(); seg++){
    seg->second.renderEffect();
  }
}
void LEDController::splitSegment(EffectType newEffect, Direction direction, segmentId segId, segmentId newId) {
  bool validSplit = mapHasSegment(segId);
  if (validSplit) {
    segmentNumLEDs numLEDs = segments[segId].getNumLEDs();
    EffectType effect = segments[segId].getEffect();
    segmentId toSplitId = segments[segId].getId();
    bool splitBefore = direction == BEFORE;
    /* Segment Before */
    segmentId segBeforeId = splitBefore ? newId : toSplitId;
    EffectType segBeforeEffect = splitBefore ? newEffect : effect;
    segmentOffset segBeforeOffset = segments[segId].getOffset();
    segmentNumLEDs segBeforeNumLEDs = (numLEDs > 0) ?  numLEDs / 2 : 0;
    /* Segment After */
    segmentId segAfterId = splitBefore ? toSplitId : newId;
    EffectType segAfterEffect = splitBefore ? effect : newEffect;
    segmentNumLEDs segAfterNumLEDs = numLEDs - segBeforeNumLEDs;
    segmentOffset segAfterOffset = segBeforeOffset + segBeforeNumLEDs;

    SegmentIds::iterator segIdIt;
    for(segIdIt = segmentIds.begin(); segIdIt != segmentIds.end(); segIdIt++) {
      const segmentId currentId = *segIdIt.base();
      if(currentId == segId) {
        SegmentIds::iterator insertionPoint  = splitBefore ? segIdIt : segIdIt++;
        segmentIds.insert(insertionPoint, newId);
        break;
      }
    }
    segments[newId] = splitBefore ?
    LEDSegment(allLEDs, segBeforeNumLEDs, segBeforeEffect, segBeforeOffset, segBeforeId) :
    LEDSegment(allLEDs, segAfterNumLEDs, segAfterEffect, segAfterOffset, segAfterId);
    switch(direction) {
      case BEFORE:
        segments[segAfterId].setNumLEDs(segAfterNumLEDs);
        segments[segAfterId].setOffset(segAfterOffset);
        break;
      case AFTER:
        segments[segBeforeId].setNumLEDs(segBeforeNumLEDs);
        break;
    }
  } else {
    REMOTE_LOG(
      (int)COMMAND_FAILURE, (int)SPLIT_SEGMENT,
      "segId", segId
    );
  }

}
void LEDController::resizeSegmentsFromBoundaries(JsonArray boundaries) {
  uint8_t numBoundaries = boundaries.size();
  uint8_t numSegments = segmentIds.size();
  for(uint8_t i = 0; i < numBoundaries; i++) {

    segmentOffset boundary = boundaries[i];
    bool start = i == 0;
    if(start) segments[segmentIds[i]].setNumLEDs(boundary);

    bool end = (i + 1) == numBoundaries;
    bool middle = !end && (numSegments > 2);
    if(middle) {
      segmentOffset nextBoundary = boundaries[i+1];
      segmentNumLEDs numLEDs = nextBoundary - boundary;
      segments[segmentIds[i+1]].setOffset(boundary);
      segments[segmentIds[i+1]].setNumLEDs(numLEDs);
    }
    if(end && (numSegments > 1)) {
      segmentNumLEDs numLEDs = totalLEDs - boundary;
      segments[segmentIds[i+1]].setOffset(boundary);
      segments[segmentIds[i+1]].setNumLEDs(numLEDs);
    };
  }
}
SegmentsMap* LEDController::getSegments() {
  return &segments;
}
SegmentIds* LEDController::getSegmentIds() {
  return &segmentIds;
}
uint16_t LEDController::getTotalLEDs() {
  return totalLEDs;
}
void LEDController::setSegmentEffect(EffectType effectType, segmentId segmentId) {
  segments[segmentId].setEffect(effectType);
}