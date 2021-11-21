/*
* main.c
*
*  Created on: 20.11.2021
*      Author: Jaroslaw Bielski (bielski.j@gmail.com)
*/

#include <avr/io.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"


UCOROUTINE_FUNC_BEGIN(led, void) {
	while (1) {
		uCoroutine_sleepMs(100);
		PORTB |= _BV(PIN5);

		uCoroutine_sleepMs(100);
		PORTB &= ~_BV(PIN5);
	}
}
UCOROUTINE_FUNC_END;


static uCoroutine ledCrtn;


int main(int argc, char *argv[]) {
	uCoroutine_platform_initialize();

	uCoroutine_initialize();

	// PIO initialize
	DDRB |= _BV(PIN5);

	sei();

	uCoroutine_prepare(&ledCrtn, NULL, UCOROUTINE_PRIORITY_MIN, led, NULL);

	uCoroutine_start(&ledCrtn);
	{
		uCoroutine_schedule();
	}
}
