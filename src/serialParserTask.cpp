#include "serialParserTask.h"

void vSerialParserTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);

    int queueSize = 0;
    while (true)
    {
        queueSize = SerialAvailable();
        if (queueSize > 0)
        {
            for (int i = 0; i < queueSize; i++)
            {
                char readValue = (char)SerialRead();
                if (readValue != -1)
                {
                    SerialPrint(readValue);
                }
                else
                {
                    SerialPrintln("Error Serial Read!");
                }
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}