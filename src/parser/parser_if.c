#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "parser.h"

static int should_loop(enum words w)
{
    if (w == WORD_ELIF)
        return 0;
    if (w == WORD_FI)
        return 0;
    if (w == WORD_ELSE)
        return 0;
    if (w == WORD_THEN)
        return 0;
    return 1;
}

static void parser_else(struct major *mj, struct ast *ast)
{
    struct token *expr = NULL;

    while ((expr = get_next_token(mj))->word != WORD_FI)
    {
        if (expr->word == WORD_EOF)
            my_err(2, mj, "parser_if: unexpected EOF");
        ast->middle = take_action(mj, NULL, expr);
    }

    token_free(expr);
}

struct ast *parser_if(struct major *mj, struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }
    struct token *cond = get_next_token(mj);
    struct ast *newast = create_ast(mj, tk);
    newast->left = get_ast(mj, newast->left, &cond);
    struct token *then = cond;
    struct token *expr = NULL;
    parser_cpdlist(mj, &expr, newast, should_loop);

    if (then->word != WORD_THEN)
    {
        token_free(then);
        my_err(2, mj, "parser_if: syntax error");
    }

    token_free(then);

    if (expr->word == WORD_ELSE)
        parser_else(mj, newast);
    if (expr->word == WORD_ELIF)
        newast->middle = parser_if(mj, newast->middle, token_cpy(mj, tk));
    token_free(expr);
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}