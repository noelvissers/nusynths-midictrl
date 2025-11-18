#include "Menu.h"
#include <Arduino.h>
#include <iostream>

// MenuItem
CMenuItem::CMenuItem(const std::string &name, const uint8_t led) : _name(name), _led(led) {}

const std::string &CMenuItem::getName() const
{
  return _name;
}

uint8_t CMenuItem::getLed() const
{
  return _led;
}

// SubMenu
CSubMenu::CSubMenu(const std::string &name, const uint8_t led) : CMenuItem(name, led) {}

bool CSubMenu::isSubMenu() const
{
  return true;
}

CSubMenu &CSubMenu::addOption(const std::string &name, const uint8_t led, std::function<void()> onSelect)
{
  _items.push_back(std::unique_ptr<CMenuItem>(new CMenuOption(name, led, onSelect)));
  return *this;
}

CSubMenu &CSubMenu::addSubMenu(const std::string &name, const uint8_t led)
{
  std::unique_ptr<CSubMenu> newSubMenu(new CSubMenu(name, led));
  CSubMenu *newSubMenuPtr = newSubMenu.get();
  _items.push_back(std::move(newSubMenu));
  return *newSubMenuPtr;
}

const std::vector<std::unique_ptr<CMenuItem>> &CSubMenu::getItems() const
{
  return _items;
}

// MenuOption
CMenuOption::CMenuOption(const std::string &name, const uint8_t led, std::function<void()> onSelect)
    : CMenuItem(name, led), onSelectCallback(onSelect) {}

bool CMenuOption::isSubMenu() const
{
  return false;
}

// Menu
CMenu::CMenu(const std::string &name, CGui &gui, CSettings &systemSettings, CMidiHandler &midiHandler)
    : CSubMenu(name, 0), mGui(gui), mSystemSettings(systemSettings), mMidiHandler(midiHandler), _currentMenu(this), _selectedIndex(0) {}

