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
	COutputs outputs;

	for (auto &output : outputs._outputs)
	{
		if (output.isActive)
		{
			switch (output.function)
			{
			case OutputFunction::Pitch:
				if (output.value == note)
					output.isActive = false;
				break;
			case OutputFunction::Velocity:
				if (output.mappedNote == note)
					output.isActive = false;
				break;
			case OutputFunction::Gate:
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
		}
	}
}

void CMidiHandler::midiNoteOn(byte note, byte velocity)
{
	COutputs outputs;

	for (auto &output : outputs._outputs)
	{
		if (!output.isActive)
		{
			switch (output.function)
			{
			case OutputFunction::Pitch:
				output.value = outputs.midiTo1VOct(note);
				output.mappedNote = note; // Use mappedNote for checking what note triggered this output
				output.isActive = output.isDirty = true;
				break;
			case OutputFunction::Velocity:
				output.value = outputs.midiToCv(velocity);
				output.mappedNote = note; // Use mappedNote for checking what note triggered this output
				output.isActive = output.isDirty = true;
				break;
			case OutputFunction::Gate:
				output.value = 32768;			// 5V for DAC, HIGH for IO
				output.mappedNote = note; // Use mappedNote for checking what note triggered this output
				output.isActive = output.isDirty = true;
				break;
			case OutputFunction::Trigger:
				if (!output.isMapped)
				{
					output.value = 32768;			// 5V for DAC, HIGH for IO
					output.resetTime = micros() + (_triggerLengthMs * 1000);
					output.mappedNote = note; // Use mappedNote for checking what note triggered this output
					output.isActive = true;
				}
				else if (output.mappedNote == note)
				{
					output.value = 32768;			// 5V for DAC, HIGH for IO
					output.resetTime = micros() + (_triggerLengthMs * 1000);
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

			// Exit after first active output in polyphonic mode
			// Note: If multiple triggers is mapped to the same note, only 1 will be triggered in polyphonic mode
			if (output.isDirty && _synthMode == SynthMode::Polyphonic)
				return;
		}
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