#include "teensy.h"
#include "debug.h"


TEENSY::TEENSY(){
    Serial.begin(57600);
    DPRINTLN(F("Initialized Serial Communication @ 57600 baud rate!"));
    delay(100);
    Wire.begin();
    DPRINTLN(F("Initialized I2C!"));
    delay(100);
}

TEENSY::~TEENSY(){}

// bool TEENSY::initDevices(){
//     sam_m10q = SAM_M10Q();
//     rockblock9603 = ROCKBLOCK9603():
//     bno085 = BNO085;
// }

