#include "ast.h"
#include "major.h"
#include "parser.h"

int is_operator(struct token *tk)
{
    return WORD_AND <= tk->word && tk->word < WORD_COMMAND;
}

struct ast *parser_operator(struct major *mj, struct ast *ast,
                            struct token **tk)
{
    struct ast *newast = create_ast(mj, *tk);
    newast->left = ast;
    *tk = get_next_token(mj);
    newast->right = get_ast(mj, newast->right, tk);

    return newast;
}