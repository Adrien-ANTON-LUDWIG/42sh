#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#include "my_utils.h"
#include "my_xmalloc.h"
#include "redirection.h"

#define TOKENS_TO_STRING                                                       \
    {                                                                          \
        "WORD_IF", "WORD_THEN", "WORD_ELIF", "WORD_ELSE", "WORD_FI",           \
            "WORD_WHILE", "WORD_UNTIL", "WORD_FOR", "WORD_IN", "WORD_DO",      \
            "WORD_DONE", "WORD_REDIR", "WORD_COMMAND", "WORD_EOF", "WORD_AND"  \
    }

struct token *token_init(struct major *mj)
{
    struct token *new = my_xmalloc(mj, sizeof(struct token));

    new->word = 0;
    new->data = NULL;
    new->redirection = 0;

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

int word_type(char *s)
{
    char *tokens_strings[] = TOKENS_STRINGS;

    for (size_t i = 0; i < WORD_REDIR; i++)
        if (!strcmp(s, tokens_strings[i]))
            return i;

    char *tokens_strings_redir[] = TOKENS_STRINGS_REDIR;

    for (size_t i = 0; i < 2; i++)
        if (!strcmp(s, tokens_strings_redir[i]))
            return WORD_REDIR;

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
    free_redirection(tk->redirection);
    free(tk);
}