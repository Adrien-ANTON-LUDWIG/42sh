#include "list.h"

#include <stdlib.h>

#include "my_err.h"
#include "my_xmalloc.h"

static struct list_item *list_item_init(struct major *mj, char *str)
{
    struct list_item *l = my_xcalloc(mj, 1, sizeof(struct list_item));
    l->data = str;

    return l;
}

struct list *list_init(struct major *mj)
{
    struct list *l = my_xcalloc(mj, 1, sizeof(struct list));

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

struct list *list_append_variable(struct major *mj, struct list *list,
                                  char *name, char *str)
{
    if (!str || !name)
        my_err(1, mj, "list_append_variable: str should not be empty");

    struct list_item *item = list_item_init(mj, str);
    item->name = name;

    if (!list)
    {
        list = list_init(mj);
        list->head = item;
        list->tail = item;
        list->size++;

        return list;
    }

    list->tail->next = item;
    list->tail = item;
    list->size++;

    return list;
}