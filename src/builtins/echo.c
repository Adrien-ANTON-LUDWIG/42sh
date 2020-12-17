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
#include "string_manipulation.h"

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

static int get_ascii_conversion(char *argv, size_t *i, int index)
{
    int to_read = 0;
    int tmp = *i;
    if (index == -2)
    {
        to_read = (nb_to_read_oct(argv, tmp));
        *i += to_read;
        return str_oct_to_dec(argv + 1, tmp, to_read);
    }
    else
    {
        to_read = nb_to_read_hx(argv, tmp);
        *i += to_read;
        return str_hx_to_dec(argv + 1, tmp, to_read);
    }
    return 42;
}

static int should_interpret(char c)
{ // add e and c ?
    if (is_in(c, "abfrv"))
        return -1;
    else if (is_in(c, "tn"))
        return (c == 'n') ? 3 : 5;
    else
        return 0;
}

static int strong_quotes(char *argv, size_t i, size_t len, int e)
{
    char str_escape[] = STRING_ESCAPE;
    int to_interpret = 0;

    while (i < len)
    {
        char c = argv[i];
        if (c == '\'')
            return i + 1;

        else if (e && c == '\\' && i++ < len)
        {
            c = argv[i];
            to_interpret = should_interpret(c);
            if (!to_interpret)
                printf("\\%c", c);
            else if (to_interpret != -1)
                printf("%c", str_escape[to_interpret]);
        }
        else
            printf("%c", argv[i]);
        i++;
    }
    return i;
}

static int weak_quotes(char *argv, size_t i, size_t len, int e)
{
    char str_escape[] = STRING_ESCAPE;
    char c;
    int escape = 0;
    while (i < len)
    {
        c = argv[i];
        if (c == '\"')
            return i + 1;
        else if (!e && c == '\\' && i + 1 < len && argv[i + 1] != '\\')
        {
            printf("\\%c", argv[i + 1]);
            i += 2;
        }
        else if (c == '\\' && ++i)
        {
            if (i < len)
                c = argv[i];
            escape = get_escape_index(c);
            /*if (escape == -1 && (*n = 1))
                return;*/
            if (escape == -2 || escape == -3)
                printf("%c", get_ascii_conversion(argv, &i, escape));
            else if (escape == -4)
                printf("%c", ESCAPE_CHAR);
            else if (escape == -5)
                printf("\\%c", c);
            else
                printf("%c", str_escape[escape]);
            i++;
        }
        else
        {
            printf("%c", c);
            i++;
        }
    }
    return i;
}

static void echo_display(char *argv, int e, int *n)
{
    char str_escape[] = STRING_ESCAPE;
    size_t len = strlen(argv);
    size_t i = 0;
    int escape = 0;
    while (i < len)
    {
        char c = argv[i];
        if (c == '\'')
            i = strong_quotes(argv, i + 1, len, e);
        else if (c == '\"')
            i = weak_quotes(argv, i + 1, len, e);
        else if (c == '\\' && i + 1 < len && (c = argv[++i]) && e && c == '\\'
                 && i++)
        {
            if (i < len)
                c = argv[i];
            if (c == '\\')
            {
                continue;
            }
            escape = get_escape_index(c);
            if (escape == -1 && (*n = 1))
                return;
            else if (escape == -2 || escape == -3)
                printf("%c", get_ascii_conversion(argv, &i, escape));
            else if (escape == -4)
                printf("%c", ESCAPE_CHAR);
            else if (escape == -5)
                printf("\\%c", c);
            else
                printf("%c", str_escape[escape]);
            i++;
        }
        else
        {
            printf("%c", c);
            i++;
        }
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