/*
 * uCoroutine.h
 *
 *  Created on: 22.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_H_
#define UCOROUTINE_H_

#include "uCoroutine/types.h"

#ifdef __cplusplus
	extern "C" {
#endif

#include "uCoroutine/internal.h"
#include "uCoroutine/config.h"


void uCoroutine_initialize(void);

void uCoroutine_terminate(void);

#ifdef UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION
uCoroutinePtr uCoroutine_new(
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
);
#endif

void uCoroutine_configure(
	uCoroutinePtr      coroutine,
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
);

void uCoroutine_start(uCoroutinePtr coroutine);

void uCoroutine_stop(uCoroutinePtr coroutine);

#define uCoroutine_yield                uCoroutine_internal_yield

#define uCoroutine_sleep(seconds)       uCoroutine_internal_sleep(seconds)
#define uCoroutine_sleepMs(miliseconds) uCoroutine_internal_sleepMs(miliseconds)
#define uCoroutine_sleepTicks(ticks)    uCoroutine_internal_sleepTicks(ticks)

void uCoroutine_schedule(void);

/*!
 * Breaks uCoroutine_schedule() method execution.
 */
void uCoroutine_interrupt(void);

void uCoroutine_idle_callback(void) __attribute__((weak));

#ifdef __cplusplus
	}
#endif

#endif /* UCOROUTINE_H_ */
