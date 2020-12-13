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
    if ((*expr)->word == WORD_NEWLINE)
        *expr = token_renew(mj, *expr, 0);
    do
    {
        if ((*expr)->word == WORD_EOF)
            my_err(2, mj, "parser_else: unexpected EOF");
        if ((*expr)->data)
            ast->middle = get_ast(mj, ast->middle, expr);
        else
            *expr = token_renew(mj, *expr, 0);
    } while (should_loop((*expr)->word));
}

static void parser_end_if(struct major *mj, struct token **tk,
                          struct ast *newast)
{
    if ((*tk)->word == WORD_ELSE)
    {
        *tk = token_renew(mj, *tk, 1);
        parser_else(mj, newast, tk);
    }
    else if ((*tk)->word == WORD_ELIF)
    {
        (*tk)->word = WORD_IF;
        newast->middle = parser_if(mj, newast->middle, tk);
    }

    if ((*tk)->word == WORD_FI) // Was already done by elif if there was one
        *tk = token_renew(mj, *tk, 0);
}

struct ast *parser_if(struct major *mj, struct ast *ast, struct token **tk)
{
    superand_creator(mj, &ast);

    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);

    parser_cpdlist(mj, tk, newast, should_loop);
    newast->left = newast->right;
    newast->right = NULL;

    if ((*tk)->word != WORD_THEN)
        my_err(2, mj, "parser_if: expected 'then'");

    *tk = token_renew(mj, *tk, 1);
    parser_cpdlist(mj, tk, newast, should_loop);

    parser_end_if(mj, tk, newast);

    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}