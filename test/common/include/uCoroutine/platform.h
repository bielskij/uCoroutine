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

#define uCoroutine_abort() linux_abort()

#define uCoroutine_debug_printf printf

#ifdef __cplusplus
extern "C" {
#endif

void linux_isr_set(bool enable);
void linux_abort();

uCoroutineTick linux_get_ticks(void);

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_PLATFORM_H_ */
