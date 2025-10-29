#include <Arduino.h>

#include "HalConfiguration.h"
#include "MidiHandler.h"
#include "Gui.h"
#include "Menu.h"
#include "Settings.h"
#include "Outputs.h"

// TODO: Change board config in mkrzero_nu_midictrl.json. Use testing VID/PID for now (https://pid.codes/) and use default bootloader VID/PID

CSettings settings;
CGui gui(_pinDisplaySs, _pinDisplaySck, _pinDisplayMosi);
COutputs outputs(gui);
CMidiHandler midiHandler(outputs, settings, gui);
CMenu menu("Root menu", gui, settings, midiHandler);

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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(_pinRotaryEncButton, INPUT);
  pinMode(_pinRotaryEncClk, INPUT);
  pinMode(_pinRotaryEncData, INPUT);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncButton), isrRotaryEncButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncClk), isrRotaryEncorder, CHANGE);

  // Initialize
  digitalWrite(LED_BUILTIN, HIGH); // Turn on LED
  menu.build();
  outputs.init();

  // Show startup animation
  gui.startup();

  // Load saved configuration from flash (or default when no settings found)
  settings.load();
  outputs.setOutputs(settings.get());

  midiHandler.begin();

  // Clear display and LEDs
  gui.clear();
}

// Main
void loop()
{
  if (_flagRotaryEncButton && ((millis() - rotaryEncButtonLast >= 1000)))
  {
    menu.bActive = true;
    while (menu.bActive)
    {
      menu.update();
      menu.waitForInput(_flagRotaryEncCw, _flagRotaryEncCcw, _flagRotaryEncButton);
      menu.handleInput();
    }
    settings.save();
    outputs.setOutputs(settings.get());
    rotaryEncButtonLast = millis(); // Make sure that menu is not immediately reopened
  }
  midiHandler.read();
  outputs.update();
}