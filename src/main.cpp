#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>
#include <imu.h>

//Global Variables
#define MIN_LOG_FREQUENCY 1000 // the max time length between logs (in ms)
#define BAUDRATE 557600

//Helper Functions
dataFormat_t* testdata();

TEENSY teensy(BAUDRATE);
LOGGER logger(MIN_LOG_FREQUENCY);
GNSS gnss;
IMU imu;

//Runs once:
void setup() {
  logger.LOGGER_init();
  teensy.GPIO_init();
  gnss.Gnss_init();
  imu.IMU_init();
  imu.IMU_version();
  imu.setReports();
  delay(100);
}


void loop() {
  teensy.LED_TOGGLE(ORANGE_LED);

  //dataFormat_t *testData = testdata();
  //logger.BufferData(testData);
  //logger.Write();

  //GNSS Data Collect
  //gnss.checkUblox();
  gnss.getLatitude();
  delay(50);
  gnss.getLongitude();
  delay(50);
  gnss.getAltitude();
  delay(50);
  gnss.getSIV();
  DPRINTLN("");
  DPRINTLN("");
  delay(50);

  //IMU Data Collect
  imu.wasReset();
  imu.getSensorEvent(imu.sensorValue);
  imu.getData();
  DPRINTLN("");

  //Iridium Check

  delay(1000);
}


//Helper function definitions:

dataFormat_t* testdata() {
  static dataFormat_t testData;

  // Manually setting some test data
  testData.epochTime = 1632674911 + millis();  // Unix time stamp for example
  testData.stepCount = 10 + millis();          // Step count as an example
  testData.dataBuf[LATITUDE] = 42300000 + millis();      // Latitude in micro degrees (e.g., 42.3 degrees)
  testData.dataBuf[LONGITUDE] = -71060000 + millis();    // Longitude in micro degrees (e.g., -71.06 degrees)
  testData.dataBuf[SPEED] = 50 + millis();               // Speed in km/h for example
  testData.dataBuf[HEADING] = 180 + millis();            // Heading in degrees
  testData.dataBuf[ALTITUDE] = 200 + millis();           // Altitude in meters
  testData.dataBuf[IRIDIUM_INDICATOR] = 1;    // Iridium message indicator

  return &testData;
}

