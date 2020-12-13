#include "dollar.h"

char *dollar_oldpwd(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_sharp: struct major should exist at this point");
    *len_var = 7;
    char *oldpwd = getenv("OLDPWD");

    if (!oldpwd)
        oldpwd = "";

    char *str = strdup(oldpwd);
    return str;
}