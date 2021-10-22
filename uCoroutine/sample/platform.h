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
 * platform.h
 *
 *  Created on: 27.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
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
