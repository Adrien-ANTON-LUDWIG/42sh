#define _GNU_SOURCE
#include "export.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
    -f refer to shell functions
    -n remove the export property from each NAME
    -p display a list of all exported variables and functions
*/

static int set_options(char *argv[], int *p, int *n, int *f)
{
    if (!argv)
        return 0;

    int nb_options = 0;
    int i = 1;
    while (argv[i] && !nb_options)
    {
        if (!strcmp(argv[i], "-p"))
            *p = 1;
        else if (!strcmp(argv[i], "-n"))
            *n = 1;
        else if (!strcmp(argv[i], "-f"))
            *f = 1;
        else
            nb_options = i;
        i++;
    }
    return nb_options;
}

static void export_display(void)
{
    for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
}

static void export_remove(int len, int nb_opt, char *argv[])
{
    for (; nb_opt < len; nb_opt++)
    {
        unsetenv(argv[nb_opt]);
    }
}

int b_export(struct major *mj, char *argv[])
{
    if (!mj || argv)
        return -1;

    int p = 0;
    int n = 0;
    int f = 0;

    int len = sizeof(*argv) / sizeof(char *);

    int nb_opt = set_options(argv, &p, &n, &f);

    if (p && !f && nb_opt == len - 1)
        export_display();
    else if (n && !p && !f)
        export_remove(len, nb_opt, argv);

    for (; nb_opt < len; nb_opt++)
        putenv(argv[1]);

    return 0;
}