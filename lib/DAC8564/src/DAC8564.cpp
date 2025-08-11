#include "DAC8564.h"

DAC8564::DAC8564(SPIClass &spi, uint32_t spiSpeed, uint8_t nss, uint8_t ldac, uint8_t enable)
    : _spi(spi), _spiSpeed(spiSpeed), _nss(nss), _ldac(ldac), _enable(enable), _active(false) {}

void DAC8564::begin()
{
  pinMode(_nss, OUTPUT);
  pinMode(_ldac, OUTPUT);
  pinMode(_enable, OUTPUT);

  digitalWrite(_nss, HIGH);
  digitalWrite(_ldac, HIGH);
  digitalWrite(_enable, HIGH);

  _spi.begin();
}

void DAC8564::enable()
{
  digitalWrite(_enable, LOW);
  digitalWrite(_nss, LOW);
  digitalWrite(_nss, HIGH);
  digitalWrite(_enable, HIGH);

  // Rising edge to reset DAC registers
  digitalWrite(_ldac, LOW);
  delayMicroseconds(10);
  digitalWrite(_ldac, HIGH);
  delayMicroseconds(10);
  digitalWrite(_ldac, LOW);

  _active = true;
}

bool DAC8564::write(Channel channel, uint16_t value)
{
  if (!_active)
    return false;

  uint8_t command[3];
  getPayload(channel, value, command);

  uint32_t data = ((uint32_t)command[0] << 16) |
                  ((uint32_t)command[1] << 8) |
                  (uint32_t)command[2];

  return write(data);
}

bool DAC8564::writeInputRegister(Channel channel, uint16_t value)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_WRITE_N | static_cast<uint8_t>(channel);
  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)(value >> 8) << 8) |
                  (uint32_t)(value & 0xFF);

  return write(data);
}

bool DAC8564::writeAll(uint16_t value)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_WRITE_UPDATE_ALL | static_cast<uint8_t>(Channel::ALL);
  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)(value >> 8) << 8) |
                  (uint32_t)(value & 0xFF);

  return write(data);
}

bool DAC8564::setOperatingMode(Channel channel, OperatingMode mode)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_POWER_CONTROL | static_cast<uint8_t>(channel);
  uint8_t highData = 0x00;
  uint8_t lowData = (static_cast<uint8_t>(mode) & 0x07) << 4;

  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)highData << 8) |
                  (uint32_t)lowData;

  return write(data);
}

bool DAC8564::setReferenceMode(ReferenceMode mode)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_INTERNAL_REF; // 0x10 in upper nibble
  uint8_t highData = static_cast<uint8_t>(mode);
  uint8_t lowData = 0x00;

  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)highData << 8) |
                  (uint32_t)lowData;

  return write(data);
}

bool DAC8564::setLdacMask(uint8_t mask)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_LDAC_MASK;
  uint8_t highData = 0x00;
  uint8_t lowData = mask & 0x0F; // 4-bit mask

  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)highData << 8) |
                  (uint32_t)lowData;

  return write(data);
}

bool DAC8564::reset()
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_SOFTWARE_RESET;
  uint8_t highData = 0x00;
  uint8_t lowData = 0x01; // Reset command

  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)highData << 8) |
                  (uint32_t)lowData;

  return write(data);
}

bool DAC8564::setClearCode(uint16_t value)
{
  if (!_active)
    return false;

  uint8_t cmdByte = CMD_SET_CLEAR_CODE;
  uint32_t data = ((uint32_t)cmdByte << 16) |
                  ((uint32_t)(value >> 8) << 8) |
                  (uint32_t)(value & 0xFF);

  return write(data);
}

void DAC8564::getPayload(Channel channel, uint16_t value, uint8_t *buffer)
{
  buffer[0] = CMD_WRITE_UPDATE_N | static_cast<uint8_t>(channel);
  buffer[1] = (value >> 8) & 0xFF;
  buffer[2] = value & 0xFF;
}

bool DAC8564::write(uint32_t data)
{
  digitalWrite(_enable, LOW);
  digitalWrite(_nss, LOW);

  _spi.beginTransaction(SPISettings(_spiSpeed, MSBFIRST, SPI_MODE1));
  _spi.transfer((data >> 16) & 0xFF);
  _spi.transfer((data >> 8) & 0xFF);
  _spi.transfer(data & 0xFF);
  _spi.endTransaction();

  digitalWrite(_nss, HIGH);
  digitalWrite(_enable, HIGH);

  return true;
}