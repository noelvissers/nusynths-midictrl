#include <Arduino.h>

#include "HalConfiguration.h"
#include "MidiHandler.h"
#include "Menu.h"
#include "Settings.h"
#include "Outputs.h"

/**
 * TODO: Phase out use of MIDISerial and USB here. Only use the handler here. Serial and USB should have get functions
 * to get the data inside the handler. This means that the handler can also contain the learn functionality.
 */
CSettings settings;
COutputs outputs;
CMidiHandler midiHandler(outputs, settings);
CMenu menu("Root menu");

// Interrupts
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

// Setup
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

  // Load saved configuration from flash
  //settings.loadSettings();

  // Initialize
  menu.init();
  outputs.init(); // TODO: init using saved settings?

  // TODO: Show startup animation
  // ...
}

// Main
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
  midiHandler.read();
  outputs.update();
}