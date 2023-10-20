#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>
#include <imu.h>
#include <Bounce2.h>

// Global Variables
#define MIN_LOG_FREQUENCY 10000 // the max time length between logs (in ms)
#define BAUDRATE 557600

// Debounce objects
Bounce debouncerBt1 = Bounce();
Bounce debouncerBt2 = Bounce();

// Helper Functions
void handleBt1StateChange();
void handleBt2StateChange();
void SOS_Sequence();
void waitForLocationLock();
void CreateNewFile();

dataFormat_t data;


// Module Objects
TEENSY teensy(BAUDRATE);
LOGGER logger(MIN_LOG_FREQUENCY);
GNSS gnss;
IMU imu;
IRIDIUM rockblock;

void setup()
{
  // Logger Init
  logger.LOGGER_init();
  delay(100);
  // Teensy Init
  teensy.DEVICE_init();
  teensy.GPIO_init();
  teensy.LEDs_off();
  delay(100);
  // GNSS Init
  gnss.Gnss_init();
  gnss.setNavigationFrequency(1); //Produce one navigation solution per second
  delay(250);
  // IMU Init
  imu.IMU_init();
  delay(100);
  imu.IMU_version();
  delay(100);
  imu.setReports();
  delay(100);
  // Rockblock Init
  rockblock.ROCKBLOCK_init();
  delay(100);
  rockblock.firmware_version();
  delay(100);
  rockblock.turnoff();
  // BT Debounce Handeling
  debouncerBt1.attach(BT1, INPUT_PULLUP);
  debouncerBt1.interval(10);               // Set debounce interval to 5ms
  debouncerBt2.attach(BT2, INPUT_PULLUP); // Same for BT2
  debouncerBt2.interval(10);
  // Button Interrupt Handling
  attachInterrupt(digitalPinToInterrupt(BT1), handleBt1StateChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BT2), handleBt2StateChange, CHANGE);
}

void loop()
{
  debouncerBt1.update();
  debouncerBt2.update();

  teensy.LED_TOGGLE(ORANGE_LED);

  imu.getSensorEvent(imu.sensorValue);
  imu.getData();

  String date = gnss.getDate();
  data.gnssFixType = gnss.getFixType();
  data.iridiumMessageCount = rockblock.getNumberMessagesSent();
  data.epochTime = gnss.getUnixEpoch();
  data.latitude = gnss.getLatitude();
  data.longitude = gnss.getLongitude();
  data.altitude = gnss.getAltitude();
  data.speed = gnss.getGroundSpeed();
  data.heading = gnss.getHeading();
  data.stepCount = imu.getStepCount();

  if (logger.Log_Status())
  {
    logger.BufferData(&data);
    logger.Write(date);
  }

  gnss.flushPVT(); // Marks data as read/stale
  delay(1000);
}

// Helper function definitions:

// Updated interrupt handlers
void handleBt1StateChange()
{
  debouncerBt1.update();
  delay(25);
  if (digitalRead(BT1) == HIGH && digitalRead(BT2) == LOW)
  {
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
  debouncerBt2.update();
  delay(25);
  if (digitalRead(BT1) == LOW && digitalRead(BT2) == HIGH)
  {
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
    else
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
  DPRINT("Sending message...");
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
  DPRINT(gnss.getFixType());
  String currentDate = gnss.getDate();
  logger.LOGGER_newFile(currentDate);
  logger.beginLogging();
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