void CMenu::build()
{
  // Config
  CSubMenu &configMenu = this->addSubMenu("Cnf", 0b11111111);
  {
    // MIDI channel
    configMenu.addSubMenu("Chn", 0)
        .addOption("All", 0, [this]()
                   { mSystemSettings.get().midiChannel = 0; }) // OMNI
        .addOption("1", 0, [this]()
                   { mSystemSettings.get().midiChannel = 1; }) // Channel 1
        .addOption("2", 0, [this]()
                   { mSystemSettings.get().midiChannel = 2; })
        .addOption("3", 0, [this]()
                   { mSystemSettings.get().midiChannel = 3; })
        .addOption("4", 0, [this]()
                   { mSystemSettings.get().midiChannel = 4; })
        .addOption("5", 0, [this]()
                   { mSystemSettings.get().midiChannel = 5; })
        .addOption("6", 0, [this]()
                   { mSystemSettings.get().midiChannel = 6; })
        .addOption("7", 0, [this]()
                   { mSystemSettings.get().midiChannel = 7; })
        .addOption("8", 0, [this]()
                   { mSystemSettings.get().midiChannel = 8; })
        .addOption("9", 0, [this]()
                   { mSystemSettings.get().midiChannel = 9; })
        .addOption("10", 0, [this]()
                   { mSystemSettings.get().midiChannel = 10; })
        .addOption("11", 0, [this]()
                   { mSystemSettings.get().midiChannel = 11; })
        .addOption("12", 0, [this]()
                   { mSystemSettings.get().midiChannel = 12; })
        .addOption("13", 0, [this]()
                   { mSystemSettings.get().midiChannel = 13; })
        .addOption("14", 0, [this]()
                   { mSystemSettings.get().midiChannel = 14; })
        .addOption("15", 0, [this]()
                   { mSystemSettings.get().midiChannel = 15; })
        .addOption("16", 0, [this]()
                   { mSystemSettings.get().midiChannel = 16; });
    // Mode
    configMenu.addSubMenu("Mod", 0)
        .addOption("Mon", 0, [this]()
                   { mSystemSettings.get().synthMode = ESynthMode::Monophonic; })
        .addOption("Pol", 0, [this]()
                   { mSystemSettings.get().synthMode = ESynthMode::Polyphonic; });
    // Pitch bend
    configMenu.addSubMenu("Pb.", 0)
        .addOption("0", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 0; }) // 0 semitones
        .addOption("1", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 1; }) // 1 semitone
        .addOption("2", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 2; }) // 2 semitones
        .addOption("3", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 3; })
        .addOption("4", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 4; })
        .addOption("5", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 5; })
        .addOption("6", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 6; })
        .addOption("7", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 7; })
        .addOption("8", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 8; })
        .addOption("9", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 9; })
        .addOption("10", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 10; })
        .addOption("11", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 11; })
        .addOption("12", 0, [this]()
                   { mSystemSettings.get().pitchBendSemitones = 12; });
    // Clock divider
    configMenu.addSubMenu("Clk", 0)
        .addOption("1", 0, [this]()
                   { mSystemSettings.get().clockDiv = 1; }) // 1:1
        .addOption("2", 0, [this]()
                   { mSystemSettings.get().clockDiv = 2; }) // /2
        .addOption("4", 0, [this]()
                   { mSystemSettings.get().clockDiv = 4; }) // /4
        .addOption("8", 0, [this]()
                   { mSystemSettings.get().clockDiv = 8; }) // /8
        .addOption("16", 0, [this]()
                   { mSystemSettings.get().clockDiv = 16; }) // /16
        .addOption("32", 0, [this]()
                   { mSystemSettings.get().clockDiv = 32; }) // /32
        .addOption("64", 0, [this]()
                   { mSystemSettings.get().clockDiv = 64; }) // /64
        .addOption("128", 0, [this]()
                   { mSystemSettings.get().clockDiv = 128; }); // /128
  }
  // CV output 1
  this->addSubMenu("1", 0b01000000)
      .addOption("Ptc", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Pitch); })
      .addOption("VEL", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Velocity); })
      .addOption("CC", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::ContinuesController); })
      .addOption("At", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::AfterTouch); })
      .addOption("Gt", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Gate); })
      .addOption("Tr", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Trigger); })
      .addOption("Rst", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Reset); })
      .addOption("-", 0b01000000, [this]()
                 { setOutputFunction(1, EOutputFunction::Unassigned); });
  // CV output 2
  this->addSubMenu("2", 0b00100000)
      .addOption("Ptc", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Pitch); })
      .addOption("VEL", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Velocity); })
      .addOption("CC", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::ContinuesController); })
      .addOption("At", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::AfterTouch); })
      .addOption("Gt", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Gate); })
      .addOption("Tr", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Reset); })
      .addOption("-", 0b00100000, [this]()
                 { setOutputFunction(2, EOutputFunction::Unassigned); });
  // CV output 3
  this->addSubMenu("3", 0b00010000)
      .addOption("Ptc", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Pitch); })
      .addOption("VEL", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Velocity); })
      .addOption("CC", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::ContinuesController); })
      .addOption("At", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::AfterTouch); })
      .addOption("Gt", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Gate); })
      .addOption("Tr", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Reset); })
      .addOption("-", 0b00010000, [this]()
                 { setOutputFunction(3, EOutputFunction::Unassigned); });
  // CV output 4
  this->addSubMenu("4", 0b00001000)
      .addOption("Ptc", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Pitch); })
      .addOption("VEL", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Velocity); })
      .addOption("CC", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::ContinuesController); })
      .addOption("At", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::AfterTouch); })
      .addOption("Gt", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Gate); })
      .addOption("Tr", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Reset); })
      .addOption("-", 0b00001000, [this]()
                 { setOutputFunction(4, EOutputFunction::Unassigned); });
  // Gate output 1
  this->addSubMenu("5", 0b00000100)
      .addOption("Gt", 0b00000100, [this]()
                 { setOutputFunction(5, EOutputFunction::Gate); })
      .addOption("Tr", 0b00000100, [this]()
                 { setOutputFunction(5, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00000100, [this]()
                 { setOutputFunction(5, EOutputFunction::Reset); })
      .addOption("-", 0b00000100, [this]()
                 { setOutputFunction(5, EOutputFunction::Unassigned); });
  // Gate output 2
  this->addSubMenu("6", 0b00000010)
      .addOption("Gt", 0b00000010, [this]()
                 { setOutputFunction(6, EOutputFunction::Gate); })
      .addOption("Tr", 0b00000010, [this]()
                 { setOutputFunction(6, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00000010, [this]()
                 { setOutputFunction(6, EOutputFunction::Reset); })
      .addOption("-", 0b00000010, [this]()
                 { setOutputFunction(6, EOutputFunction::Unassigned); });
  // Gate output 3
  this->addSubMenu("7", 0b00000001)
      .addOption("Gt", 0b00000001, [this]()
                 { setOutputFunction(7, EOutputFunction::Gate); })
      .addOption("Tr", 0b00000001, [this]()
                 { setOutputFunction(7, EOutputFunction::Trigger); })
      .addOption("Rst", 0b00000001, [this]()
                 { setOutputFunction(7, EOutputFunction::Reset); })
      .addOption("-", 0b00000001, [this]()
                 { setOutputFunction(7, EOutputFunction::Unassigned); });
  // Gate output 4
  this->addSubMenu("8", 0b10000000)
      .addOption("Gt", 0b10000000, [this]()
                 { setOutputFunction(8, EOutputFunction::Gate); })
      .addOption("Tr", 0b10000000, [this]()
                 { setOutputFunction(8, EOutputFunction::Trigger); })
      .addOption("Rst", 0b10000000, [this]()
                 { setOutputFunction(8, EOutputFunction::Reset); })
      .addOption("-", 0b10000000, [this]()
                 { setOutputFunction(8, EOutputFunction::Unassigned); });
}

void CMenu::display() const
{
  if (!_currentMenu)
    return;

  const auto &items = _currentMenu->getItems();

  mGui.setString(items[_selectedIndex]->getName());
  mGui.setLed(3, items[_selectedIndex]->getLed());
}

void CMenu::waitForInput(volatile bool &next, volatile bool &prev, volatile bool &press)
{
  while (press)
    ; // Wait for button to be released

  next = prev = false;
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

void CMenu::update()
{
  if (!_currentMenu || _currentMenu->getItems().empty())
  {
    _next = _prev = _back = _select = false;
    return;
  }

  const auto &items = _currentMenu->getItems();
  int itemCount = items.size();

  if (_next)
    _selectedIndex = (_selectedIndex + 1) % itemCount;
  else if (_prev)
    _selectedIndex = (_selectedIndex - 1 + itemCount) % itemCount;
  else if (_select)
  {
    CMenuItem *selectedItem = items[_selectedIndex].get();
    if (selectedItem->isSubMenu())
    {
      _navigationStack.push({_currentMenu, _selectedIndex});
      _currentMenu = static_cast<CSubMenu *>(selectedItem);
      _selectedIndex = getIndexFromSetting(selectedItem);
    }
    else
    {
      CMenuOption *option = static_cast<CMenuOption *>(selectedItem);
      if (option->onSelectCallback)
      {
        option->onSelectCallback();

        // Blink all leds to confirm action
        mGui.setLed(3, 0b11111111);
        delay(250);

        // Back action after executing function
        if (!_navigationStack.empty())
        {
          auto lastState = _navigationStack.top();
          _currentMenu = lastState.first;
          _selectedIndex = lastState.second;
          _navigationStack.pop();
        }
        else
        {
          // Exit menu if back action is on root
          bActive = false;
          mGui.clear();
        }
      }
    }
  }
  else if (_back)
  {
    if (!_navigationStack.empty())
    {
      auto &lastState = _navigationStack.top();
      _currentMenu = lastState.first;
      _selectedIndex = lastState.second;
      _navigationStack.pop();
    }
    else
    {
      // Exit menu if back is pressed on root
      bActive = false;
      mGui.clear();
    }
  }

  interrupts();
  _next = _prev = _back = _select = false;
}

int CMenu::getIndexFromSetting(CMenuItem *selected)
{
  if (!selected->isSubMenu())
    return 0;

  if (_navigationStack.empty()) // In root menu
  {
    if (selected->getName() == "Cnf")
      return 0; // Config menu
    else
    {
      try
      {
        int outputChannel = stoi(selected->getName());
        EOutputFunction function = mSystemSettings.get().outputSettings.at(outputChannel).function;
        if (outputChannel <= 4) // CV
          return static_cast<int>(function);
        else // Gate
          return (static_cast<int>(function) - 4);
      }
      catch (const std::exception e)
      {
        return 0;
      }
    }
  }
  else if (_navigationStack.top().first->getName() == "Cnf") // Part of Cnf menu
  {
    if(selected->getName() == "Chn")
      return mSystemSettings.get().midiChannel;
    else if(selected->getName() == "Mod")
      return static_cast<int>(mSystemSettings.get().synthMode);
    else if(selected->getName() == "Pb.")
      return mSystemSettings.get().pitchBendSemitones;
    else if(selected->getName() == "Clk")
      switch (mSystemSettings.get().clockDiv)
      {
      case 1:
        return 0;
      case 2:
        return 1;
      case 4:
        return 2;
      case 8:
        return 3;
      case 16:
        return 4;
      case 32:
        return 5;
      case 64:
        return 6;
      case 128:
        return 7;
      default:
        return 0;
      }
  }
  return 0;
}

void CMenu::setOutputFunction(uint16_t index, EOutputFunction function)
{
  if (function == EOutputFunction::ContinuesController)
  {
    // MIDI learn required
    uint8_t cc;
    if (mMidiHandler.learn(cc, _select))
    {
      mSystemSettings.get().outputSettings[index].function = function;
      mSystemSettings.get().outputSettings[index].isMapped = true;
      mSystemSettings.get().outputSettings[index].mappedTo = cc;
    }
  }
  else if (function == EOutputFunction::Trigger)
  {
    // MIDI learn optional
    uint8_t key;
    if (mMidiHandler.learn(key, _select))
    {
      mSystemSettings.get().outputSettings[index].isMapped = true;
      mSystemSettings.get().outputSettings[index].mappedTo = key;
    }
    else
    {
      mSystemSettings.get().outputSettings[index].isMapped = false;
    }
    mSystemSettings.get().outputSettings[index].function = function;
  }
  else
  {
    mSystemSettings.get().outputSettings[index].function = function;
  }
}