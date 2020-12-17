#define _POSIX_C_SOURCE 200809L
#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "my_err.h"
#include "my_xmalloc.h"

#define TOKENS_TO_STRING                                                       \
    {                                                                          \
        "WORD_IF", "WORD_THEN", "WORD_ELIF", "WORD_ELSE", "WORD_FI",           \
            "WORD_WHILE", "WORD_UNTIL", "WORD_FOR", "WORD_IN", "WORD_DO",      \
            "WORD_DONE", "WORD_FUNCTION", "WORD_LBRACKET", "WORD_RBRACKET",    \
            "WORD_AND", "WORD_OR", "WORD_PIPE", "WORD_REDIR_LR",               \
            "WORD_REDIR_RP", "WORD_REDIR_LA", "WORD_REDIR_RA",                 \
            "WORD_REDIR_LLM", "WORD_REDIR_LL", "WORD_REDIR_RR",                \
            "WORD_REDIR_L", "WORD_REDIR_R", "WORD_COMMAND", "WORD_EOF",        \
            "WORD_SUPERAND", "WORD_SEMIC", "WORD_NEWLINE",                     \
            "WORD_LPARENTHESIS", "WORD_RPARENTHESIS", "WORD_DPARENTHESIS",     \
            "WORD_WORD", "WORD_ASSIGNMENT", "WORD_DSEMIC"                      \
    }

struct token *token_init(struct major *mj, enum words word)
{
    struct token *new = my_xcalloc(mj, 1, sizeof(struct token));

    new->word = word;

    return new;
}

struct token *token_cpy(struct major *mj, struct token *src)
{
    if (src->word == WORD_COMMAND)
        my_err(1, mj,
               "token_cpy: stop right there! this was not made for that");

    struct token *new = token_init(mj, src->word);

    return new;
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