#pragma once

#include "Outputs.h"
#include "Settings.h"
#include <MIDI.h>
#include <Arduino.h>

class CMidiHandler
{
public:
  CMidiHandler(COutputs &outputs, CSettings &settings);

  void init();
  void update(uint8_t channel, uint8_t type, byte data1, byte data2);

private:
  COutputs &mOutputs;
  CSettings &mSettings;

  // MIDI data messages
  void midiNoteOff(byte note, byte velocity);
  void midiNoteOn(byte note, byte velocity);
  void midiControlChange(byte byte1, byte byte2);
  void midiAfterTouchChannel(byte velocity);
  void midiPitchBend(int value);

  // MIDI system messages
  void systemClock();
  void systemStart();
  void systemStop();
  void systemReset();

};