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
// #include <BluetoothSerial.h>
#include <BluetoothSerial.h>
#endif


// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
// #define qsubd(x, b)  ((x>b)?b:0)                              // Digital
// unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed
// value. #define qsuba(x, b)  ((x>b)?x-b:0)                            //
// Analog Unsigned subtraction macro. if result <0, then => 0
// CRGB leds[288];
// Initialize our LED
// array.
EEPROMSettings settings = EEPROMSettings();
#ifdef USE_TEENSY
CRGB leds[288];
ClientSerial<HardwareSerial> client(Serial);
#endif
#ifdef USE_ESP32
CRGB leds[144];
BluetoothSerial serial;
ClientSerial<BluetoothSerial> client(serial);
#endif

CommandParser command;
LEDController controller;

void setup() {
  delay(5000);  // Soft startup to ease the flow of electrons.
  Serial.begin(115200);

  controller = LEDController(&settings, leds);
  command = CommandParser(&controller, &settings, &client);
  #ifdef USE_ESP32
  
  
  serial.begin("Remote_Lights");
  serial.enableSSP();
  // serial.setPin("1234");
  
  CLEDController *strip1 = &FastLED
    .addLeds<APA102, 17, 16, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144)
    .setCorrection(TypicalSMD5050);
  #endif
  
  #ifdef USE_TEENSY
  command = CommandParser<HardwareSerial>(&controller, &settings, &client);
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
  // #ifdef USE_ESP32
  while (client.serial.available() > 0) {
    char recieved = client.serial.read();
    inData += recieved;
    if (recieved == '\n') {
      command.parseCommand(inData);
      break;
    }
  }
  // #endif
  // #ifdef USE_TEENSY
  // while (Serial.available() > 0) {
  //   char recieved = Serial.read();
  //   inData += recieved;
  //   if (recieved == '\n') {
  //     command.parseCommand(inData);
  //     break;
  //   }
  // }
  // #endif
  controller.renderEffects();
  FastLED.show();
#ifdef ENABLE_REMOTE_LIGHTS_DEBUG
  static CEveryNSeconds pingTime = CEveryNSeconds(10);
  if (pingTime) {
    REMOTE_LOG(PING, "micro ping...");
  }
#endif
  // FastLED.delay(20);
}  // loop()
