/*
 * platform.cpp
 *
 *  Created on: 03.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#include "uCoroutine/platform.h"


static pthread_mutex_t isrGurad = PTHREAD_MUTEX_INITIALIZER;
static struct timespec baseTime = { 0, 0 };


#define timespecsub(tsp, usp, vsp)                        \
	do {                                                  \
		(vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;    \
		(vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec; \
		if ((vsp)->tv_nsec < 0) {                         \
			(vsp)->tv_sec--;                              \
			(vsp)->tv_nsec += 1000000000L;                \
		}                                                 \
	} while (0)


void linux_isr_set(bool enable) {
	if (enable) {
		pthread_mutex_unlock(&isrGurad);

	} else {
		pthread_mutex_lock(&isrGurad);
	}
}


uCoroutineTick linux_get_ticks(void) {
	struct timespec currTime;
	struct timespec diffTime;

	if ((baseTime.tv_sec == 0) && (baseTime.tv_nsec == 0)) {
		clock_gettime(CLOCK_MONOTONIC, &baseTime);
	}

	clock_gettime(CLOCK_MONOTONIC, &currTime);

	timespecsub(&currTime, &baseTime, &diffTime);

	return (diffTime.tv_sec * 1000) + (diffTime.tv_nsec / 1000000);
}


void linux_abort() {
	abort();
}
