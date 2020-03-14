#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>

CommandParser::CommandParser(LEDController controller) {
  this->controller = controller;
}
void CommandParser::parseCommand(String commandStr) {
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    errorResponse("deserializeJson() Failed.", error.c_str());
    // response.clear();
    // response["error"] = "error deserializeJson() failed.";
    // response["message"] = error.c_str();
    // serializeJson(response, Serial);
    // Serial.print('\n');
  } else {
    uint8_t cmd = command["cmd"];
    if (cmd == CMD_Brightness) {
      parseBrightness();
    } else if (cmd == CMD_Effect) {

    } else if (cmd == CMD_Info) {

    }
  }
}
void CommandParser::errorResponse(const char* error, const char* message) {
  response.clear();
  response["error"] = error;
  response["message"] = message;
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}

void CommandParser::parseBrightness() {
 uint8_t method = command["method"];
 if (method == MTHD_Set) {
   uint8_t brightness = command["value"];
   setBrightness(brightness);
 } else if (method == MTHD_Get) {
   getBrightness();
 }
}
void CommandParser::getBrightness() {
  response.clear();
  response["client"] = command["client"];
  response["prop"] = CMD_Brightness;
  response["value"] = FastLED.getBrightness();
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
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