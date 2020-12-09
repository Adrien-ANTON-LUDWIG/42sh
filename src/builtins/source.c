#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "b_source.h"

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

static char *iter_dir(char *base_path, char *dest_path)
{
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if (!dir)
        return NULL;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if (dp->d_type == DT_DIR)
            {
                char *rec_path = base_path;
                strcpy(rec_path, base_path);
                strcat(rec_path, "/");
                strcat(rec_path, dp->d_name);
                rec_path = iter_dir(rec_path, dest_path);
                if (rec_path)
                {
                    closedir(dir);
                    return rec_path;
                }
            }

            else if (!strcmp(dp->d_name, dest_path))
            {
                char *ret = base_path;
                strcat(ret, "/");
                strcat(ret, dp->d_name);
                closedir(dir);
                return ret;
            }
        }
    }
    closedir(dir);
    return NULL;
}

static char *get_path(char *str)
{
    if (strstr(str, "/") != NULL)
        return str;

    char *PATH = getenv("PATH");
    char *index = PATH;

    while (index)
    {
        char *next_path = strstr(index, ":");

        char *actual_path = strndup(index, next_path - index);
        char *found = iter_dir(actual_path, str);
        if (found)
            return found;

        index = strstr(index, ":");

        if (index)
            index++;

        free(actual_path);
        actual_path = NULL;
    }

    return "test.txt";
}

char *source(char *argv[])
{
    int argc = argv_len(argv);

    if (argc < 2)
    {
        warnx("filename required");
        return "2";
    }

    char *path = get_path(argv[1]);

    return path;
}