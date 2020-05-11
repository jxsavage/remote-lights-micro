#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>
#include <enums.h>
#include <logger.h>
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
void CommandParser::parseCommand(String commandStr) {
  StaticJsonDocument<400> command;
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    REMOTE_LOG(ERROR, "deserializeJson() Failed.", error.c_str());
  } else {
    uint8_t cmd = command[0];
    if (cmd == GET_STATE) {
      getState();
    } else if (cmd == RESET_MICRO) {
    } else if (cmd == SPLIT_SEGMENT) {
      uint8_t effect = command[1];
      uint8_t direction = command[2];
      segmentId segId = command[3];
      segmentId newId = command[4];
      splitSegment(static_cast<EffectType>(effect), static_cast<Direction>(direction), segId, newId);
    } else if (cmd == SET_BRIGHTNESS) {
      setBrightness(command[1]);
    } else if (cmd == MERGE_SEGMENTS) {
      uint8_t direction = command[1];
      segmentId segmentId = command[2];
      mergeSegments(static_cast<Direction>(direction), segmentId);
    } else if (cmd == SET_SEGMENT_EFFECT) {
      uint8_t effect = command[1];
      segmentId segmentId = command[2];
      setSegmentEffect(static_cast<EffectType>(effect), segmentId);
    } else if (cmd == RESIZE_SEGMENTS_FROM_BOUNDARIES) {
      JsonArray boundaries = command[1];
      resizeSegmentsFromBoundaries(boundaries);
    }
  }
}
void CommandParser::mergeSegments(Direction direction, segmentId segId) {
  controller->mergeSegments(direction, segId);
}
void CommandParser::splitSegment(EffectType newEffect, Direction direction, segmentId segId, segmentId newId) {
  controller->splitSegment(newEffect, direction, segId, newId);
}
void CommandParser::resizeSegmentsFromBoundaries(JsonArray boundaries) {
  controller->resizeSegmentsFromBoundaries(boundaries);
}
void CommandParser::setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
}
void CommandParser::getState() {
  StaticJsonDocument<JSON_ARRAY_SIZE(36)> responseDoc;
  JsonArray response = responseDoc.to<JsonArray>();
  response.add((int)GET_STATE);
  response.add(settings->getId());
  response.add(controller->getTotalLEDs());
  response.add(FastLED.getBrightness());
  response.add(getSegments());
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}
void CommandParser::setState() {

}
JsonArray CommandParser::getSegments() {
  SegmentIds* segmentIds = controller->getSegmentIds();
  SegmentsMap* segments = controller->getSegments();
  StaticJsonDocument<JSON_ARRAY_SIZE(33)> segmentsDoc;
  JsonArray segmentsArr = segmentsDoc.to<JsonArray>();
  
  SegmentIds::iterator segId;
  for (segId = segmentIds->begin(); segId != segmentIds->end(); segId++){
    JsonArray segment = segmentsArr.createNestedArray();
    const segmentId id = *segId.base();
    segment.add(segments->at(id).getOffset());
    segment.add(segments->at(id).getNumLEDs());
    segment.add((int)segments->at(id).getEffect());
    segment.add(segments->at(id).getId());
  }
  return segmentsArr;
}
void CommandParser::setSegmentEffect(EffectType effectType, segmentId segId) {
  controller->setSegmentEffect(effectType, segId);
}