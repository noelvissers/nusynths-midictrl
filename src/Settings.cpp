#include "Settings.h"
#include "FlashStorage_SAMD.h"

CSettings::CSettings()
{
  // Default initialization
  SSettings defaultSettings;
  mSettings = defaultSettings;
}

// Load settings from flash
void CSettings::loadSettings(SSettings &settings)
{
  // TODO: implementation of loadSettings
}

// Save settings to flash
// WARNING: Flash can perform a limited amount of write cycles to the same block.
// Make sure to limit writing cycles as much as possible.
void CSettings::saveSettings(const SSettings &settings)
{
  // TODO: implementation of saveSettings
}

SSettings CSettings::getSettings() const
{
  return mSettings;
}

/**
 * TODO:
 * - Finish implementing EEPROM/Flash load save functionality
 * - Finish implementing MIDI handling
 * - Finish menu handling with actual functions instead of nullptr
 * - Add GUI
 * - Add midi learn functionality
 * - add briefs to all header files
 */