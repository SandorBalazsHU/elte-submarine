#include <SoftwareSerial.h>

class RS485Send {
private:
    SoftwareSerial rs485Serial;

public:
    RS485Send(int rxPin, int txPin)
        : rs485Serial(rxPin, txPin) {}

    void init(long baudrate = 9600) {
        rs485Serial.begin(baudrate);
    };

    void send(int x, int y, bool sw, bool a, bool b, bool c, bool d, bool e, bool f) {
        //delayMicroseconds(10);             // kis késleltetés
        // Alakítsuk szöveggé a küldendő adatokat
        String message = String(x) + "," + String(y) + "," +
                         String(sw) + "," + String(a) + "," + String(b) + "," +
                         String(c) + "," + String(d) + "," + String(e) + "," + String(f);

        rs485Serial.println(message);  // üzenet küldése új sorral
        //rs485Serial.flush();           // megvárjuk a küldés végét
    };
};
