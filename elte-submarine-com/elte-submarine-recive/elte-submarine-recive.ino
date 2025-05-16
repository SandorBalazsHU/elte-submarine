#include "RS485Recive.h"
#include "MotorController.h"
#include "JoystickInputRS485.h"

JoystickInputRS485 joystic = JoystickInputRS485();
MotorController motor = MotorController(9, 8);

void setup() {
    Serial.begin(9600);
    joystic.init();
    motor.init();
}

void loop() {
    joystic.update();

    int joyX = joystic.getX();
    //Serial.println(joyX);
    int joyY = joystic.getY();

    motor.processJoystickInput(joyX, joyY);
    /*if(joystic.isDPressed()) motor.leftTurn();
    if(joystic.isDReleased()) motor.reset();
    if(joystic.isBPressed()) motor.rightTurn();
    if(joystic.isBReleased()) motor.reset();*/

    Serial.print("LEF: ");
    Serial.print(motor.getLeftSpeed());
    Serial.print(" | RIG: ");
    Serial.println(motor.getRightSpeed());

    delay(200);
}