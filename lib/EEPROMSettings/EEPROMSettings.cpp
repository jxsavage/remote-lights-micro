#include "EEPROMSettings.h"
#include <logger.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>

Settings defaultSettings = {
  microId: "unitialized",
  totalLEDs: 288,
  defaultBrightness: 20,
  strips: {
    {
      offset: 0,
      numLEDs: 144,
      effect: ET_BlendWave,
    },
    {
      offset: 144,
      numLEDs: 144,
      effect: ET_ColorWaves,
    }
  }
};

EEPROMSettings::EEPROMSettings()
{
  settingsLoaded = false;
  if (!settingsLoaded)
  {
    writeDefault();
    settingsLoaded = true;
  };
  EEPROM.get(1, settings);
  settingsLoaded = true;
}
void EEPROMSettings::writeDefault()
{
  EEPROM.put(1, defaultSettings);
  EEPROM.put(0, 255);
}
void EEPROMSettings::clearEEPROM()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}
const boolean EEPROMSettings::areSettingsLoaded()
{
  return EEPROM.read(0) == 255;
}
Settings const EEPROMSettings::getSettings()
{
  return settings;
}