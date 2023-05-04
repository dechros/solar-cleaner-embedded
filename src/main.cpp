#include "serialParser.h"
#include "secondTask.h"

void setup()
{
    /* TODO :    - Initialize IO's
     *              + Digital IO R/W
     *              + Analog IO R/W
     *              + PWM IO
     *
     *          - Initialize UART channel for taking commands with interrupt
     *              + 9600 Baud
     *              + Use for debugging purpose
     *
     */
    BaseType_t xReturned;
    Serial.begin(9600);
    xReturned = xTaskCreate(vSecondTask,
                            "vSecondTask",
                            configMINIMAL_STACK_SIZE,
                            (void *)1,
                            tskIDLE_PRIORITY,
                            &xSecondTaskHandle);
    if (xReturned == pdPASS)
    {
        serialWrite("vSecondTask Created Successfully!");
    }
    vTaskStartScheduler();
}

void loop()
{
}