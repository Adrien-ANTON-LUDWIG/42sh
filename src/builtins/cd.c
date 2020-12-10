#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
Options:
      -@	on systems that support it, present a file with extended
            attributes as a directory containing the file attributes
*/

// Argv should have "cd" in first place in this implementation
// main cd..
char *update_path(char *path, char *section)
{
    printf("path = %s, section = %s\n", path, section);
    char *new = NULL;
    if (!strcmp(section, ".."))
    {
        char *last_slash = strrchr(path, '/');
        *last_slash = '\0';
        new = strdup(path);
    }
    else if (!strcmp(section, "~"))
    {
        new = strdup(getenv("HOME"));
    }
    else
    {
        new = strdup(path);
        new = realloc(new, sizeof(char) * (strlen(new) + strlen(section) + 2));
        if (new[strlen(new) - 1] != '/')
            strcat(new, "/");
        strcat(new, section);
    }
    printf("new = %s\n", new);
    return new;
}

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

static char *get_path_destination(char **argv, int i)
{
    char *path = strdup(getenv("PWD"));
    char *index = argv[0];

    if (i == 0)
        path = update_path(path, "~");

    printf("[get_path_dest] %s\n", index);
    while (i == 1 && index)
    {
        char *next_path = strstr(index, "/");

        printf("[get_path_dest] len: %ld\n", next_path - index);
        char *actual_path = strndup(index, next_path - index);
        char *new_path = update_path(path, actual_path);

        if (!new_path)
        {
            free(actual_path);
            return strdup("/");
        }

        index = strstr(index, "/");

        if (index)
            index++;

        printf("[get_path_dest] index after strstr: %s\n", index);
        path = realloc(path, strlen(new_path) * sizeof(char) + 1);
        path = strcpy(path, new_path);

        free(actual_path);
        free(new_path);

        new_path = NULL;
        actual_path = NULL;
    }

    return path;
}

char *b_cd(char **argv)
{
    int i = argv_len(argv);

    if (i > 2)
    {
        printf("%d\n", i);
        errx(1, "cd: too many arguments");
    }

    printf("i = %d\n", i);
    return get_path_destination(argv + 1, i - 1);
}

int main(int argc, char *argv[])
{
    if (!argv || argc < 0)
        return -1;

    char *path = b_cd(argv);
    printf("[main]path: %s\n", path);
    printf("[main]PWD: %s\n", getenv("PWD"));
    printf("[main]OLDPWD: %s\n", getenv("OLDPWD"));
    int r_value = chdir(path);

    if (r_value != 0)
    {
        free(path);
        return r_value;
    }

    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", path, 1);

    errno = 0;
    FILE *file = fopen("filetest.h", "w+");

    if (!file)
        printf("[main]%s\n", strerror(errno));
    else
        fclose(file);

    printf("After chdir\n");
    printf("[main]PWD: %s\n", getenv("PWD"));
    printf("[main]OLDPWD: %s\n", getenv("OLDPWD"));

    free(path);

    return r_value;
}