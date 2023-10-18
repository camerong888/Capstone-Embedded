#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>
#include <imu.h>
#include <Bounce2.h>

// Global Variables
#define MIN_LOG_FREQUENCY 1000 // the max time length between logs (in ms)
#define BAUDRATE 557600
#define TESTING 1

// Debounce objects
Bounce debouncerBt1 = Bounce();
Bounce debouncerBt2 = Bounce();

// Helper Functions
void handleBt1StateChange();
void handleBt2StateChange();
void handleBt1andBt2Press();
void handleBt1Press();
void handleBt2Press();

dataFormat_t *testdata();

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
  debouncerBt1.interval(5);               // Set debounce interval to 5ms
  debouncerBt2.attach(BT2, INPUT_PULLUP); // Same for BT2
  debouncerBt2.interval(5);
  // Button Interrupt Handling
  attachInterrupt(digitalPinToInterrupt(BT1), handleBt1StateChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BT2), handleBt2StateChange, CHANGE);
}

void loop()
{
  debouncerBt1.update();
  debouncerBt2.update();
  if (debouncerBt1.rose() && debouncerBt2.rose()) // If both buttons are pressed down and held...
  {
    delay(5000);
    handleBt1andBt2Press();
  }
  else if (debouncerBt1.rose())
  {
    handleBt1Press();
  }
  else if (debouncerBt2.rose())
  {
    handleBt2Press();
  }

  teensy.LED_TOGGLE(ORANGE_LED);

  if (TESTING)
  {
    // dataFormat_t *testData = testdata();
    // logger.BufferData(testData);
    // logger.Write();

    // GNSS Data Collect
    gnss.getLatitude();
    delay(50);
    gnss.getLongitude();
    delay(50);
    gnss.getAltitude();
    delay(50);
    gnss.getSIV();
    DPRINTLN("");
    delay(50);

    // IMU Data Collect
    // imu.wasReset();
    imu.getSensorEvent(imu.sensorValue);
    imu.getData();
    DPRINTLN("");

    // Iridium Check
    // rockblock.test_signal_quality();
    DPRINTLN("");
    // rockblock.turnon();
  }
  delay(1000);
}

// Helper function definitions:

// Updated interrupt handlers
void handleBt1StateChange()
{
  debouncerBt1.update();
}

void handleBt2StateChange()
{
  debouncerBt2.update();
}

void handleBt1andBt2Press()
{
  if (digitalRead(BT1) == HIGH && digitalRead(BT2) == HIGH) // If both buttons are still pressed after 5 seconds
  {
    while (gnss.getSIV() == 0)
    {
      teensy.LED_TOGGLE(RED1);
      teensy.LED_TOGGLE(RED2);
      teensy.LED_TOGGLE(RED3);
      DPRINT("Waiting for location lock... ");
      delay(500);
      teensy.LEDs_off();
      delay(500);
    }
    DPRINT("Sending message...");
    teensy.LED_TOGGLE(GREEN1);
    teensy.LED_TOGGLE(GREEN2);
    teensy.LED_TOGGLE(GREEN3);
    teensy.LED_TOGGLE(RED1);
    teensy.LED_TOGGLE(RED2);
    teensy.LED_TOGGLE(RED3);
    rockblock.send_message(teensy.deviceID(), gnss.getLatitude(), gnss.getLongitude());
    if (rockblock.getMessageResult() != ISBD_SUCCESS )// Blink LEDs if it worked or not...
    {
      teensy.LEDs_off();
      teensy.LED_TOGGLE(RED1);
      teensy.LED_TOGGLE(RED2);
      teensy.LED_TOGGLE(RED3);
      delay(500);
      teensy.LEDs_off();
      delay(500);
      teensy.LED_TOGGLE(RED1);
      teensy.LED_TOGGLE(RED2);
      teensy.LED_TOGGLE(RED3);
      delay(500);
      teensy.LEDs_off();
      delay(500);
      teensy.LED_TOGGLE(RED1);
      teensy.LED_TOGGLE(RED2);
      teensy.LED_TOGGLE(RED3);
      delay(500);
      teensy.LEDs_off();
    }
    else
    {
      teensy.LEDs_off();
      teensy.LED_TOGGLE(GREEN1);
      teensy.LED_TOGGLE(GREEN2);
      teensy.LED_TOGGLE(GREEN3);
      delay(500);
      teensy.LEDs_off();
      delay(500);
      teensy.LED_TOGGLE(GREEN1);
      teensy.LED_TOGGLE(GREEN2);
      teensy.LED_TOGGLE(GREEN3);
      delay(500);
      teensy.LEDs_off();
      delay(500);
      teensy.LED_TOGGLE(GREEN1);
      teensy.LED_TOGGLE(GREEN2);
      teensy.LED_TOGGLE(GREEN3);
      delay(500);
      teensy.LEDs_off();
      rockblock.turnoff();
    }
  }
}

void handleBt1Press()
{
  teensy.SOC_LED();
}

void handleBt2Press()
{
}

dataFormat_t *testdata()
{
  static dataFormat_t testData;

  // Manually setting some test data
  testData.epochTime = 1632674911 + millis();         // Unix time stamp for example
  testData.stepCount = 10 + millis();                 // Step count as an example
  testData.dataBuf[LATITUDE] = 42300000 + millis();   // Latitude in micro degrees (e.g., 42.3 degrees)
  testData.dataBuf[LONGITUDE] = -71060000 + millis(); // Longitude in micro degrees (e.g., -71.06 degrees)
  testData.dataBuf[SPEED] = 50 + millis();            // Speed in km/h for example
  testData.dataBuf[HEADING] = 180 + millis();         // Heading in degrees
  testData.dataBuf[ALTITUDE] = 200 + millis();        // Altitude in meters
  testData.dataBuf[IRIDIUM_INDICATOR] = 1;            // Iridium message indicator

  return &testData;
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