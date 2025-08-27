#include "Settings.h"
#include "FlashStorage_SAMD.h"

CSettings::CSettings()
{
}

// Load settings from flash
bool CSettings::load()
{
  int signature;
  EEPROM.get(SETTINGS_ADDRESS, signature);

  if (signature == SETTINGS_SIGNATURE)
  {
    // Valid signature, load settings
    EEPROM.get(SETTINGS_ADDRESS + sizeof(signature), mSettings);
    return true;
  }
  return false;
}

// Save settings to flash
void CSettings::save()
{
  EEPROM.put(SETTINGS_ADDRESS, SETTINGS_SIGNATURE);
  EEPROM.put(SETTINGS_ADDRESS + sizeof(SETTINGS_SIGNATURE), mSettings);

  if (!EEPROM.getCommitASAP())
    EEPROM.commit();
}

SSettings &CSettings::get()
{
  return mSettings;
}