/**
 * @file imu.h
 * @brief Utilities to receive IMU data
 */

#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <Wire.h>
#include "debug.h"

//IMU Variables
#define BNO085_I2C_ADR 0x4A
#define BNO08X_RESET -1

class IMU{
  private:
    uint32_t StepCount;
  public:
    sh2_SensorValue_t sensorValue;
    IMU();
    ~IMU();
    void IMU_init();
    void IMU_version();
    void setReports();
    void wasReset();
    void getSensorEvent(sh2_SensorValue_t &sensorValue);
    void printActivity(uint8_t activity_id);
    void getData();
    uint32_t getStepCount();
};

#endif