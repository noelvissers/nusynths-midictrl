#include <Arduino.h>
#include "Outputs.h"
#include "Config.h"

// Initialize outputs with default values
void COutputs::init()
{
  // Output specific initialization
  _outputs[0].type = OutputType::Digital; // Sync output
  _outputs[0].outputPin = _pinSync;

  _outputs[1].type = OutputType::Analog;
  _outputs[1].dacChannel = _dacChannelCv1;
  _outputs[2].type = OutputType::Analog;
  _outputs[2].dacChannel = _dacChannelCv2;
  _outputs[3].type = OutputType::Analog;
  _outputs[3].dacChannel = _dacChannelCv3;
  _outputs[4].type = OutputType::Analog;
  _outputs[4].dacChannel = _dacChannelCv4;

  _outputs[5].type = OutputType::Digital;
  _outputs[5].outputPin = _pinGate1;
  _outputs[6].type = OutputType::Digital;
  _outputs[6].outputPin = _pinGate2;
  _outputs[7].type = OutputType::Digital;
  _outputs[7].outputPin = _pinGate3;
  _outputs[8].type = OutputType::Digital;
  _outputs[8].outputPin = _pinGate4;
}

// Write the actual values to either the DAC or IO pins
void COutputs::update()
{
  for (auto &output : _outputs)
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
// TODO: seperate function for this is a bit overkill, might delete later
void COutputs::setOutputConfig(uint8_t output, OutputFunction function)
{
  if (output < _outputs.size())
    _outputs[output].function = function;
}

// Map MIDI note to 1V/Oct value
uint16_t COutputs::midiTo1VOct(uint8_t byte)
{
  if (byte < 12 || byte > 127)
    return 0; // Out of range, return 0V

  // C0  = MIDI: 12  = 0V  = DAC: 0
  // C10 = MIDI: 132 = 10V = DAC: 65535
  return round(map(byte, 12, 132, 0, 65535));
}

// Map MIDI data to CV value
uint16_t COutputs::midiToCv(uint8_t byte)
{
  return round(map(byte, 0, 127, 0, 65535));
}