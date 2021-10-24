/*
 * types.h
 *
 *  Created on: 22.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_TYPES_H_
#define UCOROUTINE_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef __cplusplus
	#include <stddef.h>

	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
		#include <stdbool.h>
	#else
		#ifndef bool
			#define bool char
			#define true  (1 == 1)
			#define false (! true)
		#endif /* bool */
	#endif /* __STDC_VERSION__ */
#endif

#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_TYPES_H_ */
