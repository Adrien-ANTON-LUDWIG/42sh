#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "parser.h"

static int should_loop(enum words w)
{
    return w != WORD_DONE;
}

// na = newast tkt c'est normal
struct ast *parser_for(struct major *mj, struct ast *ast, struct token **tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *na = add_single_command(mj, ast, NULL);
        ast = na;
    }

    struct ast *na = create_ast(mj, *tk);
    *tk = get_next_token(mj);
    na->left = create_ast(mj, *tk);

    *tk = get_next_token(mj);
    if ((*tk)->word == WORD_NEWLINE)
        *tk = token_renew(mj, *tk, 1);
    if ((*tk)->word == WORD_IN)
    {
        *tk = token_renew(mj, *tk, 0);
        na->middle = build_command(mj, tk, get_next_token(mj), na->middle);
        *tk = token_renew(mj, *tk, 1);
    }
    else if ((*tk)->word == WORD_SEMIC)
        *tk = token_renew(mj, *tk, 1);
    if ((*tk)->word != WORD_DO)
        my_err(2, mj, "parser_for: expected ';' or '\\n' or 'do'");

    *tk = token_renew(mj, *tk, 1);
    parser_cpdlist(mj, tk, na, should_loop);

    *tk = token_renew(mj, *tk, 1);
    if (ast)
        ast->right = ast;
    else
        ast = na;
    return ast;
}