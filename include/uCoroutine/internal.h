/*
 * internal.h
 *
 *  Created on: 23.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_INTERNAL_H_
#define UCOROUTINE_INTERNAL_H_

#include "uCoroutine/config.h"
#include "uCoroutine/utils/slist.h"
#include "uCoroutine/utils/list.h"

struct _uCoroutine {
	uCoroutineFunc     func;
	void              *funcData;
	uCoroutinePriority priority;
	char               name[UCOROUTINE_CONFIG_NAME_LENGTH + 1];

	uCoroutineTick  delayTicks;
	uCoroutineState state;
	List            stateListItem; // Item in ready, delayed state lists
	List            eventListItem; // Item in event lists
};


struct _uCoroutineQueue {
	int8_t *memoryAreaHead;  // Head of the queue storage area.
	int8_t *memoryAreaTail;  // Tail of the queue storage area.
	int8_t *memoryAreaRead;  // Pointer to prev used item in storage area.
	int8_t *memoryAreaWrite; // Pointer to next free item in storage area.

	size_t size;
	size_t used;
	size_t itemSize;

	List waitingToSend;    // List of coroutines waiting on posting a message into queue.
	List waitingToReceive; // List of coroutines waiting on reading a message from queue.
};


#define __UCOROUTINE_BEGIN(_ucPtr) \
	switch (((uCoroutine *)_ucPtr)->state) { \
		case UCOROUTINE_STATE_NULL:

#define __UCOROUTINE_END() \
		default: \
			break; \
	}

#define __UCOROUTINE_FUNC_BEGIN(__name, __dataType) static uCoroutineState __name(uCoroutine *self, void *_taskData) { \
	__dataType *context = (__dataType *) _taskData; \
	\
	__UCOROUTINE_BEGIN(self);

#define __UCOROUTINE_FUNC_END \
	__UCOROUTINE_END(); \
	\
	return UCOROUTINE_STATE_NULL; \
}

#define _UC_STATE(_order) ((__LINE__ * 2) + (_order))

#define __uCoroutine_yield(_order) \
	return _UC_STATE(_order); case _UC_STATE(_order):

#define __uCoroutine_sleepTicks(_ticks) \
	if (_ticks != UCOROUTINE_IMMEDIATE) { \
		_uCoroutine_suspend(_ticks, NULL); \
	} \
	__uCoroutine_yield(0);

bool _uCoroutine_wakeup(List *eventList);
void _uCoroutine_suspend(uCoroutineTick timeout, List *eventList);


void            _uCoroutine_queue_init       (uCoroutineQueuePtr queuePtr, void *memoryArea, size_t itemSize, size_t itemsCount);
uCoroutineError _uCoroutine_queue_send       (uCoroutineQueuePtr queuePtr, void *itemPtr, uCoroutineTick timeoutTicks);
bool            _uCoroutine_queue_send_isr   (uCoroutineQueuePtr queuePtr, void *itemPtr, bool coroutineWasWokenUp);
uCoroutineError _uCoroutine_queue_receive    (uCoroutineQueuePtr queuePtr, void *itemPtr, uCoroutineTick timeoutTicks);
bool            _uCoroutine_queue_receive_isr(uCoroutineQueuePtr queuePtr, void *itemPtr, bool coroutineWasWokenUp);


#endif /* UCOROUTINE_INTERNAL_H_ */
