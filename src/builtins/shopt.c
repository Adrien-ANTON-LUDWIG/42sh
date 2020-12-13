#define _XOPEN_SOURCE
#include "shopt.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"
#include "my_err.h"
#include "my_xmalloc.h"

#define SHOPT_OPT_LEN 7
#define SHOPT_OPT                                                              \
    {                                                                          \
        "dotglob", "expand_aliases", "extglob", "nocaseglob", "nullglob",      \
            "sourcepath", "xpg_echo"                                           \
    }

static void print_shopt_opt_plus(struct major *mj)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        printf("shopt -%s %s\n", (temp->value) ? "u" : "s", temp->name);
        temp = temp->next;
    }
}

static void print_shopt_opt_minus(struct major *mj)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        printf("%s %s\n", temp->name, (temp->value) ? "yes" : "no");
        temp = temp->next;
    }
}

static void print_shopt_opt_same_value(struct major *mj, int value)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        if (temp->value == value)
            printf("%s %s\n", temp->name, (temp->value) ? "yes" : "no");
        temp = temp->next;
    }
}

static int print_shopt_opt(struct major *mj, char *arg, int should_print)
{
    int rvalue = 0;

    if (!arg && should_print && (rvalue = 1))
        print_shopt_opt_minus(mj);
    else if (!strcmp(arg, "+O") && (rvalue = 1))
        print_shopt_opt_plus(mj);
    else if (!strcmp(arg, "-O") && (rvalue = 1))
        print_shopt_opt_minus(mj);
    else if (!strcmp(arg, "-s"))
        print_shopt_opt_same_value(mj, 1);
    else if (!strcmp(arg, "-u"))
        print_shopt_opt_same_value(mj, 0);
    else
        rvalue = 0;
    return rvalue;
}

static int get_shopt_index(char *name)
{
    if (!name)
        return -1;

    char *shopt_names[] = SHOPT_OPT;

    for (int i = 0; i < SHOPT_OPT_LEN; i++)
        if (!strcmp(name, shopt_names[i]))
            return i;
    return -1;
}

static int shopt_opt_set(struct major *mj, char **argv)
{
    if (!argv || !*argv)
        return print_shopt_opt(mj, "-s", 0);

    char *shopt_names[] = SHOPT_OPT;

    for (int i = 0; i < argv_len(argv); i++)
    {
        int name_index = get_shopt_index(argv[i]);

        if (name_index < 0)
        {
            warnx("shopt: %s: invalid shell option name", argv[i]);
            return 1;
        }

        set_shopt_opt(mj->shopt_opt, shopt_names[i], 1);
    }

    return 0;
}

static int shopt_opt_unset(struct major *mj, char **argv)
{
    if (!argv || !*argv)
        return print_shopt_opt(mj, "-u", 0);

    char *shopt_names[] = SHOPT_OPT;

    for (int i = 0; i < argv_len(argv); i++)
    {
        int name_index = get_shopt_index(argv[i]);

        if (name_index < 0)
        {
            warnx("shopt: %s: invalid shell option name", argv[i]);
            return 1;
        }

        set_shopt_opt(mj->shopt_opt, shopt_names[i], 0);
    }

    return 0;
}

int shopt_opt_is_set(struct major *mj, char *opt_name)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp && strcmp(temp->name, opt_name) != 0)
    {
        temp = temp->next;
    }

    if (!temp)
        return 0;

    return temp->value;
}

int shopt_options_argv(struct major *mj, char **argv)
{
    if (!argv || (strcmp(argv[0], "+O") != 0 && strcmp(argv[0], "-O") != 0))
        return 0;

    if (argv_len(argv) > 1)
        my_err(2, mj, "invalid shell option name");

    if (!mj->shopt_opt)
        init_shopt_opt_list(mj);

    return print_shopt_opt(mj, argv[0], 0);
}

int b_shopt_options(struct major *mj, char **argv)
{
    if (!argv)
        return 0;

    if (!mj->shopt_opt)
        init_shopt_opt_list(mj);

    int len = argv_len(argv);

    if (len == 1)
        return print_shopt_opt(mj, NULL, 1);

    if (!strcmp(argv[1], "-s"))
        return shopt_opt_set(mj, argv + 2);

    if (!strcmp(argv[1], "-u"))
        return shopt_opt_unset(mj, argv + 2);

    return 0;
}