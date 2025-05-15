#include <Servo.h>

class MotorController {
private:
    Servo leftMotor;
    Servo rightMotor;

    int leftMotorPin;
    int rightMotorPin;

    int leftSpeed;
    int rightSpeed;

    int leftDirection;
    int rightDirection;

    bool movementEnabled;
    bool joystickEnabled;

    const int PWM_HARD_LIMIT = 96;
    const int NEUTRAL_PWM = 90;

    const int MAX_FORWARD_PWM = 96;
    const int MIN_BACKWARD_PWM = 84;

    const int LEVELS = 6;

public:
    MotorController(int leftPin, int rightPin) {
        leftMotorPin = leftPin;
        rightMotorPin = rightPin;

        leftSpeed = NEUTRAL_PWM;
        rightSpeed = NEUTRAL_PWM;

        leftDirection = 1;
        rightDirection = 1;

        movementEnabled = true;
        joystickEnabled = true;
    }

    void init() {
        leftMotor.attach(leftMotorPin);
        rightMotor.attach(rightMotorPin);
        updateMotors();
    }

    void updateMotors() {
        if (!movementEnabled) {
            leftMotor.write(NEUTRAL_PWM);
            rightMotor.write(NEUTRAL_PWM);
            return;
        }

        leftMotor.write(leftSpeed);
        rightMotor.write(rightSpeed);
    }

    void processJoystickInput(int x, int y) {
        if (!movementEnabled || !joystickEnabled)
            return;

        // Kalibrációs értékek
        const int JOY_CENTER_X = 332;
        const int JOY_CENTER_Y = 346;

        const float FORWARD_STEP = (MAX_FORWARD_PWM - NEUTRAL_PWM) / float(LEVELS);  // 1.0
        const float BACKWARD_STEP = (NEUTRAL_PWM - MIN_BACKWARD_PWM) / float(LEVELS); // 1.0

        float normX = (x - JOY_CENTER_X) / 341.0;
        float normY = (y - JOY_CENTER_Y) / 341.0;

        int baseSpeed = NEUTRAL_PWM;

        if (abs(normY) <= 0.05) {
            baseSpeed = NEUTRAL_PWM;
        } else if (normY > 0.05) {
            baseSpeed = NEUTRAL_PWM + int(normY * LEVELS * FORWARD_STEP);
        } else if (normY < -0.05) {
            baseSpeed = NEUTRAL_PWM + int(normY * LEVELS * BACKWARD_STEP); // negatív -> csökken
        }

        baseSpeed = constrain(baseSpeed, MIN_BACKWARD_PWM, MAX_FORWARD_PWM);

        const int MAX_DELTA = 3;
        int delta = int(normX * MAX_DELTA);

        int left = baseSpeed - delta;
        int right = baseSpeed + delta;

        setLeftSpeed(left);
        setRightSpeed(right);
    }

    void setLeftSpeed(int speed) {
        if (speed > NEUTRAL_PWM) {
            speed = constrain(speed, NEUTRAL_PWM, min(PWM_HARD_LIMIT, MAX_FORWARD_PWM));
        } else if (speed < NEUTRAL_PWM) {
            speed = constrain(speed, MIN_BACKWARD_PWM, NEUTRAL_PWM);
        }
        leftSpeed = speed;
        updateMotors();
    }

    void setRightSpeed(int speed) {
        if (speed > NEUTRAL_PWM) {
            speed = constrain(speed, NEUTRAL_PWM, min(PWM_HARD_LIMIT, MAX_FORWARD_PWM));
        } else if (speed < NEUTRAL_PWM) {
            speed = constrain(speed, MIN_BACKWARD_PWM, NEUTRAL_PWM);
        }
        rightSpeed = speed;
        updateMotors();
    }

    void setLeftDirection(int dir) {
        leftDirection = (dir >= 0) ? 1 : -1;
    }

    void setRightDirection(int dir) {
        rightDirection = (dir >= 0) ? 1 : -1;
    }

    void stop() {
        movementEnabled = false;
        updateMotors();
    }

    void start() {
        movementEnabled = true;
        updateMotors();
    }

    void leftFull() {
        joystickEnabled = false;
        setLeftSpeed(MIN_BACKWARD_PWM);
        setRightSpeed(MAX_FORWARD_PWM);
    }

    void rightFull() {
        joystickEnabled = false;
        setLeftSpeed(MAX_FORWARD_PWM);
        setRightSpeed(MIN_BACKWARD_PWM);
    }

    void reset() {
        joystickEnabled = true;
        movementEnabled = true;
        setLeftSpeed(NEUTRAL_PWM);
        setRightSpeed(NEUTRAL_PWM);
    }

    int getLeftSpeed() const { return leftSpeed; }
    int getRightSpeed() const { return rightSpeed; }
    bool isMovementEnabled() const { return movementEnabled; }
    bool isJoystickEnabled() const { return joystickEnabled; }
};


class JoystickInput {
private:
    int pinX, pinY, pinSW, pinA, pinB, pinC, pinD;
    int xValue, yValue;
    bool swPressed, aPressed, bPressed, cPressed, dPressed;

public:
    JoystickInput(int xPin = A0, int yPin = A1, int swPin = 8,
                  int aPin = 2, int bPin = 3, int cPin = 4, int dPin = 5) {
        pinX = xPin;
        pinY = yPin;
        pinSW = swPin;
        pinA = aPin;
        pinB = bPin;
        pinC = cPin;
        pinD = dPin;

        pinMode(pinSW, INPUT_PULLUP);
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
        pinMode(pinC, INPUT_PULLUP);
        pinMode(pinD, INPUT_PULLUP);

        xValue = 512;
        yValue = 512;
        swPressed = false;
        aPressed = false;
        bPressed = false;
        cPressed = false;
        dPressed = false;
    }

    void update() {
        xValue = analogRead(pinX);
        yValue = analogRead(pinY);
        swPressed = (digitalRead(pinSW) == LOW);
        aPressed = (digitalRead(pinA) == LOW);
        bPressed = (digitalRead(pinB) == LOW);
        cPressed = (digitalRead(pinC) == LOW);
        dPressed = (digitalRead(pinD) == LOW);
    }

    int getX() const { return xValue; }
    int getY() const { return yValue; }

    bool isSWPressed() const { return swPressed; }
    bool isAPressed() const { return aPressed; }
    bool isBPressed() const { return bPressed; }
    bool isCPressed() const { return cPressed; }
    bool isDPressed() const { return dPressed; }
};


// Példányosítás
JoystickInput joystic = JoystickInput();
MotorController motor = MotorController(10, 11);

void setup() {
    Serial.begin(9600);
    motor.init();
}

void loop() {
    joystic.update();

    int joyX = joystic.getX();
    int joyY = joystic.getY();

    motor.processJoystickInput(joyX, joyY);

    Serial.print("X: ");
    Serial.print(joyX);
    Serial.print(" | Y: ");
    Serial.println(joyY);

    delay(200);
}
