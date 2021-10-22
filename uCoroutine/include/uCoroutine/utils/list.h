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
 * list.h
 *
 *  Created on: 10.09.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
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
