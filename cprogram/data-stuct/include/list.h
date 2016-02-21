/*
 *this libary just use to study the c program,
 *
*/
#ifndef LIST_H
#define LIST_H 1

#include <stdbool.h>
#include <stddef.h>

struct list{
    struct list *prev;
    struct list *next;
};


static inline void list_init(struct list* list);

static inline void list_insert_before(struct list* before, struct list* node);
static inline void list_insert_behind(struct list* behind, struct list* node);
static inline void list_push_front(struct list* head, struct list* node);
static inline void list_push_back(struct list* list, struct list* node);
static inline void list_splice(struct list* before, struct list* first, struct list* last);
static inline void list_replace(struct list* node, struct list* position);

static inline struct list* list_remove(struct list* node);
static inline struct list* list_pop_front(struct list* head);
static inline struct list* list_pop_back(struct list* head);
static inline struct list* list_front(struct list* head);
static inline struct list* list_back(struct list* head);

#define LIST_FOR_EACH(ITER, MEMBER, LIST)                               \
    for (INIT_CONTAINER(ITER, (LIST)->next, MEMBER);                    \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER))
#define LIST_FOR_EACH_CONTINUE(ITER, MEMBER, LIST)                      \
    for (ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER);             \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.next, MEMBER))
#define LIST_FOR_EACH_REVERSE(ITER, MEMBER, LIST)                       \
    for (INIT_CONTAINER(ITER, (LIST)->prev, MEMBER);                    \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER))
#define LIST_FOR_EACH_REVERSE_CONTINUE(ITER, MEMBER, LIST)              \
    for (ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER);           \
         &(ITER)->MEMBER != (LIST);                                     \
         ASSIGN_CONTAINER(ITER, (ITER)->MEMBER.prev, MEMBER))
#define LIST_FOR_EACH_SAFE(ITER, NEXT, MEMBER, LIST)               \
    for (INIT_CONTAINER(ITER, (LIST)->next, MEMBER);               \
         (&(ITER)->MEMBER != (LIST)                                \
          ? INIT_CONTAINER(NEXT, (ITER)->MEMBER.next, MEMBER), 1   \
          : 0);                                                    \
         (ITER) = (NEXT))
#define LIST_FOR_EACH_POP(ITER, MEMBER, LIST)                      \
    while (!list_is_empty(LIST)                                    \
           && (INIT_CONTAINER(ITER, list_pop_front(LIST), MEMBER), 1))

#endif
