#define _GNU_SOURCE

#include "export.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"

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
        i += (nb_options) ? 0 : 1;
    }
    nb_options = i - 1;
    return nb_options;
}

static void export_display(void)
{
    int len = 0;
    char c = ' ';
    for (int i = 0; environ[i]; i++)
    {
        printf("export ");
        int found_equal = 0;
        len = strlen(environ[i]);
        for (int j = 0; j < len; j++)
        {
            c = environ[i][j];
            if (c == '=' && j < len - 1 && !found_equal)
            {
                found_equal = 1;
                printf("=\"");
            }
            else if (j == len - 1)
            {
                if (environ[i][j] != '=')
                    printf("%c", environ[i][j]);
                if (found_equal)
                    printf("\"");
            }
            else
            {
                printf("%c", environ[i][j]);
            }
        }
        printf("\n");
    }
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
    {
        if (strstr(argv[i], "=") == NULL)
            setenv(argv[i], "", 0);
        else
            putenv(argv[i]);
    }
}

int b_export(char *argv[])
{
    if (!argv)
        return 1;

    int p = 0;
    int n = 0;
    int f = 0;

    int len = argv_len(argv);

    int nb_opt = set_options(argv, &p, &n, &f);

    if ((p && !f && nb_opt == len - 1) || !len)
        export_display();
    else if (n && !p && !f)
        export_remove(len, nb_opt, argv);
    else
        export_putenv(len, nb_opt, argv);
    return 0;
}