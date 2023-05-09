#include "waterPumpDriver.h"

void WaterPumpOn()
{
    digitalWrite(WATER_ON_OFF_PIN, HIGH);
}

void WaterPumpOff()
{
    digitalWrite(WATER_ON_OFF_PIN, LOW);
}