#include "ast.h"
#include "major.h"
#include "my_err.h"
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
    newast->right = take_action(mj, newast->right, tk);
    return newast;
}

struct ast *parser_redir(struct major *mj, struct token **tk)
{
    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);

    if ((*tk)->word != WORD_WORD)
        my_err(2, mj, "parser_redir: filename needed after redirection");

    newast->left = create_ast(mj, *tk);

    *tk = get_next_token(mj);
    return newast;
}