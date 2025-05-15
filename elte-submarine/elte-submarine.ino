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



oid setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
