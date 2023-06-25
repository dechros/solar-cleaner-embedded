/**
 * @file waterPumpDriver.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief Source file of water pump driver.
 * @version 0.1
 * @date 2023-05-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "waterPumpDriver.h"

void WaterPumpOn()
{
    digitalWrite(WATER_ON_OFF_PIN, HIGH);
}

void WaterPumpOff()
{
    digitalWrite(WATER_ON_OFF_PIN, LOW);
}