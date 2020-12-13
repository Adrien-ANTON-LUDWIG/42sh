#include "echo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "argument_handler.h"
#include "b_utils.h"
#include "echo_utils.h"
#include "my_err.h"

#define ESCAPE_CHAR 27
#define NB_ESCAPE_SEQ 7

#define CHAR_ESCAPE                                                            \
    {                                                                          \
        'a', 'b', 'f', 'n', 'r', 't', 'v'                                      \
    }
#define STRING_ESCAPE                                                          \
    {                                                                          \
        '\a', '\b', '\f', '\n', '\r', '\t', '\v'                               \
    }

static int get_escape_index(char c)
{
    char char_escape[] = CHAR_ESCAPE;
    if (c == 'c')
        return -1;
    if (c == '0')
        return -2;
    if (c == 'x')
        return -3;
    if (c == 'e')
        return -4;
    for (int i = 0; i < NB_ESCAPE_SEQ; i++)
    {
        if (c == char_escape[i])
            return i;
    }
    return -5;
}

static size_t set_hard_quoting(int *hard_quoting, int i, char c, int e)
{
    if (c == '\'')
    {
        *hard_quoting = e ? 0 : !*hard_quoting;
        i++;
    }
    return i;
}

static int get_ascii_conversion(char *argv, size_t *i, int index)
{
    int to_read = 0;
    if (index == -2)
    {
        to_read = (nb_to_read_oct(argv, *i));
        *i += to_read;
        return str_oct_to_dec(argv + 1, *i, to_read);
    }
    else
    {
        to_read = nb_to_read_hx(argv, *i);
        *i += to_read;
        return str_hx_to_dec(argv + 1, *i, to_read);
    }
    return 42;
}

static void echo_display(char *argv, int e, int *n)
{
    char str_escape[] = STRING_ESCAPE;
    char c = ' ';
    int hard_quoting = 0;
    size_t len = strlen(argv);
    for (size_t i = 0; i < len; i++)
    {
        c = argv[i];
        i = set_hard_quoting(&hard_quoting, i, c, e);

        if (!hard_quoting && e && c == '\\' && argv[++i] == '\\' && i++)
        {
            int index = get_escape_index(argv[i]);
            if (index == -1 && (*n = 1))
                return;
            else if (index == -2 || index == -3)
                printf("%c", get_ascii_conversion(argv, &i, index));
            else if (index == -4)
                printf("%c", ESCAPE_CHAR);
            else if (index == -5)
                printf("%c%c", c, argv[i]);
            else
                printf("%c", str_escape[index]);
        }
        else if ((i < len)
                 && ((!hard_quoting && argv[i] != '\\' && argv[i] != '\'')
                     || (hard_quoting && argv[i] != '\'')))
            putchar(argv[i]);
    }
}

int b_echo(char **argv)
{
    struct stat statbuff;
    if (fstat(STDOUT_FILENO, &statbuff) == -1)
        return 1;
    int argc = argv_len(argv);

    if (argc < 2)
    {
        printf("\n");
        return 0;
    }

    int n = 0;
    int e = 0;
    int E = 0;
    int nb_opt = set_options(argv, &n, &e, &E);
    char *str = merge_arguments(argc - nb_opt - 1, argv + nb_opt + 1);

    echo_display(str, e, &n);
    free(str);
    if (!n)
        printf("\n");

    fflush(stdout);
    return 0;
}