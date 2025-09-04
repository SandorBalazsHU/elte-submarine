#ifndef RS485RECEIVE_H
#define RS485RECEIVE_H

class RS485Receive {
private:
    int    joyX;
    int    joyY;
    int buttonS;
    int buttonA;
    int buttonB;
    int buttonC;
    int buttonD;
    int buttonE;
    int buttonF;

public:

    RS485Receive() {}

    int getX() const { return joyX; }
    int getY() const { return joyY; }

    bool isSPressed() const { return buttonS; }
    bool isAPressed() const { return buttonA; }
    bool isBPressed() const { return buttonB; }
    bool isCPressed() const { return buttonC; }
    bool isDPressed() const { return buttonD; }
    bool isEPressed() const { return buttonE; }
    bool isFPressed() const { return buttonF; }


    void init(long baudrate = 9600) {
        Serial.begin(baudrate);
    }

    void update() {
        if (Serial.available()) {
            String message = Serial.readStringUntil('\n');
            message.trim(); // felesleges whitespace eltávolítása

            // Az üzenet darabolása vesszők mentén
            int values[9]; // 9 értéket várunk
            int lastIndex = 0;
            int index = 0;

            for (int i = 0; i < 9; ++i) {
                index = message.indexOf(',', lastIndex);
                if (index == -1 && i < 8) {
                    return;
                }

                String part = (i < 8) ? message.substring(lastIndex, index) : message.substring(lastIndex);
                values[i] = part.toInt();
                lastIndex = index + 1;
            }

               joyX = values[0];
               joyY = values[1];
            buttonS = values[2];
            buttonA = values[3];
            buttonB = values[4];
            buttonC = values[5];
            buttonD = values[6];
            buttonE = values[7];
            buttonF = values[8];
        }
    }
};

#endif