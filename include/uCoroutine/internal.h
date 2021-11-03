/*
 * internal.h
 *
 *  Created on: 23.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_INTERNAL_H_
#define UCOROUTINE_INTERNAL_H_

#include "uCoroutine/slist.h"

struct _uCoroutine {
	uCoroutineFunc  func;
	void           *funcData;
	uCoroutineState pc;

	SList           stateListItem; // Item in ready, delayed state lists
	SList           eventListItem; // Item in event lists
};


#define __UCOROUTINE_BEGIN(_ucPtr) \
	switch (((uCoroutine *)_ucPtr)->pc) { \
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
	_uCoroutine_sleepTicks(_ticks); \
	__uCoroutine_yield(0);

void _uCoroutine_sleepTicks(uCoroutineTick ticks);


#endif /* UCOROUTINE_INTERNAL_H_ */
