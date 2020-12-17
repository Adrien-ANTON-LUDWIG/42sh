#define _POSIX_C_SOURCE 200809L
#include "alias.h"

#include <stdio.h>
#include <string.h>

#include "b_utils.h"
#include "list.h"
#include "my_err.h"

static int print_aliases(struct major *mj)
{
    if (!mj->alias)
        my_err(2, mj, "print alias: the list of aliases does not exist");

    struct list_item *aliases = mj->alias->head;

    while (aliases)
    {
        printf("alias %s='%s'\n", aliases->name, aliases->data);
        aliases = aliases->next;
    }
    return 0;
}

static int set_alias(struct major *mj, char **argv)
{
    // if (len < 3)
    //    my_soft_err(mj, 1, "alias not found");

    char *name_start = *argv;
    char *name_end = strstr(*argv, "=");

    if (!name_end)
        my_soft_err(mj, 1, "alias not found");
    else
    {
        size_t name_len = name_end - name_start;
        char *name = strndup(name_start, name_len);

        char *arg_start = name_end + 1;
        char *arg = strdup(arg_start);

        mj->alias = list_append_aliases(mj, mj->alias, name, arg);
    }

    return 0;
}

int b_alias(struct major *mj, char **argv)
{
    if (!mj)
        my_err(2, mj, "alias: struct major should not be NULL at this point");

    if (!argv)
        return 0;

    if (!mj->alias)
        mj->alias = list_init(mj);

    int len = argv_len(argv);

    if (len == 1)
        return print_aliases(mj);

    int index = 1;

    if (len > 1 && !strcmp(argv[1], "-p") && index++)
        print_aliases(mj);

    return set_alias(mj, argv + index);
}