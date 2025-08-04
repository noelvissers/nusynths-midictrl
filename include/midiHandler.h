#pragma once

class CMidiHandler
{
public:
  void init();

  // MIDI data messages
  void midiNoteOff(byte channel, byte note, byte velocity);
  void midiNoteOn(byte channel, byte note, byte velocity);
  void midiControlChange(byte channel, byte byte1, byte byte2);
  void midiAfterTouchChannel(byte channel, byte velocity);
  void midiPitchBend(byte channel, int value);

  // MIDI system messages
  void systemClock();
  void systemStart();
  void systemStop();
  void systemReset();
private:
};