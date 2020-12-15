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

#define UNSET 0
#define SET 1

#define MULTIPLE_TIME 0
#define ONE_TIME 1

#define SHOULD_NOT_PRINT 0
#define SHOULD_PRINT 1

#define IS_PLUS 0
#define IS_MINUS 1

#define SHOPT_OPT                                                              \
    {                                                                          \
        "dotglob", "expand_aliases", "extglob", "nocaseglob", "nullglob",      \
            "sourcepath", "xpg_echo"                                           \
    }

static void print_shopt_opt(struct major *mj, int is_minus, int value,
                            char *name)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        if (!name && !is_minus)
            printf("shopt -%s %s\n", (temp->value) ? "u" : "s", temp->name);
        else if (!name && (value < 0 || temp->value == value))
            printf("%s %s\n", temp->name, (temp->value) ? "yes" : "no");
        else if (name && !strcmp(temp->name, name))
            printf("%s %s\n", temp->name, (temp->value) ? "yes" : "no");
        temp = temp->next;
    }
}

static int shopt_opt_print(struct major *mj, char *arg, int should_print)
{
    int rvalue = 0;

    if (!arg && should_print && (rvalue = 1))
        print_shopt_opt(mj, IS_MINUS, -1, NULL);
    else if (!strcmp(arg, "+O") && (rvalue = 1))
        print_shopt_opt(mj, IS_PLUS, -1, NULL);
    else if (!strcmp(arg, "-O") && (rvalue = 1))
        print_shopt_opt(mj, IS_MINUS, -1, NULL);
    else if (!strcmp(arg, "-s"))
        print_shopt_opt(mj, IS_MINUS, 1, NULL);
    else if (!strcmp(arg, "-u"))
        print_shopt_opt(mj, IS_MINUS, 0, NULL);
    else
        rvalue = 0;
    return rvalue;
}

static int shopt_get_index(char *name)
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
        return (should_set) ? shopt_opt_print(mj, "-s", SHOULD_NOT_PRINT)
                            : shopt_opt_print(mj, "-u", SHOULD_NOT_PRINT);

    char *shopt_names[] = SHOPT_OPT;

    for (int i = 0; i < argv_len(argv); i++)
    {
        int name_index = shopt_get_index(argv[i]);

        if (name_index < 0)
            my_err(2, mj, "shopt: invalid shell option name");

        shopt_set_opt(mj, shopt_names[name_index], should_set);

        if (one_time)
            break;
    }

    return 0;
}

int shopt_opt_is_set(struct major *mj, char *opt_name)
{
    if (!mj->shopt_opt)
        shopt_init_list(mj);

    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp && strcmp(temp->name, opt_name) != 0)
        temp = temp->next;

    if (!temp && my_soft_err(mj, 2, "shopt: invalid shell option name"))
        return 0;

    return temp->value;
}

int shopt_options_argv(struct major *mj, char **argv)
{
    if (!argv || (strcmp(argv[0], "+O") != 0 && strcmp(argv[0], "-O") != 0))
        return 0;

    int len = argv_len(argv);

    if (!mj->shopt_opt)
        shopt_init_list(mj);

    if (len > 1)
    {
        if (!strcmp(argv[0], "-O"))
            shopt_opt_manage(mj, argv + 1, SET, ONE_TIME);
        else
            shopt_opt_manage(mj, argv + 1, UNSET, ONE_TIME);
        return 2;
    }

    return shopt_opt_print(mj, argv[0], SHOULD_NOT_PRINT);
}

int b_shopt_options(struct major *mj, char **argv)
{
    if (!argv)
        return 0;

    if (!mj->shopt_opt)
        shopt_init_list(mj);

    int len = argv_len(argv);

    if (len == 1)
        return shopt_opt_print(mj, NULL, SHOULD_PRINT);

    if (!strcmp(argv[1], "-s"))
        return shopt_opt_manage(mj, argv + 2, SET, MULTIPLE_TIME);

    if (!strcmp(argv[1], "-u"))
        return shopt_opt_manage(mj, argv + 2, UNSET, MULTIPLE_TIME);

    int index = shopt_get_index(argv[1]);

    if (index >= 0)
        print_shopt_opt(mj, IS_MINUS, -1, argv[1]);

    return 0;
}