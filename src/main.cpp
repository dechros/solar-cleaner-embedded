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

#define FORWARD  		(0)
#define BACKWARD 		(1)

#define RAMP_UP_SPEED  	(1)
#define RAMP_DOWN_SPEED (1)

#define REMOTE_CONTROL_MESSAGE_ID_1 	(0x1E4)
#define REMOTE_CONTROL_MESSAGE_ID_2 	(0x2E4)

typedef struct 
{
	uint8_t targetWay;
	uint8_t targetSpeed;
	uint8_t currentWay;
	int16_t currentSpeed;
}MotorRotationInfo_t;

MotorRotationInfo_t rightPalletMotorInfo;
MotorRotationInfo_t leftPalletMotorInfo;
MotorRotationInfo_t brushesMotorInfo;

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
		struct
		{
			uint8_t startFirstSpeed	  :1;
			uint8_t startSecondSpeed  :1;
			uint8_t stopRelaysOpened  :1;
			uint8_t lowBattery		  :1;
			uint8_t tiltSwitch		  :1;
			uint8_t outOfRange		  :1;
			uint8_t 				  :1;
			uint8_t stopRelaysClosed  :1;
		}byte1;
		struct
		{
			uint8_t paddleANorth		:1;
			uint8_t paddleASouth		:1;
			uint8_t paddleBNorth		:1;
			uint8_t paddleBSouth		:1;
			uint8_t potantiometerActive :1;
			uint8_t 					:1;
			uint8_t paddleDNorth		:1;
			uint8_t paddleDSouth		:1;
		}byte2;
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
		struct
		{
			uint8_t bit1	:1;
			uint8_t bit2	:1;
			uint8_t 		:6;
		}byte4;
		struct
		{
			uint8_t paddleAValue;
		}byte5;
		struct 
		{
			uint8_t paddleBValue;
		}byte6;
		struct 
		{
			uint8_t potantiometerValue;
		}byte7;
		struct 
		{
			uint8_t paddleDValue;
		}byte8;
	};
}Message_t;
Message_t canmsg;

void setup()
{
	/*
	InitializePins();
	InitializeMotors();
	InitializeEEPROM();
	WaterPumpOff();
	SerialBegin(9600);
	*/
	InitializePins();
	Serial.begin(115200);
	memset(&canmsg, 0, sizeof(Message_t));
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

	/* 50ms timer interrupt for motor speed control */
	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = 12499;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);  
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

uint8_t cycleCounter = 0;

void loop()
{
	if (!CRC32ErrorFlag)
	{
		if (CANMessageReceived)
		{
			uint8_t messageLength = 0;
			uint32_t canMessageId = 0;
			if (CAN.readMsgBuf(&canMessageId, &messageLength, canmsg.data) == CAN_OK)
			{
				if (canMessageId == 0x7FF)
				{
					rightPalletMotorInfo.targetSpeed = canmsg.data[0] / 3;
					rightPalletMotorInfo.targetWay = canmsg.data[1];
				}
				else if (canMessageId == REMOTE_CONTROL_MESSAGE_ID_1)
				{
					Serial.println("Message 1 Came!");
				}
				else if (canMessageId == REMOTE_CONTROL_MESSAGE_ID_2)
				{
					Serial.println("Message 2 Came!");
				}
			}
		}

		if (send == true)
		{
			if (CRC32ErrorFlag)
			{
				Serial.println("Status: ERROR!");
				delay(1000);
			}
			
			cycleCounter++;
			if (cycleCounter == 20)
			{
				cycleCounter = 0;
				SetCRC32();
			}
			
			//CAN.sendMsgBuf(0x201, 8, array);
			if (rightPalletMotorInfo.targetWay == rightPalletMotorInfo.currentWay)
			{
				if (rightPalletMotorInfo.targetSpeed > rightPalletMotorInfo.currentSpeed)
				{
					rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed + RAMP_UP_SPEED;
					if (rightPalletMotorInfo.currentSpeed > rightPalletMotorInfo.targetSpeed)
					{
						rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				}
				else if (rightPalletMotorInfo.targetSpeed < rightPalletMotorInfo.currentSpeed)
				{
					rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
					if (rightPalletMotorInfo.currentSpeed < rightPalletMotorInfo.targetSpeed)
					{
						rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				}
			}
			else
			{
				rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
				if (rightPalletMotorInfo.currentSpeed < 0)
				{
					rightPalletMotorInfo.currentSpeed = 0;
				}
				analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				if (rightPalletMotorInfo.currentSpeed == 0)
				{
					if (rightPalletMotorInfo.targetWay == FORWARD)
					{
						digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
						digitalWrite(BRUSHES_SPEED_CONTROL_PIN, LOW);
					}
					else
					{
						digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
						digitalWrite(BRUSHES_SPEED_CONTROL_PIN, HIGH);
					}
					rightPalletMotorInfo.currentWay = rightPalletMotorInfo.targetWay;
				}
			}
			send = false;
		}
	}
	
#if 0
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
#endif
}

ISR(TIMER1_COMPA_vect)
{
	send = true;
}

#endif