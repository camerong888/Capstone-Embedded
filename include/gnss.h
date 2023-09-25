/**
 * @file gnss.h
 * @brief Utilities to receive GNSS data
 */

#ifndef GNSS_H
#define GNSS_H

#include <Arduino.h>

//GNSS Variables
//#define GNSS_SERIAL Serial1 //Uncomment for GNSS serial communiaction
//#define GNSS_BAUD_RATE 38400 //Uncomment for GNSS serial communiaction
#define SAM_M10Q_DEVID_REG 
#define SAM_M10Q_DEVID 


/* Return status codes for GNSS functions. */
typedef enum {
  GNSS_SUCCESS    = 0,
  GNSS_ERROR_INIT = 1
} GNSS_STATUS;

/*********************************************/
class GNSS
{
    private:
        uint32_t epochTime;
        uint32_t latitude;
        uint32_t longitude;
        uint32_t heading;
        uint32_t altitude;
        uint32_t speed;
        bool gnssConnected;

        /**
         * @brief Writing to the local I2C bus with the address of the GNSS Module
         * 
         * @param cmd 
         * @param num_bytes
         */
        void I2Cwrite(uint8_t *cmd, uint8_t num_bytes);

        /**
         * @brief Requesting data to read in from the GNSS Module
         * 
         * @param msg 
         * @param num_bytes 
         * @return true 
         * @return false 
         */
        bool I2Cread(uint8_t *msg, uint8_t num_bytes);

    public:
        GNSS();

        ~GNSS();
        
        bool verifyFunctionality();

        GNSS_STATUS GnssGetEpochTime();
        uint32_t accessepochTime();
        GNSS_STATUS GnssGetLatitude();
        GNSS_STATUS GnssGetLongitude();
        GNSS_STATUS GnssGetAltitude();
        GNSS_STATUS GnssGetGroundSpeed();
        GNSS_STATUS GnssGetFixOk();
        GNSS_STATUS GnssGetHeading();

};

#endif