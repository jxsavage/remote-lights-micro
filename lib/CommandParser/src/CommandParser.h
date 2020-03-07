#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <ArduinoJson.h>
class CommandParser {
  StaticJsonDocument<200> command;
  public:
    CommandParser() {
    }
    void parseCommand(String);
    void parseBrightness();
    void getBrightness();
    void setBrightness(uint8_t);
    void parseEffect();
    void parseInfo();
};
#endif