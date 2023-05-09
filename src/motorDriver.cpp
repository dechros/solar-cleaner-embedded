#include "motorDriver.h"
    
void InitializeMotors()
{
    digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, LOW);
    digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, HIGH);
    digitalWrite(PALLET_RIGHT_STOP_PIN, HIGH);

    digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, LOW);
    digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, HIGH);
    digitalWrite(PALLET_LEFT_STOP_PIN, HIGH);

    digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
    digitalWrite(BRUSHES_REVERSE_TURN_PIN, HIGH);
    digitalWrite(BRUSHES_STOP_PIN, HIGH);
}

void MotorStart(Motor_t motorType)
{
    if (motorType == RIGHT_PALLET)
    {
        digitalWrite(PALLET_RIGHT_STOP_PIN, HIGH);
    }
    else if(motorType == LEFT_PALLET)
    {
        digitalWrite(PALLET_LEFT_STOP_PIN, HIGH);
    }
    else if(motorType == BRUSHES)
    {
        digitalWrite(BRUSHES_STOP_PIN, HIGH);
    }
    else
    {
        SerialPrintln("MotorStart: Unknown Motor!");
    }
}

void MotorStop(Motor_t motorType)
{
    if (motorType == RIGHT_PALLET)
    {
        digitalWrite(PALLET_RIGHT_STOP_PIN, LOW);
    }
    else if(motorType == LEFT_PALLET)
    {
        digitalWrite(PALLET_LEFT_STOP_PIN, LOW);
    }
    else if(motorType == BRUSHES)
    {
        digitalWrite(BRUSHES_STOP_PIN, LOW);
    }
    else
    {
        SerialPrintln("MotorStop: Unknown Motor!");
    }
}

void MotorRotationChange(Motor_t motorType, Direction_t direction)
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
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
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
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
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
            SerialPrintln("MotorRotationChange: Unknown Rotation!");
        }
    }
    else
    {
        SerialPrintln("MotorRotationChange: Unknown Motor!");
    }
}