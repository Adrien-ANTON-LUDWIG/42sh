#define _POSIX_C_SOURCE 200809L
#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#include "my_utils.h"
#include "my_xmalloc.h"

#define TOKENS_TO_STRING                                                       \
    {                                                                          \
        "WORD_IF", "WORD_THEN", "WORD_ELIF", "WORD_ELSE", "WORD_FI",           \
            "WORD_WHILE", "WORD_UNTIL", "WORD_FOR", "WORD_IN", "WORD_DO",      \
            "WORD_DONE", "WORD_AND", "WORD_OR", "WORD_PIPE", "WORD_REDIR_LR",  \
            "WORD_REDIR_RP", "WORD_REDIR_LA", "WORD_REDIR_RA",                 \
            "WORD_REDIR_LLM", "WORD_REDIR_LL", "WORD_REDIR_RR",                \
            "WORD_REDIR_L", "WORD_REDIR_R", "WORD_COMMAND", "WORD_EOF",        \
            "WORD_SUPERAND"                                                    \
    }

struct token *token_init(struct major *mj)
{
    struct token *new = my_xmalloc(mj, sizeof(struct token));

    new->word = 0;
    new->data = NULL;

    return new;
}

struct token *token_cpy(struct major *mj, struct token *src)
{
    if (src->word == WORD_COMMAND)
        my_err(1, mj,
               "token_cpy: stop right there! this was node made for that");

    struct token *new = token_init(mj);

    new->word = src->word;
    return new;
}

int word_type(struct major *mj, struct token *tk, char *s)
{
    char *tokens_strings[] = TOKENS_STRINGS;

    for (size_t i = 0; i < WORD_REDIR_LR; i++)
        if (!strcmp(s, tokens_strings[i]))
            return i;

    char *tokens_strings_redir[] = TOKENS_STRINGS_REDIR;
    char *defaut_value[] = DEFAULT_REDIR_VALUE;

    char c = (*s >= '0' && *s <= '9') ? *s : 0;

    char *temp = s;
    if (c)
        temp++;

    for (size_t i = 0; i < sizeof(tokens_strings_redir) / sizeof(char *); i++)
        if (!strcmp(temp, tokens_strings_redir[i]))
        {
            char *number = strndup( (c) ? &c : defaut_value[i], 1);
            tk->data = list_append(mj, tk->data, number);
            return i + WORD_REDIR_LR;
        }

    return WORD_COMMAND;
}

char *token2string(struct token *tk)
{
    char *tokens_strings[] = TOKENS_TO_STRING;
    return tokens_strings[tk->word];
}

void token_free(struct token *tk)
{
    if (!tk)
        return;

    list_free(tk->data);
    free(tk);
}