/**
 * @file readInputPinsTask.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _READ_INPUT_PINS_TASK_H_
#define _READ_INPUT_PINS_TASK_H_

#include "threadSafeSerial.h"
#include "pinOperations.h"

/**
 * @brief
 *
 * @param pvParameters
 */
void vReadInputPinsTask(void *pvParameters);

#endif /* _READ_INPUT_PINS_TASK_H_ */