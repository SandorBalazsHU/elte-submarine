#ifndef RS485RECEIVE_H
#define RS485RECEIVE_H

#include <SoftwareSerial.h>

class RS485Receive {
private:
    SoftwareSerial rs485Serial;
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

    RS485Receive(int rxPin, int txPin)
        : rs485Serial(rxPin, txPin) {}

    int getX() const { return joyX; }
    int getY() const { return joyY; }

    bool isSPressed() const { return buttonS; }
    bool isAPressed() const { return buttonA; }
    bool isBPressed() const { return buttonB; }
    bool isCPressed() const { return buttonC; }
    bool isDPressed() const { return buttonD; }
    bool isEPressed() const { return buttonE; }
    bool isFPressed() const { return buttonF; }


    void init(long baudrate = 115200) {
        rs485Serial.begin(baudrate);
    }

    void update() {
        if (rs485Serial.available()) {
            String message = rs485Serial.readStringUntil('\n');
            message.trim(); // felesleges whitespace eltávolítása

            // Az üzenet darabolása vesszők mentén
            int values[9]; // 9 értéket várunk
            int lastIndex = 0;
            int index = 0;

            for (int i = 0; i < 9; ++i) {
                index = message.indexOf(',', lastIndex);
                if (index == -1 && i < 8) {
                    Serial.println("Hibás formátum: hiányzó érték.");
                    return;
                }

                String part = (i < 8) ? message.substring(lastIndex, index) : message.substring(lastIndex);
                values[i] = part.toInt();
                lastIndex = index + 1;
            }

            if(false){
              Serial.print("X: "); Serial.print(values[0]);
              Serial.print(" | Y: "); Serial.print(values[1]);
              Serial.print(" | SW: "); Serial.print(values[2]);
              Serial.print(" | A: "); Serial.print(values[3]);
              Serial.print(" | B: "); Serial.print(values[4]);
              Serial.print(" | C: "); Serial.print(values[5]);
              Serial.print(" | D: "); Serial.print(values[6]);
              Serial.print(" | E: "); Serial.print(values[7]);
              Serial.print(" | F: "); Serial.println(values[8]);
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
        }else{
            Serial.println("RS485 NOT AVAILABLE!");
        }
    }
};

#endif