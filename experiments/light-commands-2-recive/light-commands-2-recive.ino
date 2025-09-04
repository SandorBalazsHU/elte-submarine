#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayIsOn = false;

// --- Konfiguráció ---
const int ldrPin = A0;
const int pwmPin = 10;

const int idlePWM = 1500;
const int pwm3 = 1600;
const int pwm4 = 1700;
const int pwm5 = 1500;

const int pulseDuration = 100; // ms - elfogadott impulzushossz középérték
const int pulseTolerance = 20; // ms - tűrés impulzushosszra
const int impulseTimeout = 200; // ms - ha ennyi ideig nincs új impulzus, vége a parancsnak

const int ldrThreshold = 150; // ennyivel kell nőnie az értéknek impulzushoz

// --- Állapotváltozók ---
int baseLdr = 0;
bool lastPulse = false;
unsigned long lastPulseTime = 0;
unsigned long startTime = 0;
int pulseCount = 0;

void setup() {
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, pwmToAnalog(idlePWM)); // alapérték
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    displayIsOn = false;
    Serial.println("NO DISPLAY");
  }else{
  displayIsOn = true;
  Serial.println("DISPLAY");
}

  // Környezeti fény mérése
  baseLdr = analogRead(ldrPin);
  delay(100);
}

void loop() {
  int ldrVal = analogRead(ldrPin);
  unsigned long now = millis();

  // Impulzusdetektálás: ha most nagy a fény és eddig nem volt
  if (ldrVal > baseLdr + ldrThreshold && !lastPulse) {
    unsigned long pulseStart = now;

    // Figyeljük, hogy tényleg impulzusról van szó: hossza megfelelő-e?
    while (analogRead(ldrPin) > baseLdr + ldrThreshold) {
      // Ne blokkoljuk a főciklust: csak nagyon rövid várakozás!
      delayMicroseconds(500);
      if (millis() - pulseStart > pulseDuration + pulseTolerance) break;
    }
    unsigned long pulseLength = millis() - pulseStart;

    if (pulseLength >= pulseDuration - pulseTolerance &&
        pulseLength <= pulseDuration + pulseTolerance) {
      pulseCount++;
      lastPulseTime = now;
      Serial.print("Impulzus: "); Serial.println(pulseCount);
    }
    lastPulse = true; // impulzus feldolgozva
  }

  // Ha nincs impulzus, állítsuk vissza a "készenléti" állapotot
  if (ldrVal < baseLdr + ldrThreshold / 2) {
    lastPulse = false;
  }

  // Ha túl régen volt impulzus, parancs lezárása
  if (pulseCount > 0 && now - lastPulseTime > impulseTimeout) {
    handleCommand(pulseCount);
    pulseCount = 0;
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

void handleCommand(int count) {
  if (count == 3) {
    analogWrite(pwmPin, pwmToAnalog(pwm3));
    Serial.println("3 impulzus: 1600 PWM");
  } else if (count == 4) {
    analogWrite(pwmPin, pwmToAnalog(pwm4));
    Serial.println("4 impulzus: 1700 PWM");
  } else if (count == 5) {
    analogWrite(pwmPin, pwmToAnalog(pwm5));
    Serial.println("5 impulzus: 1500 PWM");
  } else {
    Serial.print("Ismeretlen parancs: "); Serial.println(count);
  }
}

// Arduino UNO: 8 bites PWM, 0..255 (kb. 490Hz)
// Át lehet méretezni a PWM értéket 1000..2000 közöttiről 0..255-re
int pwmToAnalog(int pwm) {
  // Feltételezve, hogy 1000..2000 közti értékeket akarunk kiadni
  // 1000 -> 0, 2000 -> 255
  int val = map(pwm, 1000, 2000, 0, 255);
  val = constrain(val, 0, 255);
  return val;
}
