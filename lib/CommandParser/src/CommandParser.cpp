#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>
/*
* Command codes sent from controller
*/
const enum Commands {Brightness = 0, Effect = 1, Info = 2};
/*
* Effect codes sent from controller
*/
const enum Effects { Colorwaves = 0, Blendwave = 1};
/*
* Method codes sent from controller
*/
const enum Methods {Get = 0, Set = 1};

void CommandParser::parseCommand(String commandStr) {
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    StaticJsonDocument<200> er;
    JsonObject err = er.to<JsonObject>();
    err["error"] = "error deserializeJson() failed.";
    err["message"] = error.c_str();
    serializeJson(err, Serial);
    Serial.print('\n');
  } else {
    uint8_t cmd = command["cmd"];
    if (cmd == Brightness) {
      parseBrightness();
    } else if (cmd == Effect) {

    } else if (cmd == Info) {

    }
  }
}

void CommandParser::parseBrightness() {
 uint8_t method = command["method"];
 if (method == Set) {
   uint8_t brightness = command["value"];
   setBrightness(brightness);
 } else if (method == Get) {
   getBrightness();
 }
}
void CommandParser::getBrightness() {

}
void CommandParser::setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
}
void CommandParser::parseEffect() {
  uint8_t method = command["method"];
}
void CommandParser::parseInfo() {
  uint8_t method = command["method"];
}