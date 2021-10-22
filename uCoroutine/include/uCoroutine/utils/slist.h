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
 * slist.h
 *
 *  Created on: 13.09.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_UTILS_SLIST_H_
#define UCOROUTINE_UTILS_SLIST_H_

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

#endif /* UCOROUTINE_UTILS_SLIST_H_ */
