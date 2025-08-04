#pragma once

#include <cstdint>

enum class OutputType
{
  Cv,
  Gate,
  Undefined,
};

enum class OutputFunction
{
  Sync = 1,
  Pitch = 2,
  Velocity = 3,
  ContinuesController = 4,
  AfterTouch = 5,
  Gate = 6,
  Trigger = 7,
  StartStop = 8,
  Unassigned = 0
};

struct Output
{
  const char *name;
  OutputType type;
  OutputFunction funciton;
  bool isActive;
  uint16_t value;
  unsigned long resetTime;
};

class COutputs
{
public:
  void update();
  void setOutputs();

private:
};