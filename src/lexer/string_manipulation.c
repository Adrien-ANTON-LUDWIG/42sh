#include "string_manipulation.h"

#include <stddef.h>

#include "custom_descriptor.h"

int is_in(char c, char *s)
{
    for (size_t i = 0; s[i]; i++)
        if (c == s[i])
            return 1;
    return 0;
}

int is_not_in(char c, char *s)
{
    return !is_in(c, s);
}

int at_end(struct custom_FILE *f)
{
    return f->lexer_index >= f->len;
}

char get_char(struct custom_FILE *f, int incr)
{
    f->lexer_index += incr; // YOU made me do this, moulinette
    return f->str[f->lexer_index - incr];
}

void skip(struct custom_FILE *f, int (*predicate)(char, char *), char *s)
{
    while (!at_end(f) && predicate(get_char(f, 0), s))
        f->lexer_index++;
}