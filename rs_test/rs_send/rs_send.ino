#include <SoftwareSerial.h>

#define TX_PIN 12    // TX pin a MAX485 DI lábához
#define RX_PIN 13     // RX pin nem használt küldésnél

SoftwareSerial rs485Serial(RX_PIN, TX_PIN);  // RX, TX – itt RX nem fog hasznosulni

void setup() {

  rs485Serial.begin(9600);
  Serial.begin(9600);
  Serial.println("RS485 Küldő indul...");
}

void loop() {
  String uzenet = "Hello RS485!";
  rs485Serial.println(uzenet);
  Serial.println("Üzenet elküldve: " + uzenet);
  delay(1000);
}
