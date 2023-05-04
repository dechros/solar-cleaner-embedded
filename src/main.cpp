#include "main.h"

TaskHandle_t xSerialParserTaskHandle = NULL;
TaskHandle_t xSecondTaskHandle = NULL;
SemaphoreHandle_t xSerialMutex;

void vSerialParserTask( void * pvParameters )
{
    configASSERT((( uint32_t )pvParameters) == 1);

    uint16_t queueSize = 0;

    for(;;)
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
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }
}

void vSecondTask( void * pvParameters )
{
    configASSERT((( uint32_t )pvParameters) == 1);

    for(;;)
    {
        xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        Serial.println("Second");
        xSemaphoreGive(xSerialMutex);

        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void setup() 
{
    /* 
     * TODO: 
     *
     *  - Initialize IO's
     *      + Digital IO R/W
     *      + Analog IO R/W
     *      + PWM IO
     * 
     *  - Initialize UART channel for taking commands with interrupt
     *      + 9600 Baud
     *      + Use for debugging purpose
     *  
     *  - 
     */

    /* Create a queue capable of containing 10 unsigned long values. */
    
    Serial.begin(9600);

    xSerialMutex = xSemaphoreCreateMutex();

    BaseType_t xReturned = xTaskCreate  (vSerialParserTask, 
                                        "vSerialParserTask", 
                                        configMINIMAL_STACK_SIZE, 
                                        (void *)1, 
                                        tskIDLE_PRIORITY, 
                                        &xSerialParserTaskHandle);
    
    if(xReturned == pdPASS)
    {
        Serial.println("vSerialParserTask Created Successfully!");
    }

    xReturned = xTaskCreate( vSecondTask, 
                                        "vSecondTask", 
                                        configMINIMAL_STACK_SIZE, 
                                        (void *)1, 
                                        tskIDLE_PRIORITY, 
                                        &xSecondTaskHandle);
    
    if(xReturned == pdPASS)
    {
        xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        Serial.println("vSecondTask Created Successfully!");
        xSemaphoreGive(xSerialMutex);
    }

    
}

void loop() 
{
    xSemaphoreTake(xSerialMutex, portMAX_DELAY);
    Serial.println("Memory Error Scheduler Broke! Please Restart");
    xSemaphoreGive(xSerialMutex);
}