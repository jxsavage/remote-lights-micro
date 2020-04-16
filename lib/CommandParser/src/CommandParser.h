#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
#include <EffectFactory.h>
#include <LEDController.h>
#include <vector>
typedef uint8_t segmentIndex;
/*
* Command codes sent from controller
*/
enum Commands {
  GET_STATE = 1, RESET_MICRO, SPLIT_SEGMENT,
  SET_BRIGHTNESS, MERGE_SEGMENTS, SET_SEGMENT_EFFECT,
  RESIZE_SEGMENTS_FROM_BOUNDARIES};
/*
* Method codes sent from controller
*/
class CommandParser {
  LEDController* controller;
  EEPROMSettings* settings;
  public:
    CommandParser(LEDController* controller, EEPROMSettings* settings) {
      this->controller = controller;
      this-> settings = settings;
    }
    void parseCommand(String serialCommand);
    void setBrightness(uint8_t brightness);
    void setSegmentEffect(EffectType, segmentIndex);
    void getState();
    void setState();
    JsonArray getSegments();
    void errorResponse(const char*, const char*);
};
#endif