#include "secondTask.h"

void vSecondTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);

    while (true)
    {
        xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        Serial.println("Second");
        xSemaphoreGive(xSerialMutex);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        taskYIELD();
    }
}