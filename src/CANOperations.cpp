#include "CANOperations.h"
#include "globals.h"
#include "pinOperations.h"
#include "waterPump.h"
#include "mcp_can.h"

#define RPM_MAX_SPEED (5)
#define RPM_MIN_SPEED (1)

MCP_CAN CAN(SPI_CS_PIN);
Message_t CANMessage;
uint8_t RPM_SPEED = 3; /* 1 - 5 */

uint16_t analogValues[4];

void InitCANBus()
{
	memset(&CANMessage, 0, sizeof(Message_t));
	while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ))
	{
		delay(100);
	}
	CAN.setMode(MCP_NORMAL);
}

void SetAnalogValues(uint16_t value, MotorPosition_t position)
{
	if (position == LEFT_TRACK)
	{
		analogValues[0] = value;
	}
	else if (position == RIGHT_TRACK)
	{
		analogValues[1] = value;
	}
	else if (position == BRUSHES)
	{
		analogValues[2] = value;
		analogValues[3] = value;
	}
}

void UpdateCANAnalogModule()
{
	/* TODO: Test with PCAN */
	CAN.sendMsgBuf(0x455, 8, (uint8_t*)&analogValues);
}

void RemoteStart()
{
	uint8_t startRemote = 1;
	CAN.sendMsgBuf(0x0, 1, &startRemote);
}

void CheckCANMessage()
{
	if (CANMessageReceived == true)
	{
		firstMessageCame = true;
		messageTimeoutCounter = 0;
		controllerError = false;

		uint8_t messageLength = 0;
		uint32_t canMessageId = 0;
		if (CAN.readMsgBuf(&canMessageId, &messageLength, CANMessage.data) == CAN_OK)
		{
			if (canMessageId == REMOTE_CONTROL_MESSAGE_ID_1)
			{
				static uint8_t waterPumpButtonOldState = 0;
				static uint8_t previousRampUpValue = 0;
				static uint8_t previousRampDownValue = 0;

				if (CANMessage.byte3.speedButton1 == 1 && previousRampUpValue == 0)
				{
					RPM_SPEED++;
					if (RPM_SPEED > RPM_MAX_SPEED)
					{
						RPM_SPEED = RPM_MAX_SPEED;
					}
				}
				else if (CANMessage.byte3.speedButton2 == 1 && previousRampDownValue == 0)
				{
					RPM_SPEED--;
					if (RPM_SPEED < RPM_MIN_SPEED)
					{
						RPM_SPEED = RPM_MIN_SPEED;
					}
				}
				previousRampUpValue = CANMessage.byte3.speedButton1;
				previousRampDownValue = CANMessage.byte3.speedButton2;

				if (CANMessage.byte8.paddleDValue > SystemParameters.joystickDeadZone)
				{
					uint16_t mappedValue = map(CANMessage.byte8.paddleDValue,
											   SystemParameters.joystickMinValue,
											   SystemParameters.joystickMaxValue,
											   100 * SystemParameters.rightMinSpeed,
											   100 * SystemParameters.rightMaxSpeed * ((double)RPM_SPEED / RPM_MAX_SPEED));
					RightTrackMotor.SetTargetSpeed(mappedValue);
				}
				else
				{
					RightTrackMotor.SetTargetSpeed(0);
				}

				if (CANMessage.byte2.paddleDNorth == 1 && CANMessage.byte2.paddleDSouth == 0)
				{
					RightTrackMotor.SetTargetDirection(REVERSE);
				}
				else if (CANMessage.byte2.paddleDNorth == 0 && CANMessage.byte2.paddleDSouth == 1)
				{
					RightTrackMotor.SetTargetDirection(FORWARD);
				}

				if (CANMessage.byte5.paddleAValue > SystemParameters.joystickDeadZone)
				{
					uint16_t mappedValue = map(CANMessage.byte5.paddleAValue,
											   SystemParameters.joystickMinValue,
											   SystemParameters.joystickMaxValue,
											   100 * SystemParameters.leftMinSpeed,
											   100 * SystemParameters.leftMaxSpeed * ((double)RPM_SPEED / RPM_MAX_SPEED));
					LeftTrackMotor.SetTargetSpeed(mappedValue);
				}
				else
				{
					LeftTrackMotor.SetTargetSpeed(0);
				}

				if (CANMessage.byte2.paddleANorth == 1 && CANMessage.byte2.paddleASouth == 0)
				{
					LeftTrackMotor.SetTargetDirection(FORWARD);
				}
				else if (CANMessage.byte2.paddleANorth == 0 && CANMessage.byte2.paddleASouth == 1)
				{
					LeftTrackMotor.SetTargetDirection(REVERSE);
				}

				if (CANMessage.byte7.potantiometerValue > SystemParameters.joystickDeadZone)
				{
					uint16_t mappedValue = map(CANMessage.byte8.paddleDValue,
											   SystemParameters.potantiometerMinValue,
											   SystemParameters.potantiometerMaxValue,
											   100 * SystemParameters.brushMinSpeed,
											   100 * SystemParameters.brushMaxSpeed);
					BrushesMotor.SetTargetSpeed(mappedValue);
				}
				else
				{
					BrushesMotor.SetTargetSpeed(0);
				}

				if (CANMessage.byte3.frontBrushCW == 0 && CANMessage.byte3.frontBrushCCW == 1)
				{
					BrushesMotor.SetTargetDirection(FORWARD);
				}
				else if (CANMessage.byte3.frontBrushCW == 1 && CANMessage.byte3.frontBrushCCW == 0)
				{
					BrushesMotor.SetTargetDirection(REVERSE);
				}

				/* TODO: Check emergency switch bit if water pump does not work */
				if (CANMessage.byte1.rcStopRelaysOpened == 1)
				{
					WaterPumpHandler.Request(TURN_OFF);
				}
				else
				{
					if (CANMessage.byte3.waterPumpButton == 1 && waterPumpButtonOldState == 0)
					{
						WaterPumpHandler.Request(TOGGLE);
					}
					waterPumpButtonOldState = CANMessage.byte3.waterPumpButton;
				}
			}
		}
	}
}