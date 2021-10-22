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


TEST(api, basic) {
	Context ctx;

	uCoroutine master;
	uCoroutine *slave = uCoroutine_new(UCOROUTINE_PRIORITY_MIN, "slave", basic_routine_slave, &ctx);

	uCoroutine_prepare(&master, "master", UCOROUTINE_PRIORITY_MIN, basic_routine_master, &ctx);

	uCoroutine_start(&master);
	uCoroutine_start(slave);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(slave);
	uCoroutine_stop(&master);

	uCoroutine_free(slave);

	ASSERT_EQ(ctx.val, 0xa5);
}
