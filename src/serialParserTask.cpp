#include "serialParserTask.h"

static uint8_t calculateChecksum(uint8_t* dataptr, uint8_t size);
static bool checksumControl(uint8_t* dataptr, uint8_t size);
static void SendResponse(uint8_t charCode, uint8_t messageId, ResponseType_t responseType);
static void ProcessMessage(SerialMessage_t message);

void vSerialParserTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);

    int queueSize = 0;
    uint8_t discardCounter = 0;
    while (true)
    {
        queueSize = SerialAvailable();
        if (queueSize > 0)
        {
            if (queueSize >= MESSAGE_SIZE)
            {
                discardCounter = 0;
                SerialMessage_t message;
                message.header = SerialRead();
                if (message.header == MESSAGE_DEBUG_HEADER || message.header == MESSAGE_ESP32_HEADER)
                {
                    for (int i = 1; i < MESSAGE_SIZE; i++)
                    {
                        message.dataArray[i] = SerialRead();
                    }
                    ProcessMessage(message);
                }
                else
                {
                    SerialPrintln("Serial unkown header!");
                }
            }
            else
            {
                discardCounter = discardCounter + 1;
                if (discardCounter == 20)
                {
                    discardCounter = 0;
                    SerialReceiveBufferClear();
                }
            }
        }
        vTaskDelay(1);
    }
}

static void ProcessMessage(SerialMessage_t message)
{
    bool result = checksumControl(&(message.dataArray[0]), MESSAGE_SIZE);
    if (result == true)
    {
        switch (message.charCode)
        {
            case MESSAGE_RIGHT_PALLET_CHAR_CODE:
            {
                switch (message.messageId)
                {
                    case MOTOR_START_STOP:
                    {
                        MotorStartStop(RIGHT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_SPEED_CHANGE:
                    {
                        MotorSpeedChange(RIGHT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_ROTATION_CHANGE:
                    {
                        MotorRotationChange(RIGHT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    default:
                    {
                        SendResponse(message.charCode, message.messageId, ERROR_ACKNOWLEDGE);
                        break;
                    }
                }
                break;
            }
            case MESSAGE_LEFT_PALLET_CHAR_CODE:
            {
                switch (message.messageId)
                {
                    case MOTOR_START_STOP:
                    {
                        MotorStartStop(LEFT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_SPEED_CHANGE:
                    {
                        MotorSpeedChange(LEFT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_ROTATION_CHANGE:
                    {
                        MotorRotationChange(LEFT_PALLET, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    default:
                    {
                        SendResponse(message.charCode, message.messageId, ERROR_ACKNOWLEDGE);
                        break;
                    }
                }
                break;
            }
            case MESSAGE_BRUSHES_PALLET_CHAR_CODE:
            {
                switch (message.messageId)
                {
                    case MOTOR_START_STOP:
                    {
                        MotorStartStop(BRUSHES, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_SPEED_CHANGE:
                    {
                        MotorSpeedChange(BRUSHES, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    case MOTOR_ROTATION_CHANGE:
                    {
                        MotorRotationChange(BRUSHES, message.data[0]);
                        SendResponse(message.charCode, message.messageId, ACKNOWLEDGE);
                        break;
                    }
                    default:
                    {
                        SendResponse(message.charCode, message.messageId, ERROR_ACKNOWLEDGE);
                        break;
                    }
                }
                break;
            }
            case MESSAGE_EEPROM_CHAR_CODE:
            {
                break;
            }
            case MESSAGE_ERROR_CHAR_CODE:
            {
                break;
            }
            case MESSAGE_GENERAL_CHAR_CODE:
            {
                break;
            }
            default:
            {
                SendResponse(message.charCode, message.messageId, ERROR_ACKNOWLEDGE);
                break;
            }
        }
    }
    else
    {
        SendResponse(message.charCode, message.messageId, NEGATIVE_ACKNOWLEDGE);
    }
}

static void SendResponse(uint8_t charCode, uint8_t messageId, ResponseType_t responseType)
{
    SerialResponse_t response;
    response.charCode = charCode;
    response.messageId = messageId;
    switch (responseType)
    {
        case ACKNOWLEDGE:
        {
            response.data[0] = 'A';
            response.data[1] = 'C';
            response.data[2] = 'K';
            response.checksum = calculateChecksum(&(response.dataArray[0]), RESPONSE_SIZE);
            SerialWrite((const char*)&(response.dataArray[0]), RESPONSE_SIZE);
            break;
        }
        case NEGATIVE_ACKNOWLEDGE:
        {
            response.data[0] = 'N';
            response.data[1] = 'A';
            response.data[2] = 'K';
            response.checksum = calculateChecksum(&(response.dataArray[0]), RESPONSE_SIZE);
            SerialWrite((const char*)&(response.dataArray[0]), RESPONSE_SIZE);
            break;
        }
        case ERROR_ACKNOWLEDGE:
        {
            response.data[0] = 'E';
            response.data[1] = 'R';
            response.data[2] = 'R';
            response.checksum = calculateChecksum(&(response.dataArray[0]), RESPONSE_SIZE);
            SerialWrite((const char*)&(response.dataArray[0]), RESPONSE_SIZE);
            break;
        }
        case SOFTWARE_VERSION:
        {
            break;
        }
        case SOFTWARE_UPDATE_DATE:
        {
            break;
        }
        case ERROR_COUNT:
        {
            break;
        }
        case DETAILED_ERROR_DATA:
        {
            break;
        }
        case DOWNLOAD_ERRORS:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

static bool checksumControl(uint8_t* dataptr, uint8_t size)
{
    uint8_t checksum = calculateChecksum(dataptr, size);
    if (checksum == dataptr[size - 1])
    {
        return true;
    }
    else
    {
        return false;
    }
}

static uint8_t calculateChecksum(uint8_t* dataptr, uint8_t size)
{
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < size - 1; i++)
    {
        checksum ^= dataptr[i];
    }
    checksum = ~checksum;
    checksum = (checksum + 127) % 256;
    return checksum;
}