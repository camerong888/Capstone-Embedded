#include "gnss.h"

GNSS::GNSS()
{
}
GNSS::~GNSS() {}

GNSS_STATUS GNSS::Gnss_init()
{
    if (myGNSS.begin() == false)
    {
        DPRINTLN(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
        while (1);
    }

    myGNSS.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); // Set the I2C port to output both NMEA and UBX messages
    myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); // Save (only) the communications port settings to flash and BBR

    // This will pipe all NMEA sentences to the serial port so we can see them
    //myGNSS.setNMEAOutputPort(Serial);
    DPRINTLN(F("GNSS Module Initalized"));
}

bool GNSS::checkUblox()
{
    return myGNSS.checkUblox();
}

void GNSS::getLatitude()
{
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);
}
void GNSS::getLongitude()
{
    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));
}
void GNSS::getAltitude()
{
    long altitude = myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));
}
void GNSS::getSIV()
{
    byte SIV = myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);
}

// #ifdef GNSS_SERIAL{ //Serial init
//         port = GNSS_SERIAL;
//         (*port).begin(GNSS_BAUD_RATE);
//         delay(100);
//         do {
//             if (gnss.begin(GNSS_SERIAL)) {
//             gnss.setSerialRate(GNSS_BAUD_RATE);
//             gnss.setUART1Output(COM_TYPE_UBX);
//             gnss.setI2COutput(COM_TYPE_UBX);
//             gnss.saveConfiguration();
//             DPRINTLN(F("GNSS module serial connection successfully initialized!"));
//             return GNSS_STATUS::GNSS_SUCCESS;
//             }
//             DPRINTLN(F("Failed to initialize GNSS module..."));
//             delay(1000);
//         } while (1);
//     #else //I2C init

//     #endif