#include <stdio.h>
#include <stdlib.h>

#include "my_xmalloc.h"
#include "parser.h"
#include "printer.h"
#include "structures.h"
#include "tokens.h"

struct ast *create_ast(struct major *mj, struct token *tk)
{
    if (!tk)
        return NULL;
    struct ast *ast = my_xcalloc(mj, 1, sizeof(struct ast));
    ast->data = tk;
    return ast;
}

void ast_printer(struct ast *ast)
{
    if (!ast)
        return;
    ast_printer(ast->left);
    print_token(ast->data);
    ast_printer(ast->right);
}

void ast_free(struct ast *ast)
{
    if (!ast)
        return;
    token_free(ast->data);
    ast_free(ast->left);
    ast_free(ast->right);
}