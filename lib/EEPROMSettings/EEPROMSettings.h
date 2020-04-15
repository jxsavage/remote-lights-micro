#ifndef EEPROM_SETTINGS
#define EEPROM_SETTINGS
#include <logger.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <Effect.h>
typedef uint8_t stripPins[2];
struct FastLEDStripSettings {
  ESPIChipsets chipset;
  stripPins pins;
  EOrder order;
  uint16_t offset;
  uint16_t numLEDs;
  EffectType effect;
};
FastLEDStripSettings defaultStrip1 = {
  .chipset = APA102,
  .pins = {[0] = 7, [1] = 14},
  .order = GRB,
  .offset = 0,
  .numLEDs = 144,
  .effect = ET_BlendWave,
};
FastLEDStripSettings defaultStrip2 = {
  .chipset = APA102,
  .pins = {[0] = 21, [1] = 20},
  .order = GRB,
  .offset = 144,
  .numLEDs = 144,
  .effect = ET_ColorWaves,
};
struct Settings {
  char microId[12];
  uint8_t dataRate;
  uint16_t totalLEDs;
  FastLEDStripSettings strips[2];
};
Settings defaultSettings = {
  .microId = "unitialized",
  .dataRate = 2,
  .totalLEDs = 288,
  .strips = {
    defaultStrip1,
    defaultStrip2
  }   
};
class EEPROMSettings {
  public:
    EEPROMSettings() {
      settingsLoaded = areSettingsLoaded();
      if (!settingsLoaded) {
        writeDefault();
        settingsLoaded = true;
      };
      EEPROM.get(1, settings);
      settingsLoaded = true;
    }
    void writeDefault() {
      EEPROM.put(1, defaultSettings);
      EEPROM.put(0, 255);
    }
    void clearEEPROM() {
      for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
      }
    }
    const boolean areSettingsLoaded() {
      return EEPROM.read(0) == 255;
    }
    Settings const getSettings() {
      return settings;
    }
  private:
    Settings settings;
    boolean settingsLoaded;

};
#endif