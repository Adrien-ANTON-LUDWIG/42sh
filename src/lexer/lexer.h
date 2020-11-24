#ifndef LEXER_H
#define LEXER_H

#include "../utils/my_utils.h"
#include "tokens/tokens.h"

struct token_list
{
    struct token *tk;
    struct token_list *next;
    struct token_list *prev;
};

struct lexer
{
    struct token_list *head;
    struct token_list *tail;
};

#endif /* LEXER_H */