/*
 * stack.c
 *
 *  Created on: 2016年2月21日
 *      Author: root
 */


#include "../include/stack.h"


static inline void init_stack(pStack stack)
{
    list_init(stack);
}

static inline void push(pStack stack, pNode pnode)
{
    list_push_front(stack, pnode);
}

static inline bool stack_is_empty(pStack stack)
{
    return list_is_empty(stack);
}

static inline size_t stack_size(pStack stack)
{
    return list_size(stack);
}

static inline pStack pop(pStack stack)
{
    return list_pop_front(stack);
}



