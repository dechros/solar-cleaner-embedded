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

#define REMOTE_CONTROL_MESSAGE_ID_1 	(0x1E4)
#define REMOTE_CONTROL_MESSAGE_ID_2 	(0x2E4)

/* TODO: SAG SOL PALET ICIN IKIYE BOL */
#define RPM_MAX_SPEED   		(5)
#define JOYSTICK_DEAD_ZONE 		(5)
#define MIN_MOTOR_SPEED			(10)
#define MAX_MOTOR_SPEED			(85)
/**************************************/

#define GSM_UPDATE_INTERVAL  		(100)
#define ACCELERATION_SENSOR_FILTER  (5)
#define ERROR_CHECK_INTERVAL 		(5)

uint8_t RAMP_UP_SPEED = 5;
uint8_t RAMP_DOWN_SPEED = 5;

uint8_t RPM_SPEED = 3; /* 1 - 5 */
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
uint32_t cycleCounter = 0;
bool send = false;
bool increment = true;
uint8_t increValue = 0;
uint32_t previousCanIncrementCounter = 0;
uint32_t canIncrementCounter = 0;
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

	rightPalletMotorInfo.currentSpeed = 0;
	rightPalletMotorInfo.currentWay = REVERSE;
	rightPalletMotorInfo.targetWay = REVERSE;
	rightPalletMotorInfo.targetSpeed = 0;

	leftPalletMotorInfo.currentSpeed = 0;
	leftPalletMotorInfo.currentWay = FORWARD;
	leftPalletMotorInfo.targetWay = FORWARD;
	leftPalletMotorInfo.targetSpeed = 0;

	brushesMotorInfo.currentSpeed = 0;
	brushesMotorInfo.currentWay = FORWARD;
	brushesMotorInfo.targetWay = FORWARD;
	brushesMotorInfo.targetSpeed = 0;

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

