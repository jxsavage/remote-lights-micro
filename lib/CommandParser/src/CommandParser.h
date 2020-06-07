#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
#include <LEDController.h>
#include <LEDSegment.h>

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
    void setMicroId(uint32_t);
    void setSegmentId(segmentId oldId, segmentId newId);
    void setBrightness(uint8_t brightness);
    void parseCommand(String serialCommand);
    void setSegmentEffect(EffectType, segmentId);
    void resizeSegmentsFromBoundaries(JsonArray boundaries);
    void mergeSegments (Direction, segmentId);
    void splitSegment(EffectType, Direction, segmentId, segmentId newId);
};
#endif