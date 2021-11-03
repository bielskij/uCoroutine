/*
 * uCoroutine.c
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */


#include "uCoroutine.h"


static SList readyCoroutines[UCOROUTINE_CONFIG_PRIORITIES];
static SList delayedCoroutineLists[2];

static SList pendingReadyCoroutines;
static SList *delayedCoroutines    = NULL;
static SList *ovfDelayedCoroutines = NULL;

static uCoroutine *currentCoroutine = NULL;


void uCoroutine_initialize(void) {
	delayedCoroutines    = &delayedCoroutineLists[0];
	ovfDelayedCoroutines = &delayedCoroutineLists[1];

	for (uCoroutinePriority p = UCOROUTINE_PRIORITY_MIN; p <= UCOROUTINE_PRIORITY_MAX; p++) {
		slist_initialize(&readyCoroutines[p - UCOROUTINE_PRIORITY_MIN]);
	}

	slist_initialize(&pendingReadyCoroutines);

	slist_initialize(delayedCoroutines);
	slist_initialize(ovfDelayedCoroutines);
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
	coroutine->pc       = UCOROUTINE_STATE_NULL;
	coroutine->func     = func;
	coroutine->funcData = funcData;

	slist_initialize(&coroutine->stateListItem);
	slist_initialize(&coroutine->eventListItem);
}


void uCoroutine_start(uCoroutinePtr coroutine) {
	coroutine->pc = UCOROUTINE_STATE_NULL;
}


void uCoroutine_stop(uCoroutinePtr coroutine) {
}


void uCoroutine_schedule(void) {

}


void uCoroutine_interrupt(void) {

}

/* Internals */
void _uCoroutine_sleepTicks(uCoroutineTick ticks) {

}
