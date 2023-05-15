/**
 * @file pinOperations.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief This source file contains pin related functions, 
 *        non-mapped and standart pin interrupt handlers.
 * @version 0.1
 * @date 2023-05-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "pinOperations.h"
#include "EnableInterrupt.h"

SemaphoreHandle_t xPinMutex;

void InitializePins()
{
    xPinMutex = xSemaphoreCreateMutex();
    if(xPinMutex == NULL)
    {
        SerialPrintln("xPinMutex Create Error!");
        while (true);
    }
    pinMode(BRUSHES_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(BRUSHES_FORWARD_TURN_PIN , OUTPUT);
    pinMode(BRUSHES_STOP_PIN         , OUTPUT);
    pinMode(BRUSHES_REVERSE_TURN_PIN , OUTPUT);
    pinMode(BRUSHES_ERROR_PIN        , INPUT);

    pinMode(WATER_ON_OFF_PIN, OUTPUT);

    pinMode(PALLET_LEFT_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(PALLET_LEFT_FORWARD_TURN_PIN , OUTPUT);
    pinMode(PALLET_LEFT_STOP_PIN         , OUTPUT);
    pinMode(PALLET_LEFT_REVERSE_TURN_PIN , OUTPUT);
    pinMode(PALLET_LEFT_ERROR_PIN        , INPUT);

    pinMode(PALLET_RIGHT_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(PALLET_RIGHT_FORWARD_TURN_PIN , OUTPUT);
    pinMode(PALLET_RIGHT_STOP_PIN         , OUTPUT);
    pinMode(PALLET_RIGHT_REVERSE_TURN_PIN , OUTPUT);
    pinMode(PALLET_RIGHT_ERROR_PIN        , INPUT);

    pinMode(ENCODER_1_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_1_B_PIN, INPUT_PULLUP);
    pinMode(ENCODER_1_0_PIN, INPUT_PULLUP);
    pinMode(ENCODER_2_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_2_B_PIN, INPUT_PULLUP);
    pinMode(ENCODER_2_0_PIN, INPUT_PULLUP);

    pinModeExtended(PORT_J, PJ2, INPUT);

    enableInterrupt(PJ2_PCINT11_PIN, MCP2515InterruptCallback, RISING);
    enableInterrupt(ENCODER_1_A_PIN, Encoder1InterruptACallback, RISING);
    enableInterrupt(ENCODER_1_B_PIN, Encoder1InterruptBCallback, RISING);
    enableInterrupt(ENCODER_1_0_PIN, Encoder1Interrupt0Callback, RISING);
    enableInterrupt(ENCODER_2_A_PIN, Encoder2InterruptACallback, RISING);
    enableInterrupt(ENCODER_2_B_PIN, Encoder2InterruptBCallback, RISING);
    enableInterrupt(ENCODER_2_0_PIN, Encoder2Interrupt0Callback, RISING);
}

void MCP2515InterruptCallback()
{
    /* TODO: Implement Logic */
}

void Encoder1InterruptACallback()
{
    /* TODO: Implement Logic */
    SerialPrintln("1ACallBack!");
}

void Encoder1InterruptBCallback()
{
    /* TODO: Implement Logic */
    SerialPrintln("1BCallBack!");
}

void Encoder1Interrupt0Callback()
{
    /* TODO: Implement Logic */
    SerialPrintln("10CallBack!");
}

void Encoder2InterruptACallback()
{
    /* TODO: Implement Logic */
    SerialPrintln("2ACallBack!");
}

void Encoder2InterruptBCallback()
{
    /* TODO: Implement Logic */
    SerialPrintln("2BCallBack!");
}

void Encoder2Interrupt0Callback()
{
    /* TODO: Implement Logic */
    SerialPrintln("20CallBack!");
}

int DigitalReadThreadSafe(uint8_t pin)
{
    int pinValue = 0;
    xSemaphoreTake(xPinMutex, portMAX_DELAY);
    if (pin >= 70)
    {
        pinValue = digitalReadExtended(pin);
    }
    else
    {
        pinValue = digitalRead(pin);
    }
    xSemaphoreGive(xPinMutex);
    return pinValue;
}

void DigitalWriteThreadSafe(uint8_t pin, uint8_t val)
{
    xSemaphoreTake(xPinMutex, portMAX_DELAY);
    if (pin >= 70)
    {
        digitalWriteExtended(pin, val);
    }
    else
    {
        digitalWrite(pin, val);
    }
    xSemaphoreGive(xPinMutex);
}

