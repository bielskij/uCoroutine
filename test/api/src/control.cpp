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
 * control.cpp
 *
 *  Created on: 18.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine.h"


struct ContextControl {
	int counter;
	int startSlaveDelay;
	int stopTestDelay;

	std::string order;

	uCoroutine slaveCoroutine;
	uCoroutine masterCoroutine;

	ContextControl(int startSlaveDelay, int stopTestDelay) {
		this->counter         = 0;
		this->startSlaveDelay = startSlaveDelay;
		this->stopTestDelay   = stopTestDelay;
	}
};


UCOROUTINE_FUNC_BEGIN(control_start_master_routine, ContextControl) {
	while (1) {
		uCoroutine_sleepMs(100);

		context->order.push_back(self->name[0]);

		if (++context->counter == context->startSlaveDelay) {
			uCoroutine_start(&context->slaveCoroutine);
		}

		if (context->counter == context->stopTestDelay) {
			uCoroutine_interrupt();
			uCoroutine_yield();
		}
	}
}
UCOROUTINE_FUNC_END;


UCOROUTINE_FUNC_BEGIN(control_stop_master_routine, ContextControl) {
	while (1) {
		uCoroutine_sleepMs(100);

		context->order.push_back(self->name[0]);

		if (++context->counter == context->startSlaveDelay) {
			uCoroutine_stop(&context->slaveCoroutine);
		}

		if (context->counter == context->stopTestDelay) {
			uCoroutine_interrupt();
			uCoroutine_yield();
		}
	}
}
UCOROUTINE_FUNC_END;


UCOROUTINE_FUNC_BEGIN(control_slave, ContextControl) {
	while (1) {
		context->order.push_back(self->name[0]);

		uCoroutine_sleepMs(200);
	}
}
UCOROUTINE_FUNC_END;


TEST(control_start, simple) {
	ContextControl context(4, 8);

	uCoroutine_prepare(&context.masterCoroutine, "A", UCOROUTINE_PRIORITY_MIN, control_start_master_routine, &context);
	uCoroutine_prepare(&context.slaveCoroutine,  "B", UCOROUTINE_PRIORITY_MIN, control_slave,  &context);
	uCoroutine_start(&context.masterCoroutine);

	uCoroutine_schedule();

	uCoroutine_stop(&context.masterCoroutine);
	uCoroutine_stop(&context.slaveCoroutine);

	ASSERT_EQ(context.counter, context.stopTestDelay);
	ASSERT_EQ(context.order, "AAAABABAABA");
}


TEST(control_stop, simple) {
	ContextControl context(4, 8);

	uCoroutine_prepare(&context.masterCoroutine, "A", UCOROUTINE_PRIORITY_MIN, control_stop_master_routine, &context);
	uCoroutine_prepare(&context.slaveCoroutine,  "B", UCOROUTINE_PRIORITY_MIN, control_slave,  &context);
	uCoroutine_start(&context.masterCoroutine);
	uCoroutine_start(&context.slaveCoroutine);

	uCoroutine_schedule();

	uCoroutine_stop(&context.masterCoroutine);

	ASSERT_EQ(context.counter, context.stopTestDelay);
	ASSERT_EQ(context.order, "BABAABAAAAA");
}
