/**
 * @file teensy.h
 * @author Cameron Gordon
 * @brief
 * @date 2023-09-24
 */
#ifndef TEENSY__H
#define TEENSY__H

/*************************************************/
#include <Arduino.h>
#include <Wire.h>
#include "debug.h"
#include "gnss.h"

// Teensy Pinout
#define RX1 (0)
#define TX1 (1)
#define IMU_RST (2)
#define IMU_INT (3)
#define GNSS_EXTINT (4)
#define GNSS_RST (5)
#define RX2 (7)
#define TX2 (8)
#define CS0 (10)
#define MOSI0 (11)
#define MISO0 (12)
#define ORANGE_LED (13)
#define SCL2 (24)
#define SDA2 (25)
#define BT1 (26)
#define BT2 (27)
#define BT3 (28)
#define GREEN2 (29)
#define RED2 (30)
#define GREEN3 (31)
#define RED3 (32)
#define CRX1 (23)
#define CTX1 (22)
#define SCL0 (19)
#define SDA0 (18)
#define SDA1 (17)
#define SCL1 (16)
#define SCK (13)
#define RED1 (41)
#define ROCKBLOCK_ONOFF (40)
#define GREEN1 (39)
#define SOC (A14) // Analog State of Charge
#define ROCKBLOCK_RI (37)
#define ROCKBLOCK_NETAV (36)
#define PWR_SRC (35)
#define CHARGE_STAT2 (34)
#define CHARGE_STAT1 (33)

/*********************************************/
typedef enum
{
    FAULTED = 1,
    NOT_FAULTED = 0
} FaultStatus_t;

/*********************************************/
class TEENSY
{
private:
    bool sleepMode;
    uint32_t uid[2];
    unsigned long buttonPressTime; // Store time when button is pressed
    bool buttonPressed;            // True when button is pressed, false otherwise
public:
    TEENSY(int baudrate);

    ~TEENSY();

    void GPIO_init();

    void LED_TOGGLE(int led);

    void DEVICE_init();

    uint32_t *deviceID();

    void LEDs_off();

    void SOC_LED();

    void blinkRedLEDs();

    void blinkGreenLEDs();
};

#endif