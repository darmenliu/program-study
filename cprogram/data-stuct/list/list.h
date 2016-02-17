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


void list_init(struct list* list);

void list_insert_before(struct list* before, struct list* node);
void list_insert_behind(struct list* behind, struct list* node);
void list_push_front(struct list* head, struct list* node);
void list_push_back(struct list* list, struct list* node);
void list_splice(struct list* before, struct list* first, struct list* last);

struct list* list_remove(struct list* node);
struct list* list_pop_front(struct list* head);
struct list* list_pop_back(struct list* head);

unsigned int list_size(const struct list* list);
bool list_is_empty(const struct list* list);

#endif
