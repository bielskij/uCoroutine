/*
 * platform.c
 *
 *  Created on: 21.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"


static volatile uCoroutineTick _currentTime = 0;


ISR(TIMER0_COMPA_vect){
	_currentTime++;
}


uCoroutineTick uCoroutine_platform_getTicks() {
	uCoroutineTick ret;

	cli();
	{
		ret = _currentTime;
	}
	sei();

	return ret;
}


void uCoroutine_platform_initialize() {
	// Timer0 (1ms interval)
	{
		TCCR0A = 0;
		TCCR0B = 0;

		TCNT0  = 0;

		OCR0A = 249;

		TCCR0B |= _BV(WGM01);
		TCCR0B |= (_BV(CS01) | _BV(CS00));

		TIMSK0 |= _BV(OCIE0A);
	}
}
