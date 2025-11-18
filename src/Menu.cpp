#include "Menu.h"
#include <Arduino.h>
#include <cstdint>
#include <math.h>

CMenu::CMenu(CGui &gui, CSettings &settings, CMidiHandler &midi) : mGui(gui), mSettings(settings), mMidi(midi) {}

// Root menu (DEPTH = 0)
void CMenu::display()
{
  switch (_index[0])
  {
  case -1: // Wrap around to last menu
    _index[0] = 8;
    display();
    break;

  case 0: // Configuration
    subMenuConfig();
    break;
  case 1 ... 4: // Output 1 ... 4
    subMenuOutputCv(_index[0]);
    break;
  case 5 ... 8: // Output 5 ... 8
    subMenuOutputGate(_index[0]);
    break;

  default:
    _index[0] = 0;
    display();
    break;
  }
}

void CMenu::update(volatile bool &next, volatile bool &prev, volatile bool &press)
{
  waitForInput(next, prev, press);

  if (_next)
    _index[_depth]++;
  else if (_prev)
    _index[_depth]--;
  else if (_select)
  {
    if (_depth < int(sizeof(_index)))
    {
      _depth++;
      _index[_depth] = -2;
    }
  }
  else if (_back)
  {
    if (_depth <= 0) // Exit menu
    {
      std::fill(std::begin(_index), std::end(_index), 0);
      _active = false;
    }
    else
      _depth--;
  }
}

