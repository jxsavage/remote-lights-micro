#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
#include <LEDController.h>
#include <vector>
typedef uint8_t segmentIndex;
/*
* Command codes sent from controller
*/
enum Commands {
  GET_STATE = 1, RESET_MICRO, SPLIT_SEGMENT,
  SET_BRIGHTNESS, MERGE_SEGMENTS, SET_SEGMENT_EFFECT,
  RESIZE_SEGMENTS_FROM_BOUNDARIES
};
/*
* Method codes sent from controller
*/
class CommandParser {
  LEDController* controller;
  EEPROMSettings* settings;
  public:
    CommandParser() = default;
    CommandParser(LEDController* controller, EEPROMSettings* settings) {
      this->controller = controller;
      this-> settings = settings;
    }
    void getState();
    void setState();
    JsonArray getSegments();
    void setBrightness(uint8_t brightness);
    void parseCommand(String serialCommand);
    void setSegmentEffect(EffectType, segmentIndex);
    void resizeSegmentsFromBoundaries(JsonArray boundaries);
    void mergeSegments (Direction direciton, uint8_t segmentIndex);
    void splitSegment(EffectType effect, Direction direction, uint8_t segmentIndex);
    void errorResponse(const char*, const char*);
};
#endif