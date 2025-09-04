#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayIsOn = false;

// --- Konfiguráció ---
const int ledPin = 3;            // MOSFET vezérlés, nagy fényerő
const int buttonPins[3] = {1, 2, 4}; // D1, D2, D4 (D3 a LED!)
const int commands[3] = {3, 4, 5};   // Parancsok villanásszáma
const unsigned long pulseDuration = 100; // ms, villanás hossza
const unsigned long pauseDuration = 200; // ms, villanások közötti szünet

// --- Állapotváltozók ---
bool lastButtonState[3] = {HIGH, HIGH, HIGH};
bool sentCommand[3] = {false, false, false};

void setup() {
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    Serial.begin(9600);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      displayIsOn = false;
      Serial.println("NO DISPLAY");
    }else{
      displayIsOn = true;
      Serial.println("DISPLAY");
    }
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    bool buttonState = digitalRead(buttonPins[i]);
    if (lastButtonState[i] == HIGH && buttonState == LOW && !sentCommand[i]) {
      // Gomb lenyomva és még nem küldtük el
      sendFlashes(commands[i]);
      sentCommand[i] = true;
    } else if (buttonState == HIGH) {
      sentCommand[i] = false; // gomb felengedve, újra küldhető
    }
    lastButtonState[i] = buttonState;
    if(displayIsOn)
    {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(F("SENDED!"));
    display.println(serialRS485.getMessage().c_str());
    display.print(F("T: "));
    display.println(millis());
    display.display();
    }
  }
}

void sendFlashes(int flashes) {
  for (int i = 0; i < flashes; i++) {
    digitalWrite(ledPin, HIGH);
    delay(pulseDuration);
    digitalWrite(ledPin, LOW);
    if (i < flashes - 1) delay(pauseDuration);
  }
}
