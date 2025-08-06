#include "MidiOverUsb.h"
#include <MIDI.h> // Included for enums
#include <MIDIUSB.h>

CMidiUsb::CMidiUsb(CMidiHandler &midiHandler)
    : mMidiHandler(midiHandler) {}

void CMidiUsb::read()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
    if (rx.header != 0)
    {
      // TODO: verify that this is correct
      uint8_t channel = rx.byte1 & 0x0F;
      uint8_t type = rx.byte1;
      byte data1 = rx.byte2;
      byte data2 = rx.byte3;

      mMidiHandler.update(channel, type, data1, data2);
    }
  } while (rx.header != 0);
}

// TODO: Implement learn functionality
// Something like:
// bool CMidiUsb::getMidiInput(byte &note, byte &ccValue, volatile bool &cancel);