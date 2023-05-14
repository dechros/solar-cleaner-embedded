/**
 * @file threadSafeSerial.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief A source file of thread safed Hardware Serial functions. 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "threadSafeSerial.h"

/* TODO: This define could be changed to 
 *       any other serial if needed like "Serial1" 
 */
#define PC_SERIAL Serial

SemaphoreHandle_t xSerialMutex;

void SerialBegin(int baudRate)
{
    xSerialMutex = xSemaphoreCreateMutex();
    if(xSerialMutex == NULL)
    {
        PC_SERIAL.println("xSerialMutex Create Error!");
        while (true);
    }

    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.begin(baudRate);
    xSemaphoreGive(xSerialMutex);
}

int SerialAvailable()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    int available = PC_SERIAL.available();
    xSemaphoreGive(xSerialMutex);
    return available;
}

void SerialWrite(const char *buffer, size_t size)
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.write(buffer, size);
    xSemaphoreGive(xSerialMutex);
}

void SerialPrint(const char *message)
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.print(message);
    xSemaphoreGive(xSerialMutex);
}

void SerialPrint(char character)
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.print(character);
    xSemaphoreGive(xSerialMutex);
}

void SerialPrint(int number)
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.print(number);
    xSemaphoreGive(xSerialMutex);
}

void SerialPrintln(const char *message)
{
    /* TODO: This function needs to be configured for Verbose on/off */
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.println(message);
    xSemaphoreGive(xSerialMutex);
}

uint8_t SerialRead()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    uint8_t readByte = (uint8_t)PC_SERIAL.read();
    xSemaphoreGive(xSerialMutex);
    return readByte;
}

uint8_t SerialPeek()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    uint8_t peekByte = (uint8_t)PC_SERIAL.peek();
    xSemaphoreGive(xSerialMutex);
    return peekByte;
}

void SerialEnd()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.flush();
    PC_SERIAL.end();
    xSemaphoreGive(xSerialMutex);
}

void SerialFlush()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    PC_SERIAL.flush();
    xSemaphoreGive(xSerialMutex);
}

void SerialReceiveBufferClear()
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    uint8_t byteCount = PC_SERIAL.available();
    PC_SERIAL.print("Buffer Cleared. Size - ");
    PC_SERIAL.println(byteCount);
    for (uint8_t i = 0; i < byteCount; i++)
    {
        PC_SERIAL.read();
    }
    xSemaphoreGive(xSerialMutex);
}