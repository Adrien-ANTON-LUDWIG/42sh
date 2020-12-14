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

static void print_shopt_opt(struct major *mj, int is_minus, int value)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        if (!is_minus)
            printf("shopt -%s %s\n", (temp->value) ? "u" : "s", temp->name);
        else if (value < 0 || temp->value == value)
            printf("%s %s\n", temp->name, (temp->value) ? "yes" : "no");
        temp = temp->next;
    }
}

static int shopt_opt_print(struct major *mj, char *arg, int should_print)
{
    int rvalue = 0;

    if (!arg && should_print && (rvalue = 1))
        print_shopt_opt(mj, 1, -1);
    else if (!strcmp(arg, "+O") && (rvalue = 1))
        print_shopt_opt(mj, 0, -1);
    else if (!strcmp(arg, "-O") && (rvalue = 1))
        print_shopt_opt(mj, 1, -1);
    else if (!strcmp(arg, "-s"))
        print_shopt_opt(mj, 1, 1);
    else if (!strcmp(arg, "-u"))
        print_shopt_opt(mj, 1, 0);
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

static int shopt_opt_manage(struct major *mj, char **argv, int should_set,
                            int one_time)
{
    if (!argv || !*argv)
        return (should_set) ? shopt_opt_print(mj, "-s", 0)
                            : shopt_opt_print(mj, "-u", 0);

    char *shopt_names[] = SHOPT_OPT;

    for (int i = 0; i < argv_len(argv); i++)
    {
        int name_index = get_shopt_index(argv[i]);

        if (name_index < 0)
            my_err(2, mj, "shopt: invalid shell option name");

        set_shopt_opt(mj->shopt_opt, shopt_names[name_index], should_set);

        if (one_time)
            break;
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

    int len = argv_len(argv);

    if (len > 1 && strcmp(argv[0], "+O"))
        my_err(2, mj, "invalid shell option name");

    if (!mj->shopt_opt)
        init_shopt_opt_list(mj);

    if (len > 1 && !strcmp(argv[0], "-O"))
        return shopt_opt_manage(mj, argv + 1, 1, 1) + 2;

    return shopt_opt_print(mj, argv[0], 0);
}

int b_shopt_options(struct major *mj, char **argv)
{
    if (!argv)
        return 0;

    if (!mj->shopt_opt)
        init_shopt_opt_list(mj);

    int len = argv_len(argv);

    if (len == 1)
        return shopt_opt_print(mj, NULL, 1);

    if (!strcmp(argv[1], "-s"))
        return shopt_opt_manage(mj, argv + 2, 1, 0);

    if (!strcmp(argv[1], "-u"))
        return shopt_opt_manage(mj, argv + 2, 0, 0);

    return 0;
}