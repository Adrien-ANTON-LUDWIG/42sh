#define _POSIX_C_SOURCE 200809L
#include "alias.h"

#include <stdio.h>
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

static int alias_print(struct major *mj, struct list_item *alias, char *name,
                       char *p)
{
    if (!mj->alias)
        my_err(2, mj, "print_alias: the list of aliases does not exist");

    int rvalue = 0;

    if (alias && !name)
        printf("%s%s='%s'\n", p, alias->name, alias->data);
    else if (name)
    {
        struct list_item *ali = alias_get(mj->alias, name);

        if (!ali && ++rvalue)
            my_soft_err(mj, 1, "print_alias: alias name not found");
        else
            printf("%s='%s'\n", ali->name, ali->data);
    }
    return rvalue;
}

static int aliases_print_all(struct major *mj, char *p)
{
    if (!mj->alias)
        my_err(2, mj, "print_all_alias: the list of aliases does not exist");

    struct list_item *aliases = mj->alias->head;

    while (aliases)
    {
        alias_print(mj, aliases, NULL, p);
        aliases = aliases->next;
    }
    return 0;
}

static int set_alias(struct major *mj, char **argv)
{
    // if (len < 3)
    //    my_soft_err(mj, 1, "alias not found");

    if (!*argv)
        return 0;

    while (*argv)
    {
        char *name_start = *argv;
        char *name_end = strstr(*argv, "=");

        if (!name_end)
            return alias_print(mj, NULL, name_start, 0);

        size_t name_len = name_end - name_start;
        char *name = strndup(name_start, name_len);

        char *arg_start = name_end + 1;
        char *arg = strdup(arg_start);

        mj->alias = list_append_aliases(mj, mj->alias, name, arg);
        argv++;
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
        return aliases_print_all(mj, "");

    int index = 1;

    if (len > 1 && !strcmp(argv[1], "-p") && ++index)
        aliases_print_all(mj, "alias ");

    return set_alias(mj, argv + index);
}