/*
 * uCoroutine.c
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */


#include "uCoroutine.h"
#include "uCoroutine/platform.h"


static List readyCoroutines[UCOROUTINE_CONFIG_PRIORITIES];
static List delayedCoroutineLists[2];

static List  pendingReadyCoroutines;
static List *delayedCoroutines    = NULL;
static List *ovfDelayedCoroutines = NULL;

static bool               scheduleInterrupt = false;
static uCoroutinePriority topReadyPriority  = UCOROUTINE_PRIORITY_MIN;
static uCoroutineTick     currentTickCount  = 0;
static uCoroutine        *currentCoroutine  = NULL;


static void _addCoRoutineToReadyQueue(uCoroutine *coroutine) {
	if (coroutine->priority > topReadyPriority) {
		topReadyPriority = coroutine->priority;
	}

	list_insert(&readyCoroutines[coroutine->priority], &coroutine->stateListItem, false);
}


static void _addCoRoutineToDelayQueue(uCoroutine *coroutine, uCoroutineTick delay, List *eventList) {
	list_remove(&coroutine->stateListItem);

	coroutine->delayTicks = currentTickCount + delay;

	if (coroutine->delayTicks < currentTickCount) {
		list_insert(ovfDelayedCoroutines, &coroutine->stateListItem, false);

	} else {
		list_insert(delayedCoroutines, &coroutine->stateListItem, false);
	}

	if (NOT_NULL(eventList)) {
		list_insert(eventList, &coroutine->eventListItem, false);
	}
}


void uCoroutine_initialize(void) {
	delayedCoroutines    = &delayedCoroutineLists[0];
	ovfDelayedCoroutines = &delayedCoroutineLists[1];

	for (uCoroutinePriority p = UCOROUTINE_PRIORITY_MIN; p <= UCOROUTINE_PRIORITY_MAX; p++) {
		list_initialize(&readyCoroutines[p - UCOROUTINE_PRIORITY_MIN]);
	}

	list_initialize(&pendingReadyCoroutines);

	list_initialize(delayedCoroutines);
	list_initialize(ovfDelayedCoroutines);
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
	coroutine->func     = func;
	coroutine->funcData = funcData;
	coroutine->priority = priority;

	coroutine->state = UCOROUTINE_STATE_NULL;

	list_initialize(&coroutine->stateListItem);
	list_initialize(&coroutine->eventListItem);
}


void uCoroutine_start(uCoroutinePtr coroutine) {
	coroutine->state = UCOROUTINE_STATE_NULL;

	_addCoRoutineToReadyQueue(coroutine);
}


void uCoroutine_stop(uCoroutinePtr coroutine) {
	list_remove(&coroutine->eventListItem);
	list_remove(&coroutine->stateListItem);
}


void uCoroutine_schedule(void) {
	scheduleInterrupt = false;

	while (! scheduleInterrupt) {
		uCoroutine *coroutine;

		// Check pending ready coroutines
		while (! list_isEmpty(&pendingReadyCoroutines)) {
			uCoroutine_platform_isr_disable();
			{
				coroutine = list_first(&pendingReadyCoroutines, uCoroutine, eventListItem);

				list_remove(&coroutine->eventListItem);
			}
			uCoroutine_platform_isr_enable();

			list_remove(&coroutine->stateListItem);

			_addCoRoutineToReadyQueue(coroutine);
		}

		// Check delayed coroutines
		{
			uCoroutineTick lastTickPeriod = uCoroutine_platform_getTicks() - currentTickCount;

			while (lastTickPeriod) {
				currentTickCount++;
				lastTickPeriod--;

				// Time overflowed. Swap delayed coroutines lists.
				if (currentTickCount == 0) {
					List *tmp = delayedCoroutines;

					delayedCoroutines    = ovfDelayedCoroutines;
					ovfDelayedCoroutines = tmp;
				}

				while (! list_isEmpty(delayedCoroutines)) {
					coroutine = list_first(delayedCoroutines, uCoroutine, stateListItem);

					if (currentTickCount < coroutine->delayTicks) {
						// timeout has not yet expired
						break;
					}

					uCoroutine_platform_isr_disable();
					{
						list_remove(&coroutine->stateListItem);
						list_remove(&coroutine->eventListItem);
					}
					uCoroutine_platform_isr_enable();

					_addCoRoutineToReadyQueue(coroutine);
				}
			}
		}

		// Find the highest priority queue that contains ready co-routines.
		while (list_isEmpty(&readyCoroutines[topReadyPriority])) {
			if (topReadyPriority == UCOROUTINE_PRIORITY_MIN) {
				break; // no more coroutines to check
			}

			--topReadyPriority;
		}

		if (! list_isEmpty(&readyCoroutines[topReadyPriority])) {
			currentCoroutine = list_first(&readyCoroutines[topReadyPriority], uCoroutine, stateListItem);

			list_remove(&currentCoroutine->stateListItem);
			list_insert(&readyCoroutines[topReadyPriority], &currentCoroutine->stateListItem, false);

			currentCoroutine->state = currentCoroutine->func(currentCoroutine, currentCoroutine->funcData);
		}
	}
}


void uCoroutine_interrupt(void) {
	scheduleInterrupt = true;
}

/* Internals */
void _uCoroutine_sleepTicks(uCoroutineTick ticks) {
	_addCoRoutineToDelayQueue(currentCoroutine, ticks, NULL);
}
