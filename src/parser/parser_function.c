#include <stdlib.h>

#include "ast.h"
#include "my_err.h"
#include "my_xmalloc.h"
#include "parser.h"
#include "tokens.h"

/**
 * @brief Predicate used by parser_function() to know if this is still a
 * compound list.
 *
 * @param w
 * @return int
 */
static int should_loop(enum words w)
{
    return w == WORD_RBRACKET;
}

void add_func_list(struct major *mj, struct ast *ast)
{
    struct funclist *head = mj->flist;
    struct funclist *new = my_xcalloc(mj, 1, sizeof(struct funclist));
    new->ast = ast;
    while (head && head->next)
        head = head->next;
    if (head)
        head->next = new;
    else
        mj->flist = new;
}

/**
 * @brief Handles the parsing of an "function" token
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_function(struct major *mj, struct ast *ast,
                            struct token **tk, struct token *tk2)
{
    if (ast)
        my_err(2, mj, "Syntax error on function declaration");
    struct token *fun_name = *tk;
    if ((*tk)->word == WORD_FUNCTION)
    {
        fun_name = tk2;
        token_free(*tk);
        struct token *parenthesis = get_next_token(mj);
        if (parenthesis->word != WORD_DPARENTHESIS)
            my_err(2, mj, "Syntax error on function declaration");
        token_free(parenthesis);
    }
    else
        token_free(tk2);
    fun_name->word = WORD_FUNCTION;
    struct ast *newast = create_ast(mj, fun_name);

    struct token *temp = get_next_token(mj);
    if (temp->word == WORD_LBRACKET)
        parser_cpdlist(mj, &temp, newast, should_loop);
    else
        ast->left = get_ast(mj, newast->left, &temp);
    add_func_list(mj, newast);
    return newast;
}
