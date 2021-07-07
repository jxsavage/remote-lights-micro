#include <FastLED.h>
#include <Effect.h>
#include "Plasma.h"

Plasma::Plasma(CRGB* LEDs, uint16_t numLEDs, uint16_t offset = 0)
  : Effect(LEDs, numLEDs, PLASMA, offset) {
    renderTimer = CEveryNMillis(50);
    palletteBlendTimer = CEveryNMillis(100);
    newPalleteTimer = CEveryNSeconds(5);
    currentBlending = LINEARBLEND;
    currentPalette = OceanColors_p;
    targetPalette = LavaColors_p;
  }
#define qsuba(x, b)  ((x>b)?x-b:0)
void Plasma::render() {
  if(renderTimer) {
    // Setting phase change for a couple of waves.
    int thisPhase = beatsin8(6,-64,64);
    int thatPhase = beatsin8(7,-64,64);
    // For each of the LED's in the strand, set a brightness based on a wave as follows:
    for (int k=0; k<numLEDs; k++) {
      // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
      int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;
      // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
      int thisBright = qsuba(colorIndex, beatsin8(7,0,96));
      // Let's now add the foreground colour.
      LEDs[offset+k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);
    }
  }
  if(palletteBlendTimer) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }
  if(newPalleteTimer) {
    // You can use this as a baseline colour if you want similar hues in the next line.
    uint8_t baseC = random8();
    targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
  }
}