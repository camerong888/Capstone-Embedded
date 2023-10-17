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

//GNSS Variables
//#define GNSS_SERIAL Serial1 //Uncomment for GNSS serial communiaction
//#define GNSS_BAUD_RATE 38400 //Uncomment for GNSS serial communiaction
//#define SAM_M10Q_DEVID_REG 
//#define SAM_M10Q_DEVID 
#define SAM_M10Q_I2C_ADR 0x84


/* Return status codes for GNSS functions. */
typedef enum {
  GNSS_SUCCESS    = 0,
  GNSS_ERROR_INIT = 1
} GNSS_STATUS;

/*********************************************/
class GNSS{
  private:
    SFE_UBLOX_GNSS myGNSS;
    bool gnssConnected;

  public:
    GNSS();

    ~GNSS();

    GNSS_STATUS Gnss_init();
    bool checkUblox();
    long getLatitude();
    long getLongitude();
    void getAltitude();
    byte getSIV();
    

};

#endif