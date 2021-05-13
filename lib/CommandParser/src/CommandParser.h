#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <LEDSegment.h>
#include <ArduinoJson.h>
#include <ClientSerial.h>
#include <LEDController.h>
#include <EEPROMSettings.h>
#ifdef USE_TEENSY
typedef HardwareSerial SerialType;
#endif
#ifdef USE_ESP32
#include <BluetoothSerial.h>
typedef BluetoothSerial SerialType;
#endif

/*
* Method codes sent from controller
*/
class CommandParser {
  LEDController* controller;
  EEPROMSettings* settings;
  ClientSerial<SerialType>* client;
  uint8_t currentCommandId;
  uint8_t currentResponseHeader;
  public:
    CommandParser() = default;
    CommandParser(LEDController* controller, EEPROMSettings* settings, ClientSerial<SerialType>* client) {
      this-> settings = settings;
      this->controller = controller;
      this->client = client;
    }
    
    void getState();
    void setState();
    void loadEEPROM();
    void writeEEPROM();
    void resetMicro();
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