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
	uCoroutineQueue queue;
	uint8_t         queueMemory[8];

	Context() {
		this->val       = 0;
		this->condition = true;

		uCoroutine_queue_init(&this->queue, this->queueMemory, 1, sizeof(this->queueMemory));
	}
};


UCOROUTINE_FUNC_BEGIN(basic_routine_master, Context) {
	while (1) {
		uCoroutine_sleep(1);
		uCoroutine_sleepMs(100);
		uCoroutine_sleepTicks(UC_MS_TO_TICKS(100));
		uCoroutine_yield();

		{
			uCoroutineError sendRet;
			uint8_t val;

			val = 0xa5;
			uCoroutine_queue_send(&context->queue, &val, UC_MS_TO_TICKS(100), &sendRet);

			if (sendRet == UC_NO_ERROR) {
			}
		}
	}
}
UCOROUTINE_FUNC_END;


UCOROUTINE_FUNC_BEGIN(basic_routine_slave, Context) {
	while (1) {
		uCoroutine_sleepMs(100);

		{
			uCoroutineError recvRet;

			uCoroutine_queue_receive(&context->queue, &context->val, UCOROUTINE_INFINITY, &recvRet);

			if (recvRet == UC_NO_ERROR) {

			}
		}

		uCoroutine_interrupt();
	}
}
UCOROUTINE_FUNC_END;


TEST(static_api, basic) {
	uCoroutine master;
	uCoroutine slave;

	Context ctx;

	uCoroutine_prepare(&master, UCOROUTINE_PRIORITY_MIN, basic_routine_master, &ctx);
	uCoroutine_prepare(&slave,  UCOROUTINE_PRIORITY_MIN, basic_routine_slave,  &ctx);

	uCoroutine_start(&master);
	uCoroutine_start(&slave);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&slave);
	uCoroutine_stop(&master);

	ASSERT_EQ(ctx.val, 0xa5);
}
