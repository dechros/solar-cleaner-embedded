#include "globals.h"
#include "motor.h"
#include "timerOperations.h"
#include "pinOperations.h"
#include "waterPump.h"
#include "eepromOperations.h"
#include "errorOperations.h"
#include "maintenanceOperations.h"
#include "sensorOperations.h"
#include "CANOperations.h"

#if CAN_MODE
#else
#include "routerOperations.h"
#endif

#define GSM_UPDATE_INTERVAL (100)
#define ACCELERATION_SENSOR_FILTER (5)
#define ERROR_CHECK_INTERVAL (5)

void setup()
{
	InitPins();
	InitParameters();
	WaterPumpHandler.Init();
	LeftTrackMotor.Init();
	RightTrackMotor.Init();
	BrushesMotor.Init();
	InitTimer();
	InitCANBus();

#if CAN_MODE
#else
	InitRouterCommunication();
#endif

}

void loop()
{
#if CAN_MODE
	CheckCANMessage();
	if (sendRemoteStartMessage == true)
	{
		RemoteStart();
		sendRemoteStartMessage = false;
	}
#else
	if (CheckMaintenancePin() == LOW && IsRouterMaintenanceActive() == false)
	{
		CheckTCPMessage();
	}
#endif

	if (updateMotorsParameters == true)
	{
		if (DetectErrors() == true || CheckMaintenancePin() == HIGH || 
			(sensorReadActive == true && DetectSensorActivity() == true))
		{
			SystemStop();
		}
		
		WaterPumpHandler.ProcessRequests();
		LeftTrackMotor.RunRampSupport();
		RightTrackMotor.RunRampSupport();
		BrushesMotor.RunRampSupport();
		UpdateCANAnalogModule();
		updateMotorsParameters = false;
	}

	if (CheckMaintenancePin() == HIGH)
	{
		if (IsRouterMaintenanceActive() == false)
		{
			ActivateRouterMaintenanceMode();
		}
		else
		{
			CheckMaintenanceMessages();
		}
	}
	else
	{
		if (IsRouterMaintenanceActive() == true)
		{
			DeactivateRouterMaintenanceMode();
		}
	}
}
