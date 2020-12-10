#include "b_utils.h"

int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}