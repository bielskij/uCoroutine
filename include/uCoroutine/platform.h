/*
 * platform.h
 *
 *  Created on: 27.10.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_PLATFORM_H_
#define UCOROUTINE_PLATFORM_H_

#include "uCoroutine/types.h"


void uCoroutine_platform_isr_enable(void);

void uCoroutine_platform_isr_disable(void);

uCoroutineTick uCoroutine_platform_getTicks(void);

#endif /* UCOROUTINE_PLATFORM_H_ */
