#include <Arduino.h>
#include "MidiHandler.h"
#include "Outputs.h"
#include "Config.h"

void CMidiHandler::init()
{
}

// MIDI data messages
void CMidiHandler::midiNoteOff(byte note, byte velocity)
{
	// Handle pitch
	// Handle velocity
	// Handle gate
	// Handle trigger
}

void CMidiHandler::midiNoteOn(byte note, byte velocity)
{
	COutputs outputs;

	size_t index = 0;
	for (auto &output : outputs.aOutputs)
	{
		if (!output.isActive)
		{
			switch (output.function)
			{
			case OutputFunction::Pitch:
				outputs.setOutputValue(index, note);
				output.isDirty = true;
				break;
			case OutputFunction::Velocity:
				outputs.setOutputValue(index, velocity);
				output.isDirty = true;
				break;
			case OutputFunction::Gate:
				outputs.setOutputValue(index, 1);
				output.isDirty = true;
				break;
			case OutputFunction::Trigger:
				if (!output.isMapped)
					outputs.setOutputValue(index, 1);
				else if (output.mappedNote == note)
					outputs.setOutputValue(index, 1);
				else
					break;
				output.isDirty = true;
				break;

			default:
				break;
			}

			// Exit after first active output in polyphonic mode
			// Note: If multiple triggers is mapped to the same note, only 1 will be triggered in polyphonic mode
			if (output.isDirty && _synthMode == SynthMode::Polyphonic)
				return;
		}
		index++;
	}
}

void CMidiHandler::midiControlChange(byte byte1, byte byte2)
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

// TODO: Implement learn functionality