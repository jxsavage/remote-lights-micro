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
uint8_t max_bright = 15;                                     // Overall brightness definition. It can be changed on the fly.
CRGB leds[288];                                   // Initialize our LED array.


// LEDSegment SegmentTwo = LEDSegment(SEGMENT_ONE_NUM_LEDS);
const int capacity = JSON_OBJECT_SIZE(3);
// DynamicJsonDocument doc(capacity);
LEDController controller = LEDController(leds, 288);
LEDSegment SegmentOne = LEDSegment(leds, SEGMENT_ONE_NUM_LEDS, ET_ColorWaves, 0);
LEDSegment SegmentTwo = LEDSegment(leds, SEGMENT_ONE_NUM_LEDS, ET_BlendWave, 144);
void setup() {
	controller.addSegment(144, ET_BlendWave, 0);
	controller.addSegment(144, ET_ColorWaves, 144);
  Serial.begin(115200);
	                                      // Initialize serial port for debugging.
  delay(5000);                                                // Soft startup to ease the flow of electrons.

  FastLED.addLeds<APA102, 7, 14, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144).setCorrection(TypicalSMD5050);
  FastLED.addLeds<APA102, 21, 20, GBR, DATA_RATE_MHZ(DATA_RATE)>(leds, 144, 144).setCorrection(TypicalSMD5050);


  FastLED.setBrightness(max_bright);

  //set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()

/*
* Command JSON Doc
*/
StaticJsonDocument<200> doc;
/*
* Command codes sent from controller
*/
enum Commands {Brightness = 0, CMD_Effect = 1, Info = 2};
/*
* Effect codes sent from controller
*/
enum Effects { Colorwaves = 0, Blendwave = 1};
int currentEffect = 0;
/*
* Method codes sent from controller
*/
enum Methods {Get = 0, Set = 1};


// Effect* colorwaveEffect = createEffect(ET_ColorWaves, leds, SEGMENT_ONE_NUM_LEDS, 0);
// Effect* blendwaveEffect = createEffect(ET_BlendWave,leds, SEGMENT_TWO_NUM_LEDS, 0);
void loop () {
	String inData;
	Serial.println("Loop\n");
	while (Serial.available() > 0) {
		char recieved = Serial.read();
		inData += recieved;
		if (recieved == '\n') {
			DeserializationError error = deserializeJson(doc, inData);
			if (error) {
				StaticJsonDocument<200> er;
				JsonObject err = er.to<JsonObject>();
				err["type"] = "error deserializeJson() failed.";
				err["message"] = error.c_str();
				serializeJson(err, Serial);
				Serial.print('\n');
			}
			int cmd = (int) doc["cmd"];
			int method = (int) doc["method"];
			/*
			* Method Switch
			*/
			switch(method) {
				/*
				* Setting Valaue value = 1
				*/
				case Set:
							/*
							* Setting Brightness: cmd = 0
							*/
						if (cmd == Brightness) {
							uint8_t brightness = doc["value"];
							FastLED.setBrightness(brightness);
						} else if (cmd == CMD_Effect) {
							/*
							* Setting Effect: cmd = 1
							*/
							uint8_t effect = doc["value"];
							
							switch(effect) {
								/*
								* Setting Colorwaves
								*/
								case Colorwaves:
								currentEffect = 0;
								break;
								/*
								* Setting Blendwave
								*/
								case Blendwave:
								currentEffect = 1;
								break;
							}//end effect switch
						}
					break;
				/*
				* Getting Value: value = 0
				*/
				case Get:
					StaticJsonDocument<200> jsonBuffer;
					JsonObject root = jsonBuffer.to<JsonObject>();
					
					switch(cmd) {
						/*
						* Getting Brightness
						*/
						case Brightness: 
							root["client"] = doc["client"];
							root["prop"] = Brightness;
							root["value"] = FastLED.getBrightness();
							serializeJson(root, Serial);
							Serial.print('\n');
							Serial.flush();
							break;
						/*
						* Getting Effect
						*/
						case CMD_Effect:
							root["client"] = doc["client"];
							root["prop"] = CMD_Effect;
							root["value"] = currentEffect;
							serializeJson(root, Serial);
							Serial.print('\n');
							Serial.flush();
							break;
						/*
						* Getting Info
						*/
						case Info:
							//TODO: figure out why this was returning true when set to Info
							root["prop"] = 2;
							root["name"] = NAME;
							root["numLEDs"] = SEGMENT_ONE_NUM_LEDS;
							root["brightness"] = FastLED.getBrightness();
							root["effect"] = currentEffect;
							serializeJson(root, Serial);
							Serial.print('\n');
							Serial.flush();
							break;
					}//end get command switch
					break;
				default:
					Serial.println('default cmd');
					break;
			}//end method switch
		}
	}

	switch(currentEffect) {
		/*
		* Colorwaves Effect: value = 0
		*/
		case Colorwaves:
			// colorwaveEffect->render();
			// blendwaveEffect->render();
			// SegmentOne.renderEffect();
			// SegmentTwo.renderEffect();
			controller.renderEffects();
			break;
		/*
		* Blendwave Effect: value = 1
		*/
		case Blendwave:
		  controller.renderEffects();
			// blendwaveEffect->render();
			// colorwaveEffect->render();
			// SegmentOne.renderEffect();
			// SegmentTwo.renderEffect();
			break;
	}// end effect loop switch;
	FastLED.show();
	//FastLED.delay(20);
} // loop()
void setMaxBrightness(uint8_t brightness) {
	FastLED.setBrightness(brightness);
}