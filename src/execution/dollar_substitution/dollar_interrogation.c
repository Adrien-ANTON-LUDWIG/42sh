#include <stdio.h>

#include "dollar.h"

char *dollar_interrogation(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_sharp: struct major should exist at this point");
    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);

    sprintf(str, "%d", mj->rvalue);

    return str;
}