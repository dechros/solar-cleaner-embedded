/**
 * @file sensorOperations.h
 * @author Halit Çetin (halitcetin@live.com)
 * @brief Proximity sensor operations header file.
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SENSOR_OPERATIONS_H
#define SENSOR_OPERATIONS_H

#include "Arduino.h"
#include "pinDefinitions.h"

/**
 * @brief Reads the distance sensor outputs on the machine.
 * 
 * @details There are 4 sensors on the machine. Front, back, left, right.
 * These sensors use ultrasonic signals to detect obstacles. We are using
 * them to detect spaces.
 * 
 * @return true Gap detected
 * @return false Gap is not detected
 */
bool DetectSensorActivity(void);

#endif