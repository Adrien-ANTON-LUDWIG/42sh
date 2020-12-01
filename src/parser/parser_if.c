#include "ast.h"
#include "parser.h"
#include "tokens.h"

/**
 * @brief Predicate used by parser_if() to know if this is still a compound
 * list.
 *
 * @param w
 * @return int
 */
static int should_loop(enum words w)
{
    if (w == WORD_ELIF)
        return 0;
    if (w == WORD_FI)
        return 0;
    if (w == WORD_ELSE)
        return 0;
    return 1;
}

/**
 * @brief Handles the parsing of an "else" condition
 *
 * @param mj major structure
 * @param ast
 * @return struct ast*
 */
static void parser_else(struct major *mj, struct ast *ast)
{
    struct token *expr = NULL;

    while ((expr = get_next_token(mj))->word != WORD_FI)
    {
        if (expr->word == WORD_EOF)
            my_err(1, mj, "parser_if: unexpected EOF");
        ast->middle = take_action(mj, NULL, expr);
    }

    token_free(expr);
}

/**
 * @brief Handles the parsing of an "if" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_if(struct major *mj, struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }
    struct token *cond = get_next_token(mj);
    struct token *then = get_next_token(mj);
    struct token *expr = NULL;
    struct ast *newast = create_ast(mj, tk);
    newast->left = take_action(mj, newast->left, cond);
    while (should_loop((expr = get_next_token(mj))->word))
    {
        if (expr->word == WORD_EOF)
            my_err(1, mj, "parser_if: unexpected EOF");
        newast->right = take_action(mj, newast->right, expr);
    }
    if (then->word != WORD_THEN)
    {
        token_free(then);
        my_err(1, mj, "parser_if: syntax error");
    }

    token_free(then);

    if (expr->word == WORD_ELSE)
        parser_else(mj, newast);
    if (expr->word == WORD_ELIF)
        newast->middle = parser_if(mj, newast->middle, token_cpy(mj, tk));
    token_free(expr);
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}