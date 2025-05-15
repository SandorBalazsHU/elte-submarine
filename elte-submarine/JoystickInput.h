class JoystickInput {
private:
    int pinX, pinY, pinS, pinA, pinB, pinC, pinD, pinE, pinF;
    int xValue, yValue;
    bool sPressed, aPressed, bPressed, cPressed, dPressed, ePressed, fPressed;

public:
    JoystickInput(int xPin = A0, int yPin = A1, int sPin = 8,
                  int aPin = 2, int bPin = 3, int cPin = 4, int dPin = 5, int ePin = 6, int fPin = 7) {
        pinX = xPin;
        pinY = yPin;
        pinS = sPin;
        pinA = aPin;
        pinB = bPin;
        pinC = cPin;
        pinD = dPin;
        pinE = ePin;
        pinF = fPin;

        pinMode(pinS, INPUT_PULLUP);
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
        pinMode(pinC, INPUT_PULLUP);
        pinMode(pinD, INPUT_PULLUP);
        pinMode(pinE, INPUT_PULLUP);
        pinMode(pinF, INPUT_PULLUP);

        const int X_MID = 331;
        const int Y_MID = 346;
        xValue = X_MID;
        yValue = Y_MID;
        sPressed = false;
        aPressed = false;
        bPressed = false;
        cPressed = false;
        dPressed = false;
        ePressed = false;
        fPressed = false;
    }

    void update() {
        xValue = analogRead(pinX);
        yValue = analogRead(pinY);
        sPressed = (digitalRead(pinS) == LOW);
        aPressed = (digitalRead(pinA) == LOW);
        bPressed = (digitalRead(pinB) == LOW);
        cPressed = (digitalRead(pinC) == LOW);
        dPressed = (digitalRead(pinD) == LOW);
        ePressed = (digitalRead(pinE) == LOW);
        fPressed = (digitalRead(pinF) == LOW);
    }

    int getX() const { return xValue; }
    int getY() const { return yValue; }

    bool isSPressed() const { return sPressed; }
    bool isAPressed() const { return aPressed; }
    bool isBPressed() const { return bPressed; }
    bool isCPressed() const { return cPressed; }
    bool isDPressed() const { return dPressed; }
    bool isEPressed() const { return ePressed; }
    bool isFPressed() const { return fPressed; }
};