#include "continue.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "b_utils.h"
#include "my_err.h"

int b_continue(struct major *mj, char **argv)
{
    if (!mj || !argv)
        my_err(2, mj, "b_continue: arguments needed");

    if (mj->loop_counter == 0)
        return 0;

    int len = argv_len(argv);

    if (len > 2)
        my_err(128, mj, "b_continue: too many arguments");

    if (len == 1)
    {
        mj->break_counter = 1;
        return 0;
    }

    char *endptr = NULL;

    int n = strtol(argv[1], &endptr, 10);

    if (!endptr || *endptr != '\0')
        my_err(128, mj, "b_continue: numeric argument required");

    if (n <= 0)
    {
        mj->break_counter = BREAK_AST;
        return my_soft_err(mj, 1, "b_continue: loop count out of range");
    }

    mj->continue_counter = MIN(n, mj->loop_counter);

    return 0;
}