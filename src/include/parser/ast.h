#ifndef AST_H
#define AST_H

#include "structures.h"
#include "tokens.h"

struct ast
{
    struct ast *left;
    struct ast *middle;
    struct ast *right;
    struct token *data;
};

struct ast *create_ast(struct major *mj, struct token *tk);
void ast_free(struct ast *ast);
void ast_printer(struct ast *ast);

#endif /* AST_H */