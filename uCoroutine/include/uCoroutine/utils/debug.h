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
 * debug.h
 *
 *  Created on: 15.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_UTILS_DEBUG_H_
#define UCOROUTINE_UTILS_DEBUG_H_

#include "uCoroutine/utils/debug/levels.h"

#include "uCoroutine/config.h"
#include "uCoroutine/platform.h"

// Forced debug level (globally)
#if defined(UC_FORCE_DEBUG_LEVEL)
	#if defined(UC_DEBUG_LEVEL)
		#undef UC_DEBUG_LEVEL
	#endif

	#define UC_DEBUG_LEVEL UC_FORCE_DEBUG_LEVEL
#endif

// Default debug level
#if !defined(UC_DEBUG_LEVEL)
	#define UC_DEBUG_LEVEL UC_DEBUG_LEVEL_WRN
#endif

#ifdef UC_DEBUG
	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_TRC
		#define UC_TRC(x) do { \
			uCoroutine_platform_printf("[TRC %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_platform_printf x; \
			uCoroutine_platform_printf("\r\n"); \
		} while(0);
	#else
		#define UC_TRC(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_DBG
		#define UC_DBG(x) do { \
			uCoroutine_platform_printf("[DBG %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_platform_printf x; \
			uCoroutine_platform_printf("\r\n"); \
		} while(0);
	#else
		#define UC_DBG(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_LOG
		#define UC_LOG(x) do { \
			uCoroutine_platform_printf("[LOG %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_platform_printf x; \
			uCoroutine_platform_printf("\r\n"); \
		} while(0);
	#else
		#define UC_LOG(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_WRN
		#define UC_WARN(x) do { \
			uCoroutine_platform_printf("[WRN %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_platform_printf x; \
			uCoroutine_platform_printf("\r\n"); \
		} while(0);
	#else
		#define UC_WARN(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_ERR
		#define UC_ERR(x) do { \
			uCoroutine_platform_printf("[ERR %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_platform_printf x; \
			uCoroutine_platform_printf("\r\n"); \
		} while(0);
	#else
		#define UC_ERR(x) do { } while (0);
	#endif

#else
	#define UC_TRC(x)  do {} while (0);
	#define UC_DBG(x)  do {} while (0);
	#define UC_LOG(x)  do {} while (0);
	#define UC_WARN(x) do {} while (0);
	#define UC_ERR(x)  do {} while (0);
#endif

#if defined(UC_ASSERT_API)
	#define UC_ASSERT(x) do { \
		if (! (x)) { \
			uCoroutine_platform_printf("ASSERT (%s:%d) ! (" #x ") != TRUE\r\n", __FILE__, __LINE__); \
			uCoroutine_platform_abort(); \
		} \
	} while (0);
#else
	#define UC_ASSERT(x) do { } while (0);
#endif

#endif /* UCOROUTINE_UTILS_DEBUG_H_ */
