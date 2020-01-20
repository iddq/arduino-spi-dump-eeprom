#include <SPI.h>

#define SPI_READ_CMD 0x03
#define CS_PIN1 52 // software CS
#define CS_PIN2 10 // SPI hw CS

#ifndef PIN_LED
#define PIN_LED 13
#endif

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(CS_PIN1, OUTPUT);
  //pinMode(CS_PIN2, INPUT);
  digitalWrite(CS_PIN1, HIGH);
  //digitalWrite(CS_PIN2, HIGH);
  Serial.begin(115200);
  SPI.begin();
}

void read_eeprom(unsigned int num_bytes) {
  unsigned int addr;
  byte resp;

  SPI.beginTransaction(CS_PIN2, SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  digitalWrite(CS_PIN1, LOW);

  /* transmit read command with 3 byte start address */
  SPI.transfer(CS_PIN2, SPI_READ_CMD);
  SPI.transfer(CS_PIN2, 0x00);
  SPI.transfer(CS_PIN2, 0x00);
  //SPI.transfer(CS_PIN2, 0x00);

  for (addr = 0; addr < num_bytes; addr++) {
    resp = SPI.transfer(CS_PIN2, 0xff);
    Serial.write(resp);
  }
  digitalWrite(CS_PIN1, HIGH);

  SPI.endTransaction();
  
}

void dump() {
  unsigned int num_bytes;
  unsigned int i;

  /* wait for the integer with the requested number of bytes */
  while (Serial.available() < 4) {
  }

  num_bytes = 0;

  /* merge four bytes to single integer */
  for (i = 0; i < 4; i++)
    num_bytes |=  Serial.read() << (i * 8);

  read_eeprom(num_bytes);
}

void loop() {

  digitalWrite(PIN_LED, LOW);

  if (Serial.read() == 'D') {
    digitalWrite(PIN_LED, HIGH);
    dump();
  }

}

