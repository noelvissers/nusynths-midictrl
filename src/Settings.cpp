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

// Print settings to serial for debugging
void CSettings::print()
{
  Serial.println("Settings:");
  Serial.print("├─ MIDI Channel: ");
  Serial.println(mSettings.midiChannel);
  Serial.print("├─ Mode: ");
  Serial.println(static_cast<int>(mSettings.synthMode));
  Serial.print("├─ Pitch Bend: ");
  Serial.println(mSettings.pitchBendSemitones);
  Serial.print("├─ Clock Division: ");
  Serial.println(mSettings.clockDiv);
  Serial.println("└─ Outputs:");
  for (size_t i = 0; i < mSettings.outputSettings.size(); ++i)
  {
    if (i == 8)
      Serial.print("   └─ ");
    else
      Serial.print("   ├─ ");
      
    Serial.print(i);
    Serial.print(", mode=");

    switch (mSettings.outputSettings[i].function)
    {
    case EOutputFunction::Pitch:
      Serial.print("Pitch              ");
      break;
    case EOutputFunction::Velocity:
      Serial.print("Velocity           ");
      break;
    case EOutputFunction::ContinuesController:
      Serial.print("ContinuesController");
      break;
    case EOutputFunction::AfterTouch:
      Serial.print("AfterTouch         ");
      break;
    case EOutputFunction::Gate:
      Serial.print("Gate               ");
      break;
    case EOutputFunction::Trigger:
      Serial.print("Trigger            ");
      break;
    case EOutputFunction::Sync:
      Serial.print("Sync               ");
      break;
    case EOutputFunction::Reset:
      Serial.print("Reset              ");
      break;
    case EOutputFunction::Unassigned:
      Serial.print("Unassigned         ");
      break;
    default:
      break;
    }

    Serial.print(" isMapped=");
    Serial.print(mSettings.outputSettings[i].isMapped ? "true " : "false");
    Serial.print(" mappedTo=");
    Serial.println(mSettings.outputSettings[i].mappedTo);
  }
}