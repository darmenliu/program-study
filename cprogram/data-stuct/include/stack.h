/*
 * stack.h
 *
 *  Created on: 2016年2月21日
 *      Author: root
 */

#ifndef DATA_STUCT_INCLUDE_STACK_H_
#define DATA_STUCT_INCLUDE_STACK_H_
#include "../include/list.h"

typedef struct list Stack;
typedef struct list* pStack;
typedef struct list* pNode;

static inline void init_stack(pStack stack);
static inline void push(pStack stack, pNode pnode);
static inline bool stack_is_empty(pStack stack);
static inline size_t stack_size(pStack stack);
static inline pStack pop(pStack stack);


#endif /* DATA_STUCT_INCLUDE_STACK_H_ */
