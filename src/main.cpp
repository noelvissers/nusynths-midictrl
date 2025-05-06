#include <Arduino.h>

#include "menu.h"
#include "midiSerial.h"
#include "midiUsb.h"
#include "config.h"

CMenu menu;
CMidiSerial midiSerial;
CMidiUsb midiUsb;

// --------------------------------------------------
// Interrupts
// --------------------------------------------------

void interruptHandlerRotaryEncorder()
{
}

unsigned long rotaryButtonLast = 0;
void interruptHandlerRotaryButton()
{
  _flagRotaryButton = true;
  rotaryButtonLast = millis();
}

// --------------------------------------------------
// Main
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  // TODO: Set pinModes()
  // TODO: Attach interrupts
  // TODO: Init things that need initialization
  // TODO: Load configuration
  // TODO: Show startup animation
}

void loop()
{
  if (_flagRotaryButton)
  {
    // Reset:     > 100ms  & button released
    // Show menu: > 1000ms & button still pressed
  }
  midiSerial.Update();
  midiUsb.Update();
}