/**
 * @file motorDriver.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief Motor driver library source file for 
 *        all three motors in the system.
 * @version 0.1
 * @date 2023-05-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "motorDriver.h"
    
void InitializeMotors()
{
    digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, LOW);
    digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, HIGH);
    digitalWrite(PALLET_RIGHT_STOP_PIN, LOW);

    digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, LOW);
    digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, HIGH);
    digitalWrite(PALLET_LEFT_STOP_PIN, LOW);

    digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
    digitalWrite(BRUSHES_REVERSE_TURN_PIN, LOW);
    digitalWrite(BRUSHES_STOP_PIN, LOW);

    analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, 0);
    analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, 0);
    analogWrite(BRUSHES_SPEED_CONTROL_PIN, 0);
}

void MotorStartStop(Motor_t motorType, uint8_t startStop)
{
    switch (motorType)
    {
        case RIGHT_PALLET:
        {
            if (startStop == STOP)
            {
                digitalWrite(PALLET_RIGHT_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                digitalWrite(PALLET_RIGHT_STOP_PIN, HIGH);
            }
            break;
        }
        case LEFT_PALLET:
        {
            if (startStop == STOP)
            {
                digitalWrite(PALLET_LEFT_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                digitalWrite(PALLET_LEFT_STOP_PIN, HIGH);
            }
            break;
        }
        case BRUSHES:
        {
            if (startStop == STOP)
            {
                digitalWrite(BRUSHES_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                digitalWrite(BRUSHES_STOP_PIN, HIGH);
            }
            break;
        }
        default:
        {
            Serial.println("MotorStop: Unknown Motor!");
            break;
        }
    }
}

void MotorRotationChange(Motor_t motorType, uint8_t direction)
{
    if (motorType == RIGHT_PALLET)
    {
        if (direction == FORWARD)
        {
            digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, LOW);
            digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, LOW);
            digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            Serial.println("MotorRotationChange: Unknown Rotation!");
        }
    }
    else if(motorType == LEFT_PALLET)
    {
        if (direction == FORWARD)
        {
            digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, LOW);
            digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, LOW);
            digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            Serial.println("MotorRotationChange: Unknown Rotation!");
        }
    }
    else if(motorType == BRUSHES)
    {
        if (direction == FORWARD)
        {
            digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
            digitalWrite(BRUSHES_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            digitalWrite(BRUSHES_REVERSE_TURN_PIN, LOW);
            digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            Serial.println("MotorRotationChange: Unknown Rotation!");
        }
    }
    else
    {
        Serial.println("MotorRotationChange: Unknown Motor!");
    }
}

void MotorSpeedChange(Motor_t motorType, uint8_t speed)
{
    if (motorType == RIGHT_PALLET)
    {
        analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, speed);
    }
    else if(motorType == LEFT_PALLET)
    {
        analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, speed);
    }
    else if(motorType == BRUSHES)
    {
        analogWrite(BRUSHES_SPEED_CONTROL_PIN, speed);
    }
    else
    {
        Serial.println("MotorSpeedChange: Unknown Motor!");
    }
}