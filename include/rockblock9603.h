
/**
 * @file imu.h
 * @brief Utilities to receive IMU data
 */

#ifndef ROCKBLOCK_H
#define ROCKBLOCK_H

#include <IridiumSBD.h>
#include "teensy.h"
#include "debug.h"
#define IridiumSerial Serial2
#define DIAGNOSTICS false // Change this to see diagnostics

class IRIDIUM{
  private:
    int signalQuality = -1;
    int err;
    bool sleep;
    uint8_t NumberMessagesSent;
  public:
    void ROCKBLOCK_init();
    void firmware_version();
    void test_signal_quality();
    void send_message(long lat, long lon);
    bool sleepstatus();
    void turnoff();
    void turnon();
    int getMessageResult();
    uint8_t getNumberMessagesSent();
};

#endif

//Iridium notes:

/*
To open up serial connection on MAC:
/dev/cu.DEVICE
screen dev/cu.DEVICE 19200

AT Commands to send a basic message:

Test Connection: AT
RESPONSE: OK

Check Signal Strength: AT+CSQ
RESPONSE: +CSQ:0-5\n\nOK

Send the Message: AT+SBDWT=TEXT MESSAGE
Response: OK

Initiate Sending of Message: AT+SBDIX
Response: +SBDIX: 0, 1, 0, 0, 0, 0

OK

*/ 
