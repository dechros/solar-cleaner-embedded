#include <Arduino.h>
#include "maintenanceOperations.h"
#include "routerOperations.h"
#include "pinDefinitions.h"

bool maintenanceActive = false;

uint8_t CheckMaintenanceMode(void)
{
    return digitalRead(MAINTENANCE_PIN);  
}

bool SendMaintenanceModeMessage(bool mode)
{
    if (mode == true)
    {
        ROUTER_SERIAL.write(MAINTENANCE_MODE_ACTIVE_MESSAGE, 4);
    }
    else
    {
        ROUTER_SERIAL.write(MAINTENANCE_MODE_DEACTIVE_MESSAGE, 4);
    }
    return ReceiveACK();
}

bool ReceiveACK()
{
    bool returnVal = false;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (ROUTER_SERIAL.available() < 3)
        {
            delay(10);
        }
        else
        {
            uint8_t message[3] = {0};
            ROUTER_SERIAL.readBytes(message, 3);
            if (strncmp((const char*)message, ACK_MESSAGE, 3) == 0)
            {
                returnVal = true;
                break;
            }
        }
    }
    return returnVal;
}

void CheckMaintenanceMessages(void)
{
    if(ROUTER_SERIAL.available() > 0)
    {
        if (ROUTER_SERIAL.peek() == 'G' && ROUTER_SERIAL.available() >= 5)
        {
            uint8_t message[5] = {0};
            ROUTER_SERIAL.readBytes(message, 5);
            if (strncmp((const char*)message, GET_MACHINE_NAME_MESSAGE, 5) == 0)
            {
                /*WWW*/
            }
        }
    }
}
