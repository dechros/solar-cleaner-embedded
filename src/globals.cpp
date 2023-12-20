#include "globals.h"

Motor LeftTrackMotor(LEFT_TRACK);
Motor RightTrackMotor(RIGHT_TRACK);
Motor BrushesMotor(BRUSHES);

WaterPump WaterPumpHandler;

Parameters_t SystemParameters;

uint8_t messageTimeoutCounter = 0;
bool controllerError = false;
bool firstMessageCame = false;

void SystemStop()
{
    LeftTrackMotor.Stop();
    RightTrackMotor.Stop();
    BrushesMotor.Stop();
    WaterPumpHandler.Request(TURN_OFF);
}
