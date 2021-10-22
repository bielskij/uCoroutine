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
