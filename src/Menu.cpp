#include "Menu.h"
#include <Arduino.h>

// MenuItem
CMenuItem::CMenuItem(const std::string &name, uint8_t led) : _name(name) {}

const std::string &CMenuItem::getName() const
{
  return _name;
}

uint8_t CMenuItem::getLed() const
{
  return _led;
}

// SubMenu
CSubMenu::CSubMenu(const std::string &name, uint8_t led) : CMenuItem(name, led) {}

bool CSubMenu::isSubMenu() const
{
  return true;
}

CSubMenu &CSubMenu::addOption(const std::string &name, uint8_t led, std::function<void()> onSelect)
{
  _items.push_back(std::unique_ptr<CMenuItem>(new CMenuOption(name, led, onSelect)));
  return *this;
}

CSubMenu &CSubMenu::addSubMenu(const std::string &name, uint8_t led)
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
CMenuOption::CMenuOption(const std::string &name, uint8_t led, std::function<void()> onSelect)
    : CMenuItem(name, led), onSelectCallback(onSelect) {}

bool CMenuOption::isSubMenu() const
{
  return false;
}

// Menu
CMenu::CMenu(const std::string &name, CGui &gui)
    : CSubMenu(name, 0), mGui(gui), _currentMenu(this), _selectedIndex(0) {}

