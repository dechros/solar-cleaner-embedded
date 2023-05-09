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

typedef enum
{
    FORWARD,
    REVERSE
}Direction_t;

/**
 * @brief 
 * 
 */
void InitializeMotors();

/**
 * @brief 
 * 
 * @param motorType 
 */
void MotorStart(Motor_t motorType);

/**
 * @brief 
 * 
 * @param motorType 
 */
void MotorStop(Motor_t motorType);

/**
 * @brief 
 * 
 * @param motorType 
 * @param direction All motor pins are active high and 
 *                  must be connected to GND
 */
void MotorRotationChange(Motor_t motorType, Direction_t direction);



#endif