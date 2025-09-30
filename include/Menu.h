#pragma once

#include "Gui.h"

#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

// Standard menu item
class CMenuItem
{
public:
  explicit CMenuItem(const std::string &name, uint8_t led);
  virtual ~CMenuItem() = default;

  const std::string &getName() const;
  uint8_t getLed() const;
  virtual bool isSubMenu() const = 0;

protected:
  std::string _name;
  uint8_t _led;
};

// Menu item that conains other menu items
class CSubMenu : public CMenuItem
{
public:
  explicit CSubMenu(const std::string &name, uint8_t led);
  bool isSubMenu() const override;

  CSubMenu &addOption(const std::string &name, uint8_t led, std::function<void()> onSelect);
  CSubMenu &addSubMenu(const std::string &name, uint8_t led);

  const std::vector<std::unique_ptr<CMenuItem>> &getItems() const;

private:
  std::vector<std::unique_ptr<CMenuItem>> _items;
};

// Menu item that executes a function when selected
class CMenuOption : public CMenuItem
{
public:
  CMenuOption(const std::string &name, uint8_t led, std::function<void()> onSelect);
  std::function<void()> onSelectCallback;
  bool isSubMenu() const override;
};

// Main menu class to manage the menu system
class CMenu : public CSubMenu
{
public:
  CMenu(const std::string &name, CGui &gui);
  ~CMenu() = default;

  void build();

  void update() const;
  void waitForInput(volatile bool &next, volatile bool &prev, volatile bool &press);
  void handleInput();

  bool bActive = false; // Flag to indicate if the menu is active

private:
  CGui &mGui;
  CSubMenu *_currentMenu;
  int _selectedIndex;
  std::stack<std::pair<CSubMenu *, int>> _navigationStack;

  bool _next = false;
  bool _prev = false;
  bool _back = false;
  bool _select = false;
};