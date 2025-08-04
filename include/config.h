#pragma once

#include <cstdint>

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

class CConfig
{
public:
  void Example();
private:
};

// Example 
enum class OutputType {
  ANALOG,
  DIGITAL,
  UNDEFINED,
};

struct OutputConfig {
  const char* name;  // Descriptive name for the output
  uint8_t pinNumber; // The hardware pin number
  OutputType type;   // The type of output
};

enum class Output {
  SYNC,
  CV1,
  CV2,
  CV3,
  CV4,
  GATE1,
  GATE2,
  GATE3,
  GATE4,
};

// TODO: Probably add output function (Pitch/Velocity/CC/AfterTouch/Gate/Trigger/StartStop) and other configurable items
const OutputConfig outputConfig[] = {
  { "SYNC", 0, OutputType::DIGITAL },
  { "CV1", 0, OutputType::ANALOG },
  { "CV1", 0, OutputType::ANALOG },
  { "CV1", 0, OutputType::ANALOG },
  { "CV1", 0, OutputType::ANALOG },
  { "GATE1", 0, OutputType::DIGITAL },
  { "GATE2", 0, OutputType::DIGITAL },
  { "GATE3", 0, OutputType::DIGITAL },
  { "GATE4", 0, OutputType::DIGITAL },
};

inline const OutputConfig& getOutput(Output output) {
  return outputConfig[static_cast<int>(output)];
}

// TODO: things to save/configure
/**
 * Value (used for analog values)
 * Set time (used for triggers) [micros()]
 */