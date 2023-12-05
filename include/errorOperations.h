/**
 * @file errorOperations.h
 * @author Halit Cetin (halitcetin@live.com)
 * @brief Error handling operations header file.
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ERROR_OPERATIONS_H
#define ERROR_OPERATIONS_H

#include "Arduino.h"
#include "pinDefinitions.h"

/**
 * @brief Detects driver errors vis error output pin coming
 * from drivers.
 * 
 * @return true Error is detected
 * @return false Error is not detected
 */
bool detectDriverError(void);

#endif