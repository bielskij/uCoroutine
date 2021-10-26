/*
 * test.cpp
 *
 *  Created on: 26.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine/slist.h"
#include "uCoroutine/utils.h"

struct SListNode1 {
	SListNode node;

	int value;
};


TEST(slist, initialize_1) {
	SList l = SLIST_INITILIZER(l);

	ASSERT_TRUE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == 0);
}


TEST(slist, initialize_2) {
	SList l;

	slist_initialize(&l);

	ASSERT_TRUE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == 0);
}


TEST(slist, insert_after) {
	SList l = SLIST_INITILIZER(l);

	SListNode1 nodes[3];

	for (size_t i = 0; i < array_size(nodes); i++) {
		SListNode *node = &nodes[i].node;

		container_of(node, SListNode1, node)->value = i + 1;

		slist_insert(&l, node);
	}

	ASSERT_FALSE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == array_size(nodes));

	ASSERT_TRUE(container_of(slist_first(&l), SListNode1, node)->value == 3);
	ASSERT_TRUE(container_of(slist_last(&l),  SListNode1, node)->value == 1);
}


TEST(slist, insert_before) {
	SList l = SLIST_INITILIZER(l);

	SListNode1 nodes[3];

	for (size_t i = 0; i < array_size(nodes); i++) {
		SListNode *node = &nodes[i].node;

		container_of(node, SListNode1, node)->value = i + 1;

		slist_append(&l, node);
	}

	ASSERT_FALSE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == array_size(nodes));

	ASSERT_TRUE(container_of(slist_first(&l), SListNode1, node)->value == 1);
	ASSERT_TRUE(container_of(slist_last(&l),  SListNode1, node)->value == 3);
}


TEST(slist, loop_macro) {
	SList l = SLIST_INITILIZER(l);

	SListNode1 nodes[5];

	for (size_t i = 0; i < array_size(nodes); i++) {
		SListNode *node = &nodes[i].node;

		container_of(node, SListNode1, node)->value = i + 1;

		slist_append(&l, node);
	}

	int i = 1;
	slist_for_each(&l) {
		ASSERT_TRUE(container_of(it, SListNode1, node)->value == i++);
	}
}


TEST(slist, remove_one_element_list) {
	SList l = SLIST_INITILIZER(l);

	SListNode node;

	slist_insert(&l, &node);
	slist_remove(&l, &node);

	ASSERT_TRUE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == 0);
}


TEST(slist, remove_element_list) {
	SList l = SLIST_INITILIZER(l);

	SListNode1 nodes[5];

	for (size_t i = 0; i < array_size(nodes); i++) {
		SListNode *node = &nodes[i].node;

		container_of(node, SListNode1, node)->value = i + 1;

		slist_append(&l, node);
	}

	ASSERT_TRUE(container_of(slist_first(&l), SListNode1, node)->value == 1);
	{
		slist_remove(&l, slist_first(&l));
	}
	ASSERT_TRUE(container_of(slist_first(&l), SListNode1, node)->value == 2);

	ASSERT_TRUE(container_of(slist_last(&l), SListNode1, node)->value == array_size(nodes));
	{
		slist_remove(&l, slist_last(&l));
	}
	ASSERT_TRUE(container_of(slist_last(&l), SListNode1, node)->value == array_size(nodes) - 1);

	ASSERT_FALSE(slist_isEmpty(&l));
	ASSERT_TRUE(slist_size(&l) == 3);
}
