/*
 * utils.h
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_UTILS_H_
#define UCOROUTINE_UTILS_H_

#if !defined(NOT_NULL)
	#define NOT_NULL(x) ((x) != ((void *) 0))
#endif

#if !defined(IS_NULL)
	#define IS_NULL(x) ((x) == ((void *) 0))
#endif

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define array_size(array) ((size_t)(sizeof(array) / sizeof((array)[0])))

#endif /* UCOROUTINE_UTILS_H_ */
