#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NB_ESCAPE_SEQ 8
#define CHAR_ESCAPE                                                            \
    {                                                                          \
        'a', 'b', 'e', 'f', 'n', 'r', 't', 'v'                                 \
    }
#define STRING_ESCAPE                                                          \
    {                                                                          \
        '\a', '\b', '\e', '\f', '\n', '\r', '\t', '\v'                         \
    }

static int nb_to_read(char *s, int i)
{
    int j = i;
    for (; j < i + 3 && s[j]; j++)
    {
        if ('0' > s[j] || s[j] > '7')
            break;
    }
    return j - i;
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
        printf("pow = %d\n", to_read - j - 1);
        n += (((int)(s[i + j]) - 48) * my_pow(8, to_read - j - 1));
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
            *e = 1;
        else if (!strcmp(argv[i], "-E"))
            *E = 1;
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
    for (int i = 0; i < NB_ESCAPE_SEQ; i++)
    {
        if (c == char_escape[i])
            return i;
    }
    return -3;
}

static void echo_display(char *argv, int e, int *n)
{
    char c = ' ';
    char str_escape[] = STRING_ESCAPE;
    for (size_t i = 0; i < strlen(argv); i++)
    {
        c = argv[i];
        if (e && c == '\\')
        {
            i++;
            int index = get_escape_index(argv[i]);

            if (index == -1)
            {
                *n = 1;
                return;
            }
            if (index == -2)
            {
                int to_read = nb_to_read(argv, i);

                int ascii = str_oct_to_dec(argv + 1, i, to_read);
                printf("%c", ascii);
                i += to_read;
            }
            if (index == -3)
            {
                putchar(c);
                putchar(argv[i]);
            }
            else
                printf("%c", str_escape[index]);
        }
        else
        {
            putchar(argv[i]);
        }
    }
}

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

/*
    -n do not output the trailing newline

    -e enable interpretation of backslash escapes

    -E disable interpretation of backslash escapes (default)
*/

int main(int argc, char **argv)
{
    // printf("%c", atoi("041"));
    // printf("%o\n", atoi("33"));
    if (argc < 2)
    {
        printf("\n");
        return 1;
    }

    int n = 0;
    int e = 0;
    int E = 0;

    int nb_opt = set_options(argv, &n, &e, &E);
    int len = argv_len(argv + 1);

    if (nb_opt == 0)
        E = 1;

    printf("len = %d\n", len);
    printf("nb_opt: %d\n", nb_opt);
    printf("n=%d\ne=%d\nE=%d\n", n, e, E);

    echo_display(argv[nb_opt + 1], e, &n);

    if (!n)
        printf("\n");

    return nb_opt;
}