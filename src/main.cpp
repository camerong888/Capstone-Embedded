#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>
#include <imu.h>

// Global Variables
#define MIN_LOG_FREQUENCY 1000 // the max time length between logs (in ms)
#define BAUDRATE 557600

// Helper Functions
dataFormat_t *testdata();

TEENSY teensy(BAUDRATE);
LOGGER logger(MIN_LOG_FREQUENCY);
GNSS gnss;
IMU imu;
IRIDIUM rockblock;

void setup()
{
  teensy.DEVICE_init();
  logger.LOGGER_init();
  delay(100);
  teensy.GPIO_init();
  delay(100);
  gnss.Gnss_init();
  delay(250);
  imu.IMU_init();
  delay(100);
  imu.IMU_version();
  delay(100);
  imu.setReports();
  delay(100);
  rockblock.ROCKBLOCK_init();
  delay(100);
  rockblock.firmware_version();
  delay(100);
  rockblock.turnoff();

  if (0)
  {
    while (gnss.getSIV() == 0)
    {
      DPRINT("Waiting... ");
      delay(500);
    }
    rockblock.send_message(teensy.deviceID(), gnss.getLatitude(), gnss.getLongitude());
  }
}

void loop()
{
  teensy.LED_TOGGLE(ORANGE_LED);

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
  //imu.wasReset();
  imu.getSensorEvent(imu.sensorValue);
  imu.getData();
  DPRINTLN("");

  // Iridium Check
  // rockblock.test_signal_quality();
  DPRINTLN("");
  delay(1000);
  // rockblock.turnon();
}

// Helper function definitions:

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