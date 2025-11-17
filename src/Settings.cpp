#include "Settings.h"
#include "FlashStorage_SAMD.h"

CSettings::CSettings()
{
}

// Load settings from flash
void CSettings::load()
{
  int signature;
  EEPROM.get(SETTINGS_ADDRESS, signature);

  if (signature == SETTINGS_SIGNATURE)
  {
    // Valid signature, load settings
    EEPROM.get(SETTINGS_ADDRESS + sizeof(signature), mSettings);
  }
  else
  {
    // Invalid signature, load defaults
    mSettings = SSystemSettings();
  }
}

// Save settings to flash
void CSettings::save()
{
  // Load settings to compare
  int signature;
  SSystemSettings savedSettings;
  EEPROM.get(SETTINGS_ADDRESS, signature);
  if (signature == SETTINGS_SIGNATURE)
    EEPROM.get(SETTINGS_ADDRESS + sizeof(signature), savedSettings);

  // Check if settings have changed to avoid unnecessary writes
  if (memcmp(&savedSettings, &mSettings, sizeof(SSystemSettings)) != 0)
  {
    EEPROM.put(SETTINGS_ADDRESS, SETTINGS_SIGNATURE);
    EEPROM.put(SETTINGS_ADDRESS + sizeof(SETTINGS_SIGNATURE), mSettings);

    if (!EEPROM.getCommitASAP())
      EEPROM.commit();
  }
}

SSystemSettings &CSettings::get()
{
  return mSettings;
}