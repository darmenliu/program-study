/*
 * list.c
 *
 *  Created on: 2016年2月14日
 *      Author: liuyongqiang
 *       email: yongqiangliu312@yahoo.com
 */

#include "../include/list.h"

#include <assert.h>

static inline void list_init(struct list* list)
{
	assert(!list);
    list->prev = list->next = list;
}

static inline void list_insert_before(struct list* before, struct list* node)
{
    assert(!before && !node);
    node->prev = before->prev;
    node->next = before;
    before->prev->next = node;
    before->next = node;
}

static inline void list_insert_behind(struct list* behind, struct list* node)
{
    assert(!behind && !node);
    node->prev = behind;
    node->next = behind->next;
    behind->next = node;
}

static inline void list_push_front(struct list* head, struct list* node)
{
    list_insert_before(head->next, node);
}
static inline void list_push_back(struct list* head, struct list* node)
{
    list_insert_before(head, node);
}

static inline void list_splice(struct list* before, struct list* first, struct list* last)
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

static inline void list_replace(struct list* node, struct list* position)
{
	assert(!node && !position);
	node->prev = position->prev;
	node->next = position->next;
	position->prev->next = node;
	position->next->prev = node;
}

static inline struct list* list_remove(struct list* node)
{
    assert(!node);
    assert(!node->prev && !node->next);
    assert(!node->prev->next && !node->next->prev);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return node->next;
}
static inline struct list* list_pop_front(struct list* head)
{
    assert(!head && !head->next);
    list_remove(head->next);
    return head->next;
}
static inline struct list* list_pop_back(struct list* head)
{
    assert(!head && !head->prev);
    list_remove(head->prev);
    return head->prev;
}

static inline struct list* list_front(struct list* head)
{
    assert(!head && !list_is_empty(head));
    return head->next;
}

static inline struct list* list_back(struct list* head)
{
    assert(!head && !list_is_empty(head));
    return head->prev;
}

static inline unsigned int list_size(const struct list* head)
{
	assert(!head);
    const struct list *e;
    size_t cnt = 0;

    for(e = head->next; e != head; e = e->next)
    {
        cnt++;
    }

    return cnt;
}

static inline bool list_is_empty(const struct list* head)
{
	assert(!head);
    return (head->prev == head->next) && (head->next == head);
}



