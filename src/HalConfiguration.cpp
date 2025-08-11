#include "HalConfiguration.h"
#include <Arduino.h>
#include <cstdint>

// LED
const uint8_t _pinLed = LED_BUILTIN;

// Rotary encoder
const uint8_t _pinRotaryEncButton = 5; // PB11
const uint8_t _pinRotaryEncClk = 4;    // PB10
const uint8_t _pinRotaryEncData = 1;   // PA23

// Rotary encoder port and bit masks for faster IO read/writes
const uint8_t _maskRotaryEncButton = digitalPinToBitMask(_pinRotaryEncButton);
volatile uint32_t *_portRotaryEncButton = portInputRegister(digitalPinToPort(_pinRotaryEncButton));
const uint8_t _maskRotaryEncClk = digitalPinToBitMask(_pinRotaryEncClk);
volatile uint32_t *_portRotaryEncClk = portInputRegister(digitalPinToPort(_pinRotaryEncClk));
const uint8_t _maskRotaryEncData = digitalPinToBitMask(_pinRotaryEncData);
volatile uint32_t *_portRotaryEncData = portInputRegister(digitalPinToPort(_pinRotaryEncData));

// Rotary encoder flags
volatile bool _flagRotaryEncButton = false;
volatile bool _flagRotaryEncCw = false;
volatile bool _flagRotaryEncCcw = false;

// Gates [5V DIO]
const uint8_t _pinGate1 = 16; // PB02
const uint8_t _pinGate2 = 17; // PB03
const uint8_t _pinGate3 = 18; // PA04
const uint8_t _pinGate4 = 19; // PA05

// Sync [5V DIO]
const uint8_t _pinSync = 20; // PA06

// SPI
const uint32_t _spiSpeed = 24000000; // 24 MHz (Maximum SPI speed for MKRZero)

// MIDI
const uint8_t _pinMidiIn = 0; // PA22

// DAC8564
const uint8_t _pinDacNss = 7;    // PA21
const uint8_t _pinDacLdac = 11;  // PA08
const uint8_t _pinDacEnable = 6; // PA20

const uint8_t _pinDisplayCs = 21; // PA07
const uint8_t _pinDisplayClk = 3; // PA11
const uint8_t _pinDisplayDin = 2; // PA10