#pragma once

#include <cstdint>

enum class SynthMode
{
  Monophonic = 0,
  Polyphonic = 1
};

// LED
extern const uint8_t _pinLed;

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

// CV
extern const uint8_t _dacChannelCv1;
extern const uint8_t _dacChannelCv2;
extern const uint8_t _dacChannelCv3;
extern const uint8_t _dacChannelCv4;

// Gates
extern const uint8_t _pinGate1;
extern const uint8_t _pinGate2;
extern const uint8_t _pinGate3;
extern const uint8_t _pinGate4;

// Sync
extern const uint8_t _pinSync;

// SPI
extern const uint8_t _pinSpiMOSI;
extern const uint8_t _pinSpiMISO;
extern const uint8_t _pinSpiCLK;

extern const uint8_t _pinCsDisplay;
extern const uint8_t _pinCsDac;

// Trigger
extern const uint8_t _triggerLengthMs; // Length of the trigger pulse in milliseconds

// Mode
extern const SynthMode _synthMode;

class CConfig
{
public:
  void loadSettings();
  void saveSettings();

private:
};