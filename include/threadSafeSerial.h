/**
 * @file threadSafeSerial.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief A header file of thread safed Hardware Serial functions. 
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
 * @brief Thread safe Serial.begin();
 * 
 * @param baudRate Baudrate of the serial port.
 */
void SerialBegin(int baudRate);

/**
 * @brief Thread safe Serial.available();
 * 
 * @return int Returns available byte count.
 */
int SerialAvailable();

/**
 * @brief Thread safe Serial.write();
 * 
 * @param buffer Pointer to the buffer.
 * @param size Size of message to write
 */
void SerialWrite(const char *buffer, size_t size);

/**
 * @brief Thread safe Serial.print();
 * 
 * @param message Null terminated char* to print.
 */
void SerialPrint(const char *message);

/**
 * @brief Thread safe Serial.print();
 * 
 * @param character Char to print
 */
void SerialPrint(char character);

/**
 * @brief Thread safe Serial.print();
 * 
 * @param number number to print
 */
void SerialPrint(int number);

/**
 * @brief Thread safe Serial.println();
 * 
 * @param message Null terminated char* to print.
 */
void SerialPrintln(const char *message);

/**
 * @brief Thread safe Serial.read();
 * 
 * @return uint8_t Returns the value from 
 *         front of serial buffer and pops it.
 */
uint8_t SerialRead(void);

/**
 * @brief Thread safe Serial.peek();
 * 
 * @return uint8_t Returns the value from 
 *         front of serial buffer. Just peeks.
 */
uint8_t SerialPeek(void);

/**
 * @brief This function flushes the serial buffer 
 *        and ends the serial connection.
 * 
 */
void SerialEnd(void);

/**
 * @brief This function flushes the serial buffer 
 * 
 */
void SerialFlush(void);

/**
 * @brief Clears serial receive buffer
 * 
 */
void SerialReceiveBufferClear(void);

#endif /* _THREAD_SAFE_SERIAL_H_ */