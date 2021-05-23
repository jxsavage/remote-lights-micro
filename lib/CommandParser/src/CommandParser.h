#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <LEDSegment.h>
#include <ArduinoJson.h>
#include <ClientSerial.h>
#include <LEDController.h>
#include <EEPROMSettings.h>
/*
* Method codes sent from controller
*/
class CommandParser {
  LEDController* controller;
  EEPROMSettings* settings;
  uint16_t currentCommandId;
  uint8_t currentResponseHeader;
  public:
    CommandParser() = default;
    CommandParser(LEDController* controller, EEPROMSettings* settings) {
      this-> settings = settings;
      this->controller = controller;
    }
    
    void getState();
    void setState();
    void resetMicro();
    void loadEEPROM();
    void writeEEPROM();
    void restoreDefault();
    JsonArray getSegments();
    void setMicroId(uint32_t);
    void setBrightness(uint8_t brightness);
    void parseCommand(String serialCommand);
    void mergeSegments (Direction, segmentId);
    void setSegmentEffect(EffectType, segmentId);
    void setSegmentId(segmentId oldId, segmentId newId);
    void resizeSegmentsFromBoundaries(JsonArray boundaries);
    void splitSegment(EffectType, Direction, segmentId, segmentId newId);
};
#endif