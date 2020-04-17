#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>
#include <vector>
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
void CommandParser::parseCommand(String commandStr) {
  StaticJsonDocument<400> command;
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    errorResponse("deserializeJson() Failed.", error.c_str());
  } else {
    uint8_t cmd = command[0];
    if (cmd == GET_STATE) {
      getState();
    } else if (cmd == RESET_MICRO) {
    } else if (cmd == SPLIT_SEGMENT) {
      uint8_t effect = command[1];
      uint8_t direction = command[2];
      uint8_t segmentIndex = command[3];
      splitSegment(static_cast<EffectType>(effect), static_cast<Direction>(direction), segmentIndex);
    } else if (cmd == SET_BRIGHTNESS) {
      setBrightness(command[1]);
    } else if (cmd == MERGE_SEGMENTS) {
      uint8_t direction = command[1];
      uint8_t segmentIndex = command[2];
      mergeSegments(static_cast<Direction>(direction), segmentIndex);
    } else if (cmd == SET_SEGMENT_EFFECT) {
      uint8_t effect = command[1];
      uint8_t segmentIndex = command[2];
      setSegmentEffect(static_cast<EffectType>(effect), segmentIndex);
    } else if (cmd == RESIZE_SEGMENTS_FROM_BOUNDARIES) {
      JsonArray boundaries = command[1];
      resizeSegmentsFromBoundaries(boundaries);
    }
  }
}
void CommandParser::mergeSegments(Direction direction, uint8_t segmentIndex) {
  controller->mergeSegments(direction, segmentIndex);
}
void CommandParser::splitSegment(EffectType newEffect, Direction direction, uint8_t segmentIndex) {
  controller->splitSegment(newEffect, direction, segmentIndex);
}
void CommandParser::resizeSegmentsFromBoundaries(JsonArray boundaries) {
  controller->resizeSegmentsFromBoundaries(boundaries);
}
void CommandParser::errorResponse(const char* error, const char* message) {
  StaticJsonDocument<200> responseDoc;
  JsonArray response = responseDoc.to<JsonArray>();
  response.add(100);
  response.add(error);
  response.add(message);
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
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
  vector<LEDSegment>* segments = controller->getSegments();
  StaticJsonDocument<JSON_ARRAY_SIZE(33)> segmentsDoc;
  JsonArray segmentsArr = segmentsDoc.to<JsonArray>();
  
  vector<LEDSegment>::iterator seg;
  for (seg = segments->begin(); seg != segments->end(); seg++){
    JsonArray segment = segmentsArr.createNestedArray();
    segment.add(seg->getOffset());
    segment.add(seg->getNumLEDs());
    segment.add((int)seg->getEffect());
    segment.add(seg->getId());
  }
  return segmentsArr;
}
void CommandParser::setSegmentEffect(EffectType effectType, uint8_t segmentIndex) {
  controller->setSegmentEffect(effectType, segmentIndex);
}