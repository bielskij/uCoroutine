/*
 * uCoroutine.c
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */


#include "uCoroutine.h"


void uCoroutine_initialize(void) {

}


void uCoroutine_terminate(void) {

}

#ifdef UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION
uCoroutinePtr uCoroutine_new() {

}
#endif

void uCoroutine_configure(
	uCoroutinePtr      coroutine,
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
) {
	coroutine->pc       = UCOROUTINE_PC_INIT;
	coroutine->func     = func;
	coroutine->funcData = funcData;
}


void uCoroutine_start(uCoroutinePtr coroutine) {
	coroutine->pc = UCOROUTINE_PC_INIT;
}


void uCoroutine_stop(uCoroutinePtr coroutine) {
}


void uCoroutine_schedule(void) {

}


void uCoroutine_interrupt(void) {

}
