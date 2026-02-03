#include "DAC8564.h"
#include <SPI.h>

DAC8564::DAC8564(uint32_t spiSpeed, int8_t enable, int8_t nss)
    : _spiSpeed(spiSpeed), _enable(enable), _nss(nss) {}

void DAC8564::begin()
{
  pinMode(_enable, OUTPUT);
  digitalWrite(_enable, HIGH);
  delay(10);

  pinMode(_nss, OUTPUT);
  digitalWrite(_nss, HIGH);
  delay(10);

  SPI.begin();
  SPI.beginTransaction(SPISettings(_spiSpeed, MSBFIRST, SPI_MODE2));
}

bool DAC8564::write(Address address, Load load, Channel channel, uint16_t value)
{
  // cmd = first 8 bytes: A1-A0-LD1-LD0-0-S1-S0-PD0
  uint8_t cmd = (static_cast<uint8_t>(address) << 6) |
                (static_cast<uint8_t>(load) << 4) |
                (static_cast<uint8_t>(channel) << 1);

  uint32_t data = ((uint32_t)cmd << 16) |
                  ((uint32_t)(value >> 8) << 8) |
                  (uint32_t)(value & 0xFF);

  return write(data);
}

bool DAC8564::broadcast(Broadcast mode, uint16_t value)
{
  uint8_t cmd = (static_cast<uint8_t>(0b11) << 4) |
                (static_cast<uint8_t>(mode) << 1);

  uint32_t data = ((uint32_t)cmd << 16) |
                  ((uint32_t)(value >> 8) << 8) |
                  (uint32_t)(value & 0xFF);

  return write(data);
}

bool DAC8564::setOperatingMode(Address address, Load load, Channel channel, OperatingMode mode)
{
  uint8_t cmd = (static_cast<uint8_t>(address) << 6) |
                (static_cast<uint8_t>(load) << 4) |
                (static_cast<uint8_t>(channel) << 1) |
                1;

  uint32_t data = ((uint32_t)cmd << 16) |
                  (uint32_t)(uint8_t(mode) << 12) |
                  uint8_t(0x00);

  return write(data);
}

bool DAC8564::setOperatingModeAll(OperatingMode mode)
{
  uint8_t cmd = (static_cast<uint8_t>(0b11) << 4) |
                (static_cast<uint8_t>(0b10) << 1) |
                1;

  uint32_t data = ((uint32_t)cmd << 16) |
                  (uint32_t)(uint8_t(mode) << 12) |
                  uint8_t(0x00);

  return write(data);
}

bool DAC8564::setReferenceMode(ReferenceMode mode)
{
  uint8_t cmd = 1;

  uint32_t data = ((uint32_t)cmd << 16) |
                  (uint32_t)(uint8_t(mode) << 12);

  return write(data);
}

bool DAC8564::write(uint32_t data)
{
  digitalWrite(_enable, LOW);
  digitalWrite(_nss, LOW);

  SPI.transfer((data >> 16) & 0xFF);
  SPI.transfer((data >> 8) & 0xFF);
  SPI.transfer(data & 0xFF);

  digitalWrite(_nss, HIGH);
  digitalWrite(_enable, HIGH);

  return true;
}
