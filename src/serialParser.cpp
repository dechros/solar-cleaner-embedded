#include "serialParser.h"

void vSerialParserTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);

    uint16_t queueSize = 0;

    while (true)
    {
        xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        queueSize = Serial.available();
        xSemaphoreGive(xSerialMutex);
        if (queueSize > 0)
        {
            for (uint16_t i = 0; i < queueSize; i++)
            {
                xSemaphoreTake(xSerialMutex, portMAX_DELAY);
                int readValue = Serial.read();
                xSemaphoreGive(xSerialMutex);

                if (readValue != -1)
                {
                    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
                    Serial.println((char)readValue);
                    xSemaphoreGive(xSerialMutex);
                }
                else
                {
                    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
                    Serial.println("Error Serial Read!");
                    xSemaphoreGive(xSerialMutex);
                }
            }
        }
        else
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}