#include <SoftwareSerial.h>

class RS485Receive {
private:
    SoftwareSerial rs485Serial;

public:
    RS485Receive(int rxPin, int txPin)
        : rs485Serial(rxPin, txPin) {}

    void init(long baudrate = 9600) {
        rs485Serial.begin(baudrate);
    }

    void update() {
        Serial.print("started");
        if (rs485Serial.available()) {
            Serial.print("available");
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

            // Kiírás a soros monitorra
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
    }
};
