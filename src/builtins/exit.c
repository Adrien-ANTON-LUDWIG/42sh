#include "exit.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "b_utils.h"
#include "my_err.h"

static void argv_free(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        free(argv[(i++)]);
    free(argv);
}

int b_exit(struct major *mj, char *argv[])
{
    if (!mj || !argv)
        return -1;

    int len = argv_len(argv);

    if (len > 2)
    {
        warnx("too many arguments");
        return 1;
    }

    int rvalue = 0;
    char *endptr = NULL;

    if (argv && *(argv + 1))
        rvalue = strtol(*(argv + 1), &endptr, 10);

    if (endptr && *endptr != '\0')
        my_err(2, mj, "exit: numeric argument required");

    argv_free(argv);
    major_free(mj);
    exit(rvalue);
}