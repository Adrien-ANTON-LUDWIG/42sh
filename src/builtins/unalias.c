#define _POSIX_C_SOURCE 200809L

#include "unalias.h"

#include <string.h>

#include "b_utils.h"
#include "list.h"
#include "my_err.h"

static struct list_item *alias_get(struct list *aliases, char *name)
{
    struct list_item *tmp = aliases->head;

    while (tmp)
    {
        if (!strcmp(tmp->name, name))
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

static int unalias_name(struct major *mj, char *name)
{
    if (!name)
        return 1;

    if (!unalias_get(mj->alias, name))
        return my_soft_err(mj, 1, "unalias_name: name not found");

    mj->alias = list_remove(mj->alias, name);
    return 0;
}

static int unalias_all(struct major *mj)
{
    while (mj->alias && mj->alias->size > 0)
    {
        mj->alias = list_remove(mj->alias, mj->alias->head->name);
    }
    return 0;
}

int b_unalias(struct major *mj, char **argv)
{
    if (!mj)
        my_err(2, mj, "unalias: struct major should not be NULL at this point");

    if (!argv)
        return 0;

    int len = argv_len(argv);
    if (!mj->alias || len == 1)
        return my_soft_err(mj, 2, "unalias: alias name not found");

    if (!strcmp(*(argv + 1), "-a"))
        return unalias_all(mj);

    int rvalue = 0;

    for (int i = 1; i < len && rvalue == 0; i++)
        rvalue = unalias_name(mj, argv[i]);

    return rvalue;
}