#include <rockblock9603.h>

// Declare the IridiumSBD object
IridiumSBD modem(IridiumSerial);

void IRIDIUM::ROCKBLOCK_init()
{
    IridiumSerial.begin(19200);
    delay(10);
    DPRINTLN("Starting modem...");
    err = modem.begin();
    if (err != ISBD_SUCCESS)
    {
        DPRINT("Begin failed: error ");
        DPRINTLN(err);
        if (err == ISBD_NO_MODEM_DETECTED)
            DPRINTLN("No modem detected: check wiring.");
        return;
    }
    NumberMessagesSent = 0;
}

void IRIDIUM::firmware_version()
{
    if (sleepstatus())
    {
        turnon();
    }
    char version[12];
    err = modem.getFirmwareVersion(version, sizeof(version));
    if (err != ISBD_SUCCESS)
    {
        DPRINT("FirmwareVersion failed: error ");
        DPRINTLN(err);
        return;
    }
    DPRINT("Firmware Version is ");
    DPRINT(version);
    DPRINTLN(".");
}

void IRIDIUM::test_signal_quality()
{
    if (sleepstatus())
    {
        turnon();
    }
    // This returns a number between 0 and 5.
    // 2 or better is preferred.
    err = modem.getSignalQuality(signalQuality);
    if (err != ISBD_SUCCESS)
    {
        DPRINT("SignalQuality failed: error ");
        DPRINTLN(err);
        return;
    }

    DPRINT("On a scale of 0 to 5, signal quality is currently ");
    DPRINT(signalQuality);
    DPRINTLN(".");
}

void IRIDIUM::send_message(long lat, long lon)
{
    if (sleepstatus())
    {
        turnon();
    }
    DPRINTLN("Trying to send the message.  This might take several minutes.\r\n");
    String message = "lat: " + String(lat) +
                     "; lon: " + String(lon);
    DPRINT("Message: ");
    DPRINTLN(message);
    err = modem.sendSBDText(message.c_str());
    if (err != ISBD_SUCCESS)
    {
        DPRINT("sendSBDText failed: error ");
        DPRINTLN(err);
        if (err == ISBD_SENDRECEIVE_TIMEOUT)
            DPRINTLN("Try again with a better view of the sky.");
    }

    else
    {
        DPRINTLN("Hey, it worked!");
        NumberMessagesSent = NumberMessagesSent + 1;
    }
}

void IRIDIUM::turnoff()
{
    digitalWrite(ROCKBLOCK_ONOFF, HIGH);
    sleep = 1;
}

void IRIDIUM::turnon()
{
    digitalWrite(ROCKBLOCK_ONOFF, LOW);
    sleep = 0;
    delay(500);
    //ROCKBLOCK_init();
}

bool IRIDIUM::sleepstatus()
{
    return sleep;
}

int IRIDIUM::getMessageResult()
{
    return err;
}

uint8_t IRIDIUM::getNumberMessagesSent()
{
    return NumberMessagesSent;
}