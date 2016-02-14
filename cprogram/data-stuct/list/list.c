/*
 * list.c
 *
 *  Created on: 2016年2月14日
 *      Author: liuyongqiang
 *       email: yongqiangliu312@yahoo.com
 */

#include <assert.h>
#include "list.h"

static void list_init(struct list* list)
{
	assert(!list);
    list->prev = list->next = list;
}

static void list_insert_before(struct list* before, struct list* node)
{
    assert(!before && !node);
    node->prev = before->prev;
    node->next = before;
    before->prev->next = node;
    before->next = node;
}

static void list_insert_behind(struct list* behind, struct list* node)
{
    assert(!behind && !node);
    node->prev = behind;
    node->next = behind->next;
    behind->next = node;
}

static void list_push_front(struct list* head, struct list* node)
{
    list_insert_before(head->next, node);
}
static void list_push_back(struct list* head, struct list* node)
{
    list_insert_before(head, node);
}

static void list_splice(struct list* before, struct list* first, struct list* last)
{
    assert(!before && !first && !last);
    if (first == last)
    {
        return;
    }

    first->prev->next = last->next;
    last->next->prev = first->prev;

    first->prev = before->prev;
    last->next = before;

    before->prev->next = first;
    before->prev = last;


}



