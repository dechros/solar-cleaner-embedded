#include <Arduino.h>
#include "timerOperations.h"
#include "globals.h"

bool updateMotorsParameters = false;
bool sendRemoteStartMessage = false;

uint8_t remoteStartCounter = 0;

void InitTimer()
{
    /* 50ms timer interrupt for motor speed control */
	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = 12499;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);  
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	updateMotorsParameters = true;
    remoteStartCounter++;
	messageTimeoutCounter++;
    if (remoteStartCounter == 20) /* 1 sec */
    {
        sendRemoteStartMessage = true;
        remoteStartCounter = 0;
    }

	if (messageTimeoutCounter == 20)
	{
		if (firstMessageCame == true)
		{
			controllerError = true;
		}
		messageTimeoutCounter = 0;
	}
}