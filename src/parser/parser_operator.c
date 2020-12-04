#include "ast.h"
#include "parser.h"
#include "structures.h"

struct ast *parser_operator(struct major *mj, struct ast *ast, struct token *tk)
{
    mj->pending = NULL;
    struct ast *newast = create_ast(mj, tk);
    newast->left = ast;
    newast->right = take_action(mj, newast->right, get_next_token(mj));
    return newast;
}