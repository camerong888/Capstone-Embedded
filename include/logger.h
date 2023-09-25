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
typedef enum {
  LGR_SUCCESS           = 0,
  LGR_ERROR_BUFFER_FULL = 1,
  LGR_ERROR_SD_CARD     = 2,
  LGR_ERROR_NO_WRITE    = 3
} LOGGER_STATUS;

/*********************************************/
class LOGGER
{
    private:
        uint32_t LogFrequency = 1000;
        uint32_t LastLogTime = 0;
        bool initialized = false;
    public:
        /**
         * @brief Initializes the SD logging functionality
         * @note sets logFrequency minimum rate at which to log the buffered messages
         * @return LOGGER_STATUS
         */
        LOGGER(uint32_t logfrequency);

        ~LOGGER();

        /**
         * @brief Writes the messages currently buffered to the SD card
         * @return LOGGER_STATUS
         */
        LOGGER_STATUS Write();

        /**
         * @brief Resets the buffers, SD card
         * @return LOGGER_STATUS
         */
        LOGGER_STATUS Reset();

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
        bool LoggerActive();

};

#endif