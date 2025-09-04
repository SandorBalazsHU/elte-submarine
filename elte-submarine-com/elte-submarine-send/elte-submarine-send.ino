#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "JoystickInput.h"
#include "RS485Send.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayIsOn = false;

JoystickInput joystic = JoystickInput();
RS485Send serialRS485 = RS485Send();

void setup() {
    serialRS485.init();

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        displayIsOn = false;
    }else{
        displayIsOn = true;
    }
}

void loop() {
    joystic.update();
    serialRS485.send(joystic.getX(),joystic.getY(),joystic.isSPressed(),joystic.isAPressed(),joystic.isBPressed(),joystic.isCPressed(),joystic.isDPressed(),joystic.isEPressed(),joystic.isFPressed());
    if(displayIsOn)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.println(F("RUNNING!"));
        display.println(F("MESSAGE:"));
        display.println(serialRS485.getMessage().c_str());
        display.print(F("TIME: "));
        display.println(millis());
        display.display();
    }
    delay(200);
}