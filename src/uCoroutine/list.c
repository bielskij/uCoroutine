/*
 * list.c
 *
 *  Created on: 10.09.2021
 *      Author: jarko
 */


#include "uCoroutine/list.h"


void list_initialize(List *list) {
	list->next = list->prev = list;
}


void list_insert(ListNode *node, ListNode *newNode, bool after) {
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
	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node->prev = node;
}


bool list_isEmpty(List *list) {
	return list->next == list;
}


size_t list_size(List *list) {
	size_t size = 0;

	{
		const List *it = list;

		while (it->next != list) {
			it = it->next; size++;
		}
	}

	return size;
}
