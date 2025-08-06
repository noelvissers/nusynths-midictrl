#pragma once

#include "MidiHandler.h"
#include <MIDIUSB.h>

struct SMidiUsbPacket
{
  uint8_t channel;
  uint8_t type;
  uint8_t dataByte1;
  uint8_t dataByte2;
};

class CMidiUsb
{
public:
  CMidiUsb();
  ~CMidiUsb() = default;

  bool getPacket(SMidiUsbPacket& midiEventPacket);

private:
};