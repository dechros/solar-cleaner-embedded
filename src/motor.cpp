/**
 * @file motor.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "motor.h"

Motor::Motor(MotorPosition_t position)
{
    this->motorPosition = position;
}

void Motor::Init()
{
    switch (motorPosition)
    {
        case LEFT_TRACK:
        {
            forwardPin = TRACK_LEFT_FORWARD_TURN_PIN;
            reversePin = TRACK_LEFT_REVERSE_TURN_PIN;
            speedControlPin = TRACK_LEFT_SPEED_CONTROL_PIN;
            stopPin = TRACK_LEFT_STOP_PIN;
            errorPin = TRACK_LEFT_ERROR_PIN;

            digitalWrite(forwardPin, HIGH);
            digitalWrite(stopPin, LOW);
            digitalWrite(reversePin, LOW);
            analogWrite(speedControlPin, 0);

            currentSpeed = 0;
            targetSpeed = 0;
            currentDirection = FORWARD;
            targetDirection = FORWARD;
            defaultDirection = FORWARD;

            break;
        }
        case RIGHT_TRACK:
        {
            forwardPin = TRACK_RIGHT_FORWARD_TURN_PIN;
            reversePin = TRACK_RIGHT_REVERSE_TURN_PIN;
            speedControlPin = TRACK_RIGHT_SPEED_CONTROL_PIN;
            stopPin = TRACK_RIGHT_STOP_PIN;
            errorPin = TRACK_RIGHT_ERROR_PIN;

            digitalWrite(forwardPin, LOW);
            digitalWrite(stopPin, LOW);
            digitalWrite(reversePin, HIGH);
            analogWrite(speedControlPin, 0);

            currentSpeed = 0;
            targetSpeed = 0;
            currentDirection = REVERSE;
            targetDirection = REVERSE;
            defaultDirection = REVERSE;

            break;
        }
        case BRUSHES:
        {
            forwardPin = BRUSHES_FORWARD_TURN_PIN;
            reversePin = BRUSHES_REVERSE_TURN_PIN;
            speedControlPin = BRUSHES_SPEED_CONTROL_PIN;
            stopPin = BRUSHES_STOP_PIN;
            errorPin = BRUSHES_ERROR_PIN;

            digitalWrite(forwardPin, HIGH);
            digitalWrite(stopPin, LOW);
            digitalWrite(reversePin, LOW);
            analogWrite(speedControlPin, 0);

            currentSpeed = 0;
            targetSpeed = 0;
            currentDirection = FORWARD;
            targetDirection = FORWARD;
            defaultDirection = FORWARD;

            break;
        }
        default:
        {
            break;
        }
    }

    minMotorSpeed = MIN_MOTOR_SPEED;
    maxMotorSpeed = MAX_MOTOR_SPEED;
    rampUpSpeed = RAMP_UP_SPEED;
    rampDownSpeed = RAMP_DOWN_SPEED;
}

void Motor::Stop()
{
    targetSpeed = 0;
    targetDirection = defaultDirection;
}

uint8_t Motor::IsError()
{
    return (digitalRead(errorPin));
}

void Motor::SetTargetDirection(MotorDirectionType_t direction)
{
    targetDirection = direction;
}

void Motor::SetTargetSpeed(uint8_t speed)
{
    targetSpeed = speed;
}

void Motor::RunAlgorithm()
{
    if (targetDirection == currentDirection)
    {
        if (targetSpeed > currentSpeed)
        {
            currentSpeed = currentSpeed + RAMP_UP_SPEED;
            if (currentSpeed > targetSpeed)
            {
                currentSpeed = targetSpeed;
            }
            analogWrite(speedControlPin, (uint8_t)currentSpeed);
        }
        else if (targetSpeed < currentSpeed)
        {
            currentSpeed = currentSpeed - RAMP_DOWN_SPEED;
            if (currentSpeed < targetSpeed)
            {
                currentSpeed = targetSpeed;
            }
            analogWrite(speedControlPin, (uint8_t)currentSpeed);
        }
    }
    else
    {
        currentSpeed = currentSpeed - RAMP_DOWN_SPEED;
        if (currentSpeed < 0)
        {
            currentSpeed = 0;
        }
        analogWrite(speedControlPin, (uint8_t)currentSpeed);
        if (currentSpeed == 0)
        {
            if (targetDirection == FORWARD)
            {
                switch (motorPosition)
                {
                    case LEFT_TRACK:
                    {
                        digitalWrite(forwardPin, HIGH);
                        digitalWrite(reversePin, LOW);
                        break;
                    }
                    case RIGHT_TRACK:
                    {
                        digitalWrite(forwardPin, LOW);
                        digitalWrite(reversePin, HIGH);
                        break;
                    }
                    case BRUSHES:
                    {
                        digitalWrite(forwardPin, HIGH);
                        digitalWrite(reversePin, LOW);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else
            {
                switch (motorPosition)
                {
                    case LEFT_TRACK:
                    {
                        digitalWrite(forwardPin, LOW);
                        digitalWrite(reversePin, HIGH);
                        break;
                    }
                    case RIGHT_TRACK:
                    {
                        digitalWrite(forwardPin, HIGH);
                        digitalWrite(reversePin, LOW);
                        break;
                    }
                    case BRUSHES:
                    {
                        digitalWrite(forwardPin, LOW);
                        digitalWrite(reversePin, HIGH);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            currentDirection = targetDirection;
        }
    }
}