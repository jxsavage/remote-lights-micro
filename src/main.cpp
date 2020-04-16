#define USE_OCTOWS2811
#define DATA_RATE 2
#include <Arduino.h>
#include <OctoWS2811.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <LEDController.h>
#include <CommandParser.h>
#include <EEPROMSettings.h>
// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
// #define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
// #define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0
//CRGB leds[288];					 // Initialize our LED array.
EEPROMSettings settings = EEPROMSettings();
CRGB leds[288];
LEDController controller;
CommandParser command;

void setup()
{
	Serial.begin(115200);
	randomSeed(analogRead(0));
	delay(5000); // Soft startup to ease the flow of electrons.
	
	controller = LEDController(&settings, leds);
	command = CommandParser(&controller, &settings);
	FastLED.addLeds<APA102, 7, 14, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144).setCorrection(TypicalSMD5050);
	FastLED.addLeds<APA102, 21, 20, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144, 144).setCorrection(TypicalSMD5050);
	// FastLED.setBrightness(max_bright);
	//set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} // setup()

void loop()
{
	String inData;
	while (Serial.available() > 0)
	{
		char recieved = Serial.read();
		inData += recieved;
		if (recieved == '\n')
		{
			command.parseCommand(inData);
		}
	}
	controller.renderEffects();
	FastLED.show();
	//FastLED.delay(20);
} // loop()