#pragma once

#include <Arduino.h>

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

  const byte DIGIT_0 = 0b01111110;
  const byte DIGIT_1 = 0b00110000;
  const byte DIGIT_2 = 0b01101101;
  const byte DIGIT_3 = 0b01111001;
  const byte DIGIT_4 = 0b00110011;
  const byte DIGIT_5 = 0b01011011;
  const byte DIGIT_6 = 0b01011111;
  const byte DIGIT_7 = 0b01110000;
  const byte DIGIT_8 = 0b01111111;
  const byte DIGIT_9 = 0b01111011;

  const byte CHAR_A = 0b01110111;
  const byte CHAR_a = 0b01111101;
  const byte CHAR_B = 0b01111111;
  const byte CHAR_b = 0b00011111;
  const byte CHAR_C = 0b01001110;
  const byte CHAR_c = 0b00011001;
  const byte CHAR_D = 0b01111100;
  const byte CHAR_d = 0b00111101;
  const byte CHAR_E = 0b01001111;
  const byte CHAR_e = 0b01101111;
  const byte CHAR_F = 0b01000111;
  const byte CHAR_f = 0b01000111;
  const byte CHAR_G = 0b01011110;
  const byte CHAR_g = 0b01111011;
  const byte CHAR_H = 0b00110111;
  const byte CHAR_h = 0b00010111;
  const byte CHAR_I = 0b00110000;
  const byte CHAR_i = 0b00010000;
  const byte CHAR_J = 0b00111100;
  const byte CHAR_j = 0b00111000;
  const byte CHAR_K = 0b00101111;
  const byte CHAR_k = 0b01010111;
  const byte CHAR_L = 0b00001110;
  const byte CHAR_l = 0b00000110;
  const byte CHAR_M = 0b01010100;
  const byte CHAR_m = 0b00010100;
  const byte CHAR_N = 0b01110110;
  const byte CHAR_n = 0b00010101;
  const byte CHAR_O = 0b01111110;
  const byte CHAR_o = 0b00011101;
  const byte CHAR_P = 0b01100111;
  const byte CHAR_p = 0b01100111;
  const byte CHAR_Q = 0b01101011;
  const byte CHAR_q = 0b01110011;
  const byte CHAR_R = 0b01100110;
  const byte CHAR_r = 0b00000101;
  const byte CHAR_S = 0b01011011;
  const byte CHAR_s = 0b01011011;
  const byte CHAR_T = 0b01110000;
  const byte CHAR_t = 0b00001111;
  const byte CHAR_U = 0b00111110;
  const byte CHAR_u = 0b00011100;
  const byte CHAR_V = 0b00100111;
  const byte CHAR_v = 0b00100011;
  const byte CHAR_W = 0b00111111;
  const byte CHAR_w = 0b00101010;
  const byte CHAR_X = 0b00010011;
  const byte CHAR_x = 0b00010011;
  const byte CHAR_Y = 0b00111011;
  const byte CHAR_y = 0b00111011;
  const byte CHAR_Z = 0b01101101;
  const byte CHAR_z = 0b01101101;
  const byte CHAR_dash = 0b00000001;
}

class CGui
{
public:
  CGui(int pinSck, int pinMosi, int pinSs);
  CGui(int pinSs);
  ~CGui() = default;

  void setLed(int digit, int value, bool dot = false);
  void setBrightness(int brightness);
  void shutdown(bool status);
  void clear();

private:
  void init();
  byte spiData[16];
  void spiTransfer(byte opcode, byte data);
  byte status[64];

  int SPI_SCK;
  int SPI_MOSI;
  int SPI_SS;
};