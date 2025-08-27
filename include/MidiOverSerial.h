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

  /**
   * @brief Gets a MIDI packet.
   * @param midiEventPacket pointer to event packet struct.
   * @return Returns true if successful.
   */
  bool getPacket(SMidiSerialPacket &midiEventPacket);

  /**
   * @brief Flushes all MIDI packets.
   */
  void flush();

private:
};