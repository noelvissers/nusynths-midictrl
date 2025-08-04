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
  Sync,
  Pitch,
  Velocity,
  ContinuesController,
  AfterTouch,
  Gate,
  Trigger,
  StartStop,
  Unassigned
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
  void Example();

private:
};