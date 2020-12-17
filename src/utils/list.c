#include "list.h"

#include <stdlib.h>
#include <string.h>

#include "my_err.h"
#include "my_xmalloc.h"

static struct list_item *list_item_init(struct major *mj, char *str)
{
    struct list_item *l = my_xcalloc(mj, 1, sizeof(struct list_item));
    l->data = str;

    return l;
}

static void list_free_one_item(struct list_item *item)
{
    free(item->name);
    free(item->data);
    free(item);
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
        free(temp->name);
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

struct list *list_append_aliases(struct major *mj, struct list *list,
                                 char *name, char *value)
{
    if (!value || !name)
        my_err(1, mj, "list_append_aliases: value & name should not be empty");

    struct list_item *item = list_item_init(mj, value);
    item->name = name;

    if (!list)
    {
        list = list_init(mj);
        list->head = item;
        list->tail = item;
        list->size++;

        return list;
    }
    struct list_item *iter = list->head;

    if (iter && item < iter)
    {
        item->next = iter;
        list->head = item;
    }
    else
    {
        while (iter->next && item < iter->next)
            iter = iter->next;

        if (!iter)
        {
            list->tail->next = item;
            list->tail = item;
        }
        else
        {
            item->next = iter->next;
            iter->next = item;
        }
    }
    list->size++;
    return list;
}

struct list *list_remove(struct list *list, char *name)
{
    if (!list || !name)
        return NULL;

    struct list_item *item = list->head;
    struct list_item *prev = list->head;

    while (item && strcmp(item->name, name) != 0)
    {
        prev = item;
        item = item->next;
    }

    if (!item)
        return list;

    if (item == prev)
    {
        item = item->next;
        list_free_one_item(prev);
        list->head = item;
    }
    else if (!item->next)
    {
        list_free_one_item(item);
        list->tail = prev;
        prev->next = NULL;
    }
    else
    {
        prev->next = item->next;
        list_free_one_item(item);
    }

    list->size--;
    return list;
}