// Private submenus
// Configuration menu
void CMenu::subMenuConfig()
{
  if (_depth >= 1) // Menu is selected
  {
    switch (_index[1])
    {
    case 0: // MIDI channel
      subMenuConfigMidiChannel();
      break;
    case 1: // Synth mode
      subMenuConfigMode();
      break;
    case 2: // Pitch bend semitones
      subMenuConfigPitchBend();
      break;
    case 3: // Clock division
      subMenuConfigClock();
      break;

    case -2: // Not saved in settings, return 0
    {
      _index[1] = 0;
      subMenuConfig();
      break;
    }
    case -1: // Wrap around to last menu
      _index[1] = 3;
      subMenuConfig();
      break;
    default:
      _index[1] = 0;
      subMenuConfig();
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString("Cnf");
    mGui.setLed(3, 0b11111111);
  }
}
void CMenu::subMenuConfigMidiChannel()
{
  if (_depth >= 2) // Menu is selected
  {
    switch (_index[2])
    {
    case -2: // Get index from current setting
    {
      _index[2] = mSettings.get().midiChannel;
      subMenuConfigMidiChannel();
      break;
    }
    case -1: // Wrap around to last menu
      _index[2] = 16;
      subMenuConfigMidiChannel();
      break;

    case 0:
      optionConfigMidiChannel(0, "All");
      break;
    case 1 ... 16:
      optionConfigMidiChannel(_index[2], std::to_string(_index[2]));
      break;

    default:
      _index[2] = 0;
      subMenuConfigMidiChannel();
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString("Chn");
    mGui.setLed(3, 0);
  }
}
void CMenu::optionConfigMidiChannel(uint8_t channel, std::string onHighlight)
{
  if (_depth >= 3) // Menu is selected
  {
    mSettings.get().midiChannel = channel;
    confirmOption();
  }
  else // Menu is highlighted
  {
    mGui.setString(onHighlight);
    mGui.setLed(3, 0);
  }
}
void CMenu::subMenuConfigMode()
{
  if (_depth >= 2) // Menu is selected
  {
    switch (_index[2])
    {
    case -2: // Get index from current setting
    {
      _index[2] = static_cast<int>(mSettings.get().synthMode);
      subMenuConfigMode();
      break;
    }
    case -1: // Wrap around to last menu
      _index[2] = 1;
      subMenuConfigMode();
      break;

    case 0: // Monophonic
      optionConfigMode(ESynthMode::Monophonic, "Mon");
      break;
    case 1: // Polyphonic
      optionConfigMode(ESynthMode::Polyphonic, "Pol");
      break;

    default:
      _index[2] = 0;
      subMenuConfigMode();
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString("Mod");
    mGui.setLed(3, 0);
  }
}
void CMenu::optionConfigMode(ESynthMode mode, std::string onHighlight)
{
  if (_depth >= 3) // Menu is selected
  {
    mSettings.get().synthMode = mode;
    confirmOption();
  }
  else // Menu is highlighted
  {
    mGui.setString(onHighlight);
    mGui.setLed(3, 0);
  }
}
void CMenu::subMenuConfigPitchBend()
{
  if (_depth >= 2) // Menu is selected
  {
    switch (_index[2])
    {
    case -2: // Get index from current setting
    {
      _index[2] = mSettings.get().pitchBendSemitones;
      subMenuConfigPitchBend();
      break;
    }
    case -1: // Wrap around to last menu
      _index[2] = 12;
      subMenuConfigPitchBend();
      break;

    case 0 ... 12:
      optionConfigPitchBend(_index[2], std::to_string(_index[2]));
      break;

    default:
      _index[2] = 0;
      subMenuConfigPitchBend();
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString("Pb");
    mGui.setLed(3, 0);
  }
}
void CMenu::optionConfigPitchBend(uint8_t pitchBendSemitones, std::string onHighlight)
{
  if (_depth >= 3) // Menu is selected
  {
    mSettings.get().pitchBendSemitones = pitchBendSemitones;
    confirmOption();
  }
  else // Menu is highlighted
  {
    mGui.setString(onHighlight);
    mGui.setLed(3, 0);
  }
}
void CMenu::subMenuConfigClock()
{
  if (_depth >= 2) // Menu is selected
  {
    switch (_index[2])
    {
    case -2: // Get index from current setting
    {
      _index[2] = log2(mSettings.get().clockDiv);
      subMenuConfigClock();
      break;
    }
    case -1: // Wrap around to last menu
      _index[2] = 7;
      subMenuConfigClock();
      break;

    case 0 ... 7:
    {
      uint8_t clkdiv = uint8_t(pow(2, _index[2]));
      optionConfigClock(clkdiv, std::to_string(clkdiv));
      break;
    }
    default:
      _index[2] = 0;
      subMenuConfigClock();
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString("Clk");
    mGui.setLed(3, 0);
  }
}
void CMenu::optionConfigClock(uint8_t clockDiv, std::string onHighlight)
{
  if (_depth >= 3) // Menu is selected
  {
    mSettings.get().clockDiv = clockDiv;
    confirmOption();
  }
  else // Menu is highlighted
  {
    mGui.setString(onHighlight);
    mGui.setLed(3, 0);
  }
}

// Output menu
void CMenu::subMenuOutputCv(uint16_t outputIndex)
{
  if (_depth >= 1) // Menu is selected
  {
    switch (_index[1])
    {
    case -2: // Get index from current setting
    {
      EOutputFunction output = mSettings.get().outputSettings[outputIndex].function;
      switch (output)
      {
      case EOutputFunction::Pitch:
        _index[1] = 0;
        break;
      case EOutputFunction::Velocity:
        _index[1] = 1;
        break;
      case EOutputFunction::ContinuesController:
        _index[1] = 2;
        break;
      case EOutputFunction::AfterTouch:
        _index[1] = 3;
        break;
      case EOutputFunction::Gate:
        _index[1] = 4;
        break;
      case EOutputFunction::Trigger:
        _index[1] = 5;
        break;
      case EOutputFunction::Reset:
        _index[1] = 6;
        break;
      case EOutputFunction::Unassigned:
        _index[1] = 7;
        break;
      default:
        _index[1] = 0;
        break;
      }
      subMenuOutputCv(outputIndex);
      break;
    }
    case -1: // Wrap around to last menu
      _index[1] = 7;
      subMenuOutputCv(outputIndex);
      break;

    case 0: // Pitch
      optionOutput(outputIndex, EOutputFunction::Pitch, "Ptc");
      break;
    case 1: // Velocity
      optionOutput(outputIndex, EOutputFunction::Velocity, "uEl");
      break;
    case 2: // CC
      optionOutput(outputIndex, EOutputFunction::ContinuesController, "Cc");
      break;
    case 3: // Aftertouch
      optionOutput(outputIndex, EOutputFunction::AfterTouch, "At");
      break;
    case 4: // Gate
      optionOutput(outputIndex, EOutputFunction::Gate, "Gt");
      break;
    case 5: // Trigger
      optionOutput(outputIndex, EOutputFunction::Trigger, "tr");
      break;
    case 6: // Reset
      optionOutput(outputIndex, EOutputFunction::Reset, "Rst");
      break;
    case 7: // Unassigned
      optionOutput(outputIndex, EOutputFunction::Unassigned, "-");
      break;
    default:
      _index[1] = 0;
      subMenuOutputCv(outputIndex);
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString(std::to_string(outputIndex));
    uint8_t ledValue = (outputIndex <= 8) ? _ledMap[outputIndex] : 0;
    mGui.setLed(3, ledValue);
  }
}
void CMenu::subMenuOutputGate(uint16_t outputIndex)
{
  if (_depth >= 1) // Menu is selected
  {
    switch (_index[1])
    {
    case -2: // Get index from current setting
    {
      EOutputFunction output = mSettings.get().outputSettings[outputIndex].function;
      switch (output)
      {
      case EOutputFunction::Gate:
        _index[1] = 0;
        break;
      case EOutputFunction::Trigger:
        _index[1] = 1;
        break;
      case EOutputFunction::Reset:
        _index[1] = 2;
        break;
      case EOutputFunction::Unassigned:
        _index[1] = 3;
        break;
      default:
        _index[1] = 0;
        break;
      }
      subMenuOutputGate(outputIndex);
      break;
    }
    case -1: // Wrap around to last menu
      _index[1] = 3;
      subMenuOutputGate(outputIndex);
      break;

    case 0: // Gate
      optionOutput(outputIndex, EOutputFunction::Gate, "Gt");
      break;
    case 1: // Trigger
      optionOutput(outputIndex, EOutputFunction::Trigger, "tr");
      break;
    case 2: // Reset
      optionOutput(outputIndex, EOutputFunction::Reset, "Rst");
      break;
    case 3: // Unassigned
      optionOutput(outputIndex, EOutputFunction::Unassigned, "-");
      break;
    default:
      _index[1] = 0;
      subMenuOutputGate(outputIndex);
      break;
    }
  }
  else // Menu is highlighted
  {
    mGui.setString(std::to_string(outputIndex));
    uint8_t ledValue = (outputIndex <= 8) ? _ledMap[outputIndex] : 0;
    mGui.setLed(3, ledValue);
  }
}
void CMenu::optionOutput(uint16_t outputIndex, EOutputFunction function, std::string onHighlight)
{
  if (_depth >= 2) // Menu is selected
  {
    setOutputFunction(outputIndex, function);
  }
  else // Menu is highlighted
  {
    mGui.setString(onHighlight);
    uint8_t ledValue = (outputIndex <= 8) ? _ledMap[outputIndex] : 0;
    mGui.setLed(3, ledValue);
  }
}

// Private functions
void CMenu::waitForInput(volatile bool &next, volatile bool &prev, volatile bool &press)
{
  while (press)
    ; // Wait for button to be released

  next = prev = false;                     // Reset external flags
  _next = _prev = _select = _back = false; // Reset internal flags

  while (!next && !prev && !press)
    ; // Wait for any input

  if (next)
    _next = true;
  else if (prev)
    _prev = true;
  else if (press)
  {
    unsigned long pressedAt = millis();
    while (press)
    {
      if (millis() - pressedAt >= 1000)
      {
        _back = true; // Long press for back
        return;
      }
    }
    _select = true;
  }
  return;
}

void CMenu::setOutputFunction(uint16_t index, EOutputFunction function)
{
  if (function == EOutputFunction::ContinuesController)
  {
    // MIDI learn required
    uint8_t cc;
    if (mMidi.learn(cc, _select))
    {
      mSettings.get().outputSettings[index].function = function;
      mSettings.get().outputSettings[index].isMapped = true;
      mSettings.get().outputSettings[index].mappedTo = cc;
    }
    else
      return;
  }
  else if (function == EOutputFunction::Trigger)
  {
    // MIDI learn optional
    uint8_t key;
    if (mMidi.learn(key, _select))
    {
      mSettings.get().outputSettings[index].isMapped = true;
      mSettings.get().outputSettings[index].mappedTo = key;
    }
    else
    {
      mSettings.get().outputSettings[index].isMapped = false;
    }
    mSettings.get().outputSettings[index].function = function;
  }
  else
  {
    mSettings.get().outputSettings[index].function = function;
  }
  confirmOption();
}

void CMenu::confirmOption()
{
  _depth -= 2;
  if (_depth < 0)
    _depth = 0;
  mGui.setLed(3, 0b11111111);
  delay(250);
  mGui.setLed(3, 0);
  display();
}