#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"

char *update_path(char *path, char *section)
{
    char *new = NULL;

    if (!strcmp(section, ".."))
    {
        char *last_slash = strrchr(path, '/');
        *last_slash = '\0';
        new = strdup(path);
    }
    else if (!strcmp(section, "~"))
        new = strdup(getenv("HOME"));
    else if (!strcmp(section, "/"))
        new = strdup("/");
    else if (!strcmp(section, "-"))
    {
        printf("%s\n", getenv("OLDPWD"));
        new = strdup(getenv("OLDPWD"));
    }
    else
    {
        new = strdup(path);
        new = realloc(new, sizeof(char) * (strlen(new) + strlen(section) + 2));
        if (new[strlen(new) - 1] != '/')
            strcat(new, "/");
        strcat(new, section);
    }

    free(path);
    return new;
}

static char *get_path_destination(int argc, char **argv)
{
    char *pwd = getenv("PWD");

    if (!pwd)
        pwd = getenv("HOME");

    char *path = strdup(pwd);

    if (!argc || argv[0][0] == '~')
        path = update_path(path, "~");
    else if (strstr(argv[0], "-"))
    {
        argc = 0;
        if (strlen(argv[0]) == 1)
            path = update_path(path, "-");
        else
        {
            warnx("No such file or directory");
            free(path);
            path = strdup(getenv("PWD"));
        }
    }
    else if (argv[0][0] == '/')
        path = update_path(path, "/");

    char *path_to_iter = argv[0];
    char *next_section = NULL;

    while (argc && (next_section = strtok(path_to_iter, "/")) != NULL)
    {
        path = update_path(path, next_section);
        path_to_iter = NULL;
    }

    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", path, 1);

    return path;
}

int b_cd(char **argv)
{
    int i = argv_len(argv);

    if (i > 2)
        errx(1, "cd: too many arguments");

    char *path = get_path_destination(i - 1, argv + 1);

    int r_value = chdir(path);
    free(path);
    fflush(stdout);
    if (r_value != 0)
    {
        warnx("No such file or directory");
        return 1;
    }
    return 0;
}