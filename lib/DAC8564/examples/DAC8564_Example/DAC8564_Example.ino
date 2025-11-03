#include <SPI.h>
#include <DAC8564.h>

#define PIN_NSS 10 // SYNC pin on DAC8564
#define PIN_LDAC 9
#define PIN_ENABLE 8
#define SPI_SPEED 24000000 // 24MHz

DAC8564 dac(SPI_SPEED, PIN_ENABLE, PIN_NSS);

void setup()
{
  dac.begin();

  dac.setReferenceMode(DAC8564::ReferenceMode::REF_DEFAULT); // Set to default mode
  // dac.setReferenceMode(DAC8564::ReferenceMode::REF_POWERED_DOWN); // Power down internal reference
  // dac.setReferenceMode(DAC8564::ReferenceMode::REF_POWERED_UP);   // Enable internal reference powered up

  // Set all DACs to normal operation
  dac.setOperatingModeAll(DAC8564::OperatingMode::NORMAL_OPERATION);

  // Write mid-scale to all channels
  dac.broadcast(DAC8564::Broadcast::WRITE_ALL_LOAD_ALL, 32768);
}

void loop()
{
  // Write mid-scale to channel A of DAC at address 0
  dac.write(DAC8564::Address::ADDR_0, DAC8564::Load::WRITE_N, DAC8564::Channel::A, 32768);
  
  // Write mid-scale to channel B of DAC at address 0 and update (load) output
  dac.write(DAC8564::Address::ADDR_0, DAC8564::Load::WRITE_N_LOAD_N, DAC8564::Channel::B, 32768);
  
  // Write 0 to channel C of DAC at address 0 and update (load) all outputs
  dac.write(DAC8564::Address::ADDR_0, DAC8564::Load::WRITE_N_LOAD_ALL, DAC8564::Channel::C, 32768);
}