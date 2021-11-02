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

/*!
 * The following two macros define a coroutine block of code. They need a pointer to uCoroutine
 * structure.
 */
#define UCOROUTINE_BEGIN(_ucPtr) __UCOROUTINE_BEGIN(_ucPtr)
#define UCOROUTINE_END()         __UCOROUTINE_END()

/*!
 * Useful macros for simple defining coroutine functions.
 *
 * @param __name     coroutine function name
 * @param __dataType type of user data. User data will be available trough 'context'
 *                   pointer in the routine code.
 */
#define UCOROUTINE_FUNC_BEGIN(__name, __dataType) __UCOROUTINE_FUNC_BEGIN(__name, __dataType)
#define UCOROUTINE_FUNC_END                       __UCOROUTINE_FUNC_END

/*!
 * Initializes internals.
 */
void uCoroutine_initialize(void);

/*!
 * Terminates internals.
 */
void uCoroutine_terminate(void);

#ifdef UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION
/*!
 * Allocates a new uCorotine object and configures it by calling
 * uCoroutine_configure() internally.
 *
 * @param priority coroutine priority
 * @param func     coroutine function
 * @param funcData user data provided to coroutine function
 */
uCoroutinePtr uCoroutine_new(
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
);
#endif

/*!
 * Prepares/configures uCoroutine object. This method should be called
 * before starting coroutine. Calling this method does not automatically start
 * the coroutine.
 *
 * @param priority coroutine priority
 * @param func     coroutine function
 * @param funcData user data provided to coroutine function
 */
void uCoroutine_configure(
	uCoroutinePtr      coroutine,
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
);

/*!
 *
 */
void uCoroutine_start(uCoroutinePtr coroutine);

/*!
 *
 */
void uCoroutine_stop(uCoroutinePtr coroutine);

/*!
 *
 */
#define uCoroutine_yield                __uCoroutine_yield

/*!
 *
 */
#define uCoroutine_sleep(seconds)       __uCoroutine_sleep(seconds)

/*!
 *
 */
#define uCoroutine_sleepMs(miliseconds) __uCoroutine_sleepMs(miliseconds)

/*!
 *
 */
#define uCoroutine_sleepTicks(ticks)    __uCoroutine_sleepTicks(ticks)

/*!
 * Coroutines scheduler mainloop. This method never returns unless
 * uCoroutine_interrupt() function is called.
 */
void uCoroutine_schedule(void);

/*!
 * Breaks uCoroutine_schedule() method execution.
 */
void uCoroutine_interrupt(void);

/*!
 * This function is called when no coroutine can run in a loop run.
 */
void uCoroutine_idle_callback(void) __attribute__((weak));

#ifdef __cplusplus
	}
#endif

#endif /* UCOROUTINE_H_ */
