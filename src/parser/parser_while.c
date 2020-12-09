#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "parser.h"

static int should_loop(enum words w)
{
    if (w == WORD_DO)
        return 0;
    if (w == WORD_DONE)
        return 0;
    return 1;
}

struct ast *parser_while(struct major *mj, struct ast *ast, struct token **tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }
    struct token *cond = get_next_token(mj);
    struct ast *newast = create_ast(mj, *tk);
    newast->left = get_ast(mj, ast, &cond);
    struct token *t_do = cond;
    struct token *expr = cond;
    parser_cpdlist(mj, &expr, newast, should_loop);

    if (t_do->word != WORD_DO)
    {
        token_free(t_do);
        my_err(2, mj, "parser_while: syntax error");
    }

    token_free(t_do);
    token_free(expr);
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}