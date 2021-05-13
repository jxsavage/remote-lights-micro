#include <EEPROMSettings.h>
#include <logger.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>

#ifdef USE_TEENSY
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
      id: 1,
      offset : 0,
      numLEDs : 144,
      effect : BLENDWAVE
    },
    {
      id: 3,
      offset : 144,
      numLEDs : 144,
      effect : COLORWAVES
    },
  }
};
#endif
#ifdef USE_ESP32
Settings defaultSettings = {
  microId : 0,
  totalLEDs : 144,
  defaultBrightness : 20,
  numStrips : 1,
  strips : {
    {
      offset : 0,
      numLEDs : 144,
    }
  },
  numSegments : 1,
  segments : {
    {
      id: 1,
      offset : 0,
      numLEDs : 144,
      effect : BLENDWAVE
    }
  }
};
#endif

EEPROMSettings::EEPROMSettings()
{
  // settingsLoaded = areSettingsLoaded();
  // if (!settingsLoaded)
  // {
  //   writeDefault();
  //   settingsLoaded = true;
  // };
  // EEPROM.get(1, settings);
  settings = defaultSettings;
}
void EEPROMSettings::writeDefault()
{
  int settingsLoaded = 255;
  uint16_t eepromSize = sizeof defaultSettings + sizeof settingsLoaded;
  EEPROM.begin(eepromSize);
  EEPROM.put(1, defaultSettings);
  EEPROM.put(0, 255);
  EEPROM.end();
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
void EEPROMSettings::setId(uint32_t id) {
  settings.microId = id;
  writeEEPROM();
}
void EEPROMSettings::setSegmentId(segmentId oldId, segmentId newId) {
  for(int i = 0; i < settings.numSegments; i++) {
    if(settings.segments[i].id == oldId) {
      settings.segments[i].id = newId;
      writeEEPROM();
      break;
    }
  }
}
void EEPROMSettings::writeEEPROM() {
  EEPROM.put(1, settings);
  EEPROM.commit();
}
