#pragma once

#include <cstdint>
#include <array>
#include "Outputs.h"

constexpr int TRIGGER_LENGHT_MS = 1; // Trigger length in milliseconds

enum class ESynthMode
{
  Monophonic = 0,
  Polyphonic = 1
};

// Settings
struct SSettings
{
  uint8_t midiChannel = 0; // 1-16, 0 for omni
  ESynthMode synthMode = ESynthMode::Monophonic;
  uint8_t pitchBendSemitones = 12;
  uint8_t clockDiv = 1;
  std::array<EOutputFunction, 9> outputFunctions;
  std::array<bool, 9> outputIsMapped;
  std::array<uint8_t, 9> outputMappedTo;

  SSettings()
  {
    outputFunctions.fill(EOutputFunction::Unassigned);
    outputIsMapped.fill(false);
    outputMappedTo.fill(0);
  }
};

const uint32_t SETTINGS_SIGNATURE = 0x0B00B1E5;
const uint32_t SETTINGS_ADDRESS = 0x0;

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
   * @brief Get the current settings
   * @return Current settings structure
   */
  SSettings& get();

private:
  SSettings mSettings;
};