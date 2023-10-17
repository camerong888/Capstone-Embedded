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

long GNSS::getLatitude()
{
    long latitude = myGNSS.getLatitude();
    DPRINT(F("Lat: "));
    DPRINT(latitude);
    return latitude;
}
long GNSS::getLongitude()
{
    long longitude = myGNSS.getLongitude();
    DPRINT(F(" Long: "));
    DPRINT(longitude);
    DPRINTLN(F(" (degrees * 10^-7)"));
    return longitude;
}
void GNSS::getAltitude()
{
    long altitude = myGNSS.getAltitude();
    DPRINT(F(" Alt: "));
    DPRINT(altitude);
    DPRINT(F(" (mm)"));
}
byte GNSS::getSIV()
{
    byte SIV = myGNSS.getSIV();
    DPRINT(F(" SIV: "));
    DPRINTLN(SIV);
    return SIV;
}
