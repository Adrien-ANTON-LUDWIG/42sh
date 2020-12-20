#define _POSIX_C_SOURCE 200809L

#include "cd.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"

static char *get_home(void)
{
    char *home = getenv("HOME");
    if (!home)
    {
        char *usr = getenv("USER");

        if (!usr)
            return strdup("/home/");

        home = malloc(strlen("/home/") + strlen(usr) + 1);
        home = strcpy(home, "/home/");
        home = strcat(home, usr);
    }
    else
        home = strdup(home);
    return home;
}

static char *get_pwd(void)
{
    char *pwd = getenv("PWD");
    char *p = NULL;
    if (!pwd)
    {
        pwd = get_home();
        setenv("PWD", pwd, 1);
    }
    else
        pwd = strdup(pwd);
    p = strdup(pwd);
    free(pwd);
    return p;
}

static char *get_oldpwd(void)
{
    char *oldpwd = getenv("OLDPWD");
    if (!oldpwd)
    {
        warnx("cd: OLDPWD not set");
        return NULL;
    }
    return oldpwd;
}

static char *handle_minus(int *argc, char **argv, char *pwd, char *path)
{
    *argc = 0;
    if (strlen(argv[0]) == 1)
    {
        path = update_path(path, "-");
        if (!path)
            return NULL;
    }
    else
    {
        warnx("No such file or directory");
        free(path);
        path = strdup(pwd);
    }
    return path;
}

static char *handle_pointpoint(char *path)
{
    char *last_slash = strrchr(path, '/');
    if (last_slash)
        *last_slash = '\0';
    if (strlen(path) == 0)
        return strdup("/");
    else
        return strdup(path);
}

char *update_path(char *path, char *section)
{
    char *new = NULL;

    if (!strcmp(section, ".."))
        new = handle_pointpoint(path);
    else if (!strcmp(section, "~"))
        new = get_home();
    else if (!strcmp(section, "/"))
        new = strdup("/");
    else if (!strcmp(section, "-"))
    {
        char *oldpwd = get_oldpwd();
        if (!oldpwd || !*oldpwd)
        {
            free(path);
            return NULL;
        }
        printf("%s\n", oldpwd);
        new = strdup(oldpwd);
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
    char *pwd = get_pwd();
    char *path = strdup(pwd);

    if (!argc || argv[0][0] == '~')
        path = update_path(path, "~");
    else if (argv[0][0] == '-')
        path = handle_minus(&argc, argv, pwd, path);
    else if (argv[0][0] == '/')
        path = update_path(path, "/");
    char *path_to_iter = argv[0];
    char *next_section = NULL;

    while (argc && (next_section = strtok(path_to_iter, "/")) != NULL)
    {
        path = update_path(path, next_section);
        path_to_iter = NULL;
    }

    setenv("OLDPWD", pwd, 1);
    if (path)
        setenv("PWD", path, 1);
    free(pwd);

    return path;
}

int b_cd(char **argv)
{
    int i = argv_len(argv);

    if (i > 2)
        errx(1, "cd: too many arguments");

    char *path = get_path_destination(i - 1, argv + 1);

    if (!path)
        return 1;

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