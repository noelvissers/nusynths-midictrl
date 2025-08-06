#include "MidiHandler.h"

CMidiHandler::CMidiHandler(COutputs &outputs, CSettings &settings)
    : mOutputs(outputs), mSettings(settings) {}

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

bool CMidiHandler::learn(uint8_t &note, byte &ccValue, volatile bool &cancel)
{
  // TODO: implement learn
  // returns true if learned, returns false if cancelled
  return false;
}

void CMidiHandler::update(uint8_t channel, uint8_t type, byte data1, byte data2)
{
  if (channel == 0 || channel == mSettings.getSettings().midiChannel)
  {
    switch (type)
    {
    case midi::InvalidType:
      // Ignore
      break;
    case midi::NoteOff:
      midiNoteOff(data1, data2);
      break;
    case midi::NoteOn:
      midiNoteOn(data1, data2);
      break;
    case midi::AfterTouchPoly:
      // Not supported
      break;
    case midi::ControlChange:
      midiControlChange(data1, data2);
      break;
    case midi::ProgramChange:
      // Not supported
      break;
    case midi::AfterTouchChannel:
      midiAfterTouchChannel(data2);
      break;
    case midi::PitchBend:
      midiPitchBend(((data2 << 7) + data1) - 8192);
      break;
    case midi::SystemExclusive:
    case midi::TimeCodeQuarterFrame:
    case midi::SongPosition:
    case midi::SongSelect:
    case midi::TuneRequest:
    case midi::SystemExclusiveEnd:
      // Ignore
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
    case midi::ActiveSensing:
      // Ignore
      break;
    case midi::SystemReset:
      systemReset();
      break;
    default:
      // Ignore
      break;
    }
  }
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
        if (output.value == note)
          output.isActive = false;
        break;
      case EOutputFunction::Velocity:
        if (output.mappedNote == note)
          output.isActive = false;
        break;
      case EOutputFunction::Gate:
        if (output.mappedNote == note)
        {
          output.value = 0;
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
        output.mappedNote = note; // Use mappedNote for checking what note triggered this output
        output.isActive = output.isDirty = true;
        break;
      case EOutputFunction::Velocity:
        output.value = mOutputs.midiToCv(velocity);
        output.mappedNote = note; // Use mappedNote for checking what note triggered this output
        output.isActive = output.isDirty = true;
        break;
      case EOutputFunction::Gate:
        output.value = 32768;     // 5V for DAC, HIGH for IO
        output.mappedNote = note; // Use mappedNote for checking what note triggered this output
        output.isActive = output.isDirty = true;
        break;
      case EOutputFunction::Trigger:
        if (!output.isMapped)
        {
          output.value = 32768; // 5V for DAC, HIGH for IO
          output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
          output.mappedNote = note; // Use mappedNote for checking what note triggered this output
          output.isActive = true;
        }
        else if (output.mappedNote == note)
        {
          output.value = 32768; // 5V for DAC, HIGH for IO
          output.resetTime = micros() + (TRIGGER_LENGHT_MS * 1000);
          output.mappedNote = note; // Use mappedNote for checking what note triggered this output
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
      // Note: If multiple triggers is mapped to the same note, only 1 will be triggered in polyphonic mode
      if (output.isDirty && mSettings.getSettings().synthMode == ESynthMode::Polyphonic)
        return;
    }
  }
}

void CMidiHandler::midiControlChange(byte subchannel, byte value)
{
  // Handle continuous controller
}

void CMidiHandler::midiAfterTouchChannel(byte velocity)
{
  // Handle aftertouch
}

void CMidiHandler::midiPitchBend(int value)
{
  // Handle pitch
}

// MIDI system messages
void CMidiHandler::systemClock()
{
  // Handle clock/sync
}

void CMidiHandler::systemStart()
{
  // Handle start
}

void CMidiHandler::systemStop()
{
  // Handle stop
}

void CMidiHandler::systemReset()
{
  // Implement?
}
