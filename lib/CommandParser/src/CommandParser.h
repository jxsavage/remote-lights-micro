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
enum Commands {CMD_Brightness = 0, CMD_Segment = 1,
  CMD_Info = 2};
enum SegmentCommands {SCMD_Effect};
/*
* Method codes sent from controller
*/
enum Methods {MTHD_Get = 0, MTHD_Set = 1};
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