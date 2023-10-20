#include "teensy.h"

TEENSY::TEENSY(int baudrate)
{
  while (!Serial && 0)
  {
    delay(10);
  }
  Serial.begin(baudrate);
  DPRINTLN(F("Initialized Serial Communication @ {baudrate} baud rate!"));
  delay(10);
  Wire.begin();
  DPRINTLN(F("Initialized I2C!"));
  delay(10);
}

TEENSY::~TEENSY() {}

void TEENSY::GPIO_init()
{
  // LED Digial Pins
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN3, OUTPUT);
  pinMode(RED3, OUTPUT);
  // LED pin initialization
  digitalWrite(ORANGE_LED, HIGH);
  digitalWrite(GREEN1, HIGH);
  digitalWrite(RED1, HIGH);
  digitalWrite(GREEN2, HIGH);
  digitalWrite(RED2, HIGH);
  digitalWrite(GREEN3, HIGH);
  digitalWrite(RED3, HIGH);
  // Digital outputs
  pinMode(IMU_RST, OUTPUT);
  digitalWrite(IMU_RST, HIGH);
  pinMode(GNSS_EXTINT, OUTPUT);
  digitalWrite(GNSS_EXTINT, LOW);
  pinMode(GNSS_RST, OUTPUT);
  digitalWrite(GNSS_RST, HIGH);
  pinMode(ROCKBLOCK_ONOFF, OUTPUT);
  digitalWrite(ROCKBLOCK_ONOFF, LOW);
  // Digital Inputs
  pinMode(IMU_INT, INPUT);
  pinMode(PWR_SRC, INPUT);
  pinMode(CHARGE_STAT1, INPUT);
  pinMode(CHARGE_STAT2, INPUT);
  pinMode(ROCKBLOCK_NETAV, INPUT);
  pinMode(ROCKBLOCK_RI, INPUT);
  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
  pinMode(BT3, INPUT);
  // Analog Inputs
  pinMode(SOC, INPUT);
}

void TEENSY::DEVICE_init()
{
  uid[0] = HW_OCOTP_MAC0;
  uid[1] = HW_OCOTP_MAC1;
  for (int i = 0; i < 2; ++i)
  {
    DPRINTF("UID%d: 0x%08x\n", i, uid[i]);
  }
}

uint32_t *TEENSY::deviceID()
{
  return uid;
}

void TEENSY::LED_TOGGLE(int led)
{
  if (digitalRead(led) == HIGH)
  {
    digitalWrite(led, LOW);
  }
  else
  {
    digitalWrite(led, HIGH);
  }
}

void TEENSY::LEDs_off()
{
  digitalWrite(GREEN1, HIGH);
  digitalWrite(RED1, HIGH);
  digitalWrite(GREEN2, HIGH);
  digitalWrite(RED2, HIGH);
  digitalWrite(GREEN3, HIGH);
  digitalWrite(RED3, HIGH);
}

void TEENSY::SOC_LED()
{

  LEDs_off();
  int socReading = analogRead(SOC); // 2.5V+/- .1V if charging, otherwise SOC = 1/2 of VBAT, where VBAT operates between 2.5V and 4.2V with a nominal voltage of 3.7V.
  float voltage = (socReading / 1023.0) * 3.35;
  DPRINT("SOC Voltage: ");
  DPRINTLN(voltage);
  if (voltage >= 2.4 && voltage <= 2.6)
  {
    digitalWrite(GREEN1, LOW);
    delay(500);
    digitalWrite(GREEN2, LOW);
    delay(500);
    digitalWrite(GREEN3, LOW);
    delay(500);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, HIGH);
    delay(500);
    digitalWrite(GREEN1, LOW);
    delay(500);
    digitalWrite(GREEN2, LOW);
    delay(500);
    digitalWrite(GREEN3, LOW);
    delay(500);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, HIGH);
    delay(500);
    digitalWrite(GREEN1, LOW);
    delay(500);
    digitalWrite(GREEN2, LOW);
    delay(500);
    digitalWrite(GREEN3, LOW);
  }
  else
  {
    float batteryPercentage = (voltage - 1.25) / (2.1 - 1.25) * 100;
    if (batteryPercentage >= 0 && batteryPercentage <= 33)
    {
      digitalWrite(GREEN1, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(RED3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(RED3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(RED3, LOW);
    }
    else if (batteryPercentage > 33 && batteryPercentage <= 66)
    {
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(RED3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(GREEN2, HIGH);
      digitalWrite(RED3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(RED3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(GREEN2, HIGH);
      digitalWrite(RED3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(RED3, LOW);
    }
    else if (batteryPercentage > 66 && batteryPercentage <= 100)
    {
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(GREEN2, HIGH);
      digitalWrite(GREEN3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      delay(500);
      digitalWrite(GREEN1, HIGH);
      digitalWrite(GREEN2, HIGH);
      digitalWrite(GREEN3, HIGH);
      delay(500);
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
    }
  }

  delay(500);
  LEDs_off();
}

void TEENSY::blinkRedLEDs()
{
  LEDs_off();
  LED_TOGGLE(RED1);
  LED_TOGGLE(RED2);
  LED_TOGGLE(RED3);
  delay(500);
  LEDs_off();
  delay(500);
  LED_TOGGLE(RED1);
  LED_TOGGLE(RED2);
  LED_TOGGLE(RED3);
  delay(500);
  LEDs_off();
  delay(500);
  LED_TOGGLE(RED1);
  LED_TOGGLE(RED2);
  LED_TOGGLE(RED3);
  delay(500);
  LEDs_off();
}

void TEENSY::blinkGreenLEDs()
{
  LEDs_off();
  LED_TOGGLE(GREEN1);
  LED_TOGGLE(GREEN2);
  LED_TOGGLE(GREEN3);
  delay(500);
  LEDs_off();
  delay(500);
  LED_TOGGLE(GREEN1);
  LED_TOGGLE(GREEN2);
  LED_TOGGLE(GREEN3);
  delay(500);
  LEDs_off();
  delay(500);
  LED_TOGGLE(GREEN1);
  LED_TOGGLE(GREEN2);
  LED_TOGGLE(GREEN3);
  delay(500);
  LEDs_off();
}