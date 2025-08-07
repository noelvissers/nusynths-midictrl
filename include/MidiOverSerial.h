#pragma once

#include <cstdint>

struct SMidiSerialPacket
{
  uint8_t channel;
  uint8_t type;
  uint8_t dataByte1;
  uint8_t dataByte2;
};

class CMidiSerial
{
public:
  CMidiSerial();
  ~CMidiSerial() = default;

  bool getPacket(SMidiSerialPacket& midiEventPacket);
  void flush();

private:
};