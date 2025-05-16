#ifndef JOYSTICK_INPUT_RS485
#define JOYSTICK_INPUT_RS485

#include "RS485Recive.h"

class JoystickInputRS485 {
private:
    RS485Receive serialRS485;

    int xValue, yValue;

    bool sPressed, aPressed, bPressed, cPressed, dPressed, ePressed, fPressed;
    bool sPrev, aPrev, bPrev, cPrev, dPrev, ePrev, fPrev;
    bool sReleased, aReleased, bReleased, cReleased, dReleased, eReleased, fReleased;

public:
    JoystickInputRS485() : serialRS485(RS485Receive(13,12)) {

        const int X_MID = 331;
        const int Y_MID = 346;
        xValue = X_MID;
        yValue = Y_MID;

        sPressed = aPressed = bPressed = cPressed = dPressed = ePressed = fPressed = false;
        sPrev    = aPrev    = bPrev    = cPrev    = dPrev    = ePrev    = fPrev    = false;
        sReleased = aReleased = bReleased = cReleased = dReleased = eReleased = fReleased = false;
    }

    void init(){
      serialRS485.init();
    }

    void update() {
        serialRS485.update();
        xValue = serialRS485.getX();
        yValue = serialRS485.getY();

        updateButton(serialRS485.isSPressed(), sPressed, sPrev, sReleased);
        updateButton(serialRS485.isAPressed(), aPressed, aPrev, aReleased);
        updateButton(serialRS485.isBPressed(), bPressed, bPrev, bReleased);
        updateButton(serialRS485.isCPressed(), cPressed, cPrev, cReleased);
        updateButton(serialRS485.isDPressed(), dPressed, dPrev, dReleased);
        updateButton(serialRS485.isDPressed(), ePressed, ePrev, eReleased);
        updateButton(serialRS485.isEPressed(), fPressed, fPrev, fReleased);
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
    void updateButton(bool value, bool &current, bool &previous, bool &released) {
        bool now = (value == LOW);
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

#endif