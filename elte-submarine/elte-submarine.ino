#include "MotorController.h"
#include "JoystickInput.h"

JoystickInput joystic = JoystickInput();
MotorController motor = MotorController(11, 10);

void setup() {
    Serial.begin(9600);
    motor.init();
}

void loop() {
    joystic.update();

    int joyX = joystic.getX();
    int joyY = joystic.getY();

    motor.processJoystickInput(joyX, joyY);

    Serial.print("LEF: ");
    Serial.print(motor.getLeftSpeed());
    Serial.print(" | RIG: ");
    Serial.println(motor.getRightSpeed());

    delay(200);
}
