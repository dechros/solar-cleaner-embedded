/**
 * @file serialParser.h
 * @author Halit Cetin (halitcetin@live.com)
 * @brief This file includes serial debugging operations.
 * @version 0.1
 * @date 04-05-2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _SERIAL_PARSER_H_
#define _SERIAL_PARSER_H_

#include "globals.h"

/**
 * @brief Thread safe serial write function
 * 
 * @param text 
 */
void serialWrite(String text);

/**
 * @brief Thread safe serial read function
 * 
 * @return String 
 */
String serialRead();

#endif /* _SERIAL_PARSER_H_ */