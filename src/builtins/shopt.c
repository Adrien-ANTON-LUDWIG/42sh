#define _POSIX_C_SOURCE 200809L
#include "shopt.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"
#include "my_err.h"
#include "my_xmalloc.h"

#define SHOPT_OPT_LEN 7
#define SHOPT_OPT_PARAM 3

#define Q 0
#define S 1
#define U 2

#define UNSET 1
#define SET 2

#define MULTIPLE_TIME 4
#define ONE_TIME 8

#define SHOULD_NOT_PRINT 0
#define SHOULD_PRINT 1

#define IS_PLUS 0
#define IS_MINUS 1

#define SHOPT_OPT                                                              \
    {                                                                          \
        "dotglob", "expand_aliases", "extglob", "nocaseglob", "nullglob",      \
            "sourcepath", "xpg_echo"                                           \
    }

static int print_shopt_opt(struct major *mj, int is_minus, int value,
                           char *name)
{
    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        int len = 16 - strlen(temp->name) - 1;
        char tab[16];
        if (len <= 0)
        {
            tab[0] = '\t';
            tab[1] = 0;
        }
        else
        {
            for (int i = 0; i < len; i++)
                tab[i] = ' ';
            tab[len] = '\t';
            tab[len + 1] = 0;
        }

        if (!name && !is_minus)
            printf("shopt -%s %s\n", (temp->value) ? "u" : "s", temp->name);
        else if (!name && (value < 0 || temp->value == value))
            printf("%s%s%s\n", temp->name, tab, (temp->value) ? "on" : "off");
        else if (name && !strcmp(temp->name, name))
            printf("%s%s%s\n", temp->name, tab, (temp->value) ? "on" : "off");
        temp = temp->next;
    }
    if (!name)
        return 0;
    return shopt_opt_is_set(mj, name);
}

static int shopt_print_all(struct major *mj, char **argv, int len, int *param)
{
    if (param[Q])
        return 0;

    int rvalue = 0;
    for (int i = 1; i < len; i++)
        rvalue += print_shopt_opt(mj, IS_MINUS, -1, argv[i]);
    return rvalue != 0;
}

static int shopt_opt_print(struct major *mj, char *arg, int should_print,
                           int *param)
{
    int rvalue = 0;

    if (param[Q])
        return 0;

    if (!arg && should_print)
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

static int shopt_opt_manage(struct major *mj, char **argv,
                            int should_set_and_one_time, int *param)
{
    if (!argv || !*argv)
    {
        if (!param[Q])
            return (should_set_and_one_time & SET)
                ? shopt_opt_print(mj, "-s", SHOULD_NOT_PRINT, param)
                : shopt_opt_print(mj, "-u", SHOULD_NOT_PRINT, param);
        return 0;
    }

    char *shopt_names[] = SHOPT_OPT;
    int rvalue = 0;

    for (int i = 0; i < argv_len(argv); i++)
    {
        int name_index = shopt_get_index(argv[i]);

        if (name_index < 0)
            my_err(1, mj, "shopt: invalid shell option name");

        rvalue =
            shopt_set_opt(mj, shopt_names[name_index], should_set_and_one_time);

        if ((should_set_and_one_time & ONE_TIME) || rvalue != 0)
            break;
    }

    return rvalue;
}

static int shopt_set_param(char **argv, int *opt_len, int **param)
{
    if (!argv || !*argv)
        return 0;

    int opt = 0;
    int len = argv_len(argv);
    while ((opt = getopt(len, argv, ":qsu")) != -1)
    {
        switch (opt)
        {
        case 'q':
            param[0][Q] = 1;
            break;
        case 's':
            param[0][S] = 1;
            break;
        case 'u':
            param[0][U] = 1;
            break;
        default:
            warnx("Usage: shopt [-qsu] [optname ...]");
            return 2;
        }
    }
    *opt_len = optind;
    optind = 0;
    if (param[0][S] && param[0][U])
    {
        warnx("shopt: cannot set and unset shell options simultaneously");
        return 1;
    }
    return 0;
}

int shopt_opt_is_set(struct major *mj, char *opt_name)
{
    if (!mj->shopt_opt)
        shopt_init_list(mj);

    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp && opt_name && strcmp(temp->name, opt_name) != 0)
        temp = temp->next;

    if ((!temp || !opt_name)
        && my_soft_err(mj, 1, "shopt_opt_is_set: invalid shell option name"))
        return 1;

    return temp->value == 0;
}

int shopt_options_argv(struct major *mj, char **argv)
{
    if (!argv || (strcmp(argv[0], "+O") != 0 && strcmp(argv[0], "-O") != 0))
        return 0;

    int len = argv_len(argv);
    int param[SHOPT_OPT_PARAM] = { 0 };

    if (!mj->shopt_opt)
        shopt_init_list(mj);

    if (len > 1)
    {
        if (!strcmp(argv[0], "-O"))
            return shopt_opt_manage(mj, argv + 1, SET | ONE_TIME, param);
        else
            return shopt_opt_manage(mj, argv + 1, UNSET | ONE_TIME, param);
        return 2;
    }

    return shopt_opt_print(mj, argv[0], SHOULD_NOT_PRINT, param);
}

int b_shopt_options(struct major *mj, char **argv)
{
    if (!argv)
        return 0;

    if (!mj->shopt_opt)
        shopt_init_list(mj);

    int *param = my_xcalloc(mj, 1, SHOPT_OPT_PARAM * sizeof(int));
    int opt_len = 0;
    int opt_err = 0;
    int rvalue = -1;

    if (rvalue == -1 && (opt_err = shopt_set_param(argv, &opt_len, &param)) > 0)
        rvalue = opt_err;

    if (argv_len(argv) == 1 || (!argv[opt_len] && !param[U] && !param[S]))
        rvalue = shopt_opt_print(mj, NULL, SHOULD_PRINT, param);

    if (rvalue == -1 && param[S])
        rvalue =
            shopt_opt_manage(mj, argv + opt_len, SET | MULTIPLE_TIME, param);

    if (rvalue == -1 && param[U])
        rvalue =
            shopt_opt_manage(mj, argv + opt_len, UNSET | MULTIPLE_TIME, param);

    int index = shopt_get_index(argv[opt_len]);

    if (rvalue == -1 && index >= 0)
        rvalue = shopt_print_all(mj, argv, argv_len(argv), param);
    else if (rvalue == -1)
        rvalue = my_soft_err(mj, 1, "shopt_options: invalid shell option name");

    free(param);
    return rvalue;
}