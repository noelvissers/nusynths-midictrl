#include <Arduino.h>
#include "MidiHandler.h"

void CMidiHandler::init()
{
}

// MIDI data messages
void CMidiHandler::midiNoteOff(byte channel, byte note, byte velocity)
{
}

void CMidiHandler::midiNoteOn(byte channel, byte note, byte velocity)
{
}

void CMidiHandler::midiControlChange(byte channel, byte byte1, byte byte2)
{
}

void CMidiHandler::midiAfterTouchChannel(byte channel, byte velocity)
{
}

void CMidiHandler::midiPitchBend(byte channel, int value)
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