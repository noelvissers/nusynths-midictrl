#include "MidiOverUsb.h"
#include <MIDIUSB.h>

CMidiUsb::CMidiUsb() {}

bool CMidiUsb::getPacket(SMidiUsbPacket &midiEventPacket)
{
  midiEventPacket_t rx = MidiUSB.read();
  if (rx.header != 0)
  {
    midiEventPacket.channel = rx.byte1 & 0x0F;
    midiEventPacket.type = rx.byte1 & 0xF0;
    midiEventPacket.dataByte1 = rx.byte2;
    midiEventPacket.dataByte2 = rx.byte3;
    return true;
  }
  return false;
}

void CMidiUsb::flush()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
  } while (rx.header != 0);
}