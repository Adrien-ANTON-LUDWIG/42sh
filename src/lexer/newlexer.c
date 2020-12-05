#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "custom_descriptor.h"
#include "lexer.h"
#include "lexer_in.h"
#include "my_utils.h"
#include "my_xmalloc.h"
#include "redirection.h"
#include "tokens.h"

#define IS_NEWLINE "\n\r"
#define MY_IS_SPACE " \t\v"
#define IS_NOT_WORD "\0 \t\v\n\r;&|<>()"
#define IS_OPERATOR "&|()<>"

static int is_in(char c, char *s)
{
    for (size_t i = 0; s[i]; i++)
        if (c == s[i])
            return 1;
    return 0;
}

static is_not_in(char c, char *s)
{
    return !is_in(c, s);
}

static int at_end(struct custom_FILE *f)
{
    return f->lexer_index >= f->len;
}

static char get_char(struct custom_FILE *f)
{
    return f->str[f->lexer_index];
}

static void skip(struct custom_FILE *f, int (*predicate)(char, char *), char *s)
{
    while (!at_end(f) && predicate(get_char(f), s))
        f->lexer_index++;
}

struct token *get_operator(struct major *mj)
{
    struct custom_FILE *f = mj->file;

    size_t start = f->lexer_index;

    skip(f, is_in, IS_OPERATOR);
}

struct token *get_next_token(struct major *mj)
{
    struct custom_FILE *f = mj->file;

    while (!at_end(f)
           && !(is_in(get_char(f), IS_OPERATOR)
                || is_not_in(get_char(f), IS_NOT_WORD)))
    {
        skip(f, is_in, MY_IS_SPACE);
        skip(f, is_in, IS_NEWLINE);

        if (!at_end(f) && get_char(f) == '#')
            skip(f, is_not_in, IS_NEWLINE);

        if (at_end(f))
            if (!(custom_getline(mj)))
                return token_init(mj);
    }

    if (is_in(get_char(f), IS_OPERATOR))
        return get_token_operator(mj);

    return get_token_word(mj);
}