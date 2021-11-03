/*
 * platform.h
 *
 *  Created on: 27.10.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_PLATFORM_H_
#define UCOROUTINE_PLATFORM_H_

#include "uCoroutine/types.h"

/**
 * This macro/procedure is responsible for enabling global hardware interrupts
 *
 * @see uCoroutine_platform_isr_disable
 * @return nothing
 */
#define uCoroutine_platform_isr_enable()

/**
 * This macro/procedure is responsible for disabling global hardware interrupts
 *
 * @see uCoroutine_platform_isr_enable
 * @return nothing
 */
#define uCoroutine_platform_isr_disable()

/**
 * This macro/function returns current time in ticks.
 *
 * @see uCoroutineTick
 * @return current time in ticks (uCoroutineTick)
 */
#define uCoroutine_platform_getTicks()

#endif /* UCOROUTINE_PLATFORM_H_ */