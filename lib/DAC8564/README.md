# DAC8564 Arduino Library

An **Arduino-compatible driver** for the **Texas Instruments DAC8564**  
— a 16-bit, quad-channel, SPI digital-to-analog converter (DAC).

This library provides a **full-featured API** to control all DAC8564 functions,  
including writing to channels, setting operating modes, controlling the internal reference,  
LDAC masking, software reset, and more.

---

## Features
- ✅ Write to individual channels or all channels
- ✅ Write to input registers without updating outputs
- ✅ Set **operating modes** (normal, power-down 1 kΩ, power-down 100 kΩ, high-Z)
- ✅ Control **internal reference** (default (depending on DAC(s) state), powered up, powered down)
- ✅ Set **LDAC mask** to control update behavior
- ✅ Perform **software reset**
- ✅ Set **clear code** for CLR pin behavior

---

## Hardware Requirements
- Arduino board with SPI support (Uno, Mega, Nano, Due, ESP32, etc.)
- SPI wiring:
  - **SCLK** → Arduino SCK
  - **DIN** → Arduino MOSI
  - **SYNC** (chip select) → user-defined pin
  - **LDAC** → user-defined pin
  - **CLR** (optional) → tie high or control externally
  - **VREF** → external reference or use internal reference

---

## Basic Usage
```cpp
#include <SPI.h>
#include <DAC8564.h>

// Pin definitions
#define PIN_NSS    10 // SYNC pin on DAC8564
#define PIN_LDAC   9
#define PIN_ENABLE 8
#define SPI_SPEED  1000000

DAC8564 dac(SPI, SPI_SPEED, PIN_NSS, PIN_LDAC, PIN_ENABLE);

void setup() {
    Serial.begin(115200);

    dac.begin();   // Initialize SPI and pins
    dac.enable();  // Enable DAC

    // Enable internal reference (powered up)
    dac.setReferenceMode(DAC8564::REF_POWERED_UP);

    // Set channel A to normal operation
    dac.setOperatingMode(DAC8564::A, DAC8564::NORMAL_OPERATION);

    // Write mid-scale value to channel A
    dac.write(DAC8564::A, 32768);
}

void loop() {
    static uint16_t value = 0;
    dac.write(DAC8564::A, value);
    value += 512;
    delay(100);
}
```

---

## API Reference

### Constructor
```cpp
DAC8564(SPIClass &spi, uint8_t nssPin, uint8_t ldacPin, uint8_t enablePin);
```
- `spi` — SPI interface (usually `SPI`)
- `nssPin` — SYNC / chip select pin
- `ldacPin` — LDAC pin
- `enablePin` — hardware enable pin

---

### Initialization
```cpp
void begin();
void enable();
```
- `begin()` — sets pin modes and starts SPI
- `enable()` — toggles control pins to activate DAC

---

### Writing Data
```cpp
bool write(Channel channel, uint16_t value);
bool writeInputRegister(Channel channel, uint16_t value);
bool writeAll(uint16_t value);
```
- `write()` — write to channel and update output
- `writeInputRegister()` — write without updating output
- `writeAll()` — write same value to all channels

---

### Operating Modes
```cpp
bool setOperatingMode(Channel channel, OperatingMode mode);
```
Modes:
- `NORMAL_OPERATION`
- `POWER_DOWN_1K`
- `POWER_DOWN_100K`
- `POWER_DOWN_HIZ`

---

### Reference Modes
```cpp
bool setReferenceMode(ReferenceMode mode);
```
Modes:
- `REF_POWERED_DOWN` — 0x012000
- `REF_DEFAULT` — 0x010000
- `REF_POWERED_UP` — 0x011000

---

### LDAC Mask
```cpp
bool setLdacMask(uint8_t mask);
```
- 4-bit mask: `1` = ignore LDAC, `0` = respond to LDAC

---

### Software Reset
```cpp
bool reset();
```
- Resets DAC registers to default

---

### Clear Code
```cpp
bool setClearCode(uint16_t value);
```
- Sets output value when CLR pin is asserted
