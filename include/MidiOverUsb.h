#pragma once

#include "MidiHandler.h"

class CMidiUsb
{
public:
  CMidiUsb(CMidiHandler &midiHandler);
  ~CMidiUsb() = default;

  void read();

private:
  CMidiHandler &mMidiHandler;
};