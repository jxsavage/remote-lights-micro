#define USE_OCTOWS2811
#include <Arduino.h>
#include <OctoWS2811.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <Colorwaves.h>
#include <Blendwave.h>
#include <LEDSegment.h>
#include <EffectFactory.h>
#include <LEDController.h>
#include <CommandParser.h>

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
// #define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
// #define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#define NAME "TEENSY_001"
#define SEGMENT_ONE_NUM_LEDS 144
#define SEGMENT_TWO_NUM_LEDS 144
#define NUM_LEDS 288
#define DATA_PIN 6
#define LED_TYPE NEOPIXEL
#define COLOR_ORDER GRB
#define DATA_RATE 2
#include <EffectFactory.h>
#include <EEPROMSettings.h>
uint8_t max_bright = 15; // Overall brightness definition. It can be changed on the fly.
CRGB leds[288];					 // Initialize our LED array.
EEPROMSettings settings = EEPROMSettings();
LEDController controller = LEDController(leds, 288);
CommandParser command = CommandParser(&controller);

void setup()
{
	controller.addSegment(144, ET_BlendWave, 0);
	controller.addSegment(144, ET_ColorWaves, 144);
	Serial.begin(115200);
	// Initialize serial port for debugging.
	delay(5000); // Soft startup to ease the flow of electrons.

	FastLED.addLeds<APA102, 7, 14, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144).setCorrection(TypicalSMD5050);
	FastLED.addLeds<APA102, 21, 20, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144, 144).setCorrection(TypicalSMD5050);
	FastLED.setBrightness(max_bright);

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