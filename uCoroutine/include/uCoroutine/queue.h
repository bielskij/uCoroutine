/*
 * queue.h
 *
 *  Created on: 16.11.2021
 *      Author: jarko
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
