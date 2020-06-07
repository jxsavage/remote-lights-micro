// Copyright [2020] <Jake Savage>
#include "LEDController.h"
#include <EEPROMSettings.h>
#include <FastLED.h>
#include <LEDSegment.h>
#include <enums.h>
#include <logger.h>

LEDController::LEDController(EEPROMSettings* settings, CRGB* leds) {
  this->allLEDs = leds;
  this->settings = settings;
  this->totalLEDs = settings->getTotalLEDs();

  FastLED.setBrightness(settings->getDefaultBrightness());
  for (int i = 0; i < settings->getNumSegments(); i++) {
    const RemoteLightsSegmentSettings segment = settings->getSegment(i);
    uint16_t offset = segment.offset;
    uint16_t numLEDs = segment.numLEDs;
    EffectType effect = segment.effect;
    segmentId segId = segment.id;
    addSegment(numLEDs, effect, offset, segId);
  }
}
void LEDController::setSegmentId(segmentId oldId, segmentId newId) {
  SegmentIds::iterator id;
  for (id = segmentIds.begin(); id != segmentIds.end(); id++) {
    if (*id.base() == oldId) {
      segmentIds.emplace(id, newId);
      break;
    }
  }
  SegmentIds::iterator segId;
  for (segId = segmentIds.begin(); segId != segmentIds.end(); segId++) {
    if (*segId.base() == oldId) {
      segmentIds.erase(segId);
      break;
    }
  }
  segments[newId] = LEDSegment(
    segments[oldId].getLEDs(), segments[oldId].getNumLEDs(), 
    segments[oldId].getEffect(), segments[oldId].getOffset(), newId);
  removeSegmentFromMap(oldId);
  settings->setSegmentId(oldId, newId);
}
segmentOffset LEDController::getSegmentOffset(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getOffset();
  return -1;
}
segmentNumLEDs LEDController::getSegmentNumLEDs(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getNumLEDs();
  return -1;
}
EffectType LEDController::getSegmentEffectType(segmentId segId) {
  SegmentsMap::iterator seg = segments.find(segId);
  if (seg != segments.end()) return seg->second.getEffect();
  return INVALID_EFFECT;
}
segmentId LEDController::getAdjacentSegId(Direction dir, segmentId segId) {
  SegmentIds::iterator id;
  for (id = segmentIds.begin(); id != segmentIds.end(); id++) {
    if (*id.base() == segId) {
      dir == BEFORE ? id-- : id++;
      return *id.base();
    }
  }
  return 0;
}
void LEDController::removeSegmentId(segmentId segId) {
  SegmentIds::iterator id;
  boolean segmentRemoved = false;
  for (id = segmentIds.begin(); id != segmentIds.end(); id++) {
    if (*id.base() == segId) {
      segmentIds.erase(id);
      segmentRemoved = true;
      break;
    }
  }
  if (!segmentRemoved) {
    REMOTE_LOG((int)ERROR, "failed to remove segmentId", segId);
  }
}
void LEDController::removeSegmentFromMap(segmentId segId) {
  SegmentsMap::iterator segment = segments.find(segId);
  if(segment != segments.end()) {
    segments.erase(segment);
  } else {
    REMOTE_LOG((int)ERROR, "Attempted to remove no existent segment from map.", "Segment Id:", (int)segId);
  };
}
bool LEDController::mapHasSegment(segmentId segId) {
  return segments.find(segId) != segments.end();
}
void LEDController::removeSegment(segmentId segId) {
  removeSegmentId(segId);
  removeSegmentFromMap(segId);
}
void LEDController::mergeSegments(Direction direction, segmentId segId) {
  bool mergeBefore = direction == BEFORE;
  segmentId segIdToMerge = getAdjacentSegId(direction, segId);
  bool validMerge = mapHasSegment(segId) && mapHasSegment(segIdToMerge);
  if (validMerge) {
    segmentOffset offset =
        mergeBefore ? getSegmentOffset(segIdToMerge) : getSegmentOffset(segId);
    segmentNumLEDs numLEDs =
        getSegmentNumLEDs(segIdToMerge) + getSegmentNumLEDs(segId);
    segments[segId].setOffset(offset);
    segments[segId].setNumLEDs(numLEDs);
    removeSegment(segIdToMerge);
  } else {
    /* Invalid Merge */
    REMOTE_LOG((int)COMMAND_FAILURE, (int)MERGE_SEGMENTS, "segId", segId,
               "segIdToMerge", segIdToMerge);
  }
}
void LEDController::addSegment(
  segmentNumLEDs numLEDs, EffectType effectType,
  segmentOffset offset, segmentId id
) {
  segmentIds.push_back(id);
  segments[id] = LEDSegment(allLEDs, numLEDs, effectType, offset, id);
}
void LEDController::renderEffects() {
  SegmentsMap::iterator seg;
  for (seg = segments.begin(); seg != segments.end(); seg++) {
    seg->second.renderEffect();
  }
}
void LEDController::splitSegment(EffectType newEffect, Direction direction,
                                 segmentId segId, segmentId newId) {
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
    segmentNumLEDs segBeforeNumLEDs = (numLEDs > 0) ? numLEDs / 2 : 0;
    /* Segment After */
    segmentId segAfterId = splitBefore ? toSplitId : newId;
    EffectType segAfterEffect = splitBefore ? effect : newEffect;
    segmentNumLEDs segAfterNumLEDs = numLEDs - segBeforeNumLEDs;
    segmentOffset segAfterOffset = segBeforeOffset + segBeforeNumLEDs;

    SegmentIds::iterator segIdIt;
    for (segIdIt = segmentIds.begin(); segIdIt != segmentIds.end(); segIdIt++) {
      const segmentId currentId = *segIdIt.base();
      if (currentId == segId) {
        SegmentIds::iterator insertionPoint = splitBefore ? segIdIt : segIdIt++;
        segmentIds.insert(insertionPoint, newId);
        break;
      }
    }
    segments[newId] =
        splitBefore ? LEDSegment(allLEDs, segBeforeNumLEDs, segBeforeEffect,
                                 segBeforeOffset, segBeforeId)
                    : LEDSegment(allLEDs, segAfterNumLEDs, segAfterEffect,
                                 segAfterOffset, segAfterId);
    switch (direction) {
      case BEFORE:
        segments[segAfterId].setNumLEDs(segAfterNumLEDs);
        segments[segAfterId].setOffset(segAfterOffset);
        break;
      case AFTER:
        segments[segBeforeId].setNumLEDs(segBeforeNumLEDs);
        break;
    }
  } else {
    REMOTE_LOG((int)COMMAND_FAILURE, (int)SPLIT_SEGMENT, "segId", segId);
  }
}
void LEDController::resizeSegmentsFromBoundaries(JsonArray boundaries) {
  uint8_t numBoundaries = boundaries.size();
  uint8_t numSegments = segmentIds.size();
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
  REMOTE_LOG((int)DEBUG, "numBoundaries", numBoundaries, "numSegments",
             numSegments);
#endif
  for (uint8_t i = 0; i < numBoundaries; i++) {
    segmentOffset boundary = boundaries[i];
    bool start = i == 0;
    segmentId id = segmentIds[i];
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
    REMOTE_LOG((int)DEBUG, "boundary", boundary, "segId", id, "isStart", start);
#endif
    if (start) {
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
      REMOTE_LOG((int)DEBUG, "hasStart", mapHasSegment(id));
#endif
      segments[id].setNumLEDs(boundary);
    }

    bool end = (i + 1) == numBoundaries;
    bool middle = !end && (numSegments > 2);
    if (middle) {
      segmentOffset nextBoundary = boundaries[i + 1];
      segmentNumLEDs numLEDs = nextBoundary - boundary;
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
      REMOTE_LOG(
        (int)DEBUG, "MnextBoundary", nextBoundary, "MnumLEDs", numLEDs);
#endif
      segmentId nextId = segmentIds[i + 1];
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
      REMOTE_LOG(
        (int)DEBUG, "hasMiddle", mapHasSegment(id),
        "hasMiddleNext", mapHasSegment(nextId));
#endif
      segments[nextId].setOffset(boundary);
      segments[nextId].setNumLEDs(numLEDs);
    }
    if (end && (numSegments > 1)) {
      segmentId nextId = segmentIds[i + 1];
      segmentNumLEDs numLEDs = totalLEDs - boundary;
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
      REMOTE_LOG((int)DEBUG, "hasEnd", mapHasSegment(id), "hasEndNext",
                 mapHasSegment(nextId), "endNumLEDs", numLEDs);
#endif
      segments[nextId].setOffset(boundary);
      segments[nextId].setNumLEDs(numLEDs);
    }
  }
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
  REMOTE_LOG((int)DEBUG, "Resize Complete!");
#endif
}
SegmentsMap* LEDController::getSegments() { return &segments; }
SegmentIds* LEDController::getSegmentIds() { return &segmentIds; }
uint16_t LEDController::getTotalLEDs() { return totalLEDs; }
void LEDController::setSegmentEffect(
  EffectType effectType, segmentId segmentId
) {
  segments[segmentId].setEffect(effectType);
}
