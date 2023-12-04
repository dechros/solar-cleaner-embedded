/**
 * @file waterPumpDriver.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief Header file of water pump driver.
 * @version 0.1
 * @date 2023-05-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _WATER_PUMP_DRIVER_H_
#define _WATER_PUMP_DRIVER_H_

#include <Arduino.h>
#include "pinOperations.h"

/**
 * @brief Opens the water pump by writing HIGH to related pin.
 * 
 */
void WaterPumpOn(void);

/**
 * @brief Closes the water pump by writing LOW to related pin.
 * 
 */
void WaterPumpOff(void);

/**
 * @brief Toggles the water pump by writing LOW or HIGH to related pin.
 * 
 */
void WaterPumpToggle(void);

#endif