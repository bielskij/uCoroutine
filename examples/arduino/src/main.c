/*
* main.c
*
*  Created on: 20.11.2021
*      Author: Jaroslaw Bielski (bielski.j@gmail.com)
*/

#include <avr/io.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"

#include "uart.h"

static uCoroutineQueue _uartQueue;
static char            _uartQueueStorage[8];

static uCoroutine ledCrtn;
static uCoroutine uartCrtn;


UCOROUTINE_FUNC_BEGIN(led, void) {
	while (1) {
		uCoroutine_sleepMs(100);
		PORTB |= _BV(PIN5);

		uCoroutine_sleepMs(100);
		PORTB &= ~_BV(PIN5);
	}
}
UCOROUTINE_FUNC_END;


static uCoroutineState uart(uCoroutinePtr self, void *coroutineData) {
	char rxByte;
	uCoroutineError ret;

	UCOROUTINE_BEGIN(self);
	{
		while (1) {
			uCoroutine_queue_receive(&_uartQueue, &rxByte, UCOROUTINE_INFINITY, &ret);

			if (ret == UC_NO_ERROR) {
				uart_send(rxByte);
			}
		}
	}
	UCOROUTINE_END();

	return UCOROUTINE_STATE_NULL;
}


int main(int argc, char *argv[]) {
	uCoroutine_platform_initialize();

	uCoroutine_initialize();

	uCoroutine_queue_init(&_uartQueue, _uartQueueStorage, 1, sizeof(_uartQueueStorage));

	uart_init(&_uartQueue);

	// PIO initialize
	DDRB |= _BV(PIN5);

	sei();

	{
		uCoroutine_prepare(&ledCrtn, NULL, UCOROUTINE_PRIORITY_MIN, led, NULL);

		uCoroutine_start(&ledCrtn);
	}

	{
		uCoroutine_prepare(&uartCrtn, NULL, UCOROUTINE_PRIORITY_MAX, uart, NULL);

		uCoroutine_start(&uartCrtn);
	}

	uCoroutine_schedule();
}
