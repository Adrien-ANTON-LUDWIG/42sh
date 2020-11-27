#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "structures.h"
#include "tokens.h"

struct ast
{
    struct ast *left;
    struct ast *right;
    struct token *data;
};

struct ast *parser(struct major *mj, struct lexer *lex);
struct ast *take_action(struct major *mj, struct ast *ast, struct lexer *lex,
                        struct token *tk);

#endif /* PARSER_H */