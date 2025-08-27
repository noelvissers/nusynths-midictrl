#ifndef DAC8564_H
#define DAC8564_H

#include <Arduino.h>
#include <SPI.h>

class DAC8564
{
public:
  // DAC Channels
  enum class Channel : uint8_t
  {
    A = 0b0000,
    B = 0b0010,
    C = 0b0100,
    D = 0b0110,
    ALL = 0b0111
  };

  // Operating modes (see datasheet)
  enum class OperatingMode : uint8_t
  {
    NORMAL_OPERATION = 0b000,
    POWER_DOWN_1K = 0b001,
    POWER_DOWN_100K = 0b010,
    POWER_DOWN_HIZ = 0b011
  };

  // Reference modes
  enum class ReferenceMode : uint8_t
  {
    REF_DEFAULT = 0x00,     // State depending on DAC(s) state
    REF_POWERED_UP = 0x10,  // Always powered up
    REF_POWERED_DOWN = 0x20 // Always powered down
  };

  // DAC8564 Command Codes (C3..C0 bits)
  static constexpr uint8_t CMD_WRITE_UPDATE_N = 0b0001 << 4;   // Write to and update channel n
  static constexpr uint8_t CMD_WRITE_N = 0b0010 << 4;          // Write to input register n
  static constexpr uint8_t CMD_WRITE_UPDATE_ALL = 0b0011 << 4; // Write to and update all channels
  static constexpr uint8_t CMD_POWER_CONTROL = 0b0100 << 4;    // Power control
  static constexpr uint8_t CMD_LDAC_MASK = 0b0101 << 4;        // LDAC mask
  static constexpr uint8_t CMD_SOFTWARE_RESET = 0b0110 << 4;   // Software reset
  static constexpr uint8_t CMD_INTERNAL_REF = 0b1000 << 4;     // Internal reference control
  static constexpr uint8_t CMD_SET_CLEAR_CODE = 0b1001 << 4;   // Set clear code

  // Constructor
  DAC8564(SPIClass &spi, uint32_t spiSpeed, uint8_t nss, uint8_t ldac, uint8_t enable);

  /**
   * @brief Initialize SPI and pins
   */
  void begin();

  /**
   * @brief  Enable DAC
   */
  void enable();

  /**
   * @brief Write value to channel and update output
   * @param channel Channel to write to (A, B, C, D or ALL)
   * @param value Value to write to output, between 0 and 65535
   * @return true if successful
   */
  bool write(Channel channel, uint16_t value);

  /**
   * @brief Write value to channel input register only (no update)
   * @param channel Channel to write to (A, B, C, D or ALL)
   * @param value Value to write to output, between 0 and 65535
   * @return true if successful
   */
  bool writeInputRegister(Channel channel, uint16_t value);

  /**
   * @brief Write value to all channels and update
   * @param value Value to write to all outputs, between 0 and 65535
   * @return true if successful
   */
  bool writeAll(uint16_t value);

  /**
   * @brief Set operating mode for a channel
   * @param channel Channel to set operating mode to (A, B, C, D or ALL)
   * @param mode Operating mode
   * @return true if successful
   */
  bool setOperatingMode(Channel channel, OperatingMode mode);

  /**
   * @brief Set internal reference mode
   * @param mode Reference mode
   * @return true if successful
   */
  bool setReferenceMode(ReferenceMode mode);

  /**
   * @brief Software reset
   * @return true if successful
   */
  bool reset();

private:
  SPIClass &_spi;
  uint32_t _spiSpeed;
  uint8_t _nss;
  uint8_t _ldac;
  uint8_t _enable;
  bool _active;

  // Helper to build payload for normal write
  void getPayload(Channel channel, uint16_t value, uint8_t *buffer);

  // Private low-level write (24-bit command)
  bool write(uint32_t data);
};

#endif