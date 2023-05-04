#include "serialParser.h"
#include "secondTask.h"

void setup()
{
    /**
     * @todo    - Initialize IO's
     *              + Digital IO R/W
     *              + Analog IO R/W
     *              + PWM IO
     *
     *          - Initialize UART channel for taking commands with interrupt
     *              + 9600 Baud
     *              + Use for debugging purpose
     *
     */

    /* Create a queue capable of containing 10 unsigned long values. */

    Serial.begin(9600);
    xSerialMutex = xSemaphoreCreateMutex();
    BaseType_t xReturned = xTaskCreate(vSerialParserTask,
                                       "vSerialParserTask",
                                       configMINIMAL_STACK_SIZE,
                                       (void *)1,
                                       tskIDLE_PRIORITY,
                                       &xSerialParserTaskHandle);
    if (xReturned == pdPASS)
    {
        Serial.println("vSerialParserTask Created Successfully!");
    }
    xReturned = xTaskCreate(vSecondTask,
                            "vSecondTask",
                            configMINIMAL_STACK_SIZE,
                            (void *)1,
                            tskIDLE_PRIORITY,
                            &xSecondTaskHandle);
    if (xReturned == pdPASS)
    {
        xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        Serial.println("vSecondTask Created Successfully!");
        xSemaphoreGive(xSerialMutex);
    }
    vTaskStartScheduler();
}

void loop()
{
}