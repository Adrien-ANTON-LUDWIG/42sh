#define _XOPEN_SOURCE
#include "shopt.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"
#include "my_xmalloc.h"

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
        printf("%s: %s\n", temp->name, (temp->value) ? "yes" : "no");
        temp = temp->next;
    }
}

static int print_shopt_opt(struct major *mj, char *arg)
{
    int rvalue = 0;

    if (!strcmp(arg, "+O") && (rvalue = 1))
        print_shopt_opt_plus(mj);
    else if (!strcmp(arg, "-O") && (rvalue = 1))
        print_shopt_opt_minus(mj);
    else
        rvalue = 0;
    return rvalue;
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

    if (!mj->shopt_opt)
        init_shopt_opt_list(mj);
    return print_shopt_opt(mj, argv[0]);

    /*   int opt;
       while ((opt = getopt(*i, argv, ":s:u:")))
       {
           switch (opt)
           {
           case 's':
               set_shopt_opt(mj->shopt_opt, optarg, 1);
               break;
           case 'u':
               set_shopt_opt(mj->shopt_opt, optarg, 0);
               break;
           case ':':
               warnx("option needs a value");
               break;
           case '?':
               warnx("unknown option: %c", optopt);
               break;
           }
       }
   */
}

int b_shopt_options(struct major *mj, char *arg)
{}