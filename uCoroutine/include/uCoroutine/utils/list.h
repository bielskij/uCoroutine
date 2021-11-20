/*
 * list.h
 *
 *  Created on: 10.09.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_UTILS_LIST_H_
#define UCOROUTINE_UTILS_LIST_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "uCoroutine/types.h"

typedef struct _ListNode {
	struct _ListNode *next;
	struct _ListNode *prev;
} ListNode;

typedef ListNode List;


#define LIST_INITILIZER(list) { &(list), &(list) }

#define list_for_each(list) \
	for (ListNode *it = (list)->next; it != (list); it = it->next)

#define list_for_each_safe(list) \
	for (ListNode *it = (list)->next, *nextIt = it->next; it != (list); it = nextIt, nextIt = it->next)

#define list_first(list, type, member) \
	container_of((list)->next, type, member)

#define list_last(list, type, member) \
	container_of((list)->prev, type, member)


void list_initialize(List *list);

void list_insert(ListNode *node, ListNode *newNode, bool after);

void list_remove(ListNode *node);

bool list_isEmpty(List *list);

size_t list_size(List *list);


#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_UTILS_LIST_H_ */
