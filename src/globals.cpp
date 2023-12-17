#include "globals.h"

Motor LeftTrackMotor(LEFT_TRACK);
Motor RightTrackMotor(RIGHT_TRACK);
Motor BrushesMotor(BRUSHES);

WaterPump WaterPumpHandler;

Parameters_t SystemParameters;

void SystemStop()
{
    LeftTrackMotor.Stop();
    RightTrackMotor.Stop();
    BrushesMotor.Stop();
    WaterPumpHandler.Request(TURN_OFF);
}

void SystemResume()
{
    LeftTrackMotor.ResumeIfStopped();
    RightTrackMotor.ResumeIfStopped();
    BrushesMotor.ResumeIfStopped();
}