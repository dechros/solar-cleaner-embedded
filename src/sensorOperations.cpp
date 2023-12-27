/**
 * @file sensorOperations.h
 * @author Halit Çetin (halitcetin@live.com)
 * @brief Proximity sensor operations file.
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sensorOperations.h"

bool sensorReadActive = false;

bool DetectSensorActivity()
{
    bool retVal = false;
    if (digitalRead(SENSOR_FRONT) || digitalRead(SENSOR_REAR) || digitalRead(SENSOR_LEFT) || digitalRead(SENSOR_RIGHT))
    {
        retVal = true;
    }
    return retVal;
}