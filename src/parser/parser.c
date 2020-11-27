#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "my_xmalloc.h"

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
    while ((expr = lexer_pop_head(mj, lex))->word != WORD_FI)
    {
        if (expr->word == WORD_EOF)
            my_err(1, mj, "parser_if: unexpected EOF");
        newast->left = create_ast(mj, cond);
        if (expr->word != WORD_COMMAND)
            newast->right = take_action(mj, newast->right, lex, expr);
        else
            newast->right = create_ast(mj, expr);
    }
    if (then->word != WORD_THEN)
        my_err(1, mj, "parser_if: syntax error");
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}

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

struct ast *parser(struct major *mj, struct lexer *lex)
{
    struct token *tk = NULL;
    struct ast *ast = NULL;
    while ((tk = lexer_pop_head(mj, lex))->word != WORD_EOF)
    {
        ast = take_action(mj, ast, lex, tk);
    }
    return ast;
}