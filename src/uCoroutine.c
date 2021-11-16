/*
 * uCoroutine.c
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */
#include <string.h>

#include "uCoroutine.h"
#include "uCoroutine/platform.h"

#ifdef UC_DEBUG_LEVEL_LIST
	#define UC_DEBUG_LEVEL UC_DEBUG_LEVEL_UCOROUTINE
#endif
#include "uCoroutine/utils/debug.h"

static List readyCoroutines[UCOROUTINE_CONFIG_PRIORITIES];
// Delayed lists keep coroutines in delayTicks and priority order
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

	// Add new coroutine at the end
	list_insert(&readyCoroutines[coroutine->priority], &coroutine->stateListItem, false);
}


void uCoroutine_initialize(void) {
	UC_ASSERT(IS_NULL(delayedCoroutines));
	UC_ASSERT(IS_NULL(ovfDelayedCoroutines));

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
	delayedCoroutines    = NULL;
	ovfDelayedCoroutines = NULL;
}

#ifdef UCOROUTINE_CONFIG_DYNAMIC_ALLOCATION
uCoroutinePtr uCoroutine_new(
	uCoroutinePriority priority,
	const char        *name,
	uCoroutineFunc     func,
	void              *funcData
) {

}
#endif

void uCoroutine_prepare(
	uCoroutinePtr      coroutine,
	const char        *name,
	uCoroutinePriority priority,
	uCoroutineFunc     func,
	void              *funcData
) {
	UC_ASSERT(NOT_NULL(coroutine));
	UC_ASSERT(priority >= UCOROUTINE_PRIORITY_MIN);
	UC_ASSERT(priority <= UCOROUTINE_PRIORITY_MAX);
	UC_ASSERT(NOT_NULL(func));

	coroutine->func     = func;
	coroutine->funcData = funcData;
	coroutine->priority = priority;

	memset(coroutine->name, 0, sizeof(coroutine->name));

	if (NOT_NULL(name)) {
		strncpy(coroutine->name, name, UCOROUTINE_CONFIG_NAME_LENGTH);
	}

	coroutine->state = UCOROUTINE_STATE_NULL;

	list_initialize(&coroutine->stateListItem);
	list_initialize(&coroutine->eventListItem);
}


void uCoroutine_start(uCoroutinePtr coroutine) {
	UC_ASSERT(NOT_NULL(coroutine));
	UC_ASSERT(NOT_NULL(coroutine->func));
	UC_ASSERT(coroutine->priority >= UCOROUTINE_PRIORITY_MIN);
	UC_ASSERT(coroutine->priority <= UCOROUTINE_PRIORITY_MAX);
	UC_ASSERT(list_isEmpty(&coroutine->eventListItem));
	UC_ASSERT(list_isEmpty(&coroutine->stateListItem));

	coroutine->state = UCOROUTINE_STATE_NULL;

	_addCoRoutineToReadyQueue(coroutine);
}


void uCoroutine_stop(uCoroutinePtr coroutine) {
	UC_ASSERT(NOT_NULL(coroutine));

	list_remove(&coroutine->eventListItem);
	list_remove(&coroutine->stateListItem);
}


void uCoroutine_schedule(void) {
	scheduleInterrupt = false;

	UC_ASSERT(NOT_NULL(delayedCoroutines));
	UC_ASSERT(NOT_NULL(ovfDelayedCoroutines));

	while (! scheduleInterrupt) {
		uCoroutine *coroutine;

		// Check pending ready coroutines. They are added to the list from
		// ISR routines.
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

			while (lastTickPeriod--) {
				currentTickCount++;

				// Time overflowed. Swap delayed coroutines lists.
				if (currentTickCount == 0) {
					List *tmp = delayedCoroutines;

					delayedCoroutines    = ovfDelayedCoroutines;
					ovfDelayedCoroutines = tmp;
				}

				while (! list_isEmpty(delayedCoroutines)) {
					coroutine = list_first(delayedCoroutines, uCoroutine, stateListItem);

					if (currentTickCount < coroutine->delayTicks) {
						// Timeout has not yet expired. List is sorted.
						break;
					}

					// Timeout occurred - move the coroutine to ready list.
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
			// Get first available coroutine
			currentCoroutine = list_first(&readyCoroutines[topReadyPriority], uCoroutine, stateListItem);

			// Move it on back in ready queue
			list_remove(&currentCoroutine->stateListItem);
			list_insert(&readyCoroutines[topReadyPriority], &currentCoroutine->stateListItem, false);

			// Execute coroutine function and save new state
			currentCoroutine->state = currentCoroutine->func(currentCoroutine, currentCoroutine->funcData);
		}
	}
}


void uCoroutine_interrupt(void) {
	scheduleInterrupt = true;
}


bool _uCoroutine_wakeup(List *eventList) {
	uCoroutine *wokenCoroutine = list_first(eventList, uCoroutine, eventListItem);

	list_remove(&wokenCoroutine->eventListItem);

	list_insert(&pendingReadyCoroutines, &wokenCoroutine->eventListItem, false);

	if (wokenCoroutine->priority >= currentCoroutine->priority) {
		return true;
	}

	return false;
}


void _uCoroutine_suspend(uCoroutineTick timeout, List *eventList) {
	uCoroutine *coroutine = currentCoroutine;

	list_remove(&coroutine->stateListItem);

	if (timeout != UCOROUTINE_INFINITY) {
		coroutine->delayTicks = currentTickCount + timeout;

		{
			List *destList;

			if (coroutine->delayTicks < currentTickCount) {
				destList = ovfDelayedCoroutines;

			} else {
				destList = delayedCoroutines;
			}

			// Insert new node in correct order (time and priorities)
			list_for_each(destList) {
				uCoroutine *tmp = container_of(it, uCoroutine, stateListItem);

				if (coroutine->delayTicks == tmp->delayTicks) {
					// follow priorities.
					if (coroutine->priority > tmp->delayTicks) {
						list_insert(it, &coroutine->stateListItem, false);
						break;
					}

				} else if (coroutine->delayTicks < tmp->delayTicks) {
					list_insert(it, &coroutine->stateListItem, false);
					break;
				}
			}

			// Just push at the end if it was not inserted in previous step.
			if (list_isEmpty(&( coroutine->stateListItem ))) {
				list_insert(destList, &coroutine->stateListItem, false);
			}
		}
	}

	if (NOT_NULL(eventList)) {
		list_remove(&coroutine->eventListItem);

		list_for_each(eventList) {
			uCoroutine *tmp = container_of(it, uCoroutine, eventListItem);

			if (coroutine->priority > tmp->priority) {
				list_insert(it, &coroutine->eventListItem, false);
				break;
			}
		}

		if (list_isEmpty(&coroutine->eventListItem)) {
			list_insert(eventList, &coroutine->eventListItem, false);
		}
	}
}
