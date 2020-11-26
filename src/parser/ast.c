#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "printer.h"
#include "structures.h"
#include "tokens.h"

struct ast *ast_init(struct major *mj, struct token *tk, struct ast *left,
                     struct ast *right)
{
    struct ast *ast = malloc(sizeof(struct ast));

    if (!ast)
        my_err(1, mj, "ast_init: malloc failed");

    ast->data = tk;
    ast->left = left;
    ast->right = right;

    return ast;
}

struct ast *ast_init_with_tokens(struct major *mj, struct token *tk,
                                 struct token *left, struct token *right)
{
    struct ast *ast_left = ast_init(mj, left, NULL, NULL);
    struct ast *ast_right = ast_init(mj, right, NULL, NULL);
    struct ast *ast = ast_init(mj, tk, ast_left, ast_right);

    return ast;
}

struct ast *ast_add(struct major *mj, struct ast *left, struct ast *right)
{
    if (!left)
        return right;
    struct token *tk_and = token_init(mj);
    tk_and->word = WORD_AND;

    struct ast *and = ast_init(mj, tk_and, left, right);

    return and;
}

void ast_printer(struct ast *ast)
{
    if (!ast)
        return;
    ast_printer(ast->left);
    print_token(ast->data);
    ast_printer(ast->right);
}

void ast_free(struct ast *ast)
{
    if (!ast)
        return;
    token_free(ast->data);
    ast_free(ast->left);
    ast_free(ast->right);
}