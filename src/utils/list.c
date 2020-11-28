#include "my_utils.h"

static struct list_item *list_item_init(struct major *mj, char *str)
{
    struct list_item *l = malloc(sizeof(struct list_item));

    if (!l)
        my_err(1, mj, "list_item_init: malloc failed");

    l->next = NULL;
    l->data = str;

    return l;
}

struct list *list_init(struct major *mj)
{
    struct list *l = malloc(sizeof(struct list));

    if (!l)
        my_err(1, mj, "list_init: error with malloc");

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;

    return l;
}

void list_free(struct list *l)
{
    if (!l)
        return;

    struct list_item *item = l->head;
    while (item)
    {
        struct list_item *temp = item;
        item = item->next;
        free(temp->data);
        free(temp);
    }

    free(l);
}

struct list *list_append(struct major *mj, struct list *list, char *str)
{
    if (!str)
        my_err(1, mj, "list_append: str should not be empty");

    if (!list)
    {
        list = list_init(mj);
        struct list_item *item = list_item_init(mj, str);
        list->head = item;
        list->tail = item;
        list->size++;

        return list;
    }

    struct list_item *item = list_item_init(mj, str);
    list->tail->next = item;
    list->tail = item;
    list->size++;

    return list;
}