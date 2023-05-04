#include "secondTask.h"

void vSecondTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);

    while (true)
    {
        serialWrite("Second");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        /**
         * TODO: Convert this task to taskYIELD();
         */
    }
}