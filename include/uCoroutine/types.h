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

typedef struct _uCoroutine  uCoroutine;
typedef struct _uCoroutine *uCoroutinePtr;

typedef uint16_t (*uCoroutineFunc)(uCoroutinePtr self, void *coroutineData);

typedef uint8_t  uCoroutinePriority;

typedef uint32_t uCoroutineTick;

#define UCOROUTINE_PRIORITY_MIN 0
#define UCOROUTINE_PRIORITY_MAX UCOROUTINE_CONFIG_PRIORITIES

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_TYPES_H_ */
