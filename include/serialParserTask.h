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

/**
 * @brief 
 * 
 * @param pvParameters 
 */
void vSerialParserTask(void *pvParameters);

#endif /* _SERIAL_PARSER_TASK_H_ */