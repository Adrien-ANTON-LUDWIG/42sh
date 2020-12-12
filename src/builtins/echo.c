#include "echo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argument_handler.h"
#include "b_utils.h"

#define CHAR_ZERO 48
#define TO_UPPER 32
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

static int nb_to_read_oct(char *s, int i)
{
    int j = i + 1;
    for (; j < i + 4 && s[j]; j++)
    {
        if (s[j] < '0' || '7' < s[j])
        {
            break;
        }
    }
    return j - i - 1;
}

static int nb_to_read_hx(char *s, int i)
{
    int j = i + 1;
    for (; j < i + 3 && s[j]; j++)
    {
        if ((s[j] < '0' || '9' < s[j]) && (s[j] < 'A' || 'F' < s[j])
            && (s[j] < 'a' || 'f' < s[j]))
        {
            break;
        }
    }
    return j - i - 1;
}

static int my_pow(int a, int b)
{
    int res = 1;
    for (int i = 0; i < b; i++)
        res *= a;
    return res;
}

static int str_oct_to_dec(char *s, int i, int to_read)
{
    int n = 0;
    for (int j = 0; j < to_read; j++)
    {
        n += (((int)(s[i + j]) - CHAR_ZERO) * my_pow(8, to_read - j - 1));
    }
    return n;
}

static int get_hex_val(char *c)
{
    if ('a' <= *c && *c <= 'f')
        *c -= TO_UPPER;
    int ret = 0;
    if (*c >= '0' && '9' >= *c)
        ret = *c - '0';
    else
        ret = *c - 'A' + 10;
    return ret;
}

static int str_hx_to_dec(char *s, int i, int to_read)
{
    int n = 0;
    for (int j = 0; j < to_read; j++)
    {
        n += (get_hex_val(s + i + j)) * my_pow(16, to_read - j - 1);
    }
    return n;
}

static int set_options(char *argv[], int *n, int *e, int *E)
{
    if (!argv)
        return 1;

    int nb_options = 0;
    int i = 1;
    while (argv[i] && !nb_options)
    {
        if (!strcmp(argv[i], "-n"))
            *n = 1;
        else if (!strcmp(argv[i], "-e"))
            *e = (*E == 0);
        else if (!strcmp(argv[i], "-E"))
        {
            *E = 1;
            *e = 0;
        }
        else
            nb_options = i;
        i += (nb_options) ? 0 : 1;
    }

    nb_options = i - 1;
    return nb_options;
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

static void echo_display(char *argv, int e, int *n)
{
    char str_escape[] = STRING_ESCAPE;
    char c = ' ';
    for (size_t i = 0; i < strlen(argv); i++)
    {
        c = argv[i];
        if (e && c == '\\' && i++ && argv[i] == '\\')
        {
            i++;
            int index = get_escape_index(argv[i]);
            if (index == -1 && (*n = 1))
                return;
            else if (index == -2 || index == -3)
            {
                int to_read = (index == -2) ? nb_to_read_oct(argv, i)
                                            : nb_to_read_hx(argv, i);
                int ascii = (index == -2) ? str_oct_to_dec(argv + 1, i, to_read)
                                          : str_hx_to_dec(argv + 1, i, to_read);
                printf("%c", ascii);
                i += to_read;
            }
            else if (index == -4)
                printf("%c", ESCAPE_CHAR);
            else if (index == -5)
            {
                putchar(c);
                putchar(argv[i]);
            }
            else
                printf("%c", str_escape[index]);
        }
        else
            putchar(argv[i]);
    }
}

/*
    -n do not output the trailing newline

    -e enable interpretation of backslash escapes

    -E disable interpretation of backslash escapes (default)
*/
int b_echo(char **argv)
{
    fflush(stdout);
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