#pragma once

#include "Outputs.h"
#include "Settings.h"
#include "MidiOverSerial.h"
#include "MidiOverUsb.h"
#include <MIDI.h>
#include <Arduino.h>

class CMidiHandler
{
public:
  CMidiHandler(COutputs &outputs, CSettings &settings);
  ~CMidiHandler() = default;

  void read();
  bool learn(uint8_t &note, byte &ccValue, volatile bool &cancel);

private:
  COutputs &mOutputs;
  CSettings &mSettings;

  CMidiSerial mMidiSerial;
  CMidiUsb mMidiUsb;
  
  void update(uint8_t channel, uint8_t type, byte data1, byte data2);

  // MIDI data messages
  void midiNoteOff(byte note, byte velocity);
  void midiNoteOn(byte note, byte velocity);
  void midiControlChange(byte subchannel, byte value);
  void midiAfterTouchChannel(byte velocity);
  void midiPitchBend(int value);

  // MIDI system messages
  void systemClock();
  void systemStart();
  void systemStop();
  void systemReset();
};