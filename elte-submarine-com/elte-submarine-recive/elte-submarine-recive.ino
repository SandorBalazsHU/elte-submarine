#include "RS485Recive.h"

RS485Receive serialRS485 = RS485Receive(13,12);

void setup() {
    Serial.begin(9600);
    serialRS485.init();
}

void loop() {
  Serial.println("Recive...");
  serialRS485.update();
  delay(200);
}
