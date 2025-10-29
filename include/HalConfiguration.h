#pragma once

#include <cstdint>

// Rotary encoder
extern const uint8_t _pinRotaryEncButton;
extern const uint8_t _pinRotaryEncClk;
extern const uint8_t _pinRotaryEncData;

// Rotary encoder port and bit masks for faster IO read/writes
extern const uint8_t _maskRotaryEncButton;
extern volatile uint32_t *_portRotaryEncButton;
extern const uint8_t _maskRotaryEncClk;
extern volatile uint32_t *_portRotaryEncClk;
extern const uint8_t _maskRotaryEncData;
extern volatile uint32_t *_portRotaryEncData;

// Rotary encoder flags
extern volatile bool _flagRotaryEncButton;
extern volatile bool _flagRotaryEncCw;
extern volatile bool _flagRotaryEncCcw;

// Gates [5V DIO]
extern const uint8_t _pinGate1;
extern const uint8_t _pinGate2;
extern const uint8_t _pinGate3;
extern const uint8_t _pinGate4;

// Sync [5V DIO]
extern const uint8_t _pinSync;

// SPI
extern const uint32_t _spiSpeed;

// MIDI
extern const uint8_t _pinMidiRx;
extern const uint8_t _pinMidiTx;

// DAC8564
extern const uint8_t _pinDacSync;
extern const uint8_t _pinDacLdac;
extern const uint8_t _pinDacEnable;
// extern const uint8_t _pinDacMosi;

extern const uint8_t _pinDisplaySs;
extern const uint8_t _pinDisplaySck;
extern const uint8_t _pinDisplayMosi;