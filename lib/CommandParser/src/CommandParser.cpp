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
    } else if (cmd == SET_MICRO_ID) {
      uint32_t newId = command[1];
      setMicroId(newId);
    } else if (cmd == SET_SEGMENT_ID) {
      segmentId oldId = command[1];
      segmentId newId = command[2];
      setSegmentId(oldId, newId);
    } else if (cmd == WRITE_EEPROM) {
      writeEEPROM();
    }
  }
}
void CommandParser::writeEEPROM() {
  controller->writeEEPROM();
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
  response.add((int)settings->getId());
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
void CommandParser::setSegmentId(segmentId oldId, segmentId newId) {
  controller->setSegmentId(oldId, newId);
}
void CommandParser::setMicroId(uint32_t id) {
  settings->setId(id);
}
// https://forum.pjrc.com/threads/24304-_reboot_Teensyduino()-vs-_restart_Teensyduino()
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
void CommandParser::resetMicro() {
  CPU_RESTART
}