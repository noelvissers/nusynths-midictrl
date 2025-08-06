#pragma once

#include "MidiHandler.h"

class CMidiSerial
{
public:
  CMidiSerial(CMidiHandler &midiHandler);
  ~CMidiSerial() = default;

  void init();
  void read();

private:
  CMidiHandler &mMidiHandler;
};