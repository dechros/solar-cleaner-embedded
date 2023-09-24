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
	LeftPalletMotor.Init();
	RightPalletMotor.Init();
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
#endif
	if (updateMotorsParameters == true)
	{
		LeftPalletMotor.RunAlgorithm();
		RightPalletMotor.RunAlgorithm();
		BrushesMotor.RunAlgorithm();
		updateMotorsParameters = false;
	}
}
