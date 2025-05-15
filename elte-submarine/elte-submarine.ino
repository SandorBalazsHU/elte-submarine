#include <Servo.h>

class MotorController {
private:
    Servo leftMotor;         // Bal oldali ESC vezérlő PWM
    Servo rightMotor;        // Jobb oldali ESC vezérlő PWM

    int leftMotorPin;
    int rightMotorPin;

    int leftSpeed;           // Aktuális bal motor PWM érték (0–180)
    int rightSpeed;          // Aktuális jobb motor PWM érték (0–180)

    int leftDirection;       // +1 előre, -1 hátra (jelenleg nem használjuk külön, de előkészített)
    int rightDirection;

    bool movementEnabled;    // Általános mozgástiltás
    bool joystickEnabled;    // Engedélyezzük-e a joystick vezérlést

    const int NEUTRAL_SPEED = 90;  // Motor áll, nincs előre/hátra mozgás

    const int SPEED_RANGE = 40;    // Max +/- eltérés a semlegestől
    const int TURN_RANGE = 20;     // Max +/- különbség bal és jobb motor közt kormányzáshoz

    const int MIN_SPEED = NEUTRAL_SPEED - SPEED_RANGE;  // = 90 - 40 = 50
    const int MAX_SPEED = NEUTRAL_SPEED + SPEED_RANGE;  // = 90 + 40 = 130

public:
    // Konstruktor: inicializálja a pineket és változókat
    MotorController(int leftPin, int rightPin) {
        leftMotorPin = leftPin;
        rightMotorPin = rightPin;

        leftSpeed = NEUTRAL_SPEED;
        rightSpeed = NEUTRAL_SPEED;

        leftDirection = 1;
        rightDirection = 1;

        movementEnabled = true;
        joystickEnabled = true;
    }

    // Inicializálás: attach-eljük a motorokat és semleges állapotba állítjuk őket
    void init() {
        leftMotor.attach(leftMotorPin);
        rightMotor.attach(rightMotorPin);
        updateMotors();
    }

    // Frissíti a motor PWM jeleit, ha a mozgás engedélyezve van
    void updateMotors() {
        if (!movementEnabled) {
            leftMotor.write(NEUTRAL_SPEED);
            rightMotor.write(NEUTRAL_SPEED);
            return;
        }

        leftMotor.write(leftSpeed);
        rightMotor.write(rightSpeed);
    }

    // jelenlegi álló motor 180-190 x tengely
    // Joystick input feldolgozása: irány és sebesség kiszámítása
    void processJoystickInput(int x, int y) {
        if (!movementEnabled || !joystickEnabled)
            return;

        const int JOY_MIN = 0;
        const int JOY_MAX = 673;
        const int JOY_CENTER_X = 332;
        const int JOY_CENTER_Y = 346;

        const float X_OFFSET_CORRECTION = -0.10;  // 🔧 finomhangolható! (negatív → jobbra tolt semleges pont)

        x = constrain(x, JOY_MIN, JOY_MAX);
        y = constrain(y, JOY_MIN, JOY_MAX);

        int centeredX = x - JOY_CENTER_X;
        int centeredY = y - JOY_CENTER_Y;

        float normX = centeredX / 341.0;
        float normY = centeredY / 341.0;

        float curvedX = normX * abs(normX) + X_OFFSET_CORRECTION;
        float curvedY = normY * abs(normY);

        const int SPEED_RANGE = 40;
        const int TURN_RANGE = 20;

        int baseSpeed = NEUTRAL_SPEED + int(curvedY * SPEED_RANGE);
        int delta = int(curvedX * TURN_RANGE);

        int left = constrain(baseSpeed - delta, MIN_SPEED, MAX_SPEED);
        int right = constrain(baseSpeed + delta, MIN_SPEED, MAX_SPEED);

        setLeftSpeed(left);
        setRightSpeed(right);
    }


    // Állítsa be a bal motor sebességét
    void setLeftSpeed(int speed) {
        leftSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
        updateMotors();
    }

