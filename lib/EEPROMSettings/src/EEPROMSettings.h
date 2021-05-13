#ifndef EEPROM_SETTINGS
#define EEPROM_SETTINGS
#define USE_ESP32
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
  uint32_t microId;
  uint16_t totalLEDs;
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
  void writeDefault();
  void setId(uint32_t);
  const uint32_t getId();
  void setSegmentId(segmentId oldId, segmentId newId);
  const Settings getSettings();
  const uint16_t getTotalLEDs();
  const uint8_t getDefaultBrightness();
  const boolean areSettingsLoaded();
  const uint8_t getNumStrips();
  const FastLEDStripSettings getStrip(uint8_t stripIndex);
  const uint8_t getNumSegments();
  const RemoteLightsSegmentSettings getSegment(uint8_t segmentIndex);
  

private:
  Settings settings;
  boolean settingsLoaded;
};
#endif