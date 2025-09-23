#include "Gui.h"

#include <unordered_map>

#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

CGui::CGui(int pinMosi, int pinSs, int pinSck)
{
  SPI_SS = pinSs;
  SPI_SCK = pinSck;
  SPI_MOSI = pinMosi;

  pinMode(SPI_SS, OUTPUT);
  pinMode(SPI_SCK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);

  digitalWrite(SPI_SS, HIGH); // Disable device

  spiTransfer(OP_DISPLAYTEST, 0);
  spiTransfer(OP_DECODEMODE, 0);
  clear();
  shutdown(true);
}

void CGui::setString(const std::string &str)
{
  static const std::unordered_map<char, byte> charMap = {
      {'0', 0b01111110},
      {'1', 0b00110000},
      {'2', 0b01101101},
      {'3', 0b01111001},
      {'4', 0b00110011},
      {'5', 0b01011011},
      {'6', 0b01011111},
      {'7', 0b01110000},
      {'8', 0b01111111},
      {'9', 0b01111011},
      {'A', 0b01110111},
      {'a', 0b01111101},
      {'B', 0b01111111},
      {'b', 0b00011111},
      {'C', 0b01001110},
      {'c', 0b00011001},
      {'D', 0b01111100},
      {'d', 0b00111101},
      {'E', 0b01001111},
      {'e', 0b01101111},
      {'F', 0b01000111},
      {'f', 0b01000111},
      {'G', 0b01011110},
      {'g', 0b01111011},
      {'H', 0b00110111},
      {'h', 0b00010111},
      {'I', 0b00110000},
      {'i', 0b00010000},
      {'J', 0b00111100},
      {'j', 0b00111000},
      {'K', 0b00101111},
      {'k', 0b01010111},
      {'L', 0b00001110},
      {'l', 0b00000110},
      {'M', 0b01010100},
      {'m', 0b00010100},
      {'N', 0b01110110},
      {'n', 0b00010101},
      {'O', 0b01111110},
      {'o', 0b00011101},
      {'P', 0b01100111},
      {'p', 0b01100111},
      {'Q', 0b01101011},
      {'q', 0b01110011},
      {'R', 0b01100110},
      {'r', 0b00000101},
      {'S', 0b01011011},
      {'s', 0b01011011},
      {'T', 0b01110000},
      {'t', 0b00001111},
      {'U', 0b00111110},
      {'u', 0b00011100},
      {'V', 0b00100111},
      {'v', 0b00100011},
      {'W', 0b00111111},
      {'w', 0b00101010},
      {'X', 0b00010011},
      {'x', 0b00010011},
      {'Y', 0b00111011},
      {'y', 0b00111011},
      {'Z', 0b01101101},
      {'z', 0b01101101},
      {'-', 0b00000001}};

  int index = 0;

  for (int i = 0; i < str.size() && index < 3; i++)
  {
    char ch = str[i];

    if (ch == '.')
      continue;

    auto it = charMap.find(ch);
    uint16_t data = 0x00;
    if (it != charMap.end())
      data = it->second;

    if (i + 1 < str.size() && str[i + 1] == '.')
    {
      data |= 0x80; // Add dot
      i++;
    }
    setLed(index, data);
    index++;
  }
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