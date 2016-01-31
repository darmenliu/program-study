/*
 *this libary just use to study the c program,
 *
*/
#ifndef LIST_H
#define LIST_H 1


struct list{
    struct list *prev;
    sturct list *next;
}


static void list_init(struct list* list);
static void list_poison(struct list* list);

static void list_insert(struct list* list, struct list* node);
static void list_push_front(struct list* list, struct list* node);
static void list_push_back(struct list* list, struct list* node);
static void list_splice(struct list* before, struct list* first, struct list* last);

static struct list* list_remove(struct list* node);
static struct list* list_pop_front(struct list* node);
static struct list* list_pop_back(struct list* node);

static unsigned int list_size(const struct list* list);
static bool list_is_empty(const struct list* list);

#endif
