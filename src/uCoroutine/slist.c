/*
 * slist.c
 *
 *  Created on: 13.09.2021
 *      Author: jarko
 */

#include "uCoroutine/utils.h"
#include "uCoroutine/slist.h"


void slist_initialize(SList *list) {
	list->next = NULL;
}


void slist_insert(SListNode *node, SListNode *nextNode) {
	nextNode->next = node->next;
	node->next     = nextNode;
}


void slist_append(SList *node, SListNode *newNode) {
	while (NOT_NULL(node->next)) {
		node = node->next;
	}

	node->next    = newNode;
	newNode->next = NULL;
}


void slist_remove(SList *list, SListNode *node) {
	while (NOT_NULL(list->next) && list->next != node) list = list->next;

	if (NOT_NULL(list->next)) {
		list->next = list->next->next;
	}
}


bool slist_isEmpty(SList *list) {
	return list->next == NULL;
}


size_t slist_size(SList *list) {
	size_t ret = 0;

	list = list->next;
	while (NOT_NULL(list)) {
		list = list->next;
		ret ++;
	}

	return ret;
}


SListNode *slist_first(SList *list) {
	return list->next;
}


SListNode *slist_last(SList *list) {
	while (NOT_NULL(list->next)) {
		list = list->next;
	}

	return list;
}
