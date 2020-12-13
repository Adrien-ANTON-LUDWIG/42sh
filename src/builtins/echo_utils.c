#include "echo_utils.h"

#include <string.h>

#define CHAR_ZERO 48
#define TO_UPPER 32

int set_options(char *argv[], int *n, int *e, int *E)
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

int nb_to_read_oct(char *s, int i)
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

int nb_to_read_hx(char *s, int i)
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

int str_oct_to_dec(char *s, int i, int to_read)
{
    int n = 0;
    for (int j = 0; j < to_read; j++)
    {
        n += (((s[i + j]) - CHAR_ZERO) * my_pow(8, to_read - j - 1));
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

int str_hx_to_dec(char *s, int i, int to_read)
{
    int n = 0;
    for (int j = 0; j < to_read; j++)
    {
        n += (get_hex_val(s + i + j)) * my_pow(16, to_read - j - 1);
    }
    return n;
}
