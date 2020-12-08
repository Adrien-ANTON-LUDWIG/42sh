#include "exit.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void argv_free(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        free(argv[(i++)]);
    free(argv);
}

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

int b_exit(struct major *mj, char *argv[])
{
    if (!mj || !argv)
        return -1;

    int len = argv_len(argv);

    if (len > 2)
    {
        warnx("too many arguments\n");
        return 1;
    }

    int rvalue = 0;

    if (argv && *(argv + 1))
        rvalue = atoi(*(argv + 1));

    major_free(mj);
    argv_free(argv);

    exit(rvalue);
}