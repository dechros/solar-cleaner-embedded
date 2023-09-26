#include "globals.h"
#include "motor.h"
#include "timerOperations.h"
#include "pinOperations.h"
#include "waterPumpDriver.h"
#include "eepromOperations.h"

#if CAN_MODE
#include "CANOperations.h"
#else
#include "routerOperations.h"
#endif

#define GSM_UPDATE_INTERVAL  		(100)
#define ACCELERATION_SENSOR_FILTER  (5)
#define ERROR_CHECK_INTERVAL 		(5)

void setup()
{
	Serial.begin(115200);
	InitPins();
	LeftTrackMotor.Init();
	RightTrackMotor.Init();
	BrushesMotor.Init();
#if CAN_MODE
	InitCANBus();
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
	CheckTCPMessage();
	if (checkMessageTimeoutFlag == true)
	{
		CheckMessageTimeout();
		checkMessageTimeoutFlag = false;
	}
#endif
	if (updateMotorsParameters == true)
	{
		LeftTrackMotor.RunAlgorithm();
		RightTrackMotor.RunAlgorithm();
		BrushesMotor.RunAlgorithm();
		updateMotorsParameters = false;
	}

}
