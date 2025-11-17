#pragma once

#include <Arduino.h>
#include <string>

class CGui
{
public:
  CGui(int pinSs, int pinSck = PIN_SPI_SCK, int pinMosi = PIN_SPI_MOSI);
  ~CGui() = default;

  /**
   * @brief Displays startup animation.
   */  
  void startup();

  /**
   * @brief Display idle state.
   */
  void idle();

  /**
   * @brief Sets the string to be displayed on the 7-segment display.
   *        Supports characters 0-9, A-Z, a-z, '-' and '.'.
   * @param str String to display (max 3 characters).
   */
  void setString(const std::string &str);

  /**
   * @brief Sets the state of an output LED.
   * @param mask LED to update (bitmask).
   * @param state True to turn on, false to turn off.
   */
  void setOutputLed(uint8_t mask, bool state);

  /**
   * @brief Sets the value of a specific LED.
   * @param digit Digit position (0, 1, 2 for 7 segment display, 3 for output LEDs).
   * @param value Value to display (0-127, where each bit represents a segment).
   * @param dot Whether to light up the decimal point.
   */
  void setLed(int digit, int value, bool dot = false);

  /**
   * @brief Sets the brightness of the display.
   * @param brightness Brightness level (0-15).
   */
  void setBrightness(int brightness);

  /**
   * @brief Shuts down or powers up the display.
   * @param status True to power up, false to shut down.
   */
  void shutdown(bool status);

  /**
   * @brief Clears the display.
   */
  void clear();

private:
  uint8_t mOutputLeds = 0b00000000; // State of output LEDs

  void setScanLimit(int limit);
  void spiTransfer(byte opcode, byte data);

  int SPI_SCK;
  int SPI_MOSI;
  int SPI_SS;
};