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
			uCoroutine_debug_printf("[TRC %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_debug_printf x; \
			uCoroutine_debug_printf("\r\n"); \
		} while(0);
	#else
		#define UC_TRC(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_DBG
		#define UC_DBG(x) do { \
			uCoroutine_debug_printf("[DBG %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_debug_printf x; \
			uCoroutine_debug_printf("\r\n"); \
		} while(0);
	#else
		#define UC_DBG(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_LOG
		#define UC_LOG(x) do { \
			uCoroutine_debug_printf("[LOG %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_debug_printf x; \
			uCoroutine_debug_printf("\r\n"); \
		} while(0);
	#else
		#define UC_LOG(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_WRN
		#define UC_WARN(x) do { \
			uCoroutine_debug_printf("[WRN %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_debug_printf x; \
			uCoroutine_debug_printf("\r\n"); \
		} while(0);
	#else
		#define UC_WARN(x) do { } while (0);
	#endif

	#if UC_DEBUG_LEVEL >= UC_DEBUG_LEVEL_ERR
		#define UC_ERR(x) do { \
			uCoroutine_debug_printf("[ERR %s:%d]: ", __FUNCTION__, __LINE__); \
			uCoroutine_debug_printf x; \
			uCoroutine_debug_printf("\r\n"); \
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
			uCoroutine_debug_printf("ASSERT (%s:%d) ! (" #x ") != TRUE\r\n", __FILE__, __LINE__); \
			uCoroutine_abort(); \
		} \
	} while (0);
#else
	#define UC_ASSERT(x) do { } while (0);
#endif

#endif /* UCOROUTINE_UTILS_DEBUG_H_ */
