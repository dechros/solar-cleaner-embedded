#include "secondTask.h"

void vSecondTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (true)
    {
        SerialPrintln("Second");
        vTaskDelayUntil(&xLastWakeTime, 200);
    }
}