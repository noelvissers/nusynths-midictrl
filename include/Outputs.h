#pragma once

#include <SPI.h>
#include <DAC8564.h>
#include <cstdint>
#include <array>

constexpr uint8_t N_OUTPUTS = 9;        // Total number of outputs (1 SYNC, 4 CV, 4 GATE)
constexpr uint16_t OUTPUT_HIGH = 32768; // 5V for DAC, HIGH for IO
constexpr uint16_t OUTPUT_LOW = 0;

enum class EOutputType : uint8_t
{
  Analog = 1,
  Digital = 2,
  Undefined = 0,
};

enum class EOutputFunction : uint8_t
{
  Pitch = 1,
  Velocity = 2,
  ContinuesController = 3,
  AfterTouch = 4,
  Gate = 5,
  Trigger = 6,
  Sync = 7,
  Reset = 8,
  Unassigned = 0
};

struct SOutput
{
  EOutputType type = EOutputType::Undefined;
  EOutputFunction function = EOutputFunction::Unassigned;
  bool isActive = false;                             // Indicates if the output is currently active (and/or can be overwritten)
  bool isDirty = true;                               // Indicates if the output value has changed and needs to be updated
  uint16_t value = 0;                                // Value to be written to the output
  long pitchBend = 0;                                // Pitch bend value (midpoint = 0)
  uint8_t outputPin = 0;                             // GPIO pin for Gate outputs (default = 0)
  DAC8564::Channel dacChannel = DAC8564::Channel::A; // DAC channel for CV outputs (default = A)
  bool isMapped = false;                             // Indicates if the output is mapped to a specific MIDI note or CC
  uint8_t mappedTo = 0;                              // MIDI note number for triggers and CC (default = 0)
  unsigned long resetTime = 0;                       // Time when the output should be reset (used for triggers)
};

class COutputs
{
public:
  COutputs();
  ~COutputs() = default;

  void init();
  void update();

  void setOutput(uint16_t index, const SOutput &outputConfig);
  SOutput getOutput(uint16_t index) const;

  uint16_t midiTo1VOct(uint8_t byte);
  uint16_t midiToCv(uint8_t byte);
  long pitchBendToCv(int pitchBend, uint8_t semitones);

private:
  DAC8564 mDac;
  std::array<SOutput, N_OUTPUTS> mOutputs;
  uint16_t getPitch(uint16_t pitch, long pitchBend);
};