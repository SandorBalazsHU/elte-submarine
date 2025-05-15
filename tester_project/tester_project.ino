#include <Servo.h>

/*
1525 min óramutató szerint
1512 cicereg
1513 cicereg
1512 áll
1498 KÖZÉP
1484 áll
1483 cicereg
1475 min óramutatóval ellentétes
*/


const int pinMotor = 10;
const int pinA = 2;  // Gomb A: növel
const int pinC = 4;  // Gomb C: csökkent

Servo motor;

// ==== Állítható paraméterek ====
const int CENTER_US = 1500;       // Semleges jel (közép)
const int STEP_US = 1;           // Egy gombnyomás ennyi µs-sel növel/csökkent
const int MIN_US = 1200;          // Legkisebb jel
const int MAX_US = 1800;          // Legnagyobb jel
// ==============================

int pwmUs = CENTER_US;

bool prevA = HIGH;
bool prevC = HIGH;

void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);

  motor.attach(pinMotor);
  motor.writeMicroseconds(pwmUs);

  Serial.begin(9600);
  Serial.println("Microseconds alapú ESC vezérlés indul.");
}

void loop() {
  bool currentA = digitalRead(pinA);
  bool currentC = digitalRead(pinC);

  if (currentA == LOW && prevA == HIGH) {
    pwmUs += STEP_US;
    pwmUs = constrain(pwmUs, MIN_US, MAX_US);
    Serial.print("↑ PWM us: ");
    Serial.println(pwmUs);
  }

  if (currentC == LOW && prevC == HIGH) {
    pwmUs -= STEP_US;
    pwmUs = constrain(pwmUs, MIN_US, MAX_US);
    Serial.print("↓ PWM us: ");
    Serial.println(pwmUs);
  }

  motor.writeMicroseconds(pwmUs);

  prevA = currentA;
  prevC = currentC;

  delay(200); // 1 másodperces ciklusidő
}
