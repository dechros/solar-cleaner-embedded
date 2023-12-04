#ifndef _CAN_OPERATIONS_H_
#define _CAN_OPERATIONS_H_

#include <Arduino.h>
#include "motor.h"

#define SPI_CS_PIN 53
#define JOYSTICK_DEAD_ZONE_CAN_REMOTE (5)
#define REMOTE_CONTROL_MESSAGE_ID_1 (0x1E4)
#define REMOTE_CONTROL_MESSAGE_ID_2 (0x2E4)

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
			uint8_t rcStartButton : 1;
			uint8_t bit2 : 1;
			uint8_t rcStopRelaysOpened : 1;
			uint8_t rcLowBattery : 1;
			uint8_t bit5 : 1;
			uint8_t bit6 : 1;
			uint8_t bit7 : 1;
			uint8_t rcStopRelaysClosed : 1;
		} byte1;
		struct
		{
			uint8_t paddleANorth : 1;
			uint8_t paddleASouth : 1;
			uint8_t bit3 : 1;
			uint8_t bit4 : 1;
			uint8_t potActive : 1;
			uint8_t bit6 : 1;
			uint8_t paddleDNorth : 1;
			uint8_t paddleDSouth : 1;
		} byte2;
		struct
		{
			uint8_t bit1 : 1;
			uint8_t waterPumpButton : 1;
			uint8_t frontBrushCW : 1;
			uint8_t frontBrushCCW : 1;
			uint8_t sensorActive : 1;
			uint8_t speedButton1 : 1;
			uint8_t speedButton2 : 1;
			uint8_t lamp : 1;
		} byte3;
		struct
		{
			uint8_t rearBrushCW : 1;
			uint8_t rearBrushCCW : 1;
			uint8_t bit3 : 1;
			uint8_t bit4 : 1;
			uint8_t bit5 : 1;
			uint8_t bit6 : 1;
			uint8_t bit7 : 1;
			uint8_t bit8 : 1;
		} byte4;
		struct
		{
			uint8_t paddleAValue;
		} byte5;
		struct
		{
			uint8_t emptyByte;
		} byte6;
		struct
		{
			uint8_t potantiometerValue;
		} byte7;
		struct
		{
			uint8_t paddleDValue;
		} byte8;
	};
} Message_t;

#endif /* _CAN_OPERATIONS_H_ */