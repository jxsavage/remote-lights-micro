#ifndef COLORWAVES_H
#define COLORWAVES_H
#include <FastLED.h>
#include <Effect.h>
#include <enums.h>
class ColorWaves: public Effect
{
	protected:
		CEveryNSeconds newPatternTime;
		CEveryNMillis newBlendTime;
		TProgmemRGBGradientPalettePtr palettesArr;
		uint8_t paletteCount;
		uint8_t currentPaletteNumber;
		CRGBPalette16 currentPalette;
		CRGBPalette16 targetPalette;
	

	public:
		ColorWaves(CRGB* LEDs, uint16_t numLEDs, uint16_t offset);
		virtual ~ColorWaves() = default;
		void render();
};
#endif
