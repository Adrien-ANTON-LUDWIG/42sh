#define _POSIX_C_SOURCE 200809L
#include "tokens.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "my_err.h"
#include "my_xmalloc.h"

struct token *token_init(struct major *mj, enum words word)
{
    struct token *new = my_xcalloc(mj, 1, sizeof(struct token));

    new->word = word;

    return new;
}

struct token *token_cpy(struct major *mj, struct token *src)
{
    struct token *new = token_init(mj, src->word);

    if (!src->data)
        return new;

    for (struct list_item *l = src->data->head; l; l = l->next)
    {
        char *str = strdup(l->data);
        new->data = list_append(mj, new->data, str);
    }

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