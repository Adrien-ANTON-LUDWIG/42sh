#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "major.h"
#include "my_xmalloc.h"
#include "parser.h"
#include "printer.h"
#include "tokens.h"

struct ast *create_ast(struct major *mj, struct token *tk)
{
    struct ast *ast = my_xcalloc(mj, 1, sizeof(struct ast));
    ast->data = tk;
    return ast;
}

void ast_free(struct ast *ast)
{
    if (!ast)
        return;
    token_free(ast->data);
    ast_free(ast->left);
    ast_free(ast->middle);
    ast_free(ast->right);
    free(ast);
}