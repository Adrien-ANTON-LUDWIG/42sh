#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "parser.h"

static int should_loop(enum words w)
{
    return w != WORD_DONE;
}

static struct token *token_renew(struct major *mj, struct token *tk,
                                 int skipline)
{
    token_free(tk);
    tk = get_next_token(mj);
    if (skipline && (tk->word == WORD_NEWLINE || tk->word == WORD_SEMIC))
        return token_renew(mj, tk, 0);
    return tk;
}

// 26 Lignes !
struct ast *parser_for(struct major *mj, struct ast *ast, struct token **tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }

    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);
    newast->left = create_ast(mj, *tk);

    *tk = get_next_token(mj);
    if ((*tk)->word == WORD_NEWLINE)
        *tk = token_renew(mj, *tk, 1);
    if ((*tk)->word == WORD_IN)
    {
        *tk = token_renew(mj, *tk, 0);
        newast->middle =
            build_command(mj, tk, get_next_token(mj), newast->middle);
        *tk = token_renew(mj, *tk, 1);
    }
    else if ((*tk)->word == WORD_SEMIC)
        *tk = token_renew(mj, *tk, 1);
    if ((*tk)->word != WORD_DO)
        my_err(2, mj, "parser_for: expected ';' or '\\n'");

    *tk = token_renew(mj, *tk, 1);
    parser_cpdlist(mj, tk, newast, should_loop);

    *tk = token_renew(mj, *tk, 1);
    if (ast)
        ast->right = ast;
    else
        ast = newast;
    return ast;
}