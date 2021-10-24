/*
 * main.c
 *
 *  Created on: 24.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include "gtest/gtest.h"

#include "uCoroutine.h"


int main(int argc, char *argv[]) {
	int ret = 0;

	{
		uCoroutine_initialize();

		testing::InitGoogleTest(&argc, argv);

		ret = RUN_ALL_TESTS();

		uCoroutine_terminate();
	}

	return ret;
}
