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
    newast->right = create_ast(mj, tk);
    return newast;
}

/**
 * @brief Handles the parsing of an "if" condition
 *
 * @param mj major structure
 * @param lex the lexer
 * @param ast
 * @param tk
 * @return struct ast*
 */
static struct ast *parser_if(struct major *mj, struct lexer *lex,
                             struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, ast->data);
        ast = newast;
    }
    struct token *cond = lexer_pop_head(mj, lex);
    struct token *then = lexer_pop_head(mj, lex);
    struct token *expr = NULL;
    struct ast *newast = create_ast(mj, tk);
    newast->left = take_action(mj, newast->left, lex, cond);
    while ((expr = lexer_pop_head(mj, lex))->word != WORD_FI)
    {
        if (expr->word == WORD_EOF)
            my_err(1, mj, "parser_if: unexpected EOF");
        newast->right = take_action(mj, newast->right, lex, expr);
    }
    if (then->word != WORD_THEN)
        my_err(1, mj, "parser_if: syntax error");
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}

/**
 * @brief Decides which function to call depending on the kind of token
 *
 * @param mj
 * @param ast
 * @param lex
 * @param tk
 * @return struct ast*
 */
struct ast *take_action(struct major *mj, struct ast *ast, struct lexer *lex,
                        struct token *tk)
{
    if (tk->word == WORD_IF)
        ast = parser_if(mj, lex, ast, tk);
    else if (tk->word == WORD_COMMAND)
        ast = add_single_command(mj, ast, tk);
    else
        my_err(1, mj, "parser: syntax error");
    return ast;
}

/**
 * @brief Parses and executes
 *
 * @param mj
 * @param lex
 * @return struct ast*
 */
struct ast *parser(struct major *mj, struct lexer *lex)
{
    struct token *tk = NULL;
    struct ast *ast = NULL;
    while ((tk = lexer_pop_head(mj, lex))->word != WORD_EOF)
    {
        ast = take_action(mj, ast, lex, tk);
        exec_ast(mj, ast);
        ast_free(ast);
        ast = NULL;
    }
    return ast;
}