void AnalogWriteThreadSafe(uint8_t pin, uint8_t val)
{
    xSemaphoreTake(xPinMutex, portMAX_DELAY);
    analogWrite(pin, val);
    xSemaphoreGive(xPinMutex);
}

int AnalogReadThreadSafe(uint8_t pin)
{
    xSemaphoreTake(xPinMutex, portMAX_DELAY);
    int value = analogRead(pin);
    xSemaphoreGive(xPinMutex);
    return value;
}

int digitalReadExtended(uint8_t pin)
{
    if (pin >= 70)
    {
        uint8_t bit = digitalPinToBitMask(PORT_J_FIRST_PIN);
        uint8_t port = digitalPinToPort(PORT_J_FIRST_PIN);

        uint8_t multiplier = pow(2, (pin - 69));
        if (*portInputRegister(port) & (bit * multiplier))   
        {
            return HIGH;
        }
        else
        {
            return LOW;
        }
    }
    else
    {
        return LOW;
    }
}

void digitalWriteExtended(uint8_t pin, uint8_t val)
{
    if (pin >= 70)
    {
        uint8_t bit = digitalPinToBitMask(PORT_J_FIRST_PIN);
        uint8_t port = digitalPinToPort(PORT_J_FIRST_PIN);
        volatile uint8_t *out;

        out = portOutputRegister(port);

        uint8_t oldSREG = SREG;
        cli();

        if (val == LOW) {
            *out &= ~(bit << (pin - 69));
        } else {
            *out |= (bit << (pin - 69));
        }

        SREG = oldSREG;
    }
}

void pinModeExtended(uint8_t port, uint8_t pin, uint8_t ioMode)
{
    switch (port)
    {
        case PORT_A:
        {
            if (ioMode == INPUT)
            {
                DDRA &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRA |= (1 << pin);
            }
            else
            {
                DDRA &= ~(1 << pin);
                PORTA |= (1 << pin);
            }
            break;
        }
        case PORT_B:
        {
            if (ioMode == INPUT)
            {
                DDRB &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRB |= (1 << pin);
            }
            else
            {
                DDRB &= ~(1 << pin);
                PORTB |= (1 << pin);
            }
            break;
        }    
        case PORT_C:
        {
            if (ioMode == INPUT)
            {
                DDRC &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRC |= (1 << pin);
            }
            else
            {
                DDRC &= ~(1 << pin);
                PORTC |= (1 << pin);
            }
            break;
        }  
        case PORT_D:
        {
            if (ioMode == INPUT)
            {
                DDRD &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRD |= (1 << pin);
            }
            else
            {
                DDRD &= ~(1 << pin);
                PORTD |= (1 << pin);
            }
            break;
        }  
        case PORT_E:
        {
            if (ioMode == INPUT)
            {
                DDRE &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRE |= (1 << pin);
            }
            else
            {
                DDRE &= ~(1 << pin);
                PORTE |= (1 << pin);
            }
            break;
        }  
        case PORT_F:
        {
            if (ioMode == INPUT)
            {
                DDRF &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRF |= (1 << pin);
            }
            else
            {
                DDRF &= ~(1 << pin);
                PORTF |= (1 << pin);
            }
            break;
        }  
        case PORT_G:
        {
            if (ioMode == INPUT)
            {
                DDRG &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRG |= (1 << pin);
            }
            else
            {
                DDRG &= ~(1 << pin);
                PORTG |= (1 << pin);
            }
            break;
        }  
        case PORT_H:
        {
            if (ioMode == INPUT)
            {
                DDRH &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRH |= (1 << pin);
            }
            else
            {
                DDRH &= ~(1 << pin);
                PORTH |= (1 << pin);
            }
            break;
        }  
        case PORT_J:
        {
            if (ioMode == INPUT)
            {
                DDRJ &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRJ |= (1 << pin);
            }
            else
            {
                DDRJ &= ~(1 << pin);
                PORTJ |= (1 << pin);
            }
            break;
        }  
        case PORT_K:
        {
            if (ioMode == INPUT)
            {
                DDRK &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRK |= (1 << pin);
            }
            else
            {
                DDRK &= ~(1 << pin);
                PORTK |= (1 << pin);
            }
            break;
        }  
        case PORT_L:
        {
            if (ioMode == INPUT)
            {
                DDRL &= ~(1 << pin);
            }
            else if (ioMode == OUTPUT)
            {
                DDRL |= (1 << pin);
            }
            else
            {
                DDRL &= ~(1 << pin);
                PORTL |= (1 << pin);
            }
            break;
        }  
        default:
        {
            break;
        }
    }
}