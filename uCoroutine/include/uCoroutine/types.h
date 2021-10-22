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
 * types.h
 *
 *  Created on: 22.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_TYPES_H_
#define UCOROUTINE_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "uCoroutine/utils/debug/levels.h"

#ifndef __cplusplus
	#include <stddef.h>

	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
		#include <stdbool.h>
	#else
		#ifndef bool
			#define bool char
			#define true  (1 == 1)
			#define false (! true)
		#endif /* bool */
	#endif /* __STDC_VERSION__ */
#endif

typedef enum _uCoroutineError {
	UC_NO_ERROR,

	UC_ERROR_YIELD,
	UC_ERROR_BLOCKED,

	UC_ERROR_FULL,
	UC_ERROR_EMPTY
} uCoroutineError;

typedef struct _uCoroutine  uCoroutine;
typedef struct _uCoroutine *uCoroutinePtr;

typedef struct _uCoroutineQueue  uCoroutineQueue;
typedef struct _uCoroutineQueue *uCoroutineQueuePtr;

/*!
 * Coroutine state tupe. For internal purposes only.
 */
typedef uint16_t uCoroutineState;

#define UCOROUTINE_STATE_NULL 0


typedef uCoroutineState (*uCoroutineFunc)(uCoroutinePtr self, void *coroutineData);

/*!
 * Coroutine priority type.
 */
typedef uint8_t  uCoroutinePriority;

#define UCOROUTINE_PRIORITY_MIN  0
#define UCOROUTINE_PRIORITY_MAX (UCOROUTINE_CONFIG_PRIORITIES - 1)

/*!
 * Time tick type.
 */
typedef uint32_t uCoroutineTick;

#define UCOROUTINE_IMMEDIATE 0
#define UCOROUTINE_INFINITY  UINT32_MAX

/*!
 * Indicates millisecond period between ticks.
 */
#define UCOROUTINE_MS_PER_TICK (1000 / UCOROUTINE_CONFIG_TICK_HZ)

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_TYPES_H_ */
