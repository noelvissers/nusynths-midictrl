#include "MidiOverSerial.h"
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

CMidiSerial::CMidiSerial(CMidiHandler &midiHandler)
    : mMidiHandler(midiHandler) {}

void CMidiSerial::init()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void CMidiSerial::read()
{
  if (MIDI.read())
  {
    uint8_t channel = MIDI.getChannel();
    uint8_t type = MIDI.getType();
    byte data1 = MIDI.getData1();
    byte data2 = MIDI.getData2();

    mMidiHandler.update(channel, type, data1, data2);
  }
}

// TODO: Implement learn functionality
// Something like:
// bool CMidiSerial::getMidiInput(byte &note, byte &ccValue, volatile bool &cancel);