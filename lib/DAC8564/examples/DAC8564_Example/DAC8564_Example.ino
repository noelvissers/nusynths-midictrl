#include <SPI.h>
#include <DAC8564.h>

#define PIN_NSS 10 // SYNC pin on DAC8564
#define PIN_LDAC 9
#define PIN_ENABLE 8
#define SPI_SPEED 1000000

DAC8564 dac(SPI, SPI_SPEED, PIN_NSS, PIN_LDAC, PIN_ENABLE);

void setup()
{
  Serial.begin(115200);
  dac.begin();
  dac.enable();

  // Enable internal reference powered up
  dac.setReferenceMode(DAC8564::REF_POWERED_UP);

  // Set to default mode
  dac.setReferenceMode(DAC8564::REF_DEFAULT);

  // Power down internal reference
  dac.setReferenceMode(DAC8564::REF_POWERED_DOWN);

  // Set all channels to normal operation
  dac.setOperatingMode(DAC8564::ALL, DAC8564::NORMAL_OPERATION);

  // Write mid-scale to all channels
  dac.writeAll(32768);

  // Mask LDAC for channel B
  dac.setLdacMask(0b0010);

  // Set clear code to zero
  dac.setClearCode(0x0000);

  // Reset DAC
  dac.reset();
}

void loop()
{
  static uint16_t value = 0;
  dac.write(DAC8564::A, value);
  value += 512;
  delay(100);
}