/*
 * list.c
 *
 *  Created on: 10.09.2021
 *      Author: jarko
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