void loop()
{
	//if (!CRC32ErrorFlag)
	{
		if (CANMessageReceived)
		{
			canIncrementCounter++;
			uint8_t messageLength = 0;
			uint32_t canMessageId = 0;

			if (CAN.readMsgBuf(&canMessageId, &messageLength, canmsg.data) == CAN_OK)
			{
				if (canMessageId == REMOTE_CONTROL_MESSAGE_ID_1)
				{
					static uint8_t previousRampUpValue = 0;
					static uint8_t previousRampDownValue = 0;
					if (canmsg.byte4.bit1 == 1 && previousRampUpValue == 0)
					{
						RPM_SPEED++;
						if (RPM_SPEED > 5)
						{
							RPM_SPEED = 5;
						}
						Serial.println(RPM_SPEED);
					}
					else if (canmsg.byte4.bit2 == 1 && previousRampDownValue == 0)
					{
						RPM_SPEED--;
						if (RPM_SPEED < 1)
						{
							RPM_SPEED = 1;
						}
						Serial.println(RPM_SPEED);
					}
					previousRampUpValue = canmsg.byte4.bit1;
					previousRampDownValue = canmsg.byte4.bit2;
					
					if (canmsg.byte8.paddleDValue > JOYSTICK_DEAD_ZONE)
					{
						rightPalletMotorInfo.targetSpeed = (canmsg.byte8.paddleDValue / 3) * ((double)RPM_SPEED / RPM_MAX_SPEED);
						Serial.println(rightPalletMotorInfo.targetSpeed);
					}
					else
					{
						rightPalletMotorInfo.targetSpeed = 0;
					}
					
					if (canmsg.byte2.paddleDNorth == 1 && canmsg.byte2.paddleDSouth == 0)
					{
						rightPalletMotorInfo.targetWay = FORWARD;
					}
					else if (canmsg.byte2.paddleDNorth == 0 && canmsg.byte2.paddleDSouth == 1)
					{
						rightPalletMotorInfo.targetWay = BACKWARD;
					}

					if (canmsg.byte5.paddleAValue > JOYSTICK_DEAD_ZONE)
					{
						leftPalletMotorInfo.targetSpeed = (canmsg.byte5.paddleAValue / 3) * ((double)RPM_SPEED / RPM_MAX_SPEED);
					}
					else
					{
						leftPalletMotorInfo.targetSpeed = 0;
					}
					
					if (canmsg.byte2.paddleANorth == 1 && canmsg.byte2.paddleASouth == 0)
					{
						leftPalletMotorInfo.targetWay = FORWARD;
					}
					else if (canmsg.byte2.paddleANorth == 0 && canmsg.byte2.paddleASouth == 1)
					{
						leftPalletMotorInfo.targetWay = BACKWARD;
					}

					if (canmsg.byte7.potantiometerValue > JOYSTICK_DEAD_ZONE)
					{
						brushesMotorInfo.targetSpeed = canmsg.byte7.potantiometerValue;
					}
					else
					{
						brushesMotorInfo.targetSpeed = 0;
					}
					
					if (canmsg.byte3.bit6 == 0 && canmsg.byte3.bit5 == 1)
					{
						brushesMotorInfo.targetWay = FORWARD;
					}
					else if (canmsg.byte3.bit6 == 1 && canmsg.byte3.bit5 == 0)
					{
						brushesMotorInfo.targetWay = BACKWARD;
					}

					if (canmsg.byte3.bit7 == 1)
					{
						//Serial.println("Bit 7 on");
						WaterPumpOn();
					}
					else if (canmsg.byte3.bit7 == 0)
					{
						//Serial.println("Bit 7 off");
						WaterPumpOff();
					}
				}
			}
		}

		if (send == true)
		{
			/*
			if (CRC32ErrorFlag)
			{
				Serial.println("Status: ERROR!");
				delay(1000);
			}*/
			
			cycleCounter++;
			if (/* condition */)
			{
				/* code */
			}
			
			if (cycleCounter == 20)
			{	
				uint8_t startRemote = 1;
				CAN.sendMsgBuf(0x0, 1, &startRemote);
				//SetCRC32();
			}
			
			CAN.sendMsgBuf(0x201, 8, array);

			/*RIGHT*/
			if (rightPalletMotorInfo.targetWay == rightPalletMotorInfo.currentWay)
			{
				if (rightPalletMotorInfo.targetSpeed > rightPalletMotorInfo.currentSpeed)
				{
					rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed + RAMP_UP_SPEED;
					if (rightPalletMotorInfo.currentSpeed > rightPalletMotorInfo.targetSpeed)
					{
						rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				}
				else if (rightPalletMotorInfo.targetSpeed < rightPalletMotorInfo.currentSpeed)
				{
					rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
					if (rightPalletMotorInfo.currentSpeed < rightPalletMotorInfo.targetSpeed)
					{
						rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				}
			}
			else
			{
				rightPalletMotorInfo.currentSpeed = rightPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
				if (rightPalletMotorInfo.currentSpeed < 0)
				{
					rightPalletMotorInfo.currentSpeed = 0;
				}
				analogWrite(PALLET_RIGHT_SPEED_CONTROL_PIN, (uint8_t)rightPalletMotorInfo.currentSpeed);
				if (rightPalletMotorInfo.currentSpeed == 0)
				{
					if (rightPalletMotorInfo.targetWay == FORWARD)
					{
						digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, LOW);
						digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, HIGH);
					}
					else
					{
						digitalWrite(PALLET_RIGHT_FORWARD_TURN_PIN, HIGH);
						digitalWrite(PALLET_RIGHT_REVERSE_TURN_PIN, LOW);
					}
					rightPalletMotorInfo.currentWay = rightPalletMotorInfo.targetWay;
				}
			}
			
			/*LEFT*/
			if (leftPalletMotorInfo.targetWay == leftPalletMotorInfo.currentWay)
			{
				if (leftPalletMotorInfo.targetSpeed > leftPalletMotorInfo.currentSpeed)
				{
					leftPalletMotorInfo.currentSpeed = leftPalletMotorInfo.currentSpeed + RAMP_UP_SPEED;
					if (leftPalletMotorInfo.currentSpeed > leftPalletMotorInfo.targetSpeed)
					{
						leftPalletMotorInfo.currentSpeed = leftPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)leftPalletMotorInfo.currentSpeed);
				}
				else if (leftPalletMotorInfo.targetSpeed < leftPalletMotorInfo.currentSpeed)
				{
					leftPalletMotorInfo.currentSpeed = leftPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
					if (leftPalletMotorInfo.currentSpeed < leftPalletMotorInfo.targetSpeed)
					{
						leftPalletMotorInfo.currentSpeed = leftPalletMotorInfo.targetSpeed;
					}
					analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)leftPalletMotorInfo.currentSpeed);
				}
			}
			else
			{
				leftPalletMotorInfo.currentSpeed = leftPalletMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
				if (leftPalletMotorInfo.currentSpeed < 0)
				{
					leftPalletMotorInfo.currentSpeed = 0;
				}
				analogWrite(PALLET_LEFT_SPEED_CONTROL_PIN, (uint8_t)leftPalletMotorInfo.currentSpeed);
				if (leftPalletMotorInfo.currentSpeed == 0)
				{
					if (leftPalletMotorInfo.targetWay == FORWARD)
					{
						digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, LOW);
						digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, HIGH);
					}
					else
					{
						digitalWrite(PALLET_LEFT_REVERSE_TURN_PIN, HIGH);
						digitalWrite(PALLET_LEFT_FORWARD_TURN_PIN, LOW);
					}
					leftPalletMotorInfo.currentWay = leftPalletMotorInfo.targetWay;
				}
			}
			
			/*BRUSH*/
			if (brushesMotorInfo.targetWay == brushesMotorInfo.currentWay)
			{
				if (brushesMotorInfo.targetSpeed > brushesMotorInfo.currentSpeed)
				{
					brushesMotorInfo.currentSpeed = brushesMotorInfo.currentSpeed + RAMP_UP_SPEED;
					if (brushesMotorInfo.currentSpeed > brushesMotorInfo.targetSpeed)
					{
						brushesMotorInfo.currentSpeed = brushesMotorInfo.targetSpeed;
					}
					analogWrite(BRUSHES_SPEED_CONTROL_PIN, (uint8_t)brushesMotorInfo.currentSpeed);
				}
				else if (brushesMotorInfo.targetSpeed < brushesMotorInfo.currentSpeed)
				{
					brushesMotorInfo.currentSpeed = brushesMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
					if (brushesMotorInfo.currentSpeed < brushesMotorInfo.targetSpeed)
					{
						brushesMotorInfo.currentSpeed = brushesMotorInfo.targetSpeed;
					}
					analogWrite(BRUSHES_SPEED_CONTROL_PIN, (uint8_t)brushesMotorInfo.currentSpeed);
				}
			}
			else
			{
				brushesMotorInfo.currentSpeed = brushesMotorInfo.currentSpeed - RAMP_DOWN_SPEED;
				if (brushesMotorInfo.currentSpeed < 0)
				{
					brushesMotorInfo.currentSpeed = 0;
				}
				analogWrite(BRUSHES_SPEED_CONTROL_PIN, (uint8_t)brushesMotorInfo.currentSpeed);
				if (brushesMotorInfo.currentSpeed == 0)
				{
					if (brushesMotorInfo.targetWay == FORWARD)
					{
						digitalWrite(BRUSHES_REVERSE_TURN_PIN, LOW);
						digitalWrite(BRUSHES_FORWARD_TURN_PIN, HIGH);
					}
					else
					{
						digitalWrite(BRUSHES_REVERSE_TURN_PIN, HIGH);
						digitalWrite(BRUSHES_FORWARD_TURN_PIN, LOW);
					}
					brushesMotorInfo.currentWay = brushesMotorInfo.targetWay;
				}
			}
			send = false;
		}
	}
	
#if 1
	int size = Serial.available();
	if (size > 1)
	{
		uint8_t readByte = Serial.read();
		if (readByte == 'U')
		{
			readByte = Serial.read();
			RAMP_UP_SPEED = readByte;
		}
		else if (readByte == 'D')
		{
			readByte = Serial.read();
			RAMP_UP_SPEED = readByte;
		}
	}
#endif
}

ISR(TIMER1_COMPA_vect)
{
	send = true;
}

#endif