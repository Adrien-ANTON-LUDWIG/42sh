#include "echo_utils.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "b_utils.h"

#define CHAR_ZERO 48
#define TO_UPPER 32

int set_options(char *argv[], int *n, int *e, int *E)
{
    int c = 0;
    int len = argv_len(argv);
    int stop = 0;
    optind = 0;
    opterr = 0;

    while (!stop && (c = getopt(len, argv, ":neE")) != -1)
    {
        switch (c)
        {
        case 'n':
            *n = 1;
            break;
        case 'e':
            *e = 1;
            break;
        case 'E':
            *E = 1;
            break;
        default:
            optind -= 1;
            stop = 1;
            break;
        }
    }
    return optind - 1;
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
