/*
 * uart.c
 *
 *  Created on: 25.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"


#define UART_BAUD 38400
#define UART_BAUD_REG (((F_CPU / 16) / UART_BAUD) - 1)

#define _waitForTransmit() while (! (UCSR0A & _BV(UDRE0)));


static uCoroutineQueuePtr _inputFifo = NULL;


ISR(USART_RX_vect) {
	char byte = UDR0;

	uCoroutine_queue_send_isr(_inputFifo, &byte, false);
}


void uart_init(uCoroutineQueuePtr inputQueue) {
	_inputFifo = inputQueue;

	UBRR0H = ((UART_BAUD_REG) >> 8);
	UBRR0L = ((UART_BAUD_REG) & 0x00FF);

	// 8bit, 1bit stop, no parity
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	// Enable USART
	UCSR0B |= (_BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0));
}


void uart_send(char c) {
	_waitForTransmit();

	UDR0 = c;
}
