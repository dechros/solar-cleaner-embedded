/**
 * @file motorDriver.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief Motor driver library header file for 
 *        all three motors in the system.
 * @version 0.1
 * @date 2023-05-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include "stdint.h"
#include "threadSafeSerial.h"
#include "pinDefinitions.h"

typedef enum
{
    RIGHT_PALLET,
    LEFT_PALLET,
    BRUSHES     
}Motor_t;

enum
{
    REVERSE,
    FORWARD
};

enum
{
    STOP,
    START
};

/**
 * @brief This function sets motor pins to their initial values.
 * 
 * @note Sets all STOP pins to LOW so all motors at STOP condition.
 *       They all rotate FORWARD as default. And their speed at 0.
 */
void InitializeMotors();

/**
 * @brief 
 * 
 * @param motorType Enum value of one of the three motors.
 * @param startStop 0 = Stop, 1 = Start
 */
void MotorStartStop(Motor_t motorType, uint8_t startStop);

/**
 * @brief Sets HIGH on one way LOW on the other way rotation pins
 *        to change motor rotation.
 * 
 * @param uint8_t Enum value of one of the three motors.
 * @param direction 0 = Reverse, 1 = Forward
 */
void MotorRotationChange(Motor_t motorType, uint8_t direction);

/**
 * @brief Sets the PWM value of the analog pin to control motor speed.
 * 
 * @param motorType Enum value of one of the three motors.
 * @param speed 0-255 motor speed value.
 */
void MotorSpeedChange(Motor_t motorType, uint8_t speed);

#endif