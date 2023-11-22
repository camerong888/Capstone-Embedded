/**
 * @file logger.h
 * @author Cameron Gordon
 * @brief
 * @date 2023-09-24
 */
#ifndef LOGGER__H
#define LOGGER__H
#include <Arduino.h>
#include <dataformat.h>
#include <SD.h>
#include <SPI.h>

/*********************************************/
typedef enum
{
  LGR_SUCCESS = 0,
  LGR_ERROR_BUFFER_FULL = 1,
  LGR_ERROR_SD_CARD = 2,
  LGR_ERROR_NO_WRITE = 3
} LOGGER_STATUS;

/*********************************************/
class LOGGER
{
private:
  uint32_t LogFrequency = 1000;
  uint32_t LastLogTime = 0;
  uint32_t lastEpochTime = 0;
  //bool initialized = false;
  bool log;

public:
  LOGGER(uint32_t logfrequency);

  ~LOGGER();

  LOGGER_STATUS LOGGER_init();

  void LOGGER_newFile(String date);

  /**
   * @brief Writes the messages currently buffered to the SD card
   * @return LOGGER_STATUS
   */
  LOGGER_STATUS Write(String date);

  /**
   * @brief Adds the given message to the log buffer.
   *
   * @param data pointer to the data
   * @return LOGGER_STATUS
   */
  LOGGER_STATUS BufferData(dataFormat_t *data);

  /**
   * @brief Finds whether the logger is currently logging messages
   * @return true when active, false otherwise
   */
  bool Log_Status();

  void beginLogging();

  void pauseLogging();

  bool isOpen();

  void close();

};

#endif