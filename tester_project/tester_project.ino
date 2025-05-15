#include <Servo.h>

/*
1540 max
1525 min óramutató szerint
1512 cicereg
1513 cicereg
1512 áll
1498 KÖZÉP
1484 áll
1483 cicereg
1475 min óramutatóval ellentétes
1460 min
*/
const int pinMotor = 10;
const int pinA = 2;  // Gomb A: növel
const int pinC = 4;  // Gomb C: csökkent
const int pinD = 3;  // Gomb D: reset

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
bool prevD = HIGH;

void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  pinMode(pinD, INPUT_PULLUP);

  motor.attach(pinMotor);
  motor.writeMicroseconds(pwmUs);

  Serial.begin(9600);
  Serial.println("Microseconds alapú ESC vezérlés indul.");
}

void loop() {
  bool currentA = digitalRead(pinA);
  bool currentC = digitalRead(pinC);
  bool currentD = digitalRead(pinD);

  if (currentD == LOW && prevD == HIGH) {
    pwmUs = CENTER_US;
    Serial.print("RESET");
    Serial.print("* PWM us: ");
    Serial.println(pwmUs);
  }

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
  prevD = currentD;

  delay(100); // 1 másodperces ciklusidő
}
