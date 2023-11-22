/**
 * @file main.cpp
 * @author Cameron Gordon
 * @brief Main driver program for a device interfacing with the Teensy platform. This program initializes and manages various modules such as a GNSS (Global Navigation Satellite System) for location tracking, an IMU (Inertial Measurement Unit) for motion tracking, and the RockBlock 9603 satellite modem for communication. It also provides functionality for data logging with debounce-supported button controls to manage logging, as well as an emergency SOS sequence leveraging the GNSS and RockBlock modules.
 * @date 2023-10-21
 */

#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>
#include <imu.h>
#include <dataformat.h>
#include <Bounce2.h>
#include <Snooze.h>

// Global Variables
#define MIN_LOG_FREQUENCY 20000 // the max time length between logs (in ms)
#define BAUDRATE 557600

// Button Debounce Objects
Bounce debouncerBt1 = Bounce();
Bounce debouncerBt2 = Bounce();

// Sleep configuration
SnoozeDigital snoozeBt1;
SnoozeBlock config_teensy(snoozeBt1);

// Helper Functions
void handleBt1StateChange();
void handleBt2StateChange();
void SOS_Sequence();
void waitForLocationLock();
void CreateNewFile();

// Module Objects
TEENSY teensy(BAUDRATE);
LOGGER logger(MIN_LOG_FREQUENCY);
GNSS gnss;
IMU imu;
IRIDIUM rockblock;

// Data Item
dataFormat_t data;

bool initSuccess = 0;

// Module Initialization
void setup()
{
  // Teensy Init
  initSuccess = imu.IMU_init();
  teensy.DEVICE_init();
  teensy.GPIO_init();
  teensy.LEDs_off();
  teensy.orangeLEDs();
  delay(50);
  // GNSS Init
  gnss.Gnss_init();
  gnss.setNavigationFrequency(1); // Produce one navigation solution per second
  delay(100);
  imu.IMU_version();
  delay(100);
  imu.setReports();
  delay(50);
  // Rockblock Init
  rockblock.ROCKBLOCK_init();
  delay(100);
  rockblock.firmware_version();
  delay(100);
  rockblock.turnoff();
  // Logger Init
  logger.LOGGER_init();
  delay(50);
  snoozeBt1.pinMode(BT1, INPUT_PULLUP, FALLING);
  // BT Debounce Handeling
  debouncerBt1.attach(BT1, INPUT_PULLUP);
  debouncerBt1.interval(10);              // Set debounce interval to 5ms
  debouncerBt2.attach(BT2, INPUT_PULLUP); // Same for BT2
  debouncerBt2.interval(10);
  // Button Interrupt Handling
  attachInterrupt(digitalPinToInterrupt(BT1), handleBt1StateChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BT2), handleBt2StateChange, CHANGE);
  delay(250);
  // IMU Init
  if (!initSuccess)
  {
    Snooze.deepSleep(config_teensy);
  }
  teensy.LEDs_off();
}

// Steady State
void loop()
{
  debouncerBt1.update();
  debouncerBt2.update();

  teensy.LED_TOGGLE(ORANGE_LED);

  imu.getSensorEvent(imu.sensorValue);
  imu.getData();
  DPRINTLN("");
  String date = gnss.getDate();
  data.gnssFixType = gnss.getFixType();
  data.iridiumMessageCount = rockblock.getNumberMessagesSent();
  data.epochTime = gnss.getUnixEpoch();
  data.latitude = gnss.getLatitude();
  data.longitude = gnss.getLongitude();
  data.altitude = gnss.getAltitude();
  data.speed = gnss.getGroundSpeed();
  data.heading = gnss.getHeading();
  gnss.flushPVT(); // Marks GNSS data as read/stale
  data.stepCount = imu.getStepCount();
  delay(500);

  if (logger.Log_Status())
  {
    logger.BufferData(&data);
    logger.Write(date);
  }

  delay(500);
}

// Helper function definitions:
void handleBt1StateChange()
{
  static unsigned long lastPressTime = 0;
  debouncerBt1.update();
  delay(25);
  if (digitalRead(BT1) == HIGH && digitalRead(BT2) == LOW)
  {
    lastPressTime = millis();
    while (digitalRead(BT1) == HIGH)
    {
      if (millis() - lastPressTime > 3000)
      {
        Snooze.deepSleep(config_teensy);
      }
    }
    teensy.SOC_LED();
  }
  else if (digitalRead(BT1) == HIGH && digitalRead(BT2) == HIGH)
  {
    delay(3000);
    if (digitalRead(BT1) == HIGH && digitalRead(BT2) == HIGH)
    {
      SOS_Sequence();
    }
  }
}

void handleBt2StateChange()
{
  static unsigned long lastPressTime = 0;
  bool closedFile = 0;
  debouncerBt2.update();
  delay(25);
  if (digitalRead(BT1) == LOW && digitalRead(BT2) == HIGH)
  {
    lastPressTime = millis();
    while (digitalRead(BT2) == HIGH && logger.isOpen())
    {
      if (millis() - lastPressTime > 3000)
      {
        logger.close();
        closedFile = 1;
        teensy.blinkGreenLEDs();
      }
    }
    if (logger.isOpen()) // Checks if file is open
    {
      if (logger.Log_Status())
      {
        // Pause logging
        logger.pauseLogging();
        teensy.blinkRedLEDs();
        delay(100);
      }
      else
      {
        // Resume logging
        logger.beginLogging();
        teensy.blinkGreenLEDs();
        delay(100);
      }
    }
    else if (!closedFile)
    {
      CreateNewFile();
    }
  }
  else if (digitalRead(BT1) == HIGH && digitalRead(BT2) == HIGH)
  {
    delay(3000);
    if (digitalRead(BT1) == HIGH && digitalRead(BT2) == HIGH)
    {
      SOS_Sequence();
    }
  }
}

void SOS_Sequence()
{
  waitForLocationLock();
  DPRINT("Sending message... ");
  teensy.LED_TOGGLE(GREEN1);
  teensy.LED_TOGGLE(GREEN2);
  teensy.LED_TOGGLE(GREEN3);
  teensy.LED_TOGGLE(RED1);
  teensy.LED_TOGGLE(RED2);
  teensy.LED_TOGGLE(RED3);
  rockblock.send_message(gnss.getLatitude(), gnss.getLongitude());
  if (rockblock.getMessageResult() != ISBD_SUCCESS) // Blink LEDs if it worked or not...
  {
    teensy.blinkRedLEDs();
  }
  else
  {
    teensy.blinkGreenLEDs();
  }
}

void CreateNewFile()
{
  waitForLocationLock();
  String currentDate = gnss.getDate();
  logger.LOGGER_newFile(currentDate);
  logger.beginLogging();
  teensy.blinkGreenLEDs();
}

void waitForLocationLock()
{
  while (gnss.getFixType() < 3)
  {
    teensy.LED_TOGGLE(RED1);
    teensy.LED_TOGGLE(RED2);
    teensy.LED_TOGGLE(RED3);
    DPRINT("Waiting for location lock... ");
    delay(500);
    teensy.LEDs_off();
    delay(500);
  }
}


#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif
