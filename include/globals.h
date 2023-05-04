#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>
#include <stdint.h>

#include "Arduino_FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

extern TaskHandle_t xSerialParserTaskHandle;
extern TaskHandle_t xSecondTaskHandle;
extern SemaphoreHandle_t xSerialMutex;

#endif /* _GLOBALS_H_ */