#pragma once

#include "Settings.h"
#include "MidiHandler.h"
#include "Outputs.h"
#include "Gui.h"

class CMenu
{
public:
  CMenu(CGui &gui, CSettings &settings, CMidiHandler &midi);
  ~CMenu() = default;

  void display();
  void update(volatile bool &next, volatile bool &prev, volatile bool &press);
  void start() { _active = true; }
  bool active() { return _active; }

private:
  CGui &mGui;
  CSettings &mSettings;
  CMidiHandler &mMidi;

  std::array<int, 3> _index = {0, 0, 0};
  int _depth = 0;
  bool _active = false;

  // Submenu configuration
  void subMenuConfig();
  void subMenuConfigMidiChannel();
  void optionConfigMidiChannel(uint8_t channel, std::string onHighlight);
  void subMenuConfigMode();
  void optionConfigMode(ESynthMode mode, std::string onHighlight);
  void subMenuConfigPitchBend();
  void optionConfigPitchBend(uint8_t pitchBendSemitones, std::string onHighlight);
  void subMenuConfigClock();
  void optionConfigClock(uint8_t clockDiv, std::string onHighlight);

  // Submenu output
  void subMenuOutputCv(uint16_t outputIndex);
  void subMenuOutputGate(uint16_t outputIndex);
  void optionOutput(uint16_t outputIndex, EOutputFunction function, std::string onHighlight);

  // Functions
  void waitForInput(volatile bool &next, volatile bool &prev, volatile bool &press);
  void setOutputFunction(uint16_t index, EOutputFunction function);

  bool _next = false;
  bool _prev = false;
  bool _select = false;
  bool _back = false;

  uint8_t _ledMap[9] = {
      0,          // unused (index 0)
      0b01000000, // 1
      0b00100000, // 2
      0b00010000, // 3
      0b00001000, // 4
      0b00000100, // 5
      0b00000010, // 6
      0b00000001, // 7
      0b10000000  // 8
  };
};