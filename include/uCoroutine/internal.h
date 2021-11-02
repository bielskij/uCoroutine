/*
 * internal.h
 *
 *  Created on: 23.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_INTERNAL_H_
#define UCOROUTINE_INTERNAL_H_

#define UCOROUTINE_PC_INIT 0

struct _uCoroutine {
	uCoroutineFunc func;
	void          *funcData;
	uint16_t pc;
};


#define __UCOROUTINE_BEGIN(_ucPtr) \
	switch (((uCoroutine *)_ucPtr)->pc) { \
		case UCOROUTINE_PC_INIT:

#define __UCOROUTINE_END() \
		default: \
			break; \
	}

#define __UCOROUTINE_FUNC_BEGIN(__name, __dataType) static void __name(uCoroutine *_self, void *_taskData) { \
	__dataType *context = (__dataType *) __taskData; \
	\
	__UCOROUTINE_BEGIN(_self);

#define __UCOROUTINE_FUNC_END \
	__UCOROUTINE_END();

#endif /* UCOROUTINE_INTERNAL_H_ */
