#include "Menu.h"
#include <Arduino.h>

// MenuItem
CMenuItem::CMenuItem(const std::string &name) : _name(name) {}
const std::string &CMenuItem::getName() const
{
  return _name;
}

// SubMenu
CSubMenu::CSubMenu(const std::string &name) : CMenuItem(name) {}
bool CSubMenu::isSubMenu() const
{
  return true;
}

CSubMenu &CSubMenu::addOption(const std::string &name, std::function<void()> onSelect)
{
  _items.push_back(std::unique_ptr<CMenuItem>(new CMenuOption(name, onSelect)));
  return *this;
}

CSubMenu &CSubMenu::addSubMenu(const std::string &name)
{
  std::unique_ptr<CSubMenu> newSubMenu(new CSubMenu(name));
  CSubMenu *newSubMenuPtr = newSubMenu.get();
  _items.push_back(std::move(newSubMenu));
  return *newSubMenuPtr;
}

const std::vector<std::unique_ptr<CMenuItem>> &CSubMenu::getItems() const
{
  return _items;
}

// MenuOption
CMenuOption::CMenuOption(const std::string &name, std::function<void()> onSelect)
    : CMenuItem(name), onSelectCallback(onSelect) {}

bool CMenuOption::isSubMenu() const
{
  return false;
}

// Menu
CMenu::CMenu(const std::string &name) : CSubMenu(name), _currentMenu(this), _selectedIndex(0) {}

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
    CSubMenu &configMenu = this->addSubMenu("Cnf");
    {
      // MIDI channel
      configMenu.addSubMenu("Chn")
          .addOption("All", nullptr) // OMNI
          .addOption("1", nullptr)   // Channel 1
          .addOption("2", nullptr)   // Channel 2
          .addOption("3", nullptr)   // Channel 3
          .addOption("4", nullptr)   // Channel 4
          .addOption("5", nullptr)   // Channel 5
          .addOption("6", nullptr)   // Channel 6
          .addOption("7", nullptr)   // Channel 7
          .addOption("8", nullptr)   // Channel 8
          .addOption("9", nullptr)   // Channel 9
          .addOption("10", nullptr)  // Channel 10
          .addOption("11", nullptr)  // Channel 11
          .addOption("12", nullptr)  // Channel 12
          .addOption("13", nullptr)  // Channel 13
          .addOption("14", nullptr)  // Channel 14
          .addOption("15", nullptr)  // Channel 15
          .addOption("16", nullptr); // Channel 16
      // Mode
      configMenu.addSubMenu("Mod")
          .addOption("Mon", nullptr)  // Mono
          .addOption("Pol", nullptr); // Polyphonic
      // Pitch bend
      configMenu.addSubMenu("Pb.")
          .addOption("0", nullptr)   // 0 semitones
          .addOption("1", nullptr)   // 1 semitone
          .addOption("2", nullptr)   // 2 semitones
          .addOption("3", nullptr)   // 3 semitones
          .addOption("4", nullptr)   // 4 semitones
          .addOption("5", nullptr)   // 5 semitones
          .addOption("6", nullptr)   // 6 semitones
          .addOption("7", nullptr)   // 7 semitones
          .addOption("8", nullptr)   // 8 semitones
          .addOption("9", nullptr)   // 9 semitones
          .addOption("10", nullptr)  // 10 semitones
          .addOption("11", nullptr)  // 11 semitones
          .addOption("12", nullptr); // 12 semitones
      // Clock divider
      configMenu.addSubMenu("Clk")
          .addOption("1", nullptr)    // 1:1
          .addOption("2", nullptr)    // /2
          .addOption("4", nullptr)    // /4
          .addOption("8", nullptr)    // /8
          .addOption("16", nullptr)   // /16
          .addOption("32", nullptr)   // /32
          .addOption("64", nullptr)   // /64
          .addOption("128", nullptr)  // /128
          .addOption("256", nullptr); // /256
    }

    // Gate output 1
    this->addSubMenu("1")
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // Gate output 2
    this->addSubMenu("2")
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // Gate output 3
    this->addSubMenu("3")
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // Gate output 4
    this->addSubMenu("4")
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned

    // CV output 1 (5)
    this->addSubMenu("5")
        .addOption("Ptc", nullptr) // Pitch
        .addOption("VEL", nullptr) // Velocity
        .addOption("CC", nullptr)  // Continues controller
        .addOption("At", nullptr)  // Aftertouch
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // CV output 2 (6)
    this->addSubMenu("6")
        .addOption("Ptc", nullptr) // Pitch
        .addOption("VEL", nullptr) // Velocity
        .addOption("CC", nullptr)  // Continues controller
        .addOption("At", nullptr)  // Aftertouch
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // CV output 3 (7)
    this->addSubMenu("7")
        .addOption("Ptc", nullptr) // Pitch
        .addOption("VEL", nullptr) // Velocity
        .addOption("CC", nullptr)  // Continues controller
        .addOption("At", nullptr)  // Aftertouch
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
    // CV output 4 (8)
    this->addSubMenu("8")
        .addOption("Ptc", nullptr) // Pitch
        .addOption("VEL", nullptr) // Velocity
        .addOption("CC", nullptr)  // Continues controller
        .addOption("At", nullptr)  // Aftertouch
        .addOption("Gt", nullptr)  // Gate
        .addOption("Tr", nullptr)  // Trigger
        .addOption("Rst", nullptr) // Reset
        .addOption("-", nullptr);  // Unassigned
  }
}

void CMenu::update() const
{
  // TODO: Implement GUI here. Names can he handled in GUI functions to show which
  // menu is currently active. Specific functions can be mapped in initialization function.


  // TODO: Implement GUI function to print string or leds based on menu structure
  //            7seg, leds
  // gui.render(ABC,  0b00000001);
  
  // if (!_currentMenu) return;
  // std::cout << "--- " << _currentMenu->getName() << " ---\n";
  // const auto &items = _currentMenu->getItems();
  // for (int i = 0; i < items.size(); ++i) {
  //   if (i == _selectedIndex) {
  //     std::cout << "> "; // Indicator for selected item
  //   } else {
  //     std::cout << "  ";
  //   }
  //   std::cout << items[i]->getName();
  //   if (items[i]->isSubMenu()) {
  //     std::cout << " ->"; // Indicator for a submenu
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "---------------------\n";
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
