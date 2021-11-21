/*
 * platform.h
 *
 *  Created on: 27.10.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_PLATFORM_H_
#define UCOROUTINE_PLATFORM_H_

#include <avr/interrupt.h>

#include "uCoroutine/types.h"


void uCoroutine_platform_initialize();

/**
 * This macro/procedure is responsible for enabling global hardware interrupts
 *
 * @see uCoroutine_platform_isr_disable
 * @return nothing
 */
#define uCoroutine_platform_isr_enable() sei()

/**
 * This macro/procedure is responsible for disabling global hardware interrupts
 *
 * @see uCoroutine_platform_isr_enable
 * @return nothing
 */
#define uCoroutine_platform_isr_disable() cli()

/**
 * This macro/function returns current time in ticks.
 *
 * @see uCoroutineTick
 * @return current time in ticks (uCoroutineTick)
 */
uCoroutineTick uCoroutine_platform_getTicks();

/**
 * Allocates block of memory of size 'size' on heap.
 *
 * @param size size of memory block to allocate
 * @return pointer to allocated memory
 */
#define uCoroutine_platform_malloc(size)

/**
 * Disposes a memory block allocated by uCoroutine_platform_malloc()
 *
 * @param ptr pointer to memory block.
 */
#define uCoroutine_platform_free(ptr)     linux_free((ptr))

/**
 * This macro/function is responsible for wrapping printf() standard function.
 * On some platforms custom or particular implementation might be used.
 */
#define uCoroutine_platform_printf()

/**
 * This macro/procedure aborts current execution. It should finally do reset whole platform.
 */
#define uCoroutine_platform_abort()

#endif /* UCOROUTINE_PLATFORM_H_ */
