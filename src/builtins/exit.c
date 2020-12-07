#include "exit.h"

#include <stdlib.h>

void b_exit(struct major *mj, char *argv[])
{
    int rvalue = 0;

    if (argv && *argv)
        rvalue = atoi(*argv);

    major_free(mj);

    if (argv)
        free(*argv);
        
    free(argv);

    exit(rvalue);
}