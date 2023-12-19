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

#include "globals.h"
#include "errorOperations.h"
#include "eepromOperations.h"

bool DetectErrors(void)
{
    static uint8_t brushErrorOldState = LOW;
    static uint8_t leftErrorOldState = LOW;
    static uint8_t rightErrorOldState = LOW;
    static bool controllerErrorOldState = false;

    uint8_t brushErrorCurrentState = digitalRead(BRUSHES_ERROR_PIN);
    uint8_t leftErrorCurrentState = digitalRead(TRACK_LEFT_ERROR_PIN);
    uint8_t rightErrorCurrentState = digitalRead(TRACK_RIGHT_ERROR_PIN);
    bool controllerErrorCurrentState = controllerError;

    bool retVal = false;
    if (brushErrorCurrentState == HIGH && brushErrorOldState == LOW)
    {
        LogError(BRUSH_ERROR);
        retVal = true;
    }
    if (leftErrorCurrentState == HIGH && leftErrorOldState == LOW)
    {
        LogError(LEFT_ERROR);
        retVal = true;
    }
    if (rightErrorCurrentState == HIGH && rightErrorOldState == LOW)
    {
        LogError(RIGHT_ERROR);
        retVal = true;
    }
    if (controllerErrorCurrentState == true && controllerErrorOldState == false)
    {
        Serial.println("Controller Error!");
        LogError(CONTROLLER_ERROR);
        retVal = true;
    }

    brushErrorOldState = brushErrorCurrentState;
    leftErrorOldState = leftErrorCurrentState;
    rightErrorOldState = rightErrorCurrentState;
    controllerErrorOldState = controllerErrorCurrentState;

    return retVal;
}