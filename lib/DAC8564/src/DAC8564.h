#ifndef DAC8564_H
#define DAC8564_H

#include <Arduino.h>

class DAC8564
{
public:
  // DAC Address (A0 and A1 pins)
  enum class Address : uint8_t
  {
    ADDR_0 = 0b00,
    ADDR_1 = 0b01,
    ADDR_2 = 0b10,
    ADDR_3 = 0b11,
  };

  // DAC LD
  enum class Load : uint8_t
  {
    WRITE_N = 0b00,
    WRITE_N_LOAD_N = 0b01,
    WRITE_N_LOAD_ALL = 0b10
  };

  // DAC Channels
  enum class Channel : uint8_t
  {
    A = 0b00,
    B = 0b01,
    C = 0b10,
    D = 0b11
  };

  // Broadcast modes
  enum class Broadcast : uint8_t
  {
    LOAD_ALL = 0b00,
    WRITE_ALL_LOAD_ALL = 0b10
  };

  // Operating modes (see datasheet)
  enum class OperatingMode : uint8_t
  {
    NORMAL_OPERATION = 0b0000,
    POWER_DOWN_1K = 0b1010,
    POWER_DOWN_100K = 0b1100,
    POWER_DOWN_HIZ = 0b1110
  };

  // Reference modes
  enum class ReferenceMode : uint16_t
  {
    REF_DEFAULT = 0x00,     // State depending on DAC(s) state
    REF_POWERED_UP = 0x01,  // Always powered up
    REF_POWERED_DOWN = 0x10 // Always powered down
  };

  // Constructor
  DAC8564(uint32_t spiSpeed, int8_t enable, int8_t nss);

  /**
   * @brief Initialize DAC/SPI
   */
  void begin();

  /**
   * @brief Write value to channel and update output
   * @param address DAC address (A0 and A1 pins)
   * @param load Output load type (LD0 and LD1, load to buffer, write/update output n or all)
   * @param channel Channel to write to (A, B, C, D or ALL)
   * @param value Value to write to output, between 0 and 65535
   * @return true if successful
   */
  bool write(Address address, Load load, Channel channel, uint16_t value);

  /**
   * @brief Broadcast write to all channels of all connected DAC8564 devices
   * @param mode Broadcast mode (load stored data or write+load new data)
   * @param value Value to write to output, between 0 and 65535 (default 0 since not used in LOAD_ALL mode)
   * @return true if successful
   */
  bool broadcast(Broadcast mode, uint16_t value = 0);

  /**
   * @brief Set operating mode for a channel
   * @param address DAC address (A0 and A1 pins)
   * @param load Output load type (LD0 and LD1, load to buffer, write/update output n or all)
   * @param channel Channel to set operating mode to (A, B, C, D or ALL)
   * @param mode Operating mode
   * @return true if successful
   */
  bool setOperatingMode(Address address, Load load, Channel channel, OperatingMode mode);

    /**
   * @brief Set operating mode for all connected DAC8564 devices
   * @param mode Operating mode
   * @return true if successful
   */
  bool setOperatingModeAll(OperatingMode mode);

  /**
   * @brief Set internal reference mode
   * @param mode Reference mode
   * @return true if successful
   */
  bool setReferenceMode(ReferenceMode mode);

private:
  uint32_t _spiSpeed;
  int8_t _enable;
  int8_t _nss;

  // Private low-level write (24-bit command)
  bool write(uint32_t data);
};

#endif