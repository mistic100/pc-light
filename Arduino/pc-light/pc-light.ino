#include <FastLED.h>
#include <CmdParser.hpp>
#include <EEPROM.h>

#include "constants.h"
#include "modes.h"
#include "animations.h"

// animation manager
Animations *animations; 

// global brightness
uint8_t brightness = 255;

// turn off everything
boolean isStop = false;

CmdParser cmdParser;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  // animations configuration
  animations = new Animations();

  uint8_t mode = EEPROM.read(MODE_ADDR);
  brightness = EEPROM.read(BRIGHT_ADDR);
  uint8_t color = EEPROM.read(COLOR_ADDR);
  
  animations->staticColor = color;
  animations->setCurrentMode(Modes(mode % NUM_MODES));

  FastLED.addLeds<LED_TYPE, PIN_LED, COLOR_ORDER>(animations->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
  while (Serial.available() > 0) {
    CmdBuffer<32> myBuffer;

    if (myBuffer.readFromSerial(&Serial, 2000) && cmdParser.parseCmd(&myBuffer) != CMDPARSER_ERROR) {
      if (cmdParser.equalCommand_P(PSTR("ON"))) {
        isStop = false;
      }
      else if (cmdParser.equalCommand_P(PSTR("OFF"))) {
        isStop = true;
      }
      else if (cmdParser.equalCommand_P(PSTR("BRIGHT"))) {
        brightness = atoi(cmdParser.getCmdParam(1));
        save();
      }
      else if (cmdParser.equalCommand_P(PSTR("MODE"))) {
        uint8_t mode = atoi(cmdParser.getCmdParam(1));
        animations->setCurrentMode(Modes(mode % NUM_MODES));
        save();
      }
      else if (cmdParser.equalCommand_P(PSTR("COLOR"))) {
        uint8_t color = atoi(cmdParser.getCmdParam(1));
        animations->staticColor = color;
        animations->setCurrentMode(STATIC);
        save();
      }
    }
  }
  
  EVERY_N_MILLIS_I(timer, 10) {
    timer.setPeriod(animations->period);
    animations->run(isStop);
    FastLED.setBrightness(brightness);
    FastLED.show();
  }
}

void save() {
  EEPROM.write(MODE_ADDR, animations->currentMode);
  EEPROM.write(BRIGHT_ADDR, brightness);
  EEPROM.write(COLOR_ADDR, animations->staticColor);
}
