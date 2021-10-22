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
 * platform.c
 *
 *  Created on: 21.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"


#define UART_BAUD 38400
#define UART_BAUD_REG (((F_CPU / 16) / UART_BAUD) - 1)

#define _waitForTransmit() while (! (UCSR0A & _BV(UDRE0)));


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
