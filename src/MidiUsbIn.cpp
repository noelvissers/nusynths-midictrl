#include <Arduino.h>
#include <MIDIUSB.h>

#include "MidiUsbIn.h"
#include "MidiHandler.h"

CMidiHandler midiHandlerUsb;

void CMidiUsb::init()
{
}

void CMidiUsb::update()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();

  } while (rx.header != 0);
}

// TODO: Implement learn functionality