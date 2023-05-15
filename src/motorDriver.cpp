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
                DigitalWriteThreadSafe(PALLET_RIGHT_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                DigitalWriteThreadSafe(PALLET_RIGHT_STOP_PIN, HIGH);
            }
            break;
        }
        case LEFT_PALLET:
        {
            if (startStop == STOP)
            {
                DigitalWriteThreadSafe(PALLET_LEFT_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                DigitalWriteThreadSafe(PALLET_LEFT_STOP_PIN, HIGH);
            }
            break;
        }
        case BRUSHES:
        {
            if (startStop == STOP)
            {
                DigitalWriteThreadSafe(BRUSHES_STOP_PIN, LOW);
            }
            else if (startStop == START)
            {
                DigitalWriteThreadSafe(BRUSHES_STOP_PIN, HIGH);
            }
            break;
        }
        default:
        {
            SerialPrintln("MotorStop: Unknown Motor!");
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
            DigitalWriteThreadSafe(PALLET_RIGHT_FORWARD_TURN_PIN, LOW);
            DigitalWriteThreadSafe(PALLET_RIGHT_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            DigitalWriteThreadSafe(PALLET_RIGHT_REVERSE_TURN_PIN, LOW);
            DigitalWriteThreadSafe(PALLET_RIGHT_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
        }
    }
    else if(motorType == LEFT_PALLET)
    {
        if (direction == FORWARD)
        {
            DigitalWriteThreadSafe(PALLET_LEFT_FORWARD_TURN_PIN, LOW);
            DigitalWriteThreadSafe(PALLET_LEFT_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            DigitalWriteThreadSafe(PALLET_LEFT_REVERSE_TURN_PIN, LOW);
            DigitalWriteThreadSafe(PALLET_LEFT_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
        }
    }
    else if(motorType == BRUSHES)
    {
        if (direction == FORWARD)
        {
            DigitalWriteThreadSafe(BRUSHES_FORWARD_TURN_PIN, LOW);
            DigitalWriteThreadSafe(BRUSHES_REVERSE_TURN_PIN, HIGH);
        }
        else if(direction == REVERSE)
        {
            DigitalWriteThreadSafe(BRUSHES_REVERSE_TURN_PIN, LOW);
            DigitalWriteThreadSafe(BRUSHES_FORWARD_TURN_PIN, HIGH);
        }
        else
        {
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
        }
    }
    else
    {
        SerialPrintln("MotorRotationChange: Unknown Motor!");
    }
}

void MotorSpeedChange(Motor_t motorType, uint8_t speed)
{
    if (motorType == RIGHT_PALLET)
    {
        AnalogWriteThreadSafe(PALLET_RIGHT_SPEED_CONTROL_PIN, speed);
    }
    else if(motorType == LEFT_PALLET)
    {
        AnalogWriteThreadSafe(PALLET_LEFT_SPEED_CONTROL_PIN, speed);
    }
    else if(motorType == BRUSHES)
    {
        AnalogWriteThreadSafe(BRUSHES_SPEED_CONTROL_PIN, speed);
    }
    else
    {
        SerialPrintln("MotorSpeedChange: Unknown Motor!");
    }
}