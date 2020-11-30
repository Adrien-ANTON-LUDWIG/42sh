#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "exec_ast.h"
#include "my_xmalloc.h"

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
    and->word = WORD_AND;
    struct ast *newast = create_ast(mj, and);
    newast->left = ast;
    newast->right = NULL;
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
    else
        my_err(1, mj, "parser: syntax error");
    return ast;
}

/**
 * @brief Parses and executes
 *
 * @param mj
 * @return struct ast*
 */
struct ast *parser(struct major *mj)
{
    struct token *tk = NULL;
    struct ast *ast = NULL;
    while ((tk = lexer_build(mj))->word != WORD_EOF)
    {
        ast = take_action(mj, ast, tk);
        exec_ast(mj, ast);
        ast_free(ast);
        ast = NULL;
    }
    token_free(tk);
    return ast;
}