#include "globals.h"
#include "motor.h"
#include "timerOperations.h"
#include "pinOperations.h"
#include "waterPump.h"
#include "eepromOperations.h"
#include "errorOperations.h"
#include "maintenanceOperations.h"
#include "sensorOperations.h"

#if CAN_MODE
#include "CANOperations.h"
#else
#include "routerOperations.h"
#endif

#define GSM_UPDATE_INTERVAL  		(100)
#define ACCELERATION_SENSOR_FILTER  (5)
#define ERROR_CHECK_INTERVAL 		(5)

void PrintParameters();

void setup()
{
	InitPins();
	InitParameters();
	WaterPumpHandler.Init();
	LeftTrackMotor.Init();
	RightTrackMotor.Init();
	BrushesMotor.Init();
	InitTimer();

#if CAN_MODE
	InitCANBus();
#else
	InitRouterCommunication();
#endif

	PrintParameters();
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
		if (DetectErrors() == true || DetectSensorActivity() == true || CheckMaintenancePin() == HIGH)
		{
			SystemStop();
		}
		
		WaterPumpHandler.ProcessRequests();
		LeftTrackMotor.RunRampSupport();
		RightTrackMotor.RunRampSupport();
		BrushesMotor.RunRampSupport();
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

void PrintParameters()
{
	Serial.println(SystemParameters.controlValue);
    Serial.println(SystemParameters.versionMajor);
    Serial.println(SystemParameters.versionMinor);
    Serial.println(SystemParameters.versionPatch);
    for (uint8_t i = 0; i < MACHINE_NAME_SIZE; i++)
    {
        Serial.println(SystemParameters.companyName[i]);
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        Serial.println(SystemParameters.machineIP[i]);
    }
    Serial.println(SystemParameters.leftErrorCount        );
    Serial.println(SystemParameters.rightErrorCount       );
    Serial.println(SystemParameters.brushErrorCount       );
    Serial.println(SystemParameters.controllerErrorCount  );
    Serial.println(SystemParameters.leftRampUp            );
    Serial.println(SystemParameters.leftRampDown          );
    Serial.println(SystemParameters.leftMinSpeed          );
    Serial.println(SystemParameters.leftMaxSpeed          );
    Serial.println(SystemParameters.rightRampUp           );
    Serial.println(SystemParameters.rightRampDown         );
    Serial.println(SystemParameters.rightMinSpeed         );
    Serial.println(SystemParameters.rightMaxSpeed         );
    Serial.println(SystemParameters.brushRampUp           );
    Serial.println(SystemParameters.brushRampDown         );
    Serial.println(SystemParameters.brushMinSpeed         );
    Serial.println(SystemParameters.brushMaxSpeed         );
    Serial.println(SystemParameters.joystickMiddleValue   );  
    Serial.println(SystemParameters.joystickDeadZone      );
    Serial.println(SystemParameters.joystickMinValue      );
    Serial.println(SystemParameters.joystickMaxValue      ); 
    Serial.println(SystemParameters.potantiometerMinValue );      
    Serial.println(SystemParameters.potantiometerMaxValue ); 
}
