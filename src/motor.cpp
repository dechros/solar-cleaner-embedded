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
#include "globals.h"

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

            break;
        }
        case RIGHT_TRACK:
        {
            forwardPin = TRACK_RIGHT_FORWARD_TURN_PIN;
            reversePin = TRACK_RIGHT_REVERSE_TURN_PIN;
            speedControlPin = TRACK_RIGHT_SPEED_CONTROL_PIN;
            stopPin = TRACK_RIGHT_STOP_PIN;
            errorPin = TRACK_RIGHT_ERROR_PIN;

            break;
        }
        case BRUSHES:
        {
            forwardPin = BRUSHES_FORWARD_TURN_PIN;
            reversePin = BRUSHES_REVERSE_TURN_PIN;
            speedControlPin = BRUSHES_SPEED_CONTROL_PIN;
            stopPin = BRUSHES_STOP_PIN;
            errorPin = BRUSHES_ERROR_PIN;

            break;
        }
        default:
        {
            break;
        }
    }

    digitalWrite(forwardPin, LOW);
    digitalWrite(stopPin, HIGH);
    digitalWrite(reversePin, LOW);
    analogWrite(speedControlPin, 0);

    currentSpeed = 0;
    targetSpeed = 0;
    currentDirection = STOP;
    targetDirection = STOP;
    defaultDirection = STOP;

    if (motorPosition == LEFT_TRACK)
    {
        minMotorSpeed = SystemParameters.leftMinSpeed;
        maxMotorSpeed = SystemParameters.leftMaxSpeed;
        rampUpSpeed   = SystemParameters.leftRampUp;
        rampDownSpeed = SystemParameters.leftRampDown;
    }
    else if (motorPosition == RIGHT_TRACK)
    {
        minMotorSpeed = SystemParameters.rightMinSpeed;
        maxMotorSpeed = SystemParameters.rightMaxSpeed;
        rampUpSpeed   = SystemParameters.rightRampUp;
        rampDownSpeed = SystemParameters.rightRampDown;
    }
    else
    {
        minMotorSpeed = SystemParameters.brushMinSpeed;
        maxMotorSpeed = SystemParameters.brushMaxSpeed;
        rampUpSpeed   = SystemParameters.brushRampUp;
        rampDownSpeed = SystemParameters.brushRampDown;
    }
}

void Motor::Stop()
{
    targetSpeed = 0;
    targetDirection = defaultDirection;
    digitalWrite(stopPin, HIGH);
    stopped = true;
}

void Motor::ResumeIfStopped(void)
{
    if (stopped == true)
    {
        digitalWrite(stopPin, HIGH);
        stopped = false;
    }
}

uint8_t Motor::CheckError()
{
    return (digitalRead(errorPin));
}

void Motor::SetTargetDirection(MotorDirectionType_t direction)
{
    /**
     * @note: Target direction actually refers to
     *        motors rotation direction. Clockwise or Counter-clockwise
     *        FORWARD => Clockwise rotation
     *        REVERSE => Counter-clockwise rotation
     *
     *        So when we want to move machine forward. We need to make
     *        Left Motor Direction = FORWARD
     *        Right Motor Direction = REVERSE
     *
     *        It actually makes sense because when right motor turns clockwise
     *        it moves machine to reverse. But when left motor turns clockwise
     *        it moves machine to forward.
     *
     * @summary: Right track direction means the opposite direction for machine
     */
    targetDirection = direction;
}

void Motor::SetTargetSpeed(uint8_t speed)
{
    targetSpeed = speed;
}

void Motor::RunRampSupport()
{
    if (targetSpeed > maxMotorSpeed)
    {
        targetSpeed = maxMotorSpeed;
    }
    
    if (targetDirection == currentDirection)
    {
        if (targetSpeed > currentSpeed)
        {
            currentSpeed = currentSpeed + rampUpSpeed;
            if (currentSpeed > targetSpeed)
            {
                currentSpeed = targetSpeed;
            }
            analogWrite(speedControlPin, (uint8_t)currentSpeed);
        }
        else if (targetSpeed < currentSpeed)
        {
            currentSpeed = currentSpeed - rampDownSpeed;
            if (currentSpeed < targetSpeed)
            {
                currentSpeed = targetSpeed;
            }
            analogWrite(speedControlPin, (uint8_t)currentSpeed);
        }
    }
    else
    {
        currentSpeed = currentSpeed - rampDownSpeed;
        if (currentSpeed < 0)
        {
            currentSpeed = 0;
        }
        analogWrite(speedControlPin, (uint8_t)currentSpeed);
        if (currentSpeed == 0)
        {
            if (targetDirection == FORWARD)
            {
                digitalWrite(forwardPin, HIGH);
                digitalWrite(reversePin, LOW);
                digitalWrite(stopPin, LOW);
            }
            else if (targetDirection == REVERSE)
            {
                digitalWrite(forwardPin, LOW);
                digitalWrite(reversePin, HIGH);
                digitalWrite(stopPin, LOW);
            }
            else
            {
                digitalWrite(forwardPin, LOW);
                digitalWrite(reversePin, LOW);
                digitalWrite(stopPin, HIGH);
            }
            currentDirection = targetDirection;
        }
    }
}