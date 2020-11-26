#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#define TOKENS_TO_STRING                                                       \
    {                                                                          \
        "WORD_IF", "WORD_THEN", "WORD_FI", "WORD_COMMAND", "WORD_EOF"          \
    }

struct token *token_init(struct major *major)
{
    struct token *new = malloc(sizeof(struct token));

    if (!new)
        my_err(1, major, "token_init: malloc failed");

    new->word = 0;
    new->data = NULL;

    return new;
}

int token_get(char *s)
{
    char *tokens_strings[] = TOKENS_STRINGS;

    for (size_t i = 0; i < WORD_COMMAND; i++)
        if (!strcmp(s, tokens_strings[i]))
            return i;

    return WORD_COMMAND;
}

char *token2string(struct token *tk)
{
    char *tokens_strings[] = TOKENS_TO_STRING;
    return tokens_strings[tk->word];
}