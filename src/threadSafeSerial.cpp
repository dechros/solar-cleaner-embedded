/**
 * @file threadSafeSerial.cpp
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "threadSafeSerial.h"

/* TODO: This define will be changed to "Serial1" on real board 
 *       because it uses that port with 0 and 1 pins.
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

void SerialPrintln(const char *message)
{
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