#include <Arduino.h>
#include "Outputs.h"
#include "Config.h"

// Initialize outputs with default values
void COutputs::init()
{
  // Output specific initialization
  aOutputs[0].type = OutputType::Digital; // Sync output
  aOutputs[0].outputPin = _pinSync;

  aOutputs[1].type = OutputType::Analog;
  aOutputs[1].dacChannel = _dacChannelCv1;
  aOutputs[2].type = OutputType::Analog;
  aOutputs[2].dacChannel = _dacChannelCv2;
  aOutputs[3].type = OutputType::Analog;
  aOutputs[3].dacChannel = _dacChannelCv3;
  aOutputs[4].type = OutputType::Analog;
  aOutputs[4].dacChannel = _dacChannelCv4;

  aOutputs[5].type = OutputType::Digital;
  aOutputs[5].outputPin = _pinGate1;
  aOutputs[6].type = OutputType::Digital;
  aOutputs[6].outputPin = _pinGate2;
  aOutputs[7].type = OutputType::Digital;
  aOutputs[7].outputPin = _pinGate3;
  aOutputs[8].type = OutputType::Digital;
  aOutputs[8].outputPin = _pinGate4;
}

// Write the actual values to either the DAC or IO pins
void COutputs::update()
{
  for (auto &output : aOutputs)
  {
    // Handle reset for triggers
    if (output.function == OutputFunction::Trigger && output.isActive == true)
    {
      // Handle roll-overs
      if (micros() >= output.resetTime || micros() < (output.resetTime + (_triggerLengthMs * 1000)))
      {
        output.value = 0;
        output.isActive = false;
        output.isDirty = true;
      }
    }

    if (output.isDirty)
    {
      switch (output.type)
      {
      case OutputType::Analog:
        // TODO: dacWrite(output.outputAdc, output.value);
        break;
      case OutputType::Digital:
        digitalWrite(output.outputPin, output.value > 0 ? HIGH : LOW);
        break;
      default:
        break;
      }
      output.isDirty = false;
    }
  }
}

// Change the config of a specific output
void COutputs::setOutputConfig(uint8_t output, OutputFunction function)
{
  if (output < aOutputs.size())
    aOutputs[output].function = function;
}

// Change the value of a specific output
void COutputs::setOutputValue(uint8_t output, uint16_t value, bool isActive)
{
  if (output < aOutputs.size())
  {
    aOutputs[output].value = value;
    aOutputs[output].isActive = isActive;

    // Set the reset time for triggers
    if (aOutputs[output].function == OutputFunction::Trigger)
      aOutputs[output].resetTime = micros() + (_triggerLengthMs * 1000); 
    
    aOutputs[output].isDirty = true; // Mark the output as dirty to update it
  }
}