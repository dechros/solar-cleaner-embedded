#include <Arduino.h>
#include "timerOperations.h"

bool updateMotorsParameters = false;
bool sendRemoteStartMessage = false;
bool checkMessageTimeoutFlag = false;

uint8_t remoteStartCounter = 0;

void SetupTimer()
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
    if (remoteStartCounter == 20) /* 1 sec */
    {
        sendRemoteStartMessage = true;
		checkMessageTimeoutFlag = true;
        remoteStartCounter = 0;
    }
}