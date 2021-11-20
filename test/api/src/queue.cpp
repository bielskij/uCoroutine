/*
 * queue.cpp
 *
 *  Created on: 20.11.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */
#include <gtest/gtest.h>

#include "uCoroutine.h"


struct ContextQueueTimeout {
	uCoroutineQueue queue;
	char            queueStorage[4];

	uCoroutineError queueRet;
	uCoroutineTick  timeout;
	uCoroutineTick  startTime;
	uCoroutineTick  endTime;
};


UCOROUTINE_FUNC_BEGIN(queue_recv_timeout, ContextQueueTimeout) {
	uCoroutine_queue_init(&context->queue, context->queueStorage, 1, sizeof(context->queueStorage));

	context->startTime = uCoroutine_platform_getTicks();
	{
		char item;

		uCoroutine_queue_receive(&context->queue, &item, context->timeout, &context->queueRet);
	}
	context->endTime = uCoroutine_platform_getTicks();

	uCoroutine_interrupt();
	uCoroutine_yield();
}
UCOROUTINE_FUNC_END;


TEST(queue, receive_timeout) {
	ContextQueueTimeout ctx;
	uCoroutine coroutine;

	ctx.timeout = UC_SEC_TO_TICKS(1);

	uCoroutine_prepare(&coroutine, "a", UCOROUTINE_PRIORITY_MIN, queue_recv_timeout, &ctx);

	uCoroutine_start(&coroutine);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&coroutine);

	ASSERT_EQ(ctx.queueRet, UC_ERROR_EMPTY);
	ASSERT_GE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(1000));
	ASSERT_LE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(1100));
}


TEST(queue, receive_timeout_immediate) {
	ContextQueueTimeout ctx;
	uCoroutine coroutine;

	ctx.timeout = UCOROUTINE_IMMEDIATE;

	uCoroutine_prepare(&coroutine, "a", UCOROUTINE_PRIORITY_MIN, queue_recv_timeout, &ctx);

	uCoroutine_start(&coroutine);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&coroutine);

	ASSERT_EQ(ctx.queueRet, UC_ERROR_EMPTY);
	ASSERT_GE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(0));
	ASSERT_LE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(100));
}


UCOROUTINE_FUNC_BEGIN(queue_send_timeout, ContextQueueTimeout) {
	uCoroutine_queue_init(&context->queue, context->queueStorage, 1, sizeof(context->queueStorage));

	char item;
	long unsigned int i;

	item = 5;

	for (i = 0; i < sizeof(context->queueStorage); i++) {
		uCoroutine_queue_send(&context->queue, &item, UCOROUTINE_IMMEDIATE, &context->queueRet);
	}

	context->startTime = uCoroutine_platform_getTicks();
	{
		uCoroutine_queue_send(&context->queue, &item, context->timeout, &context->queueRet);
	}
	context->endTime = uCoroutine_platform_getTicks();

	uCoroutine_interrupt();
	uCoroutine_yield();
}
UCOROUTINE_FUNC_END;


TEST(queue, send_timeout) {
	ContextQueueTimeout ctx;
	uCoroutine coroutine;

	ctx.timeout = UC_MS_TO_TICKS(1000);

	uCoroutine_prepare(&coroutine, "a", UCOROUTINE_PRIORITY_MIN, queue_send_timeout, &ctx);

	uCoroutine_start(&coroutine);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&coroutine);

	ASSERT_EQ(ctx.queueRet, UC_ERROR_FULL);
	ASSERT_GE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(1000));
	ASSERT_LE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(1100));
}


TEST(queue, send_timeout_immediate) {
	ContextQueueTimeout ctx;
	uCoroutine coroutine;

	ctx.timeout = UCOROUTINE_IMMEDIATE;

	uCoroutine_prepare(&coroutine, "a", UCOROUTINE_PRIORITY_MIN, queue_send_timeout, &ctx);

	uCoroutine_start(&coroutine);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&coroutine);

	ASSERT_EQ(ctx.queueRet, UC_ERROR_FULL);
	ASSERT_GE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(0));
	ASSERT_LE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(100));
}


struct ContextSendRecv {
	uCoroutineQueue queue;
	char            queueStorage[4];

	uCoroutineError queueRet;
	uCoroutineTick  startTime;
	uCoroutineTick  endTime;

	std::vector<char> result;
};


UCOROUTINE_FUNC_BEGIN(queue_send_recv, ContextSendRecv) {
	uCoroutine_queue_init(&context->queue, context->queueStorage, 1, sizeof(context->queueStorage));

	long unsigned int i;

	for (i = 0; i < sizeof(context->queueStorage); i++) {
		uCoroutine_queue_send(&context->queue, &i, UCOROUTINE_IMMEDIATE, &context->queueRet);

		if (context->queueRet != UC_NO_ERROR) {
			break;
		}
	}

	context->startTime = uCoroutine_platform_getTicks();
	{
		char recv;

		for (i = 0; i < sizeof(context->queueStorage); i++) {
			uCoroutine_queue_receive(&context->queue, &recv, UCOROUTINE_IMMEDIATE, &context->queueRet);

			if (context->queueRet != UC_NO_ERROR) {
				break;
			}

			context->result.push_back(recv);
		}

		if (context->queueRet == UC_NO_ERROR) {
			uCoroutine_queue_receive(&context->queue, &recv, UCOROUTINE_IMMEDIATE, &context->queueRet);
		}
	}
	context->endTime = uCoroutine_platform_getTicks();

	uCoroutine_interrupt();
	uCoroutine_yield();
}
UCOROUTINE_FUNC_END;


TEST(queue, send_recv) {
	ContextSendRecv ctx;
	uCoroutine coroutine;

	uCoroutine_prepare(&coroutine, "a", UCOROUTINE_PRIORITY_MIN, queue_send_recv, &ctx);

	uCoroutine_start(&coroutine);
	{
		uCoroutine_schedule();
	}
	uCoroutine_stop(&coroutine);

	ASSERT_EQ(ctx.queueRet, UC_ERROR_EMPTY);
	ASSERT_GE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(0));
	ASSERT_LE(ctx.endTime - ctx.startTime, UC_MS_TO_TICKS(100));
	ASSERT_EQ(ctx.result.size(), sizeof(ctx.queueStorage));

	for (size_t i = 0; i < sizeof(ctx.queueStorage); i++) {
		ASSERT_EQ(ctx.result[i], i);
	}
}
