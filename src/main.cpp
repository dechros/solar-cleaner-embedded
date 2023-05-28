#if 1
#include <UIPEthernet.h>

EthernetServer server = EthernetServer(4567);

void setup()
{
    Serial.begin(9600);
    uint8_t mac[6] = {0x01, 0x04, 0x07, 0x05, 0x04, 0x05};
    IPAddress myIP(192, 168, 1, 125);
    Ethernet.begin(mac, myIP);
    server.begin();
}

void loop()
{
	int size = 0;
	EthernetClient client = server.accept();
	if (client.connected())
	{
		Serial.println("Client Connected");
		while (client.connected())
		{
			size = Serial.available();
			if (size > 0)
			{
				uint8_t *msg = (uint8_t *)malloc(size);
				for (int i = 0; i < size; i++)
				{
					msg[i] = Serial.read();
				}
				client.write(msg, size);
				free(msg);
			}
			size = client.available();
			if (size > 0)
			{
				uint8_t *msg = (uint8_t *)malloc(size);
				client.read(msg, size);
				Serial.write(msg, size);
				free(msg);
			}
            delay(100);
		}
		Serial.println("Client Disconnected");
	}
    delay(100);
}

#endif

#if 0
#include <UIPEthernet.h>

EthernetClient client;
signed long next;

void setup()
{
    Serial.begin(9600);

    Serial.print("Before Begin\n");
    uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

    Ethernet.begin(mac);
    Serial.print("After Begin\n");

    Serial.print("localIP: ");
    Serial.println(Ethernet.localIP());
    Serial.print("subnetMask: ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("gatewayIP: ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("dnsServerIP: ");
    Serial.println(Ethernet.dnsServerIP());

    while (true)
    {
        if (client.connect(IPAddress(192, 168, 1, 50), 1234))
        {
            break;
        }
        else
        {
            Serial.println("Client connect failed");
            delay(2000);
        }
    }
    client.println("Connected to Server\n");
}
void loop()
{
    int size = Serial.available();
    if (size > 0)
    {
        uint8_t *msg = (uint8_t *)malloc(size);
        for (int i = 0; i < size; i++)
        {
            msg[i] = Serial.read();
        }
        client.write(msg, size);
        free(msg);
    }
    size = 0;
    size = client.available();
    if (size > 0)
    {
        uint8_t *msg = (uint8_t *)malloc(size);
        size = client.read(msg, size);
        Serial.write(msg, size);
        free(msg);
    }
}

#endif

#if 0

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

#endif