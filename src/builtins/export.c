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
        return 1;

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
        i += (nb_options) ? 0 : 1;
    }
    nb_options = i - 1;
    return nb_options;
}

static void export_display(void)
{
    for (int i = 0; environ[i]; i++)
        printf("declare -x %s\n", environ[i]);
}

static void export_remove(int len, int nb_opt, char *argv[])
{
    for (int i = nb_opt + 1; i < len; i++)
    {
        unsetenv(argv[i]);
    }
}

static void export_putenv(int len, int nb_opt, char *argv[])
{
    for (int i = nb_opt + 1; i < len; i++)
        putenv(argv[i]);
}

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

// Implement -f
// When exporting stg vithou a value (toto vs toto=incredible), writes it
// "toto=" and not "toto"

int b_export(struct major *mj, char *argv[])
{
    if (!mj || !argv)
        return 1;

    int p = 0;
    int n = 0;
    int f = 0;

    int len = argv_len(argv);
    int nb_opt = set_options(argv, &p, &n, &f);

    if ((p && !f && nb_opt == len - 1) || len == 1)
        export_display();
    else if (n && !p && !f)
        export_remove(len, nb_opt, argv);
    else
        export_putenv(len, nb_opt, argv);

    return 0;
}