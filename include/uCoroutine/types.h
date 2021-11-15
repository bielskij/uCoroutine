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

#include "uCoroutine/debug/levels.h"

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
	UC_ERROR_BLOCKED
} uCoroutineError;

typedef struct _uCoroutine  uCoroutine;
typedef struct _uCoroutine *uCoroutinePtr;

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

/*!
 * Time tick type.
 */
typedef uint32_t uCoroutineTick;

#define UCOROUTINE_PRIORITY_MIN 0
#define UCOROUTINE_PRIORITY_MAX UCOROUTINE_CONFIG_PRIORITIES

/*!
 * Indicates millisecond period between ticks.
 */
#define UCOROUTINE_MS_PER_TICK (1000 / UCOROUTINE_CONFIG_TICK_HZ)

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_TYPES_H_ */
