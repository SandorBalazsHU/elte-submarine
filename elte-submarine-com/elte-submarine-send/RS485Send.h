#include <string.h>

class RS485Send {
private:
    String message;

public:
    RS485Send() {}

    void init(long baudrate = 9600) {
        Serial.begin(baudrate);
    };
    String getMessage() {
      return message;
    }

    void send(int x, int y, bool sw, bool a, bool b, bool c, bool d, bool e, bool f) {
        //delayMicroseconds(10);             // kis késleltetés
        String message2device = "";
        message2device += String(x);
        message2device += ",";
        message2device += String(y);
        message2device += ",";
        message2device += String(sw);
        message2device += ",";
        message2device += String(a);
        message2device += ",";
        message2device += String(b);
        message2device += ",";
        message2device += String(c);
        message2device += ",";
        message2device += String(d);
        message2device += ",";
        message2device += String(e);
        message2device += ",";
        message2device += String(f);
        
        Serial.println(message2device);  // üzenet küldése új sorral
        message = message2device;
        Serial.flush();           // megvárjuk a küldés végét
    };
};
