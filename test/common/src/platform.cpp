////////////////////////////////////////////////////////////////////////////////
// MIT License
// 
// Copyright (c) 2021 Jaroslaw Bielski (bielski.j@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////
/*
 * platform.cpp
 *
 *  Created on: 03.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

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


void *linux_malloc(size_t size) {
	return malloc(size);
}


void linux_free(void *ptr) {
	free(ptr);
}

extern "C" {
void uCoroutine_idle_callback(void) {
	usleep(1 * 1000);
}
}
