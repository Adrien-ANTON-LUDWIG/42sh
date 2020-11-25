#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <string.h>

#include "my_utils.h"
#include "tokens.h"

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

struct lexer *lexer_init(struct major *mj);
void lexer_append(struct major *mj, struct lexer *lex, struct token *tk);
struct token *lexer_pop_head(struct major *mj, struct lexer *lex);
void lexer_free(struct lexer *lex);

#endif /* LEXER_H */