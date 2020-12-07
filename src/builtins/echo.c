#include "echo.h"

/*
    -n do not output the trailing newline

    -e enable interpretation of backslash escapes

    -E disable interpretation of backslash escapes (default)
*/

// Argv should have echo in first place for this implementation
int echo(struct major *mj, char **argv)
{
    if (!mj || !argv)
        return -1;
    return 0;
}