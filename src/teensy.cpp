#include "teensy.h"

TEENSY::TEENSY(int baudrate){
    while (!Serial) { delay(10); }
    Serial.begin(baudrate);
    DPRINTLN(F("Initialized Serial Communication @ {baudrate} baud rate!"));
    delay(10);
    Wire.begin();
    DPRINTLN(F("Initialized I2C!"));
    delay(10);
}

TEENSY::~TEENSY(){}

void TEENSY::GPIO_init(){
    //LED Digial Pins
    pinMode(ORANGE_LED, OUTPUT);
    pinMode(GREEN1, OUTPUT);
    pinMode(RED1, OUTPUT);
    pinMode(GREEN2, OUTPUT);
    pinMode(RED2, OUTPUT);
    pinMode(GREEN3, OUTPUT);
    pinMode(RED3, OUTPUT);
    //LED pin initialization
    digitalWrite(ORANGE_LED, HIGH);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(RED1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(RED2, HIGH);
    digitalWrite(GREEN3, HIGH);
    digitalWrite(RED3, HIGH);
    //Digital outputs
    pinMode(IMU_RST, OUTPUT);
    digitalWrite(IMU_RST, HIGH);
    pinMode(GNSS_EXTINT, OUTPUT);
    digitalWrite(GNSS_EXTINT, LOW);
    pinMode(GNSS_RST, OUTPUT);
    digitalWrite(GNSS_RST, HIGH);
    pinMode(ROCKBLOCK_ONOFF, OUTPUT);
    digitalWrite(ROCKBLOCK_ONOFF, LOW);
    //Digital Inputs
    pinMode(IMU_INT, INPUT);
    pinMode(PWR_SRC, INPUT);
    pinMode(CHARGE_STAT1, INPUT);
    pinMode(CHARGE_STAT2, INPUT);
    pinMode(ROCKBLOCK_NETAV, INPUT);
    pinMode(ROCKBLOCK_RI, INPUT);
    pinMode(BT1, INPUT);
    pinMode(BT2, INPUT);
    pinMode(BT3, INPUT);
    //Analog Inputs
    pinMode(SOC, INPUT);
}

void TEENSY::DEVICE_init(){
    //sam_m10q = GNSS();
//     rockblock9603 = ROCKBLOCK9603():
//     bno085 = BNO085;
}

void TEENSY::LED_TOGGLE(int led)
{
  if (digitalRead(led) == HIGH){
    digitalWrite(led, LOW);
  } else
  {
    digitalWrite(led, HIGH);
  }
}

