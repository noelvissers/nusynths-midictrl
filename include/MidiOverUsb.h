#pragma once

#include <cstdint>

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

  /**
   * @brief Gets a USB MIDI packet.
   * @param midiEventPacket pointer to event packet struct.
   * @return Returns true if successful.
   */
  bool getPacket(SMidiUsbPacket &midiEventPacket);

  /**
   * @brief Flushes all MIDI packets.
   */
  void flush();

private:
};