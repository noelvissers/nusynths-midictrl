#pragma once

#include <cstdint>
#include <array>

enum class OutputType
{
  Analog = 1,
  Digital = 2,
  Undefined = 0,
};

enum class OutputFunction
{
  Pitch = 1,
  Velocity = 2,
  ContinuesController = 3,
  AfterTouch = 4,
  Gate = 5,
  Trigger = 6,
  Sync = 7,
  StartStop = 8,
  Unassigned = 0
};

struct Output
{
  OutputType type = OutputType::Undefined;
  OutputFunction function = OutputFunction::Unassigned;
  bool isActive = false;       // Indicates if the output is currently active (and/or can be overwritten)
  bool isDirty = false;        // Indicates if the output value has changed and needs to be updated
  uint16_t value = 0;          // Value to be written to the output
  uint8_t outputPin = 0;       // GPIO pin for Gate outputs (default = 0)
  uint8_t dacChannel = 0;      // DAC channel for CV outputs (default = 0)
  bool isMapped = false;       // Indicates if the output is mapped to a specific MIDI note or CC
  byte mappedNote = 0;         // MIDI note number for triggers and CC (default = 0)
  unsigned long resetTime = 0; // Time when the output should be reset (used for triggers)
};

class COutputs
{
public:
  std::array<Output, 9> aOutputs; // 0 is Sync, 1-4 are CV outputs, 5-9 are GATE outputs

  void init();
  void update();
  void setOutputConfig(uint8_t output, OutputFunction function = OutputFunction::Unassigned);
  void setOutputValue(uint8_t output, uint16_t value, bool isActive = true);
private:
};