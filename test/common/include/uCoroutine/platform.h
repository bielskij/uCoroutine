/*
 * platform.h
 *
 *  Created on: 27.10.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_PLATFORM_H_
#define UCOROUTINE_PLATFORM_H_

#include <stdio.h>

#include "uCoroutine/types.h"

#define uCoroutine_platform_isr_enable()  linux_isr_set(true)
#define uCoroutine_platform_isr_disable() linux_isr_set(false)

#define uCoroutine_platform_getTicks()    linux_get_ticks()

#define uCoroutine_platform_malloc(size)  linux_malloc((size))
#define uCoroutine_platform_free(ptr)     linux_free((ptr))

#define uCoroutine_platform_abort() linux_abort()

#define uCoroutine_platform_printf printf

#ifdef __cplusplus
extern "C" {
#endif

void  linux_isr_set(bool enable);
void  linux_abort();
void *linux_malloc(size_t size);
void  linux_free(void *ptr);

uCoroutineTick linux_get_ticks(void);

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_PLATFORM_H_ */
