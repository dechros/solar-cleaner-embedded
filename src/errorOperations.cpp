/**
 * @file errorOperations.h
 * @author Halit Cetin (halitcetin@live.com)
 * @brief Error handling operations file.
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "errorOperations.h"
#include "eepromOperations.h"

bool DetectDriverError(void)
{
    bool retVal = false;
    if (digitalRead(BRUSHES_ERROR_PIN))
    {
        LogError(BRUSH_ERROR);
        retVal = true;
    }
    if (digitalRead(TRACK_LEFT_ERROR_PIN))
    {
        LogError(LEFT_ERROR);
        retVal = true;
    }
    if (digitalRead(TRACK_RIGHT_ERROR_PIN))
    {
        LogError(RIGHT_ERROR);
        retVal = true;
    }
    if (false) /* TODO: Log error for controller */
    {
        LogError(CONTROLLER_ERROR);
        retVal = true;
    }
    return retVal;
}