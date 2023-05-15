#include "readInputPinsTask.h"

void vReadInputPinsTask(void *pvParameters)
{
    configASSERT(((uint32_t)pvParameters) == 1);
    
    uint8_t leftPalletError = 0;
    uint8_t rightPalletError = 0;
    uint8_t brushesError = 0;

    while (true)
    {
        (void)leftPalletError;
        (void)rightPalletError;
        (void)brushesError;
        leftPalletError = DigitalReadThreadSafe(PALLET_LEFT_ERROR_PIN);
        rightPalletError = DigitalReadThreadSafe(PALLET_RIGHT_ERROR_PIN);
        brushesError = DigitalReadThreadSafe(BRUSHES_ERROR_PIN);
        taskYIELD();
    }
}