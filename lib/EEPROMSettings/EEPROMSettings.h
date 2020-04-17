#ifndef EEPROM_SETTINGS
#define EEPROM_SETTINGS
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
#include <string>
struct FastLEDStripSettings
{
  uint16_t offset;
  uint16_t numLEDs;
};
struct RemoteLightsSegmentSettings
{
  uint32_t id;
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
  void writeDefault();
  const uint32_t getId();
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