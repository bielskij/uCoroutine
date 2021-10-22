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
 * queue.c
 *
 *  Created on: 16.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <string.h>

#include "uCoroutine/queue.h"
#include "uCoroutine/platform.h"


static void _pushItem(uCoroutineQueuePtr queue, void *item) {
	memcpy(queue->memoryAreaWrite, item, queue->itemSize);

	queue->memoryAreaWrite += queue->itemSize;
	if (queue->memoryAreaWrite >= queue->memoryAreaTail) {
		queue->memoryAreaWrite = queue->memoryAreaHead;
	}

	queue->used++;
}


static void _popItem(uCoroutineQueuePtr queue, void *item) {
	queue->memoryAreaRead += queue->itemSize;
	if (queue->memoryAreaRead >= queue->memoryAreaTail) {
		queue->memoryAreaRead = queue->memoryAreaHead;
	}

	memcpy(item, queue->memoryAreaRead, queue->itemSize);

	queue->used--;
}


static uCoroutineError _wake(uCoroutineQueuePtr queue, bool read) {
	List *waitingList = read ? &queue->waitingToReceive : &queue->waitingToSend;

	if (! list_isEmpty(waitingList)) {
		if (_uCoroutine_wakeup(waitingList)) {
			return UC_ERROR_YIELD;
		}
	}

	return UC_NO_ERROR;
}


void _uCoroutine_queue_init(uCoroutineQueuePtr queue, void *memoryArea, size_t itemSize, size_t itemsCount) {
	queue->memoryAreaHead  = memoryArea;
	queue->memoryAreaTail  = queue->memoryAreaHead + (itemSize * itemsCount);
	queue->memoryAreaWrite = queue->memoryAreaHead;
	queue->memoryAreaRead  = queue->memoryAreaTail - itemSize;

	queue->itemSize = itemSize;
	queue->size     = itemsCount;
	queue->used     = 0;

	list_initialize(&queue->waitingToReceive);
	list_initialize(&queue->waitingToSend);
}


uCoroutineError _uCoroutine_queue_send(uCoroutineQueuePtr queuePtr, void *itemPtr, uCoroutineTick timeoutTicks) {
	uCoroutineError ret = UC_NO_ERROR;

	uCoroutine_platform_isr_disable();
	{
		if (queuePtr->used == queuePtr->size) {
			if (timeoutTicks == UCOROUTINE_IMMEDIATE) {
				ret = UC_ERROR_FULL;

			} else {
				_uCoroutine_suspend(timeoutTicks, &queuePtr->waitingToSend);

				ret = UC_ERROR_BLOCKED;
			}
		}
	}
	uCoroutine_platform_isr_enable();

	if (ret == UC_NO_ERROR) {
		uCoroutine_platform_isr_disable();
		{
			if (queuePtr->used == queuePtr->size) {
				ret = UC_ERROR_FULL;

			} else {
				_pushItem(queuePtr, itemPtr);

				ret = _wake(queuePtr, true);
			}
		}
		uCoroutine_platform_isr_enable();
	}

	return ret;
}


uCoroutineError _uCoroutine_queue_receive(uCoroutineQueuePtr queuePtr, void *itemPtr, uCoroutineTick timeoutTicks) {
	uCoroutineError ret = UC_NO_ERROR;

	uCoroutine_platform_isr_disable();
	{
		if (queuePtr->used == 0) {
			if (timeoutTicks == UCOROUTINE_IMMEDIATE) {
				ret = UC_ERROR_EMPTY;

			} else {
				_uCoroutine_suspend(timeoutTicks, &queuePtr->waitingToReceive);

				ret = UC_ERROR_BLOCKED;
			}
		}
	}
	uCoroutine_platform_isr_enable();

	if (ret == UC_NO_ERROR) {
		uCoroutine_platform_isr_disable();
		{
			if (queuePtr->used == 0) {
				ret = UC_ERROR_EMPTY;

			} else {
				_popItem(queuePtr, itemPtr);

				ret = _wake(queuePtr, false);
			}
		}
		uCoroutine_platform_isr_enable();
	}

	return ret;
}


bool _uCoroutine_queue_send_isr(uCoroutineQueuePtr queuePtr, void *itemPtr, bool coroutineWasWokenUp) {
	if (queuePtr->used < queuePtr->size) {
		_pushItem(queuePtr, itemPtr);

		if (! coroutineWasWokenUp) {
			return _wake(queuePtr, true) == UC_ERROR_YIELD;
		}
	}

	return coroutineWasWokenUp;
}


bool _uCoroutine_queue_receive_isr(uCoroutineQueuePtr queuePtr, void *itemPtr, bool coroutineWasWokenUp) {
	if (queuePtr->used > 0) {
		_popItem(queuePtr, itemPtr);

		if (! coroutineWasWokenUp) {
			return _wake(queuePtr, false) == UC_ERROR_YIELD;
		}
	}

	return coroutineWasWokenUp;
}
