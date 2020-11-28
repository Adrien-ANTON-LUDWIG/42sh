#include <stdio.h>
#include <stdlib.h>

#include "my_xmalloc.h"
#include "parser.h"
#include "printer.h"
#include "structures.h"
#include "tokens.h"

/**
 * @brief Creates a new ast and puts tk as it's data
 *
 * @param mj
 * @param tk
 * @return struct ast*
 */
struct ast *create_ast(struct major *mj, struct token *tk)
{
    if (!tk)
        return NULL;
    struct ast *ast = my_xcalloc(mj, 1, sizeof(struct ast));
    ast->data = tk;
    return ast;
}

/**
 * @brief Print the ast's infix traversal
 *
 * @param ast
 */
void ast_printer(struct ast *ast)
{
    if (!ast)
        return;
    ast_printer(ast->left);
    print_token(ast->data);
    ast_printer(ast->right);
}

/**
 * @brief Frees the ast
 *
 * @param ast
 */
void ast_free(struct ast *ast)
{
    if (!ast)
        return;
    token_free(ast->data);
    ast_free(ast->left);
    ast_free(ast->right);
    free(ast);
}