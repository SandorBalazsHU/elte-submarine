#include "JoystickInput.h"
#include "RS485Send.h"


JoystickInput joystic = JoystickInput();
RS485Send serialRS485 = RS485Send(13,12);

void setup() {
    Serial.begin(9600);
    serialRS485.init();
}

void loop() {
    joystic.update();
    serialRS485.send(joystic.getX(),joystic.getY(),joystic.isSPressed(),joystic.isAPressed(),joystic.isBPressed(),joystic.isCPressed(),joystic.isDPressed(),joystic.isEPressed(),joystic.isFPressed());
    Serial.println("Message sended.");
    delay(200);
}
