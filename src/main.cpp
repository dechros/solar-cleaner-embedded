#include "globals.h"
#include "serialParserTask.h"
#include "secondTask.h"
#include "CAN.h"

void setup()
{
    InitializePins();
    SerialBegin(9600);

    BaseType_t xReturned;
    xReturned = xTaskCreate(vSecondTask,
                            "vSecondTask",
                            configMINIMAL_STACK_SIZE,
                            (void *)1,
                            tskIDLE_PRIORITY,
                            &xSecondTaskHandle);
    if (xReturned == pdPASS)
    {
        SerialPrintln("vSecondTask Created Successfully!");
    }
    xReturned = xTaskCreate(vSerialParserTask,
                            "vSerialParserTask",
                            configMINIMAL_STACK_SIZE,
                            (void *)1,
                            tskIDLE_PRIORITY,
                            &xSerialParserTaskHandle);
    if (xReturned == pdPASS)
    {
        SerialPrintln("vSerialParserTask Created Successfully!");
    }
    vTaskStartScheduler();
}

void loop()
{
    /* Leave Empty! */
}