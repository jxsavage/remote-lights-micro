#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
#include <EffectFactory.h>
#include <LEDController.h>
/*
* Command codes sent from controller
*/
enum Commands {CMD_Brightness = 0, CMD_Effect = 1, CMD_Info = 2};
/*
* Method codes sent from controller
*/
enum Methods {MTHD_Get = 0, MTHD_Set = 1};
class CommandParser {
  StaticJsonDocument<200> command;
  JsonObject response;
  LEDController* controller;
  public:
    CommandParser(LEDController* controller) {
      StaticJsonDocument<200> responseDoc;
      response = responseDoc.to<JsonObject>();
        this->controller = controller;
    }
    void parseCommand(String);
    void parseBrightness();
    void getBrightness();
    void setBrightness(uint8_t);
    void parseEffect();
    void getEffect();
    void setEffect(EffectType);
    void parseInfo();
    void errorResponse(const char*, const char*);
};
#endif