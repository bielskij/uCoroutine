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
 * uCoroutine.h
 *
 *  Created on: 22.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_H_
#define UCOROUTINE_H_

// Headers provided by an implementer
#include "uCoroutine/config.h"
#include "uCoroutine/platform.h"

#include "uCoroutine/types.h"
#include "uCoroutine/queue.h"

#include "uCoroutine/utils/utils.h"

#ifdef __cplusplus
	extern "C" {
#endif

#include "uCoroutine/internal.h"

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
 * uCoroutine_prepare() internally.
 *
 * @param priority coroutine priority
 * @param func     coroutine function
 * @param funcData user data provided to coroutine function
 */
uCoroutinePtr uCoroutine_new(
	uCoroutinePriority priority,
	const char        *name,
	uCoroutineFunc     func,
	void              *funcData
);

/**
 * Disposes dynamically created coroutine.
 *
 * @param coroutine coroutine pointer
 */
void uCoroutine_free(uCoroutinePtr coroutine);
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
void uCoroutine_prepare(
	uCoroutinePtr      coroutine,
	const char        *name,
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
#define uCoroutine_yield()              __uCoroutine_yield(0)

/*!
 *
 */
#define uCoroutine_sleep(seconds)       __uCoroutine_sleepTicks(UC_SEC_TO_TICKS(seconds))

/*!
 *
 */
#define uCoroutine_sleepMs(miliseconds) __uCoroutine_sleepTicks(UC_MS_TO_TICKS(miliseconds))

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
