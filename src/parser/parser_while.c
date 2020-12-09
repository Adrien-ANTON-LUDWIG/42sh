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

    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);
    parser_cpdlist(mj, tk, newast, should_loop);
    newast->left = newast->right;
    newast->right = NULL;

    if ((*tk)->word != WORD_DO)
        my_err(2, mj, "parser_while: expected 'do'");

    *tk = token_renew(mj, *tk, 1);

    parser_cpdlist(mj, tk, newast, should_loop);
    token_free(*tk);
    *tk = get_next_token(mj);

    if (ast)
        ast->right = newast;
    else
        return newast;
    return ast;
}