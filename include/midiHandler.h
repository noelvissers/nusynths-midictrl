#pragma once

class CMidiHandler
{
public:
  void init();

  // MIDI data messages
  void midiNoteOff();
  void midiNoteOn();
  void midiControlChange();
  void midiChannelAfterTouch();
  void midiPitchWheel();

  // MIDI system messages
  void systemTimingClock();
  void systemStart();
  void systemStop();
  void systemReset();
private:
};