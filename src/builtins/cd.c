#define _POSIX_C_SOURCE 200809L
#include "cd.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "my_err.h"

/*
Options:
      -@	on systems that support it, present a file with extended
            attributes as a directory containing the file attributes
*/

// Argv should have "cd" in first place in this implementation
static char *get_path_destination(struct major *mj, char **argv)
{
    int i = 0;
    if (!argv)
        my_err(2, mj, "get_path_destination: no argument specified");

    while (argv[i])
        i++;

    if (i > 2)
        my_err(1, mj, "cd: too many arguments");

    char *path = NULL;

    if (i == 0)
        path = strdup("~");
    else if (!strcmp("-", argv[i - 1]))
        path = mj->oldpwd_dir;
    else
        path = strdup(argv[i - 1]);

    return path;
}

void cd(struct major *mj, char **argv)
{
    char *path = get_path_destination(mj, argv);
    int r_value = chdir(path);
    if (r_value == 0)
    {
        if (mj->oldpwd_dir)
            free(mj->oldpwd_dir);
        mj->oldpwd_dir = strdup(mj->pwd_dir);

        if (mj->pwd_dir)
            free(mj->pwd_dir);

        // Does not work
        mj->pwd_dir = strdup(path);
    }

    exit(r_value);
}