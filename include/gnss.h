/**
 * @file gnss.h
 * @brief Utilities to receive GNSS data
 */

#ifndef GNSS_H
#define GNSS_H

#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_v3.h>
#include <Wire.h>
#include "debug.h"

// GNSS Variables
// #define GNSS_SERIAL Serial1 //Uncomment for GNSS serial communiaction
// #define GNSS_BAUD_RATE 38400 //Uncomment for GNSS serial communiaction
// #define SAM_M10Q_DEVID_REG
// #define SAM_M10Q_DEVID
#define SAM_M10Q_I2C_ADR 0x84

/* Return status codes for GNSS functions. */
typedef enum
{
  GNSS_SUCCESS = 0,
  GNSS_ERROR_INIT = 1
} GNSS_STATUS;

/*********************************************/
class GNSS
{
private:
  SFE_UBLOX_GNSS myGNSS;
  bool gnssConnected;

public:
  GNSS();

  ~GNSS();

  GNSS_STATUS Gnss_init();
  bool checkUblox();
  /*
  The fix type is as follows:
  0 = no fix
  1 = dead reckoning (requires external sensors)
  2 = 2D (not quite enough satellites in view)
  3 = 3D (the standard fix)
  4 = GNSS + dead reckoning (requires external sensors)
  5 = Time fix only
  */
  byte getFixType();
  int32_t getLatitude();
  int32_t getLongitude();
  int32_t getAltitude();
  int32_t getGroundSpeed();
  int32_t getHeading();
  byte getSIV();
  String getDate();
  void getTime();
  uint32_t getUnixEpoch();
  void flushPVT();
  void setNavigationFrequency(int n);
};

#endif