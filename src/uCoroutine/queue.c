/*
 * queue.c
 *
 *  Created on: 16.11.2021
 *      Author: jarko
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
