/*
 * uCoroutine.h
 *
 *  Created on: 22.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_H_
#define UCOROUTINE_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "uCoroutine/types.h"


typedef struct _uCoroutine  uCoroutine;
typedef struct _uCoroutine *uCoroutinePtr;

typedef uint16_t (*uCoroutineFunc)(void *data);

struct _uCoroutine {
	uint16_t pc;
};

#define UCOROUTINE_INITIALIZER { \
	.pc = 0; \
}


void uCoroutine_initialize(void);

void uCoroutine_terminate(void);

#ifdef UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION
uCoroutinePtr uCoroutine_new();
#endif

void uCoroutine_schedule(void);

/*!
 * Breaks uCoroutine_schedule() method execution.
 */
void uCoroutine_interrupt(void);

#ifdef __cplusplus
	}
#endif

#endif /* UCOROUTINE_H_ */
