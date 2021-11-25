/*
 * uart.h
 *
 *  Created on: 25.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UART_H_
#define UART_H_

#include "uCoroutine.h"

void uart_init(uCoroutineQueuePtr inputQueue);

void uart_send(char c);

#endif /* UART_H_ */
