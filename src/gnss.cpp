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
        while (1)
            ;
    }

    myGNSS.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); // Set the I2C port to output both NMEA and UBX messages
    myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); // Save (only) the communications port settings to flash and BBR

    // This will pipe all NMEA sentences to the serial port so we can see them
    // myGNSS.setNMEAOutputPort(Serial);
    DPRINTLN(F("GNSS Module Initalized"));
}

bool GNSS::checkUblox()
{
    return myGNSS.checkUblox();
}

int32_t GNSS::getLatitude()
{
    int32_t latitude = myGNSS.getLatitude();
    DPRINT(F("Lat: "));
    DPRINT(latitude);
    return latitude;
}
int32_t GNSS::getLongitude()
{
    int32_t longitude = myGNSS.getLongitude();
    DPRINT(F(" Long: "));
    DPRINT(longitude);
    DPRINT(F(" (degrees * 10^-7)"));
    return longitude;
}
int32_t GNSS::getAltitude()
{
    int32_t altitude = myGNSS.getAltitude();
    DPRINT(F(" Alt: "));
    DPRINT(altitude);
    DPRINT(F(" (mm) "));
    return altitude;
}
int32_t GNSS::getGroundSpeed()
{
    int32_t speed = myGNSS.getGroundSpeed();
    DPRINT("Speed ");
    DPRINT(speed);
    DPRINT(F(" (mm/s) "));
    return speed;
}
int32_t GNSS::getHeading()
{
    int32_t heading = myGNSS.getHeading();
    DPRINT("Heading: ");
    DPRINT(heading);
    DPRINT(F(" (degrees * 10^-5) "));
    return heading;
}
byte GNSS::getSIV()
{
    byte SIV = myGNSS.getSIV();
    DPRINT(F(" SIV: "));
    DPRINTLN(SIV);
    return SIV;
}

void GNSS::flushPVT()
{
    myGNSS.flushPVT();
}

String GNSS::getDate()
{
    int day = myGNSS.getDay();
    int month = myGNSS.getMonth();
    int year = myGNSS.getYear() % 100;
    char date[9]; // MM-DD-YY\0
    snprintf(date, sizeof(date), "%02d-%02d-%02d", month, day, year);
    DPRINT("Date: ");
    DPRINT(month);
    DPRINT("-");
    DPRINT(day);
    DPRINT("-");
    DPRINT(year);
    DPRINT(" ");
    return String(date);
}

void GNSS::getTime()
{
    int UTChour = myGNSS.getHour();
    int ESThour = UTChour - 4;
    if (ESThour < 0)
    {
        ESThour += 24;
    }
    DPRINT(ESThour);
    DPRINT(":");
    DPRINT(myGNSS.getMinute());
    DPRINT(":");
    DPRINTLN(myGNSS.getSecond());
}

byte GNSS::getFixType()
{
    return myGNSS.getFixType();
}

void GNSS::setNavigationFrequency(int n = 1)
{
    myGNSS.setNavigationFrequency(n);
}

uint32_t GNSS::getUnixEpoch()
{
    return myGNSS.getUnixEpoch();
}