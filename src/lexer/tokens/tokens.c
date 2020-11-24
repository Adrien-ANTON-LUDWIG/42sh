#include "tokens.h"

#include <stdlib.h>

struct token *token_init(struct major *major)
{
    struct token *new = malloc(sizeof(struct token));

    if (!new)
        my_err(1, major, "token_init: malloc failed");

    new->tk = 0;
    new->data.op = 0;

    return new;
}