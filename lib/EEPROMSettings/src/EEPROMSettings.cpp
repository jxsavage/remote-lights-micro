#include <EEPROMSettings.h>
#include <logger.h>
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
#define FORCE_DEFAULT_MEM_RESET
#ifdef USE_TEENSY
#include <EEPROM.h>
#endif
#ifdef USE_ESP32
#include <LITTLEFS.h>
#include "FS.h"
#define FORMAT_LITTLEFS_IF_FAILED true
Settings defaultSettings = {
  microId : 0,
  totalLEDs : 120,
  defaultBrightness : 20,
  numStrips : 1,
  strips : {
    {
      offset : 0,
      numLEDs : 60,
    },
    {
      offset : 60,
      numLEDs : 60,
    },
  },
  numSegments : 2,
  segments : {
    {
      id: 1,
      offset : 0,
      numLEDs : 60,
      effect : BLENDWAVE
    },
    {
      id: 2,
      offset : 60,
      numLEDs : 60,
      effect : CONFETTI
    },
  }
};
// Settings defaultSettings = {
//   microId : 0,
//   totalLEDs : 432,
//   defaultBrightness : 20,
//   numStrips : 3,
//   strips : {
//     {
//       offset : 0,
//       numLEDs : 144,
//     },
//     {
//       offset : 144,
//       numLEDs : 144,
//     },
//     {
//       offset : 288,
//       numLEDs : 144,
//     }
//   },
//   numSegments : 3,
//   segments : {
//     {
//       id: 1,
//       offset : 0,
//       numLEDs : 144,
//       effect : BLENDWAVE
//     },
//     {
//       id: 2,
//       offset : 144,
//       numLEDs : 144,
//       effect : COLORWAVES
//     },
//     {
//       id: 3,
//       offset : 288,
//       numLEDs : 144,
//       effect : BLENDWAVE
//     }
//   }
// };
EEPROMSettings::EEPROMSettings()
{
  #ifdef FORCE_DEFAULT_MEM_RESET
  #pragma message("FORCE_DEFAULT_MEM_RESET IS SET, RESETTING TO DEFAULT SETTINGS")
  writeDefault();
  #endif
  settingsLoaded = areSettingsLoaded();
  if (!settingsLoaded)
  {
    writeDefault();
    settingsLoaded = true;
  };
  loadSettings();
  // settings = defaultSettings;
}
void EEPROMSettings::restoreDefault() {
  writeDefault();
  loadSettings();
}
void EEPROMSettings::loadSettings() {
  File settingsFile = LITTLEFS.open("/settings", FILE_READ);
  if(!settingsFile) {
    Serial.println("File not available.");
  } else if (settingsFile.available() <= 0) {
    Serial.println("File exists but is empty.");
  } else {
    settingsFile.read((byte *)&settings, sizeof(settings));
  }
  settingsFile.close();
}
void EEPROMSettings::writeDefault()
{
  File settingsFile = LITTLEFS.open("/settings", FILE_WRITE);
  settingsFile.write((byte *)&defaultSettings, sizeof(defaultSettings));
  settingsFile.close();
}
void EEPROMSettings::clearEEPROM()
{
  writeDefault();
}
boolean EEPROMSettings::areSettingsLoaded()
{
  File settingsFile = LITTLEFS.open("/settings", FILE_READ);
  boolean areLoaded = false;
  if (settingsFile) {
    areLoaded = true;
  }
  settingsFile.close();
  return areLoaded;
}
Settings EEPROMSettings::getSettings()
{
  return settings;
}

uint16_t EEPROMSettings::getTotalLEDs()
{
  return settings.totalLEDs;
}
uint8_t EEPROMSettings::getDefaultBrightness()
{
  return settings.defaultBrightness;
}
FastLEDStripSettings EEPROMSettings::getStrip(uint8_t stripIndex)
{
  return settings.strips[stripIndex];
}
RemoteLightsSegmentSettings EEPROMSettings::getSegment(uint8_t segmentIndex)
{
  return settings.segments[segmentIndex];
}
uint8_t EEPROMSettings::getNumStrips()
{
  return settings.numStrips;
}
uint8_t EEPROMSettings::getNumSegments()
{
  return settings.numSegments;
}
int EEPROMSettings::getId()
{
  return settings.microId;
}
void EEPROMSettings::setId(int id) {
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
  File settingsFile = LITTLEFS.open("/settings", FILE_WRITE);
  settingsFile.write((byte *)&settings, sizeof(settings));
  settingsFile.close();
}
#endif
#ifdef USE_TEENSY
Settings defaultSettings = {
  microId : 0,
  totalLEDs : 288,
  defaultBrightness : 15,
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
      id: 5,
      offset : 0,
      numLEDs : 144,
      effect : COLORWAVES
    },
    {
      id: 6,
      offset : 144,
      numLEDs : 144,
      effect : COLORWAVES
    },
  }
};
EEPROMSettings::EEPROMSettings()
{
  #ifdef FORCE_DEFAULT_MEM_RESET
  #pragma message("FORCE_DEFAULT_MEM_RESET IS SET, RESETTING TO DEFAULT SETTINGS")
  restoreDefault();
  #endif
  settingsLoaded = areSettingsLoaded();
  if (!settingsLoaded)
  {
    writeDefault();
    settingsLoaded = true;
  };
  loadSettings();
}
void EEPROMSettings::restoreDefault() {
  writeDefault();
  loadSettings();
}
void EEPROMSettings::loadSettings() {
  EEPROM.get(sizeof(settingsLoaded), settings);
}
void EEPROMSettings::writeDefault()
{
  EEPROM.put(0, true);
  EEPROM.put(sizeof(settingsLoaded), defaultSettings);
}
void EEPROMSettings::clearEEPROM()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}
boolean EEPROMSettings::areSettingsLoaded()
{
  return EEPROM.get(0, settingsLoaded) == true;
}
Settings EEPROMSettings::getSettings()
{
  return settings;
}

uint16_t EEPROMSettings::getTotalLEDs()
{
  return settings.totalLEDs;
}
uint8_t EEPROMSettings::getDefaultBrightness()
{
  return settings.defaultBrightness;
}
FastLEDStripSettings EEPROMSettings::getStrip(uint8_t stripIndex)
{
  return settings.strips[stripIndex];
}
RemoteLightsSegmentSettings EEPROMSettings::getSegment(uint8_t segmentIndex)
{
  return settings.segments[segmentIndex];
}
uint8_t EEPROMSettings::getNumStrips()
{
  return settings.numStrips;
}
uint8_t EEPROMSettings::getNumSegments()
{
  return settings.numSegments;
}
int EEPROMSettings::getId()
{
  // Serial.println(defaultSettings.microId); // 0
  // Serial.println(settings.microId); // 536814352 or 536814296?????
  return settings.microId;
}
void EEPROMSettings::setId(int id) {
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
  EEPROM.put(sizeof(settingsLoaded), settings);
}
#endif