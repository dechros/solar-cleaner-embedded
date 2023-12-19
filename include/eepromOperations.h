#ifndef _EEPROM_OPERATIONS_H_
#define _EEPROM_OPERATIONS_H_

#include <EEPROM.h>
#include <string.h>
#include <stdint.h>
#include "softwareVersion.h"
#include "pinOperations.h"

#define CONTROL_VALUE                  31

#define LEFT_ERROR_ADDRESS             58   
#define RIGHT_ERROR_ADDRESS            60 
#define BRUSH_ERROR_ADDRESS            62 
#define CONTROLLER_ERROR_ADDRESS       64 

typedef enum
{
    LEFT_ERROR,
    RIGHT_ERROR,
    BRUSH_ERROR,
    CONTROLLER_ERROR,
    NO_ERROR
}ErrorType_t;

typedef struct
{
    uint8_t name[50];
}MachineName_t;


typedef struct /* 84 byte */
{
    uint8_t controlValue;
    uint8_t versionMajor;
    uint8_t versionMinor;
    uint8_t versionPatch;
    uint8_t companyName[50];
    uint8_t machineIP[4];
    uint16_t leftErrorCount;
    uint16_t rightErrorCount;
    uint16_t brushErrorCount;
    uint16_t controllerErrorCount;
    uint8_t leftRampUp;
    uint8_t leftRampDown;
    uint8_t leftMinSpeed;
    uint8_t leftMaxSpeed;
    uint8_t rightRampUp;
    uint8_t rightRampDown;
    uint8_t rightMinSpeed;
    uint8_t rightMaxSpeed;
    uint8_t brushRampUp;
    uint8_t brushRampDown;
    uint8_t brushMinSpeed;
    uint8_t brushMaxSpeed;
    uint8_t joystickMiddleValue;
    uint8_t joystickDeadZone;
    uint8_t joystickMinValue;
    uint8_t joystickMaxValue;
    uint8_t potantiometerMinValue;
    uint8_t potantiometerMaxValue;
}Parameters_t;

/**
 * @brief 
 * 
 */
void InitParameters(void);

/**
 * @brief 
 * 
 */
void ClearEEPROM(void);

/**
 * @brief 
 * 
 * @param error 
 */
void LogError(ErrorType_t error);

/**
 * @brief 
 * 
 * @return Parameters_t 
 */
Parameters_t ReadParametersFromEEPROM(void);

/**
 * @brief 
 * 
 * @param name 
 */
void ReadMachineNameFromEEPROM(uint8_t* name);

/**
 * @brief 
 * 
 * @param parameters 
 */
void WriteParametersToEEPROM(Parameters_t parameters);

/**
 * @brief 
 * 
 * @param address 
 * @return uint16_t 
 */
uint16_t ReadErrorCountFromEEPROM(uint16_t address);

/**
 * @brief 
 * 
 * @param address 
 * @param count 
 */
void WriteErrorCountToEEPROM(uint16_t address, uint16_t count);

/**
 * @brief Set the Default Parameters To EEPROM
 * 
 */
void SetDefaultParametersToEEPROM(void);

#endif /* _EEPROM_OPERATIONS_H_ */