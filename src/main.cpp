#if 0
#include <UIPEthernet.h>

EthernetServer server = EthernetServer(1234);

void setup()
{
	Serial.begin(9600);
	uint8_t myMac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	Ethernet.begin(myMac, IPAddress(192, 168, 1, 125));
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
		}
		Serial.println("Client Disconnected");
	}
}
#endif

#if 1

#include "globals.h"
#include "pinOperations.h"
#include "motorDriver.h"
#include "waterPumpDriver.h"
#include "eepromOperations.h"
#include "mcp_can.h"

#define SPI_CS_PIN  53

uint8_t value = 0;
bool send = false;
bool increment = true;
uint8_t array[8] = {0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x23};

MCP_CAN CAN(SPI_CS_PIN); 

typedef union
{
	uint8_t data[8];
	struct
	{
		union
		{
			uint8_t byte1TotalValue;
			struct 
			{
				uint8_t startFirstSpeed:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte1;
		};
		union
		{
			uint8_t byte2TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte2;
		};
		union
		{
			uint8_t byte3TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte3;
		};
		union
		{
			uint8_t byte4TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte4;
		};
		union
		{
			uint8_t byte5TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte5;
		};
		union
		{
			uint8_t byte6TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte6;
		};
		union
		{
			uint8_t byte7TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte7;
		};
		union
		{
			uint8_t byte8TotalValue;
			struct 
			{
				uint8_t bit1:1;
				uint8_t bit2:1;
				uint8_t bit3:1;
				uint8_t bit4:1;
				uint8_t bit5:1;
				uint8_t bit6:1;
				uint8_t bit7:1;
				uint8_t bit8:1;
			}byte8;
		};
	};
}Message_t;

void setup()
{
	InitializePins();
	Serial.begin(115200);
	while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ))
	{
		delay(100);
		Serial.println("CAN BUS Shield init fail");
		Serial.println(" Init CAN BUS Shield again");	
	}
	CAN.setMode(MCP_NORMAL);

	analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, 0);
	digitalWrite(BRUSHES_SPEED_CONTROL_PIN, LOW);
	digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);

	digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
	analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, 0);

	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = 12499;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);  
	TIMSK1 |= (1 << OCIE1A);
	sei();

	/*
	InitializePins();
	InitializeMotors();
	InitializeEEPROM();
	WaterPumpOff();
	SerialBegin(9600);
	*/
}
#define FORWARD  		(0)
#define BACKWARD 		(1)

#define RAMP_UP_SPEED  	(1)
#define RAMP_DOWN_SPEED (1)

#define REMOTE_CONTROL_MESSAGE_ID_1 	(0x1E4)
#define REMOTE_CONTROL_MESSAGE_ID_2 	(0x2E4)

uint8_t targetWay = FORWARD;
uint8_t targetSpeed = 0;
uint8_t currentWay = FORWARD;
int16_t currentSpeed = 0;
void loop()
{
	if (CANMessageReceived)
	{
		byte readArray[8] = {0};
		uint8_t messageLength = 0;
		uint32_t canMessageId = 0;
		if (CAN.readMsgBuf(&canMessageId, &messageLength, readArray) == CAN_OK)
		{
			if (canMessageId == 0x7FF)
			{
				targetSpeed = readArray[0] / 3;
				targetWay = readArray[1];
			}
			else if (canMessageId == REMOTE_CONTROL_MESSAGE_ID_1)
			{
				/* code */
			}
			
		}
	}

	if (send == true)
	{
		//CAN.sendMsgBuf(0x201, 8, array);
		if (targetWay == currentWay)
		{
			if (targetSpeed > currentSpeed)
			{
				currentSpeed = currentSpeed + RAMP_UP_SPEED;
				if (currentSpeed > targetSpeed)
				{
					currentSpeed = targetSpeed;
				}
				analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)currentSpeed);
			}
			else if (targetSpeed < currentSpeed)
			{
				currentSpeed = currentSpeed - RAMP_DOWN_SPEED;
				if (currentSpeed < targetSpeed)
				{
					currentSpeed = targetSpeed;
				}
				analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)currentSpeed);
			}
		}
		else
		{
			currentSpeed = currentSpeed - RAMP_DOWN_SPEED;
			if (currentSpeed < 0)
			{
				currentSpeed = 0;
			}
			analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)currentSpeed);
			if (currentSpeed == 0)
			{
				if (targetWay == FORWARD)
				{
					digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
					digitalWrite(BRUSHES_SPEED_CONTROL_PIN, LOW);
				}
				else
				{
					digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
					digitalWrite(BRUSHES_SPEED_CONTROL_PIN, HIGH);
				}
				currentWay = targetWay;
			}
		}
		send = false;
	}
	
	/*
	int size = Serial.available();
	if (size > 0)
	{
		uint8_t readByte = Serial.read();
		Serial.println(readByte);
		if (readByte == 'A')
		{
			digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
		}
		else if (readByte == 'B')
		{
			digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
		}
		else if (readByte == 'C')
		{
			digitalWrite(BRUSHES_REVERSE_TURN_PIN, LOW);
		}
		else if (readByte == 'D')
		{
			digitalWrite(BRUSHES_REVERSE_TURN_PIN, HIGH);
		}
		else if (readByte == 'E')
		{
			digitalWrite(BRUSHES_STOP_PIN, LOW);
		}
		else if (readByte == 'F')
		{
			digitalWrite(BRUSHES_STOP_PIN, HIGH);
		}
		else if (readByte == 'G')
		{
			digitalWrite(BRUSHES_SPEED_CONTROL_PIN, HIGH);
		}
		else if (readByte == 'H')
		{
			analogWrite(BRUSHES_SPEED_CONTROL_PIN, 127);
		}
		else if (readByte == 'I')
		{
			digitalWrite(BRUSHES_SPEED_CONTROL_PIN, LOW);
		}
	}
	*/
}

ISR(TIMER1_COMPA_vect)
{
	send = true;
}

#endif