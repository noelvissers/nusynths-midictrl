#include <Arduino.h>

#include "midiSerial.h"
#include "midiUsb.h"
#include "config.h"
#include "menu.h"

CMidiSerial midiSerial;
CMidiUsb midiUsb;
CMenu menu("Root menu");

// --------------------------------------------------
// Interrupts
// --------------------------------------------------

bool rotaryEncClkLast = 0;
void isrRotaryEncorder()
{
  bool rotaryEncClk = (*_portRotaryEncClk & _maskRotaryEncClk) != 0;
  if (rotaryEncClk != rotaryEncClkLast && rotaryEncClk == 1)
  {
    bool rotaryEncData = (*_portRotaryEncData & _maskRotaryEncData) != 0;
    if (rotaryEncClk == rotaryEncData)
      _flagRotaryEncCcw = true;
    else
      _flagRotaryEncCw = true;
  }
  rotaryEncClkLast = rotaryEncClk;
}

unsigned long rotaryEncButtonLast = 0;
void isrRotaryEncButton()
{
  bool rotaryEncButton = (*_portRotaryEncButton & _maskRotaryEncButton) != 0;
  if (!rotaryEncButton)
  {
    _flagRotaryEncButton = true;
    rotaryEncButtonLast = millis();
  }
  else
    _flagRotaryEncButton = false;
}

// --------------------------------------------------
// Main
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  // Pin modes
  pinMode(_pinRotaryEncButton, INPUT);
  pinMode(_pinRotaryEncClk, INPUT);
  pinMode(_pinRotaryEncData, INPUT);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncButton), isrRotaryEncButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncClk), isrRotaryEncorder, CHANGE);

  // Initialize
  menu.init();

  // TODO: Load saved configuration
  // ...

  // TODO: Show startup animation
  // ...
}

void loop()
{
  if (_flagRotaryEncButton && ((millis() - rotaryEncButtonLast >= 1000)))
  {
    Serial.write("Launching menu...");
    menu.bActive = true;    
    while (menu.bActive)
    {
      menu.update();
      menu.waitForInput(_flagRotaryEncCw, _flagRotaryEncCcw, _flagRotaryEncButton);
      menu.handleInput();
    }
    Serial.write("Exiting menu...");
  }
  midiSerial.Update();
  midiUsb.Update();
}