    // Állítsa be a jobb motor sebességét
    void setRightSpeed(int speed) {
        rightSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);
        updateMotors();
    }

    // Bal motor irány (előre/hátra) — jövőbeli bővítéshez
    void setLeftDirection(int dir) {
        leftDirection = (dir >= 0) ? 1 : -1;
    }

    void setRightDirection(int dir) {
        rightDirection = (dir >= 0) ? 1 : -1;
    }

    // Mozgás letiltása (mindkét motor STOP)
    void stop() {
        movementEnabled = false;
        updateMotors();  // azonnal állítsa meg a motorokat
    }

    // Mozgás engedélyezése (ismét aktív joystick vezérlés)
    void start() {
        movementEnabled = true;
        updateMotors();  // aktuális értékkel újraindít
    }

    // Bal oldal teljes fordulat, joystick input figyelmen kívül hagyva
    void leftFull() {
        joystickEnabled = false;
        setLeftSpeed(MIN_SPEED);
        setRightSpeed(MAX_SPEED);
    }

    // Jobb oldal teljes fordulat, joystick input figyelmen kívül hagyva
    void rightFull() {
        joystickEnabled = false;
        setLeftSpeed(MAX_SPEED);
        setRightSpeed(MIN_SPEED);
    }

    // Visszaállít minden tiltást — joystick és mozgás újra aktív
    void reset() {
        joystickEnabled = true;
        movementEnabled = true;
        setLeftSpeed(NEUTRAL_SPEED);
        setRightSpeed(NEUTRAL_SPEED);
    }

    // Getterek
    int getLeftSpeed() const { return leftSpeed; }
    int getRightSpeed() const { return rightSpeed; }
    bool isMovementEnabled() const { return movementEnabled; }
    bool isJoystickEnabled() const { return joystickEnabled; }
};


class JoystickInput {
private:
    // Alapértelmezett pinek (módosíthatók konstruktorban)
    int pinX;
    int pinY;
    int pinSW;      // Joystick gomb
    int pinA;
    int pinB;
    int pinC;
    int pinD;

    // Bemeneti értékek tárolása
    int xValue;     // 0–1023
    int yValue;
    bool swPressed;
    bool aPressed;
    bool bPressed;
    bool cPressed;
    bool dPressed;

public:
    // Konstruktor, opcionálisan testreszabható pinekkel
    JoystickInput(int xPin = A0, int yPin = A1, int swPin = 8,
                  int aPin = 2, int bPin = 3, int cPin = 4, int dPin = 5) {
        pinX = xPin;
        pinY = yPin;
        pinSW = swPin;
        pinA = aPin;
        pinB = bPin;
        pinC = cPin;
        pinD = dPin;

        // Beállítjuk a gomb pineket bemenetként
        pinMode(pinSW, INPUT_PULLUP);
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
        pinMode(pinC, INPUT_PULLUP);
        pinMode(pinD, INPUT_PULLUP);

        // Inicializálás
        xValue = 512;
        yValue = 512;
        swPressed = false;
        aPressed = false;
        bPressed = false;
        cPressed = false;
        dPressed = false;
    }

    // Állapotok frissítése – minden bemenetet olvas
    void update() {
        xValue = analogRead(pinX);
        yValue = analogRead(pinY);
        swPressed = (digitalRead(pinSW) == LOW);
        aPressed = (digitalRead(pinA) == LOW);
        bPressed = (digitalRead(pinB) == LOW);
        cPressed = (digitalRead(pinC) == LOW);
        dPressed = (digitalRead(pinD) == LOW);
    }

    // Getterek
    int getX() const { return xValue; }
    int getY() const { return yValue; }

    bool isSWPressed() const { return swPressed; }
    bool isAPressed() const { return aPressed; }
    bool isBPressed() const { return bPressed; }
    bool isCPressed() const { return cPressed; }
    bool isDPressed() const { return dPressed; }
};

JoystickInput joystic = JoystickInput();

MotorController motor = MotorController(10,11);

void setup() {
  Serial.begin(9600);
  motor.init();
}

//x tengely: 0-673 kozép: 332
//y tengely: 0-673 közép 346
void loop() {
  // put your main code here, to run repeatedly:
  joystic.update();
  //Serial.println(joystic.getX());

  int joyX = joystic.getX();
  int joyY = joystic.getY();

  motor.processJoystickInput(joyX, joyY);

  Serial.println("X:");
  Serial.println(joyX);
  Serial.println("Y:");
  Serial.println(joyY);

  delay(200);
}

