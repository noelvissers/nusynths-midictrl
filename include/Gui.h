#pragma once

#include <Arduino.h>
#include <string>

namespace GUI
{
  const byte LED_CV_1 = 0b01000000;
  const byte LED_CV_2 = 0b00100000;
  const byte LED_CV_3 = 0b00010000;
  const byte LED_CV_4 = 0b00001000;
  const byte LED_GATE_1 = 0b00000100;
  const byte LED_GATE_2 = 0b00000010;
  const byte LED_GATE_3 = 0b00000001;
  const byte LED_GATE_4 = 0b10000000;
}

class CGui
{
public:
  CGui(int pinSs, int pinSck = PIN_SPI_SCK, int pinMosi = PIN_SPI_MOSI);
  ~CGui() = default;

  void setString(const std::string &str);
  void setLed(int digit, int value, bool dot = false);
  void setBrightness(int brightness);
  void shutdown(bool status);
  void clear();

private:
  void setScanLimit(int limit);
  void spiTransfer(byte opcode, byte data);

  int SPI_SCK;
  int SPI_MOSI;
  int SPI_SS;
};