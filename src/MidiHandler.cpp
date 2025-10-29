#include "MidiHandler.h"
#include "HalConfiguration.h"

Uart SerialMidi(&sercom5, _pinMidiRx, _pinMidiTx, SERCOM_RX_PAD_0, UART_TX_PAD_2);

void SERCOM5_Handler() {
  SerialMidi.IrqHandler();
}

CMidiHandler::CMidiHandler(COutputs &outputs, CSettings &settings, CGui &gui)
    : mOutputs(outputs), mSettings(settings), mGui(gui) {
      MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, SerialMidi, MIDI, midi::DefaultSettings);
    }

void CMidiHandler::read()
{
  // Read MIDI over serial
  SMidiSerialPacket midiSerialPacket;
  if (mMidiSerial.getPacket(midiSerialPacket))
    update(midiSerialPacket.channel, midiSerialPacket.type, midiSerialPacket.dataByte1, midiSerialPacket.dataByte2);

  // Read MIDI over USB
  SMidiUsbPacket midiUsbPacket;
  if (mMidiUsb.getPacket(midiUsbPacket))
    update(midiUsbPacket.channel, midiUsbPacket.type, midiUsbPacket.dataByte1, midiUsbPacket.dataByte2);
}

bool CMidiHandler::learn(uint8_t &value, volatile bool &cancel)
{
  SMidiSerialPacket midiSerialPacket;
  SMidiUsbPacket midiUsbPacket;

  mGui.setString("Lrn.");
  bool ledState = true;
  unsigned long ledStart = millis();

  // Clear midi buffers
  midiFlush();

  do
  {
    // Blink LED
    unsigned long currentMillis = millis();
    if (currentMillis - ledStart >= 500)
    {
      ledStart = currentMillis;
      if (ledState)
        mGui.setString("Lrn");
      else
        mGui.setString("Lrn.");
      ledState = !ledState;
    }

    if (mMidiSerial.getPacket(midiSerialPacket))
    {
      if (validateLearn(midiSerialPacket.channel, midiSerialPacket.type, midiSerialPacket.dataByte1, midiSerialPacket.dataByte2, value))
        return true;
    }
    if (mMidiUsb.getPacket(midiUsbPacket))
    {
      if (validateLearn(midiUsbPacket.channel, midiUsbPacket.type, midiUsbPacket.dataByte1, midiUsbPacket.dataByte2, value))
        return true;
    }
  } while (!cancel);
  return false;
}

void CMidiHandler::update(uint8_t channel, uint8_t type, byte data1, byte data2)
{
  if (channel == 0 || channel == mSettings.get().midiChannel)
  {
    switch (type)
    {
    case midi::NoteOff:
      midiNoteOff(data1, data2);
      break;
    case midi::NoteOn:
      midiNoteOn(data1, data2);
      break;
    case midi::ControlChange:
      midiControlChange(data1, data2);
      break;
    case midi::AfterTouchChannel:
      midiAfterTouchChannel(data2);
      break;
    case midi::PitchBend:
      midiPitchBend(((data2 << 7) + data1) - 8192);
      break;
    case midi::Clock:
      systemClock();
      break;
    case midi::Start:
    case midi::Continue:
      systemStart();
      break;
    case midi::Stop:
      systemStop();
      break;
    case midi::SystemReset:
      systemReset();
      break;
    case midi::InvalidType:
    case midi::AfterTouchPoly:
    case midi::ProgramChange:
    case midi::SystemExclusive:
    case midi::TimeCodeQuarterFrame:
    case midi::SongPosition:
    case midi::SongSelect:
    case midi::TuneRequest:
    case midi::SystemExclusiveEnd:
    case midi::ActiveSensing:
      // Ignore / Not supported
      break;
    default:
      // Ignore
      break;
    }
  }
}

void CMidiHandler::midiFlush()
{
  mMidiSerial.flush();
  mMidiUsb.flush();
}

bool CMidiHandler::validateLearn(uint8_t channel, uint8_t type, byte data1, byte data2, uint8_t &learnValue)
{
  if (channel == 0 || channel == mSettings.get().midiChannel)
  {
    switch (type)
    {
    case midi::NoteOn:
      // Learn the note value
      learnValue = data1;
      return true;
    case midi::ControlChange:
      // Learn the subchannel value
      learnValue = data1;
      return true;
    case midi::SystemReset:
      systemReset();
      break;
    case midi::InvalidType:
    case midi::NoteOff:
    case midi::AfterTouchPoly:
    case midi::ProgramChange:
    case midi::AfterTouchChannel:
    case midi::PitchBend:
    case midi::SystemExclusive:
    case midi::TimeCodeQuarterFrame:
    case midi::SongPosition:
    case midi::SongSelect:
    case midi::TuneRequest:
    case midi::SystemExclusiveEnd:
    case midi::Clock:
    case midi::Start:
    case midi::Continue:
    case midi::Stop:
    case midi::ActiveSensing:
      // Ignore
      break;
    default:
      // Ignore
      break;
    }
  }
  return false;
}

