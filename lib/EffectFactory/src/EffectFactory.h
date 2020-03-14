#ifndef EFFECT_FACTORY_H
#define EFFECT_FACTORY_H
#include <FastLED.h>
#include <Effect.h>
#include <Colorwaves.h>
#include <Blendwave.h>

inline Effect* createEffect(
			EffectType effect, CRGB* LEDs, uint16_t numLEDs, uint16_t offset){
			if(effect == ET_ColorWaves){
				return new ColorWaves(LEDs, numLEDs, offset);
			} else if (effect == ET_BlendWave) {
        return new BlendWave(LEDs, numLEDs, offset);
      } else {
        return 0;
      }
		}
#endif