#include <Arduino.h>

#include "menu.h"
#include "midiSerial.h"
#include "midiUsb.h"

CMenu menu;
CMidiSerial midiSerial;
CMidiUsb midiUsb;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  menu.Example();

  midiSerial.Update();
  midiUsb.Update();
}