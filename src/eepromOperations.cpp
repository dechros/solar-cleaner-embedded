#include "eepromOperations.h"
#include "globals.h"

void InitParameters(void)
{
    uint8_t controlValue = 0;
    EEPROM.get(0, controlValue);
    if (controlValue != CONTROL_VALUE)
    {
        ClearEEPROM();
        SetDefaultParametersToEEPROM();
    }
    SystemParameters = ReadParametersFromEEPROM();
}

void ClearEEPROM(void)
{
    for (uint16_t i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.put(i, 255);
    }
}

void LogError(ErrorType_t error)
{
    uint16_t address = 0;
    switch (error)
    {
        case LEFT_ERROR:
        {
            address = LEFT_ERROR_ADDRESS;
            break;
        }
        case RIGHT_ERROR:
        {
            address = RIGHT_ERROR_ADDRESS;
            break;
        }
        case BRUSH_ERROR:
        {
            address = BRUSH_ERROR_ADDRESS;
            break;
        }
        case CONTROLLER_ERROR:
        {
            address = CONTROLLER_ERROR_ADDRESS;
            break;
        }
        default:
        {
            break;
        }
    }
    if (address != 0)
    {
        uint16_t count = ReadErrorCountFromEEPROM(address);
        count++;
        WriteErrorCountToEEPROM(address, count);
    }
}

Parameters_t ReadParametersFromEEPROM(void)
{
    Parameters_t parameters;
    EEPROM.get(0, parameters);
    return parameters;
}

void ReadMachineNameFromEEPROM(uint8_t* name)
{
    MachineName_t localName;
    EEPROM.get(4, localName);
    memcpy(name, localName.name, sizeof(MachineName_t));
}

void WriteParametersToEEPROM(Parameters_t parameters)
{
    parameters.controlValue = CONTROL_VALUE;
    parameters.versionMajor = SOFTWARE_VERSION_MAJOR;
    parameters.versionMinor = SOFTWARE_VERSION_MINOR;
    parameters.versionPatch = SOFTWARE_VERSION_PATCH;
    EEPROM.put(0, parameters);
}

uint16_t ReadErrorCountFromEEPROM(uint16_t address)
{
    uint16_t count;
    EEPROM.get(address, count);
    return count;
}

void WriteErrorCountToEEPROM(uint16_t address, uint16_t count)
{
    EEPROM.put(address, count);
}

void SetDefaultParametersToEEPROM(void)
{
    Parameters_t parameters;
    parameters.controlValue = CONTROL_VALUE;
    parameters.versionMajor = SOFTWARE_VERSION_MAJOR;
    parameters.versionMinor = SOFTWARE_VERSION_MINOR;
    parameters.versionPatch = SOFTWARE_VERSION_PATCH;
    for (uint8_t i = 0; i < MACHINE_NAME_SIZE; i++)
    {
        parameters.companyName[i] = MACHINE_NAME[i];
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        parameters.machineIP[i] = 0;
    }
    parameters.leftErrorCount           = 0;
    parameters.rightErrorCount          = 0;
    parameters.brushErrorCount          = 0;
    parameters.controllerErrorCount     = 0;
    parameters.leftRampUp               = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.leftRampDown             = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.leftMinSpeed             = 0;
    parameters.leftMaxSpeed             = 100;
    parameters.rightRampUp              = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.rightRampDown            = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.rightMinSpeed            = 0;
    parameters.rightMaxSpeed            = 100;
    parameters.brushRampUp              = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.brushRampDown            = 5; /* 1 - 10 */ /* 250ms surucu rampa parametreleri */
    parameters.brushMinSpeed            = 0;
    parameters.brushMaxSpeed            = 100;
    parameters.joystickMiddleValue      = 127;    
    parameters.joystickDeadZone         = 0; 
    parameters.joystickMinValue         = 0; 
    parameters.joystickMaxValue         = 255; 
    parameters.potantiometerMinValue    = 0;        
    parameters.potantiometerMaxValue    = 255;    
    WriteParametersToEEPROM(parameters);
}