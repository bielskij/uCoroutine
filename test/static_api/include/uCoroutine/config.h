/*
 * config.h
 *
 *  Created on: 23.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef SAMPLE_CONFIG_H_
#define SAMPLE_CONFIG_H_

/*
 * ====================================
 * uCoroutine features
 * ====================================
 */

/*!
 * Enables dynamic allocation of coroutines.
 */
//#define UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION

/*
 * ====================================
 * uCoroutine internal configuration
 * ====================================
 */

/*!
 * Number of coroutine priorities (1 - N)
 */
#define UCOROUTINE_CONFIG_PRIORITIES 1

#define UCOROUTINE_CONFIG_TICK_HZ    1000

/*
 * ====================================
 * uCoroutine debug and diagnostics
 * configuration
 * ====================================
 */

/*!
 * Enables ASSERTs on parameters, state etc in all API calls.
 * This is useful during development (optional).
 */
#define UC_ASSERT_API

/*!
 * Enables debug messages in uCoroutine subsystem (optional).
 */
#define UC_DEBUG

/*!
 * Forces debug level globally (optional)
 */
#define UC_FORCE_DEBUG_LEVEL UC_DEBUG_LEVEL_DBG

/*!
 * Each module debug level
 */
#ifdef UC_DEBUG
	#define UC_DEBUG_LEVEL_LIST UC_DEBUG_LEVEL_WRN
#endif

#endif /* SAMPLE_CONFIG_H_ */
