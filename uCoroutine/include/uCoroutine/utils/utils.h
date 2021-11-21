/*
 * utils.h
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_UTILS_UTILS_H_
#define UCOROUTINE_UTILS_UTILS_H_

#if !defined(NOT_NULL)
	#define NOT_NULL(x) ((x) != ((void *) 0))
#endif

#if !defined(IS_NULL)
	#define IS_NULL(x) ((x) == ((void *) 0))
#endif

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - (char *)(&((type *)0)->member)))

#define array_size(array) ((size_t)(sizeof(array) / sizeof((array)[0])))

#define UC_MS_TO_TICKS(_ms)   ((uCoroutineTick)((_ms) / UCOROUTINE_MS_PER_TICK))
#define UC_SEC_TO_TICKS(_sec) UC_MS_TO_TICKS((_sec) * 1000)

#endif /* UCOROUTINE_UTILS_UTILS_H_ */
