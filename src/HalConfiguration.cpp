#include "HalConfiguration.h"
#include <Arduino.h>
#include <cstdint>

// Rotary encoder
const uint8_t _pinRotaryEncButton = 5; // PB11
const uint8_t _pinRotaryEncClk = 1;    // PA23
const uint8_t _pinRotaryEncData = 4;   // PB10

// Rotary encoder port and bit masks for faster IO read/writes
uint32_t _maskRotaryEncButton = 0;                 // Value set in setup()
volatile uint32_t *_portRotaryEncButton = nullptr; // Value set in setup()
uint32_t _maskRotaryEncClk = 0;                    // Value set in setup()
volatile uint32_t *_portRotaryEncClk = nullptr;    // Value set in setup()
uint32_t _maskRotaryEncData = 0;                   // Value set in setup()
volatile uint32_t *_portRotaryEncData = nullptr;   // Value set in setup()

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

// MIDI
const uint8_t _pinMidiRx = 0;  // PA22 SERCOM5 (alt) PAD[0]
const uint8_t _pinMidiTx = 14; // PB22 SERCOM5 (alt) PAD[2] (NC)

// SPI
const uint32_t _spiSpeed = 24000000; // 24 MHz (Maximum SPI speed for MKRZero)

// DAC8564
const uint8_t _pinDacSync = 7;   // PA21
const uint8_t _pinDacLdac = 11;  // PA08
const uint8_t _pinDacEnable = 6; // PA20
// const uint8_t _pinDacMosi = 8; // PA16
// const uint8_t _pinDacMiso = 10; // PA19
// const uint8_t _pinDacSck = 9; // PA17

const uint8_t _pinDisplaySs = 21;  // PA07
const uint8_t _pinDisplaySck = 3;  // PA11
const uint8_t _pinDisplayMosi = 2; // PA10