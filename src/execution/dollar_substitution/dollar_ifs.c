#include "dollar.h"

char *dollar_ifs(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_ifs: struct major should exist at this point");
    *len_var = 4;
    char *str = strdup(" \t\n");
    return str;
}