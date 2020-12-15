#include <stdlib.h>
#include <time.h>

#include "dollar.h"

#define MAX_RANDOM 32768

static int first_time = 0;

char *dollar_random(struct major *mj, int *len_var)
{
    if (!mj)
        my_err(1, mj, "dollar_random: struct major should exist at this point");
    *len_var = 7;
    if (!first_time)
    {
        first_time = 1;
        srand(time(NULL));
    }

    int random = rand() % MAX_RANDOM;

    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 10);

    sprintf(str, "%d", random);

    return str;
}