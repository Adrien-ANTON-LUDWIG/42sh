#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "exec_ast.h"
#include "my_xmalloc.h"

int is_operator(struct token *tk)
{
    switch (tk->word)
    {
    case WORD_AND:
        return 1;
    case WORD_OR:
        return 1;
    default:
        return 0;
    }
    return 0;
}

/**
 * @brief Adds a AND command with the ast as its left son and tk as its right
 * son
 *
 * @param mj major structure
 * @param ast an ast
 * @param tk
 * @return struct ast* The newly built ast
 */
struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token *tk)
{
    if (!ast)
    {
        ast = create_ast(mj, tk);
        return ast;
    }
    struct token *and = my_xcalloc(mj, 1, sizeof(struct token));
    and->word = WORD_SUPERAND;
    struct ast *newast = create_ast(mj, and);
    newast->left = ast;
    if (tk)
        newast->right = create_ast(mj, tk);
    return newast;
}

/**
 * @brief Decides which function to call depending on the kind of token
 *
 * @param mj
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *take_action(struct major *mj, struct ast *ast, struct token *tk)
{
    if (tk->word == WORD_IF)
        ast = parser_if(mj, ast, tk);
    else if (tk->word == WORD_COMMAND)
        ast = add_single_command(mj, ast, tk);
    else if (tk->word == WORD_REDIR)
        token_free(tk);
    else if (tk->word == WORD_WHILE || tk->word == WORD_UNTIL)
        ast = parser_while(mj, ast, tk);
    else if (tk->word == WORD_FOR)
        ast = parser_for(mj, ast, tk);
    else
        my_err(1, mj, "parser: syntax error");
    return ast;
}

struct ast *get_ast(struct major *mj, struct ast *ast, struct token **tk)
{
    ast = take_action(mj, ast, *tk);
    struct token *pending = get_next_token(mj);
    while (is_operator(pending))
    {
        ast = parser_operator(mj, ast, pending);
        pending = get_next_token(mj);
    }
    *tk = pending;
    return ast;
}

/**
 * @brief Parses and executes
 *
 * @param mj
 * @return struct ast*
 */
void parser(struct major *mj)
{
    struct token *tk = get_next_token(mj);
    struct ast *ast = NULL;
    while (tk->word != WORD_EOF)
    {
        ast = get_ast(mj, ast, &tk);
        exec_ast(mj, ast);
        ast_free(ast);
        ast = NULL;
    }
    token_free(tk);
}