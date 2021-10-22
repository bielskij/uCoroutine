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
 * slist.c
 *
 *  Created on: 13.09.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#include "uCoroutine/utils/utils.h"
#include "uCoroutine/utils/slist.h"

#ifdef UC_DEBUG_LEVEL_SLIST
	#define UC_DEBUG_LEVEL UC_DEBUG_LEVEL_SLIST
#endif
#include "uCoroutine/utils/debug.h"


void slist_initialize(SList *list) {
	UC_ASSERT(NOT_NULL(list));

	list->next = NULL;
}


void slist_insert(SListNode *node, SListNode *nextNode) {
	UC_ASSERT(NOT_NULL(node));
	UC_ASSERT(NOT_NULL(nextNode));

	nextNode->next = node->next;
	node->next     = nextNode;
}


void slist_append(SList *node, SListNode *newNode) {
	UC_ASSERT(NOT_NULL(node));
	UC_ASSERT(NOT_NULL(newNode));

	while (NOT_NULL(node->next)) {
		node = node->next;
	}

	node->next    = newNode;
	newNode->next = NULL;
}


void slist_remove(SList *list, SListNode *node) {
	UC_ASSERT(NOT_NULL(list));
	UC_ASSERT(NOT_NULL(node));

	while (NOT_NULL(list->next) && list->next != node) list = list->next;

	if (NOT_NULL(list->next)) {
		list->next = list->next->next;
	}
}


bool slist_isEmpty(SList *list) {
	UC_ASSERT(NOT_NULL(list));

	return list->next == NULL;
}


size_t slist_size(SList *list) {
	size_t ret = 0;

	UC_ASSERT(NOT_NULL(list));

	list = list->next;
	while (NOT_NULL(list)) {
		list = list->next;
		ret ++;
	}

	return ret;
}


SListNode *slist_first(SList *list) {
	UC_ASSERT(NOT_NULL(list));

	return list->next;
}


SListNode *slist_last(SList *list) {
	UC_ASSERT(NOT_NULL(list));

	while (NOT_NULL(list->next)) {
		list = list->next;
	}

	return list;
}
