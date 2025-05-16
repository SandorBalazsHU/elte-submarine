class JoystickInput {
private:
    int pinX, pinY, pinS, pinA, pinB, pinC, pinD, pinE, pinF;
    int xValue, yValue;

    bool sPressed, aPressed, bPressed, cPressed, dPressed, ePressed, fPressed;
    bool sPrev, aPrev, bPrev, cPrev, dPrev, ePrev, fPrev;
    bool sReleased, aReleased, bReleased, cReleased, dReleased, eReleased, fReleased;

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

        sPressed = aPressed = bPressed = cPressed = dPressed = ePressed = fPressed = false;
        sPrev    = aPrev    = bPrev    = cPrev    = dPrev    = ePrev    = fPrev    = false;
        sReleased = aReleased = bReleased = cReleased = dReleased = eReleased = fReleased = false;
    }

    void update() {
        xValue = analogRead(pinX);
        yValue = analogRead(pinY);

        updateButton(pinS, sPressed, sPrev, sReleased);
        updateButton(pinA, aPressed, aPrev, aReleased);
        updateButton(pinB, bPressed, bPrev, bReleased);
        updateButton(pinC, cPressed, cPrev, cReleased);
        updateButton(pinD, dPressed, dPrev, dReleased);
        updateButton(pinE, ePressed, ePrev, eReleased);
        updateButton(pinF, fPressed, fPrev, fReleased);
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

    bool isSReleased() { return consumeReleased(sReleased); }
    bool isAReleased() { return consumeReleased(aReleased); }
    bool isBReleased() { return consumeReleased(bReleased); }
    bool isCReleased() { return consumeReleased(cReleased); }
    bool isDReleased() { return consumeReleased(dReleased); }
    bool isEReleased() { return consumeReleased(eReleased); }
    bool isFReleased() { return consumeReleased(fReleased); }

private:
    void updateButton(int pin, bool &current, bool &previous, bool &released) {
        bool now = (digitalRead(pin) == LOW);
        released = (!now && previous);  // Felengedés esemény
        previous = current;
        current = now;
    }

    bool consumeReleased(bool &releasedFlag) {
        if (releasedFlag) {
            releasedFlag = false;
            return true;
        }
        return false;
    }
};
