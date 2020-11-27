#include "parser.h"

#include <stdlib.h>

#include "ast.h"

static struct ast *create_ast(struct token *tk)
{
    if (!tk)
        return NULL;
    struct ast *ast = calloc(1, sizeof(struct ast));
    ast->data = tk;
    return ast;
}

struct ast *add_single_command(struct ast *ast, struct token *tk)
{
    if (!ast)
    {
        ast = create_ast(tk);
        return ast;
    }
    struct token *and = calloc(1, sizeof(struct token));
    and->word = WORD_AND;
    struct ast *newast = create_ast(and);
    newast->left = ast;
    newast->right = create_ast(tk);
    return newast;
}

static struct ast *parser_if(struct major *mj, struct lexer *lex,
                             struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(ast, ast->data);
        ast = newast;
    }
    struct token *cond = lexer_pop_head(mj, lex);
    struct token *then = lexer_pop_head(mj, lex);
    struct token *expr = lexer_pop_head(mj, lex);
    struct ast *newast = create_ast(tk);
    newast->left = create_ast(cond);
    if (expr->word != WORD_COMMAND)
        newast->right = take_action(mj, newast->right, lex, expr);
    else
        newast->right = create_ast(expr);
    struct token *fi = lexer_pop_head(mj, lex);
    if (then->word != WORD_THEN || fi->word != WORD_FI)
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
        ast = add_single_command(ast, tk);
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