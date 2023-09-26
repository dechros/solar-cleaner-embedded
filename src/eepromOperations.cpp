#include "eepromOperations.h"

bool CRC16ErrorFlag = false;
bool CRC32ErrorFlag = false;

uint16_t errorWriteIndex = 0;

void InitializeEEPROM()
{
    EepromHeader_t eepromHeader;
    EEPROM.get(EEPROM_START_ADDRESS, eepromHeader);
    if (eepromHeader.errorCount == 0xFFFF)
    {
        eepromHeader.errorCount = 0;
    }
    if (eepromHeader.CRC16 == 0xFFFF)
    {
        eepromHeader.CRC16 = 0;
    }
    
    const char companyName[SIZE_OF_COMPANY_NAME + 1] = COMPANY_NAME;
    memcpy(&eepromHeader.companyName.character[0], companyName, SIZE_OF_COMPANY_NAME);
    eepromHeader.version.major = SOFTWARE_VERSION_MAJOR;
    eepromHeader.version.minor = SOFTWARE_VERSION_MINOR;
    eepromHeader.version.patch = SOFTWARE_VERSION_PATCH;
    eepromHeader.date.day = SOFTWARE_UPDATE_DATE_DAY;
    eepromHeader.date.month = SOFTWARE_UPDATE_DATE_MONTH;
    eepromHeader.date.year = SOFTWARE_UPDATE_DATE_YEAR;    
    EEPROM.put(EEPROM_START_ADDRESS, eepromHeader);
}

void ClearEEPROM()
{
    for (uint16_t i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.put(i, 255);
    }
}

void LogError(Error_t error)
{
    uint16_t errorCount;
    EEPROM.get(EEPROM_ERROR_COUNT_ADDRESS, errorCount);
    EEPROM.put(EEPROM_ERROR_SECTOR_START_ADDRESS + (sizeof(Error_t)*errorCount), error);
    errorCount = errorCount + 1;
    EEPROM.put(EEPROM_ERROR_COUNT_ADDRESS, errorCount);
}

CompanyName_t GetCompanyName()
{
    CompanyName_t companyName;
    EEPROM.get(EEPROM_COMPANY_NAME_ADDRESS, companyName);
    return companyName;
}

Version_t GetSoftwareVersion()
{
    Version_t softwareVersion;
    EEPROM.get(EEPROM_MAJOR_VERSION_ADDRESS, softwareVersion);
    return softwareVersion;
}

Date_t GetSoftwareUpdateDate()
{
    Date_t softwareUpdateDate;
    EEPROM.get(EEPROM_DAY_ADDRESS, softwareUpdateDate);
    return softwareUpdateDate;
}

uint16_t GetErrorCount()
{
    uint16_t errorCount;
    EEPROM.get(EEPROM_ERROR_COUNT_ADDRESS, errorCount);
    return errorCount;
}

Error_t GetError(uint16_t errorNo)
{
    Error_t error;
    uint16_t errorIndex = errorNo - 1;
    EEPROM.get(EEPROM_ERROR_SECTOR_START_ADDRESS + (sizeof(Error_t)*errorIndex), error);
    return error;
}

void ErrorHistoryClear()
{
    EepromHeader_t eepromHeader;
    const char companyName[SIZE_OF_COMPANY_NAME + 1] = COMPANY_NAME;
    memcpy(&eepromHeader.companyName.character[0], companyName, SIZE_OF_COMPANY_NAME);
    eepromHeader.version.major = SOFTWARE_VERSION_MAJOR;
    eepromHeader.version.minor = SOFTWARE_VERSION_MINOR;
    eepromHeader.version.patch = SOFTWARE_VERSION_PATCH;
    eepromHeader.date.day = SOFTWARE_UPDATE_DATE_DAY;
    eepromHeader.date.month = SOFTWARE_UPDATE_DATE_MONTH;
    eepromHeader.date.year = SOFTWARE_UPDATE_DATE_YEAR;  
    eepromHeader.errorCount = 0;
    EEPROM.put(EEPROM_START_ADDRESS, eepromHeader);
    for (uint16_t i = sizeof(eepromHeader); i < EEPROM.length(); i++)
    {
        EEPROM.put(i, 255);
    }
}

void SetCRC32()
{
    if (!CRC32ErrorFlag)
    {
        uint32_t readCrc32;
        EEPROM.get(EEPROM_CRC32_ADDRESS, readCrc32);
        readCrc32 = readCrc32 + 1;
        if (readCrc32 > CRC32_VALUE)
        {
            digitalWrite(BRUSHES_SPEED_CONTROL_PIN     , LOW);
            digitalWrite(BRUSHES_FORWARD_TURN_PIN      , LOW);
            digitalWrite(BRUSHES_STOP_PIN              , LOW);
            digitalWrite(BRUSHES_REVERSE_TURN_PIN      , LOW);
            digitalWrite(WATER_ON_OFF_PIN              , LOW);
            digitalWrite(TRACK_LEFT_SPEED_CONTROL_PIN , LOW);
            digitalWrite(TRACK_LEFT_FORWARD_TURN_PIN  , LOW);
            digitalWrite(TRACK_LEFT_STOP_PIN          , LOW);
            digitalWrite(TRACK_LEFT_REVERSE_TURN_PIN  , LOW);
            digitalWrite(TRACK_RIGHT_SPEED_CONTROL_PIN, LOW);
            digitalWrite(TRACK_RIGHT_FORWARD_TURN_PIN , LOW);
            digitalWrite(TRACK_RIGHT_STOP_PIN         , LOW);
            digitalWrite(TRACK_RIGHT_REVERSE_TURN_PIN , LOW);
            CRC32ErrorFlag = true;
        }
        EEPROM.put(EEPROM_CRC32_ADDRESS, readCrc32);
    }
}

uint32_t GetCRC32()
{
    uint32_t crc32;
    EEPROM.get(EEPROM_CRC32_ADDRESS, crc32);
    return crc32;
}