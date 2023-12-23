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
    uint8_t CheckError(void);
    void SetTargetDirection(MotorDirectionType_t direction);
    void SetTargetSpeed(uint16_t speed);
    void RunRampSupport(void);
    void UpdateMotorParameters(void);

private:
    MotorPosition_t motorPosition;
    uint8_t forwardPin;
    uint8_t reversePin;
    uint8_t speedControlPin;
    uint8_t stopPin;
    uint8_t errorPin;
    uint8_t minMotorSpeed;
    uint8_t maxMotorSpeed;
    uint16_t rampUpSpeed;
    uint16_t rampDownSpeed;
    MotorDirectionType_t defaultDirection;
    MotorDirectionType_t targetDirection;
	MotorDirectionType_t currentDirection;
	uint16_t targetSpeed;
	int32_t currentSpeed;
    bool stopped = false;
};

#endif /* _MOTOR_H_ */