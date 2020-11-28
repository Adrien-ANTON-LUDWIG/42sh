#ifndef EXEC_AST_H
#define EXEC_AST_H

/**
 * @brief Exectutes an AST.
 *
 * @param mj
 * @param ast The ast to execute
 * @return int
 */
int exec_ast(struct major *mj, struct ast *ast);

#endif