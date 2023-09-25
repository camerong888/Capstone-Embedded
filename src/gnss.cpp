#include <SparkFun_u-blox_GNSS_v3.h>
#include <Wire.h>
#include "gnss.h"
#include "debug.h"

static SFE_UBLOX_GNSS gnss;
static HardwareSerial *port;

GNSS::GNSS(){
    #ifdef GNSS_SERIAL{ //Serial init
        port = GNSS_SERIAL;
        (*port).begin(GNSS_BAUD_RATE);
        delay(100);
        do {
            if (gnss.begin(GNSS_SERIAL)) {
            gnss.setSerialRate(GNSS_BAUD_RATE);
            gnss.setUART1Output(COM_TYPE_UBX);
            gnss.setI2COutput(COM_TYPE_UBX);
            gnss.saveConfiguration();
            DPRINTLN(F("GNSS module serial connection successfully initialized!"));
            return GNSS_STATUS::GNSS_SUCCESS;
            }
            DPRINTLN(F("Failed to initialize GNSS module..."));
            delay(1000);
        } while (1);
    #else //I2C init
        
    #endif
}


// bool GNSS::verifyFunctionality()
// {
//     uint8_t msg[1];
//     uint8_t cmd[1]= {SAM_M10Q_DEVID_REG};
//     I2Cwrite(cmd,1);
//     if(I2Cread(msg,1))
//     {
//         if(msg[0] == SAM_M10Q_DEVID)
//         {
//             return true;
//         }
//         return false;
//     }
//     return false;
// }



