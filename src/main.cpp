//#include <Arduino.h>
//#include "EnableInterrupt.h"
//#include "eepromOperations.h"
//#include "CAN.h"
//
//uint8_t brightness = 0;
//
//void setup()
//{
//    Serial.begin(9600);
//    Serial.println("Code Begin");
//    InitializeEEPROM();
//    //ErrorHistoryClear();
//
//    Error_t myError;
//    myError.errorID = 5;
//    myError.errorSource = 'R';
//    LogError(myError);
//
//    Error_t readError;
//    readError = GetError(1);
//    uint16_t errorCount = GetErrorCount();
//    Serial.println(readError.errorID);
//    Serial.println((char)readError.errorSource);
//    Serial.println(errorCount);
//
//    EepromHeader_t readHeader;
//    Serial.println((uint8_t)sizeof(EepromHeader_t));
//    EEPROM.get(EEPROM_START_ADDRESS, readHeader);
//
//    for (uint8_t i = 0; i < 8; i++)
//    {
//        Serial.print((char)readHeader.companyName.character[i]);
//    }
//    Serial.println("");
//
//    Serial.println(readHeader.version.major);
//    Serial.println(readHeader.version.minor);
//    Serial.println(readHeader.version.patch);
//    Serial.println(readHeader.date.day);
//    Serial.println(readHeader.date.month);
//    Serial.println(readHeader.date.year);
//    Serial.println(readHeader.errorCount);
//    Serial.println(readHeader.CRC16);
//}
//
//void loop()
//{
//    
//}

#include "globals.h"
#include "serialParserTask.h"
#include "readInputPinsTask.h"
#include "pinOperations.h"
#include "motorDriver.h"
#include "waterPumpDriver.h"
#include "eepromOperations.h"
#include "CAN.h"

void setup()
{
    InitializePins();
    InitializeMotors();
    InitializeEEPROM();
    WaterPumpOff();
    SerialBegin(9600);

    BaseType_t xReturned;
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
    xReturned = xTaskCreate(vReadInputPinsTask, 
                        "vReadInputPinsTask", 
                        configMINIMAL_STACK_SIZE, 
                        (void *)1, 
                        tskIDLE_PRIORITY, 
                        &xSecondTaskHandle);
    if (xReturned == pdPASS)
    {
        SerialPrintln("vSecondTask Created Successfully!");
    }
    vTaskStartScheduler();
}

void loop()
{
    /* Leave Empty! */
}