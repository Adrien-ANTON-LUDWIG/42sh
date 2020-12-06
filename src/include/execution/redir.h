#ifndef REDIR_H
#define REDIR_H

#include "ast.h"
#include "structures.h"

/**
 * @brief Execute an ast which root is a WORD_REDIR
 *
 * @param mj
 * @param ast
 * @return int
 */
int exec_redir(struct major *mj, struct ast *ast);

#endif