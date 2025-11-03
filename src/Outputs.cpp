#include "Outputs.h"
#include "Settings.h"
#include "HalConfiguration.h"
#include <Arduino.h>

COutputs::COutputs(CGui &gui)
    : mGui(gui), mDac(_spiSpeed, _pinDacEnable, _pinDacSync)
{
  // Default initialization
  SOutput defaultOutput;

  for (auto i = 0; i < N_OUTPUTS; ++i)
    mOutputs[i] = defaultOutput;

  // Output specific initialization
  mOutputs[0].type = EOutputType::Digital; // Sync output
  mOutputs[0].outputPin = _pinSync;
  pinMode(_pinSync, OUTPUT);

  mOutputs[1].type = EOutputType::Analog;
  mOutputs[1].dacChannel = DAC8564::Channel::A;
  mOutputs[1].ledMask = 0b01000000;
  mOutputs[2].type = EOutputType::Analog;
  mOutputs[2].dacChannel = DAC8564::Channel::B;
  mOutputs[2].ledMask = 0b00100000;
  mOutputs[3].type = EOutputType::Analog;
  mOutputs[3].dacChannel = DAC8564::Channel::C;
  mOutputs[3].ledMask = 0b00010000;
  mOutputs[4].type = EOutputType::Analog;
  mOutputs[4].dacChannel = DAC8564::Channel::D;
  mOutputs[4].ledMask = 0b00001000;

  mOutputs[5].type = EOutputType::Digital;
  mOutputs[5].outputPin = _pinGate1;
  mOutputs[5].ledMask = 0b00000100;
  pinMode(_pinGate1, OUTPUT);
  mOutputs[6].type = EOutputType::Digital;
  mOutputs[6].outputPin = _pinGate2;
  mOutputs[6].ledMask = 0b00000010;
  pinMode(_pinGate2, OUTPUT);
  mOutputs[7].type = EOutputType::Digital;
  mOutputs[7].outputPin = _pinGate3;
  mOutputs[7].ledMask = 0b00000001;
  pinMode(_pinGate3, OUTPUT);
  mOutputs[8].type = EOutputType::Digital;
  mOutputs[8].outputPin = _pinGate4;
  mOutputs[8].ledMask = 0b10000000;
  pinMode(_pinGate4, OUTPUT);
}

// Initialize outputs and DAC
void COutputs::init()
{
  // Digital outputs
  digitalWrite(_pinSync, LOW);
  digitalWrite(_pinGate1, LOW);
  digitalWrite(_pinGate2, LOW);
  digitalWrite(_pinGate3, LOW);
  digitalWrite(_pinGate4, LOW);

  pinMode(_pinDacLdac, OUTPUT);
  digitalWrite(_pinDacLdac, LOW); // Don't use hardware sync
  mDac.begin();
  mDac.setOperatingModeAll(DAC8564::OperatingMode::NORMAL_OPERATION);
}

void COutputs::update()
{
  for (auto &output : mOutputs)
  {
    // Handle reset for triggers
    if (output.isActive && (output.function == EOutputFunction::Trigger || output.function == EOutputFunction::Sync || output.function == EOutputFunction::Reset))
    {
      // Handle roll-overs
      if (micros() >= output.resetTime || micros() < (output.resetTime + (TRIGGER_LENGHT_MS * 1000)))
      {
        output.value = OUTPUT_LOW;
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
        mDac.write(DAC8564::Address::ADDR_0, DAC8564::Load::WRITE_N_LOAD_N, output.dacChannel, value);
        break;
      case EOutputType::Digital:
        digitalWrite(output.outputPin, output.value > 0 ? HIGH : LOW);
        break;
      default:
        break;
      }
      mGui.setOutputLed(output.ledMask, output.value > 0 ? true : false);
      output.isDirty = false;
    }
  }
}

void COutputs::setOutputs(const SSettings &settings)
{
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs[i];

    mOutputs[i].function = settings.outputFunctions[i];
    mOutputs[i].isMapped = settings.outputIsMapped[i];
    mOutputs[i].mappedTo = settings.outputMappedTo[i];

    setOutput(i, output);
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

uint16_t COutputs::midiTo1VOct(uint8_t byte)
{
  if (byte < 12 || byte > 127)
    return 0; // Out of range, return 0V

  // C0  = MIDI: 12  = 0V  = DAC: 0
  // C10 = MIDI: 132 = 10V = DAC: 65535
  return round(map(byte, 12, 132, 0, 65535));
}

uint16_t COutputs::midiToCv(uint8_t byte)
{
  return round(map(byte, 0, 127, 0, 65535));
}

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
}