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

#endif /* SAMPLE_CONFIG_H_ */
