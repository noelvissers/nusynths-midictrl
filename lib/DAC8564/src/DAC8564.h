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
  enum OperatingMode : uint8_t
  {
    NORMAL_OPERATION = 0b000,
    POWER_DOWN_1K = 0b001,
    POWER_DOWN_100K = 0b010,
    POWER_DOWN_HIZ = 0b011
  };

  // Reference modes
  enum ReferenceMode : uint8_t
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

  // Initialize SPI and pins
  void begin();

  // Enable DAC
  void enable();

  // Write value to channel (0-65535) and update output
  bool write(Channel channel, uint16_t value);

  // Write value to channel input register only (no update)
  bool writeInputRegister(Channel channel, uint16_t value);

  // Write value to all channels and update
  bool writeAll(uint16_t value);

  // Set operating mode for a channel
  bool setOperatingMode(Channel channel, OperatingMode mode);

  // Set internal reference mode
  bool setReferenceMode(ReferenceMode mode);

  // Set LDAC mask (1 = ignore LDAC, 0 = respond to LDAC)
  bool setLdacMask(uint8_t mask);

  // Software reset
  bool reset();

  // Set clear code (value loaded when CLR pin is asserted)
  bool setClearCode(uint16_t value);

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