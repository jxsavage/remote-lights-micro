#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>
#include <vector>

void CommandParser::parseCommand(String commandStr) {
  StaticJsonDocument<400> command;
  JsonArray segmentsArr = command.to<JsonArray>();
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    errorResponse("deserializeJson() Failed.", error.c_str());
  } else {
    uint8_t cmd = command[0];
    if (cmd == GET_INFO) {
      getInfo();
    } else if (cmd == RESET_MICRO) {
    } else if (cmd == SPLIT_SEGMENT) {
    } else if (cmd == SET_BRIGHTNESS) {
      setBrightness(command[1]);
    } else if (cmd == MERGE_SEGMENTS) {
      
    } else if (cmd == SET_SEGMENT_EFFECT) {
      
    } else if (cmd == RESIZE_SEGMENTS_FROM_BOUNDARIES) {
      
    }
  }
}
void CommandParser::errorResponse(const char* error, const char* message) {
  StaticJsonDocument<200> responseDoc;
  JsonObject response = responseDoc.to<JsonObject>();
  response["error"] = error;
  response["message"] = message;
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}
void CommandParser::setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
}
void CommandParser::getInfo() {
  StaticJsonDocument<JSON_ARRAY_SIZE(36)> responseDoc;
  JsonArray response = responseDoc.to<JsonArray>();
  response.add((int)GET_INFO);
  response.add(controller->getTotalLEDs());
  response.add(FastLED.getBrightness());
  response.add(getSegments());
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}
void CommandParser::setInfo() {

}
JsonArray CommandParser::getSegments() {
  vector<LEDSegment>* segments = controller->getSegments();
  StaticJsonDocument<JSON_ARRAY_SIZE(33)> segmentsDoc;
  JsonArray segmentsArr = segmentsDoc.to<JsonArray>();
  
  vector<LEDSegment>::iterator seg;
  for (seg = segments->begin(); seg != segments->end(); seg++){
    StaticJsonDocument<JSON_ARRAY_SIZE(3)> segmentDoc;
    JsonArray segment = segmentDoc.to<JsonArray>();
    segment.add(seg->getOffset());
    segment.add(seg->getNumLEDs());
    segment.add((int)seg->getEffect());
    segmentsArr.add(segment);
  }
  return segmentsArr;
}
void CommandParser::setSegmentEffect(EffectType effectType, uint8_t segmentNum) {
  controller->setSegmentEffect(effectType, segmentNum);
}