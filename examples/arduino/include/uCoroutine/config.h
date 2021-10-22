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
#define UCOROUTINE_CONFIG_PRIORITIES  2

/*!
 * Maximal length of coroutine name.
 */
#define UCOROUTINE_CONFIG_NAME_LENGTH 0

/*!
 * Number of ticks per second.
 */
#define UCOROUTINE_CONFIG_TICK_HZ     1000

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
//#define UC_ASSERT_API

/*!
 * Enables debug messages in uCoroutine subsystem (optional).
 */
//#define UC_DEBUG

/*!
 * Forces debug level globally (optional)
 */
#define UC_FORCE_DEBUG_LEVEL UC_DEBUG_LEVEL_DBG

/*!
 * Each module debug level
 */
#ifdef UC_DEBUG
	#define UC_DEBUG_LEVEL_LIST       UC_DEBUG_LEVEL_WRN
	#define UC_DEBUG_LEVEL_SLIST      UC_DEBUG_LEVEL_WRN
	#define UC_DEBUG_LEVEL_UCOROUTINE UC_DEBUG_LEVEL_WRN
#endif

#endif /* SAMPLE_CONFIG_H_ */
