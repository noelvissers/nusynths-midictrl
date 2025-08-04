#include <Arduino.h>
#include <MIDI.h> // Included for enums
#include <MIDIUSB.h>

#include "MidiUsbIn.h"
#include "MidiHandler.h"

CMidiHandler midiHandlerUsb;

void CMidiUsb::init()
{
}

void CMidiUsb::update()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
    if (rx.header != 0)
    {
      switch (rx.byte1)
      {
      case midi::InvalidType:
        // Ignore
        break;
      case midi::NoteOff:
        midiHandlerUsb.midiNoteOff((rx.byte1 & 0x0F), rx.byte2, rx.byte3);
        break;
      case midi::NoteOn:
        midiHandlerUsb.midiNoteOn((rx.byte1 & 0x0F), rx.byte2, rx.byte3);
        break;
      case midi::AfterTouchPoly:
        // Not supported
        break;
      case midi::ControlChange:
        midiHandlerUsb.midiControlChange((rx.byte1 & 0x0F), rx.byte2, rx.byte3);
        break;
      case midi::ProgramChange:
        // Not supported
        break;
      case midi::AfterTouchChannel:
        midiHandlerUsb.midiAfterTouchChannel((rx.byte1 & 0x0F), rx.byte2);
        break;
      case midi::PitchBend:
        midiHandlerUsb.midiPitchBend((rx.byte1 & 0x0F), ((rx.byte3 << 7) + rx.byte2) - 8192);
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
        midiHandlerUsb.systemClock();
        break;
      case midi::Start:
      case midi::Continue:
        midiHandlerUsb.systemStart();
        break;
      case midi::Stop:
        midiHandlerUsb.systemStop();
        break;
      case midi::ActiveSensing:
        // Ignore
        break;
      case midi::SystemReset:
        midiHandlerUsb.systemReset();
        break;
      default:
        // Ignore
        break;
      }
    }
  } while (rx.header != 0);
}

// TODO: Implement learn functionality