// MIDI data messages
void CMidiHandler::midiNoteOff(byte note, byte velocity)
{
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (output.isActive)
    {
      switch (output.function)
      {
      case EOutputFunction::Pitch:
      case EOutputFunction::Velocity:
        if (output.mappedTo == note)
          output.isActive = false;
        break;
      case EOutputFunction::Gate:
        if (output.mappedTo == note)
        {
          output.value = OUTPUT_LOW;
          output.isActive = false;
          output.isDirty = true;
        }
        break;
      default:
        break;
      }
      mOutputs.setOutput(i, output);
    }
  }
}

void CMidiHandler::midiNoteOn(byte note, byte velocity)
{
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (!output.isActive)
    {
      switch (output.function)
      {
      case EOutputFunction::Pitch:
        output.value = mOutputs.midiTo1VOct(note);
        output.mappedTo = note; // Use mappedTo for checking what note triggered this output
        if (mSettings.get().synthMode == ESynthMode::Monophonic)
          output.isActive = false; // Don't set to active in monophonic mode, so it is overwritten by the next note
        output.isDirty = true;
        break;
      case EOutputFunction::Velocity:
        output.value = mOutputs.midiToCv(velocity);
        output.mappedTo = note; // Use mappedTo for checking what note triggered this output
        if (mSettings.get().synthMode == ESynthMode::Monophonic)
          output.isActive = false; // Don't set to active in monophonic mode, so it is overwritten by the next note
        output.isDirty = true;
        break;
      case EOutputFunction::Gate:
        output.value = OUTPUT_HIGH;
        output.mappedTo = note; // Use mappedTo for checking what note triggered this output
        output.isActive = output.isDirty = true;
        break;
      case EOutputFunction::Trigger:
        if (!output.isMapped)
        {
          output.value = OUTPUT_HIGH;
          output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
          output.isActive = true;
        }
        else if (output.mappedTo == note)
        {
          output.value = OUTPUT_HIGH;
          output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
          output.isActive = true;
        }
        else
          break;
        output.isDirty = true;
        break;

      default:
        break;
      }
      mOutputs.setOutput(i, output);

      // Exit after first active output in polyphonic mode
      // Note: If multiple triggers are mapped to the same note, only 1 will be triggered in polyphonic mode
      if (output.isDirty && mSettings.get().synthMode == ESynthMode::Polyphonic)
        return;
    }
  }
}

void CMidiHandler::midiControlChange(byte subchannel, byte value)
{
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (output.function == EOutputFunction::ContinuesController && output.mappedTo == subchannel)
    {
      output.value = mOutputs.midiToCv(value);
      output.isDirty = true;
      mOutputs.setOutput(i, output);
    }
  }
}

void CMidiHandler::midiAfterTouchChannel(byte velocity)
{
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (output.function == EOutputFunction::AfterTouch)
    {
      output.value = mOutputs.midiToCv(velocity);
      output.isDirty = true;
      mOutputs.setOutput(i, output);
    }
  }
}

void CMidiHandler::midiPitchBend(int value)
{
  // 'value' is a 14 bit value, ranging from -8192 to 8191 with the center point being 0
  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (output.function == EOutputFunction::Pitch)
    {
      output.pitchBend = mOutputs.pitchBendToCv(value, mSettings.get().pitchBendSemitones);
      output.isDirty = true;
      mOutputs.setOutput(i, output);
    }
  }
}

// MIDI system messages
void CMidiHandler::systemClock()
{
  if ((_clkCount % mSettings.get().clockDiv) == 0)
  {
    SOutput output = mOutputs.getOutput(0);
    output.value = OUTPUT_HIGH;
    output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
    output.isActive = output.isDirty = true;
    mOutputs.setOutput(0, output);
  }
  _clkCount++;
}

void CMidiHandler::systemStart()
{
  // Reset clock count. Rest is handled in systemClock()
  _clkCount = 0;
}

void CMidiHandler::systemStop()
{
  // Reset clock count
  _clkCount = 0;

  for (auto i = 0; i < N_OUTPUTS; i++)
  {
    SOutput output = mOutputs.getOutput(i);

    if (output.function == EOutputFunction::Reset)
    {
      output.value = OUTPUT_HIGH;
      output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
      output.isActive = output.isDirty = true;
      mOutputs.setOutput(i, output);
    }
  }
}

void CMidiHandler::systemReset()
{
  NVIC_SystemReset();
}
