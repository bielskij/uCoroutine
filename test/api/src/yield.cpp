/*
 * yield.cpp
 *
 *  Created on: 18.11.2021
 *      Author: jarko
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
