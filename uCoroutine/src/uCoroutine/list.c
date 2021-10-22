/*******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2021 Jaroslaw Bielski (bielski.j@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
/*
 * list.c
 *
 *  Created on: 10.09.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include "uCoroutine/utils/list.h"
#include "uCoroutine/utils/utils.h"

#ifdef UC_DEBUG_LEVEL_LIST
	#define UC_DEBUG_LEVEL UC_DEBUG_LEVEL_LIST
#endif
#include "uCoroutine/utils/debug.h"


void list_initialize(List *list) {
	UC_ASSERT(NOT_NULL(list));

	list->next = list->prev = list;
}


void list_insert(ListNode *node, ListNode *newNode, bool after) {
	UC_ASSERT(NOT_NULL(node));
	UC_ASSERT(NOT_NULL(newNode));

	if (after) {
		node->next->prev = newNode;
		newNode->next    = node->next;
		node->next       = newNode;
		newNode->prev    = node;

	} else {
		node->prev->next = newNode;
		newNode->prev    = node->prev;
		node->prev       = newNode;
		newNode->next    = node;
	}
}


void list_remove(ListNode *node) {
	UC_ASSERT(NOT_NULL(node));

	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node->prev = node;
}


bool list_isEmpty(List *list) {
	UC_ASSERT(NOT_NULL(list));

	return list->next == list;
}


size_t list_size(List *list) {
	size_t size = 0;

	UC_ASSERT(NOT_NULL(list));

	{
		const List *it = list;

		while (it->next != list) {
			it = it->next; size++;
		}
	}

	return size;
}
