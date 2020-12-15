#include "argument_handler.h"
#include "b_utils.h"
#include "dollar.h"

char *dollar_star(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_star: struct major should exist at this point");

    *len_var = 2;
    int argc = argv_len(mj->arguments);
    char *str = merge_arguments(argc, mj->arguments);

    if (!str)
        str = strdup("");

    return str;
}