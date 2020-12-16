#ifndef EXEC_AST_H
#define EXEC_AST_H

#include "ast.h"
#include "major.h"

/**
 * @brief Exectutes an AST.
 *
 * @param mj
 * @param ast The ast to execute
 * @return int
 */
int exec_ast(struct major *mj, struct ast *ast);
int exec_for(struct major *mj, struct ast *ast);

#endif