#ifndef AST_H
#define AST_H

#include "major.h"
#include "tokens.h"

struct ast
{
    struct ast *left;
    struct ast *middle;
    struct ast *right;
    struct token *data;
};

/**
 * @brief Creates a new ast and puts tk as it's data
 *
 * @param mj
 * @param tk
 * @return struct ast*
 */
struct ast *create_ast(struct major *mj, struct token *tk);

/**
 * @brief Frees the ast
 *
 * @param ast
 */
void ast_free(struct ast *ast);

/**
 * @brief Print the ast's infix traversal
 *
 * @param ast
 */
void ast_printer(struct ast *ast);

#endif /* AST_H */