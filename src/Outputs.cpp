#include "Outputs.h"
#include "HalConfiguration.h"
#include "Settings.h"
#include <Arduino.h>

COutputs::COutputs()
{
  // Default initialization
  SOutput defaultOutput;

  for (auto i = 0; i < N_OUTPUTS; ++i)
    mOutputs[i] = defaultOutput;

  // Output specific initialization
  mOutputs[0].type = EOutputType::Digital; // Sync output
  mOutputs[0].outputPin = _pinSync;

  mOutputs[1].type = EOutputType::Analog;
  mOutputs[1].dacChannel = _dacChannelCv1;
  mOutputs[2].type = EOutputType::Analog;
  mOutputs[2].dacChannel = _dacChannelCv2;
  mOutputs[3].type = EOutputType::Analog;
  mOutputs[3].dacChannel = _dacChannelCv3;
  mOutputs[4].type = EOutputType::Analog;
  mOutputs[4].dacChannel = _dacChannelCv4;

  mOutputs[5].type = EOutputType::Digital;
  mOutputs[5].outputPin = _pinGate1;
  mOutputs[6].type = EOutputType::Digital;
  mOutputs[6].outputPin = _pinGate2;
  mOutputs[7].type = EOutputType::Digital;
  mOutputs[7].outputPin = _pinGate3;
  mOutputs[8].type = EOutputType::Digital;
  mOutputs[8].outputPin = _pinGate4;
}

// Write the actual values to either the DAC or IO pins
void COutputs::update()
{
  for (auto &output : mOutputs)
  {
    // Handle reset for triggers
    if (output.function == EOutputFunction::Trigger && output.isActive == true)
    {
      // Handle roll-overs
      if (micros() >= output.resetTime || micros() < (output.resetTime + (TRIGGER_LENGHT_MS * 1000)))
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
      case EOutputType::Analog:
        uint16_t value;
        if (output.function == EOutputFunction::Pitch)
          value = getPitch(output.value, output.pitchBend);
        else
          value = output.value;
        // TODO: dacWrite(output.dacChannel, value);
        break;
      case EOutputType::Digital:
        digitalWrite(output.outputPin, output.value > 0 ? HIGH : LOW);
        break;
      default:
        break;
      }
      output.isDirty = false;
    }
  }
}

void COutputs::setOutput(uint16_t index, const SOutput &outputConfig)
{
  if (index < mOutputs.size())
    mOutputs[index] = outputConfig;
}

SOutput COutputs::getOutput(uint16_t index) const
{
  SOutput output;

  if (index < mOutputs.size())
    output = mOutputs[index];
  return output;
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

// Map semitones to CV value
long COutputs::pitchBendToCv(int pitchBend, uint8_t semitones)
{
  // 'pitchBend' is a 14 bit value, ranging from -8192 to 8191 with the center point being 0.
  // 'semitones' is the range of pitch bend (12 semitones = 1 oct = 1V)

  long maxValueDac = map(semitones, 0, 120, 0, 65535);
  return round(map(pitchBend, -8192, 8191, -maxValueDac, maxValueDac));
}

uint16_t COutputs::getPitch(uint16_t pitch, long pitchBend)
{
  long totalPitch = static_cast<long>(pitch) + pitchBend;

  if (pitch < 0)
    return 0;
  if (pitch > 65535)
    return 65535;
  return static_cast<uint16_t>(totalPitch);
  ;
}