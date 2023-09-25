#include "logger.h"
#include "debug.h"
#include "gnss.h"

#define MAX_BUFFERED_MESSAGES 50 // max number of buffered messages before logging to SD card
#define MAX_LOG_FILE_SIZE 50000000U // 50 Megabytes
#define FILE_NAME_SIZE 19 // 10 for timestamp, 8 for text, 1 for termination

File logFile; // file logging object
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
static void setFileName() {
  uint32_t fileNum = 0;
  uint32_t currentTime;
  GNSS* gnss;
//   GNSS_STATUS timeStatus = gnss->GnssGetEpochTime();
//   if (timeStatus == GNSS_STATUS::GNSS_SUCCESS) {
//     fileNum = gnss->accessepochTime();
//   } else {
//     DPRINTLN("File not named from time...");
//   }
//   delete gnss;

  snprintf(fileName, FILE_NAME_SIZE, "log-%lu.txt", fileNum);
  while (SD.exists(fileName)) {
    fileNum++;
    snprintf(fileName, FILE_NAME_SIZE, "log-%lu.txt", fileNum); 
  }
}


LOGGER::LOGGER(uint32_t logfrequency = 1000)
{
    LogFrequency=logfrequency;
    if (!SD.begin(BUILTIN_SDCARD))
    {
        DPRINTLN(F("Error initializing SD card logging..."));
        initialized = false;
        return;
    }
    
    //setFileName();
    snprintf(fileName, FILE_NAME_SIZE, "log-%lu.txt", 12345);
    DPRINT(F("Setup complete. File name is "));
    DPRINTLN(fileName);

    
    logFile = SD.open(fileName, FILE_WRITE);
    initialized = true;

    DPRINTLN(F("Initialized SD card logging!"));
    return;
}

LOGGER::~LOGGER() {}

bool LOGGER::LoggerActive() {
  return initialized;
}

LOGGER_STATUS LOGGER::BufferData(dataFormat_t *message) {
  if (!initialized) {
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }

  noInterrupts();
  // find appropriate buffer to use
  dataFormat_t *messageBuf = usingBuf1 ? messageBuf1 : messageBuf2; 
  int bufLength = usingBuf1 ? buf1Length : buf2Length;

  if (bufLength >= MAX_BUFFERED_MESSAGES) {
    DPRINTLN(F("Tried to write to already full buffer"));
    return LOGGER_STATUS::LGR_ERROR_BUFFER_FULL;
  }

  messageBuf[bufLength] = *message;

  if (usingBuf1) {
    buf1Length++;
  } else {
    buf2Length++;
  }
  interrupts();

  return LOGGER_STATUS::LGR_SUCCESS;
}

LOGGER_STATUS LOGGER::Write() {
  if (!initialized) {
    DPRINTLN("Cannot write to SD card, not initalized...");
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }

  // find appropriate buffer to use
  dataFormat_t *messageBuf = usingBuf1 ? messageBuf1 : messageBuf2; 
  int bufLength = usingBuf1 ? buf1Length : buf2Length;

  // only write on certain conditions
  if ((millis() - LastLogTime) <= LogFrequency && bufLength < MAX_BUFFERED_MESSAGES) {
    return LOGGER_STATUS::LGR_ERROR_NO_WRITE;
  }

  usingBuf1 = !usingBuf1; // Switch main log buffer during write process

  // create a new file if current one is too large
  if (logFile.size() > MAX_LOG_FILE_SIZE) {
    DPRINTLN("Max File size reached, creating a new log file...");
    logFile.close();
    setFileName();
    logFile = SD.open(fileName, FILE_WRITE);
  }

  DPRINT(F(" -- "));
  DPRINT(F("Added from buf "));
  if (usingBuf1) {
    DPRINT(F("2: "));
  } else {
    DPRINT(F("1: "));
  }
  DPRINT(bufLength);
  DPRINT(F(" -- \n"));

  if (logFile) {
    DPRINTLN(F("Writing to SD card..."));

    // write all buffered messages to the SD card
    for (int i = 0; i < 3; i++) {
      logFile.printf("%.3lu", messageBuf[i].epochTime);
      logFile.print(F(" "));
      logFile.print(messageBuf[i].stepCount);
      logFile.print(F("]\n"));
    }

    LastLogTime = millis();
    
    // clear bufLength variables (must clear the opposite of whatever one is in use)
    if (usingBuf1) {
      DPRINTLN(F("Reset buffer 2"));
      buf2Length = 0;
    } else {
      DPRINTLN(F("Reset buffer 1"));
      buf1Length = 0;
    }
    logFile.flush();
    return LOGGER_STATUS::LGR_SUCCESS;
  } 
  else {
    DPRINTLN(F("Could not open file on SD card"));
    //Reset();
    return LOGGER_STATUS::LGR_ERROR_SD_CARD;
  }
}