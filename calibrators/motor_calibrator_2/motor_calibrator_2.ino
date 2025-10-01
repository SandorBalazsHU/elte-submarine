#include <Servo.h>

Servo esc;  
int pwmValue = 1000;  // alapértelmezett induló érték (minimum gáz)

void setup() {
  Serial.begin(9600);
  esc.attach(10);               // ESC jel bemenet a D10 pinre
  esc.writeMicroseconds(1000);  // biztos minimumon indul
  delay(3000);                  // ESC inicializálás (csipogás ideje)
  Serial.println("Írj be egy értéket (1000 - 2000):");
}

void loop() {
  if (Serial.available() > 0) {
    int val = Serial.parseInt();  // beolvas egy egész számot
    if (val >= 1000 && val <= 2000) {
      pwmValue = val;
      esc.writeMicroseconds(pwmValue);
      Serial.print("Küldve: ");
      Serial.println(pwmValue);
    } else {
      Serial.println("Hibas ertek! (1000 - 2000 közötti számot írj)");
    }
  }
}
