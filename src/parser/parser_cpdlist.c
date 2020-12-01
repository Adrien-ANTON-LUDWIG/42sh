#include "ast.h"
#include "parser.h"
#include "tokens.h"

void parser_cpdlist(struct major *mj, struct token **expr, struct ast *newast,
                    int (*should_loop)(enum words))
{
    while (should_loop((*expr = get_next_token(mj))->word))
    {
        if ((*expr)->word == WORD_EOF)
            my_err(1, mj, "parser_if: unexpected EOF");
        newast->right = take_action(mj, newast->right, *expr);
    }
}