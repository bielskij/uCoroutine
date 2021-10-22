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
 * queue.h
 *
 *  Created on: 16.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_QUEUE_H_
#define UCOROUTINE_QUEUE_H_

#include "uCoroutine/types.h"

#ifdef __cplusplus
	extern "C" {
#endif

#include "uCoroutine/internal.h"


#define uCoroutine_queue_init(queuePtr, memoryAreaPtr, itemSize, itemsCount) \
	_uCoroutine_queue_init(queuePtr, memoryAreaPtr, itemSize, itemsCount)


#define uCoroutine_queue_send(queuePtr, itemPtr, timeoutTicks, errorCodePtr) \
	do { \
		*(errorCodePtr) = _uCoroutine_queue_send((queuePtr), (itemPtr), (timeoutTicks)); \
\
		if (*(errorCodePtr) == UC_ERROR_BLOCKED) { \
			__uCoroutine_yield(0); \
\
			*(errorCodePtr) = _uCoroutine_queue_send((queuePtr), (itemPtr), 0); \
		} \
\
		if (*(errorCodePtr) == UC_ERROR_YIELD) { \
			__uCoroutine_yield(1); \
\
			*(errorCodePtr) = UC_NO_ERROR; \
		} \
	} while (0);


#define uCoroutine_queue_receive(queuePtr, itemPtr, timeoutTicks, errorCodePtr) \
	do { \
		*(errorCodePtr) = _uCoroutine_queue_receive((queuePtr), (itemPtr), (timeoutTicks)); \
\
		if (*(errorCodePtr) == UC_ERROR_BLOCKED) { \
			__uCoroutine_yield(0); \
\
			*(errorCodePtr) = _uCoroutine_queue_receive((queuePtr), (itemPtr), 0); \
		} \
\
		if (*(errorCodePtr) == UC_ERROR_YIELD) { \
			__uCoroutine_yield(1); \
\
			*(errorCodePtr) = UC_NO_ERROR; \
		} \
	} while (0);


#define uCoroutine_queue_send_isr(queuePtr, itemPtr, coroutineWasWokenUpPtr) \
	_uCoroutine_queue_send_isr((queuePtr), (itemPtr), (coroutineWasWokenUpPtr))


#define uCoroutine_queue_receive_isr(queuePtr, itemPtr, coroutineWasWokenUpPtr) \
	_uCoroutine_queue_receive_isr((queuePtr), (itemPtr), (coroutineWasWokenUpPtr))

#ifdef __cplusplus
	}
#endif

#endif /* UCOROUTINE_QUEUE_H_ */
