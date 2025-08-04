#include <Arduino.h>
#include <MIDI.h>
#include "MidiSerial.h"
#include "MidiHandler.h"

CMidiHandler midiHandlerSerial;

MIDI_CREATE_DEFAULT_INSTANCE();

void CMidiSerial::init()
{
  // TODO: Check if this should be loaded from settings or if filtered manually
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void CMidiSerial::update()
{
  if (MIDI.read())
  {
    switch (MIDI.getType())
    {
      case midi::InvalidType:
        // Ignore
        break;
      case midi::NoteOff:
        midiHandlerSerial.midiNoteOff(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOn:
        midiHandlerSerial.midiNoteOn(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::AfterTouchPoly:
        // Not supported
        break;
      case midi::ControlChange:
        midiHandlerSerial.midiControlChange(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::ProgramChange:
        // Not supported
        break;
      case midi::AfterTouchChannel:
        midiHandlerSerial.midiAfterTouchChannel(MIDI.getChannel(), MIDI.getData2());
        break;
      case midi::PitchBend:
        midiHandlerSerial.midiPitchBend(MIDI.getChannel(), MIDI.getData2() | (MIDI.getData1() << 7) - 8192);
        break;
      case midi::SystemExclusive:
        // Ignore
        break;
      case midi::TimeCodeQuarterFrame:
        // Ignore
        break;
      case midi::SongPosition:
        // Ignore
        break;
      case midi::SongSelect:
        // Ignore
        break;
      case midi::TuneRequest:
        // Ignore
        break;
      case midi::SystemExclusiveEnd:
        // Ignore
        break;
      case midi::Clock:
        midiHandlerSerial.systemClock();
        break;
      case midi::Start:
      case midi::Continue:
        midiHandlerSerial.systemStart();
        break;
      case midi::Stop:
        midiHandlerSerial.systemStop();
        break;
      case midi::ActiveSensing:
        // Ignore
        break;
      case midi::SystemReset:
        break;
      default:
        // Ignore
        break;
    }
  }
}

// TODO: Implement learn functionality