#include "ast.h"
#include "my_err.h"
#include "parser.h"

void parser_cpdlist(struct major *mj, struct token **expr, struct ast *newast,
                    int (*should_loop)(enum words))
{
    while (should_loop((*expr = get_next_token(mj))->word))
    {
        if ((*expr)->word == WORD_EOF)
            my_err(2, mj, "parser cpd: unexpected EOF");
        if ((*expr)->data)
            newast->right = take_action(mj, newast->right, *expr);
        else
            token_free(*expr);
    }
}