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

static void parser_else(struct major *mj, struct ast *ast, struct token **expr)
{
    do
    {
        if ((*expr)->word == WORD_EOF)
            my_err(2, mj, "parser_else: unexpected EOF");
        if ((*expr)->data)
            ast->middle = get_ast(mj, ast->middle, expr);
        else
        {
            token_free(*expr);
            *expr = get_next_token(mj);
        }
    } while (should_loop((*expr)->word));
}

static void parser_end_if(struct major *mj, struct token **tk,
                          struct token *expr, struct ast *newast)
{
    if (expr->word == WORD_ELSE)
    {
        token_free(expr);
        expr = get_next_token(mj);
        parser_else(mj, newast, &expr);
    }
    else if (expr->word == WORD_ELIF)
    {
        token_free(expr);
        expr = token_cpy(mj, *tk);
        newast->middle = parser_if(mj, newast->middle, &expr);
    }

    *tk = expr;

    if (expr->word == WORD_FI) // Was already done by elif if there was one
    {
        token_free(expr);
        *tk = get_next_token(mj);
    }
}

struct ast *parser_if(struct major *mj, struct ast *ast, struct token **tk)
{
    superand_creator(mj, &ast);

    struct token *cond = get_next_token(mj);
    struct ast *newast = create_ast(mj, *tk);
    newast->left = get_ast(mj, newast->left, &cond);
    struct token *then = cond;
    struct token *expr = get_next_token(mj);
    parser_cpdlist(mj, &expr, newast, should_loop);

    if (then->word != WORD_THEN)
    {
        token_free(then);
        my_err(2, mj, "parser_if: syntax error");
    }

    token_free(then);

    parser_end_if(mj, tk, expr, newast);

    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}