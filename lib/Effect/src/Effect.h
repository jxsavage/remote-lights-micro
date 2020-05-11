#ifndef EFFECT_H
#define EFFECT_H

#include <FastLED.h>
typedef enum {INVALID_EFFECT = -1, COLORWAVES, BLENDWAVE} EffectType;
class Effect
{
	protected:
		CRGB* LEDs;
		int16_t numLEDs;
		int16_t offset;
		EffectType effectType;
	public:
		Effect(CRGB* LEDs, int16_t numLEDs, EffectType effectType, int16_t offset = 0){
			setLEDs(LEDs);
			setNumLEDs(numLEDs);
			setOffset(offset);
			this->effectType = effectType;
		}
		virtual ~Effect() = default;
		EffectType getEffect() {
			return effectType;
		}
		void setLEDs(CRGB* LEDs) {
			this->LEDs = LEDs;
		}
		void setOffset(int16_t offset) {
			this->offset = offset;
		}
		void setNumLEDs(int16_t numLEDs) {
			this->numLEDs = numLEDs;
		}
		virtual void render() = 0;

};
#endif