#include "MidiOverSerial.h"

#include <Arduino.h>
#include <MIDI.h>
#include "wiring_private.h"

// Serial1 pin and pad definitions
#define PIN_SERIAL1_RX (0ul)  // PA22 SERCOM5 (alt) PAD[0]
#define PIN_SERIAL1_TX (14ul) // PB22 SERCOM5 (alt) PAD[2]
#define PAD_SERIAL1_TX (UART_TX_PAD_2)
#define PAD_SERIAL1_RX (SERCOM_RX_PAD_0)

// Custom UART on SERCOM5:
Uart Serial1(&sercom5, PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

CMidiSerial::CMidiSerial() {}

void CMidiSerial::begin()
{
  Serial1.begin(31250);
  pinPeripheral(PIN_SERIAL1_RX, PIO_SERCOM_ALT);
  pinPeripheral(PIN_SERIAL1_TX, PIO_SERCOM_ALT);

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

void SERCOM5_Handler() // Interrupt handler for SERCOM5
{
  Serial1.IrqHandler();
}