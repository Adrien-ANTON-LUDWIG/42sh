#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "parser.h"

static int should_loop(enum words w)
{
    if (w == WORD_RBRACKET)
        return 0;
    return 1;
}

struct ast *parser_bracket(struct major *mj, struct ast *ast, struct token **tk)
{
    superand_creator(mj, &ast);

    *tk = token_renew(mj, *tk, 1);
    struct ast *newast = create_ast(mj, *tk);
    parser_cpdlist(mj, tk, newast, should_loop);
    struct ast *dirty = newast->right;
    free(newast);
    newast = dirty;

    *tk = token_renew(mj, *tk, 0);

    if (!(((*tk)->word >= WORD_AND && (*tk)->word <= WORD_NEWLINE)
          || (*tk)->word == WORD_DSEMIC || (*tk)->word == WORD_EOF))
        my_err(2, mj, "parser_bracket: Expected separator");

    if (ast)
        ast->right = newast;
    else
        return newast;
    return ast;
}
