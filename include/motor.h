/**
 * @file motor.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "pinDefinitions.h"

#define MIN_MOTOR_SPEED			(3)
#define MAX_MOTOR_SPEED			(85)
#define RAMP_UP_SPEED           (5)
#define RAMP_DOWN_SPEED         (5)

typedef enum
{
    LEFT_TRACK,
    RIGHT_TRACK,
    BRUSHES
}MotorPosition_t;

typedef enum
{
    FORWARD,
    REVERSE,
    STOP
}MotorDirectionType_t;

class Motor
{
public:
    Motor(MotorPosition_t position);
    void Init(void);
    void Stop(void);
    uint8_t IsError(void);
    void SetTargetDirection(MotorDirectionType_t direction);
    void SetTargetSpeed(uint8_t speed);
    void RunRampSupport(void);

private:
    MotorPosition_t motorPosition;
    uint8_t forwardPin;
    uint8_t reversePin;
    uint8_t speedControlPin;
    uint8_t stopPin;
    uint8_t errorPin;
    uint8_t minMotorSpeed;
    uint8_t maxMotorSpeed;
    uint8_t rampUpSpeed;
    uint8_t rampDownSpeed;
    MotorDirectionType_t defaultDirection;
    MotorDirectionType_t targetDirection;
	MotorDirectionType_t currentDirection;
	uint8_t targetSpeed;
	int16_t currentSpeed;
};

#endif /* _MOTOR_H_ */