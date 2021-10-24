/*
 * slist.h
 *
 *  Created on: 13.09.2021
 *      Author: jarko
 */

#ifndef UCOROUTINE_SLIST_H_
#define UCOROUTINE_SLIST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "uCoroutine/types.h"


typedef struct _SListNode {
	struct _SListNode *next;
} SListNode;

typedef SListNode SList;


#define SLIST_INITILIZER(list) { NULL }

#define slist_for_each(list) \
	for (SListNode *it = (list)->next; it != NULL; it = it->next)


void slist_initialize(SList *list);

void slist_insert(SListNode *node, SListNode *nextNode);

void slist_append(SList *list, SListNode *newNode);

void slist_remove(SList *list, SListNode *node);

bool slist_isEmpty(SList *list);

size_t slist_size(SList *list);

SListNode *slist_first(SList *list);

SListNode *slist_last(SList *list);


#ifdef __cplusplus
}
#endif

#endif /* UCOROUTINE_SLIST_H_ */
