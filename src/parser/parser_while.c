#include "ast.h"
#include "parser.h"
#include "tokens.h"

/**
 * @brief Predicate used by parser_while() to know if this is still a compound
 * list.
 *
 * @param w
 * @return int
 */
static int should_loop(enum words w)
{
    if (w == WORD_DO)
        return 0;
    if (w == WORD_DONE)
        return 0;
    return 1;
}

/**
 * @brief Handles the parsing of an "while" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_while(struct major *mj, struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }
    struct token *cond = get_next_token(mj);
    struct token *t_do = get_next_token(mj);
    struct token *expr = NULL;
    struct ast *newast = create_ast(mj, tk);
    newast->left = take_action(mj, newast->left, cond);
    parser_cpdlist(mj, &expr, newast, should_loop);

    if (t_do->word != WORD_DO)
    {
        token_free(t_do);
        my_err(2, mj, "parser_if: syntax error");
    }

    token_free(t_do);
    token_free(expr);
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}