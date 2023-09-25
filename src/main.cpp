#include <teensy.h>
#include <logger.h>
#include <rockblock9603.h>
#include <debug.h>

//Global Variables
#define MIN_LOG_FREQUENCY 1000 // the max time length between logs (in ms)

//Helper Functions
void ORANGE_LED_TOGGLE();
dataFormat_t* testdata();

TEENSY teensy;
LOGGER logger(MIN_LOG_FREQUENCY);

//Runs once:
void setup() {
  logger.init();
  
  pinMode(ORANGE_LED, OUTPUT);
  digitalWrite(ORANGE_LED, HIGH);
  
}


void loop() {
  ORANGE_LED_TOGGLE();

  dataFormat_t *testData = testdata();
  logger.BufferData(testData);
  logger.Write();

  delay(1000);
}


//Helper function definitions:
void ORANGE_LED_TOGGLE()
{
  if (digitalRead(ORANGE_LED) == HIGH){
    digitalWrite(ORANGE_LED, LOW);
  } else
  {
    digitalWrite(ORANGE_LED, HIGH);
  }
}

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


