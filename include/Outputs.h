#pragma once

#include <SPI.h>
#include <DAC8564.h>
#include <cstdint>
#include <array>

#define N_OUTPUTS 9       // Total number of outputs (1 SYNC, 4 CV, 4 GATE)
#define OUTPUT_HIGH 32768 // 5V for DAC, HIGH for IO
#define OUTPUT_LOW 0

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

  /**
   * @brief Sets up the DAC and initializes all digital output pins.
   */
  void init();

  /**
   * @brief Updates the outputs based on their current state.
   */
  void update();

  /**
   * @brief Sets the configuration for a specific output.
   * @param index The index of the output to set (0 to N_OUTPUTS-1).
   * @param outputConfig The configuration for the output.
   */
  void setOutput(uint16_t index, const SOutput &outputConfig);

  /**
   * @brief Gets the configuration for a specific output.
   * @param index The index of the output to get (0 to N_OUTPUTS-1).
   * @return The configuration for the output.
   */
  SOutput getOutput(uint16_t index) const;

  /**
   * @brief Maps a MIDI note to a 16 bit 1V/Oct value for the DAC.
   * @param byte The MIDI note number (0-127).
   * @return The CV value (0-65535).
   */
  uint16_t midiTo1VOct(uint8_t byte);

  /**
   * @brief Maps a MIDI value to a 16 bit value for the DAC.
   * @param byte The MIDI value (0-127).
   * @return The CV value (0-65535).
   */
  uint16_t midiToCv(uint8_t byte);

  /**
   * @brief Maps the MIDI pitch bend value to a 16 bit value.
   * @param pitchBend The MIDI pitch bend value (14 bit, -8192 to 8191).
   * @param semitones The range of pitch bend in semitones.
   * @return The pitch bend value (32 bit, -65535 to 65535).
   */
  long pitchBendToCv(int pitchBend, uint8_t semitones);

private:
  DAC8564 mDac;
  std::array<SOutput, N_OUTPUTS> mOutputs;
  uint16_t getPitch(uint16_t pitch, long pitchBend);
};