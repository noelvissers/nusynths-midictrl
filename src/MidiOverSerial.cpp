#include "MidiOverSerial.h"

#include "HalConfiguration.h"
#include "wiring_private.h"
#include <MIDI.h>

// Custom UART on SERCOM5:
Uart SerialMidi(&sercom5, _pinMidiRx, _pinMidiTx, SERCOM_RX_PAD_0, UART_TX_PAD_2);

// Create a MIDI interface bound to SerialMidi
MIDI_CREATE_INSTANCE(HardwareSerial, SerialMidi, MIDI);

void SERCOM5_Handler() { SerialMidi.IrqHandler(); }

CMidiSerial::CMidiSerial() {}

void CMidiSerial::begin()
{
  // Route the pins to SERCOM ALT function
  pinPeripheral(0, PIO_SERCOM_ALT);  // D0 (PA22) as SERCOM5 PAD[0]
  pinPeripheral(14, PIO_SERCOM_ALT); // D14 (PB22) as SERCOM5 PAD[2]

  SerialMidi.begin(31250);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

bool CMidiSerial::getPacket(SMidiSerialPacket &midiEventPacket)
{
  if (MIDI.read())
  {
    midiEventPacket.channel = MIDI.getChannel();
    midiEventPacket.type = MIDI.getType();
    midiEventPacket.dataByte1 = MIDI.getData1();
    midiEventPacket.dataByte2 = MIDI.getData2();
    return true;
  }
  return false;
}

void CMidiSerial::flush()
{
  while (MIDI.read())
    ;
}