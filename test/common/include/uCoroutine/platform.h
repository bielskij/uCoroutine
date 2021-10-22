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
