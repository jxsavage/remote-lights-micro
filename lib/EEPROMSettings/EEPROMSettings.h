#ifndef EEPROM_SETTINGS
#define EEPROM_SETTINGS
#include <logger.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
typedef uint8_t stripPins[2];
struct FastLEDStripSettings
{
  uint16_t offset;
  uint16_t numLEDs;
  EffectType effect;
};
// FastLEDStripSettings defaultStrip1 = {
//     .offset = 0,
//     .numLEDs = 144,
//     .effect = ET_BlendWave,
// };
// FastLEDStripSettings defaultStrip2 = {
//     .offset = 144,
//     .numLEDs = 144,
//     .effect = ET_ColorWaves,
// };
struct Settings
{
  char microId[12];
  uint16_t totalLEDs;
  uint8_t defaultBrightness;
  FastLEDStripSettings strips[2];
};

class EEPROMSettings
{
public:
  EEPROMSettings();
  void writeDefault();
  void clearEEPROM();
  const boolean areSettingsLoaded();
  Settings const getSettings();

private:
  Settings settings;
  boolean settingsLoaded;
};
#endif