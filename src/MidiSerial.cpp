#include <Arduino.h>
#include <MIDI.h>
#include "MidiSerial.h"
#include "MidiHandler.h"

CMidiHandler midiHandlerSerial;

MIDI_CREATE_DEFAULT_INSTANCE();

void CMidiSerial::init()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void CMidiSerial::update()
{
  if (MIDI.read())
  {
      // TODO: actually filter midi channel here
    if (MIDI.getChannel() == 1)
    {

      switch (MIDI.getType())
      {
      case midi::InvalidType:
        // Ignore
        break;
      case midi::NoteOff:
        midiHandlerSerial.midiNoteOff(MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOn:
        midiHandlerSerial.midiNoteOn(MIDI.getData1(), MIDI.getData2());
        break;
      case midi::AfterTouchPoly:
        // Not supported
        break;
      case midi::ControlChange:
        midiHandlerSerial.midiControlChange(MIDI.getData1(), MIDI.getData2());
        break;
      case midi::ProgramChange:
        // Not supported
        break;
      case midi::AfterTouchChannel:
        midiHandlerSerial.midiAfterTouchChannel(MIDI.getData2());
        break;
      case midi::PitchBend:
        midiHandlerSerial.midiPitchBend(((MIDI.getData2() << 7) + MIDI.getData1()) - 8192);
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
        midiHandlerSerial.systemReset();
        break;
      default:
        // Ignore
        break;
      }
    }
  }
}

// TODO: Implement learn functionality