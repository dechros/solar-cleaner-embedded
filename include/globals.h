/**
 * @file globals.h
 * @author Halit Cetin (halitcetin@live.com)
 * @brief This file includes global variables.
 * @version 0.1
 * @date 04-05-2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "motor.h"
#include "eepromOperations.h"
#include "waterPump.h"

#define CAN_MODE 0

extern bool controllerError;
extern uint8_t messageTimeoutCounter;
extern bool firstMessageCame;

extern Motor LeftTrackMotor;
extern Motor RightTrackMotor;
extern Motor BrushesMotor;

extern WaterPump WaterPumpHandler;

extern Parameters_t SystemParameters;

void SystemStop();

#endif /* _GLOBALS_H_ */