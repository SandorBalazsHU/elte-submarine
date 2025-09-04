#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "RS485Recive.h"
#include "MotorController.h"
#include "JoystickInputRS485.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayIsOn = false;


JoystickInputRS485 joystic = JoystickInputRS485();
MotorController motor = MotorController(10, 11);

// Lábra kötött szenzor és vezérlés
const int waterSensorPin = A0;
const int powerControlPin = 7;

// Érzékelési küszöb (ez a szenzortól függően változhat)
const int waterThreshold = 300;

void setup() {
    pinMode(powerControlPin, OUTPUT);
    digitalWrite(powerControlPin, HIGH); // Alapból bekapcsolt állapot
    joystic.init();
    motor.init();
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        displayIsOn = false;
    }else{
        displayIsOn = true;
    }
}

void loop() {
    joystic.update();

    int joyX = joystic.getX();
    int joyY = joystic.getY();

    motor.processJoystickInput(joyX, joyY);
    if(joystic.isDPressed()) motor.leftTurn();
    if(joystic.isDReleased()) motor.reset();
    if(joystic.isBPressed()) motor.rightTurn();
    if(joystic.isBReleased()) motor.reset();

    if(joystic.isAPressed()) motor.forward();
    if(joystic.isAReleased()) motor.reset();
    if(joystic.isCPressed()) motor.backward();
    if(joystic.isCReleased()) motor.reset();

    int sensorValue = analogRead(waterSensorPin);

    if (sensorValue > waterThreshold) {
      digitalWrite(powerControlPin, LOW);  // Áramot elvesz
    } else {
      digitalWrite(powerControlPin, HIGH); // Áramot ad
    }

    if(displayIsOn)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.print(F("VERTICAL MOTOR CONTROLLER!"));
        display.print(F("LEFT  MOTOR: "));
        display.println(motor.getLeftSpeed());
        display.print(F("RIGHT MOTOR: "));
        display.println(motor.getRightSpeed());

        display.print(F("YOJ X: "));
        display.println(joyX);
        display.print(F("YOJ Y: "));
        display.println(joyY);
        display.print(F("WATER: "));
        display.println(sensorValue);
        display.print(F("T: "));
        display.println(millis());
        display.display();
    } 

    delay(200);
}