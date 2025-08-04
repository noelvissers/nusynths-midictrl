#include <Arduino.h>
#include "MidiHandler.h"

void CMidiHandler::init()
{
}

// MIDI data messages
void CMidiHandler::midiNoteOff(byte note, byte velocity)
{
}

void CMidiHandler::midiNoteOn(byte note, byte velocity)
{
}

void CMidiHandler::midiControlChange(byte byte1, byte byte2)
{
}

void CMidiHandler::midiAfterTouchChannel(byte velocity)
{
}

void CMidiHandler::midiPitchBend(int value)
{
}

// MDII system messages
void CMidiHandler::systemClock()
{
}

void CMidiHandler::systemStart()
{
}

void CMidiHandler::systemStop()
{
}

void CMidiHandler::systemReset()
{
}

// TODO: Implement learn functionality