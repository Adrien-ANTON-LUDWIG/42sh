#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "dollar.h"

char *dollar_uid(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_uid: struct major should exist at this point");
    *len_var = 4;

    int pid = getuid();

    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);
    sprintf(str, "%d", pid);
    return str;
}