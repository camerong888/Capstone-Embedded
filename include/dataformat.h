/**
 * @file dataformat.h
 * @brief Defines standard data format to log.
 *
 */

#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <stdint.h>

typedef struct
{
  uint8_t gnssFixType;         // GNSS fix type (data type: byte)
  uint8_t iridiumMessageCount; // Iridium Message Count (data type: uint8_t)
  uint32_t epochTime;          // epoch Time (data type: uint32_t)
  int32_t latitude;            // Latitude (data type: int32_t)
  int32_t longitude;           // Longitude (data type: int32_t)
  int32_t altitude;            // Altitude (data type: int32_t)
  int32_t speed;               // Speed (data type: int32_t)
  int32_t heading;             // Heading (data type: int32_t)
  uint32_t stepCount;          // stepCount (data type: uint32_t)
} dataFormat_t;

#endif