#include "logger.h"
#include "debug.h"
#include "gnss.h"

#define MAX_BUFFERED_MESSAGES 50    // max number of buffered messages before logging to SD card
#define MAX_LOG_FILE_SIZE 50000000U // 50 Megabytes
#define FILE_NAME_SIZE 21           // 10 for timestamp, 8 for text, 1 for termination

File logFile;                  // file logging object
char fileName[FILE_NAME_SIZE]; // format is log-1652888997.txt

// Buffer information (use 2 buffers to prevent overwrites during logging delays)
int buf1Length = 0;
int buf2Length = 0;
dataFormat_t messageBuf1[MAX_BUFFERED_MESSAGES];
dataFormat_t messageBuf2[MAX_BUFFERED_MESSAGES];
bool usingBuf1 = true;

/**
 * @brief Sets the logging file name using the format log-NUM.txt, where NUM is either the current
 *        unix time if using the RTC or a valid numeric index starting from 0.
 *
 */
void LOGGER::LOGGER_newFile(String date)
{
  uint32_t fileNum = 1;
  snprintf(fileName, FILE_NAME_SIZE, "%s_Trip_#%lu.txt", date.c_str(), fileNum);
  while (SD.exists(fileName))
  {
    fileNum++;
    snprintf(fileName, FILE_NAME_SIZE, "%s_Trip_#%lu.txt", date.c_str(), fileNum);
  }
  DPRINT(F("SD card file created. File name is "));
  DPRINTLN(fileName);

  logFile = SD.open(fileName, FILE_WRITE);
}

LOGGER::LOGGER(uint32_t logfrequency = 10000)
{
  LogFrequency = logfrequency;
  delay(100);
}

LOGGER::~LOGGER() {}

LOGGER_STATUS LOGGER::LOGGER_init()
{
  if (!SD.begin(BUILTIN_SDCARD))
  {
    DPRINTLN(F("Error initializing SD card logging..."));
    initialized = false;
    return LGR_ERROR_SD_CARD;
  }

  initialized = true;

  DPRINTLN(F("Initialized SD card!"));
  return LGR_SUCCESS;
}

bool LOGGER::Log_Status()
{
  return log;
}

LOGGER_STATUS LOGGER::BufferData(dataFormat_t *message)
{
  if (!initialized)
  {
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }

  noInterrupts();
  // find appropriate buffer to use
  dataFormat_t *messageBuf = usingBuf1 ? messageBuf1 : messageBuf2;
  int bufLength = usingBuf1 ? buf1Length : buf2Length;

  if (bufLength >= MAX_BUFFERED_MESSAGES)
  {
    DPRINTLN(F("Tried to write to already full buffer"));
    return LOGGER_STATUS::LGR_ERROR_BUFFER_FULL;
  }

  messageBuf[bufLength] = *message;

  if (usingBuf1)
  {
    buf1Length++;
  }
  else
  {
    buf2Length++;
  }
  interrupts();

  return LOGGER_STATUS::LGR_SUCCESS;
}

LOGGER_STATUS LOGGER::Write(String date)
{
  if (!initialized)
  {
    DPRINTLN("Cannot write to SD card, not initialized...");
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }

  // find appropriate buffer to use
  dataFormat_t *messageBuf = usingBuf1 ? messageBuf1 : messageBuf2;
  int bufLength = usingBuf1 ? buf1Length : buf2Length;

  // only write on certain conditions
  if ((millis() - LastLogTime) <= LogFrequency && bufLength < MAX_BUFFERED_MESSAGES)
  {
    return LOGGER_STATUS::LGR_ERROR_NO_WRITE;
  }

  usingBuf1 = !usingBuf1; // Switch main log buffer during write process

  // create a new file if current one is too large
  if (logFile.size() > MAX_LOG_FILE_SIZE)
  {
    DPRINTLN("Max File size reached, creating a new log file...");
    logFile.close();
    LOGGER_newFile(date);
    logFile = SD.open(fileName, FILE_WRITE);
  }

  DPRINT(F(" -- "));
  DPRINT(F("Added from buf "));
  if (usingBuf1)
  {
    DPRINT(F("2: "));
  }
  else
  {
    DPRINT(F("1: "));
  }
  DPRINT(bufLength);
  DPRINT(F(" -- \n"));

  static dataFormat_t lastEntry;
  if (logFile)
  { // Need to update
    DPRINTLN(F("Writing to SD card..."));

    // write all buffered messages to the SD card
    for (int i = 0; i < bufLength; i++)
    {
      if (i == 0 && isSameEntry(lastEntry, messageBuf[i]))
      {
        // If the current entry is the same as the last written one, skip
        DPRINTLN("Duplicate logging entry found...");
        continue;
      }
      logFile.print(messageBuf[i].gnssFixType);
      logFile.print(F(","));

      logFile.print(messageBuf[i].iridiumMessageCount);
      logFile.print(F(","));

      logFile.printf("%.3lu", messageBuf[i].epochTime);
      logFile.print(F(","));

      logFile.printf("%ld", messageBuf[i].latitude); // Considering Latitude is int32_t
      logFile.print(F(","));

      logFile.printf("%ld", messageBuf[i].longitude); // Considering Longitude is int32_t
      logFile.print(F(","));

      logFile.printf("%ld", messageBuf[i].altitude); // Considering Altitude is int32_t
      logFile.print(F(","));

      logFile.printf("%ld", messageBuf[i].speed); // Considering Speed is int32_t
      logFile.print(F(","));

      logFile.printf("%ld", messageBuf[i].heading); // Considering Heading is int32_t
      logFile.print(F(","));

      logFile.print(messageBuf[i].stepCount);
      // continue for all items in struct
      logFile.print(F("\n"));
      lastEntry = messageBuf[i];
    }

    LastLogTime = millis();

    // clear bufLength variables (must clear the opposite of whatever one is in use)
    if (usingBuf1)
    {
      DPRINTLN(F("Reset buffer 2"));
      buf2Length = 0;
    }
    else
    {
      DPRINTLN(F("Reset buffer 1"));
      buf1Length = 0;
    }
    logFile.flush();
    return LOGGER_STATUS::LGR_SUCCESS;
  }
  else
  {
    DPRINTLN(F("Could not open file on SD card"));
    // Reset();
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }
}

void LOGGER::beginLogging()
{
  if (log)
  {
    DPRINTLN("Already Logging");
  }
  else
  {
    log = 1;
    DPRINTLN("Resuming logging...");
  }
}

void LOGGER::pauseLogging()
{
  if (!log)
  {
    DPRINTLN("Already Paused Logging");
  }
  else
  {
    log = 0;
    DPRINTLN("Pausing logging...");
  }
}

bool LOGGER::isOpen()
{
  return static_cast<bool>(logFile);
}

bool LOGGER::isSameEntry(dataFormat_t &a, dataFormat_t &b)
{
  return (a.gnssFixType == b.gnssFixType) &&
         (a.iridiumMessageCount == b.iridiumMessageCount) &&
         (a.epochTime == b.epochTime) &&
         (a.latitude == b.latitude) &&
         (a.longitude == b.longitude) &&
         (a.altitude == b.altitude) &&
         (a.speed == b.speed) &&
         (a.heading == b.heading) &&
         (a.stepCount == b.stepCount);
}