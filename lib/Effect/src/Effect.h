#ifndef EFFECT_H
#define EFFECT_H
#include <FastLED.h>

class Effect
{
	protected:
		CRGB* LEDs;
		uint16_t numLEDs;
		uint16_t offset;
	public:
		Effect(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0){
			setLEDs(LEDs);
			setNumLEDs(numLEDs);
			setOffset(offset);
		}
		virtual ~Effect() = default;
		void setLEDs(CRGB* LEDs) {
			this->LEDs = LEDs;
		}
		void setOffset(uint16_t offset) {
			this->offset = offset;
		}
		void setNumLEDs(uint16_t numLEDs) {
			this->numLEDs = numLEDs;
		}
		virtual void render() = 0;

};

#endif