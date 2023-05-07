#include "globals.h"

TaskHandle_t xSerialParserTaskHandle = NULL;
TaskHandle_t xSecondTaskHandle = NULL;

void InitializePins()
{
    pinMode(BRUSHES_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(BRUSHES_FORWARD_TURN_PIN , OUTPUT);
    pinMode(BRUSHES_STOP_PIN         , OUTPUT);
    pinMode(BRUSHES_BACKWARD_TURN_PIN, OUTPUT);
    pinMode(BRUSHES_ERROR_PIN        , INPUT);

    pinMode(WATER_ON_OFF_PIN, OUTPUT);

    pinMode(PALLET_LEFT_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(PALLET_LEFT_FORWARD_TURN_PIN , OUTPUT);
    pinMode(PALLET_LEFT_STOP_PIN         , OUTPUT);
    pinMode(PALLET_LEFT_BACKWARD_TURN_PIN, OUTPUT);
    pinMode(PALLET_LEFT_ERROR_PIN        , INPUT);

    pinMode(PALLET_RIGHT_SPEED_CONTROL_PIN, OUTPUT);
    pinMode(PALLET_RIGHT_FORWARD_TURN_PIN , OUTPUT);
    pinMode(PALLET_RIGHT_STOP_PIN         , OUTPUT);
    pinMode(PALLET_RIGHT_BACKWARD_TURN_PIN, OUTPUT);
    pinMode(PALLET_RIGHT_ERROR_PIN        , INPUT);

    pinMode(A0, OUTPUT);
}

