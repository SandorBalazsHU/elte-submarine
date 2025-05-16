#include "JoystickInput.h"
#include "RS485Send.h"


JoystickInput joystic = JoystickInput();
RS485Send serialRS485 = RS485Send(13,12);

void setup() {
    Serial.begin(9600);
    serialRS485.init();
}

void loop() {
  serialRS485.send(117,117,0,0,0,0,0,0,0);
  Serial.println("Send...");
  delay(1000);
}
