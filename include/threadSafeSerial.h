/**
 * @file threadSafeSerial.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _THREAD_SAFE_SERIAL_H_
#define _THREAD_SAFE_SERIAL_H_

#include <Arduino.h>
#include <stdint.h>

#include "Arduino_FreeRTOS.h"
#include "semphr.h"

/**
 * @brief 
 * 
 * @param baudRate 
 */
void SerialBegin(int baudRate);

/**
 * @brief 
 * 
 * @return int 
 */
int SerialAvailable();

/**
 * @brief 
 * 
 * @param buffer 
 * @param size 
 */
void SerialWrite(const char *buffer, size_t size);

/**
 * @brief 
 * 
 * @param message 
 */
void SerialPrint(const char *message);

/**
 * @brief 
 * 
 * @param character 
 */
void SerialPrint(char character);

/**
 * @brief 
 * 
 * @param message 
 */
void SerialPrintln(const char *message);

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t SerialRead(void);

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t SerialPeek(void);

/**
 * @brief 
 * 
 */
void SerialEnd(void);

#endif /* _THREAD_SAFE_SERIAL_H_ */