#ifndef _CAN_OPERATIONS_H_
#define _CAN_OPERATIONS_H_

#include <Arduino.h>
#include "motor.h"

#define SPI_CS_PIN  53
#define JOYSTICK_DEAD_ZONE_CAN_REMOTE	(5)
#define REMOTE_CONTROL_MESSAGE_ID_1 	(0x1E4)
#define REMOTE_CONTROL_MESSAGE_ID_2 	(0x2E4)

void InitCANBus(void);
void CheckCANMessage(void);
void RemoteStart(void);

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

#endif /* _CAN_OPERATIONS_H_ */