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
 * list.cpp
 *
 *  Created on: 13.09.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include <gtest/gtest.h>

#include "uCoroutine/utils.h"
#include "uCoroutine/list.h"


struct ListNode1 {
	ListNode node;

	int value;
};


TEST(list, initialize_1) {
	List l = LIST_INITILIZER(l);

	ASSERT_TRUE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == 0);
}


TEST(list, initialize_2) {
	List l;

	list_initialize(&l);

	ASSERT_TRUE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == 0);
}


TEST(list, insert_after) {
	List l = LIST_INITILIZER(l);

	ListNode1 nodes[3];

	for (size_t i = 0; i < array_size(nodes); i++) {
		ListNode *node = &nodes[i].node;

		container_of(node, ListNode1, node)->value = i + 1;

		list_insert(&l, node, true);
	}

	ASSERT_FALSE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == array_size(nodes));

	ASSERT_TRUE(list_first(&l, ListNode1, node)->value == 3);
	ASSERT_TRUE(list_last(&l, ListNode1, node)->value == 1);
}


TEST(list, insert_before) {
	List l = LIST_INITILIZER(l);

	ListNode1 nodes[3];

	for (size_t i = 0; i < array_size(nodes); i++) {
		ListNode *node = &nodes[i].node;

		container_of(node, ListNode1, node)->value = i + 1;

		list_insert(&l, node, false);
	}

	ASSERT_FALSE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == array_size(nodes));

	ASSERT_TRUE(list_first(&l, ListNode1, node)->value == 1);
	ASSERT_TRUE(list_last(&l, ListNode1, node)->value == 3);
}


TEST(list, loop_macro) {
	List l = LIST_INITILIZER(l);

	ListNode1 nodes[5];

	for (size_t i = 0; i < array_size(nodes); i++) {
		ListNode *node = &nodes[i].node;

		container_of(node, ListNode1, node)->value = i + 1;

		list_insert(&l, node, false);
	}

	int i = 1;
	list_for_each(&l) {
		ASSERT_TRUE(container_of(it, ListNode1, node)->value == i++);
	}
}


TEST(list, loop_safe_macro) {
	List l = LIST_INITILIZER(l);

	ListNode1 nodes[5];

	for (size_t i = 0; i < array_size(nodes); i++) {
		ListNode *node = &nodes[i].node;

		container_of(node, ListNode1, node)->value = i + 1;

		list_insert(&l, node, false);
	}

	int i = 1;
	list_for_each_safe(&l) {
		ASSERT_TRUE(container_of(it, ListNode1, node)->value == i++);
	}
}


TEST(list, remove_empty_list) {
	List l = LIST_INITILIZER(l);

	list_remove(&l);

	ASSERT_TRUE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == 0);
}


TEST(list, remove_one_element_list) {
	List l = LIST_INITILIZER(l);

	ListNode node;

	list_insert(&l, &node, true);
	list_remove(&l);

	ASSERT_TRUE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == 0);
}


TEST(list, remove_element_list) {
	List l = LIST_INITILIZER(l);

	ListNode1 nodes[5];

	for (int i = 0; i < array_size(nodes); i++) {
		ListNode *node = &nodes[i].node;

		container_of(node, ListNode1, node)->value = i + 1;

		list_insert(&l, node, false);
	}

	ASSERT_TRUE(list_first(&l, ListNode1, node)->value == 1);
	{
		list_remove(&list_first(&l, ListNode1, node)->node);
	}
	ASSERT_TRUE(list_first(&l, ListNode1, node)->value == 2);

	ASSERT_TRUE(list_last(&l, ListNode1, node)->value == array_size(nodes));
	{
		list_remove(&list_last(&l, ListNode1, node)->node);
	}
	ASSERT_TRUE(list_last(&l, ListNode1, node)->value == array_size(nodes) - 1);

	ASSERT_FALSE(list_isEmpty(&l));
	ASSERT_TRUE(list_size(&l) == 3);
}
