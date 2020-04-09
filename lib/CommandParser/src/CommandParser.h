#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
#include <EffectFactory.h>
#include <LEDController.h>
#include <vector>
typedef uint8_t segmentNum;
/*
* Command codes sent from controller
*/
enum Commands {
  GET_INFO = 1, RESET_MICRO, SPLIT_SEGMENT,
  SET_BRIGHTNESS, MERGE_SEGMENTS, SET_SEGMENT_EFFECT,
  RESIZE_SEGMENTS_FROM_BOUNDARIES};
/*
* Method codes sent from controller
*/
class CommandParser {
  LEDController* controller;
  public:
    CommandParser(LEDController* controller) {
      this->controller = controller;
    }
    void parseCommand(String serialCommand);
    void parseBrightness(JsonDocument&  command);
    void getBrightness(JsonDocument& command);
    void setBrightness(uint8_t brightness);
    void parseSegment(JsonDocument& command);
    void setSegmentProp(JsonDocument& command);
    void setSegmentEffect(EffectType, segmentNum);
    void parseEffect();
    void getEffect();
    void parseInfo(JsonDocument&);
    void getInfo();
    void setInfo();
    JsonArray getSegments();
    void errorResponse(const char*, const char*);
};
#endif