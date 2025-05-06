#include <Arduino.h>
#include "config.h"

// --------------------------------------------------
// Public
// --------------------------------------------------

// LED
const uint8_t _pinLed = LED_BUILTIN;

// Rotary encoder
const uint8_t _pinRotaryButton = 0;
const uint8_t _pinRotaryClk = 0;
const uint8_t _pinRotaryData = 0;

volatile bool _flagRotaryButton = false;
volatile bool _flagRotaryCw = false;
volatile bool _flagRotaryCcw = false;

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

void CConfig::Example()
{
    
}

// --------------------------------------------------
// Private
// --------------------------------------------------