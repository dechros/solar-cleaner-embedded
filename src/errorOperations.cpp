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

bool detectDriverError(void)
{
    bool retVal = false;
    if (digitalRead(BRUSHES_ERROR_PIN) || digitalRead(TRACK_LEFT_ERROR_PIN) || digitalRead(TRACK_RIGHT_ERROR_PIN))
    {
        retVal = true;
    }
    return retVal;
}