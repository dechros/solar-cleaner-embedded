/**
 * @file serialParserTask.h
 * @author Halit Cetin (halitcetin@live.com)
 * @brief This file includes serial debugging operations.
 * @version 0.1
 * @date 04-05-2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _SERIAL_PARSER_TASK_H_
#define _SERIAL_PARSER_TASK_H_

#include "threadSafeSerial.h"
#include "motorDriver.h"

#define MESSAGE_SIZE             6
#define RESPONSE_SIZE            6

#define MESSAGE_DEBUG_HEADER    'Q'
#define MESSAGE_ESP32_HEADER    'P'

#define MESSAGE_RIGHT_PALLET_CHAR_CODE      'R'
#define MESSAGE_LEFT_PALLET_CHAR_CODE       'L'
#define MESSAGE_BRUSHES_PALLET_CHAR_CODE    'B'
#define MESSAGE_EEPROM_CHAR_CODE            'M'
#define MESSAGE_ERROR_CHAR_CODE             'E'
#define MESSAGE_GENERAL_CHAR_CODE           'G'

typedef enum
{
    MOTOR_START_STOP,
    MOTOR_SPEED_CHANGE,
    MOTOR_ROTATION_CHANGE,
}MotorsMessageID_t;

typedef union 
{
    uint8_t dataArray[MESSAGE_SIZE];
    struct
    {
        uint8_t header;
        uint8_t charCode;
        uint8_t messageId;
        uint8_t data[2];
        uint8_t checksum;
    };
}SerialMessage_t;

typedef union 
{
    uint8_t dataArray[RESPONSE_SIZE];
    struct
    {
        uint8_t charCode;
        uint8_t messageId;
        uint8_t data[3];
        uint8_t checksum;
    };
}SerialResponse_t;

typedef enum
{
    ACKNOWLEDGE,
    NEGATIVE_ACKNOWLEDGE,
    ERROR_ACKNOWLEDGE,
    SOFTWARE_VERSION,
    SOFTWARE_UPDATE_DATE,
    ERROR_COUNT,
    DETAILED_ERROR_DATA,
    DOWNLOAD_ERRORS
}ResponseType_t;

/**
 * @brief 
 * 
 * @param pvParameters 
 */
void vSerialParserTask(void *pvParameters);

#endif /* _SERIAL_PARSER_TASK_H_ */