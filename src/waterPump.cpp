#include <Arduino.h>
#include "waterPump.h"
#include "pinDefinitions.h"


WaterPump::WaterPump()
{
    /* Empty Constructor */   
}

WaterPump::~WaterPump()
{
    /* Empty Destructor */  
}

void WaterPump::Init()
{
    digitalWrite(WATER_ON_OFF_PIN, LOW);
}

void WaterPump::ProcessRequests()
{
    switch (currentRequest)
    {
        case TURN_ON:
        {
            TurnOn();
            break;
        }
        case TURN_OFF:
        {
            TurnOff();
            break;
        }
        case TOGGLE:
        {
            Toggle();
            break;
        }
        default:
        {
            break;
        }
    }
    currentRequest = DO_NOTHING;
}

void WaterPump::Request(RequestType_t type)
{
    currentRequest = type;
}

void WaterPump::TurnOn()
{
    digitalWrite(WATER_ON_OFF_PIN, HIGH);
}

void WaterPump::TurnOff()
{
    digitalWrite(WATER_ON_OFF_PIN, LOW);
}

void WaterPump::Toggle()
{
    digitalWrite(WATER_ON_OFF_PIN, !digitalRead(WATER_ON_OFF_PIN));
}
