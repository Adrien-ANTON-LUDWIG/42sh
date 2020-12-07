#include "exit.h"

#include <stdlib.h>

int b_exit(struct major *mj, char *argv[])
{
    if (!mj || !argv)
        return -1;

    int rvalue = 0;

    if (argv && *argv)
        rvalue = atoi(*argv);

    major_free(mj);

    if (argv)
        free(*argv);

    free(argv);

    exit(rvalue);
}