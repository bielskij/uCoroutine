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
 * yield.cpp
 *
 *  Created on: 18.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine.h"


struct ContextYield {
	int counter;
	int max;
	std::string order;

	ContextYield(int max) {
		this->counter = 0;
		this->max     = max;
	}

	~ContextYield() {
	}
};


UCOROUTINE_FUNC_BEGIN(yield_routine, ContextYield) {
	while (1) {
		context->order.push_back(self->name[0]);

		if (++context->counter == context->max) {
			uCoroutine_interrupt();
		}

		uCoroutine_yield();
	}
}
UCOROUTINE_FUNC_END;


UCOROUTINE_FUNC_BEGIN(yield_routine_none, ContextYield) {
	while (1) {
		context->order.push_back(self->name[0]);

		uCoroutine_yield();
	}
}
UCOROUTINE_FUNC_END;


TEST(yield, simple) {
	uCoroutine coroutineA;
	uCoroutine coroutineB;
	ContextYield context(3);

	uCoroutine_prepare(&coroutineA, "A", UCOROUTINE_PRIORITY_MIN, yield_routine,      &context);
	uCoroutine_prepare(&coroutineB, "B", UCOROUTINE_PRIORITY_MIN, yield_routine_none, &context);
	uCoroutine_start(&coroutineA);
	uCoroutine_start(&coroutineB);

	uCoroutine_schedule();

	uCoroutine_stop(&coroutineB);
	uCoroutine_stop(&coroutineA);

	ASSERT_EQ(context.counter, context.max);
	ASSERT_EQ(context.order, "ABABA");
}


TEST(yield, priorities) {
	uCoroutine coroutineA;
	uCoroutine coroutineB;
	ContextYield context(3);

	uCoroutine_prepare(&coroutineA, "A", UCOROUTINE_PRIORITY_MAX, yield_routine,      &context);
	uCoroutine_prepare(&coroutineB, "B", UCOROUTINE_PRIORITY_MIN, yield_routine_none, &context);
	uCoroutine_start(&coroutineA);
	uCoroutine_start(&coroutineB);

	uCoroutine_schedule();

	uCoroutine_stop(&coroutineB);
	uCoroutine_stop(&coroutineA);

	ASSERT_EQ(context.counter, context.max);
	ASSERT_EQ(context.order, "AAA");
}


TEST(yield, priorities_2) {
	uCoroutine coroutineA;
	uCoroutine coroutineB;
	uCoroutine coroutineC;
	ContextYield context(3);

	uCoroutine_prepare(&coroutineA, "A", UCOROUTINE_PRIORITY_MAX, yield_routine,      &context);
	uCoroutine_prepare(&coroutineB, "B", UCOROUTINE_PRIORITY_MIN, yield_routine_none, &context);
	uCoroutine_prepare(&coroutineC, "C", UCOROUTINE_PRIORITY_MAX, yield_routine_none, &context);
	uCoroutine_start(&coroutineC);
	uCoroutine_start(&coroutineB);
	uCoroutine_start(&coroutineA);

	uCoroutine_schedule();

	uCoroutine_stop(&coroutineC);
	uCoroutine_stop(&coroutineB);
	uCoroutine_stop(&coroutineA);

	ASSERT_EQ(context.counter, context.max);
	ASSERT_EQ(context.order, "CACACA");
}
