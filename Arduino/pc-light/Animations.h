#pragma once

#include <FastLED.h>

#include "constants.h"
#include "modes.h"
#include "functions.h"

class Animations {

  public:

  // loop period in ms
  int period = 10;
  // palette offset
  int startIndex = 0;
  int fadeIndex = 0;
  // current animation
  enum Modes currentMode;
  // current blending mode
  TBlendType currentBlending;
  // displayed palette
  CRGBPalette16 currentPalette;
  // target palette for blending
  CRGBPalette16 targetPalette;
  // static color
  uint8_t staticColor;
  // leds
  CRGB leds[NUM_LEDS];

  /**
   * Run the current animation
   */
  void run(boolean isStop) {
    switch (currentMode) {
      case RAINBOW:
        runRainbow();
        break;
        
      case RAINBOW_2:
        runRainbow2();
        break;

      case RANDOM:
        runRandom();
        break;

      case STATIC:
        fill_solid(leds, NUM_LEDS, CHSV(staticColor, 255, 255));
        break;
    }

    EVERY_N_MILLIS(20) {
      if (!isStop && fadeIndex < NUM_LEDS) {
        fadeIndex++;
      }
      if (isStop && fadeIndex > 0) {
        fadeIndex--;
      }
    }

    for (int i = fadeIndex; i < NUM_LEDS; i++) {
      leds[NUM_LEDS - i - 1] = CRGB::Black;
    }
  }

  /**
   * Change the current animation
   */
  void setCurrentMode(enum Modes mode) {
    currentMode = mode;
    period = 10;
    currentBlending = LINEARBLEND;
    
    switch (currentMode) {
      case RAINBOW:
      case RAINBOW_2:
        currentPalette = RainbowColors_p;
        break;
  
      case RANDOM:
        currentPalette = CRGBPalette16(
          CHSV(random8(), 255, 255), 
          CHSV(random8(), 255, 255)
        );
        targetPalette = CRGBPalette16(
          CHSV(random8(), 255, 255), 
          CHSV(random8(), 255, 255)
        );
        break;

      case STATIC:
        break;
    }
  }


  private:

  /**
   * Displays the current palette
   */
  void showPalette(uint8_t range = 255) {
    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t index = 1.0 * i / NUM_LEDS * range;
      leds[i] = ColorFromPalette(currentPalette, index + startIndex, 255, currentBlending);
    }
  }

  /**
   * Run the rainbow animation
   */
  void runRainbow() {
    startIndex = startIndex + 1;
    
    showPalette(128);
  }

  /**
   * Run the rainbow 2 animation
   */
  void runRainbow2() {
    startIndex = startIndex + 1;
    
    showPalette(1);
  }

  /**
   * Run the random animation
   */
  void runRandom() {
    static bool odd = false;
    startIndex = 0;

    if (odd) {
      nblendPaletteTowardPalette(currentPalette, targetPalette, 12);
    } else {
      nblendPaletteTowardPaletteReverse(currentPalette, targetPalette, 12);
    }

    EVERY_N_MILLIS(2000) {
      if (odd) {
        targetPalette = CRGBPalette16(
          currentPalette[0], 
           CHSV(random8(), 255, 255)
        );
      }
      else {
        targetPalette = CRGBPalette16(
          CHSV(random8(), 255, 255), 
          currentPalette[15]
        );
      }

      odd = !odd;
    }

    showPalette();
  }
  
};
