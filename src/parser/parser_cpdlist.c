#include "ast.h"
#include "my_err.h"
#include "parser.h"

void parser_cpdlist(struct major *mj, struct token **expr, struct ast *newast,
                    int (*should_loop)(enum words))
{
    if ((*expr)->word == WORD_NEWLINE)
        *expr = token_renew(mj, *expr, 0);
    do
    {
        if ((*expr)->word == WORD_EOF)
            my_err(2, mj, "parser cpd: unexpected EOF");
        if ((*expr)->data)
            newast->right = get_ast(mj, newast->right, expr);
        else
        {
            token_free(*expr);
            *expr = get_next_token(mj);
        }
    } while (should_loop((*expr)->word));
}