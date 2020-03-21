#include "CommandParser.h"
#include <ArduinoJson.h>
#include <FastLED.h>
#include <vector>

void CommandParser::parseCommand(String commandStr) {
  StaticJsonDocument<400> command;
  DeserializationError error = deserializeJson(command, commandStr);
  if(error) {
    errorResponse("deserializeJson() Failed.", error.c_str());
  } else {
    uint8_t cmd = command["cmd"];
    if (cmd == CMD_Brightness) {
      parseBrightness(command);
    } else if (cmd == CMD_Segment) {
      parseSegment(command);
    } else if (cmd == CMD_Info) {
      parseInfo(command);
    }
  }
}
void CommandParser::errorResponse(const char* error, const char* message) {
  StaticJsonDocument<200> responseDoc;
  JsonObject response = responseDoc.to<JsonObject>();
  response["error"] = error;
  response["message"] = message;
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}
void CommandParser::parseSegment(JsonDocument& command) {
  uint8_t method = command["method"];
  if (method == MTHD_Set) {
    setSegmentProp(command);
  } else if (method == MTHD_Get) {
  }
}
void CommandParser::setSegmentProp(JsonDocument& command) {
  uint8_t prop = command["prop"];
  if (prop == SCMD_Effect) {
    uint8_t segment = command["segment"];
    uint8_t effectType = command["value"];
    EffectType effect = static_cast<EffectType>(effectType);
    setSegmentEffect(effect, segment);
  }
}
void CommandParser::parseBrightness(JsonDocument& command) {
 uint8_t method = command["method"];
 if (method == MTHD_Set) {
   uint8_t brightness = command["value"];
   setBrightness(brightness);
 } else if (method == MTHD_Get) {
   getBrightness(command);
 }
}
void CommandParser::getBrightness(JsonDocument& command) {
  StaticJsonDocument<200> responseDoc;
  JsonObject response = responseDoc.to<JsonObject>();
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
  // uint8_t method = command["method"];
}
void CommandParser::parseInfo(JsonDocument& command) {
  uint8_t method = command["method"];
  if (method == MTHD_Set) {
    setInfo();
  } else if (method == MTHD_Get) {
    getInfo();
  }
}
void CommandParser::getInfo() {
  StaticJsonDocument<400> responseDoc;
  JsonObject response = responseDoc.to<JsonObject>();
  response["prop"] = (int)CMD_Info;
  response["totalLEDs"] = controller->getTotalLEDs();
  response["brightness"] = FastLED.getBrightness();
  response["segments"] = getSegments();
  serializeJson(response, Serial);
  Serial.print('\n');
  Serial.flush();
}
void CommandParser::setInfo() {

}
JsonArray CommandParser::getSegments() {
  StaticJsonDocument<400> segmentsDoc;
  JsonArray segmentsArr = segmentsDoc.to<JsonArray>();
  vector<LEDSegment>* segments = controller->getSegments();
  vector<LEDSegment>::iterator seg;
  for (seg = segments->begin(); seg != segments->end(); seg++){
    StaticJsonDocument<200> segmentDoc;
    JsonObject segment = segmentDoc.to<JsonObject>();
    segment["numLEDs"] = seg->getNumLEDs();
    segment["offset"] = seg->getOffset();
    segment["effect"] = (int)seg->getEffect();
    segmentsArr.add(segment);
  }
  return segmentsArr;
}
void CommandParser::setSegmentEffect(EffectType effectType, uint8_t segmentNum) {
  controller->setSegmentEffect(effectType, segmentNum);
}