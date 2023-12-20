#include <Arduino.h>
#include "maintenanceOperations.h"
#include "routerOperations.h"
#include "pinDefinitions.h"
#include "eepromOperations.h"
#include "globals.h"

static bool maintenanceActive = false;

static void ClearMessageBuffer(void);
static bool ReceiveACK(void);

bool IsRouterMaintenanceActive(void)
{
    return maintenanceActive;
}

uint8_t CheckMaintenancePin(void)
{
    return digitalRead(MAINTENANCE_PIN);  
}

void ActivateRouterMaintenanceMode(void)
{
    ClearMessageBuffer();
    ROUTER_SERIAL.write(MAINTENANCE_MODE_ACTIVE_MESSAGE, 4);
    if (ReceiveACK() == true)
    {
        maintenanceActive = true;
    }
}

void DeactivateRouterMaintenanceMode(void)
{
    ROUTER_SERIAL.write(MAINTENANCE_MODE_DEACTIVE_MESSAGE, 4);
    if (ReceiveACK() == true)
    {
        maintenanceActive = false;
    }
}


static bool ReceiveACK()
{
    bool returnVal = false;
    for (uint16_t i = 0; i < MESSAGE_TIMEOUT_MS; i++)
    {
        if (ROUTER_SERIAL.available() < 3)
        {
            delay(1);
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
    if(ROUTER_SERIAL.available() >= 5)
    {
        if (ROUTER_SERIAL.peek() == 'G' && ROUTER_SERIAL.available() >= 5)
        {
            uint8_t message[5] = {0};
            ROUTER_SERIAL.readBytes(message, 5);
            if (strncmp((const char*)message, GET_MACHINE_NAME_MESSAGE, 5) == 0)
            {
                uint8_t name[MACHINE_NAME_SIZE] = {0};
                ReadMachineNameFromEEPROM(name);
                ROUTER_SERIAL.write(name, MACHINE_NAME_SIZE);
            }
            else if (strncmp((const char*)message, GET_PARAMETERS_MESSAGE, 5) == 0)
            {
                Parameters_t parameters = ReadParametersFromEEPROM();
                ROUTER_SERIAL.write((uint8_t*)&parameters, sizeof(Parameters_t));
            }            
        }
        else if (ROUTER_SERIAL.peek() == 'S' && ROUTER_SERIAL.available() >= 5)
        {
            uint8_t message[5] = {0};
            ROUTER_SERIAL.readBytes(message, 5);
            if (strncmp((const char*)message, SET_PARAMETERS_MESSAGE, 5) == 0)
            {
                bool timeout = true;
                uint8_t offsetIndex = 0;
                uint8_t messageCount = 0;
                Parameters_t newParameters;
                for (uint16_t i = 0; i < 1000; i++)
                {
                    uint8_t size = ROUTER_SERIAL.available();
                    if (size > 0)
                    {
                        for (uint8_t i = 0; i < size; i++)
                        {
                            newParameters.data[i + offsetIndex] = ROUTER_SERIAL.read();
                            messageCount++;
                            if (messageCount == sizeof(Parameters_t))
                            {
                                break;
                            }
                        }
                        offsetIndex += size;
                    }
                    if (messageCount == sizeof(Parameters_t))
                    {
                        timeout = false;
                        WriteParametersToEEPROM(newParameters);
                        SystemParameters = ReadParametersFromEEPROM();
                        LeftTrackMotor.UpdateMotorParameters();
                        RightTrackMotor.UpdateMotorParameters();
                        BrushesMotor.UpdateMotorParameters();
                        ROUTER_SERIAL.write(ACK_MESSAGE, 3);
                        break;
                    }
                    delay(1);
                }
                if (timeout == true)
                {
                    ClearMessageBuffer();
                }
            }
        }
        else if ((ROUTER_SERIAL.peek() == 'G' || ROUTER_SERIAL.peek() == 'S') &&
                  ROUTER_SERIAL.available() < 5)
        {
            static uint8_t missingMessageDiscardCount = 0;
            delay(1);
            missingMessageDiscardCount++;
            if (missingMessageDiscardCount == 100)
            {
                Serial.println("Missing Message Discarded");
                ClearMessageBuffer();
            }
            
        }
        else
        {
            Serial.println("Discard One Byte");
            ROUTER_SERIAL.read();
        }
    }
    else if(ROUTER_SERIAL.available() < 5 && ROUTER_SERIAL.available() > 0)
    {
        static uint8_t missingMessageDiscardCount = 0;
        delay(1);
        missingMessageDiscardCount++;
        if (missingMessageDiscardCount == 100)
        {
            Serial.println("Half Message Cleared");
            ClearMessageBuffer();
        }
    }
}

static void ClearMessageBuffer()
{
    for (uint16_t i = 0; i < (uint16_t)ROUTER_SERIAL.available(); i++)
    {
        ROUTER_SERIAL.read();
    }
}
