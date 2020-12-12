#include "dollar.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char *dollar_dollar(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_sharp: struct major should exist at this point");

    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(long int) + 1);

    int pid = getpid();

    sprintf(str, "%d", pid);

    return str;
}