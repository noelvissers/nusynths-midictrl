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
  bool learn(uint8_t &value, volatile bool &cancel);

private:
  COutputs &mOutputs;
  CSettings &mSettings;

  CMidiSerial mMidiSerial;
  CMidiUsb mMidiUsb;
  
  void update(uint8_t channel, uint8_t type, byte data1, byte data2);
  bool validateLearn(uint8_t channel, uint8_t type, byte data1, byte data2, uint8_t& learnValue);

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