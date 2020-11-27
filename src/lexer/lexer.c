#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "redirection.h"

#define BASIC_SEPARATOR "\r\v\n\t "
#define COMMAND_SEPARTOR ";\n\t"

/**
 * @brief Returns 1 if a given char is a space or equivalent. Else return 0.
 *
 * @param c
 * @return int
 */
int my_is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\v';
}

/**
 * @brief Returns 1 if a given char is not a space of separator. Else return 0;
 *
 * @param c
 * @return int
 */
int is_word(int c)
{
    return c != '\0' && !my_is_space(c) && c != ';' && c != '\n' && c != '\r';
}

/**
 * @brief
 *
 * @param classifier
 * @param cursor
 */
void skip_class(int (*classifier)(int), char **cursor)
{
    while (classifier(**cursor))
        (*cursor)++;
}

/**
 * @brief Get the word object
 *
 * @param s
 * @return char*
 */
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
            if (*s == ';' || *s == '\n' || *s == '>')
            {
                is_command = 0;
            }
        }
        else
        {
            tk = token_init(mj);
            int token = token_get(word);
            tk->word = token;
            if (token == WORD_REDIR)
            {
                tk->redirection = init_redirection(mj);
                char *file = get_word(&s);
                set_redirection(mj, tk->redirection, word, file);
            }
            else if (token == WORD_COMMAND)
            {
                is_command = 1;
                struct list *tmp = list_append(mj, tk->data, word);
                tk->data = tmp;
            }

            lexer_append(mj, lex, tk);
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