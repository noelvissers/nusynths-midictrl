#include "HalConfiguration.h"
#include <Arduino.h>
#include <cstdint>

// LED
const uint8_t _pinLed = LED_BUILTIN;

// Rotary encoder
const uint8_t _pinRotaryEncButton = 0;
const uint8_t _pinRotaryEncClk = 0;
const uint8_t _pinRotaryEncData = 0;

// Rotary encoder port and bit masks for faster IO read/writes
const uint8_t _maskRotaryEncButton = digitalPinToBitMask(_pinRotaryEncButton);
volatile uint32_t *_portRotaryEncButton = portInputRegister(digitalPinToPort(_pinRotaryEncButton));
const uint8_t _maskRotaryEncClk = digitalPinToBitMask(_pinRotaryEncClk);
volatile uint32_t *_portRotaryEncClk = portInputRegister(digitalPinToPort(_pinRotaryEncClk));
const uint8_t _maskRotaryEncData = digitalPinToBitMask(_pinRotaryEncData);
volatile uint32_t *_portRotaryEncData = portInputRegister(digitalPinToPort(_pinRotaryEncData));

// Rotary encoder flags
volatile bool _flagRotaryEncButton = false;
volatile bool _flagRotaryEncCw = false;
volatile bool _flagRotaryEncCcw = false;

// CV [DAC]
const uint8_t _dacChannelCv1 = 0;
const uint8_t _dacChannelCv2 = 0;
const uint8_t _dacChannelCv3 = 0;
const uint8_t _dacChannelCv4 = 0;

// Gates [5V DIO]
const uint8_t _pinGate1 = 0;
const uint8_t _pinGate2 = 0;
const uint8_t _pinGate3 = 0;
const uint8_t _pinGate4 = 0;

// Sync [5V DIO]
const uint8_t _pinSync = 0;

// SPI
const uint8_t _pinSpiMOSI = PIN_SPI_MOSI;
const uint8_t _pinSpiMISO = PIN_SPI_MISO;
const uint8_t _pinSpiCLK = PIN_SPI_SCK;

const uint8_t _pinCsDisplay = 0;
const uint8_t _pinCsDac = 0;