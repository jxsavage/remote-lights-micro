#include "EEPROMSettings.h"
#include <logger.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
char* generateMicroId() {
    static char id[12];
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i <= 11; ++i) {
        id[i] = alphanum[random(1, 2147483647) % (sizeof(alphanum) - 1)];
    }

    return id;
};
Settings defaultSettings = {
  microId : 0,
  totalLEDs : 288,
  defaultBrightness : 20,
  numStrips : 2,
  strips : {
    {
      offset : 0,
      numLEDs : 144,
    },
    {
      offset : 144,
      numLEDs : 144,
    }
  },
  numSegments : 2,
  segments : {
    {
      id: 0,
      offset : 0,
      numLEDs : 144,
      effect : BLENDWAVE
    },
    {
      id: 0,
      offset : 144,
      numLEDs : 144,
      effect : COLORWAVES
    },
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
  // static char microId[12];
  // static const char alphanum[] =
  //   "0123456789"
  //   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  //   "abcdefghijklmnopqrstuvwxyz";

  // for (int i = 0; i < 11; ++i) {
  //   microId[i] = alphanum[random(1, 2147483647) % (sizeof(alphanum) - 1)];
  // }
  // microId[11] = 0;
  // strcpy(defaultSettings.microId, microId);
  defaultSettings.microId = random(1, 2147483647);
  for(int i = 0; i < defaultSettings.numSegments; i++) {
    defaultSettings.segments[i].id = random(1, 2147483647);
  }
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
const Settings EEPROMSettings::getSettings()
{
  return settings;
}

const uint16_t EEPROMSettings::getTotalLEDs()
{
  return settings.totalLEDs;
}
const uint8_t EEPROMSettings::getDefaultBrightness()
{
  return settings.defaultBrightness;
}
const FastLEDStripSettings EEPROMSettings::getStrip(uint8_t stripIndex)
{
  return settings.strips[stripIndex];
}
const RemoteLightsSegmentSettings EEPROMSettings::getSegment(uint8_t segmentIndex)
{
  return settings.segments[segmentIndex];
}
const uint8_t EEPROMSettings::getNumStrips()
{
  return settings.numStrips;
}
const uint8_t EEPROMSettings::getNumSegments()
{
  return settings.numSegments;
}
const uint32_t EEPROMSettings::getId()
{
  return settings.microId;
}
