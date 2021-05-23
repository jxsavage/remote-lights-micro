#ifndef EEPROM_SETTINGS
#define EEPROM_SETTINGS
// #define __MK66FX1M0__
#if defined(__MK66FX1M0__) && !defined(USE_TEENSY)
#pragma message("Compiling with USE_TEENSY Set")
#define USE_TEENSY
#elif !defined(USE_ESP32)
#pragma message("Compiling with USE_ESP32 Set")
#define USE_ESP32
#endif
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
#include <LEDSegment.h>
struct FastLEDStripSettings
{
  uint16_t offset;
  uint16_t numLEDs;
};
struct RemoteLightsSegmentSettings
{
  segmentId id;
  uint16_t offset;
  uint16_t numLEDs;
  EffectType effect;
};
struct Settings
{
  int microId;
  int totalLEDs;
  uint8_t defaultBrightness;
  uint8_t numStrips;
  FastLEDStripSettings strips[2];
  uint8_t numSegments;
  RemoteLightsSegmentSettings segments[10];
};

class EEPROMSettings
{
public:
  EEPROMSettings();
  
  void clearEEPROM();
  void writeEEPROM();
  void loadSettings();
  void writeDefault();
  void restoreDefault();
  void setId(int);
  int getId();
  void setSegmentId(segmentId oldId, segmentId newId);
  Settings getSettings();
  uint16_t getTotalLEDs();
  uint8_t getDefaultBrightness();
  boolean areSettingsLoaded();
  uint8_t getNumStrips();
  FastLEDStripSettings getStrip(uint8_t stripIndex);
  uint8_t getNumSegments();
  RemoteLightsSegmentSettings getSegment(uint8_t segmentIndex);
  

private:
  Settings settings;
  boolean settingsLoaded;
};
#endif