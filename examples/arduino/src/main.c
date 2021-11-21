/*
* main.c
*
*  Created on: 20.11.2021
*      Author: Jaroslaw Bielski (bielski.j@gmail.com)
*/

#include <avr/io.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"


#define UART_BAUD 38400
#define UART_BAUD_REG (((F_CPU / 16) / UART_BAUD) - 1)

#define _waitForTransmit() while (! (UCSR0A & _BV(UDRE0)));


static uCoroutineQueue _uartQueue;
static char            _uartQueueStorage[8];


ISR(USART_RX_vect) {
	char byte = UDR0;

	uCoroutine_queue_send_isr(&_uartQueue, &byte, false);
}


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
				_waitForTransmit();

				UDR0 = rxByte;
			}
		}
	}
	UCOROUTINE_END();

	return UCOROUTINE_STATE_NULL;
}


static uCoroutine ledCrtn;
static uCoroutine uartCrtn;


int main(int argc, char *argv[]) {
	uCoroutine_platform_initialize();

	uCoroutine_initialize();

	// usart
	{
		UBRR0H = ((UART_BAUD_REG) >> 8);
		UBRR0L = ((UART_BAUD_REG) & 0x00FF);

		// 8bit, 1bit stop, no parity
		UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

		// Enable USART
		UCSR0B |= (_BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0));
	}

	// PIO initialize
	DDRB |= _BV(PIN5);

	sei();

	{
		uCoroutine_prepare(&ledCrtn, NULL, UCOROUTINE_PRIORITY_MIN, led, NULL);

		uCoroutine_start(&ledCrtn);
	}

	{
		uCoroutine_queue_init(&_uartQueue, _uartQueueStorage, 1, sizeof(_uartQueueStorage));

		uCoroutine_prepare(&uartCrtn, NULL, UCOROUTINE_PRIORITY_MAX, uart, NULL);

		uCoroutine_start(&uartCrtn);
	}

	uCoroutine_schedule();
}
