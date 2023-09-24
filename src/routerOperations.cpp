#include "routerOperations.h"

uint8_t bufferClearCounter = 0;

void InitRouterCommunication()
{
    ROUTER_SERIAL.begin(9600);
}

static bool ControlChecksum(uint8_t *dataPointer, uint8_t size)
{
	uint8_t checksum = 0;
	for (uint8_t i = 0; i < size - 1; i++)
	{
		checksum ^= dataPointer[i];
	}
	checksum ^= 255;
    return (dataPointer[size - 1] == checksum);
}

static void ParseTCPMessage(TCPMessage_t readTCPMessage)
{
    if (readTCPMessage.joystickX)
    {
        /* code */
    }
    
}

void CheckTCPMessage()
{
    int tcpBufferSize = ROUTER_SERIAL.available();
    if (tcpBufferSize > 0)
    {
        bufferClearCounter = 0;
        int readData = ROUTER_SERIAL.peek();
        if (readData == TCP_MESSAGE_FIRST_BYTE && tcpBufferSize >= sizeof(TCPMessage_t))
        {
            TCPMessage_t readTCPMessage;
            ROUTER_SERIAL.readBytes((uint8_t*)&readTCPMessage, sizeof(TCPMessage_t));
            /* Check the "TCP Message Header" */
            if (strncmp((const char*)&readTCPMessage, TCP_MESSAGE_HEADER, 3) == 0)
            {
                /* Control the checksum of message */
                bool result = ControlChecksum((uint8_t*)&readTCPMessage, sizeof(TCPMessage_t));
                if (result == true)
                {
                    ParseTCPMessage(readTCPMessage);
                    /* Send ACK to router */
                    ROUTER_SERIAL.write(ACK_MESSAGE, 3);
                }
                else
                {
                    Serial.println("Checksum Error!");
                }
            }
            else
            {
                ROUTER_SERIAL.read();
                Serial.println("Unkown Header!");
            }
        }
        else if (readData == 'T' && tcpBufferSize < sizeof(TCPMessage_t))
        {
            delay(10);
            bufferClearCounter++;
            if (bufferClearCounter == 5)
            {
                bufferClearCounter = 0;
                ROUTER_SERIAL.read();
                Serial.println("Half Message! Discarded!");
            }
        }
        else
        {
            ROUTER_SERIAL.read();
            Serial.println("Unkown Header!");
        }
    }
}

