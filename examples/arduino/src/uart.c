/*******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2021 Jaroslaw Bielski (bielski.j@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
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
