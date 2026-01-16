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
CMenu menu(gui, settings, midiHandler);

static volatile unsigned long lastTrig = 0;
static volatile bool stateLast = 0;
void isrRotaryEncorder()
{
  unsigned long now = millis();
  if (now - lastTrig < 5)
    return; // Debounce

  bool rotaryEncClk = (*_portRotaryEncClk & _maskRotaryEncClk) != 0;
  bool rotaryEncData = (*_portRotaryEncData & _maskRotaryEncData) != 0;

  if (rotaryEncClk == stateLast)
    return; // No change

  if (!rotaryEncClk)
  {
    if (rotaryEncClk == rotaryEncData)
      _flagRotaryEncCw = true;
    else
      _flagRotaryEncCcw = true;
  }
  lastTrig = now;
  stateLast = rotaryEncClk;
}

unsigned long rotaryEncButtonLast = 0;
void isrRotaryEncButton()
{
  // Read BUTTON pin
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

  // ISR Rotarty Encoder Button
  pinMode(_pinRotaryEncButton, INPUT);
  _maskRotaryEncButton = digitalPinToBitMask(_pinRotaryEncButton);
  _portRotaryEncButton = portInputRegister(digitalPinToPort(_pinRotaryEncButton));
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncButton), isrRotaryEncButton, CHANGE);

  // ISR Rotarty Encoder Clk
  pinMode(_pinRotaryEncClk, INPUT);
  _maskRotaryEncClk = digitalPinToBitMask(_pinRotaryEncClk);
  _portRotaryEncClk = portInputRegister(digitalPinToPort(_pinRotaryEncClk));
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncClk), isrRotaryEncorder, CHANGE);

  // ISR Rotarty Encoder Data
  pinMode(_pinRotaryEncData, INPUT);
  _maskRotaryEncData = digitalPinToBitMask(_pinRotaryEncData);
  _portRotaryEncData = portInputRegister(digitalPinToPort(_pinRotaryEncData));
  attachInterrupt(digitalPinToInterrupt(_pinRotaryEncData), isrRotaryEncorder, CHANGE);

  // LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Turn on LED

  outputs.init();

  // Show startup animation
  gui.startup();

  // Load saved configuration from flash (or default when no settings found)
  Serial.println("DEBUG: CURRENT SETTINGS:");
  settings.print();
  Serial.println("DEBUG: LOADING SETTINGS FROM MEMORY");
  settings.load();
  Serial.println("DEBUG: CURRENT SETTINGS:");
  settings.print();
  outputs.setOutputs(settings.get());

  midiHandler.begin();

  // Show idle state
  delay(1000);
  gui.idle();
}

// Main
void loop()
{
  if (_flagRotaryEncButton && ((millis() - rotaryEncButtonLast >= 1000)))
  {
    menu.start();
    while (menu.active())
    {
      menu.display();
      menu.update(_flagRotaryEncCw, _flagRotaryEncCcw, _flagRotaryEncButton);
    }
    Serial.println("DEBUG: SAVING SETTINGS TO MEMORY");
    settings.save();
    Serial.println("DEBUG: NEW SETTINGS:");
    settings.print();
    outputs.setOutputs(settings.get());
    rotaryEncButtonLast = millis(); // Make sure that menu is not immediately reopened
    gui.idle();
  }
  midiHandler.read();
  outputs.update();
}