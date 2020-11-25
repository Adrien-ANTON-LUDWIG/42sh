#include "lexer.h"

#include <ctype.h>

#define BASIC_SEPARATOR "\r\v\n\t "
#define COMMAND_SEPARTOR ";\n\t"

int my_is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\v';
}

int is_word(int c)
{
    return c != '\0' && !my_isspace(c) && c != ';' && c != '\n' && c != '\r';
}

void skip_class(int (*classifier)(int), char **cursor)
{
    while (classifier(**cursor))
        (*cursor)++;
}

static char *get_word(char **s)
{
    skip_class(my_is_space, s);
    char *start = *s;
    skip_class(is_word, s);
    char *end = *s;

    size_t len = end - start;
    char *word = strndup(start, len);

    skip_class(my_is_space, s);

    return word;
}

static void _lexer_build(struct major *mj, struct lexer *lex, char *s)
{
    int is_command = 0;
    struct token *tk;
    while (*s)
    {
        char *word = get_word(&s);

        if (is_command)
        {
            list_append(mj, tk->data, word);
            if (*s == ';' || *s == '\n')
            {
                is_command = 0;
            }
        }
        else
        {
            tk = token_init(mj);
            int token = token_get(word);
            tk->word = token;
            if (token == WORD_COMMAND)
            {
                is_command = 1;
                list_append(mj, tk->data, word);
            }
        }
        if (*s == ';' || *s == '\n')
            s++;
    }
}

struct lexer *lexer_build(struct major *mj, char *s)
{
    struct lexer *lex = lexer_init(mj);
    struct token *tk_eof = token_init(mj);

    tk_eof->word = WORD_EOF;

    if (!s)
    {
        lexer_append(mj, lex, tk_eof);
        return lex;
    }

    _lexer_build(mj, lex, s);

    lexer_append(mj, lex, tk_eof);
    return lex;
}