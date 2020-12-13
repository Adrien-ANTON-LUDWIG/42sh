#include "b_utils.h"
#include "dollar.h"

char *dollar_sharp(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_sharp: struct major should exist at this point");

    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);
    sprintf(str, "%d", argv_len(mj->arguments));

    return str;
}