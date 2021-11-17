/*
 * sleep.cpp
 *
 *  Created on: 17.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine.h"


struct Context {
	int counter;
	int max;

	Context(int max) {
		this->counter = 0;
		this->max     = max;
	}
};


UCOROUTINE_FUNC_BEGIN(sleep_interval_sec, Context) {
	while (1) {
		uCoroutine_sleep(1);

		if (++context->counter == context->max) {
			uCoroutine_interrupt();
			uCoroutine_yield();
		}
	}
}
UCOROUTINE_FUNC_END;


TEST(sleep, interval_sec) {
	uCoroutine coroutine;
	Context    context(3);

	uCoroutine_prepare(&coroutine, "test", UCOROUTINE_PRIORITY_MIN, sleep_interval_sec, &context);
	uCoroutine_start(&coroutine);

	uCoroutineTick start = uCoroutine_platform_getTicks();
	{
		uCoroutine_schedule();
	}
	uCoroutineTick end   = uCoroutine_platform_getTicks();

	uCoroutine_stop(&coroutine);

	ASSERT_EQ(context.counter, context.max);
	ASSERT_GE(end - start, UC_MS_TO_TICKS(3000));
	ASSERT_LT(end - start, UC_MS_TO_TICKS(3010));
}


UCOROUTINE_FUNC_BEGIN(sleep_interval_ms, Context) {
	while (1) {
		uCoroutine_sleepMs(100);

		if (++context->counter == context->max) {
			uCoroutine_interrupt();
			uCoroutine_yield();
		}
	}
}
UCOROUTINE_FUNC_END;


TEST(sleep, interval_ms) {
	uCoroutine coroutine;
	Context    context(8);

	uCoroutine_prepare(&coroutine, "test", UCOROUTINE_PRIORITY_MIN, sleep_interval_ms, &context);
	uCoroutine_start(&coroutine);

	uCoroutineTick start = uCoroutine_platform_getTicks();
	{
		uCoroutine_schedule();
	}
	uCoroutineTick end   = uCoroutine_platform_getTicks();

	uCoroutine_stop(&coroutine);

	ASSERT_EQ(context.counter, context.max);
	ASSERT_GE(end - start, UC_MS_TO_TICKS(800));
	ASSERT_LT(end - start, UC_MS_TO_TICKS(810));
}
