#pragma once

#include "Outputs.h"
#include "Settings.h"
#include "Gui.h"
#include "MidiOverSerial.h"
#include "MidiOverUsb.h"
#include <MIDI.h>
#include <Arduino.h>

class CMidiHandler
{
public:
  CMidiHandler(COutputs &outputs, CSettings &settings, CGui &gui);
  ~CMidiHandler() = default;

  /**
   * @brief Initializes the MIDI interfaces.
   */
  void begin();

  /**
   * @brief Reads MIDI packets from both USB and Serial interfaces and processes them.
   */
  void read();

  /**
   * @brief Learn mode to capture the next MIDI message and map it to a control
   * @param value Reference to store the learned MIDI value (note number or controller number)
   * @param cancel Reference to a volatile boolean that can be set to true to cancel the learn action
   * @return Returns true if a valid MIDI message was learned, false if cancelled
   */
  bool learn(uint8_t &value, volatile bool &cancel);

  /**
   * @brief Reads and flushes all MIDI packets from both interfaces without processing
   */
  void readAndFlush();

private:
  COutputs &mOutputs;
  CSettings &mSettings;
  CGui &mGui;

  CMidiSerial mMidiSerial;
  CMidiUsb mMidiUsb;

  uint32_t _clkCount = 0;

  void update(uint8_t channel, uint8_t type, byte data1, byte data2);
  void midiFlush();

  bool validateLearn(uint8_t channel, uint8_t type, byte data1, byte data2, uint8_t &learnValue);

  // MIDI data messages
  void midiNoteOff(byte note, byte velocity);
  void midiNoteOn(byte note, byte velocity);
  void midiControlChange(byte subchannel, byte value);
  void midiAfterTouchChannel(byte velocity);
  void midiPitchBend(int value);

  // MIDI system messages
  void systemClock();
  void systemStart();
  void systemStop();
  void systemReset();
};