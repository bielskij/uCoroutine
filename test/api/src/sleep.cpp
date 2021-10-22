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
 * sleep.cpp
 *
 *  Created on: 17.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine.h"


struct ContextSleep {
	int counter;
	int max;

	ContextSleep(int max) {
		this->counter = 0;
		this->max     = max;
	}
};


UCOROUTINE_FUNC_BEGIN(sleep_interval_sec, ContextSleep) {
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
	ContextSleep context(3);

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


UCOROUTINE_FUNC_BEGIN(sleep_interval_ms, ContextSleep) {
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
	ContextSleep context(8);

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


UCOROUTINE_FUNC_BEGIN(sleep_interval_ticks, ContextSleep) {
	while (1) {
		uCoroutine_sleepTicks(UC_MS_TO_TICKS(100));

		if (++context->counter == context->max) {
			uCoroutine_interrupt();
			uCoroutine_yield();
		}
	}
}
UCOROUTINE_FUNC_END;


TEST(sleep, interval_ticks) {
	uCoroutine coroutine;
	ContextSleep context(8);

	uCoroutine_prepare(&coroutine, "test", UCOROUTINE_PRIORITY_MIN, sleep_interval_ticks, &context);
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
