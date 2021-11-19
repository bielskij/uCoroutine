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


UCOROUTINE_FUNC_BEGIN(control_start_slave_routine, ContextControl) {
	while (1) {
		context->order.push_back(self->name[0]);

		uCoroutine_sleepMs(200);
	}
}
UCOROUTINE_FUNC_END;


TEST(control_start, simple) {
	ContextControl context(4, 8);

	uCoroutine_prepare(&context.masterCoroutine, "A", UCOROUTINE_PRIORITY_MIN, control_start_master_routine, &context);
	uCoroutine_prepare(&context.slaveCoroutine,  "B", UCOROUTINE_PRIORITY_MIN, control_start_slave_routine,  &context);
	uCoroutine_start(&context.masterCoroutine);

	uCoroutine_schedule();

	uCoroutine_stop(&context.masterCoroutine);
	uCoroutine_stop(&context.slaveCoroutine);

	ASSERT_EQ(context.counter, context.stopTestDelay);
	ASSERT_EQ(context.order, "AAAABABAABA");
}
