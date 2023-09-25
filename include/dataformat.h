/**
 * @file dataformat.h
 * @brief Defines standard data format to log.
 * 
 */

#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <stdint.h>

enum DataIndices {
  LATITUDE,
  LONGITUDE,
  SPEED,
  HEADING,
  ALTITUDE,
  IRIDIUM_INDICATOR,

  // more fields
};


/* Message structure used throughout the system. */
typedef struct {
  uint32_t epochTime;
  uint32_t stepCount;
  uint32_t dataBuf[8]; //DataIndices
} dataFormat_t;



#endif