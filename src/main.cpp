#include <Arduino.h>
#include "esp32Lib.h"
// #include "CAN.h"



void setup()
{
  startESP32();
}

void loop()
{
    delay(1000);
    sendString("naber mudur");
}



// #include "globals.h"
// #include "serialParserTask.h"
// #include "readInputPinsTask.h"
// #include "pinOperations.h"
// #include "motorDriver.h"
// #include "waterPumpDriver.h"
// #include "eepromOperations.h"
// #include "CAN.h"

// void setup()
//{
//     InitializePins();
//     InitializeMotors();
//     InitializeEEPROM();
//     WaterPumpOff();
//     SerialBegin(9600);
//
//     BaseType_t xReturned;
//     xReturned = xTaskCreate(vSerialParserTask,
//                             "vSerialParserTask",
//                             configMINIMAL_STACK_SIZE,
//                             (void *)1,
//                             tskIDLE_PRIORITY,
//                             &xSerialParserTaskHandle);
//     if (xReturned == pdPASS)
//     {
//         SerialPrintln("vSerialParserTask Created Successfully!");
//     }
//     xReturned = xTaskCreate(vReadInputPinsTask,
//                         "vReadInputPinsTask",
//                         configMINIMAL_STACK_SIZE,
//                         (void *)1,
//                         tskIDLE_PRIORITY,
//                         &xSecondTaskHandle);
//     if (xReturned == pdPASS)
//     {
//         SerialPrintln("vSecondTask Created Successfully!");
//     }
//     vTaskStartScheduler();
// }
//
// void loop()
//{
//     /* Leave Empty! */
// }