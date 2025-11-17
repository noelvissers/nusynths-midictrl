#pragma once

#include <cstdint>
#include <array>
#include "Outputs.h"

constexpr int TRIGGER_LENGHT_MS = 1; // Trigger length in milliseconds

const uint32_t SETTINGS_SIGNATURE = 0x0B00B1E5;
const uint32_t SETTINGS_ADDRESS = 0x0;

enum class ESynthMode
{
  Monophonic = 0,
  Polyphonic = 1
};

struct SOutputSettings
{
  EOutputFunction function = EOutputFunction::Unassigned;
  bool isMapped = false;    // Indicates if the output is mapped to a specific MIDI note or CC
  uint8_t mappedTo = 0;     // Mapped value
};

// Hold all necessary information about the system for load-time use, can be edited via the menu
struct SSystemSettings
{
  uint8_t midiChannel = 0; // 1-16, 0 for omni
  ESynthMode synthMode = ESynthMode::Monophonic;
  uint8_t pitchBendSemitones = 12;
  uint8_t clockDiv = 1;
  std::array<SOutputSettings, 9> outputSettings;
};

class CSettings
{
public:
  CSettings();
  ~CSettings() = default;

  /**
   * @brief Load settings from flash
   */
  void load();

  /**
   * @brief Save settings to flash
   * @warning Flash can perform a limited amount of write cycles to the same block, make sure to limit writing cycles as much as possible.
   */
  void save();

  /**
   * @brief Get the current system settings
   * @return Current system settings struct
   */
  SSystemSettings& get();

private:
  SSystemSettings mSettings;
};