#ifndef AST_H
#define AST_H

#include "structures.h"
#include "tokens.h"

struct ast *ast_add(struct major *mj, struct ast *left, struct ast *right);
struct ast *ast_init(struct major *mj, struct token *tk, struct ast *left,
                     struct ast *right);
struct ast *ast_init_with_tokens(struct major *mj, struct token *tk,
                                 struct token *left, struct token *right);
void ast_free(struct ast *ast);
void ast_printer(struct ast *ast);

#endif /* AST_H */