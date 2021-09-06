 // Copyright [2020] <Jake Savage>
// Figure out how to get rid of this stub... think it has something to do with
// using std::unordered map?
extern "C" {
int _write(int file, char *ptr, int len) {
  //  int todo;

  /* Implement your write code here, this is used by puts and printf for example
   */
  return len;
}
}
#define FASTLED_USE_GLOBAL_BRIGHTNESS 1
// #define ENABLE_REMOTE_LIGHTS_DEBUG
#define REMOTE_LIGHTS_LOGGER_LEVEL 1
// #define USE_OCTOWS2811
#define DATA_RATE 2
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
#endif
// Change between USE_ESP32 and USE_TEENSY

#ifdef USE_TEENSY
#include <OctoWS2811.h>
#endif
#include <enums.h>
#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <ClientSerial.h>
#include <LEDController.h>
#include <CommandParser.h>
#include <EEPROMSettings.h>

#ifdef USE_ESP32
// https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/BLESecurity.cpp#L67
// https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLETests/Arduino/security/StaticPIN/StaticPIN.ino
#define FORMAT_LITTLEFS_IF_FAILED true
#include <BluetoothSerial.h>
#include <LITTLEFS.h>
#include "FS.h"
BluetoothSerial clientSerial;
#endif


// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
// #define qsubd(x, b)  ((x>b)?b:0)                              // Digital
// unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed
// value. #define qsuba(x, b)  ((x>b)?x-b:0)                            //
// Analog Unsigned subtraction macro. if result <0, then => 0
// CRGB leds[288];
// Initialize our LED
// array.

#ifdef USE_TEENSY
CRGB leds[288];
#endif
#ifdef USE_ESP32
// CRGB leds[432];
CRGB leds[120];
#endif

CommandParser command;
LEDController controller;
EEPROMSettings settings;
void setup() {
  #ifdef USE_ESP32
  if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LITTLEFS Mount Failed");
  } else {
    Serial.println("LITTLEFS Mounted");
  }
  #endif
  delay(5000);  // Soft startup to ease the flow of electrons.
  Serial.begin(115200);
  settings = EEPROMSettings();
  controller = LEDController(&settings, leds);
  command = CommandParser(&controller, &settings);
  #ifdef USE_ESP32
  clientSerial.begin("Remote_Lights");
  clientSerial.enableSSP();
  clientSerial.setPin("1234");
  // CLEDController *strip1 = &FastLED
  //   .addLeds<WS2813, 19, BGR>(leds, 60)
  //   .setCorrection(TypicalSMD5050);
  // CLEDController *strip2 = &FastLED
  //   .addLeds<WS2813, 18, BGR>(leds, 60, 60)
  //   .setCorrection(TypicalSMD5050);
  // CLEDController *strip1 = &FastLED
  //   .addLeds<APA102, 16, 17, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144)
  //   .setCorrection(TypicalSMD5050);
  // CLEDController *strip2 = &FastLED
  //   .addLeds<APA102, 18, 19, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144, 144)
  //   .setCorrection(TypicalSMD5050);
  // CLEDController *strip3 = &FastLED
  //   .addLeds<APA102, 26, 25, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 288, 144)
  //   .setCorrection(TypicalSMD5050);
  #endif
  
  #ifdef USE_TEENSY
  CLEDController *strip1 = &FastLED
    .addLeds<APA102, 7, 14, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144)
    .setCorrection(TypicalSMD5050);
  CLEDController *strip2 = &FastLED
    .addLeds<APA102, 21, 20, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144, 144)
    .setCorrection(TypicalSMD5050);
  #endif

  // FastLED.setBrightness(max_bright);
  // set_max_power_in_volts_and_milliamps(5, 500);               // FastLED
  // Power management set at 5V, 500mA.
}  // setup()

void loop() {
  String inData;
  while (clientSerial.available() > 0) {
    char recieved = clientSerial.read();
    inData += recieved;
    if (recieved == '\n') {
      command.parseCommand(inData);
      break;
    }
  }
  controller.renderEffects();
  FastLED.show();
}  // loop()