void CMenu::build()
{
  // Menu structure:
  /*
   * Root
   * ├─ Settings [Cnf + All LEDs]
   * │  ├─ Midi channel [Chn]
   * │  │  ├─ All() [All]
   * │  │  ├─ 1() [1]
   * │  │  ├─ ...
   * │  │  └─ 16() [16]
   * │  ├─ Mode [Mod]
   * │  │  ├─ Mono() [Mon]
   * │  │  └─ Poly() [Pol]
   * │  ├─ Pitch bend [Pb.]
   * │  │  ├─ 0 semitones() [0]
   * │  │  ├─ 1 semitones() [1]
   * │  │  ├─ 2 semitones() [2]
   * │  │  ├─ ...
   * │  │  └─ 12 semitones() [12]
   * │  └─ Clock div [Clk]
   * │     ├─ 1 div() [1]
   * │     ├─ ...
   * │     └─ 256 div() [256]
   * ├─ Output GATE [1..4 + LED]
   * │  ├─ Gate() [Gt]
   * │  ├─ Trigger() [Tr] → Learn MIDI note [Optional]
   * │  ├─ Reset() [Rst]
   * │  └─ Unassigned() [-]
   * └─ Output CV [5..8 + LED]
   *   ├─ Pitch() [Ptc]
   *   ├─ Velocity() [VEL]
   *   ├─ CC() [CC] → Learn MIDI CC [Required]
   *   ├─ AfterTouch() [At.]
   *   ├─ Gate() [Gt]
   *   ├─ Trigger() [Tr] → Learn MIDI note [Optional]
   *   ├─ Reset() [Rst]
   *   └─ Unassigned() [-]
   */

  // Build menu
  {
    // Config
    CSubMenu &configMenu = this->addSubMenu("Cnf", 0);
    {
      // MIDI channel
      configMenu.addSubMenu("Chn", 0)
          .addOption("All", 0, nullptr) // OMNI
          .addOption("1", 0, nullptr)   // Channel 1
          .addOption("2", 0, nullptr)   // Channel 2
          .addOption("3", 0, nullptr)   // Channel 3
          .addOption("4", 0, nullptr)   // Channel 4
          .addOption("5", 0, nullptr)   // Channel 5
          .addOption("6", 0, nullptr)   // Channel 6
          .addOption("7", 0, nullptr)   // Channel 7
          .addOption("8", 0, nullptr)   // Channel 8
          .addOption("9", 0, nullptr)   // Channel 9
          .addOption("10", 0, nullptr)  // Channel 10
          .addOption("11", 0, nullptr)  // Channel 11
          .addOption("12", 0, nullptr)  // Channel 12
          .addOption("13", 0, nullptr)  // Channel 13
          .addOption("14", 0, nullptr)  // Channel 14
          .addOption("15", 0, nullptr)  // Channel 15
          .addOption("16", 0, nullptr); // Channel 16
      // Mode
      configMenu.addSubMenu("Mod", 0)
          .addOption("Mon", 0, nullptr)  // Mono
          .addOption("Pol", 0, nullptr); // Polyphonic
      // Pitch bend
      configMenu.addSubMenu("Pb.", 0)
          .addOption("0", 0, nullptr)   // 0 semitones
          .addOption("1", 0, nullptr)   // 1 semitone
          .addOption("2", 0, nullptr)   // 2 semitones
          .addOption("3", 0, nullptr)   // 3 semitones
          .addOption("4", 0, nullptr)   // 4 semitones
          .addOption("5", 0, nullptr)   // 5 semitones
          .addOption("6", 0, nullptr)   // 6 semitones
          .addOption("7", 0, nullptr)   // 7 semitones
          .addOption("8", 0, nullptr)   // 8 semitones
          .addOption("9", 0, nullptr)   // 9 semitones
          .addOption("10", 0, nullptr)  // 10 semitones
          .addOption("11", 0, nullptr)  // 11 semitones
          .addOption("12", 0, nullptr); // 12 semitones
      // Clock divider
      configMenu.addSubMenu("Clk", 0)
          .addOption("1", 0, nullptr)    // 1:1
          .addOption("2", 0, nullptr)    // /2
          .addOption("4", 0, nullptr)    // /4
          .addOption("8", 0, nullptr)    // /8
          .addOption("16", 0, nullptr)   // /16
          .addOption("32", 0, nullptr)   // /32
          .addOption("64", 0, nullptr)   // /64
          .addOption("128", 0, nullptr)  // /128
          .addOption("256", 0, nullptr); // /256
    }

    // CV output 1
    this->addSubMenu("1", 0b01000000)
        .addOption("Ptc", 0b01000000, nullptr) // Pitch
        .addOption("VEL", 0b01000000, nullptr) // Velocity
        .addOption("CC", 0b01000000, nullptr)  // Continues controller
        .addOption("At", 0b01000000, nullptr)  // Aftertouch
        .addOption("Gt", 0b01000000, nullptr)  // Gate
        .addOption("Tr", 0b01000000, nullptr)  // Trigger
        .addOption("Rst", 0b01000000, nullptr) // Reset
        .addOption("-", 0b01000000, nullptr);  // Unassigned
    // CV output 2
    this->addSubMenu("2", 0b00100000)
        .addOption("Ptc", 0b00100000, nullptr) // Pitch
        .addOption("VEL", 0b00100000, nullptr) // Velocity
        .addOption("CC", 0b00100000, nullptr)  // Continues controller
        .addOption("At", 0b00100000, nullptr)  // Aftertouch
        .addOption("Gt", 0b00100000, nullptr)  // Gate
        .addOption("Tr", 0b00100000, nullptr)  // Trigger
        .addOption("Rst", 0b00100000, nullptr) // Reset
        .addOption("-", 0b00100000, nullptr);  // Unassigned
    // CV output 3
    this->addSubMenu("3", 0b00010000)
        .addOption("Ptc", 0b00010000, nullptr) // Pitch
        .addOption("VEL", 0b00010000, nullptr) // Velocity
        .addOption("CC", 0b00010000, nullptr)  // Continues controller
        .addOption("At", 0b00010000, nullptr)  // Aftertouch
        .addOption("Gt", 0b00010000, nullptr)  // Gate
        .addOption("Tr", 0b00010000, nullptr)  // Trigger
        .addOption("Rst", 0b00010000, nullptr) // Reset
        .addOption("-", 0b00010000, nullptr);  // Unassigned
    // CV output 4
    this->addSubMenu("4", 0b00001000)
        .addOption("Ptc", 0b00001000, nullptr) // Pitch
        .addOption("VEL", 0b00001000, nullptr) // Velocity
        .addOption("CC", 0b00001000, nullptr)  // Continues controller
        .addOption("At", 0b00001000, nullptr)  // Aftertouch
        .addOption("Gt", 0b00001000, nullptr)  // Gate
        .addOption("Tr", 0b00001000, nullptr)  // Trigger
        .addOption("Rst", 0b00001000, nullptr) // Reset
        .addOption("-", 0b00001000, nullptr);  // Unassigned

    // Gate output 1
    this->addSubMenu("5", 0b00000100)
        .addOption("Gt", 0b00000100, nullptr)  // Gate
        .addOption("Tr", 0b00000100, nullptr)  // Trigger
        .addOption("Rst", 0b00000100, nullptr) // Reset
        .addOption("-", 0b00000100, nullptr);  // Unassigned
    // Gate output 2
    this->addSubMenu("6", 0b00000010)
        .addOption("Gt", 0b00000010, nullptr)  // Gate
        .addOption("Tr", 0b00000010, nullptr)  // Trigger
        .addOption("Rst", 0b00000010, nullptr) // Reset
        .addOption("-", 0b00000010, nullptr);  // Unassigned
    // Gate output 3
    this->addSubMenu("7", 0b00000001)
        .addOption("Gt", 0b00000001, nullptr)  // Gate
        .addOption("Tr", 0b00000001, nullptr)  // Trigger
        .addOption("Rst", 0b00000001, nullptr) // Reset
        .addOption("-", 0b00000001, nullptr);  // Unassigned
    // Gate output 4
    this->addSubMenu("8", 0b10000000)
        .addOption("Gt", 0b10000000, nullptr)  // Gate
        .addOption("Tr", 0b10000000, nullptr)  // Trigger
        .addOption("Rst", 0b10000000, nullptr) // Reset
        .addOption("-", 0b10000000, nullptr);  // Unassigned
  }
}

void CMenu::update() const
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

void CMenu::handleInput()
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
      _selectedIndex = 0;
    }
    else
    {
      CMenuOption *option = static_cast<CMenuOption *>(selectedItem);
      if (option->onSelectCallback)
        option->onSelectCallback();
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
      bActive = false; // Exit menu if back is pressed on root
  }

  _next = _prev = _back = _select = false;
}
