#include "MidiOverSerial.h"
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

CMidiSerial::CMidiSerial()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

bool CMidiSerial::getPacket(SMidiSerialPacket &midiEventPacket)
{
  if (MIDI.read())
  {
    midiEventPacket.channel = MIDI.getChannel();
    midiEventPacket.type = MIDI.getType();
    midiEventPacket.dataByte1 = MIDI.getData1();
    midiEventPacket.dataByte2 = MIDI.getData2();
    return true;
  }
  return false;
}