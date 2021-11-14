/*
 * test.cpp
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine.h"


struct Context {
	int  val;
	bool condition;

	Context() {
		this->val       = 0;
		this->condition = true;
	}
};


UCOROUTINE_FUNC_BEGIN(basic_routine, Context) {
	while (1) {
		uCoroutine_sleep(1);
		uCoroutine_sleepMs(100);
		uCoroutine_sleepTicks(UC_MS_TO_TICKS(100));
		uCoroutine_yield();
		uCoroutine_interrupt();

		context->val = 1;
	}
}
UCOROUTINE_FUNC_END;



TEST(static_api, basic) {
	uCoroutine c;

	Context ctx;

	uCoroutine_configure(&c, UCOROUTINE_PRIORITY_MIN, basic_routine, &ctx);

	uCoroutine_start(&c);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&c);

	ASSERT_EQ(ctx.val, 1);
}
