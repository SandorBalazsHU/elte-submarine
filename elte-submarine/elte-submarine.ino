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

    const int PWM_HARD_LIMIT = 96;

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

        // Joystick jellemzők
        const int JOY_MIN = 0;
        const int JOY_MAX = 673;
        const int JOY_CENTER_X = 332;
        const int JOY_CENTER_Y = 346;

        // Finomhangolható érzékenységi beállítások
        const float X_OFFSET_CORRECTION = -0.10;  // jobbra tolódás kompenzáció
        const float SPEED_EXPONENT = 10.0;         // nagyobb = lassabb sebességnövekedés

        // Skálázási tartomány
        const int SPEED_RANGE = 40;  // NEUTRAL ±40
        const int TURN_RANGE = 20;

        x = constrain(x, JOY_MIN, JOY_MAX);
        y = constrain(y, JOY_MIN, JOY_MAX);

        int centeredX = x - JOY_CENTER_X;
        int centeredY = y - JOY_CENTER_Y;

        // Normalizálás -1.0 ... +1.0 közé
        float normX = centeredX / 341.0;
        float normY = centeredY / 341.0;

        // Görbített, szelíd irányvezérlés (kvadratikus jellegű)
        float curvedX = normX * abs(normX) + X_OFFSET_CORRECTION;

        // Görbített, **nagyon szelíd** sebességgörbe (exponenciális)
        float curvedY = normY >= 0 ?
            pow(normY, SPEED_EXPONENT) :
          -pow(abs(normY), SPEED_EXPONENT);

        // Alap sebesség (mindkét motor)
        int baseSpeed = NEUTRAL_SPEED + int(curvedY * SPEED_RANGE);

        // Iránykorrekció (bal/jobb oldali eltérés)
        int delta = int(curvedX * TURN_RANGE);

        // Bal/jobb oldali PWM értékek
        int left = constrain(baseSpeed - delta, MIN_SPEED, min(MAX_SPEED, PWM_HARD_LIMIT));
        int right = constrain(baseSpeed + delta, MIN_SPEED, min(MAX_SPEED, PWM_HARD_LIMIT));

        setLeftSpeed(left);
        setRightSpeed(right);
    }

    void setLeftSpeed(int speed) {
        speed = constrain(speed, MIN_SPEED, min(MAX_SPEED, PWM_HARD_LIMIT));
        leftSpeed = speed;
        updateMotors();
    }

    void setRightSpeed(int speed) {
        speed = constrain(speed, MIN_SPEED, min(MAX_SPEED, PWM_HARD_LIMIT));
        rightSpeed = speed;
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


/*
  Összefoglalás:
  Joystick állás	Eredmény
  x = 332, y = 673	Egyenes előre, maximális sebességgel (PWM = 115)
  x = 332, y = 346	Áll, közép, semleges
  x = 673, y = 346	Egy helyben jobbra fordulás, vagy jobb oldali túlhajtás
  x = 0, y = 346	Egy helyben balra fordulás, vagy bal oldali túlhajtás
  x = 673, y = 673	Ívben jobbra előre
*/

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

