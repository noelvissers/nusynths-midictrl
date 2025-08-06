#pragma once

#include <cstdint>

constexpr int TRIGGER_LENGHT_MS = 1; // Default trigger length in milliseconds

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
  // TODO: Also add output settings here
};

class CSettings
{
public:
  CSettings();
  ~CSettings() = default;

  void loadSettings(SSettings& settings);
  void saveSettings(const SSettings& settings);
  SSettings getSettings() const;

private:
  SSettings mSettings; 
};