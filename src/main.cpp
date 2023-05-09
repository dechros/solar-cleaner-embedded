//#include <Arduino.h>
//#include "EnableInterrupt.h"
//#include "CAN.h"
//
//uint8_t brightness = 0;
//
//void ISR_8();
//void ISR_9();
//void ISR_10();
//void ISR_11();
//void ISR_12();
//void ISR_13();
//void ISR_14();
//void ISR_15();
//
//void setup()
//{
//    Serial.begin(9600);
//    pinMode(A8, INPUT_PULLUP);
//    pinMode(A9, INPUT_PULLUP);
//    pinMode(A10, INPUT_PULLUP);
//    pinMode(A11, INPUT_PULLUP);
//    pinMode(A12, INPUT_PULLUP);
//    pinMode(A13, INPUT_PULLUP);
//    pinMode(A14, INPUT_PULLUP);
//    pinMode(A15, INPUT_PULLUP);
//    enableInterrupt(A8,  ISR_8,  CHANGE);
//    enableInterrupt(A9,  ISR_9,  CHANGE);
//    enableInterrupt(A10, ISR_10, CHANGE);
//    enableInterrupt(A11, ISR_11, CHANGE);
//    enableInterrupt(A12, ISR_12, CHANGE);
//    enableInterrupt(A13, ISR_13, CHANGE);
//    enableInterrupt(A14, ISR_14, CHANGE);
//    enableInterrupt(A15, ISR_15, CHANGE);
//}
//
//void ISR_8()
//{
//    Serial.print("ISR_8 - ");
//    Serial.println(digitalRead(A8));
//}
//
//void ISR_9()
//{
//    Serial.print("ISR_9 - ");
//    Serial.println(digitalRead(A9));
//}
//
//void ISR_10()
//{
//    Serial.print("ISR_10 - ");
//    Serial.println(digitalRead(A10));
//}
//
//void ISR_11()
//{
//    Serial.print("ISR_11 - ");
//    Serial.println(digitalRead(A11));
//}
//
//void ISR_12()
//{
//    Serial.print("ISR_12 - ");
//    Serial.println(digitalRead(A12));
//}
//
//void ISR_13()
//{
//    Serial.print("ISR_13 - ");
//    Serial.println(digitalRead(A13));
//}
//
//void ISR_14()
//{
//    Serial.print("ISR_14 - ");
//    Serial.println(digitalRead(A14));
//}
//
//void ISR_15()
//{
//    Serial.print("ISR_15 - ");
//    Serial.println(digitalRead(A15));
//}
//
//void loop()
//{
//    
//}

#include "globals.h"
#include "serialParserTask.h"
#include "secondTask.h"
#include "pinOperations.h"
#include "motorDriver.h"
#include "waterPumpDriver.h"
#include "CAN.h"

void setup()
{
    InitializePins();
    InitializeMotors();
    WaterPumpOff();
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