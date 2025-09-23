#include "Gui.h"

#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

CGui::CGui(int pinSs, int pinSck, int pinMosi)
{
  SPI_SS = pinSs;
  SPI_SCK = pinSck;
  SPI_MOSI = pinMosi;

  pinMode(SPI_SCK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_SS, OUTPUT);

  digitalWrite(SPI_SS, HIGH); // Disable device

  spiTransfer(OP_DISPLAYTEST, 0);
  spiTransfer(OP_DECODEMODE, 0);
  clear();
  shutdown(true);
}

void CGui::setLed(int digit, int value, bool dot)
{
  byte v = value;

  if (digit < 0 || digit > 7)
    return;
  if (dot)
    v |= 0x80;

  spiTransfer(digit + 1, v);
}

void CGui::setBrightness(int brightness)
{
  if (brightness >= 0 && brightness < 16)
    spiTransfer(OP_INTENSITY, brightness);
}

void CGui::shutdown(bool status)
{
  spiTransfer(OP_SHUTDOWN, !status);
}

void CGui::clear()
{
  for (int i = 0; i < 8; i++)
    spiTransfer(i + 1, 0);
}

void CGui::spiTransfer(volatile byte opcode, volatile byte data)
{
  digitalWrite(SPI_SS, LOW);
  shiftOut(SPI_MOSI, SPI_SCK, MSBFIRST, opcode);
  shiftOut(SPI_MOSI, SPI_SCK, MSBFIRST, data);
  digitalWrite(SPI_SS, HIGH);
}