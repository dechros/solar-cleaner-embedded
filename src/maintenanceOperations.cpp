#include <Arduino.h>
#include "maintenanceOperations.h"
#include "routerOperations.h"
#include "pinDefinitions.h"

bool maintenanceActive = false;

bool CheckMaintenanceMode(void)
{
    if (digitalRead(MAINTENANCE_PIN) == HIGH)
    {
        SendMaintenanceModeMessage(true);
        return true;
    }
    SendMaintenanceModeMessage(false);
    return false;  
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

void SendMaintenanceModeMessage(bool mode)
{
    if (mode == true)
    {
        ROUTER_SERIAL.write(MAINTENANCE_MODE_ACTIVE_MESSAGE, 4);
    }
    else
    {
        ROUTER_SERIAL.write(MAINTENANCE_MODE_DEACTIVE_MESSAGE, 4);
    }
}