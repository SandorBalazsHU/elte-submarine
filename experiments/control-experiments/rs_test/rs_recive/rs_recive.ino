#include <SoftwareSerial.h>

#define TX_PIN 12     // TX pin nem használt fogadásnál
#define RX_PIN 13    // RX pin a MAX485 RO lábához

SoftwareSerial rs485Serial(RX_PIN, TX_PIN);  // RX, TX – itt TX nem fog hasznosulni

void setup() {

  rs485Serial.begin(9600);
  Serial.begin(9600);
  Serial.println("RS485 Fogadó indul...");
}

void loop() {
  if (rs485Serial.available()) {
    String bejovo = rs485Serial.readStringUntil('\n');
    Serial.println("Kapott üzenet: " + bejovo);
  }
}
